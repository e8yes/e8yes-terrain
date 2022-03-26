// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: entity.proto

#include "entity.pb.h"

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
extern PROTOBUF_INTERNAL_EXPORT_bbox_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_AABB_bbox_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_lod_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_SceneEntityResources_lod_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_transform_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_SrtTransform_transform_2eproto;
namespace e8 {
class SceneEntityProtoDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<SceneEntityProto> _instance;
} _SceneEntityProto_default_instance_;
}  // namespace e8
static void InitDefaultsscc_info_SceneEntityProto_entity_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_SceneEntityProto_default_instance_;
    new (ptr) ::e8::SceneEntityProto();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::SceneEntityProto::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<3> scc_info_SceneEntityProto_entity_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 3, 0, InitDefaultsscc_info_SceneEntityProto_entity_2eproto}, {
      &scc_info_SrtTransform_transform_2eproto.base,
      &scc_info_AABB_bbox_2eproto.base,
      &scc_info_SceneEntityResources_lod_2eproto.base,}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_entity_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_entity_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_entity_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_entity_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityProto, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityProto, id_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityProto, name_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityProto, movable_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityProto, transform_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityProto, srt_transform_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityProto, bounding_box_),
  PROTOBUF_FIELD_OFFSET(::e8::SceneEntityProto, resources_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::e8::SceneEntityProto)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_SceneEntityProto_default_instance_),
};

const char descriptor_table_protodef_entity_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\014entity.proto\022\002e8\032\nbbox.proto\032\tlod.prot"
  "o\032\017transform.proto\"\306\001\n\020SceneEntityProto\022"
  "\n\n\002id\030\001 \001(\003\022\014\n\004name\030\002 \001(\t\022\017\n\007movable\030\003 \001"
  "(\010\022\021\n\ttransform\030\004 \003(\002\022\'\n\rsrt_transform\030\005"
  " \001(\0132\020.e8.SrtTransform\022\036\n\014bounding_box\030\006"
  " \001(\0132\010.e8.AABB\022+\n\tresources\030\007 \001(\0132\030.e8.S"
  "ceneEntityResourcesb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_entity_2eproto_deps[3] = {
  &::descriptor_table_bbox_2eproto,
  &::descriptor_table_lod_2eproto,
  &::descriptor_table_transform_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_entity_2eproto_sccs[1] = {
  &scc_info_SceneEntityProto_entity_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_entity_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_entity_2eproto = {
  false, false, descriptor_table_protodef_entity_2eproto, "entity.proto", 267,
  &descriptor_table_entity_2eproto_once, descriptor_table_entity_2eproto_sccs, descriptor_table_entity_2eproto_deps, 1, 3,
  schemas, file_default_instances, TableStruct_entity_2eproto::offsets,
  file_level_metadata_entity_2eproto, 1, file_level_enum_descriptors_entity_2eproto, file_level_service_descriptors_entity_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_entity_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_entity_2eproto)), true);
