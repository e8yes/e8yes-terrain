// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: geometry.proto

#include "geometry.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_geometry_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<2> scc_info_GeometryProto_geometry_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_primitive_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_PrimitiveIndicesProto_primitive_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_primitive_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_PrimitiveVertexProto_primitive_2eproto;
namespace e8 {
class GeometryProtoDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<GeometryProto> _instance;
} _GeometryProto_default_instance_;
class GeometryLodProtoDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<GeometryLodProto> _instance;
} _GeometryLodProto_default_instance_;
}  // namespace e8
static void InitDefaultsscc_info_GeometryLodProto_geometry_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_GeometryLodProto_default_instance_;
    new (ptr) ::e8::GeometryLodProto();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::GeometryLodProto::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_GeometryLodProto_geometry_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_GeometryLodProto_geometry_2eproto}, {
      &scc_info_GeometryProto_geometry_2eproto.base,}};

static void InitDefaultsscc_info_GeometryProto_geometry_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_GeometryProto_default_instance_;
    new (ptr) ::e8::GeometryProto();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::GeometryProto::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<2> scc_info_GeometryProto_geometry_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 2, 0, InitDefaultsscc_info_GeometryProto_geometry_2eproto}, {
      &scc_info_PrimitiveVertexProto_primitive_2eproto.base,
      &scc_info_PrimitiveIndicesProto_primitive_2eproto.base,}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_geometry_2eproto[2];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_geometry_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_geometry_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_geometry_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::e8::GeometryProto, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::e8::GeometryProto, rigidity_),
  PROTOBUF_FIELD_OFFSET(::e8::GeometryProto, vertices_),
  PROTOBUF_FIELD_OFFSET(::e8::GeometryProto, primitives_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::e8::GeometryLodProto, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::e8::GeometryLodProto, id_),
  PROTOBUF_FIELD_OFFSET(::e8::GeometryLodProto, name_),
  PROTOBUF_FIELD_OFFSET(::e8::GeometryLodProto, geometry_lod_),
  PROTOBUF_FIELD_OFFSET(::e8::GeometryLodProto, min_distances_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::e8::GeometryProto)},
  { 8, -1, sizeof(::e8::GeometryLodProto)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_GeometryProto_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_GeometryLodProto_default_instance_),
};

const char descriptor_table_protodef_geometry_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016geometry.proto\022\002e8\032\017primitive.proto\"\356\001"
  "\n\rGeometryProto\0220\n\010rigidity\030\001 \001(\0162\036.e8.G"
  "eometryProto.RigidityType\022*\n\010vertices\030\002 "
  "\003(\0132\030.e8.PrimitiveVertexProto\022-\n\nprimiti"
  "ves\030\003 \003(\0132\031.e8.PrimitiveIndicesProto\"P\n\014"
  "RigidityType\022\013\n\007INVALID\020\000\022\n\n\006STATIC\020\001\022\t\n"
  "\005RIGID\020\002\022\016\n\nDEFORMABLE\020\003\022\014\n\010TEARABLE\020\004\"l"
  "\n\020GeometryLodProto\022\n\n\002id\030\001 \001(\003\022\014\n\004name\030\002"
  " \001(\t\022\'\n\014geometry_lod\030\003 \003(\0132\021.e8.Geometry"
  "Proto\022\025\n\rmin_distances\030\004 \003(\002b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_geometry_2eproto_deps[1] = {
  &::descriptor_table_primitive_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_geometry_2eproto_sccs[2] = {
  &scc_info_GeometryLodProto_geometry_2eproto.base,
  &scc_info_GeometryProto_geometry_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_geometry_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_geometry_2eproto = {
  false, false, descriptor_table_protodef_geometry_2eproto, "geometry.proto", 396,
  &descriptor_table_geometry_2eproto_once, descriptor_table_geometry_2eproto_sccs, descriptor_table_geometry_2eproto_deps, 2, 1,
  schemas, file_default_instances, TableStruct_geometry_2eproto::offsets,
  file_level_metadata_geometry_2eproto, 2, file_level_enum_descriptors_geometry_2eproto, file_level_service_descriptors_geometry_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_geometry_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_geometry_2eproto)), true);
