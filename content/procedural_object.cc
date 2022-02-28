/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <google/protobuf/repeated_field.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "content/common.h"
#include "content/procedural_basic.h"
#include "content/procedural_object.h"
#include "content/proto/procedural_object.pb.h"
#include "content/scene_entity.h"
#include "content/scene_object.h"

namespace e8 {
namespace {

std::unique_ptr<ProceduralObjectInterface> ToProceduralObject(ProceduralObjectProto const &proto) {
    switch (proto.type()) {
    case ProceduralObjectProto::PLANE: {
        return std::make_unique<ProceduralPlane>(proto);
    }
    default: {
        assert(false);
    }
    }
}

} // namespace

ProceduralObjectInterface::ProceduralObjectInterface(ProceduralObjectName const &name)
    : id(GenerateUuid()), name(name), movable(true) {}

ProceduralObjectInterface::ProceduralObjectInterface(ProceduralObjectProto const &proto)
    : id(proto.id()), name(proto.name()) {}

ProceduralObjectInterface::~ProceduralObjectInterface() {}

ProceduralObjectProto ProceduralObjectInterface::_ToBaseProto(
    ProceduralObjectProto::ProceduralObjectType const &object_type) const {
    ProceduralObjectProto proto;
    proto.set_id(id);
    proto.set_name(name);
    proto.set_type(object_type);
    proto.set_movable(movable);
    return proto;
}

google::protobuf::RepeatedPtrField<ProceduralObjectProto>
ToProto(std::map<ProceduralObjectId, std::unique_ptr<ProceduralObjectInterface>> const
            &procedural_objects) {
    google::protobuf::RepeatedPtrField<ProceduralObjectProto> proto;

    for (auto const &[_, procedural_object] : procedural_objects) {
        *proto.Add() = procedural_object->ToProto();
    }

    return proto;
}

std::map<ProceduralObjectId, std::unique_ptr<ProceduralObjectInterface>>
ToProceduralObjects(google::protobuf::RepeatedPtrField<ProceduralObjectProto> const &protos) {
    std::map<ProceduralObjectId, std::unique_ptr<ProceduralObjectInterface>> procedural_objects;

    for (auto const &proto : protos) {
        std::unique_ptr<ProceduralObjectInterface> procedural_object = ToProceduralObject(proto);
        procedural_objects.insert(
            std::make_pair(procedural_object->id, std::move(procedural_object)));
    }

    return procedural_objects;
}

} // namespace e8