namespace e8 {

// ===================================================================

void SceneEntityProto::InitAsDefaultInstance() {
  ::e8::_SceneEntityProto_default_instance_._instance.get_mutable()->srt_transform_ = const_cast< ::e8::SrtTransform*>(
      ::e8::SrtTransform::internal_default_instance());
  ::e8::_SceneEntityProto_default_instance_._instance.get_mutable()->bounding_box_ = const_cast< ::e8::AABB*>(
      ::e8::AABB::internal_default_instance());
  ::e8::_SceneEntityProto_default_instance_._instance.get_mutable()->resources_ = const_cast< ::e8::SceneEntityResources*>(
      ::e8::SceneEntityResources::internal_default_instance());
}
class SceneEntityProto::_Internal {
 public:
  static const ::e8::SrtTransform& srt_transform(const SceneEntityProto* msg);
  static const ::e8::AABB& bounding_box(const SceneEntityProto* msg);
  static const ::e8::SceneEntityResources& resources(const SceneEntityProto* msg);
};

const ::e8::SrtTransform&
SceneEntityProto::_Internal::srt_transform(const SceneEntityProto* msg) {
  return *msg->srt_transform_;
}
const ::e8::AABB&
SceneEntityProto::_Internal::bounding_box(const SceneEntityProto* msg) {
  return *msg->bounding_box_;
}
const ::e8::SceneEntityResources&
SceneEntityProto::_Internal::resources(const SceneEntityProto* msg) {
  return *msg->resources_;
}
void SceneEntityProto::clear_srt_transform() {
  if (GetArena() == nullptr && srt_transform_ != nullptr) {
    delete srt_transform_;
  }
  srt_transform_ = nullptr;
}
void SceneEntityProto::clear_bounding_box() {
  if (GetArena() == nullptr && bounding_box_ != nullptr) {
    delete bounding_box_;
  }
  bounding_box_ = nullptr;
}
void SceneEntityProto::clear_resources() {
  if (GetArena() == nullptr && resources_ != nullptr) {
    delete resources_;
  }
  resources_ = nullptr;
}
SceneEntityProto::SceneEntityProto(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  transform_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.SceneEntityProto)
}
SceneEntityProto::SceneEntityProto(const SceneEntityProto& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      transform_(from.transform_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_name().empty()) {
    name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_name(),
      GetArena());
  }
  if (from._internal_has_srt_transform()) {
    srt_transform_ = new ::e8::SrtTransform(*from.srt_transform_);
  } else {
    srt_transform_ = nullptr;
  }
  if (from._internal_has_bounding_box()) {
    bounding_box_ = new ::e8::AABB(*from.bounding_box_);
  } else {
    bounding_box_ = nullptr;
  }
  if (from._internal_has_resources()) {
    resources_ = new ::e8::SceneEntityResources(*from.resources_);
  } else {
    resources_ = nullptr;
  }
  ::memcpy(&id_, &from.id_,
    static_cast<size_t>(reinterpret_cast<char*>(&movable_) -
    reinterpret_cast<char*>(&id_)) + sizeof(movable_));
  // @@protoc_insertion_point(copy_constructor:e8.SceneEntityProto)
}

void SceneEntityProto::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_SceneEntityProto_entity_2eproto.base);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&srt_transform_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&movable_) -
      reinterpret_cast<char*>(&srt_transform_)) + sizeof(movable_));
}

SceneEntityProto::~SceneEntityProto() {
  // @@protoc_insertion_point(destructor:e8.SceneEntityProto)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void SceneEntityProto::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete srt_transform_;
  if (this != internal_default_instance()) delete bounding_box_;
  if (this != internal_default_instance()) delete resources_;
}

void SceneEntityProto::ArenaDtor(void* object) {
  SceneEntityProto* _this = reinterpret_cast< SceneEntityProto* >(object);
  (void)_this;
}
void SceneEntityProto::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void SceneEntityProto::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const SceneEntityProto& SceneEntityProto::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_SceneEntityProto_entity_2eproto.base);
  return *internal_default_instance();
}


