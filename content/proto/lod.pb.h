// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: lod.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_lod_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_lod_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_lod_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_lod_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_lod_2eproto;
namespace e8 {
class SceneEntityResources;
class SceneEntityResourcesDefaultTypeInternal;
extern SceneEntityResourcesDefaultTypeInternal _SceneEntityResources_default_instance_;
class SceneEntityResources_Lod;
class SceneEntityResources_LodDefaultTypeInternal;
extern SceneEntityResources_LodDefaultTypeInternal _SceneEntityResources_Lod_default_instance_;
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> ::e8::SceneEntityResources* Arena::CreateMaybeMessage<::e8::SceneEntityResources>(Arena*);
template<> ::e8::SceneEntityResources_Lod* Arena::CreateMaybeMessage<::e8::SceneEntityResources_Lod>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace e8 {

// ===================================================================

class SceneEntityResources_Lod PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.SceneEntityResources.Lod) */ {
 public:
  inline SceneEntityResources_Lod() : SceneEntityResources_Lod(nullptr) {};
  virtual ~SceneEntityResources_Lod();

  SceneEntityResources_Lod(const SceneEntityResources_Lod& from);
  SceneEntityResources_Lod(SceneEntityResources_Lod&& from) noexcept
    : SceneEntityResources_Lod() {
    *this = ::std::move(from);
  }

  inline SceneEntityResources_Lod& operator=(const SceneEntityResources_Lod& from) {
    CopyFrom(from);
    return *this;
  }
  inline SceneEntityResources_Lod& operator=(SceneEntityResources_Lod&& from) noexcept {
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
  static const SceneEntityResources_Lod& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const SceneEntityResources_Lod* internal_default_instance() {
    return reinterpret_cast<const SceneEntityResources_Lod*>(
               &_SceneEntityResources_Lod_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SceneEntityResources_Lod& a, SceneEntityResources_Lod& b) {
    a.Swap(&b);
  }
  inline void Swap(SceneEntityResources_Lod* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SceneEntityResources_Lod* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SceneEntityResources_Lod* New() const final {
    return CreateMaybeMessage<SceneEntityResources_Lod>(nullptr);
  }

  SceneEntityResources_Lod* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SceneEntityResources_Lod>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SceneEntityResources_Lod& from);
  void MergeFrom(const SceneEntityResources_Lod& from);
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
  void InternalSwap(SceneEntityResources_Lod* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.SceneEntityResources.Lod";
  }
  protected:
  explicit SceneEntityResources_Lod(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_lod_2eproto);
    return ::descriptor_table_lod_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kGeometryIdFieldNumber = 2,
    kMaterialIdFieldNumber = 3,
    kLightMapIdFieldNumber = 4,
    kIndirectLightMapIdFieldNumber = 5,
    kPhysicalShapeIdFieldNumber = 6,
    kApplyAfterDistanceFieldNumber = 1,
  };
  // int64 geometry_id = 2;
  void clear_geometry_id();
  ::PROTOBUF_NAMESPACE_ID::int64 geometry_id() const;
  void set_geometry_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_geometry_id() const;
  void _internal_set_geometry_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // int64 material_id = 3;
  void clear_material_id();
  ::PROTOBUF_NAMESPACE_ID::int64 material_id() const;
  void set_material_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_material_id() const;
  void _internal_set_material_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // int64 light_map_id = 4;
  void clear_light_map_id();
  ::PROTOBUF_NAMESPACE_ID::int64 light_map_id() const;
  void set_light_map_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_light_map_id() const;
  void _internal_set_light_map_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // int64 indirect_light_map_id = 5;
  void clear_indirect_light_map_id();
  ::PROTOBUF_NAMESPACE_ID::int64 indirect_light_map_id() const;
  void set_indirect_light_map_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_indirect_light_map_id() const;
  void _internal_set_indirect_light_map_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // int64 physical_shape_id = 6;
  void clear_physical_shape_id();
  ::PROTOBUF_NAMESPACE_ID::int64 physical_shape_id() const;
  void set_physical_shape_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_physical_shape_id() const;
  void _internal_set_physical_shape_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // float apply_after_distance = 1;
  void clear_apply_after_distance();
  float apply_after_distance() const;
  void set_apply_after_distance(float value);
  private:
  float _internal_apply_after_distance() const;
  void _internal_set_apply_after_distance(float value);
  public:

  // @@protoc_insertion_point(class_scope:e8.SceneEntityResources.Lod)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::int64 geometry_id_;
  ::PROTOBUF_NAMESPACE_ID::int64 material_id_;
  ::PROTOBUF_NAMESPACE_ID::int64 light_map_id_;
  ::PROTOBUF_NAMESPACE_ID::int64 indirect_light_map_id_;
  ::PROTOBUF_NAMESPACE_ID::int64 physical_shape_id_;
  float apply_after_distance_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_lod_2eproto;
};
// -------------------------------------------------------------------

class SceneEntityResources PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.SceneEntityResources) */ {
 public:
  inline SceneEntityResources() : SceneEntityResources(nullptr) {};
  virtual ~SceneEntityResources();

