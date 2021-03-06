// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: texture.proto

#include "texture.pb.h"

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
class TextureProtoDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<TextureProto> _instance;
} _TextureProto_default_instance_;
}  // namespace e8
static void InitDefaultsscc_info_TextureProto_texture_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::e8::_TextureProto_default_instance_;
    new (ptr) ::e8::TextureProto();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::e8::TextureProto::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_TextureProto_texture_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_TextureProto_texture_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_texture_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_texture_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_texture_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_texture_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::e8::TextureProto, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::e8::TextureProto, encoding_),
  PROTOBUF_FIELD_OFFSET(::e8::TextureProto, width_),
  PROTOBUF_FIELD_OFFSET(::e8::TextureProto, height_),
  PROTOBUF_FIELD_OFFSET(::e8::TextureProto, channel_count_),
  PROTOBUF_FIELD_OFFSET(::e8::TextureProto, channel_size_),
  PROTOBUF_FIELD_OFFSET(::e8::TextureProto, data_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::e8::TextureProto)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::e8::_TextureProto_default_instance_),
};

const char descriptor_table_protodef_texture_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\rtexture.proto\022\002e8\"\303\001\n\014TextureProto\022+\n\010"
  "encoding\030\001 \001(\0162\031.e8.TextureProto.Encodin"
  "g\022\r\n\005width\030\002 \001(\005\022\016\n\006height\030\003 \001(\005\022\025\n\rchan"
  "nel_count\030\004 \001(\005\022\024\n\014channel_size\030\005 \001(\005\022\014\n"
  "\004data\030\006 \001(\014\",\n\010Encoding\022\013\n\007INVALID\020\000\022\n\n\006"
  "BITMAP\020\001\022\007\n\003PNG\020\002b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_texture_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_texture_2eproto_sccs[1] = {
  &scc_info_TextureProto_texture_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_texture_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_texture_2eproto = {
  false, false, descriptor_table_protodef_texture_2eproto, "texture.proto", 225,
  &descriptor_table_texture_2eproto_once, descriptor_table_texture_2eproto_sccs, descriptor_table_texture_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_texture_2eproto::offsets,
  file_level_metadata_texture_2eproto, 1, file_level_enum_descriptors_texture_2eproto, file_level_service_descriptors_texture_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_texture_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_texture_2eproto)), true);
namespace e8 {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* TextureProto_Encoding_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_texture_2eproto);
  return file_level_enum_descriptors_texture_2eproto[0];
}
bool TextureProto_Encoding_IsValid(int value) {
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
constexpr TextureProto_Encoding TextureProto::INVALID;
constexpr TextureProto_Encoding TextureProto::BITMAP;
constexpr TextureProto_Encoding TextureProto::PNG;
constexpr TextureProto_Encoding TextureProto::Encoding_MIN;
constexpr TextureProto_Encoding TextureProto::Encoding_MAX;
constexpr int TextureProto::Encoding_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void TextureProto::InitAsDefaultInstance() {
}
class TextureProto::_Internal {
 public:
};

TextureProto::TextureProto(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:e8.TextureProto)
}
TextureProto::TextureProto(const TextureProto& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  data_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_data().empty()) {
    data_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_data(),
      GetArena());
  }
  ::memcpy(&encoding_, &from.encoding_,
    static_cast<size_t>(reinterpret_cast<char*>(&channel_size_) -
    reinterpret_cast<char*>(&encoding_)) + sizeof(channel_size_));
  // @@protoc_insertion_point(copy_constructor:e8.TextureProto)
}

void TextureProto::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_TextureProto_texture_2eproto.base);
  data_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&encoding_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&channel_size_) -
      reinterpret_cast<char*>(&encoding_)) + sizeof(channel_size_));
}

