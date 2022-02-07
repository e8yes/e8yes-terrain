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

#ifndef ISLANDS_EDITOR_WINDOW_H
#define ISLANDS_EDITOR_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <memory>

#include "renderer/context.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class IslandsEditorWindow;
}
QT_END_NAMESPACE

/**
 * @brief The TerrainEditorWindow class
 */
class IslandsEditorWindow : public QMainWindow {
    Q_OBJECT

  public:
    IslandsEditorWindow(e8::IslandsRendererContext *context, QWidget *parent = nullptr);
    ~IslandsEditorWindow();

  private:
    std::unique_ptr<Ui::IslandsEditorWindow> ui_;

    e8::IslandsRendererContext *context_;
};
#endif // ISLANDS_EDITOR_WINDOW_H
