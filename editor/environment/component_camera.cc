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

#include <QLineEdit>
#include <QObject>
#include <string>

#include "content/proto/camera.pb.h"
#include "content/scene.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/environment/component_camera.h"

namespace e8 {
namespace {

void SetCameraWidgets(Scene *scene, QLineEdit *pos_x, QLineEdit *pos_y, QLineEdit *pos_z,
                      QLineEdit *roll, QLineEdit *pitch, QLineEdit *yaw, QLineEdit *focal_length,
                      QLineEdit *sensor_width, QLineEdit *sensor_height, QLineEdit *max_distance,
                      QLineEdit *image_width) {
    Camera camera;
    {
        Scene::ReadAccess read_access = scene->GainReadAccess();
        camera = scene->camera;
    }

    pos_x->setText(std::to_string(camera.position(0)).c_str());
    pos_y->setText(std::to_string(camera.position(1)).c_str());
    pos_z->setText(std::to_string(camera.position(2)).c_str());

    roll->setText(std::to_string(camera.rotation(0)).c_str());
    pitch->setText(std::to_string(camera.rotation(1)).c_str());
    yaw->setText(std::to_string(camera.rotation(2)).c_str());

    focal_length->setText(std::to_string(camera.focal_length()).c_str());
    sensor_width->setText(std::to_string(camera.sensor_width()).c_str());
    sensor_height->setText(std::to_string(camera.sensor_height()).c_str());
    max_distance->setText(std::to_string(camera.max_distance()).c_str());
    image_width->setText(std::to_string(camera.image_width()).c_str());
}

void SetSceneCamera(QLineEdit *pos_x, QLineEdit *pos_y, QLineEdit *pos_z, QLineEdit *roll,
                    QLineEdit *pitch, QLineEdit *yaw, QLineEdit *focal_length,
                    QLineEdit *sensor_width, QLineEdit *sensor_height, QLineEdit *max_distance,
                    QLineEdit *image_width, Scene *scene) {
    Camera camera;
    {
        Scene::ReadAccess read_access = scene->GainReadAccess();
        camera = scene->camera;
    }

    camera.set_position(0, pos_x->text().toFloat());
    camera.set_position(1, pos_y->text().toFloat());
    camera.set_position(2, pos_z->text().toFloat());

    camera.set_rotation(0, roll->text().toFloat());
    camera.set_rotation(1, pitch->text().toFloat());
    camera.set_rotation(2, yaw->text().toFloat());

    camera.set_focal_length(focal_length->text().toFloat());
    camera.set_sensor_width(sensor_width->text().toFloat());
    camera.set_sensor_height(sensor_height->text().toFloat());
    camera.set_max_distance(max_distance->text().toFloat());
    camera.set_image_width(image_width->text().toInt());

    {
        Scene::WriteAccess write_access = scene->GainWriteAccess();
        scene->camera = camera;
    }
}

} // namespace

CameraComponent::CameraComponent(ModificationMonitorComponent *modification_monitor_comp,
                                 EditorContext *context)
    : modification_monitor_comp_(modification_monitor_comp), context_(context) {
    QObject::connect(context_->ui->camera_pos_x_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
    QObject::connect(context_->ui->camera_pos_y_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
    QObject::connect(context_->ui->camera_pos_z_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);

    QObject::connect(context_->ui->camera_rot_roll_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
    QObject::connect(context_->ui->camera_rot_pitch_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
    QObject::connect(context_->ui->camera_rot_yaw_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);

    QObject::connect(context_->ui->camera_focal_len_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
    QObject::connect(context_->ui->camera_sensor_width_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
    QObject::connect(context_->ui->camera_sensor_height_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
    QObject::connect(context_->ui->camera_max_dist_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
    QObject::connect(context_->ui->camera_image_width_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCamera);
}

CameraComponent::~CameraComponent() {}

void CameraComponent::OnChangeScene() {
    SetCameraWidgets(context_->scene.get(), context_->ui->camera_pos_x_edit,
                     context_->ui->camera_pos_y_edit, context_->ui->camera_pos_z_edit,
                     context_->ui->camera_rot_roll_edit, context_->ui->camera_rot_pitch_edit,
                     context_->ui->camera_rot_yaw_edit, context_->ui->camera_focal_len_edit,
                     context_->ui->camera_sensor_width_edit,
                     context_->ui->camera_sensor_height_edit, context_->ui->camera_max_dist_edit,
                     context_->ui->camera_image_width_edit);
}

void CameraComponent::OnChangeCamera() {
    SetSceneCamera(context_->ui->camera_pos_x_edit, context_->ui->camera_pos_y_edit,
                   context_->ui->camera_pos_z_edit, context_->ui->camera_rot_roll_edit,
                   context_->ui->camera_rot_pitch_edit, context_->ui->camera_rot_yaw_edit,
                   context_->ui->camera_focal_len_edit, context_->ui->camera_sensor_width_edit,
                   context_->ui->camera_sensor_height_edit, context_->ui->camera_max_dist_edit,
                   context_->ui->camera_image_width_edit, context_->scene.get());

    modification_monitor_comp_->OnModifyScene();
}

} // namespace e8
