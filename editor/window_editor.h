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

#include <QKeyEvent>
#include <QMainWindow>
#include <QWidget>
#include <memory>

#include "content/scene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class IslandsEditorWindow;
}
QT_END_NAMESPACE

namespace e8 {

/**
 * @brief The IslandsEditorWindow class The editor panel.
 */
class IslandsEditorWindow : public QMainWindow {
    Q_OBJECT

  public:
    IslandsEditorWindow(SceneInterface *scene, QWidget *parent = nullptr);
    ~IslandsEditorWindow();

    void keyPressEvent(QKeyEvent *event) override;

  private:
    std::unique_ptr<Ui::IslandsEditorWindow> ui_;

    SceneInterface *scene_;
};

/**
 * @brief RunIslandsEditorWindow Runs the editor window. This function blocks until the editor
 * window is closed.
 */
void RunIslandsEditorWindow(SceneInterface *scene, int argc, char *argv[]);

} // namespace e8

#endif // ISLANDS_EDITOR_WINDOW_H
