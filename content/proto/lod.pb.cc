// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: lod.proto

#include "lod.pb.h"

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
extern PROTOBUF_INTERNAL_EXPORT_lod_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_SceneEntityResources_Lod_lod_2eproto;
namespace e8 {
class SceneEntityResources_LodDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<SceneEntityResources_Lod> _instance;
} _SceneEntityResources_Lod_default_instance_;
class SceneEntityResourcesDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<SceneEntityResources> _instance;
} _SceneEntityResources_default_instance_;
}  // namespace e8
static void InitDefaultsscc_info_SceneEntityResources_lod_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_SceneEntityResources_default_instance_;
    new (ptr) ::e8::SceneEntityResources();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::SceneEntityResources::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_SceneEntityResources_lod_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_SceneEntityResources_lod_2eproto}, {
      &scc_info_SceneEntityResources_Lod_lod_2eproto.base,}};

static void InitDefaultsscc_info_SceneEntityResources_Lod_lod_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_SceneEntityResources_Lod_default_instance_;
    new (ptr) ::e8::SceneEntityResources_Lod();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::SceneEntityResources_Lod::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_SceneEntityResources_Lod_lod_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_SceneEntityResources_Lod_lod_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_lod_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_lod_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_lod_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_lod_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources_Lod, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources_Lod, apply_after_distance_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources_Lod, geometry_id_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources_Lod, material_id_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources_Lod, light_map_id_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources_Lod, indirect_light_map_id_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources_Lod, physical_shape_id_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityResources, lods_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::e8::SceneEntityResources_Lod)},
  { 11, -1, sizeof(::e8::SceneEntityResources)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_SceneEntityResources_Lod_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_SceneEntityResources_default_instance_),
};

const char descriptor_table_protodef_lod_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\tlod.proto\022\002e8\"\342\001\n\024SceneEntityResources"
  "\022*\n\004lods\030\001 \003(\0132\034.e8.SceneEntityResources"
  ".Lod\032\235\001\n\003Lod\022\034\n\024apply_after_distance\030\001 \001"
  "(\002\022\023\n\013geometry_id\030\002 \001(\003\022\023\n\013material_id\030\003"
  " \001(\003\022\024\n\014light_map_id\030\004 \001(\003\022\035\n\025indirect_l"
  "ight_map_id\030\005 \001(\003\022\031\n\021physical_shape_id\030\006"
  " \001(\003b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_lod_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_lod_2eproto_sccs[2] = {
  &scc_info_SceneEntityResources_lod_2eproto.base,
  &scc_info_SceneEntityResources_Lod_lod_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_lod_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_lod_2eproto = {
  false, false, descriptor_table_protodef_lod_2eproto, "lod.proto", 252,
  &descriptor_table_lod_2eproto_once, descriptor_table_lod_2eproto_sccs, descriptor_table_lod_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_lod_2eproto::offsets,
  file_level_metadata_lod_2eproto, 2, file_level_enum_descriptors_lod_2eproto, file_level_service_descriptors_lod_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_lod_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_lod_2eproto)), true);
namespace e8 {

// ===================================================================

void SceneEntityResources_Lod::InitAsDefaultInstance() {
}
class SceneEntityResources_Lod::_Internal {
 public:
};

SceneEntityResources_Lod::SceneEntityResources_Lod(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.SceneEntityResources.Lod)
}
SceneEntityResources_Lod::SceneEntityResources_Lod(const SceneEntityResources_Lod& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&geometry_id_, &from.geometry_id_,
    static_cast<size_t>(reinterpret_cast<char*>(&apply_after_distance_) -
    reinterpret_cast<char*>(&geometry_id_)) + sizeof(apply_after_distance_));
  // @@protoc_insertion_point(copy_constructor:e8.SceneEntityResources.Lod)
}

void SceneEntityResources_Lod::SharedCtor() {
  ::memset(&geometry_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&apply_after_distance_) -
      reinterpret_cast<char*>(&geometry_id_)) + sizeof(apply_after_distance_));
}

