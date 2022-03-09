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

#ifndef ISLANDS_RENDERER_VRAM_CACHE_H
#define ISLANDS_RENDERER_VRAM_CACHE_H

#include <cstdint>
#include <functional>
#include <limits>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "renderer/context.h"

namespace e8 {

/**
 * @brief The ConstrainedLruVramCache class A generic cache for objects living in the video memory.
 * It generally uses the LRU ejection policy, but it's possible to reserve certain objects to
 * prevent them from being ejected from the cache.
 */
template <typename HostObjectPtr, typename GpuObject> class ConstrainedLruVramCache {
  public:
    // A functor to return the size of the object when uploaded to the GPU.
    using ObjectSizeFn = std::function<uint64_t(HostObjectPtr host_object_ptr)>;

    // A functor to upload the specified host object to the GPU.
    using UploadFn = std::function<void(HostObjectPtr host_object_ptr, GpuObject *gpu_object)>;

    ConstrainedLruVramCache(VramUsageTracker *usage_tracker);
    ConstrainedLruVramCache(ConstrainedLruVramCache const &) = delete;
    ConstrainedLruVramCache(ConstrainedLruVramCache &&) = delete;
    ~ConstrainedLruVramCache();

    /**
     * @brief MarkMustKeep Adds/removes the specified object from the must-keep list. Objects that
     * are in the list will never be ejected from the cache.
     *
     * @param must_keep Whether or not to add the object to the must-keep list.
     */
    void MarkMustKeep(HostObjectPtr host_object_ptr, bool must_keep);

    /**
     * @brief Reset Clears the must-keep list.
     */
    void Reset();

    /**
     * @brief Upload Uploads the object from host memory to video memory if it hasn't already been
     * in cache. Otherwise, it will try to fetch the object directly from cache if it doesn't need
     * to be updated.
     *
     * @param host_object_ptr The host object to be uploaded/searched from cache.
     * @param always_upload If it's set to true, the function will upload/update the object
     * regardless it's in the cache or not.
     * @param object_size_fn Computes the size of video memory the object requires.
     * @param upload_fn The upload procedure.
     * @return Returns the uploaded object if it fits in the video memory.
     */
    GpuObject const *Upload(HostObjectPtr host_object_ptr, bool always_upload,
                            ObjectSizeFn const &object_size_fn, UploadFn const &upload_fn);

  private:
    struct CachedGpuObject;

    using RecencyListNode = typename std::list<HostObjectPtr>::iterator;
    using CacheMapNode = typename std::unordered_map<HostObjectPtr, CachedGpuObject>::iterator;

    struct CachedGpuObject {
        CachedGpuObject(RecencyListNode recency_list_node);
        CachedGpuObject(CachedGpuObject const &) = delete;
        CachedGpuObject(CachedGpuObject &&) = default;
        ~CachedGpuObject();

        bool Valid() const;

        // The object living in the video memory.
        GpuObject object;

        // The size of video memory the object occupies.
        uint64_t object_size;

        // It points to a node in the recency list. The position determines the access order.
        RecencyListNode recency_list_node;
    };

    /**
     * @brief Fetch Fetches the object from the cache. If it isn't present in the cache, it will
     * allocate an empty slot for it. Besides, it also pushes the object to the back of the recency
     * list to indicate it has just been used.
     */
    CacheMapNode Fetch(HostObjectPtr host_object_ptr);

    /**
     * @brief FreeUpSpaceFor Frees up space for the to_be_added object, if possible.
     *
     * @param to_be_added The object to make space for.
     * @param shortage The amount of which is in short.
     * @return Whether it's possible to free enough space for the object.
     */
    bool FreeUpSpaceFor(HostObjectPtr to_be_added, int64_t shortage);

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
    bool EnsureSpaceFor(HostObjectPtr to_be_added, uint64_t old_object_size,
                        uint64_t new_object_size);

    // The least recently used objects are in the front of the list.
    std::list<HostObjectPtr> recency_;

    // Currently cached objects.
    std::unordered_map<HostObjectPtr, CachedGpuObject> cache_;

    // The must-keep objects.
    std::unordered_set<HostObjectPtr> must_keeps_;

    // For tracking video memory usage.
    VramUsageTracker *usage_tracker_;
};

template <typename HostObjectPtr, typename GpuObject>
ConstrainedLruVramCache<HostObjectPtr, GpuObject>::CachedGpuObject::CachedGpuObject(
    RecencyListNode recency_list_node)
    : object_size(0), recency_list_node(recency_list_node) {}

template <typename HostObjectPtr, typename GpuObject>
ConstrainedLruVramCache<HostObjectPtr, GpuObject>::CachedGpuObject::~CachedGpuObject() {}

template <typename HostObjectPtr, typename GpuObject>
bool ConstrainedLruVramCache<HostObjectPtr, GpuObject>::CachedGpuObject::Valid() const {
    return object_size > 0;
}

template <typename HostObjectPtr, typename GpuObject>
typename ConstrainedLruVramCache<HostObjectPtr, GpuObject>::CacheMapNode
ConstrainedLruVramCache<HostObjectPtr, GpuObject>::Fetch(HostObjectPtr host_object_ptr) {
    // Appends host_object_ptr to the back of the recency list and deletes the old list node when
    // there is any.
    recency_.push_back(host_object_ptr);
    RecencyListNode tail_node = std::prev(recency_.end());

    CacheMapNode cache_node = cache_.find(host_object_ptr);
    if (cache_node == cache_.end()) {
        return cache_.insert(std::make_pair(host_object_ptr, CachedGpuObject(tail_node))).first;
    }

    auto &[_, cached_gpu_object] = *cache_node;
    recency_.erase(cached_gpu_object.recency_list_node);
    cached_gpu_object.recency_list_node = tail_node;

    return cache_node;
}

template <typename HostObjectPtr, typename GpuObject>
bool ConstrainedLruVramCache<HostObjectPtr, GpuObject>::FreeUpSpaceFor(HostObjectPtr to_be_added,
                                                                       int64_t shortage) {
    // Examines if it's possible to make space for the shortage by proposing to ejects non-must-keep
    // LRU objects.
    std::vector<RecencyListNode> to_be_ejected;
    int64_t reduction = 0;
    for (auto it = recency_.begin(); shortage > 0 && it != recency_.end(); ++it) {
        HostObjectPtr host_object_ptr = *it;

        if (host_object_ptr == to_be_added ||
            must_keeps_.find(host_object_ptr) != must_keeps_.end()) {
            continue;
        }

        to_be_ejected.push_back(it);
        reduction += cache_.at(host_object_ptr).object_size;
    }

    if (shortage - reduction > 0) {
        // Can't make up the shortage even if we eject all the non-must-keep objects.
        return false;
    }

    for (auto const &list_node : to_be_ejected) {
        HostObjectPtr host_object_ptr = *list_node;
        cache_.erase(host_object_ptr);
        recency_.erase(list_node);
    }

    usage_tracker_->used -= reduction;

    return true;
}

template <typename HostObjectPtr, typename GpuObject>
bool ConstrainedLruVramCache<HostObjectPtr, GpuObject>::EnsureSpaceFor(HostObjectPtr to_be_added,
                                                                       uint64_t old_object_size,
                                                                       uint64_t new_object_size) {
    uint64_t new_total_size = usage_tracker_->used + new_object_size - old_object_size;
    if (new_total_size <= usage_tracker_->capacity) {
        return true;
    }

    uint64_t shortage = new_total_size - usage_tracker_->capacity;
    return this->FreeUpSpaceFor(to_be_added, shortage);
}

template <typename HostObjectPtr, typename GpuObject>
ConstrainedLruVramCache<HostObjectPtr, GpuObject>::ConstrainedLruVramCache(
    VramUsageTracker *usage_tracker)
    : usage_tracker_(usage_tracker) {}

template <typename HostObjectPtr, typename GpuObject>
ConstrainedLruVramCache<HostObjectPtr, GpuObject>::~ConstrainedLruVramCache() {}

template <typename HostObjectPtr, typename GpuObject>
void ConstrainedLruVramCache<HostObjectPtr, GpuObject>::MarkMustKeep(HostObjectPtr host_object_ptr,
                                                                     bool must_keep) {
    if (must_keep) {
        must_keeps_.insert(host_object_ptr);
    } else {
        must_keeps_.erase(host_object_ptr);
    }
}

template <typename HostObjectPtr, typename GpuObject>
void ConstrainedLruVramCache<HostObjectPtr, GpuObject>::Reset() {
    must_keeps_.clear();
}

template <typename HostObjectPtr, typename GpuObject>
GpuObject const *ConstrainedLruVramCache<HostObjectPtr, GpuObject>::Upload(
    HostObjectPtr host_object_ptr, bool always_upload, ObjectSizeFn const &object_size_fn,
    UploadFn const &upload_fn) {
    auto &[_, cached_gpu_object] = *this->Fetch(host_object_ptr);
    if (cached_gpu_object.Valid() && !always_upload) {
        return &cached_gpu_object.object;
    }

    uint64_t old_object_size = cached_gpu_object.object_size;
    uint64_t new_object_size = object_size_fn(host_object_ptr);

    if (!this->EnsureSpaceFor(host_object_ptr, old_object_size, new_object_size)) {
        return nullptr;
    }

    upload_fn(host_object_ptr, &cached_gpu_object.object);
    cached_gpu_object.object_size = new_object_size;

    int64_t delta = new_object_size - old_object_size;
    usage_tracker_->used += delta;

    return &cached_gpu_object.object;
}

} // namespace e8

#endif // ISLANDS_RENDERER_VRAM_H
