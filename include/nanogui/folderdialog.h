/*
    nanogui/messagedialog.h -- Simple "OK" or "Yes/No"-style modal dialogs

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once
#include <nanogui/window.h>
#include <nanogui/treeview.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class FolderDialog folerdialog.h nanogui/folerdialog.h
 *
 * \brief Simple select folder modal dialog.
 */
class NANOGUI_EXPORT FolderDialog : public Window {
    char* Mystrsep(char** stringp);
    void start_items();
    void FillChildren(std::string Node);
public:

    FolderDialog(Widget *parent, const std::string &title = "",
                  const std::string &starting_folder = "");

    std::function<void(std::string)> callback() const { return m_callback; }
    void set_callback(const std::function<void(std::string)> &callback) { m_callback = callback; }
protected:
    std::function<void(std::string)> m_callback;
    std::string m_current_folder;
    TreeView* explore_treeview;
    Label* current_location;
};

NAMESPACE_END(nanogui)
