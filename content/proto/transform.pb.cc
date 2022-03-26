// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: transform.proto

#include "transform.pb.h"

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
namespace e8 {
class SrtTransformDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<SrtTransform> _instance;
} _SrtTransform_default_instance_;
}  // namespace e8
static void InitDefaultsscc_info_SrtTransform_transform_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_SrtTransform_default_instance_;
    new (ptr) ::e8::SrtTransform();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::SrtTransform::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_SrtTransform_transform_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_SrtTransform_transform_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_transform_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_transform_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_transform_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_transform_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::e8::SrtTransform, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::e8::SrtTransform, scaling_),
  PROTOBUF_FIELD_OFFSET(::e8::SrtTransform, rotation_),
  PROTOBUF_FIELD_OFFSET(::e8::SrtTransform, translation_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::e8::SrtTransform)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_SrtTransform_default_instance_),
};

const char descriptor_table_protodef_transform_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\017transform.proto\022\002e8\"F\n\014SrtTransform\022\017\n"
  "\007scaling\030\001 \003(\002\022\020\n\010rotation\030\002 \003(\002\022\023\n\013tran"
  "slation\030\003 \003(\002b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_transform_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_transform_2eproto_sccs[1] = {
  &scc_info_SrtTransform_transform_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_transform_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_transform_2eproto = {
  false, false, descriptor_table_protodef_transform_2eproto, "transform.proto", 101,
  &descriptor_table_transform_2eproto_once, descriptor_table_transform_2eproto_sccs, descriptor_table_transform_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_transform_2eproto::offsets,
  file_level_metadata_transform_2eproto, 1, file_level_enum_descriptors_transform_2eproto, file_level_service_descriptors_transform_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_transform_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_transform_2eproto)), true);
namespace e8 {

// ===================================================================

void SrtTransform::InitAsDefaultInstance() {
}
class SrtTransform::_Internal {
 public:
};

SrtTransform::SrtTransform(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  scaling_(arena),
  rotation_(arena),
  translation_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.SrtTransform)
}
SrtTransform::SrtTransform(const SrtTransform& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      scaling_(from.scaling_),
      rotation_(from.rotation_),
      translation_(from.translation_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:e8.SrtTransform)
}

void SrtTransform::SharedCtor() {
}

SrtTransform::~SrtTransform() {
  // @@protoc_insertion_point(destructor:e8.SrtTransform)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void SrtTransform::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void SrtTransform::ArenaDtor(void* object) {
  SrtTransform* _this = reinterpret_cast< SrtTransform* >(object);
  (void)_this;
}
void SrtTransform::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void SrtTransform::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const SrtTransform& SrtTransform::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_SrtTransform_transform_2eproto.base);
  return *internal_default_instance();
}


void SrtTransform::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.SrtTransform)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  scaling_.Clear();
  rotation_.Clear();
  translation_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SrtTransform::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated float scaling = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedFloatParser(_internal_mutable_scaling(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 13) {
          _internal_add_scaling(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr));
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // repeated float rotation = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedFloatParser(_internal_mutable_rotation(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 21) {
          _internal_add_rotation(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr));
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // repeated float translation = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedFloatParser(_internal_mutable_translation(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 29) {
          _internal_add_translation(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr));
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

::PROTOBUF_NAMESPACE_ID::uint8* SrtTransform::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.SrtTransform)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated float scaling = 1;
  if (this->_internal_scaling_size() > 0) {
    target = stream->WriteFixedPacked(1, _internal_scaling(), target);
  }

  // repeated float rotation = 2;
  if (this->_internal_rotation_size() > 0) {
    target = stream->WriteFixedPacked(2, _internal_rotation(), target);
  }

  // repeated float translation = 3;
  if (this->_internal_translation_size() > 0) {
    target = stream->WriteFixedPacked(3, _internal_translation(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.SrtTransform)
  return target;
}

size_t SrtTransform::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.SrtTransform)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated float scaling = 1;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_scaling_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _scaling_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // repeated float rotation = 2;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_rotation_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _rotation_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // repeated float translation = 3;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_translation_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _translation_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void SrtTransform::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.SrtTransform)
  GOOGLE_DCHECK_NE(&from, this);
  const SrtTransform* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<SrtTransform>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.SrtTransform)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.SrtTransform)
    MergeFrom(*source);
  }
}

void SrtTransform::MergeFrom(const SrtTransform& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.SrtTransform)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  scaling_.MergeFrom(from.scaling_);
  rotation_.MergeFrom(from.rotation_);
  translation_.MergeFrom(from.translation_);
}

void SrtTransform::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.SrtTransform)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SrtTransform::CopyFrom(const SrtTransform& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.SrtTransform)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SrtTransform::IsInitialized() const {
  return true;
}

void SrtTransform::InternalSwap(SrtTransform* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  scaling_.InternalSwap(&other->scaling_);
  rotation_.InternalSwap(&other->rotation_);
  translation_.InternalSwap(&other->translation_);
}

::PROTOBUF_NAMESPACE_ID::Metadata SrtTransform::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::e8::SrtTransform* Arena::CreateMaybeMessage< ::e8::SrtTransform >(Arena* arena) {
  return Arena::CreateMessageInternal< ::e8::SrtTransform >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
