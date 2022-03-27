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
#include <QSlider>
#include <string>

#include "common/tensor.h"
#include "content/camera.h"
#include "content/proto/camera.pb.h"
#include "content/scene.h"
#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/context.h"
#include "editor/basic/editor_storyline.h"
#include "editor/environment/component_camera.h"
#include "game/game.h"

namespace e8 {
namespace {

void SetCameraWidgets(Scene *scene, QLineEdit *pos_x, QLineEdit *pos_y, QLineEdit *pos_z,
                      QLineEdit *left_x, QLineEdit *left_y, QLineEdit *left_z, QLineEdit *up_x,
                      QLineEdit *up_y, QLineEdit *up_z, QLineEdit *forward_x, QLineEdit *forward_y,
                      QLineEdit *forward_z, QLineEdit *focal_length, QLineEdit *sensor_width,
                      QLineEdit *sensor_height, QLineEdit *max_distance, QLineEdit *image_width) {
    Camera camera;
    {
        Scene::ReadAccess read_access = scene->GainReadAccess();
        camera = scene->camera;
    }

    pos_x->setText(std::to_string(camera.position(0)).c_str());
    pos_y->setText(std::to_string(camera.position(1)).c_str());
    pos_z->setText(std::to_string(camera.position(2)).c_str());

    left_x->setText(std::to_string(camera.basis().right(0)).c_str());
    left_y->setText(std::to_string(camera.basis().right(1)).c_str());
    left_z->setText(std::to_string(camera.basis().right(2)).c_str());

    up_x->setText(std::to_string(camera.basis().down(0)).c_str());
    up_y->setText(std::to_string(camera.basis().down(1)).c_str());
    up_z->setText(std::to_string(camera.basis().down(2)).c_str());

    forward_x->setText(std::to_string(camera.basis().back(0)).c_str());
    forward_y->setText(std::to_string(camera.basis().back(1)).c_str());
    forward_z->setText(std::to_string(camera.basis().back(2)).c_str());

    focal_length->setText(std::to_string(camera.focal_length()).c_str());
    sensor_width->setText(std::to_string(camera.sensor_width()).c_str());
    sensor_height->setText(std::to_string(camera.sensor_height()).c_str());
    max_distance->setText(std::to_string(camera.max_distance()).c_str());
    image_width->setText(std::to_string(camera.image_width()).c_str());
}

void SetSceneCamera(QLineEdit *pos_x, QLineEdit *pos_y, QLineEdit *pos_z, QLineEdit *left_x,
                    QLineEdit *left_y, QLineEdit *left_z, QLineEdit *up_x, QLineEdit *up_y,
                    QLineEdit *up_z, QLineEdit *forward_x, QLineEdit *forward_y,
                    QLineEdit *forward_z, QLineEdit *focal_length, QLineEdit *sensor_width,
                    QLineEdit *sensor_height, QLineEdit *max_distance, QLineEdit *image_width,
                    Scene *scene) {
    Camera camera;
    {
        Scene::ReadAccess read_access = scene->GainReadAccess();
        camera = scene->camera;
    }

    camera.set_position(0, pos_x->text().toFloat());
    camera.set_position(1, pos_y->text().toFloat());
    camera.set_position(2, pos_z->text().toFloat());

    camera.mutable_basis()->set_right(0, left_x->text().toFloat());
    camera.mutable_basis()->set_right(1, left_y->text().toFloat());
    camera.mutable_basis()->set_right(2, left_z->text().toFloat());

    camera.mutable_basis()->set_down(0, up_x->text().toFloat());
    camera.mutable_basis()->set_down(1, up_y->text().toFloat());
    camera.mutable_basis()->set_down(2, up_z->text().toFloat());

    camera.mutable_basis()->set_back(0, forward_x->text().toFloat());
    camera.mutable_basis()->set_back(1, forward_y->text().toFloat());
    camera.mutable_basis()->set_back(2, forward_z->text().toFloat());

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
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_pos_y_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_pos_z_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);

    QObject::connect(context_->ui->camera_basis_right_x_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_basis_right_y_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_basis_right_z_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);

    QObject::connect(context_->ui->camera_basis_down_x_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_basis_down_y_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_basis_down_z_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);

    QObject::connect(context_->ui->camera_basis_back_x_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_basis_back_y_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_basis_back_z_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);

    QObject::connect(context_->ui->camera_focal_len_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_sensor_width_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_sensor_height_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_max_dist_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);
    QObject::connect(context_->ui->camera_image_width_edit, &QLineEdit::textChanged, this,
                     &CameraComponent::OnChangeCameraParameters);

    EditorInteractionComponent *interaction_component = static_cast<EditorInteractionComponent *>(
        context_->editor_storyline->FindComponent(kEditorComponent));

    QObject::connect(&interaction_component->CameraControlTask()->transmitter,
                     &EditorCameraEventTransmitter::ValueChanged, this,
                     &CameraComponent::OnChangeCameraParametersFromGame,
                     Qt::ConnectionType::QueuedConnection);
}

CameraComponent::~CameraComponent() {}

void CameraComponent::OnChangeProject() {
    SetCameraWidgets(
        context_->game->GetGameData().scene, context_->ui->camera_pos_x_edit,
        context_->ui->camera_pos_y_edit, context_->ui->camera_pos_z_edit,
        context_->ui->camera_basis_right_x_edit, context_->ui->camera_basis_right_y_edit,
        context_->ui->camera_basis_right_z_edit, context_->ui->camera_basis_down_x_edit,
        context_->ui->camera_basis_down_y_edit, context_->ui->camera_basis_down_z_edit,
        context_->ui->camera_basis_back_x_edit, context_->ui->camera_basis_back_y_edit,
        context_->ui->camera_basis_back_z_edit, context_->ui->camera_focal_len_edit,
        context_->ui->camera_sensor_width_edit, context_->ui->camera_sensor_height_edit,
        context_->ui->camera_max_dist_edit, context_->ui->camera_image_width_edit);
}

void CameraComponent::OnChangeCameraParameters() {
    SetSceneCamera(context_->ui->camera_pos_x_edit, context_->ui->camera_pos_y_edit,
                   context_->ui->camera_pos_z_edit, context_->ui->camera_basis_right_x_edit,
                   context_->ui->camera_basis_right_y_edit, context_->ui->camera_basis_right_z_edit,
                   context_->ui->camera_basis_down_x_edit, context_->ui->camera_basis_down_y_edit,
                   context_->ui->camera_basis_down_z_edit, context_->ui->camera_basis_back_x_edit,
                   context_->ui->camera_basis_back_y_edit, context_->ui->camera_basis_back_z_edit,
                   context_->ui->camera_focal_len_edit, context_->ui->camera_sensor_width_edit,
                   context_->ui->camera_sensor_height_edit, context_->ui->camera_max_dist_edit,
                   context_->ui->camera_image_width_edit, context_->game->GetGameData().scene);

    modification_monitor_comp_->OnModifyProject();
}

void CameraComponent::OnChangeCameraParametersFromGame() {
    this->OnChangeProject();
    modification_monitor_comp_->OnModifyProject();
}

} // namespace e8
