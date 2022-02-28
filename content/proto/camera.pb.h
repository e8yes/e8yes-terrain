// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: camera.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_camera_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_camera_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_camera_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_camera_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_camera_2eproto;
namespace e8 {
class Camera;
class CameraDefaultTypeInternal;
extern CameraDefaultTypeInternal _Camera_default_instance_;
}  // namespace e8
PROTOBUF_NAMESPACE_OPEN
template<> ::e8::Camera* Arena::CreateMaybeMessage<::e8::Camera>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace e8 {

// ===================================================================

class Camera PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:e8.Camera) */ {
 public:
  inline Camera() : Camera(nullptr) {};
  virtual ~Camera();

  Camera(const Camera& from);
  Camera(Camera&& from) noexcept
    : Camera() {
    *this = ::std::move(from);
  }

  inline Camera& operator=(const Camera& from) {
    CopyFrom(from);
    return *this;
  }
  inline Camera& operator=(Camera&& from) noexcept {
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
  static const Camera& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Camera* internal_default_instance() {
    return reinterpret_cast<const Camera*>(
               &_Camera_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Camera& a, Camera& b) {
    a.Swap(&b);
  }
  inline void Swap(Camera* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Camera* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Camera* New() const final {
    return CreateMaybeMessage<Camera>(nullptr);
  }

  Camera* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Camera>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Camera& from);
  void MergeFrom(const Camera& from);
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
  void InternalSwap(Camera* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "e8.Camera";
  }
  protected:
  explicit Camera(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_camera_2eproto);
    return ::descriptor_table_camera_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kPositionFieldNumber = 2,
    kRotationFieldNumber = 3,
    kIdFieldNumber = 1,
    kFocalLengthFieldNumber = 4,
    kSensorWidthFieldNumber = 5,
    kSensorHeightFieldNumber = 6,
    kMaxDistanceFieldNumber = 7,
    kImageWidthFieldNumber = 8,
  };
  // repeated float position = 2;
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

  // repeated float rotation = 3;
  int rotation_size() const;
  private:
  int _internal_rotation_size() const;
  public:
  void clear_rotation();
  private:
  float _internal_rotation(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      _internal_rotation() const;
  void _internal_add_rotation(float value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      _internal_mutable_rotation();
  public:
  float rotation(int index) const;
  void set_rotation(int index, float value);
  void add_rotation(float value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
      rotation() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
      mutable_rotation();

  // string id = 1;
  void clear_id();
  const std::string& id() const;
  void set_id(const std::string& value);
  void set_id(std::string&& value);
  void set_id(const char* value);
  void set_id(const char* value, size_t size);
  std::string* mutable_id();
  std::string* release_id();
  void set_allocated_id(std::string* id);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_id();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_id(
      std::string* id);
  private:
  const std::string& _internal_id() const;
  void _internal_set_id(const std::string& value);
  std::string* _internal_mutable_id();
  public:

  // float focal_length = 4;
  void clear_focal_length();
  float focal_length() const;
  void set_focal_length(float value);
  private:
  float _internal_focal_length() const;
  void _internal_set_focal_length(float value);
  public:

  // float sensor_width = 5;
  void clear_sensor_width();
  float sensor_width() const;
  void set_sensor_width(float value);
  private:
  float _internal_sensor_width() const;
  void _internal_set_sensor_width(float value);
  public:

  // float sensor_height = 6;
  void clear_sensor_height();
  float sensor_height() const;
  void set_sensor_height(float value);
  private:
  float _internal_sensor_height() const;
  void _internal_set_sensor_height(float value);
  public:

  // float max_distance = 7;
  void clear_max_distance();
  float max_distance() const;
  void set_max_distance(float value);
  private:
  float _internal_max_distance() const;
  void _internal_set_max_distance(float value);
  public:

  // int32 image_width = 8;
  void clear_image_width();
  ::PROTOBUF_NAMESPACE_ID::int32 image_width() const;
  void set_image_width(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_image_width() const;
  void _internal_set_image_width(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:e8.Camera)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > position_;
  mutable std::atomic<int> _position_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< float > rotation_;
  mutable std::atomic<int> _rotation_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr id_;
  float focal_length_;
  float sensor_width_;
  float sensor_height_;
  float max_distance_;
  ::PROTOBUF_NAMESPACE_ID::int32 image_width_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_camera_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Camera

// string id = 1;
inline void Camera::clear_id() {
  id_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& Camera::id() const {
  // @@protoc_insertion_point(field_get:e8.Camera.id)
  return _internal_id();
}
inline void Camera::set_id(const std::string& value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:e8.Camera.id)
}
inline std::string* Camera::mutable_id() {
  // @@protoc_insertion_point(field_mutable:e8.Camera.id)
  return _internal_mutable_id();
}
inline const std::string& Camera::_internal_id() const {
  return id_.Get();
}
inline void Camera::_internal_set_id(const std::string& value) {
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void Camera::set_id(std::string&& value) {
  
  id_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:e8.Camera.id)
}
inline void Camera::set_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:e8.Camera.id)
}
inline void Camera::set_id(const char* value,
    size_t size) {
  
  id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:e8.Camera.id)
}
inline std::string* Camera::_internal_mutable_id() {
  
  return id_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* Camera::release_id() {
  // @@protoc_insertion_point(field_release:e8.Camera.id)
  return id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void Camera::set_allocated_id(std::string* id) {
  if (id != nullptr) {
    
  } else {
    
  }
  id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), id,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:e8.Camera.id)
}
inline std::string* Camera::unsafe_arena_release_id() {
  // @@protoc_insertion_point(field_unsafe_arena_release:e8.Camera.id)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return id_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void Camera::unsafe_arena_set_allocated_id(
    std::string* id) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (id != nullptr) {
    
  } else {
    
  }
  id_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      id, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:e8.Camera.id)
}

// repeated float position = 2;
inline int Camera::_internal_position_size() const {
  return position_.size();
}
inline int Camera::position_size() const {
  return _internal_position_size();
}
inline void Camera::clear_position() {
  position_.Clear();
}
inline float Camera::_internal_position(int index) const {
  return position_.Get(index);
}
inline float Camera::position(int index) const {
  // @@protoc_insertion_point(field_get:e8.Camera.position)
  return _internal_position(index);
}
inline void Camera::set_position(int index, float value) {
  position_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.Camera.position)
}
inline void Camera::_internal_add_position(float value) {
  position_.Add(value);
}
inline void Camera::add_position(float value) {
  _internal_add_position(value);
  // @@protoc_insertion_point(field_add:e8.Camera.position)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
Camera::_internal_position() const {
  return position_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
Camera::position() const {
  // @@protoc_insertion_point(field_list:e8.Camera.position)
  return _internal_position();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
Camera::_internal_mutable_position() {
  return &position_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
Camera::mutable_position() {
  // @@protoc_insertion_point(field_mutable_list:e8.Camera.position)
  return _internal_mutable_position();
}

// repeated float rotation = 3;
inline int Camera::_internal_rotation_size() const {
  return rotation_.size();
}
inline int Camera::rotation_size() const {
  return _internal_rotation_size();
}
inline void Camera::clear_rotation() {
  rotation_.Clear();
}
inline float Camera::_internal_rotation(int index) const {
  return rotation_.Get(index);
}
inline float Camera::rotation(int index) const {
  // @@protoc_insertion_point(field_get:e8.Camera.rotation)
  return _internal_rotation(index);
}
inline void Camera::set_rotation(int index, float value) {
  rotation_.Set(index, value);
  // @@protoc_insertion_point(field_set:e8.Camera.rotation)
}
inline void Camera::_internal_add_rotation(float value) {
  rotation_.Add(value);
}
inline void Camera::add_rotation(float value) {
  _internal_add_rotation(value);
  // @@protoc_insertion_point(field_add:e8.Camera.rotation)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
Camera::_internal_rotation() const {
  return rotation_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >&
Camera::rotation() const {
  // @@protoc_insertion_point(field_list:e8.Camera.rotation)
  return _internal_rotation();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
Camera::_internal_mutable_rotation() {
  return &rotation_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< float >*
Camera::mutable_rotation() {
  // @@protoc_insertion_point(field_mutable_list:e8.Camera.rotation)
  return _internal_mutable_rotation();
}

// float focal_length = 4;
inline void Camera::clear_focal_length() {
  focal_length_ = 0;
}
inline float Camera::_internal_focal_length() const {
  return focal_length_;
}
inline float Camera::focal_length() const {
  // @@protoc_insertion_point(field_get:e8.Camera.focal_length)
  return _internal_focal_length();
}
inline void Camera::_internal_set_focal_length(float value) {
  
  focal_length_ = value;
}
inline void Camera::set_focal_length(float value) {
  _internal_set_focal_length(value);
  // @@protoc_insertion_point(field_set:e8.Camera.focal_length)
}

// float sensor_width = 5;
inline void Camera::clear_sensor_width() {
  sensor_width_ = 0;
}
inline float Camera::_internal_sensor_width() const {
  return sensor_width_;
}
inline float Camera::sensor_width() const {
  // @@protoc_insertion_point(field_get:e8.Camera.sensor_width)
  return _internal_sensor_width();
}
inline void Camera::_internal_set_sensor_width(float value) {
  
  sensor_width_ = value;
}
inline void Camera::set_sensor_width(float value) {
  _internal_set_sensor_width(value);
  // @@protoc_insertion_point(field_set:e8.Camera.sensor_width)
}

// float sensor_height = 6;
inline void Camera::clear_sensor_height() {
  sensor_height_ = 0;
}
inline float Camera::_internal_sensor_height() const {
  return sensor_height_;
}
inline float Camera::sensor_height() const {
  // @@protoc_insertion_point(field_get:e8.Camera.sensor_height)
  return _internal_sensor_height();
}
inline void Camera::_internal_set_sensor_height(float value) {
  
  sensor_height_ = value;
}
inline void Camera::set_sensor_height(float value) {
  _internal_set_sensor_height(value);
  // @@protoc_insertion_point(field_set:e8.Camera.sensor_height)
}

// float max_distance = 7;
inline void Camera::clear_max_distance() {
  max_distance_ = 0;
}
inline float Camera::_internal_max_distance() const {
  return max_distance_;
}
inline float Camera::max_distance() const {
  // @@protoc_insertion_point(field_get:e8.Camera.max_distance)
  return _internal_max_distance();
}
inline void Camera::_internal_set_max_distance(float value) {
  
  max_distance_ = value;
}
inline void Camera::set_max_distance(float value) {
  _internal_set_max_distance(value);
  // @@protoc_insertion_point(field_set:e8.Camera.max_distance)
}

// int32 image_width = 8;
inline void Camera::clear_image_width() {
  image_width_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Camera::_internal_image_width() const {
  return image_width_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Camera::image_width() const {
  // @@protoc_insertion_point(field_get:e8.Camera.image_width)
  return _internal_image_width();
}
inline void Camera::_internal_set_image_width(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  image_width_ = value;
}
inline void Camera::set_image_width(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_image_width(value);
  // @@protoc_insertion_point(field_set:e8.Camera.image_width)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace e8

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_camera_2eproto
