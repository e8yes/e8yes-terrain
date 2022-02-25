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
#include <memory>
#include <thread>

#include "editor/basic/context.h"
#include "editor/window_display.h"
#include "editor/window_editor.h"

int main(int argc, char *argv[]) {
    auto editor_context = std::make_shared<e8::EditorContext>();

    std::thread editor_thread(e8::RunIslandsEditorWindow, editor_context, argc, argv);
    std::thread display_thread(e8::RunIslandsDisplay, editor_context, /*width=*/1024,
                               /*height=*/768);

    editor_thread.join();
    display_thread.join();

    return 0;
}
