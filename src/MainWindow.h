/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <map>
#include <wx/fileconf.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include "Common/wxTools.h"

class Page;
class MainWindow : public wxFrame
{
public:
    struct ParameterKeys
    {
        const std::string tabIndex = "tabIndex";
    };

public:
    MainWindow();
    ~MainWindow() override;

    void DoUpdateWindowFrameWork();

private:
    void Init();
    void InitMenu();
    void InitMenuFile(wxMenuBar* menuBar);
    void InitMenuOptions(wxMenuBar* menuBar);
    void InitMenuOptionsThemes(wxMenu* optionMenu);
    void InitMenuOptionsShowOnTop(wxMenu* optionMenu);
    void InitMenuHelp(wxMenuBar* menuBar);
    void InitMenuHelp3rdParty(wxMenu* menuHelp);
    void InitStatusBar();

    void OnOpen(wxCommandEvent&);
    void OnSave(wxCommandEvent&);
    void OnSaveAs(wxCommandEvent&);
    void OnNew(wxCommandEvent&);
    void OnExit(wxCommandEvent&);
    void OnClose(wxCloseEvent& evt);

    void OnAbout(wxCommandEvent&);
    void DoSave(wxString fileName = wxString());
    void DoLoad(wxString fileName = wxString());
    void DoCheckForUpdates(wxCommandEvent&);
    void DoShowHistory(wxCommandEvent&);

    wxString GetHistoryFileName() const;

private:
    std::map<LinkType, Page*> m_pageMap;
    wxStatusBar* m_statusBar;
    wxTimer m_updateTimeTimer;
    wxNotebook* m_notebook;

private:
    DECLARE_DYNAMIC_CLASS(MainWindow);
    DECLARE_EVENT_TABLE();
};
