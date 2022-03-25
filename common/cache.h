/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ISLANDS_COMMON_CACHE_H
#define ISLANDS_COMMON_CACHE_H

#include <cstdint>
#include <functional>
#include <limits>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "common/device.h"

namespace e8 {

/**
 * @brief The DeviceCache class A generic cache for objects living in high speed memory (e.g. video
 * memory). It generally uses the LRU ejection policy, but it's possible to reserve already uploaded
 * objects to prevent them from being ejected from the cache.
 */
template <typename LowSpeedReference, typename HighSpeedObject> class DeviceCache {
  public:
    // A functor to return the size of the object when uploaded to high-speed device.
    using ObjectSizeFn = std::function<uint64_t(LowSpeedReference low_speed_ref)>;

    // A functor to upload the specified host object to the GPU.
    using UploadFn =
        std::function<void(LowSpeedReference low_speed_ref, uint64_t old_object_size,
                           uint64_t new_object_size, HighSpeedObject *high_speed_object)>;

    DeviceCache(MemoryUsageTracker *usage_tracker);
    DeviceCache(DeviceCache const &) = delete;
    DeviceCache(DeviceCache &&) = delete;
    ~DeviceCache();

    /**
     * @brief Reset Clears the upload list.
     */
    void Reset();

    /**
     * @brief Upload Uploads the object from low speed memory to high speed cache memory if it
     * hasn't already been in cache. If it has, it will try to fetch the object directly from cache
     * if the caller doesn't request it to be overriden. After this call, the low speed reference is
     * put in an upload list. This function will disregard any override request if the reference is
     * in the upload list.
     *
     * @param low_speed_ref The host object to be uploaded/searched from cache.
     * @param override_old_upload If it's set to true and the low speed reference has not been put
     * in the upload list, the function will upload/update the object regardless it's in the cache
     * or not.
     * @param object_size_fn Computes the size of video memory the object requires.
     * @param upload_fn The upload procedure.
     * @return Returns the uploaded object if it fits in the video memory.
     */
    HighSpeedObject *Upload(LowSpeedReference low_speed_ref, bool override_old_upload,
                            ObjectSizeFn const &object_size_fn, UploadFn const &upload_fn);

    /**
     * @brief Find Returns the high speed object pointed to by the low speed reference, if one
     * exists. Otherwise, it returns a nullptr. This call doesn't count towards an access, so it
     * won't modify the reference's access recency.
     */
    HighSpeedObject *Find(LowSpeedReference low_speed_ref);

  private:
    struct CachedHighSpeedObject;

    using RecencyListNode = typename std::list<LowSpeedReference>::iterator;
    using CacheMapNode =
        typename std::unordered_map<LowSpeedReference, CachedHighSpeedObject>::iterator;

    struct CachedHighSpeedObject {
        CachedHighSpeedObject(RecencyListNode recency_list_node);
        CachedHighSpeedObject(CachedHighSpeedObject const &) = delete;
        CachedHighSpeedObject(CachedHighSpeedObject &&) = default;
        ~CachedHighSpeedObject();

        bool Valid() const;

        // The object living in high speed memory.
        HighSpeedObject object;

        // The size of high speed memory the object occupies.
        uint64_t object_size;

        // It points to a node in the recency list. The position determines the access order.
        RecencyListNode recency_list_node;
    };

    /**
     * @brief Fetch Fetches the object from the cache. If it isn't present in the cache, it will
     * allocate an empty slot for it. Besides, it also pushes the object to the back of the recency
     * list to indicate it has just been used.
     */
    CacheMapNode Fetch(LowSpeedReference low_speed_ref);

    /**
     * @brief FreeUpSpaceFor Frees up space for the to_be_added object, if possible.
     *
     * @param to_be_added The object to make space for.
     * @param shortage The amount of which is in short.
     * @return Whether it's possible to free enough space for the object.
     */
    bool FreeUpSpaceFor(LowSpeedReference to_be_added, int64_t shortage);

    /**
     * @brief EnsureSpaceFor Ensures that the to_be_added object will fit in the video memory. If it
     * doesn't fit, it will try to eject the least recently used items to make space for it. If it
     * still doesn't fit, the function returns false.
     *
     * @param to_be_added The object to check space for.
     * @param old_object_size The size of the previously cached version of the object.
     * @param new_object_size The object's current size.
     * @return Whether it's possible to make space for the object.
     */
    bool EnsureSpaceFor(LowSpeedReference to_be_added, uint64_t old_object_size,
                        uint64_t new_object_size);

    // The least recently used objects are in the front of the list.
    std::list<LowSpeedReference> recency_;

    // Currently cached objects.
    std::unordered_map<LowSpeedReference, CachedHighSpeedObject> cache_;

    // The objects that has recently been uploaded and cannot be ejected, unless this cache gets
    // reset.
    std::unordered_set<LowSpeedReference> uploads_;

    // For tracking video memory usage.
    MemoryUsageTracker *usage_tracker_;
};

template <typename LowSpeedReference, typename HighSpeedObject>
DeviceCache<LowSpeedReference, HighSpeedObject>::CachedHighSpeedObject::CachedHighSpeedObject(
    RecencyListNode recency_list_node)
    : object_size(0), recency_list_node(recency_list_node) {}

template <typename LowSpeedReference, typename HighSpeedObject>
DeviceCache<LowSpeedReference, HighSpeedObject>::CachedHighSpeedObject::~CachedHighSpeedObject() {}

template <typename LowSpeedReference, typename HighSpeedObject>
bool DeviceCache<LowSpeedReference, HighSpeedObject>::CachedHighSpeedObject::Valid() const {
    return object_size > 0;
}

template <typename LowSpeedReference, typename HighSpeedObject>
typename DeviceCache<LowSpeedReference, HighSpeedObject>::CacheMapNode
DeviceCache<LowSpeedReference, HighSpeedObject>::Fetch(LowSpeedReference low_speed_ref) {
    // Appends low_speed_ref to the back of the recency list and deletes the old list node when
    // there is any.
    recency_.push_back(low_speed_ref);
    RecencyListNode tail_node = std::prev(recency_.end());

    CacheMapNode cache_node = cache_.find(low_speed_ref);
    if (cache_node == cache_.end()) {
        return cache_.insert(std::make_pair(low_speed_ref, CachedHighSpeedObject(tail_node))).first;
    }

    auto &[_, cached_high_speed_object] = *cache_node;
    recency_.erase(cached_high_speed_object.recency_list_node);
    cached_high_speed_object.recency_list_node = tail_node;

    return cache_node;
}

template <typename LowSpeedReference, typename HighSpeedObject>
bool DeviceCache<LowSpeedReference, HighSpeedObject>::FreeUpSpaceFor(LowSpeedReference to_be_added,
                                                                     int64_t shortage) {
    // Examines if it's possible to make space for the shortage by proposing to ejects non-must-keep
    // LRU objects.
    std::vector<RecencyListNode> to_be_ejected;
    int64_t reduction = 0;
    for (auto it = recency_.begin(); shortage > 0 && it != recency_.end(); ++it) {
        LowSpeedReference low_speed_ref = *it;

        if (low_speed_ref == to_be_added || uploads_.find(low_speed_ref) != uploads_.end()) {
            continue;
        }

        to_be_ejected.push_back(it);
        reduction += cache_.at(low_speed_ref).object_size;
    }

    if (shortage - reduction > 0) {
        // Can't make up the shortage even if we eject all the non-must-keep objects.
        return false;
    }

    for (auto const &list_node : to_be_ejected) {
        LowSpeedReference low_speed_ref = *list_node;
        cache_.erase(low_speed_ref);
        recency_.erase(list_node);
    }

    usage_tracker_->used -= reduction;

    return true;
}

template <typename LowSpeedReference, typename HighSpeedObject>
bool DeviceCache<LowSpeedReference, HighSpeedObject>::EnsureSpaceFor(LowSpeedReference to_be_added,
                                                                     uint64_t old_object_size,
                                                                     uint64_t new_object_size) {
    uint64_t new_total_size = usage_tracker_->used + new_object_size - old_object_size;
    if (new_total_size <= usage_tracker_->capacity) {
        return true;
    }

    uint64_t shortage = new_total_size - usage_tracker_->capacity;
    return this->FreeUpSpaceFor(to_be_added, shortage);
}

template <typename LowSpeedReference, typename HighSpeedObject>
DeviceCache<LowSpeedReference, HighSpeedObject>::DeviceCache(MemoryUsageTracker *usage_tracker)
    : usage_tracker_(usage_tracker) {}

template <typename LowSpeedReference, typename HighSpeedObject>
DeviceCache<LowSpeedReference, HighSpeedObject>::~DeviceCache() {}

template <typename LowSpeedReference, typename HighSpeedObject>
void DeviceCache<LowSpeedReference, HighSpeedObject>::Reset() {
    uploads_.clear();
}

template <typename LowSpeedReference, typename HighSpeedObject>
HighSpeedObject *DeviceCache<LowSpeedReference, HighSpeedObject>::Upload(
    LowSpeedReference low_speed_ref, bool override_old_upload, ObjectSizeFn const &object_size_fn,
    UploadFn const &upload_fn) {
    if (uploads_.find(low_speed_ref) != uploads_.end()) {
        // It has been recently uploaded. There is nothing to override.
        override_old_upload = false;
    } else {
        uploads_.insert(low_speed_ref);
    }

    auto &[_, cached_high_speed_object] = *this->Fetch(low_speed_ref);
    if (cached_high_speed_object.Valid() && !override_old_upload) {
        return &cached_high_speed_object.object;
    }

    uint64_t old_object_size = cached_high_speed_object.object_size;
    uint64_t new_object_size = object_size_fn(low_speed_ref);

    if (!this->EnsureSpaceFor(low_speed_ref, old_object_size, new_object_size)) {
        return nullptr;
    }

    upload_fn(low_speed_ref, old_object_size, new_object_size, &cached_high_speed_object.object);
    cached_high_speed_object.object_size = new_object_size;

    int64_t delta = new_object_size - old_object_size;
    usage_tracker_->used += delta;

    return &cached_high_speed_object.object;
}

template <typename LowSpeedReference, typename HighSpeedObject>
HighSpeedObject *
DeviceCache<LowSpeedReference, HighSpeedObject>::Find(LowSpeedReference low_speed_ref) {
    CacheMapNode cache_node = cache_.find(low_speed_ref);
    if (cache_node == cache_.end()) {
        return nullptr;
    }
    return &cache_node->second.object;
}

} // namespace e8

#endif // ISLANDS_COMMON_CACHE_H
