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

#ifndef ISLANDS_EDITOR_CONTEXT_H
#define ISLANDS_EDITOR_CONTEXT_H

#include <QApplication>
#include <QWidget>
#include <memory>
#include <thread>
#include <vector>

#include "game/game.h"
#include "game/storyline.h"
#include "ui_editor_window.h"

namespace e8 {

/**
 * @brief The EditorContext struct Stores the editor's global states.
 */
struct EditorContext {
    EditorContext(int &argc, char *argv[]);
    ~EditorContext();

    // The currently created Qt application.
    std::unique_ptr<QApplication> app;

    // The editor's UI.
    std::unique_ptr<Ui::IslandsEditorWindow> ui;

    // Created by CreateEditorStoryline().
    std::unique_ptr<Storyline> editor_storyline;

    // The current game being edited.
    std::unique_ptr<Game> game;

    // The thread
    std::unique_ptr<std::thread> game_thread;
};

/**
 * @brief RunEditorGame Runs a game with the specified editor storyline. This function should be run
 * in the game thread.
 */
void RunEditorGame(Game *game, Storyline *editor_storyline);

/**
 * @brief DeepScanWidget Returns a list of Qt widget including the target as well as all of its
 * children.
 */
void DeepScanWidget(QWidget *target, std::vector<QWidget *> *result);

} // namespace e8

#endif // ISLANDS_EDITOR_CONTEXT_H