SceneEntityResources_Lod::~SceneEntityResources_Lod() {
  // @@protoc_insertion_point(destructor:e8.SceneEntityResources.Lod)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void SceneEntityResources_Lod::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void SceneEntityResources_Lod::ArenaDtor(void* object) {
  SceneEntityResources_Lod* _this = reinterpret_cast< SceneEntityResources_Lod* >(object);
  (void)_this;
}
void SceneEntityResources_Lod::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void SceneEntityResources_Lod::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const SceneEntityResources_Lod& SceneEntityResources_Lod::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_SceneEntityResources_Lod_lod_2eproto.base);
  return *internal_default_instance();
}


void SceneEntityResources_Lod::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.SceneEntityResources.Lod)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&geometry_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&apply_after_distance_) -
      reinterpret_cast<char*>(&geometry_id_)) + sizeof(apply_after_distance_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SceneEntityResources_Lod::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // float apply_after_distance = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 13)) {
          apply_after_distance_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // int64 geometry_id = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          geometry_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 material_id = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          material_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 light_map_id = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          light_map_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 indirect_light_map_id = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          indirect_light_map_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 physical_shape_id = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 48)) {
          physical_shape_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* SceneEntityResources_Lod::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.SceneEntityResources.Lod)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // float apply_after_distance = 1;
  if (!(this->apply_after_distance() <= 0 && this->apply_after_distance() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(1, this->_internal_apply_after_distance(), target);
  }

  // int64 geometry_id = 2;
  if (this->geometry_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(2, this->_internal_geometry_id(), target);
  }

  // int64 material_id = 3;
  if (this->material_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(3, this->_internal_material_id(), target);
  }

  // int64 light_map_id = 4;
  if (this->light_map_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(4, this->_internal_light_map_id(), target);
  }

  // int64 indirect_light_map_id = 5;
  if (this->indirect_light_map_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(5, this->_internal_indirect_light_map_id(), target);
  }

  // int64 physical_shape_id = 6;
  if (this->physical_shape_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(6, this->_internal_physical_shape_id(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.SceneEntityResources.Lod)
  return target;
}

size_t SceneEntityResources_Lod::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.SceneEntityResources.Lod)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int64 geometry_id = 2;
  if (this->geometry_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_geometry_id());
  }

  // int64 material_id = 3;
  if (this->material_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_material_id());
  }

  // int64 light_map_id = 4;
  if (this->light_map_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_light_map_id());
  }

  // int64 indirect_light_map_id = 5;
  if (this->indirect_light_map_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_indirect_light_map_id());
  }

  // int64 physical_shape_id = 6;
  if (this->physical_shape_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_physical_shape_id());
  }

  // float apply_after_distance = 1;
  if (!(this->apply_after_distance() <= 0 && this->apply_after_distance() >= 0)) {
    total_size += 1 + 4;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void SceneEntityResources_Lod::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.SceneEntityResources.Lod)
  GOOGLE_DCHECK_NE(&from, this);
  const SceneEntityResources_Lod* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<SceneEntityResources_Lod>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.SceneEntityResources.Lod)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.SceneEntityResources.Lod)
    MergeFrom(*source);
  }
}

void SceneEntityResources_Lod::MergeFrom(const SceneEntityResources_Lod& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.SceneEntityResources.Lod)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.geometry_id() != 0) {
    _internal_set_geometry_id(from._internal_geometry_id());
  }
  if (from.material_id() != 0) {
    _internal_set_material_id(from._internal_material_id());
  }
  if (from.light_map_id() != 0) {
    _internal_set_light_map_id(from._internal_light_map_id());
  }
  if (from.indirect_light_map_id() != 0) {
    _internal_set_indirect_light_map_id(from._internal_indirect_light_map_id());
  }
  if (from.physical_shape_id() != 0) {
    _internal_set_physical_shape_id(from._internal_physical_shape_id());
  }
  if (!(from.apply_after_distance() <= 0 && from.apply_after_distance() >= 0)) {
    _internal_set_apply_after_distance(from._internal_apply_after_distance());
  }
}

