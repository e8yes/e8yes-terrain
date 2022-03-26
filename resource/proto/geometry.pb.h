// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: geometry.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_geometry_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_geometry_2eproto

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
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "primitive.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_geometry_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_geometry_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_geometry_2eproto;
namespace e8 {
class GeometryProto;
class GeometryProtoDefaultTypeInternal;
extern GeometryProtoDefaultTypeInternal _GeometryProto_default_instance_;
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> ::e8::GeometryProto* Arena::CreateMaybeMessage<::e8::GeometryProto>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace e8 {

enum GeometryProto_RigidityType : int {
  GeometryProto_RigidityType_INVALID = 0,
  GeometryProto_RigidityType_STATIC = 1,
  GeometryProto_RigidityType_RIGID = 2,
  GeometryProto_RigidityType_DEFORMABLE = 3,
  GeometryProto_RigidityType_TEARABLE = 4,
  GeometryProto_RigidityType_GeometryProto_RigidityType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  GeometryProto_RigidityType_GeometryProto_RigidityType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool GeometryProto_RigidityType_IsValid(int value);
constexpr GeometryProto_RigidityType GeometryProto_RigidityType_RigidityType_MIN = GeometryProto_RigidityType_INVALID;
constexpr GeometryProto_RigidityType GeometryProto_RigidityType_RigidityType_MAX = GeometryProto_RigidityType_TEARABLE;
constexpr int GeometryProto_RigidityType_RigidityType_ARRAYSIZE = GeometryProto_RigidityType_RigidityType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* GeometryProto_RigidityType_descriptor();
template<typename T>
inline const std::string& GeometryProto_RigidityType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, GeometryProto_RigidityType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function GeometryProto_RigidityType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    GeometryProto_RigidityType_descriptor(), enum_t_value);
}
inline bool GeometryProto_RigidityType_Parse(
    const std::string& name, GeometryProto_RigidityType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<GeometryProto_RigidityType>(
    GeometryProto_RigidityType_descriptor(), name, value);
}
// ===================================================================

class GeometryProto PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.GeometryProto) */ {
 public:
  inline GeometryProto() : GeometryProto(nullptr) {};
  virtual ~GeometryProto();

  GeometryProto(const GeometryProto& from);
  GeometryProto(GeometryProto&& from) noexcept
    : GeometryProto() {
    *this = ::std::move(from);
  }

