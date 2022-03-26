// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: material.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_material_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_material_2eproto

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
#include "texture.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_material_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_material_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_material_2eproto;
namespace e8 {
class MaterialProto;
class MaterialProtoDefaultTypeInternal;
extern MaterialProtoDefaultTypeInternal _MaterialProto_default_instance_;
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> ::e8::MaterialProto* Arena::CreateMaybeMessage<::e8::MaterialProto>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace e8 {

// ===================================================================

class MaterialProto PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.MaterialProto) */ {
 public:
  inline MaterialProto() : MaterialProto(nullptr) {};
  virtual ~MaterialProto();

  MaterialProto(const MaterialProto& from);
  MaterialProto(MaterialProto&& from) noexcept
    : MaterialProto() {
    *this = ::std::move(from);
  }

  inline MaterialProto& operator=(const MaterialProto& from) {
    CopyFrom(from);
    return *this;
  }
  inline MaterialProto& operator=(MaterialProto&& from) noexcept {
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
  static const MaterialProto& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const MaterialProto* internal_default_instance() {
    return reinterpret_cast<const MaterialProto*>(
               &_MaterialProto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(MaterialProto& a, MaterialProto& b) {
    a.Swap(&b);
  }
  inline void Swap(MaterialProto* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(MaterialProto* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline MaterialProto* New() const final {
    return CreateMaybeMessage<MaterialProto>(nullptr);
  }

  MaterialProto* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<MaterialProto>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const MaterialProto& from);
  void MergeFrom(const MaterialProto& from);
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
  void InternalSwap(MaterialProto* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.MaterialProto";
  }
  protected:
  explicit MaterialProto(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_material_2eproto);
    return ::descriptor_table_material_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIdFieldNumber = 1,
    kNameFieldNumber = 2,
    kAlbedoFieldNumber = 3,
    kNormalFieldNumber = 4,
    kRoughnessFieldNumber = 5,
  };
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

  // .e8.TextureProto albedo = 3;
  bool has_albedo() const;
  private:
  bool _internal_has_albedo() const;
  public:
  void clear_albedo();
  const ::e8::TextureProto& albedo() const;
  ::e8::TextureProto* release_albedo();
  ::e8::TextureProto* mutable_albedo();
  void set_allocated_albedo(::e8::TextureProto* albedo);
  private:
  const ::e8::TextureProto& _internal_albedo() const;
  ::e8::TextureProto* _internal_mutable_albedo();
  public:
  void unsafe_arena_set_allocated_albedo(
      ::e8::TextureProto* albedo);
  ::e8::TextureProto* unsafe_arena_release_albedo();

  // .e8.TextureProto normal = 4;
  bool has_normal() const;
  private:
  bool _internal_has_normal() const;
  public:
  void clear_normal();
  const ::e8::TextureProto& normal() const;
  ::e8::TextureProto* release_normal();
  ::e8::TextureProto* mutable_normal();
  void set_allocated_normal(::e8::TextureProto* normal);
  private:
  const ::e8::TextureProto& _internal_normal() const;
  ::e8::TextureProto* _internal_mutable_normal();
  public:
  void unsafe_arena_set_allocated_normal(
      ::e8::TextureProto* normal);
  ::e8::TextureProto* unsafe_arena_release_normal();

  // .e8.TextureProto roughness = 5;
  bool has_roughness() const;
  private:
  bool _internal_has_roughness() const;
  public:
  void clear_roughness();
  const ::e8::TextureProto& roughness() const;
  ::e8::TextureProto* release_roughness();
  ::e8::TextureProto* mutable_roughness();
  void set_allocated_roughness(::e8::TextureProto* roughness);
  private:
  const ::e8::TextureProto& _internal_roughness() const;
  ::e8::TextureProto* _internal_mutable_roughness();
  public:
  void unsafe_arena_set_allocated_roughness(
      ::e8::TextureProto* roughness);
  ::e8::TextureProto* unsafe_arena_release_roughness();

  // @@protoc_insertion_point(class_scope:e8.MaterialProto)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr id_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::e8::TextureProto* albedo_;
  ::e8::TextureProto* normal_;
  ::e8::TextureProto* roughness_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_material_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// MaterialProto

// string id = 1;
inline void MaterialProto::clear_id() {
  id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& MaterialProto::id() const {
  // @@protoc_insertion_point(field_get:e8.MaterialProto.id)
  return _internal_id();
}
inline void MaterialProto::set_id(const std::string& value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:e8.MaterialProto.id)
}
inline std::string* MaterialProto::mutable_id() {
  // @@protoc_insertion_point(field_mutable:e8.MaterialProto.id)
  return _internal_mutable_id();
}
inline const std::string& MaterialProto::_internal_id() const {
  return id_.Get();
}
inline void MaterialProto::_internal_set_id(const std::string& value) {
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void MaterialProto::set_id(std::string&& value) {
  
  id_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:e8.MaterialProto.id)
}
inline void MaterialProto::set_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:e8.MaterialProto.id)
}
inline void MaterialProto::set_id(const char* value,
    size_t size) {
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:e8.MaterialProto.id)
}
inline std::string* MaterialProto::_internal_mutable_id() {
  
  return id_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* MaterialProto::release_id() {
  // @@protoc_insertion_point(field_release:e8.MaterialProto.id)
  return id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void MaterialProto::set_allocated_id(std::string* id) {
  if (id != nullptr) {
    
  } else {
    
  }
  id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), id,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:e8.MaterialProto.id)
}
inline std::string* MaterialProto::unsafe_arena_release_id() {
  // @@protoc_insertion_point(field_unsafe_arena_release:e8.MaterialProto.id)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return id_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void MaterialProto::unsafe_arena_set_allocated_id(
    std::string* id) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (id != nullptr) {
    
  } else {
    
  }
  id_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      id, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.MaterialProto.id)
}