void SceneEntityResources_Lod::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.SceneEntityResources.Lod)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SceneEntityResources_Lod::CopyFrom(const SceneEntityResources_Lod& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.SceneEntityResources.Lod)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SceneEntityResources_Lod::IsInitialized() const {
  return true;
}

void SceneEntityResources_Lod::InternalSwap(SceneEntityResources_Lod* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(SceneEntityResources_Lod, apply_after_distance_)
      + sizeof(SceneEntityResources_Lod::apply_after_distance_)
      - PROTOBUF_FIELD_OFFSET(SceneEntityResources_Lod, geometry_id_)>(
          reinterpret_cast<char*>(&geometry_id_),
          reinterpret_cast<char*>(&other->geometry_id_));
}

::PROTOBUF_NAMESPACE_ID::Metadata SceneEntityResources_Lod::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void SceneEntityResources::InitAsDefaultInstance() {
}
class SceneEntityResources::_Internal {
 public:
};

SceneEntityResources::SceneEntityResources(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  lods_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.SceneEntityResources)
}
SceneEntityResources::SceneEntityResources(const SceneEntityResources& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      lods_(from.lods_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:e8.SceneEntityResources)
}

void SceneEntityResources::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_SceneEntityResources_lod_2eproto.base);
}

SceneEntityResources::~SceneEntityResources() {
  // @@protoc_insertion_point(destructor:e8.SceneEntityResources)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void SceneEntityResources::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void SceneEntityResources::ArenaDtor(void* object) {
  SceneEntityResources* _this = reinterpret_cast< SceneEntityResources* >(object);
  (void)_this;
}
void SceneEntityResources::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void SceneEntityResources::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const SceneEntityResources& SceneEntityResources::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_SceneEntityResources_lod_2eproto.base);
  return *internal_default_instance();
}


void SceneEntityResources::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.SceneEntityResources)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  lods_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SceneEntityResources::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated .e8.SceneEntityResources.Lod lods = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_lods(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
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

::PROTOBUF_NAMESPACE_ID::uint8* SceneEntityResources::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.SceneEntityResources)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .e8.SceneEntityResources.Lod lods = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_lods_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, this->_internal_lods(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.SceneEntityResources)
  return target;
}

size_t SceneEntityResources::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.SceneEntityResources)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .e8.SceneEntityResources.Lod lods = 1;
  total_size += 1UL * this->_internal_lods_size();
  for (const auto& msg : this->lods_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void SceneEntityResources::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.SceneEntityResources)
  GOOGLE_DCHECK_NE(&from, this);
  const SceneEntityResources* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<SceneEntityResources>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.SceneEntityResources)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.SceneEntityResources)
    MergeFrom(*source);
  }
}

void SceneEntityResources::MergeFrom(const SceneEntityResources& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.SceneEntityResources)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  lods_.MergeFrom(from.lods_);
}

void SceneEntityResources::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.SceneEntityResources)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SceneEntityResources::CopyFrom(const SceneEntityResources& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.SceneEntityResources)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SceneEntityResources::IsInitialized() const {
  return true;
}

void SceneEntityResources::InternalSwap(SceneEntityResources* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  lods_.InternalSwap(&other->lods_);
}

::PROTOBUF_NAMESPACE_ID::Metadata SceneEntityResources::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::e8::SceneEntityResources_Lod* Arena::CreateMaybeMessage< ::e8::SceneEntityResources_Lod >(Arena* arena) {
  return Arena::CreateMessageInternal< ::e8::SceneEntityResources_Lod >(arena);
}
template<> PROTOBUF_NOINLINE ::e8::SceneEntityResources* Arena::CreateMaybeMessage< ::e8::SceneEntityResources >(Arena* arena) {
  return Arena::CreateMessageInternal< ::e8::SceneEntityResources >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