  SceneEntityResources(const SceneEntityResources& from);
  SceneEntityResources(SceneEntityResources&& from) noexcept
    : SceneEntityResources() {
    *this = ::std::move(from);
  }

  inline SceneEntityResources& operator=(const SceneEntityResources& from) {
    CopyFrom(from);
    return *this;
  }
  inline SceneEntityResources& operator=(SceneEntityResources&& from) noexcept {
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
  static const SceneEntityResources& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const SceneEntityResources* internal_default_instance() {
    return reinterpret_cast<const SceneEntityResources*>(
               &_SceneEntityResources_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(SceneEntityResources& a, SceneEntityResources& b) {
    a.Swap(&b);
  }
  inline void Swap(SceneEntityResources* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SceneEntityResources* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SceneEntityResources* New() const final {
    return CreateMaybeMessage<SceneEntityResources>(nullptr);
  }

  SceneEntityResources* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SceneEntityResources>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SceneEntityResources& from);
  void MergeFrom(const SceneEntityResources& from);
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
  void InternalSwap(SceneEntityResources* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.SceneEntityResources";
  }
  protected:
  explicit SceneEntityResources(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_lod_2eproto);
    return ::descriptor_table_lod_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef SceneEntityResources_Lod Lod;

  // accessors -------------------------------------------------------

  enum : int {
    kLodsFieldNumber = 1,
  };
  // repeated .e8.SceneEntityResources.Lod lods = 1;
  int lods_size() const;
  private:
  int _internal_lods_size() const;
  public:
  void clear_lods();
  ::e8::SceneEntityResources_Lod* mutable_lods(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::SceneEntityResources_Lod >*
      mutable_lods();
  private:
  const ::e8::SceneEntityResources_Lod& _internal_lods(int index) const;
  ::e8::SceneEntityResources_Lod* _internal_add_lods();
  public:
  const ::e8::SceneEntityResources_Lod& lods(int index) const;
  ::e8::SceneEntityResources_Lod* add_lods();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::SceneEntityResources_Lod >&
      lods() const;

  // @@protoc_insertion_point(class_scope:e8.SceneEntityResources)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::SceneEntityResources_Lod > lods_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_lod_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SceneEntityResources_Lod

// float apply_after_distance = 1;
inline void SceneEntityResources_Lod::clear_apply_after_distance() {
  apply_after_distance_ = 0;
}
inline float SceneEntityResources_Lod::_internal_apply_after_distance() const {
  return apply_after_distance_;
}
inline float SceneEntityResources_Lod::apply_after_distance() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityResources.Lod.apply_after_distance)
  return _internal_apply_after_distance();
}
inline void SceneEntityResources_Lod::_internal_set_apply_after_distance(float value) {
  
  apply_after_distance_ = value;
}
inline void SceneEntityResources_Lod::set_apply_after_distance(float value) {
  _internal_set_apply_after_distance(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityResources.Lod.apply_after_distance)
}

// int64 geometry_id = 2;
inline void SceneEntityResources_Lod::clear_geometry_id() {
  geometry_id_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::_internal_geometry_id() const {
  return geometry_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::geometry_id() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityResources.Lod.geometry_id)
  return _internal_geometry_id();
}
inline void SceneEntityResources_Lod::_internal_set_geometry_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  geometry_id_ = value;
}
inline void SceneEntityResources_Lod::set_geometry_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_geometry_id(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityResources.Lod.geometry_id)
}

// int64 material_id = 3;
inline void SceneEntityResources_Lod::clear_material_id() {
  material_id_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::_internal_material_id() const {
  return material_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::material_id() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityResources.Lod.material_id)
  return _internal_material_id();
}
inline void SceneEntityResources_Lod::_internal_set_material_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  material_id_ = value;
}
inline void SceneEntityResources_Lod::set_material_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_material_id(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityResources.Lod.material_id)
}