// string name = 2;
inline void MaterialProto::clear_name() {
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& MaterialProto::name() const {
  // @@protoc_insertion_point(field_get:e8.MaterialProto.name)
  return _internal_name();
}
inline void MaterialProto::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:e8.MaterialProto.name)
}
inline std::string* MaterialProto::mutable_name() {
  // @@protoc_insertion_point(field_mutable:e8.MaterialProto.name)
  return _internal_mutable_name();
}
inline const std::string& MaterialProto::_internal_name() const {
  return name_.Get();
}
inline void MaterialProto::_internal_set_name(const std::string& value) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void MaterialProto::set_name(std::string&& value) {
  
  name_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:e8.MaterialProto.name)
}
inline void MaterialProto::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:e8.MaterialProto.name)
}
inline void MaterialProto::set_name(const char* value,
    size_t size) {
  
  name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:e8.MaterialProto.name)
}
inline std::string* MaterialProto::_internal_mutable_name() {
  
  return name_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* MaterialProto::release_name() {
  // @@protoc_insertion_point(field_release:e8.MaterialProto.name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void MaterialProto::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:e8.MaterialProto.name)
}
inline std::string* MaterialProto::unsafe_arena_release_name() {
  // @@protoc_insertion_point(field_unsafe_arena_release:e8.MaterialProto.name)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return name_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void MaterialProto::unsafe_arena_set_allocated_name(
    std::string* name) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (name != nullptr) {
    
  } else {
    
  }
  name_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      name, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.MaterialProto.name)
}

// .e8.TextureProto albedo = 3;
inline bool MaterialProto::_internal_has_albedo() const {
  return this != internal_default_instance() && albedo_ != nullptr;
}
inline bool MaterialProto::has_albedo() const {
  return _internal_has_albedo();
}
inline const ::e8::TextureProto& MaterialProto::_internal_albedo() const {
  const ::e8::TextureProto* p = albedo_;
  return p != nullptr ? *p : *reinterpret_cast<const ::e8::TextureProto*>(
      &::e8::_TextureProto_default_instance_);
}
inline const ::e8::TextureProto& MaterialProto::albedo() const {
  // @@protoc_insertion_point(field_get:e8.MaterialProto.albedo)
  return _internal_albedo();
}
inline void MaterialProto::unsafe_arena_set_allocated_albedo(
    ::e8::TextureProto* albedo) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(albedo_);
  }
  albedo_ = albedo;
  if (albedo) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.MaterialProto.albedo)
}
inline ::e8::TextureProto* MaterialProto::release_albedo() {
  auto temp = unsafe_arena_release_albedo();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::e8::TextureProto* MaterialProto::unsafe_arena_release_albedo() {
  // @@protoc_insertion_point(field_release:e8.MaterialProto.albedo)
  
  ::e8::TextureProto* temp = albedo_;
  albedo_ = nullptr;
  return temp;
}
inline ::e8::TextureProto* MaterialProto::_internal_mutable_albedo() {
  
  if (albedo_ == nullptr) {
    auto* p = CreateMaybeMessage<::e8::TextureProto>(GetArena());
    albedo_ = p;
  }
  return albedo_;
}
inline ::e8::TextureProto* MaterialProto::mutable_albedo() {
  // @@protoc_insertion_point(field_mutable:e8.MaterialProto.albedo)
  return _internal_mutable_albedo();
}
inline void MaterialProto::set_allocated_albedo(::e8::TextureProto* albedo) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(albedo_);
  }
  if (albedo) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(albedo)->GetArena();
    if (message_arena != submessage_arena) {
      albedo = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, albedo, submessage_arena);
    }
    
  } else {
    
  }
  albedo_ = albedo;
  // @@protoc_insertion_point(field_set_allocated:e8.MaterialProto.albedo)
}

