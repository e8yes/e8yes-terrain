// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: bbox.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_bbox_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_bbox_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_bbox_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_bbox_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_bbox_2eproto;
namespace e8 {
class AABB;
class AABBDefaultTypeInternal;
extern AABBDefaultTypeInternal _AABB_default_instance_;
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> ::e8::AABB* Arena::CreateMaybeMessage<::e8::AABB>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace e8 {

// ===================================================================

class AABB PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.AABB) */ {
 public:
  inline AABB() : AABB(nullptr) {};
  virtual ~AABB();

  AABB(const AABB& from);
  AABB(AABB&& from) noexcept
    : AABB() {
    *this = ::std::move(from);
  }

  inline AABB& operator=(const AABB& from) {
    CopyFrom(from);
    return *this;
  }
  inline AABB& operator=(AABB&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const AABB& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AABB* internal_default_instance() {
    return reinterpret_cast<const AABB*>(
               &_AABB_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(AABB& a, AABB& b) {
    a.Swap(&b);
  }
  inline void Swap(AABB* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(AABB* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline AABB* New() const final {
    return CreateMaybeMessage<AABB>(nullptr);
  }

  AABB* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<AABB>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const AABB& from);
  void MergeFrom(const AABB& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(AABB* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.AABB";
  }
  protected:
  explicit AABB(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_bbox_2eproto);
    return ::descriptor_table_bbox_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMinFieldNumber = 1,
    kMaxFieldNumber = 2,
  };
  // repeated float min = 1;
  int min_size() const;
  private:
  int _internal_min_size() const;
  public:
  void clear_min();
  private:
  float _internal_min(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      _internal_min() const;
  void _internal_add_min(float value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      _internal_mutable_min();
  public:
  float min(int index) const;
  void set_min(int index, float value);
  void add_min(float value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      min() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      mutable_min();

  // repeated float max = 2;
  int max_size() const;
  private:
  int _internal_max_size() const;
  public:
  void clear_max();
  private:
  float _internal_max(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      _internal_max() const;
  void _internal_add_max(float value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      _internal_mutable_max();
  public:
  float max(int index) const;
  void set_max(int index, float value);
  void add_max(float value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      max() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      mutable_max();

  // @@protoc_insertion_point(class_scope:e8.AABB)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > min_;
  mutable std::atomic<int> _min_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > max_;
  mutable std::atomic<int> _max_cached_byte_size_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_bbox_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AABB

// repeated float min = 1;
inline int AABB::_internal_min_size() const {
  return min_.size();
}
inline int AABB::min_size() const {
  return _internal_min_size();
}
inline void AABB::clear_min() {
  min_.Clear();
}
inline float AABB::_internal_min(int index) const {
  return min_.Get(index);
}
inline float AABB::min(int index) const {
  // @@protoc_insertion_point(field_get:e8.AABB.min)
  return _internal_min(index);
}
inline void AABB::set_min(int index, float value) {
  min_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.AABB.min)
}
inline void AABB::_internal_add_min(float value) {
  min_.Add(value);
}
inline void AABB::add_min(float value) {
  _internal_add_min(value);
  // @@protoc_insertion_point(field_add:e8.AABB.min)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
AABB::_internal_min() const {
  return min_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
AABB::min() const {
  // @@protoc_insertion_point(field_list:e8.AABB.min)
  return _internal_min();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
AABB::_internal_mutable_min() {
  return &min_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
AABB::mutable_min() {
  // @@protoc_insertion_point(field_mutable_list:e8.AABB.min)
  return _internal_mutable_min();
}

// repeated float max = 2;
inline int AABB::_internal_max_size() const {
  return max_.size();
}
inline int AABB::max_size() const {
  return _internal_max_size();
}
inline void AABB::clear_max() {
  max_.Clear();
}
inline float AABB::_internal_max(int index) const {
  return max_.Get(index);
}
inline float AABB::max(int index) const {
  // @@protoc_insertion_point(field_get:e8.AABB.max)
  return _internal_max(index);
}
inline void AABB::set_max(int index, float value) {
  max_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.AABB.max)
}
inline void AABB::_internal_add_max(float value) {
  max_.Add(value);
}
inline void AABB::add_max(float value) {
  _internal_add_max(value);
  // @@protoc_insertion_point(field_add:e8.AABB.max)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
AABB::_internal_max() const {
  return max_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
AABB::max() const {
  // @@protoc_insertion_point(field_list:e8.AABB.max)
  return _internal_max();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
AABB::_internal_mutable_max() {
  return &max_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
AABB::mutable_max() {
  // @@protoc_insertion_point(field_mutable_list:e8.AABB.max)
  return _internal_mutable_max();
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace e8

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_bbox_2eproto