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
#include <QWidget>
#include <memory>

#include "gui/islands_editor_window.h"
#include "renderer/display.h"
#include "ui_islands_editor_window.h"

IslandsEditorWindow::IslandsEditorWindow(e8::IslandsRendererContext *context, QWidget *parent)
    : QMainWindow(parent), context_(context), ui_(std::make_unique<Ui::IslandsEditorWindow>()) {
    ui_->setupUi(this);

    QWidget *display_wrapper = QWidget::createWindowContainer(context_->display);
    ui_->central_layout->addWidget(display_wrapper, 5);
}

IslandsEditorWindow::~IslandsEditorWindow() {}
