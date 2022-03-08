// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: entity.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_entity_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_entity_2eproto

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
#include "primitive.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_entity_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_entity_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_entity_2eproto;
namespace e8 {
class SceneEntityProto;
class SceneEntityProtoDefaultTypeInternal;
extern SceneEntityProtoDefaultTypeInternal _SceneEntityProto_default_instance_;
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> ::e8::SceneEntityProto* Arena::CreateMaybeMessage<::e8::SceneEntityProto>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace e8 {

// ===================================================================

class SceneEntityProto PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.SceneEntityProto) */ {
 public:
  inline SceneEntityProto() : SceneEntityProto(nullptr) {};
  virtual ~SceneEntityProto();

  SceneEntityProto(const SceneEntityProto& from);
  SceneEntityProto(SceneEntityProto&& from) noexcept
    : SceneEntityProto() {
    *this = ::std::move(from);
  }

  inline SceneEntityProto& operator=(const SceneEntityProto& from) {
    CopyFrom(from);
    return *this;
  }
  inline SceneEntityProto& operator=(SceneEntityProto&& from) noexcept {
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
  static const SceneEntityProto& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const SceneEntityProto* internal_default_instance() {
    return reinterpret_cast<const SceneEntityProto*>(
               &_SceneEntityProto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SceneEntityProto& a, SceneEntityProto& b) {
    a.Swap(&b);
  }
  inline void Swap(SceneEntityProto* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SceneEntityProto* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SceneEntityProto* New() const final {
    return CreateMaybeMessage<SceneEntityProto>(nullptr);
  }

  SceneEntityProto* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SceneEntityProto>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SceneEntityProto& from);
  void MergeFrom(const SceneEntityProto& from);
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
  void InternalSwap(SceneEntityProto* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.SceneEntityProto";
  }
  protected:
  explicit SceneEntityProto(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_entity_2eproto);
    return ::descriptor_table_entity_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kTransformFieldNumber = 4,
    kIdFieldNumber = 1,
    kNameFieldNumber = 2,
    kGeometryIdFieldNumber = 7,
    kPhysicalShapeIdFieldNumber = 8,
    kSrtTransformFieldNumber = 5,
    kBoundingBoxFieldNumber = 6,
    kMovableFieldNumber = 3,
  };
  // repeated float transform = 4;
  int transform_size() const;
  private:
  int _internal_transform_size() const;
  public:
  void clear_transform();
  private:
  float _internal_transform(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      _internal_transform() const;
  void _internal_add_transform(float value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      _internal_mutable_transform();
  public:
  float transform(int index) const;
  void set_transform(int index, float value);
  void add_transform(float value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      transform() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      mutable_transform();

  // string id = 1;
  void clear_id();
  const std::string& id() const;
  void set_id(const std::string& value);
  void set_id(std::string&& value);
  void set_id(const char* value);
  void set_id(const char* value, size_t size);
  std::string* mutable_id();
  std::string* release_id();
  void set_allocated_id(std::string* id);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_id();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_id(
      std::string* id);
  private:
  const std::string& _internal_id() const;
  void _internal_set_id(const std::string& value);
  std::string* _internal_mutable_id();
  public:

  // string name = 2;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_name();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_name(
      std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // string geometry_id = 7;
  void clear_geometry_id();
  const std::string& geometry_id() const;
  void set_geometry_id(const std::string& value);
  void set_geometry_id(std::string&& value);
  void set_geometry_id(const char* value);
  void set_geometry_id(const char* value, size_t size);
  std::string* mutable_geometry_id();
  std::string* release_geometry_id();
  void set_allocated_geometry_id(std::string* geometry_id);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_geometry_id();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_geometry_id(
      std::string* geometry_id);
  private:
  const std::string& _internal_geometry_id() const;
  void _internal_set_geometry_id(const std::string& value);
  std::string* _internal_mutable_geometry_id();
  public:

  // string physical_shape_id = 8;
  void clear_physical_shape_id();
  const std::string& physical_shape_id() const;
  void set_physical_shape_id(const std::string& value);
  void set_physical_shape_id(std::string&& value);
  void set_physical_shape_id(const char* value);
  void set_physical_shape_id(const char* value, size_t size);
  std::string* mutable_physical_shape_id();
  std::string* release_physical_shape_id();
  void set_allocated_physical_shape_id(std::string* physical_shape_id);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_physical_shape_id();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_physical_shape_id(
      std::string* physical_shape_id);
  private:
  const std::string& _internal_physical_shape_id() const;
  void _internal_set_physical_shape_id(const std::string& value);
  std::string* _internal_mutable_physical_shape_id();
  public:

  // .e8.SrtTransform srt_transform = 5;
  bool has_srt_transform() const;
  private:
  bool _internal_has_srt_transform() const;
  public:
  void clear_srt_transform();
  const ::e8::SrtTransform& srt_transform() const;
  ::e8::SrtTransform* release_srt_transform();
  ::e8::SrtTransform* mutable_srt_transform();
  void set_allocated_srt_transform(::e8::SrtTransform* srt_transform);
  private:
  const ::e8::SrtTransform& _internal_srt_transform() const;
  ::e8::SrtTransform* _internal_mutable_srt_transform();
  public:
  void unsafe_arena_set_allocated_srt_transform(
      ::e8::SrtTransform* srt_transform);
  ::e8::SrtTransform* unsafe_arena_release_srt_transform();

  // .e8.AABB bounding_box = 6;
  bool has_bounding_box() const;
  private:
  bool _internal_has_bounding_box() const;
  public:
  void clear_bounding_box();
  const ::e8::AABB& bounding_box() const;
  ::e8::AABB* release_bounding_box();
  ::e8::AABB* mutable_bounding_box();
  void set_allocated_bounding_box(::e8::AABB* bounding_box);
  private:
  const ::e8::AABB& _internal_bounding_box() const;
  ::e8::AABB* _internal_mutable_bounding_box();
  public:
  void unsafe_arena_set_allocated_bounding_box(
      ::e8::AABB* bounding_box);
  ::e8::AABB* unsafe_arena_release_bounding_box();

  // bool movable = 3;
  void clear_movable();
  bool movable() const;
  void set_movable(bool value);
  private:
  bool _internal_movable() const;
  void _internal_set_movable(bool value);
  public:

  // @@protoc_insertion_point(class_scope:e8.SceneEntityProto)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > transform_;
  mutable std::atomic<int> _transform_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr id_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr geometry_id_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr physical_shape_id_;
  ::e8::SrtTransform* srt_transform_;
  ::e8::AABB* bounding_box_;
  bool movable_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_entity_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SceneEntityProto

// string id = 1;
inline void SceneEntityProto::clear_id() {
  id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& SceneEntityProto::id() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityProto.id)
  return _internal_id();
}
inline void SceneEntityProto::set_id(const std::string& value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityProto.id)
}
inline std::string* SceneEntityProto::mutable_id() {
  // @@protoc_insertion_point(field_mutable:e8.SceneEntityProto.id)
  return _internal_mutable_id();
}
inline const std::string& SceneEntityProto::_internal_id() const {
  return id_.Get();
}
inline void SceneEntityProto::_internal_set_id(const std::string& value) {
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void SceneEntityProto::set_id(std::string&& value) {
  
  id_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:e8.SceneEntityProto.id)
}
inline void SceneEntityProto::set_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:e8.SceneEntityProto.id)
}
inline void SceneEntityProto::set_id(const char* value,
    size_t size) {
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:e8.SceneEntityProto.id)
}
inline std::string* SceneEntityProto::_internal_mutable_id() {
  
  return id_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* SceneEntityProto::release_id() {
  // @@protoc_insertion_point(field_release:e8.SceneEntityProto.id)
  return id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SceneEntityProto::set_allocated_id(std::string* id) {
  if (id != nullptr) {
    
  } else {
    
  }
  id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), id,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:e8.SceneEntityProto.id)
}
inline std::string* SceneEntityProto::unsafe_arena_release_id() {
  // @@protoc_insertion_point(field_unsafe_arena_release:e8.SceneEntityProto.id)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return id_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void SceneEntityProto::unsafe_arena_set_allocated_id(
    std::string* id) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (id != nullptr) {
    
  } else {
    
  }
  id_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      id, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.SceneEntityProto.id)
}