void SceneEntityProto::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.SceneEntityProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  transform_.Clear();
  name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  if (GetArena() == nullptr && srt_transform_ != nullptr) {
    delete srt_transform_;
  }
  srt_transform_ = nullptr;
  if (GetArena() == nullptr && bounding_box_ != nullptr) {
    delete bounding_box_;
  }
  bounding_box_ = nullptr;
  if (GetArena() == nullptr && resources_ != nullptr) {
    delete resources_;
  }
  resources_ = nullptr;
  ::memset(&id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&movable_) -
      reinterpret_cast<char*>(&id_)) + sizeof(movable_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SceneEntityProto::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
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
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "e8.SceneEntityProto.name"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bool movable = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          movable_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated float transform = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedFloatParser(_internal_mutable_transform(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 37) {
          _internal_add_transform(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr));
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // .e8.SrtTransform srt_transform = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 42)) {
          ptr = ctx->ParseMessage(_internal_mutable_srt_transform(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .e8.AABB bounding_box = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 50)) {
          ptr = ctx->ParseMessage(_internal_mutable_bounding_box(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .e8.SceneEntityResources resources = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 58)) {
          ptr = ctx->ParseMessage(_internal_mutable_resources(), ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* SceneEntityProto::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.SceneEntityProto)
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
      "e8.SceneEntityProto.name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_name(), target);
  }

  // bool movable = 3;
  if (this->movable() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(3, this->_internal_movable(), target);
  }

  // repeated float transform = 4;
  if (this->_internal_transform_size() > 0) {
    target = stream->WriteFixedPacked(4, _internal_transform(), target);
  }

  // .e8.SrtTransform srt_transform = 5;
  if (this->has_srt_transform()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        5, _Internal::srt_transform(this), target, stream);
  }

  // .e8.AABB bounding_box = 6;
  if (this->has_bounding_box()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        6, _Internal::bounding_box(this), target, stream);
  }

  // .e8.SceneEntityResources resources = 7;
  if (this->has_resources()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        7, _Internal::resources(this), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.SceneEntityProto)
  return target;
}

size_t SceneEntityProto::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.SceneEntityProto)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated float transform = 4;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_transform_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _transform_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // string name = 2;
  if (this->name().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // .e8.SrtTransform srt_transform = 5;
  if (this->has_srt_transform()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *srt_transform_);
  }

  // .e8.AABB bounding_box = 6;
  if (this->has_bounding_box()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *bounding_box_);
  }

  // .e8.SceneEntityResources resources = 7;
  if (this->has_resources()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *resources_);
  }

  // int64 id = 1;
  if (this->id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_id());
  }

  // bool movable = 3;
  if (this->movable() != 0) {
    total_size += 1 + 1;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void SceneEntityProto::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.SceneEntityProto)
  GOOGLE_DCHECK_NE(&from, this);
  const SceneEntityProto* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<SceneEntityProto>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.SceneEntityProto)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.SceneEntityProto)
    MergeFrom(*source);
  }
}

void SceneEntityProto::MergeFrom(const SceneEntityProto& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.SceneEntityProto)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  transform_.MergeFrom(from.transform_);
  if (from.name().size() > 0) {
    _internal_set_name(from._internal_name());
  }
  if (from.has_srt_transform()) {
    _internal_mutable_srt_transform()->::e8::SrtTransform::MergeFrom(from._internal_srt_transform());
  }
  if (from.has_bounding_box()) {
    _internal_mutable_bounding_box()->::e8::AABB::MergeFrom(from._internal_bounding_box());
  }
  if (from.has_resources()) {
    _internal_mutable_resources()->::e8::SceneEntityResources::MergeFrom(from._internal_resources());
  }
  if (from.id() != 0) {
    _internal_set_id(from._internal_id());
  }
  if (from.movable() != 0) {
    _internal_set_movable(from._internal_movable());
  }
}

void SceneEntityProto::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.SceneEntityProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SceneEntityProto::CopyFrom(const SceneEntityProto& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.SceneEntityProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SceneEntityProto::IsInitialized() const {
  return true;
}

void SceneEntityProto::InternalSwap(SceneEntityProto* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  transform_.InternalSwap(&other->transform_);
  name_.Swap(&other->name_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(SceneEntityProto, movable_)
      + sizeof(SceneEntityProto::movable_)
      - PROTOBUF_FIELD_OFFSET(SceneEntityProto, srt_transform_)>(
          reinterpret_cast<char*>(&srt_transform_),
          reinterpret_cast<char*>(&other->srt_transform_));
}

::PROTOBUF_NAMESPACE_ID::Metadata SceneEntityProto::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::e8::SceneEntityProto* Arena::CreateMaybeMessage< ::e8::SceneEntityProto >(Arena* arena) {
  return Arena::CreateMessageInternal< ::e8::SceneEntityProto >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
