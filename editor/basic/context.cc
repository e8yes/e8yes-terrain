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

#include <QApplication>
#include <QList>
#include <QWidget>
#include <memory>
#include <vector>

#include "editor/basic/context.h"
#include "editor/basic/editor_storyline.h"
#include "game/game.h"
#include "game/storyline.h"
#include "ui_window_editor.h"

namespace e8 {

EditorContext::EditorContext(int &argc, char *argv[])
    : app(std::make_unique<QApplication>(argc, argv)),
      ui(std::make_unique<Ui::IslandsEditorWindow>()), editor_storyline(CreateEditorStoryline()) {}

EditorContext::~EditorContext() {
    if (game != nullptr) {
        game->Shutdown();
        game_thread->join();
    }
}

void RunEditorGame(Game *game, Storyline *editor_storyline) { game->Run(editor_storyline); }

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