// string name = 2;
inline void SceneEntityProto::clear_name() {
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& SceneEntityProto::name() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityProto.name)
  return _internal_name();
}
inline void SceneEntityProto::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityProto.name)
}
inline std::string* SceneEntityProto::mutable_name() {
  // @@protoc_insertion_point(field_mutable:e8.SceneEntityProto.name)
  return _internal_mutable_name();
}
inline const std::string& SceneEntityProto::_internal_name() const {
  return name_.Get();
}
inline void SceneEntityProto::_internal_set_name(const std::string& value) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void SceneEntityProto::set_name(std::string&& value) {
  
  name_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:e8.SceneEntityProto.name)
}
inline void SceneEntityProto::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:e8.SceneEntityProto.name)
}
inline void SceneEntityProto::set_name(const char* value,
    size_t size) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:e8.SceneEntityProto.name)
}
inline std::string* SceneEntityProto::_internal_mutable_name() {
  
  return name_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* SceneEntityProto::release_name() {
  // @@protoc_insertion_point(field_release:e8.SceneEntityProto.name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SceneEntityProto::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:e8.SceneEntityProto.name)
}
inline std::string* SceneEntityProto::unsafe_arena_release_name() {
  // @@protoc_insertion_point(field_unsafe_arena_release:e8.SceneEntityProto.name)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return name_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void SceneEntityProto::unsafe_arena_set_allocated_name(
    std::string* name) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (name != nullptr) {
    
  } else {
    
  }
  name_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      name, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.SceneEntityProto.name)
}