namespace e8 {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* GeometryProto_RigidityType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_geometry_2eproto);
  return file_level_enum_descriptors_geometry_2eproto[0];
}
bool GeometryProto_RigidityType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr GeometryProto_RigidityType GeometryProto::INVALID;
constexpr GeometryProto_RigidityType GeometryProto::STATIC;
constexpr GeometryProto_RigidityType GeometryProto::RIGID;
constexpr GeometryProto_RigidityType GeometryProto::DEFORMABLE;
constexpr GeometryProto_RigidityType GeometryProto::TEARABLE;
constexpr GeometryProto_RigidityType GeometryProto::RigidityType_MIN;
constexpr GeometryProto_RigidityType GeometryProto::RigidityType_MAX;
constexpr int GeometryProto::RigidityType_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void GeometryProto::InitAsDefaultInstance() {
}
class GeometryProto::_Internal {
 public:
};

void GeometryProto::clear_vertices() {
  vertices_.Clear();
}
void GeometryProto::clear_primitives() {
  primitives_.Clear();
}
GeometryProto::GeometryProto(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  vertices_(arena),
  primitives_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.GeometryProto)
}
GeometryProto::GeometryProto(const GeometryProto& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      vertices_(from.vertices_),
      primitives_(from.primitives_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  rigidity_ = from.rigidity_;
  // @@protoc_insertion_point(copy_constructor:e8.GeometryProto)
}

void GeometryProto::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_GeometryProto_geometry_2eproto.base);
  rigidity_ = 0;
}

GeometryProto::~GeometryProto() {
  // @@protoc_insertion_point(destructor:e8.GeometryProto)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void GeometryProto::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void GeometryProto::ArenaDtor(void* object) {
  GeometryProto* _this = reinterpret_cast< GeometryProto* >(object);
  (void)_this;
}
void GeometryProto::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void GeometryProto::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const GeometryProto& GeometryProto::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_GeometryProto_geometry_2eproto.base);
  return *internal_default_instance();
}


void GeometryProto::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.GeometryProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  vertices_.Clear();
  primitives_.Clear();
  rigidity_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* GeometryProto::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // .e8.GeometryProto.RigidityType rigidity = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_rigidity(static_cast<::e8::GeometryProto_RigidityType>(val));
        } else goto handle_unusual;
        continue;
      // repeated .e8.PrimitiveVertexProto vertices = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_vertices(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
        } else goto handle_unusual;
        continue;
      // repeated .e8.PrimitiveIndicesProto primitives = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_primitives(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<26>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* GeometryProto::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.GeometryProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .e8.GeometryProto.RigidityType rigidity = 1;
  if (this->rigidity() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_rigidity(), target);
  }

  // repeated .e8.PrimitiveVertexProto vertices = 2;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_vertices_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(2, this->_internal_vertices(i), target, stream);
  }

  // repeated .e8.PrimitiveIndicesProto primitives = 3;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_primitives_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(3, this->_internal_primitives(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.GeometryProto)
  return target;
}

size_t GeometryProto::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.GeometryProto)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .e8.PrimitiveVertexProto vertices = 2;
  total_size += 1UL * this->_internal_vertices_size();
  for (const auto& msg : this->vertices_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // repeated .e8.PrimitiveIndicesProto primitives = 3;
  total_size += 1UL * this->_internal_primitives_size();
  for (const auto& msg : this->primitives_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // .e8.GeometryProto.RigidityType rigidity = 1;
  if (this->rigidity() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_rigidity());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void GeometryProto::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.GeometryProto)
  GOOGLE_DCHECK_NE(&from, this);
  const GeometryProto* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<GeometryProto>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.GeometryProto)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.GeometryProto)
    MergeFrom(*source);
  }
}

void GeometryProto::MergeFrom(const GeometryProto& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.GeometryProto)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  vertices_.MergeFrom(from.vertices_);
  primitives_.MergeFrom(from.primitives_);
  if (from.rigidity() != 0) {
    _internal_set_rigidity(from._internal_rigidity());
  }
}

void GeometryProto::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.GeometryProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void GeometryProto::CopyFrom(const GeometryProto& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.GeometryProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GeometryProto::IsInitialized() const {
  return true;
}

void GeometryProto::InternalSwap(GeometryProto* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  vertices_.InternalSwap(&other->vertices_);
  primitives_.InternalSwap(&other->primitives_);
  swap(rigidity_, other->rigidity_);
}

::PROTOBUF_NAMESPACE_ID::Metadata GeometryProto::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void GeometryLodProto::InitAsDefaultInstance() {
}
class GeometryLodProto::_Internal {
 public:
};

GeometryLodProto::GeometryLodProto(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  geometry_lod_(arena),
  min_distances_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.GeometryLodProto)
}
GeometryLodProto::GeometryLodProto(const GeometryLodProto& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      geometry_lod_(from.geometry_lod_),
      min_distances_(from.min_distances_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_name().empty()) {
    name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_name(),
      GetArena());
  }
  id_ = from.id_;
  // @@protoc_insertion_point(copy_constructor:e8.GeometryLodProto)
}

