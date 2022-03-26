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

#include <QObject>
#include <boost/log/trivial.hpp>

#include "editor/basic/component_modification_monitor.h"
#include "editor/basic/component_status.h"
#include "editor/basic/context.h"
#include "ui_window_editor.h"

namespace e8 {
namespace {

void SetSaverEnabled(bool enabled, Ui::IslandsEditorWindow *ui) {
    ui->action_save_scene->setEnabled(enabled);
}

} // namespace

ModificationMonitorComponent::ModificationMonitorComponent(StatusComponent *status_comp,
                                                           EditorContext *context)
    : status_comp_(status_comp), unsaved_modifications_(false), context_(context) {}

ModificationMonitorComponent::~ModificationMonitorComponent() {}

bool ModificationMonitorComponent::UnsavedModifications() const { return unsaved_modifications_; }

void ModificationMonitorComponent::OnReset() {
    unsaved_modifications_ = false;
    SetSaverEnabled(/*enabled=*/false, context_->ui.get());
    status_comp_->SetModificationStatus(false);
}

void ModificationMonitorComponent::OnModifyScene() {
    if (context_->game == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "OnModifyScene(): A null game was modified.";
    }

    unsaved_modifications_ = true;
    SetSaverEnabled(/*enabled=*/true, context_->ui.get());
    status_comp_->SetModificationStatus(true);
}

} // namespace e8
