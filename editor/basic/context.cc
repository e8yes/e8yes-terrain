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

#include <QList>
#include <QWidget>
#include <memory>

#include "editor/basic/context.h"
#include "ui_window_editor.h"

namespace e8 {

EditorContext::EditorContext() : ui(std::make_unique<Ui::IslandsEditorWindow>()), running(true) {}

EditorContext::~EditorContext() {}

void DeepScanWidget(QWidget *target, std::vector<QWidget *> *result) {
    if (target == nullptr) {
        return;
    }

    result->push_back(target);

    QList<QWidget *> children = target->findChildren<QWidget *>();
    for (auto child : children) {
        result->push_back(child);
    }
}

} // namespace e8