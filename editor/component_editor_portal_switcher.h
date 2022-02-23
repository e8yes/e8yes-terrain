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

#ifndef ISLANDS_EDITOR_COMPONENT_EDITOR_PORTAL_SWITCHER_H
#define ISLANDS_EDITOR_COMPONENT_EDITOR_PORTAL_SWITCHER_H

#include <QObject>

#include "editor/component_editor_context.h"

namespace e8 {

/**
 * @brief The EditorPortalSwitcherComponent class
 */
class EditorPortalSwitcherComponent : public QObject {
    Q_OBJECT

  public:
    EditorPortalSwitcherComponent(EditorContext *context);
    ~EditorPortalSwitcherComponent();

    void SetEditorPortalEnabled(bool enabled);

  private:
    EditorContext *context_;
};

} // namespace e8

#endif // ISLANDS_EDITOR_COMPONENT_EDITOR_PORTAL_SWITCHER_H
