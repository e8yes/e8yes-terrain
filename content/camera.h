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

#ifndef ISLANDS_CONTENT_CAMERA_H
#define ISLANDS_CONTENT_CAMERA_H

#include "common/tensor.h"
#include "content/proto/camera.pb.h"

namespace e8 {

/**
 * @brief DefaultCamera Creates a camera with default parameters. The camera will be pointing down
 * towards the opposite of the z axis and be setting at 2 meters above the origin.
 */
Camera DefaultCamera();

/**
 * @brief CameraRotateBasis Rotates the camera's basis using euler angles.
 *
 * @param roll The number of degree to rotate around the back basis vector.
 * @param pitch The number of degree to rotate around the right basis vector.
 * @param yaw The number of degree to rotate around the down basis vector.
 * @param camera The camera to rotate.
 */
void CameraRotateBasis(float roll, float pitch, float yaw, Camera *camera);

/**
 * @brief CameraEnsureLeveling Zeroes of the right basis vector's z component while keeping the
 * orthogonality of the basis vectors.
 */
void CameraEnsureLeveling(Camera *camera);

} // namespace e8

#endif // ISLANDS_CONTENT_CAMERA_H
