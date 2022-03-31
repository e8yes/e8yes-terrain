// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: primitive.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_primitive_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_primitive_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_primitive_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_primitive_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_primitive_2eproto;
namespace e8 {
class PrimitiveIndicesProto;
class PrimitiveIndicesProtoDefaultTypeInternal;
extern PrimitiveIndicesProtoDefaultTypeInternal _PrimitiveIndicesProto_default_instance_;
class PrimitiveVertexProto;
class PrimitiveVertexProtoDefaultTypeInternal;
extern PrimitiveVertexProtoDefaultTypeInternal _PrimitiveVertexProto_default_instance_;
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> ::e8::PrimitiveIndicesProto* Arena::CreateMaybeMessage<::e8::PrimitiveIndicesProto>(Arena*);
template<> ::e8::PrimitiveVertexProto* Arena::CreateMaybeMessage<::e8::PrimitiveVertexProto>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace e8 {

// ===================================================================

class PrimitiveVertexProto PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.PrimitiveVertexProto) */ {
 public:
  inline PrimitiveVertexProto() : PrimitiveVertexProto(nullptr) {};
  virtual ~PrimitiveVertexProto();

  PrimitiveVertexProto(const PrimitiveVertexProto& from);
  PrimitiveVertexProto(PrimitiveVertexProto&& from) noexcept
    : PrimitiveVertexProto() {
    *this = ::std::move(from);
  }