// bool movable = 3;
inline void SceneEntityProto::clear_movable() {
  movable_ = false;
}
inline bool SceneEntityProto::_internal_movable() const {
  return movable_;
}
inline bool SceneEntityProto::movable() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityProto.movable)
  return _internal_movable();
}
inline void SceneEntityProto::_internal_set_movable(bool value) {
  
  movable_ = value;
}
inline void SceneEntityProto::set_movable(bool value) {
  _internal_set_movable(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityProto.movable)
}

// repeated float transform = 4;
inline int SceneEntityProto::_internal_transform_size() const {
  return transform_.size();
}
inline int SceneEntityProto::transform_size() const {
  return _internal_transform_size();
}
inline void SceneEntityProto::clear_transform() {
  transform_.Clear();
}
inline float SceneEntityProto::_internal_transform(int index) const {
  return transform_.Get(index);
}
inline float SceneEntityProto::transform(int index) const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityProto.transform)
  return _internal_transform(index);
}
inline void SceneEntityProto::set_transform(int index, float value) {
  transform_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityProto.transform)
}
inline void SceneEntityProto::_internal_add_transform(float value) {
  transform_.Add(value);
}
inline void SceneEntityProto::add_transform(float value) {
  _internal_add_transform(value);
  // @@protoc_insertion_point(field_add:e8.SceneEntityProto.transform)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
SceneEntityProto::_internal_transform() const {
  return transform_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
SceneEntityProto::transform() const {
  // @@protoc_insertion_point(field_list:e8.SceneEntityProto.transform)
  return _internal_transform();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
SceneEntityProto::_internal_mutable_transform() {
  return &transform_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
SceneEntityProto::mutable_transform() {
  // @@protoc_insertion_point(field_mutable_list:e8.SceneEntityProto.transform)
  return _internal_mutable_transform();
}

// .e8.SrtTransform srt_transform = 5;
inline bool SceneEntityProto::_internal_has_srt_transform() const {
  return this != internal_default_instance() && srt_transform_ != nullptr;
}
inline bool SceneEntityProto::has_srt_transform() const {
  return _internal_has_srt_transform();
}
inline const ::e8::SrtTransform& SceneEntityProto::_internal_srt_transform() const {
  const ::e8::SrtTransform* p = srt_transform_;
  return p != nullptr ? *p : *reinterpret_cast<const ::e8::SrtTransform*>(
      &::e8::_SrtTransform_default_instance_);
}
inline const ::e8::SrtTransform& SceneEntityProto::srt_transform() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityProto.srt_transform)
  return _internal_srt_transform();
}
inline void SceneEntityProto::unsafe_arena_set_allocated_srt_transform(
    ::e8::SrtTransform* srt_transform) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(srt_transform_);
  }
  srt_transform_ = srt_transform;
  if (srt_transform) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.SceneEntityProto.srt_transform)
}
inline ::e8::SrtTransform* SceneEntityProto::release_srt_transform() {
  auto temp = unsafe_arena_release_srt_transform();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::e8::SrtTransform* SceneEntityProto::unsafe_arena_release_srt_transform() {
  // @@protoc_insertion_point(field_release:e8.SceneEntityProto.srt_transform)
  
  ::e8::SrtTransform* temp = srt_transform_;
  srt_transform_ = nullptr;
  return temp;
}
inline ::e8::SrtTransform* SceneEntityProto::_internal_mutable_srt_transform() {
  
  if (srt_transform_ == nullptr) {
    auto* p = CreateMaybeMessage<::e8::SrtTransform>(GetArena());
    srt_transform_ = p;
  }
  return srt_transform_;
}
inline ::e8::SrtTransform* SceneEntityProto::mutable_srt_transform() {
  // @@protoc_insertion_point(field_mutable:e8.SceneEntityProto.srt_transform)
  return _internal_mutable_srt_transform();
}
inline void SceneEntityProto::set_allocated_srt_transform(::e8::SrtTransform* srt_transform) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(srt_transform_);
  }
  if (srt_transform) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(srt_transform)->GetArena();
    if (message_arena != submessage_arena) {
      srt_transform = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, srt_transform, submessage_arena);
    }
    
  } else {
    
  }
  srt_transform_ = srt_transform;
  // @@protoc_insertion_point(field_set_allocated:e8.SceneEntityProto.srt_transform)
}