TextureProto::~TextureProto() {
  // @@protoc_insertion_point(destructor:e8.TextureProto)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void TextureProto::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  data_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void TextureProto::ArenaDtor(void* object) {
  TextureProto* _this = reinterpret_cast< TextureProto* >(object);
  (void)_this;
}
void TextureProto::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void TextureProto::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const TextureProto& TextureProto::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_TextureProto_texture_2eproto.base);
  return *internal_default_instance();
}


void TextureProto::Clear() {
// @@protoc_insertion_point(message_clear_start:e8.TextureProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  data_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::memset(&encoding_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&channel_size_) -
      reinterpret_cast<char*>(&encoding_)) + sizeof(channel_size_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* TextureProto::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // .e8.TextureProto.Encoding encoding = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_encoding(static_cast<::e8::TextureProto_Encoding>(val));
        } else goto handle_unusual;
        continue;
      // int32 width = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          width_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 height = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          height_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 channel_count = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          channel_count_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 channel_size = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          channel_size_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bytes data = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 50)) {
          auto str = _internal_mutable_data();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
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

::PROTOBUF_NAMESPACE_ID::uint8* TextureProto::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:e8.TextureProto)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .e8.TextureProto.Encoding encoding = 1;
  if (this->encoding() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_encoding(), target);
  }

  // int32 width = 2;
  if (this->width() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_width(), target);
  }

  // int32 height = 3;
  if (this->height() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(3, this->_internal_height(), target);
  }

  // int32 channel_count = 4;
  if (this->channel_count() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(4, this->_internal_channel_count(), target);
  }

  // int32 channel_size = 5;
  if (this->channel_size() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(5, this->_internal_channel_size(), target);
  }

  // bytes data = 6;
  if (this->data().size() > 0) {
    target = stream->WriteBytesMaybeAliased(
        6, this->_internal_data(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:e8.TextureProto)
  return target;
}

size_t TextureProto::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:e8.TextureProto)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes data = 6;
  if (this->data().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_data());
  }

  // .e8.TextureProto.Encoding encoding = 1;
  if (this->encoding() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_encoding());
  }

  // int32 width = 2;
  if (this->width() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_width());
  }

  // int32 height = 3;
  if (this->height() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_height());
  }

  // int32 channel_count = 4;
  if (this->channel_count() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_channel_count());
  }

  // int32 channel_size = 5;
  if (this->channel_size() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_channel_size());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void TextureProto::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:e8.TextureProto)
  GOOGLE_DCHECK_NE(&from, this);
  const TextureProto* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<TextureProto>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:e8.TextureProto)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:e8.TextureProto)
    MergeFrom(*source);
  }
}

void TextureProto::MergeFrom(const TextureProto& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:e8.TextureProto)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.data().size() > 0) {
    _internal_set_data(from._internal_data());
  }
  if (from.encoding() != 0) {
    _internal_set_encoding(from._internal_encoding());
  }
  if (from.width() != 0) {
    _internal_set_width(from._internal_width());
  }
  if (from.height() != 0) {
    _internal_set_height(from._internal_height());
  }
  if (from.channel_count() != 0) {
    _internal_set_channel_count(from._internal_channel_count());
  }
  if (from.channel_size() != 0) {
    _internal_set_channel_size(from._internal_channel_size());
  }
}

void TextureProto::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:e8.TextureProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void TextureProto::CopyFrom(const TextureProto& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:e8.TextureProto)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TextureProto::IsInitialized() const {
  return true;
}

void TextureProto::InternalSwap(TextureProto* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  data_.Swap(&other->data_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(TextureProto, channel_size_)
      + sizeof(TextureProto::channel_size_)
      - PROTOBUF_FIELD_OFFSET(TextureProto, encoding_)>(
          reinterpret_cast<char*>(&encoding_),
          reinterpret_cast<char*>(&other->encoding_));
}

::PROTOBUF_NAMESPACE_ID::Metadata TextureProto::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::e8::TextureProto* Arena::CreateMaybeMessage< ::e8::TextureProto >(Arena* arena) {
  return Arena::CreateMessageInternal< ::e8::TextureProto >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
