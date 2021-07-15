/*
    src/folderdialog.cpp -- Simple "OK" or "Yes/No"-style modal dialogs

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/folderdialog.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/label.h>
#include <nanogui/screen.h>
#include <nanogui/scrollpanel.h>
#include <filesystem>

#if WIN32
#include <windows.h> 
#else
#include <fileapi.h>
#endif

NAMESPACE_BEGIN(nanogui)

FolderDialog::FolderDialog(Widget* parent, const std::string& title,
    const std::string& starting_folder) : Window(parent, title, true) {

    set_modal(true);

    const std::string ext("\\");

    if (std::filesystem::exists(starting_folder))
        m_current_folder = starting_folder;
    else m_current_folder = std::filesystem::current_path().string();

    if (m_current_folder != ext &&
        m_current_folder.size() > ext.size() &&
        m_current_folder.substr(m_current_folder.size() - ext.size()) == "\\")
        m_current_folder = m_current_folder.substr(0, m_current_folder.size() - ext.size());

    //Widget *top_panel = new Widget(this);
    //top_panel->set_layout(new BoxLayout(Orientation::Vertical,
    //                                 Alignment::Middle, 10, 15));
    set_layout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 15));
    ScrollPanel* scroll = new ScrollPanel(this);
    scroll->set_scroll_type(ScrollPanel::ScrollTypes::Both);

    Widget* Container = new Widget(scroll);
    Container->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Middle));

    current_location = new Label(Container, m_current_folder);
    current_location->set_color(Color(0, 204, 0, 255));
    current_location->set_font_size(25);

    explore_treeview = new TreeView(Container);
    explore_treeview->set_fixed_size(Vector2i(300, 400));

    Widget* panel_buttons = new Widget(Container);
    panel_buttons->set_layout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 15));

    Button* ok_button = new Button(panel_buttons, "OK");
    ok_button->set_callback([&] { if (m_callback) m_callback(current_location->caption()); dispose(); });
    Button* cancel_button = new Button(panel_buttons, "Cancel");
    cancel_button->set_callback([&] { if (m_callback) m_callback(""); dispose(); });

    start_items();

    center();
    request_focus();
}

void FolderDialog::start_items()
{
    std::filesystem::path Curr_Root(m_current_folder);
    NanoTree* CurNanoTree = new NanoTree();
    explore_treeview->set_expand_callback([=](std::string Node)
        {
            for (auto const& CurrChild : explore_treeview->items()->Objects[Node]->Children)
            {
                try
                {
                    FillChildren(CurrChild.first);
                }
                catch (const std::exception& e) {};
            }
        });

    //Put Top Root
    CurNanoTree->set_root("This_PC");
    CurNanoTree->Objects["This_PC"]->Name = "This_PC";
    CurNanoTree->Objects["This_PC"]->Expanded = true;
    explore_treeview->set_items(CurNanoTree);
    // put driver roots
#if WIN32
    DWORD Drives = GetLogicalDrives();
    for (int Cnt = 0; Cnt < 32; Cnt++)
    {
        if ((Drives >> Cnt) & 1)
        {
            std::string Drive = ((utf8)(Cnt + 'A')) + ":\\";
            CurNanoTree->add_node("This_PC", Drive);
            CurNanoTree->Objects[Drive]->Name = Drive;
            CurNanoTree->Objects[Drive]->CallBack = [=] {current_location->set_caption(Drive); explore_treeview->perform_layout(screen()->nvg_context()); };
            FillChildren(Drive);
        }
    }
#else
    TODO try these:
https://www.linuxquestions.org/questions/programming-9/how-to-get-a-list-of-physical-drives-in-c-538365/
https://stackoverflow.com/questions/7243988/how-to-list-the-harddisks-attached-to-a-linux-machine-using-c

#endif

    explore_treeview->set_items(explore_treeview->items());

    // step into the path

    char* str, * tofree;
    char* running;
    char* token;
    str = (char*)malloc(Curr_Root.string().length() + 1);
    tofree = str;

    strcpy(str, Curr_Root.string().c_str());

    std::string TmpFolder = "";
    while (true)
    {
        token = Mystrsep(&str);
        if (token == NULL)break;
        TmpFolder += std::string(token) + "\\";

        explore_treeview->arrow_callback(TmpFolder);
    }
    free(str);

    explore_treeview->perform_layout(screen()->nvg_context());
}

void FolderDialog::FillChildren(std::string Node)
{
    if (Node == "This_PC" || explore_treeview->items()->Objects[Node]->Children.size() > 0)return;// This_PC is just a node for the root. it does not represent a location
    for (auto& CurrObject : std::filesystem::directory_iterator(Node))
    {
        try
        {
            if (std::filesystem::is_directory(CurrObject))
            {
                std::string CompleteName = CurrObject.path().string() + "\\";
                if (explore_treeview->items()->Objects.find(CompleteName) == explore_treeview->items()->Objects.end())// node does not already exist
                {
                    explore_treeview->items()->add_node(Node, CompleteName);
                    explore_treeview->items()->Objects[CompleteName]->Name = CurrObject.path().filename().string();
                    explore_treeview->items()->Objects[CompleteName]->CallBack = [=] {current_location->set_caption(CompleteName); this->perform_layout(screen()->nvg_context()); };
                }
            }
        }
        catch (const std::exception& e) {};
    }
}

// taken from here: https://stackoverflow.com/questions/8512958/is-there-a-windows-variant-of-strsep-function
char* FolderDialog::Mystrsep(char** stringp)
{
    char* start = *stringp;
    char* p;

    p = (start != NULL) ? strpbrk(start, "\\") : NULL;

    if (p == NULL)
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}
NAMESPACE_END(nanogui)