// .e8.AABB bounding_box = 6;
inline bool SceneEntityProto::_internal_has_bounding_box() const {
  return this != internal_default_instance() && bounding_box_ != nullptr;
}
inline bool SceneEntityProto::has_bounding_box() const {
  return _internal_has_bounding_box();
}
inline const ::e8::AABB& SceneEntityProto::_internal_bounding_box() const {
  const ::e8::AABB* p = bounding_box_;
  return p != nullptr ? *p : *reinterpret_cast<const ::e8::AABB*>(
      &::e8::_AABB_default_instance_);
}
inline const ::e8::AABB& SceneEntityProto::bounding_box() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityProto.bounding_box)
  return _internal_bounding_box();
}
inline void SceneEntityProto::unsafe_arena_set_allocated_bounding_box(
    ::e8::AABB* bounding_box) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(bounding_box_);
  }
  bounding_box_ = bounding_box;
  if (bounding_box) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.SceneEntityProto.bounding_box)
}
inline ::e8::AABB* SceneEntityProto::release_bounding_box() {
  auto temp = unsafe_arena_release_bounding_box();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::e8::AABB* SceneEntityProto::unsafe_arena_release_bounding_box() {
  // @@protoc_insertion_point(field_release:e8.SceneEntityProto.bounding_box)
  
  ::e8::AABB* temp = bounding_box_;
  bounding_box_ = nullptr;
  return temp;
}
inline ::e8::AABB* SceneEntityProto::_internal_mutable_bounding_box() {
  
  if (bounding_box_ == nullptr) {
    auto* p = CreateMaybeMessage<::e8::AABB>(GetArena());
    bounding_box_ = p;
  }
  return bounding_box_;
}
inline ::e8::AABB* SceneEntityProto::mutable_bounding_box() {
  // @@protoc_insertion_point(field_mutable:e8.SceneEntityProto.bounding_box)
  return _internal_mutable_bounding_box();
}
inline void SceneEntityProto::set_allocated_bounding_box(::e8::AABB* bounding_box) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(bounding_box_);
  }
  if (bounding_box) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(bounding_box)->GetArena();
    if (message_arena != submessage_arena) {
      bounding_box = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, bounding_box, submessage_arena);
    }
    
  } else {
    
  }
  bounding_box_ = bounding_box;
  // @@protoc_insertion_point(field_set_allocated:e8.SceneEntityProto.bounding_box)
}

