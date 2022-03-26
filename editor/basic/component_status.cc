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

#include <QMainWindow>
#include <QObject>
#include <QString>
#include <boost/log/trivial.hpp>
#include <string>

#include "content/scene.h"
#include "editor/basic/component_status.h"
#include "editor/basic/context.h"
#include "ui_window_editor.h"

namespace e8 {
namespace {

constexpr char const *kEditorWindowSuffix = "e8 islands editor";

QString SceneName(Game *game) {
    Scene const &scene = *game->GetGameData().scene;
    return QString::fromStdString(scene.name + '@' + std::to_string(scene.id).substr(0, 3));
}

} // namespace

StatusComponent::StatusComponent(QMainWindow *editor_window, EditorContext *context)
    : editor_window_(editor_window), modified_(false), context_(context) {
    editor_window_->setWindowTitle(kEditorWindowSuffix);
}

StatusComponent::~StatusComponent() {}

void StatusComponent::Update() {
    if (context_->game == nullptr) {
        editor_window_->setWindowTitle(kEditorWindowSuffix);
        return;
    }

    if (modified_) {
        editor_window_->setWindowTitle(SceneName(context_->game.get()) + "* - " +
                                       kEditorWindowSuffix);
    } else {
        editor_window_->setWindowTitle(SceneName(context_->game.get()) + " - " +
                                       kEditorWindowSuffix);
    }
}

void StatusComponent::OnChangeScene() { this->Update(); }

void StatusComponent::SetModificationStatus(bool modified) {
    if (context_->game == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "SetModificationStatus(): Setting modification status when "
                                    "the game has been closed.";
        return;
    }

    modified_ = modified;
    this->Update();
}

} // namespace e8