void GeometryLodProto::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_GeometryLodProto_geometry_2eproto.base);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  id_ = PROTOBUF_LONGLONG(0);
}

GeometryLodProto::~GeometryLodProto() {
  // @@protoc_insertion_point(destructor:e8.GeometryLodProto)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void GeometryLodProto::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void GeometryLodProto::ArenaDtor(void* object) {
  GeometryLodProto* _this = reinterpret_cast< GeometryLodProto* >(object);
  (void)_this;
}
void GeometryLodProto::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void GeometryLodProto::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const GeometryLodProto& GeometryLodProto::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_GeometryLodProto_geometry_2eproto.base);
  return *internal_default_instance();
}


void GeometryLodProto::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.GeometryLodProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  geometry_lod_.Clear();
  min_distances_.Clear();
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  id_ = PROTOBUF_LONGLONG(0);
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* GeometryLodProto::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int64 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string name = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_name();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "e8.GeometryLodProto.name"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated .e8.GeometryProto geometry_lod = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_geometry_lod(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<26>(ptr));
        } else goto handle_unusual;
        continue;
      // repeated float min_distances = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedFloatParser(_internal_mutable_min_distances(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 37) {
          _internal_add_min_distances(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr));
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* GeometryLodProto::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.GeometryLodProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 id = 1;
  if (this->id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(1, this->_internal_id(), target);
  }

  // string name = 2;
  if (this->name().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_name().data(), static_cast<int>(this->_internal_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "e8.GeometryLodProto.name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_name(), target);
  }

  // repeated .e8.GeometryProto geometry_lod = 3;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_geometry_lod_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(3, this->_internal_geometry_lod(i), target, stream);
  }

  // repeated float min_distances = 4;
  if (this->_internal_min_distances_size() > 0) {
    target = stream->WriteFixedPacked(4, _internal_min_distances(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.GeometryLodProto)
  return target;
}

size_t GeometryLodProto::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.GeometryLodProto)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .e8.GeometryProto geometry_lod = 3;
  total_size += 1UL * this->_internal_geometry_lod_size();
  for (const auto& msg : this->geometry_lod_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // repeated float min_distances = 4;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_min_distances_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _min_distances_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // string name = 2;
  if (this->name().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // int64 id = 1;
  if (this->id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_id());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void GeometryLodProto::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.GeometryLodProto)
  GOOGLE_DCHECK_NE(&from, this);
  const GeometryLodProto* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<GeometryLodProto>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.GeometryLodProto)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.GeometryLodProto)
    MergeFrom(*source);
  }
}

void GeometryLodProto::MergeFrom(const GeometryLodProto& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.GeometryLodProto)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  geometry_lod_.MergeFrom(from.geometry_lod_);
  min_distances_.MergeFrom(from.min_distances_);
  if (from.name().size() > 0) {
    _internal_set_name(from._internal_name());
  }
  if (from.id() != 0) {
    _internal_set_id(from._internal_id());
  }
}

void GeometryLodProto::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.GeometryLodProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void GeometryLodProto::CopyFrom(const GeometryLodProto& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.GeometryLodProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GeometryLodProto::IsInitialized() const {
  return true;
}

void GeometryLodProto::InternalSwap(GeometryLodProto* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  geometry_lod_.InternalSwap(&other->geometry_lod_);
  min_distances_.InternalSwap(&other->min_distances_);
  name_.Swap(&other->name_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  swap(id_, other->id_);
}

::PROTOBUF_NAMESPACE_ID::Metadata GeometryLodProto::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::e8::GeometryProto* Arena::CreateMaybeMessage< ::e8::GeometryProto >(Arena* arena) {
  return Arena::CreateMessageInternal< ::e8::GeometryProto >(arena);
}
template<> PROTOBUF_NOINLINE ::e8::GeometryLodProto* Arena::CreateMaybeMessage< ::e8::GeometryLodProto >(Arena* arena) {
  return Arena::CreateMessageInternal< ::e8::GeometryLodProto >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
