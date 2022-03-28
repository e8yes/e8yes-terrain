// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: light_map.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_light_5fmap_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_light_5fmap_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_light_5fmap_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_light_5fmap_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_light_5fmap_2eproto;
namespace e8 {
class LightMapProto;
class LightMapProtoDefaultTypeInternal;
extern LightMapProtoDefaultTypeInternal _LightMapProto_default_instance_;
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> ::e8::LightMapProto* Arena::CreateMaybeMessage<::e8::LightMapProto>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace e8 {

// ===================================================================

class LightMapProto PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.LightMapProto) */ {
 public:
  inline LightMapProto() : LightMapProto(nullptr) {};
  virtual ~LightMapProto();

  LightMapProto(const LightMapProto& from);
  LightMapProto(LightMapProto&& from) noexcept
    : LightMapProto() {
    *this = ::std::move(from);
  }

  inline LightMapProto& operator=(const LightMapProto& from) {
    CopyFrom(from);
    return *this;
  }
  inline LightMapProto& operator=(LightMapProto&& from) noexcept {
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
  static const LightMapProto& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const LightMapProto* internal_default_instance() {
    return reinterpret_cast<const LightMapProto*>(
               &_LightMapProto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(LightMapProto& a, LightMapProto& b) {
    a.Swap(&b);
  }
  inline void Swap(LightMapProto* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(LightMapProto* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline LightMapProto* New() const final {
    return CreateMaybeMessage<LightMapProto>(nullptr);
  }

  LightMapProto* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<LightMapProto>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const LightMapProto& from);
  void MergeFrom(const LightMapProto& from);
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
  void InternalSwap(LightMapProto* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.LightMapProto";
  }
  protected:
  explicit LightMapProto(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_light_5fmap_2eproto);
    return ::descriptor_table_light_5fmap_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRadiosityFieldNumber = 2,
    kIdFieldNumber = 1,
  };
  // .e8.TextureProto radiosity = 2;
  bool has_radiosity() const;
  private:
  bool _internal_has_radiosity() const;
  public:
  void clear_radiosity();
  const ::e8::TextureProto& radiosity() const;
  ::e8::TextureProto* release_radiosity();
  ::e8::TextureProto* mutable_radiosity();
  void set_allocated_radiosity(::e8::TextureProto* radiosity);
  private:
  const ::e8::TextureProto& _internal_radiosity() const;
  ::e8::TextureProto* _internal_mutable_radiosity();
  public:
  void unsafe_arena_set_allocated_radiosity(
      ::e8::TextureProto* radiosity);
  ::e8::TextureProto* unsafe_arena_release_radiosity();

  // int64 id = 1;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::int64 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // @@protoc_insertion_point(class_scope:e8.LightMapProto)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::e8::TextureProto* radiosity_;
  ::PROTOBUF_NAMESPACE_ID::int64 id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_light_5fmap_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LightMapProto

// int64 id = 1;
inline void LightMapProto::clear_id() {
  id_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 LightMapProto::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 LightMapProto::id() const {
  // @@protoc_insertion_point(field_get:e8.LightMapProto.id)
  return _internal_id();
}
inline void LightMapProto::_internal_set_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  id_ = value;
}
inline void LightMapProto::set_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:e8.LightMapProto.id)
}

// .e8.TextureProto radiosity = 2;
inline bool LightMapProto::_internal_has_radiosity() const {
  return this != internal_default_instance() && radiosity_ != nullptr;
}
inline bool LightMapProto::has_radiosity() const {
  return _internal_has_radiosity();
}
inline const ::e8::TextureProto& LightMapProto::_internal_radiosity() const {
  const ::e8::TextureProto* p = radiosity_;
  return p != nullptr ? *p : *reinterpret_cast<const ::e8::TextureProto*>(
      &::e8::_TextureProto_default_instance_);
}
inline const ::e8::TextureProto& LightMapProto::radiosity() const {
  // @@protoc_insertion_point(field_get:e8.LightMapProto.radiosity)
  return _internal_radiosity();
}
inline void LightMapProto::unsafe_arena_set_allocated_radiosity(
    ::e8::TextureProto* radiosity) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(radiosity_);
  }
  radiosity_ = radiosity;
  if (radiosity) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.LightMapProto.radiosity)
}
inline ::e8::TextureProto* LightMapProto::release_radiosity() {
  auto temp = unsafe_arena_release_radiosity();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::e8::TextureProto* LightMapProto::unsafe_arena_release_radiosity() {
  // @@protoc_insertion_point(field_release:e8.LightMapProto.radiosity)
  
  ::e8::TextureProto* temp = radiosity_;
  radiosity_ = nullptr;
  return temp;
}
inline ::e8::TextureProto* LightMapProto::_internal_mutable_radiosity() {
  
  if (radiosity_ == nullptr) {
    auto* p = CreateMaybeMessage<::e8::TextureProto>(GetArena());
    radiosity_ = p;
  }
  return radiosity_;
}
inline ::e8::TextureProto* LightMapProto::mutable_radiosity() {
  // @@protoc_insertion_point(field_mutable:e8.LightMapProto.radiosity)
  return _internal_mutable_radiosity();
}
inline void LightMapProto::set_allocated_radiosity(::e8::TextureProto* radiosity) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(radiosity_);
  }
  if (radiosity) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(radiosity)->GetArena();
    if (message_arena != submessage_arena) {
      radiosity = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, radiosity, submessage_arena);
    }
    
  } else {
    
  }
  radiosity_ = radiosity;
  // @@protoc_insertion_point(field_set_allocated:e8.LightMapProto.radiosity)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace e8

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_light_5fmap_2eproto
