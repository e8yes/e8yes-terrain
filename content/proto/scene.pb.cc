// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: scene.proto

#include "scene.pb.h"

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
extern PROTOBUF_INTERNAL_EXPORT_camera_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_Camera_camera_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_procedural_5fobject_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_ProceduralObjectProto_procedural_5fobject_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_scene_5fobject_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_SceneObjectProto_scene_5fobject_2eproto;
namespace e8 {
class SceneProtoDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<SceneProto> _instance;
} _SceneProto_default_instance_;
}  // namespace e8
static void InitDefaultsscc_info_SceneProto_scene_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_SceneProto_default_instance_;
    new (ptr) ::e8::SceneProto();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::SceneProto::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<3> scc_info_SceneProto_scene_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 3, 0, InitDefaultsscc_info_SceneProto_scene_2eproto}, {
      &scc_info_Camera_camera_2eproto.base,
      &scc_info_SceneObjectProto_scene_5fobject_2eproto.base,
      &scc_info_ProceduralObjectProto_procedural_5fobject_2eproto.base,}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_scene_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_scene_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_scene_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_scene_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, id_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, name_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, structure_type_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, background_color_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, camera_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, scene_objects_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, precedural_objects_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::e8::SceneProto)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_SceneProto_default_instance_),
};

const char descriptor_table_protodef_scene_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013scene.proto\022\002e8\032\014camera.proto\032\027procedu"
  "ral_object.proto\032\022scene_object.proto\"\254\002\n"
  "\nSceneProto\022\n\n\002id\030\001 \001(\003\022\014\n\004name\030\002 \001(\t\0224\n"
  "\016structure_type\030\003 \001(\0162\034.e8.SceneProto.St"
  "ructureType\022\030\n\020background_color\030\004 \003(\002\022\032\n"
  "\006camera\030\005 \001(\0132\n.e8.Camera\022+\n\rscene_objec"
  "ts\030\006 \003(\0132\024.e8.SceneObjectProto\0225\n\022preced"
  "ural_objects\030\007 \003(\0132\031.e8.ProceduralObject"
  "Proto\"4\n\rStructureType\022\013\n\007INVALID\020\000\022\n\n\006L"
  "INEAR\020\001\022\n\n\006OCTREE\020\002b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_scene_2eproto_deps[3] = {
  &::descriptor_table_camera_2eproto,
  &::descriptor_table_procedural_5fobject_2eproto,
  &::descriptor_table_scene_5fobject_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_scene_2eproto_sccs[1] = {
  &scc_info_SceneProto_scene_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_scene_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_scene_2eproto = {
  false, false, descriptor_table_protodef_scene_2eproto, "scene.proto", 387,
  &descriptor_table_scene_2eproto_once, descriptor_table_scene_2eproto_sccs, descriptor_table_scene_2eproto_deps, 1, 3,
  schemas, file_default_instances, TableStruct_scene_2eproto::offsets,
  file_level_metadata_scene_2eproto, 1, file_level_enum_descriptors_scene_2eproto, file_level_service_descriptors_scene_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_scene_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_scene_2eproto)), true);
namespace e8 {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* SceneProto_StructureType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_scene_2eproto);
  return file_level_enum_descriptors_scene_2eproto[0];
}
bool SceneProto_StructureType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr SceneProto_StructureType SceneProto::INVALID;
constexpr SceneProto_StructureType SceneProto::LINEAR;
constexpr SceneProto_StructureType SceneProto::OCTREE;
constexpr SceneProto_StructureType SceneProto::StructureType_MIN;
constexpr SceneProto_StructureType SceneProto::StructureType_MAX;
constexpr int SceneProto::StructureType_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void SceneProto::InitAsDefaultInstance() {
  ::e8::_SceneProto_default_instance_._instance.get_mutable()->camera_ = const_cast< ::e8::Camera*>(
      ::e8::Camera::internal_default_instance());
}
class SceneProto::_Internal {
 public:
  static const ::e8::Camera& camera(const SceneProto* msg);
};

const ::e8::Camera&
SceneProto::_Internal::camera(const SceneProto* msg) {
  return *msg->camera_;
}
void SceneProto::clear_camera() {
  if (GetArena() == nullptr && camera_ != nullptr) {
    delete camera_;
  }
  camera_ = nullptr;
}
void SceneProto::clear_scene_objects() {
  scene_objects_.Clear();
}
void SceneProto::clear_precedural_objects() {
  precedural_objects_.Clear();
}
SceneProto::SceneProto(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  background_color_(arena),
  scene_objects_(arena),
  precedural_objects_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.SceneProto)
}
SceneProto::SceneProto(const SceneProto& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      background_color_(from.background_color_),
      scene_objects_(from.scene_objects_),
      precedural_objects_(from.precedural_objects_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_name().empty()) {
    name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_name(),
      GetArena());
  }
  if (from._internal_has_camera()) {
    camera_ = new ::e8::Camera(*from.camera_);
  } else {
    camera_ = nullptr;
  }
  ::memcpy(&id_, &from.id_,
    static_cast<size_t>(reinterpret_cast<char*>(&structure_type_) -
    reinterpret_cast<char*>(&id_)) + sizeof(structure_type_));
  // @@protoc_insertion_point(copy_constructor:e8.SceneProto)
}

