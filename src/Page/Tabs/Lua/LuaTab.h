/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <wx/bmpbuttn.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

class LuaRunner;
class LuaTab : public wxPanel
{
public:
    LuaTab(wxWindow *parent);
    ~LuaTab() override;

private:
    void DoSetupUi();
    void DoSetupUiControllers();
    void DoSetupUiTextCtrl();

private:
    void DoUpdateRunButtonState();

    void OnRunButtonClicked(wxCommandEvent &event);

private:
    LuaRunner *m_luaRunner{nullptr};
    wxBoxSizer *m_controllerBoxSizer;
    wxGridBagSizer *m_mainSizer;

    wxTextCtrl *m_textCtrl;
    wxComboBox *m_fileComboBox;
    wxBitmapButton *m_runButton;
    wxBitmapButton *m_newButton;
    wxBitmapButton *m_openDirButton;
    wxBitmapButton *m_refreshButton;
    wxBitmapButton *m_helpButton;
};