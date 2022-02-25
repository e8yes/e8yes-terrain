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

#ifndef ISLANDS_EDITOR_COMPONENT_MODIFICATION_MONITOR_H
#define ISLANDS_EDITOR_COMPONENT_MODIFICATION_MONITOR_H

#include <QObject>

#include "editor/basic/context.h"

namespace e8 {

/**
 * @brief The ModificationMonitorComponent class It responds to scene modification and reset events.
 */
class ModificationMonitorComponent : public QObject {
    Q_OBJECT

  public:
    ModificationMonitorComponent(EditorContext *context);
    ~ModificationMonitorComponent();

    /**
     * @brief UnsavedModifications Checks if there are any currently unsaved modifications to the
     * scene.
     */
    bool UnsavedModifications() const;

  public slots:
    void OnReset();
    void OnModifyScene();

  private:
    EditorContext *context_;
    bool unsaved_modifications_;
};

} // namespace e8

#endif // ISLANDS_EDITOR_COMPONENT_MODIFICATION_MONITOR_H
