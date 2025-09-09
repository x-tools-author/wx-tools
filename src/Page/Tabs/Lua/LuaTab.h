/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <memory>

#include <wx/bmpbuttn.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

#include "Common/wxTools.h"

struct LuaTabParameterKeys
{
    std::string script{"script"};
};

class LuaRunner;
class LuaTab : public wxPanel
{
public:
    LuaTab(wxWindow *parent);
    ~LuaTab() override;

    wxtJson DoSave() const;
    void DoLoad(const wxtJson &parameters);
    void OnBytesRead(std::shared_ptr<char> data, int size);

private:
    void DoSetupUi();
    void DoSetupUiControllers();
    void DoSetupUiTextCtrl();

private:
    void DoUpdateRunButtonState();
    void DoLoadLuaFileList();
    void DoLoadLuaFileListApp();
    void DoLoadLuaFileListUser();
    void DoAddLuaFileToList(const wxString &filePath);

    void DoOpenLuaRunner();
    void DoCloseLuaRunner();
    void DoUpdateUiEnabledState(bool enabled);

    void OnRunButtonClicked(wxCommandEvent &event);
    void OnLuaFileComboBoxSelected();
    void OnLuaTextCtrlChanged(wxCommandEvent &);
    void OnOpenDirButtonClicked(wxCommandEvent &);
    void OnHelpButtonClicked(wxCommandEvent &event);
    void OnNewButtonClicked(wxCommandEvent &event);
    void OnRefreshButtonClicked(wxCommandEvent &event);

    void OnThreadFinished(wxThreadEvent &event);
    void OnThreadError(wxThreadEvent &event);
    void OnThreadInvokeWrite(wxThreadEvent &event);

    wxString GetCurrentLuaFilePath();

private:
    LuaRunner *m_luaRunner{nullptr};
    wxBoxSizer *m_controllerBoxSizer;
    wxGridBagSizer *m_mainSizer;

    wxTextCtrl *m_luaTextCtrl;
    wxTextCtrl *m_logTextCtrl;
    wxComboBox *m_fileComboBox;
    wxBitmapButton *m_runButton;
    wxBitmapButton *m_newButton;
    wxBitmapButton *m_openDirButton;
    wxBitmapButton *m_refreshButton;
    wxBitmapButton *m_helpButton;

private:
    DECLARE_DYNAMIC_CLASS(LuaTab);
    DECLARE_EVENT_TABLE();
};