// .e8.TextureProto normal = 4;
inline bool MaterialProto::_internal_has_normal() const {
  return this != internal_default_instance() && normal_ != nullptr;
}
inline bool MaterialProto::has_normal() const {
  return _internal_has_normal();
}
inline const ::e8::TextureProto& MaterialProto::_internal_normal() const {
  const ::e8::TextureProto* p = normal_;
  return p != nullptr ? *p : *reinterpret_cast<const ::e8::TextureProto*>(
      &::e8::_TextureProto_default_instance_);
}
inline const ::e8::TextureProto& MaterialProto::normal() const {
  // @@protoc_insertion_point(field_get:e8.MaterialProto.normal)
  return _internal_normal();
}
inline void MaterialProto::unsafe_arena_set_allocated_normal(
    ::e8::TextureProto* normal) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(normal_);
  }
  normal_ = normal;
  if (normal) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.MaterialProto.normal)
}
inline ::e8::TextureProto* MaterialProto::release_normal() {
  auto temp = unsafe_arena_release_normal();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::e8::TextureProto* MaterialProto::unsafe_arena_release_normal() {
  // @@protoc_insertion_point(field_release:e8.MaterialProto.normal)
  
  ::e8::TextureProto* temp = normal_;
  normal_ = nullptr;
  return temp;
}
inline ::e8::TextureProto* MaterialProto::_internal_mutable_normal() {
  
  if (normal_ == nullptr) {
    auto* p = CreateMaybeMessage<::e8::TextureProto>(GetArena());
    normal_ = p;
  }
  return normal_;
}
inline ::e8::TextureProto* MaterialProto::mutable_normal() {
  // @@protoc_insertion_point(field_mutable:e8.MaterialProto.normal)
  return _internal_mutable_normal();
}
inline void MaterialProto::set_allocated_normal(::e8::TextureProto* normal) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(normal_);
  }
  if (normal) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(normal)->GetArena();
    if (message_arena != submessage_arena) {
      normal = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, normal, submessage_arena);
    }
    
  } else {
    
  }
  normal_ = normal;
  // @@protoc_insertion_point(field_set_allocated:e8.MaterialProto.normal)
}

// .e8.TextureProto roughness = 5;
inline bool MaterialProto::_internal_has_roughness() const {
  return this != internal_default_instance() && roughness_ != nullptr;
}
inline bool MaterialProto::has_roughness() const {
  return _internal_has_roughness();
}
inline const ::e8::TextureProto& MaterialProto::_internal_roughness() const {
  const ::e8::TextureProto* p = roughness_;
  return p != nullptr ? *p : *reinterpret_cast<const ::e8::TextureProto*>(
      &::e8::_TextureProto_default_instance_);
}
inline const ::e8::TextureProto& MaterialProto::roughness() const {
  // @@protoc_insertion_point(field_get:e8.MaterialProto.roughness)
  return _internal_roughness();
}
inline void MaterialProto::unsafe_arena_set_allocated_roughness(
    ::e8::TextureProto* roughness) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(roughness_);
  }
  roughness_ = roughness;
  if (roughness) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.MaterialProto.roughness)
}
inline ::e8::TextureProto* MaterialProto::release_roughness() {
  auto temp = unsafe_arena_release_roughness();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::e8::TextureProto* MaterialProto::unsafe_arena_release_roughness() {
  // @@protoc_insertion_point(field_release:e8.MaterialProto.roughness)
  
  ::e8::TextureProto* temp = roughness_;
  roughness_ = nullptr;
  return temp;
}
inline ::e8::TextureProto* MaterialProto::_internal_mutable_roughness() {
  
  if (roughness_ == nullptr) {
    auto* p = CreateMaybeMessage<::e8::TextureProto>(GetArena());
    roughness_ = p;
  }
  return roughness_;
}
inline ::e8::TextureProto* MaterialProto::mutable_roughness() {
  // @@protoc_insertion_point(field_mutable:e8.MaterialProto.roughness)
  return _internal_mutable_roughness();
}
inline void MaterialProto::set_allocated_roughness(::e8::TextureProto* roughness) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(roughness_);
  }
  if (roughness) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(roughness)->GetArena();
    if (message_arena != submessage_arena) {
      roughness = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, roughness, submessage_arena);
    }
    
  } else {
    
  }
  roughness_ = roughness;
  // @@protoc_insertion_point(field_set_allocated:e8.MaterialProto.roughness)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace e8

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_material_2eproto