// int64 light_map_id = 4;
inline void SceneEntityResources_Lod::clear_light_map_id() {
  light_map_id_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::_internal_light_map_id() const {
  return light_map_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::light_map_id() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityResources.Lod.light_map_id)
  return _internal_light_map_id();
}
inline void SceneEntityResources_Lod::_internal_set_light_map_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  light_map_id_ = value;
}
inline void SceneEntityResources_Lod::set_light_map_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_light_map_id(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityResources.Lod.light_map_id)
}

// int64 indirect_light_map_id = 5;
inline void SceneEntityResources_Lod::clear_indirect_light_map_id() {
  indirect_light_map_id_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::_internal_indirect_light_map_id() const {
  return indirect_light_map_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::indirect_light_map_id() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityResources.Lod.indirect_light_map_id)
  return _internal_indirect_light_map_id();
}
inline void SceneEntityResources_Lod::_internal_set_indirect_light_map_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  indirect_light_map_id_ = value;
}
inline void SceneEntityResources_Lod::set_indirect_light_map_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_indirect_light_map_id(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityResources.Lod.indirect_light_map_id)
}

// int64 physical_shape_id = 6;
inline void SceneEntityResources_Lod::clear_physical_shape_id() {
  physical_shape_id_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::_internal_physical_shape_id() const {
  return physical_shape_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SceneEntityResources_Lod::physical_shape_id() const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityResources.Lod.physical_shape_id)
  return _internal_physical_shape_id();
}
inline void SceneEntityResources_Lod::_internal_set_physical_shape_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  physical_shape_id_ = value;
}
inline void SceneEntityResources_Lod::set_physical_shape_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_physical_shape_id(value);
  // @@protoc_insertion_point(field_set:e8.SceneEntityResources.Lod.physical_shape_id)
}

// -------------------------------------------------------------------

// SceneEntityResources

// repeated .e8.SceneEntityResources.Lod lods = 1;
inline int SceneEntityResources::_internal_lods_size() const {
  return lods_.size();
}
inline int SceneEntityResources::lods_size() const {
  return _internal_lods_size();
}
inline void SceneEntityResources::clear_lods() {
  lods_.Clear();
}
inline ::e8::SceneEntityResources_Lod* SceneEntityResources::mutable_lods(int index) {
  // @@protoc_insertion_point(field_mutable:e8.SceneEntityResources.lods)
  return lods_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::SceneEntityResources_Lod >*
SceneEntityResources::mutable_lods() {
  // @@protoc_insertion_point(field_mutable_list:e8.SceneEntityResources.lods)
  return &lods_;
}
inline const ::e8::SceneEntityResources_Lod& SceneEntityResources::_internal_lods(int index) const {
  return lods_.Get(index);
}
inline const ::e8::SceneEntityResources_Lod& SceneEntityResources::lods(int index) const {
  // @@protoc_insertion_point(field_get:e8.SceneEntityResources.lods)
  return _internal_lods(index);
}
inline ::e8::SceneEntityResources_Lod* SceneEntityResources::_internal_add_lods() {
  return lods_.Add();
}
inline ::e8::SceneEntityResources_Lod* SceneEntityResources::add_lods() {
  // @@protoc_insertion_point(field_add:e8.SceneEntityResources.lods)
  return _internal_add_lods();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::SceneEntityResources_Lod >&
SceneEntityResources::lods() const {
  // @@protoc_insertion_point(field_list:e8.SceneEntityResources.lods)
  return lods_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace e8

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_lod_2eproto