// string geometry_id = 7;
inline void SceneEntityProto::clear_geometry_id() {
  geometry_id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& SceneEntityProto::geometry_id() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityProto.geometry_id)
  return _internal_geometry_id();
}
inline void SceneEntityProto::set_geometry_id(const std::string& value) {
  _internal_set_geometry_id(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityProto.geometry_id)
}
inline std::string* SceneEntityProto::mutable_geometry_id() {
  // @@protoc_insertion_point(field_mutable:e8.SceneEntityProto.geometry_id)
  return _internal_mutable_geometry_id();
}
inline const std::string& SceneEntityProto::_internal_geometry_id() const {
  return geometry_id_.Get();
}
inline void SceneEntityProto::_internal_set_geometry_id(const std::string& value) {
  
  geometry_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void SceneEntityProto::set_geometry_id(std::string&& value) {
  
  geometry_id_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:e8.SceneEntityProto.geometry_id)
}
inline void SceneEntityProto::set_geometry_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  geometry_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:e8.SceneEntityProto.geometry_id)
}
inline void SceneEntityProto::set_geometry_id(const char* value,
    size_t size) {
  
  geometry_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:e8.SceneEntityProto.geometry_id)
}
inline std::string* SceneEntityProto::_internal_mutable_geometry_id() {
  
  return geometry_id_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* SceneEntityProto::release_geometry_id() {
  // @@protoc_insertion_point(field_release:e8.SceneEntityProto.geometry_id)
  return geometry_id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SceneEntityProto::set_allocated_geometry_id(std::string* geometry_id) {
  if (geometry_id != nullptr) {
    
  } else {
    
  }
  geometry_id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), geometry_id,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:e8.SceneEntityProto.geometry_id)
}
inline std::string* SceneEntityProto::unsafe_arena_release_geometry_id() {
  // @@protoc_insertion_point(field_unsafe_arena_release:e8.SceneEntityProto.geometry_id)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return geometry_id_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void SceneEntityProto::unsafe_arena_set_allocated_geometry_id(
    std::string* geometry_id) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (geometry_id != nullptr) {
    
  } else {
    
  }
  geometry_id_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      geometry_id, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.SceneEntityProto.geometry_id)
}

// string physical_shape_id = 8;
inline void SceneEntityProto::clear_physical_shape_id() {
  physical_shape_id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& SceneEntityProto::physical_shape_id() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityProto.physical_shape_id)
  return _internal_physical_shape_id();
}
inline void SceneEntityProto::set_physical_shape_id(const std::string& value) {
  _internal_set_physical_shape_id(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityProto.physical_shape_id)
}
inline std::string* SceneEntityProto::mutable_physical_shape_id() {
  // @@protoc_insertion_point(field_mutable:e8.SceneEntityProto.physical_shape_id)
  return _internal_mutable_physical_shape_id();
}
inline const std::string& SceneEntityProto::_internal_physical_shape_id() const {
  return physical_shape_id_.Get();
}
inline void SceneEntityProto::_internal_set_physical_shape_id(const std::string& value) {
  
  physical_shape_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void SceneEntityProto::set_physical_shape_id(std::string&& value) {
  
  physical_shape_id_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:e8.SceneEntityProto.physical_shape_id)
}
inline void SceneEntityProto::set_physical_shape_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  physical_shape_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:e8.SceneEntityProto.physical_shape_id)
}
inline void SceneEntityProto::set_physical_shape_id(const char* value,
    size_t size) {
  
  physical_shape_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:e8.SceneEntityProto.physical_shape_id)
}
inline std::string* SceneEntityProto::_internal_mutable_physical_shape_id() {
  
  return physical_shape_id_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* SceneEntityProto::release_physical_shape_id() {
  // @@protoc_insertion_point(field_release:e8.SceneEntityProto.physical_shape_id)
  return physical_shape_id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void SceneEntityProto::set_allocated_physical_shape_id(std::string* physical_shape_id) {
  if (physical_shape_id != nullptr) {
    
  } else {
    
  }
  physical_shape_id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), physical_shape_id,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:e8.SceneEntityProto.physical_shape_id)
}
inline std::string* SceneEntityProto::unsafe_arena_release_physical_shape_id() {
  // @@protoc_insertion_point(field_unsafe_arena_release:e8.SceneEntityProto.physical_shape_id)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return physical_shape_id_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void SceneEntityProto::unsafe_arena_set_allocated_physical_shape_id(
    std::string* physical_shape_id) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (physical_shape_id != nullptr) {
    
  } else {
    
  }
  physical_shape_id_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      physical_shape_id, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.SceneEntityProto.physical_shape_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace e8

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_entity_2eproto