  inline PrimitiveVertexProto& operator=(const PrimitiveVertexProto& from) {
    CopyFrom(from);
    return *this;
  }
  inline PrimitiveVertexProto& operator=(PrimitiveVertexProto&& from) noexcept {
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
  static const PrimitiveVertexProto& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PrimitiveVertexProto* internal_default_instance() {
    return reinterpret_cast<const PrimitiveVertexProto*>(
               &_PrimitiveVertexProto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(PrimitiveVertexProto& a, PrimitiveVertexProto& b) {
    a.Swap(&b);
  }
  inline void Swap(PrimitiveVertexProto* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PrimitiveVertexProto* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline PrimitiveVertexProto* New() const final {
    return CreateMaybeMessage<PrimitiveVertexProto>(nullptr);
  }

  PrimitiveVertexProto* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<PrimitiveVertexProto>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const PrimitiveVertexProto& from);
  void MergeFrom(const PrimitiveVertexProto& from);
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
  void InternalSwap(PrimitiveVertexProto* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.PrimitiveVertexProto";
  }
  protected:
  explicit PrimitiveVertexProto(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_primitive_2eproto);
    return ::descriptor_table_primitive_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kPositionFieldNumber = 1,
    kNormalFieldNumber = 2,
    kTangentFieldNumber = 3,
    kTexcoordFieldNumber = 4,
  };
  // repeated float position = 1;
  int position_size() const;
  private:
  int _internal_position_size() const;
  public:
  void clear_position();
  private:
  float _internal_position(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      _internal_position() const;
  void _internal_add_position(float value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      _internal_mutable_position();
  public:
  float position(int index) const;
  void set_position(int index, float value);
  void add_position(float value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      position() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      mutable_position();

  // repeated float normal = 2;
  int normal_size() const;
  private:
  int _internal_normal_size() const;
  public:
  void clear_normal();
  private:
  float _internal_normal(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      _internal_normal() const;
  void _internal_add_normal(float value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      _internal_mutable_normal();
  public:
  float normal(int index) const;
  void set_normal(int index, float value);
  void add_normal(float value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      normal() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      mutable_normal();

  // repeated float tangent = 3;
  int tangent_size() const;
  private:
  int _internal_tangent_size() const;
  public:
  void clear_tangent();
  private:
  float _internal_tangent(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      _internal_tangent() const;
  void _internal_add_tangent(float value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      _internal_mutable_tangent();
  public:
  float tangent(int index) const;
  void set_tangent(int index, float value);
  void add_tangent(float value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      tangent() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      mutable_tangent();

  // repeated float texcoord = 4;
  int texcoord_size() const;
  private:
  int _internal_texcoord_size() const;
  public:
  void clear_texcoord();
  private:
  float _internal_texcoord(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      _internal_texcoord() const;
  void _internal_add_texcoord(float value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      _internal_mutable_texcoord();
  public:
  float texcoord(int index) const;
  void set_texcoord(int index, float value);
  void add_texcoord(float value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      texcoord() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      mutable_texcoord();

  // @@protoc_insertion_point(class_scope:e8.PrimitiveVertexProto)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > position_;
  mutable std::atomic<int> _position_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > normal_;
  mutable std::atomic<int> _normal_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > tangent_;
  mutable std::atomic<int> _tangent_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > texcoord_;
  mutable std::atomic<int> _texcoord_cached_byte_size_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_primitive_2eproto;
};
// -------------------------------------------------------------------

class PrimitiveIndicesProto PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.PrimitiveIndicesProto) */ {
 public:
  inline PrimitiveIndicesProto() : PrimitiveIndicesProto(nullptr) {};
  virtual ~PrimitiveIndicesProto();

  PrimitiveIndicesProto(const PrimitiveIndicesProto& from);
  PrimitiveIndicesProto(PrimitiveIndicesProto&& from) noexcept
    : PrimitiveIndicesProto() {
    *this = ::std::move(from);
  }

  inline PrimitiveIndicesProto& operator=(const PrimitiveIndicesProto& from) {
    CopyFrom(from);
    return *this;
  }
  inline PrimitiveIndicesProto& operator=(PrimitiveIndicesProto&& from) noexcept {
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
  static const PrimitiveIndicesProto& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PrimitiveIndicesProto* internal_default_instance() {
    return reinterpret_cast<const PrimitiveIndicesProto*>(
               &_PrimitiveIndicesProto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(PrimitiveIndicesProto& a, PrimitiveIndicesProto& b) {
    a.Swap(&b);
  }
  inline void Swap(PrimitiveIndicesProto* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PrimitiveIndicesProto* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline PrimitiveIndicesProto* New() const final {
    return CreateMaybeMessage<PrimitiveIndicesProto>(nullptr);
  }

  PrimitiveIndicesProto* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<PrimitiveIndicesProto>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const PrimitiveIndicesProto& from);
  void MergeFrom(const PrimitiveIndicesProto& from);
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
  void InternalSwap(PrimitiveIndicesProto* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.PrimitiveIndicesProto";
  }
  protected:
  explicit PrimitiveIndicesProto(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_primitive_2eproto);
    return ::descriptor_table_primitive_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIndicesFieldNumber = 1,
  };
  // repeated int32 indices = 1;
  int indices_size() const;
  private:
  int _internal_indices_size() const;
  public:
  void clear_indices();
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_indices(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
      _internal_indices() const;
  void _internal_add_indices(::PROTOBUF_NAMESPACE_ID::int32 value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
      _internal_mutable_indices();
  public:
  ::PROTOBUF_NAMESPACE_ID::int32 indices(int index) const;
  void set_indices(int index, ::PROTOBUF_NAMESPACE_ID::int32 value);
  void add_indices(::PROTOBUF_NAMESPACE_ID::int32 value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
      indices() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
      mutable_indices();

  // @@protoc_insertion_point(class_scope:e8.PrimitiveIndicesProto)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 > indices_;
  mutable std::atomic<int> _indices_cached_byte_size_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_primitive_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// PrimitiveVertexProto

// repeated float position = 1;
inline int PrimitiveVertexProto::_internal_position_size() const {
  return position_.size();
}
inline int PrimitiveVertexProto::position_size() const {
  return _internal_position_size();
}
inline void PrimitiveVertexProto::clear_position() {
  position_.Clear();
}
inline float PrimitiveVertexProto::_internal_position(int index) const {
  return position_.Get(index);
}
inline float PrimitiveVertexProto::position(int index) const {
  // @@protoc_insertion_point(field_get:e8.PrimitiveVertexProto.position)
  return _internal_position(index);
}
inline void PrimitiveVertexProto::set_position(int index, float value) {
  position_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.PrimitiveVertexProto.position)
}
inline void PrimitiveVertexProto::_internal_add_position(float value) {
  position_.Add(value);
}
inline void PrimitiveVertexProto::add_position(float value) {
  _internal_add_position(value);
  // @@protoc_insertion_point(field_add:e8.PrimitiveVertexProto.position)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
PrimitiveVertexProto::_internal_position() const {
  return position_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
PrimitiveVertexProto::position() const {
  // @@protoc_insertion_point(field_list:e8.PrimitiveVertexProto.position)
  return _internal_position();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
PrimitiveVertexProto::_internal_mutable_position() {
  return &position_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
PrimitiveVertexProto::mutable_position() {
  // @@protoc_insertion_point(field_mutable_list:e8.PrimitiveVertexProto.position)
  return _internal_mutable_position();
}

// repeated float normal = 2;
inline int PrimitiveVertexProto::_internal_normal_size() const {
  return normal_.size();
}
inline int PrimitiveVertexProto::normal_size() const {
  return _internal_normal_size();
}
inline void PrimitiveVertexProto::clear_normal() {
  normal_.Clear();
}
inline float PrimitiveVertexProto::_internal_normal(int index) const {
  return normal_.Get(index);
}
inline float PrimitiveVertexProto::normal(int index) const {
  // @@protoc_insertion_point(field_get:e8.PrimitiveVertexProto.normal)
  return _internal_normal(index);
}
inline void PrimitiveVertexProto::set_normal(int index, float value) {
  normal_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.PrimitiveVertexProto.normal)
}
inline void PrimitiveVertexProto::_internal_add_normal(float value) {
  normal_.Add(value);
}
inline void PrimitiveVertexProto::add_normal(float value) {
  _internal_add_normal(value);
  // @@protoc_insertion_point(field_add:e8.PrimitiveVertexProto.normal)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
PrimitiveVertexProto::_internal_normal() const {
  return normal_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
PrimitiveVertexProto::normal() const {
  // @@protoc_insertion_point(field_list:e8.PrimitiveVertexProto.normal)
  return _internal_normal();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
PrimitiveVertexProto::_internal_mutable_normal() {
  return &normal_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
PrimitiveVertexProto::mutable_normal() {
  // @@protoc_insertion_point(field_mutable_list:e8.PrimitiveVertexProto.normal)
  return _internal_mutable_normal();
}

// repeated float tangent = 3;
inline int PrimitiveVertexProto::_internal_tangent_size() const {
  return tangent_.size();
}
inline int PrimitiveVertexProto::tangent_size() const {
  return _internal_tangent_size();
}
inline void PrimitiveVertexProto::clear_tangent() {
  tangent_.Clear();
}
inline float PrimitiveVertexProto::_internal_tangent(int index) const {
  return tangent_.Get(index);
}
inline float PrimitiveVertexProto::tangent(int index) const {
  // @@protoc_insertion_point(field_get:e8.PrimitiveVertexProto.tangent)
  return _internal_tangent(index);
}
inline void PrimitiveVertexProto::set_tangent(int index, float value) {
  tangent_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.PrimitiveVertexProto.tangent)
}
inline void PrimitiveVertexProto::_internal_add_tangent(float value) {
  tangent_.Add(value);
}
inline void PrimitiveVertexProto::add_tangent(float value) {
  _internal_add_tangent(value);
  // @@protoc_insertion_point(field_add:e8.PrimitiveVertexProto.tangent)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
PrimitiveVertexProto::_internal_tangent() const {
  return tangent_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
PrimitiveVertexProto::tangent() const {
  // @@protoc_insertion_point(field_list:e8.PrimitiveVertexProto.tangent)
  return _internal_tangent();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
PrimitiveVertexProto::_internal_mutable_tangent() {
  return &tangent_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
PrimitiveVertexProto::mutable_tangent() {
  // @@protoc_insertion_point(field_mutable_list:e8.PrimitiveVertexProto.tangent)
  return _internal_mutable_tangent();
}

// repeated float texcoord = 4;
inline int PrimitiveVertexProto::_internal_texcoord_size() const {
  return texcoord_.size();
}
inline int PrimitiveVertexProto::texcoord_size() const {
  return _internal_texcoord_size();
}
inline void PrimitiveVertexProto::clear_texcoord() {
  texcoord_.Clear();
}
inline float PrimitiveVertexProto::_internal_texcoord(int index) const {
  return texcoord_.Get(index);
}
inline float PrimitiveVertexProto::texcoord(int index) const {
  // @@protoc_insertion_point(field_get:e8.PrimitiveVertexProto.texcoord)
  return _internal_texcoord(index);
}
inline void PrimitiveVertexProto::set_texcoord(int index, float value) {
  texcoord_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.PrimitiveVertexProto.texcoord)
}
inline void PrimitiveVertexProto::_internal_add_texcoord(float value) {
  texcoord_.Add(value);
}
inline void PrimitiveVertexProto::add_texcoord(float value) {
  _internal_add_texcoord(value);
  // @@protoc_insertion_point(field_add:e8.PrimitiveVertexProto.texcoord)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
PrimitiveVertexProto::_internal_texcoord() const {
  return texcoord_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
PrimitiveVertexProto::texcoord() const {
  // @@protoc_insertion_point(field_list:e8.PrimitiveVertexProto.texcoord)
  return _internal_texcoord();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
PrimitiveVertexProto::_internal_mutable_texcoord() {
  return &texcoord_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
PrimitiveVertexProto::mutable_texcoord() {
  // @@protoc_insertion_point(field_mutable_list:e8.PrimitiveVertexProto.texcoord)
  return _internal_mutable_texcoord();
}

// -------------------------------------------------------------------

// PrimitiveIndicesProto

// repeated int32 indices = 1;
inline int PrimitiveIndicesProto::_internal_indices_size() const {
  return indices_.size();
}
inline int PrimitiveIndicesProto::indices_size() const {
  return _internal_indices_size();
}
inline void PrimitiveIndicesProto::clear_indices() {
  indices_.Clear();
}
inline ::PROTOBUF_NAMESPACE_ID::int32 PrimitiveIndicesProto::_internal_indices(int index) const {
  return indices_.Get(index);
}
inline ::PROTOBUF_NAMESPACE_ID::int32 PrimitiveIndicesProto::indices(int index) const {
  // @@protoc_insertion_point(field_get:e8.PrimitiveIndicesProto.indices)
  return _internal_indices(index);
}
inline void PrimitiveIndicesProto::set_indices(int index, ::PROTOBUF_NAMESPACE_ID::int32 value) {
  indices_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.PrimitiveIndicesProto.indices)
}
inline void PrimitiveIndicesProto::_internal_add_indices(::PROTOBUF_NAMESPACE_ID::int32 value) {
  indices_.Add(value);
}
inline void PrimitiveIndicesProto::add_indices(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_add_indices(value);
  // @@protoc_insertion_point(field_add:e8.PrimitiveIndicesProto.indices)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
PrimitiveIndicesProto::_internal_indices() const {
  return indices_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
PrimitiveIndicesProto::indices() const {
  // @@protoc_insertion_point(field_list:e8.PrimitiveIndicesProto.indices)
  return _internal_indices();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
PrimitiveIndicesProto::_internal_mutable_indices() {
  return &indices_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
PrimitiveIndicesProto::mutable_indices() {
  // @@protoc_insertion_point(field_mutable_list:e8.PrimitiveIndicesProto.indices)
  return _internal_mutable_indices();
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace e8

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_primitive_2eproto