  inline GeometryProto& operator=(const GeometryProto& from) {
    CopyFrom(from);
    return *this;
  }
  inline GeometryProto& operator=(GeometryProto&& from) noexcept {
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
  static const GeometryProto& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const GeometryProto* internal_default_instance() {
    return reinterpret_cast<const GeometryProto*>(
               &_GeometryProto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(GeometryProto& a, GeometryProto& b) {
    a.Swap(&b);
  }
  inline void Swap(GeometryProto* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(GeometryProto* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline GeometryProto* New() const final {
    return CreateMaybeMessage<GeometryProto>(nullptr);
  }

  GeometryProto* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<GeometryProto>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const GeometryProto& from);
  void MergeFrom(const GeometryProto& from);
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
  void InternalSwap(GeometryProto* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.GeometryProto";
  }
  protected:
  explicit GeometryProto(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_geometry_2eproto);
    return ::descriptor_table_geometry_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef GeometryProto_RigidityType RigidityType;
  static constexpr RigidityType INVALID =
    GeometryProto_RigidityType_INVALID;
  static constexpr RigidityType STATIC =
    GeometryProto_RigidityType_STATIC;
  static constexpr RigidityType RIGID =
    GeometryProto_RigidityType_RIGID;
  static constexpr RigidityType DEFORMABLE =
    GeometryProto_RigidityType_DEFORMABLE;
  static constexpr RigidityType TEARABLE =
    GeometryProto_RigidityType_TEARABLE;
  static inline bool RigidityType_IsValid(int value) {
    return GeometryProto_RigidityType_IsValid(value);
  }
  static constexpr RigidityType RigidityType_MIN =
    GeometryProto_RigidityType_RigidityType_MIN;
  static constexpr RigidityType RigidityType_MAX =
    GeometryProto_RigidityType_RigidityType_MAX;
  static constexpr int RigidityType_ARRAYSIZE =
    GeometryProto_RigidityType_RigidityType_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  RigidityType_descriptor() {
    return GeometryProto_RigidityType_descriptor();
  }
  template<typename T>
  static inline const std::string& RigidityType_Name(T enum_t_value) {
    static_assert(::std::is_same<T, RigidityType>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function RigidityType_Name.");
    return GeometryProto_RigidityType_Name(enum_t_value);
  }
  static inline bool RigidityType_Parse(const std::string& name,
      RigidityType* value) {
    return GeometryProto_RigidityType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kVerticesFieldNumber = 3,
    kPrimitivesFieldNumber = 4,
    kIdFieldNumber = 1,
    kRigidityFieldNumber = 2,
  };
  // repeated .e8.PrimitiveVertexProto vertices = 3;
  int vertices_size() const;
  private:
  int _internal_vertices_size() const;
  public:
  void clear_vertices();
  ::e8::PrimitiveVertexProto* mutable_vertices(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveVertexProto >*
      mutable_vertices();
  private:
  const ::e8::PrimitiveVertexProto& _internal_vertices(int index) const;
  ::e8::PrimitiveVertexProto* _internal_add_vertices();
  public:
  const ::e8::PrimitiveVertexProto& vertices(int index) const;
  ::e8::PrimitiveVertexProto* add_vertices();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveVertexProto >&
      vertices() const;

  // repeated .e8.PrimitiveIndicesProto primitives = 4;
  int primitives_size() const;
  private:
  int _internal_primitives_size() const;
  public:
  void clear_primitives();
  ::e8::PrimitiveIndicesProto* mutable_primitives(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveIndicesProto >*
      mutable_primitives();
  private:
  const ::e8::PrimitiveIndicesProto& _internal_primitives(int index) const;
  ::e8::PrimitiveIndicesProto* _internal_add_primitives();
  public:
  const ::e8::PrimitiveIndicesProto& primitives(int index) const;
  ::e8::PrimitiveIndicesProto* add_primitives();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveIndicesProto >&
      primitives() const;

  // int64 id = 1;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::int64 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // .e8.GeometryProto.RigidityType rigidity = 2;
  void clear_rigidity();
  ::e8::GeometryProto_RigidityType rigidity() const;
  void set_rigidity(::e8::GeometryProto_RigidityType value);
  private:
  ::e8::GeometryProto_RigidityType _internal_rigidity() const;
  void _internal_set_rigidity(::e8::GeometryProto_RigidityType value);
  public:

  // @@protoc_insertion_point(class_scope:e8.GeometryProto)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveVertexProto > vertices_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveIndicesProto > primitives_;
  ::PROTOBUF_NAMESPACE_ID::int64 id_;
  int rigidity_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_geometry_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// GeometryProto

// int64 id = 1;
inline void GeometryProto::clear_id() {
  id_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 GeometryProto::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 GeometryProto::id() const {
  // @@protoc_insertion_point(field_get:e8.GeometryProto.id)
  return _internal_id();
}
inline void GeometryProto::_internal_set_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  id_ = value;
}
inline void GeometryProto::set_id(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:e8.GeometryProto.id)
}

// .e8.GeometryProto.RigidityType rigidity = 2;
inline void GeometryProto::clear_rigidity() {
  rigidity_ = 0;
}
inline ::e8::GeometryProto_RigidityType GeometryProto::_internal_rigidity() const {
  return static_cast< ::e8::GeometryProto_RigidityType >(rigidity_);
}
inline ::e8::GeometryProto_RigidityType GeometryProto::rigidity() const {
  // @@protoc_insertion_point(field_get:e8.GeometryProto.rigidity)
  return _internal_rigidity();
}
inline void GeometryProto::_internal_set_rigidity(::e8::GeometryProto_RigidityType value) {
  
  rigidity_ = value;
}
inline void GeometryProto::set_rigidity(::e8::GeometryProto_RigidityType value) {
  _internal_set_rigidity(value);
  // @@protoc_insertion_point(field_set:e8.GeometryProto.rigidity)
}

// repeated .e8.PrimitiveVertexProto vertices = 3;
inline int GeometryProto::_internal_vertices_size() const {
  return vertices_.size();
}
inline int GeometryProto::vertices_size() const {
  return _internal_vertices_size();
}
inline ::e8::PrimitiveVertexProto* GeometryProto::mutable_vertices(int index) {
  // @@protoc_insertion_point(field_mutable:e8.GeometryProto.vertices)
  return vertices_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveVertexProto >*
GeometryProto::mutable_vertices() {
  // @@protoc_insertion_point(field_mutable_list:e8.GeometryProto.vertices)
  return &vertices_;
}
inline const ::e8::PrimitiveVertexProto& GeometryProto::_internal_vertices(int index) const {
  return vertices_.Get(index);
}
inline const ::e8::PrimitiveVertexProto& GeometryProto::vertices(int index) const {
  // @@protoc_insertion_point(field_get:e8.GeometryProto.vertices)
  return _internal_vertices(index);
}
inline ::e8::PrimitiveVertexProto* GeometryProto::_internal_add_vertices() {
  return vertices_.Add();
}
inline ::e8::PrimitiveVertexProto* GeometryProto::add_vertices() {
  // @@protoc_insertion_point(field_add:e8.GeometryProto.vertices)
  return _internal_add_vertices();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveVertexProto >&
GeometryProto::vertices() const {
  // @@protoc_insertion_point(field_list:e8.GeometryProto.vertices)
  return vertices_;
}

// repeated .e8.PrimitiveIndicesProto primitives = 4;
inline int GeometryProto::_internal_primitives_size() const {
  return primitives_.size();
}
inline int GeometryProto::primitives_size() const {
  return _internal_primitives_size();
}
inline ::e8::PrimitiveIndicesProto* GeometryProto::mutable_primitives(int index) {
  // @@protoc_insertion_point(field_mutable:e8.GeometryProto.primitives)
  return primitives_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveIndicesProto >*
GeometryProto::mutable_primitives() {
  // @@protoc_insertion_point(field_mutable_list:e8.GeometryProto.primitives)
  return &primitives_;
}
inline const ::e8::PrimitiveIndicesProto& GeometryProto::_internal_primitives(int index) const {
  return primitives_.Get(index);
}
inline const ::e8::PrimitiveIndicesProto& GeometryProto::primitives(int index) const {
  // @@protoc_insertion_point(field_get:e8.GeometryProto.primitives)
  return _internal_primitives(index);
}
inline ::e8::PrimitiveIndicesProto* GeometryProto::_internal_add_primitives() {
  return primitives_.Add();
}
inline ::e8::PrimitiveIndicesProto* GeometryProto::add_primitives() {
  // @@protoc_insertion_point(field_add:e8.GeometryProto.primitives)
  return _internal_add_primitives();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::e8::PrimitiveIndicesProto >&
GeometryProto::primitives() const {
  // @@protoc_insertion_point(field_list:e8.GeometryProto.primitives)
  return primitives_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace e8

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::e8::GeometryProto_RigidityType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::e8::GeometryProto_RigidityType>() {
  return ::e8::GeometryProto_RigidityType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_geometry_2eproto