void SceneProto::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_SceneProto_scene_2eproto.base);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&camera_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&structure_type_) -
      reinterpret_cast<char*>(&camera_)) + sizeof(structure_type_));
}

SceneProto::~SceneProto() {
  // @@protoc_insertion_point(destructor:e8.SceneProto)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void SceneProto::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete camera_;
}

void SceneProto::ArenaDtor(void* object) {
  SceneProto* _this = reinterpret_cast< SceneProto* >(object);
  (void)_this;
}
void SceneProto::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void SceneProto::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const SceneProto& SceneProto::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_SceneProto_scene_2eproto.base);
  return *internal_default_instance();
}


void SceneProto::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.SceneProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  background_color_.Clear();
  scene_objects_.Clear();
  precedural_objects_.Clear();
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  if (GetArena() == nullptr && camera_ != nullptr) {
    delete camera_;
  }
  camera_ = nullptr;
  ::memset(&id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&structure_type_) -
      reinterpret_cast<char*>(&id_)) + sizeof(structure_type_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SceneProto::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
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
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "e8.SceneProto.name"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .e8.SceneProto.StructureType structure_type = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_structure_type(static_cast<::e8::SceneProto_StructureType>(val));
        } else goto handle_unusual;
        continue;
      // repeated float background_color = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedFloatParser(_internal_mutable_background_color(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 37) {
          _internal_add_background_color(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr));
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // .e8.Camera camera = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 42)) {
          ptr = ctx->ParseMessage(_internal_mutable_camera(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated .e8.SceneObjectProto scene_objects = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 50)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_scene_objects(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<50>(ptr));
        } else goto handle_unusual;
        continue;
      // repeated .e8.ProceduralObjectProto precedural_objects = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 58)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_precedural_objects(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<58>(ptr));
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

::PROTOBUF_NAMESPACE_ID::uint8* SceneProto::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.SceneProto)
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
      "e8.SceneProto.name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_name(), target);
  }

  // .e8.SceneProto.StructureType structure_type = 3;
  if (this->structure_type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      3, this->_internal_structure_type(), target);
  }

  // repeated float background_color = 4;
  if (this->_internal_background_color_size() > 0) {
    target = stream->WriteFixedPacked(4, _internal_background_color(), target);
  }

  // .e8.Camera camera = 5;
  if (this->has_camera()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        5, _Internal::camera(this), target, stream);
  }

  // repeated .e8.SceneObjectProto scene_objects = 6;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_scene_objects_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(6, this->_internal_scene_objects(i), target, stream);
  }

  // repeated .e8.ProceduralObjectProto precedural_objects = 7;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_precedural_objects_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(7, this->_internal_precedural_objects(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.SceneProto)
  return target;
}

size_t SceneProto::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.SceneProto)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated float background_color = 4;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_background_color_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _background_color_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // repeated .e8.SceneObjectProto scene_objects = 6;
  total_size += 1UL * this->_internal_scene_objects_size();
  for (const auto& msg : this->scene_objects_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // repeated .e8.ProceduralObjectProto precedural_objects = 7;
  total_size += 1UL * this->_internal_precedural_objects_size();
  for (const auto& msg : this->precedural_objects_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // string name = 2;
  if (this->name().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // .e8.Camera camera = 5;
  if (this->has_camera()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *camera_);
  }

  // int64 id = 1;
  if (this->id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_id());
  }

  // .e8.SceneProto.StructureType structure_type = 3;
  if (this->structure_type() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_structure_type());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void SceneProto::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.SceneProto)
  GOOGLE_DCHECK_NE(&from, this);
  const SceneProto* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<SceneProto>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.SceneProto)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.SceneProto)
    MergeFrom(*source);
  }
}

void SceneProto::MergeFrom(const SceneProto& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.SceneProto)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  background_color_.MergeFrom(from.background_color_);
  scene_objects_.MergeFrom(from.scene_objects_);
  precedural_objects_.MergeFrom(from.precedural_objects_);
  if (from.name().size() > 0) {
    _internal_set_name(from._internal_name());
  }
  if (from.has_camera()) {
    _internal_mutable_camera()->::e8::Camera::MergeFrom(from._internal_camera());
  }
  if (from.id() != 0) {
    _internal_set_id(from._internal_id());
  }
  if (from.structure_type() != 0) {
    _internal_set_structure_type(from._internal_structure_type());
  }
}

void SceneProto::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.SceneProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SceneProto::CopyFrom(const SceneProto& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.SceneProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SceneProto::IsInitialized() const {
  return true;
}

void SceneProto::InternalSwap(SceneProto* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  background_color_.InternalSwap(&other->background_color_);
  scene_objects_.InternalSwap(&other->scene_objects_);
  precedural_objects_.InternalSwap(&other->precedural_objects_);
  name_.Swap(&other->name_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(SceneProto, structure_type_)
      + sizeof(SceneProto::structure_type_)
      - PROTOBUF_FIELD_OFFSET(SceneProto, camera_)>(
          reinterpret_cast<char*>(&camera_),
          reinterpret_cast<char*>(&other->camera_));
}

::PROTOBUF_NAMESPACE_ID::Metadata SceneProto::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::e8::SceneProto* Arena::CreateMaybeMessage< ::e8::SceneProto >(Arena* arena) {
  return Arena::CreateMessageInternal< ::e8::SceneProto >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
