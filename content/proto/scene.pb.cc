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
extern PROTOBUF_INTERNAL_EXPORT_entity_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<3> scc_info_SceneEntityCollection_entity_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_scene_5fobject_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<2> scc_info_SceneObject_scene_5fobject_2eproto;
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

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<2> scc_info_SceneProto_scene_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 2, 0, InitDefaultsscc_info_SceneProto_scene_2eproto}, {
      &scc_info_SceneObject_scene_5fobject_2eproto.base,
      &scc_info_SceneEntityCollection_entity_2eproto.base,}};

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
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, objects_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneProto, entities_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::e8::SceneProto)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_SceneProto_default_instance_),
};

const char descriptor_table_protodef_scene_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013scene.proto\022\002e8\032\014entity.proto\032\022scene_o"
  "bject.proto\"\373\001\n\nSceneProto\022\n\n\002id\030\001 \001(\t\022\014"
  "\n\004name\030\002 \001(\t\0224\n\016structure_type\030\003 \001(\0162\034.e"
  "8.SceneProto.StructureType\022\030\n\020background"
  "_color\030\004 \003(\002\022 \n\007objects\030\005 \003(\0132\017.e8.Scene"
  "Object\022+\n\010entities\030\006 \001(\0132\031.e8.SceneEntit"
  "yCollection\"4\n\rStructureType\022\013\n\007INVALID\020"
  "\000\022\n\n\006LINEAR\020\001\022\n\n\006OCTREE\020\002b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_scene_2eproto_deps[2] = {
  &::descriptor_table_entity_2eproto,
  &::descriptor_table_scene_5fobject_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_scene_2eproto_sccs[1] = {
  &scc_info_SceneProto_scene_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_scene_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_scene_2eproto = {
  false, false, descriptor_table_protodef_scene_2eproto, "scene.proto", 313,
  &descriptor_table_scene_2eproto_once, descriptor_table_scene_2eproto_sccs, descriptor_table_scene_2eproto_deps, 1, 2,
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
  ::e8::_SceneProto_default_instance_._instance.get_mutable()->entities_ = const_cast< ::e8::SceneEntityCollection*>(
      ::e8::SceneEntityCollection::internal_default_instance());
}
class SceneProto::_Internal {
 public:
  static const ::e8::SceneEntityCollection& entities(const SceneProto* msg);
};

const ::e8::SceneEntityCollection&
SceneProto::_Internal::entities(const SceneProto* msg) {
  return *msg->entities_;
}
void SceneProto::clear_objects() {
  objects_.Clear();
}
void SceneProto::clear_entities() {
  if (GetArena() == nullptr && entities_ != nullptr) {
    delete entities_;
  }
  entities_ = nullptr;
}
SceneProto::SceneProto(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  background_color_(arena),
  objects_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.SceneProto)
}
SceneProto::SceneProto(const SceneProto& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      background_color_(from.background_color_),
      objects_(from.objects_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_id().empty()) {
    id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_id(),
      GetArena());
  }
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_name().empty()) {
    name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_name(),
      GetArena());
  }
  if (from._internal_has_entities()) {
    entities_ = new ::e8::SceneEntityCollection(*from.entities_);
  } else {
    entities_ = nullptr;
  }
  structure_type_ = from.structure_type_;
  // @@protoc_insertion_point(copy_constructor:e8.SceneProto)
}

void SceneProto::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_SceneProto_scene_2eproto.base);
  id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&entities_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&structure_type_) -
      reinterpret_cast<char*>(&entities_)) + sizeof(structure_type_));
}

SceneProto::~SceneProto() {
  // @@protoc_insertion_point(destructor:e8.SceneProto)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void SceneProto::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete entities_;
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
  objects_.Clear();
  id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  if (GetArena() == nullptr && entities_ != nullptr) {
    delete entities_;
  }
  entities_ = nullptr;
  structure_type_ = 0;
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
      // string id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "e8.SceneProto.id"));
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
      // repeated .e8.SceneObject objects = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 42)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_objects(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<42>(ptr));
        } else goto handle_unusual;
        continue;
      // .e8.SceneEntityCollection entities = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 50)) {
          ptr = ctx->ParseMessage(_internal_mutable_entities(), ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* SceneProto::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.SceneProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string id = 1;
  if (this->id().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_id().data(), static_cast<int>(this->_internal_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "e8.SceneProto.id");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_id(), target);
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

  // repeated .e8.SceneObject objects = 5;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_objects_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(5, this->_internal_objects(i), target, stream);
  }

  // .e8.SceneEntityCollection entities = 6;
  if (this->has_entities()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        6, _Internal::entities(this), target, stream);
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

  // repeated .e8.SceneObject objects = 5;
  total_size += 1UL * this->_internal_objects_size();
  for (const auto& msg : this->objects_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // string id = 1;
  if (this->id().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_id());
  }

  // string name = 2;
  if (this->name().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // .e8.SceneEntityCollection entities = 6;
  if (this->has_entities()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *entities_);
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
  objects_.MergeFrom(from.objects_);
  if (from.id().size() > 0) {
    _internal_set_id(from._internal_id());
  }
  if (from.name().size() > 0) {
    _internal_set_name(from._internal_name());
  }
  if (from.has_entities()) {
    _internal_mutable_entities()->::e8::SceneEntityCollection::MergeFrom(from._internal_entities());
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
  objects_.InternalSwap(&other->objects_);
  id_.Swap(&other->id_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  name_.Swap(&other->name_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(SceneProto, structure_type_)
      + sizeof(SceneProto::structure_type_)
      - PROTOBUF_FIELD_OFFSET(SceneProto, entities_)>(
          reinterpret_cast<char*>(&entities_),
          reinterpret_cast<char*>(&other->entities_));
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
