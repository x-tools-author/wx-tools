﻿/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "MainWindow.h"

#include <fmt/format.h>
#include <fstream>
#include <wx/artprov.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "128x128.xpm"
#include "Page/Page.h"

MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, fmt::format("wxTools v{}", std::string(WXT_GIT_TAG)))
    , m_statusBar(nullptr)
{
#if !defined(WXT_RELEASE)
#if 0
    [Ubuntu]
    I20250219 13:13:10.775637 139801817956736 MainWindow.cpp:29] GetExecutablePath():/home/user/Project/wx-tools/build/Desktop_Qt_6_8_2-Debug/assets/wxTools/wxTools
    I20250219 13:13:10.775692 139801817956736 MainWindow.cpp:30] GetConfigDir():/etc
    I20250219 13:13:10.775699 139801817956736 MainWindow.cpp:31] GetUserConfigDir():/home/user
    I20250219 13:13:10.775705 139801817956736 MainWindow.cpp:32] GetDataDir():/usr/local/share/wxTools
    I20250219 13:13:10.775712 139801817956736 MainWindow.cpp:33] GetUserDataDir():/home/user/.wxTools
    I20250219 13:13:10.775716 139801817956736 MainWindow.cpp:34] GetUserLocalDataDir():/home/user/.wxTools
    I20250219 13:13:10.775719 139801817956736 MainWindow.cpp:35] GetPluginsDir():/usr/local/lib/wxTools
    I20250219 13:13:10.775722 139801817956736 MainWindow.cpp:36] MakeConfigFileName():wxTools.conf
    I20250219 13:13:10.775734 139801817956736 MainWindow.cpp:37] GetGlobalFileName()/etc/wxTools.conf
    I20250219 13:13:10.775746 139801817956736 MainWindow.cpp:38] GetLocalFileName()/home/user/.wxTools
    I20250219 13:13:10.775746 139801817956736 MainWindow.cpp:38] wxGetCwd():/home/user/Project/wx-tools/build/Desktop_Qt_6_8_2-Debug/assets/wxTools
#endif
    wxtInfo() << "GetExecutablePath():" << wxStandardPaths::Get().GetExecutablePath();
    wxtInfo() << "GetConfigDir():" << wxStandardPaths::Get().GetConfigDir();
    wxtInfo() << "GetUserConfigDir():" << wxStandardPaths::Get().GetUserConfigDir();
    wxtInfo() << "GetDataDir():" << wxStandardPaths::Get().GetDataDir();
    wxtInfo() << "GetUserDataDir():" << wxStandardPaths::Get().GetUserDataDir();
    wxtInfo() << "GetUserLocalDataDir():" << wxStandardPaths::Get().GetUserLocalDataDir();
    wxtInfo() << "GetPluginsDir():" << wxStandardPaths::Get().GetPluginsDir();
    wxtInfo() << "MakeConfigFileName():" << wxStandardPaths::Get().MakeConfigFileName("wxTools");
    wxtInfo() << "GetGlobalFileName():" << wxFileConfig::GetGlobalFileName("wxTools");
    wxtInfo() << "GetLocalFileName():" << wxFileConfig::GetLocalFileName("wxTools");
    wxtInfo() << "wxGetCwd():" << wxGetCwd();
#endif

    Init();

#if 0
#if defined(WIN32)
    wxIcon icon;
    icon.LoadFile("IDI_ICON1", wxBITMAP_TYPE_ICO_RESOURCE);
    SetIcon(icon);
#endif
#else
    SetIcon(wxIcon(wxtLogo));
#endif

    m_notebook = new wxNotebook(this, wxID_ANY);
    auto types = GetSuportedLinkTypes();
    for (auto type : types) {
        auto page = new Page(type, m_notebook);
        m_notebook->AddPage(page, GetLinkName(type));
        m_pageMap[type] = page;
    }

    m_updateTimeTimer.Bind(wxEVT_TIMER, [this](wxTimerEvent&) {
        this->m_statusBar->SetStatusText(wxDateTime::Now().FormatTime(), 1);
    });

    DoLoad();

    auto const sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_notebook, 1, wxEXPAND | wxALL, 0);
    SetSizerAndFit(sizer);

    SetSize(wxSize(1024, 600));
    Centre();
}

MainWindow::~MainWindow()
{
    DoSave();
}

void MainWindow::OnOpen(wxCommandEvent&)
{
    wxFileDialog openFileDialog(this,
                                _("Open parameters file"),
                                wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Desktop),
                                "wxTools.json",
                                _("JSON files (*.json)|*.json"),
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    openFileDialog.ShowModal();
    wxString fileName = openFileDialog.GetPath();
    DoLoad(fileName);
}

void MainWindow::OnSave(wxCommandEvent&)
{
    DoSave();
}

void MainWindow::OnSaveAs(wxCommandEvent&)
{
    wxFileDialog saveFileDialog(this,
                                _("Save parameters file"),
                                wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Desktop),
                                "wxTools.json",
                                _("JSON files (*.json)|*.json"),
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    saveFileDialog.ShowModal();
    wxString fileName = saveFileDialog.GetPath();
    wxtInfo() << fileName;
    DoSave(fileName);
}

void MainWindow::OnNew(wxCommandEvent&)
{
    wxExecute(wxStandardPaths::Get().GetExecutablePath());
}

void MainWindow::OnExit(wxCommandEvent&)
{
    wxLogMessage(_("The application will be closed!"), this);
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent&)
{
    // __DATE__ __TIME__ to wxDateTime
    wxDateTime dt;
    dt.ParseDate(__DATE__);
    wxString d = dt.FormatDate();
    wxDateTime tm;
    tm.ParseTime(__TIME__);
    wxString t = tm.FormatTime();

    wxString info = _("wxTools - A set of tools developed with wxWidgets\n");
    info += "\n";
    info += _("Version: ") + std::string(WXT_GIT_TAG) + std::string("\n");
    info += _("Author: x-tools-author\n");
    info += _("Email: x-tools@outlook.com\n");
    info += "\n";
    info += _("Commit: ") + std::string(WXT_GIT_COMMIT) + std::string("\n");
    info += _("Date: ") + std::string(WXT_GIT_COMMIT_TIME) + std::string("\n");
    info += _("Build: ") + d + wxString(" ") + t + std::string("\n");
    info += std::string("\n");
    info += _("Copyright");
    info += fmt::format(" 2024-{} x-tools-author. ", std::string(__DATE__).substr(7, 4));
    info += _("All rights reserved.\n");
    wxMessageBox(info, _("About wxTools"), wxOK | wxICON_INFORMATION, GetParent());
}

void MainWindow::Init()
{
    InitMenu();
    InitStatusBar();
}

void MainWindow::InitMenu()
{
    wxMenuBar* menuBar = new wxMenuBar;
    InitMenuFile(menuBar);
    InitMenuOptions(menuBar);
    InitMenuHelp(menuBar);

    SetMenuBar(menuBar);
}

void MainWindow::InitMenuFile(wxMenuBar* menuBar)
{
    wxMenu* menuFile = new wxMenu;
    menuBar->Append(menuFile, _("File"));

    menuFile->Append(wxID_NEW);
    menuFile->Append(wxID_OPEN);
    menuFile->Append(wxID_SAVE);
    menuFile->Append(wxID_SAVEAS);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    Bind(wxEVT_MENU, &MainWindow::OnNew, this, wxID_NEW);
    Bind(wxEVT_MENU, &MainWindow::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainWindow::OnSave, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MainWindow::OnSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);
}

void MainWindow::InitMenuOptions(wxMenuBar* menuBar)
{
    wxMenu* menuOptions = new wxMenu;
    menuBar->Append(menuOptions, _("Options"));

    wxMenuItem* item = menuOptions->Append(wxID_ANY, _("Open Log Directory"));
    item->SetHelp(_("Open the log directory."));
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(LogPath()); }, item->GetId());

    // clang-format off
    item = menuOptions->Append(wxID_ANY, _("Open Config Directory"));
    item->SetHelp(_("Open the config directory."));
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(GetSettingsPath()); }, item->GetId());
    // clang-format on

    item = menuOptions->Append(wxID_ANY, _("Open App Directory"));
    item->SetHelp(_("Open the app directory."));
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(wxtDataDir); }, item->GetId());
}

void MainWindow::InitMenuHelp(wxMenuBar* menuBar)
{
    wxMenu* menuHelp = new wxMenu;
    menuBar->Append(menuHelp, _("Help"));

    menuHelp->Append(wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);

    int tmpId = wxtNewID();
    wxMenuItem* item = menuHelp->Append(tmpId, _("About wxWidgets"));
    item->SetHelp(_("Show wxWidgets library information."));
    Bind(wxEVT_MENU, [this](wxCommandEvent&) { wxInfoMessageBox(GetParent()); }, tmpId);

    item = menuHelp->Append(wxID_HELP);
    item->SetHelp(_("Visit online documentation page."));
    static const wxString helpUrl{"https://x-tools-author.github.io/wx-tools/"};
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(helpUrl); }, wxID_HELP);

    item = menuHelp->Append(wxID_ANY, _("Check for Updates"));
    item->SetHelp(_("Check for updates online."));
    Bind(wxEVT_MENU, &MainWindow::DoCheckForUpdates, this, item->GetId());

    item = menuHelp->Append(wxID_ANY, _("Report an Issue"));
    item->SetHelp(_("Report a Issue online."));
    static const wxString bugUrl{"https://github.com/x-tools-author/wx-tools/issues/new"};
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(bugUrl); }, item->GetId());

    menuHelp->AppendSeparator();

    // if history file not found, ignore this menu item
    wxString historyFileName = GetHistoryFileName();
    if (wxFileExists(historyFileName)) {
        tmpId = wxtNewID();
        menuHelp->Append(tmpId, _("History"), _("Show the history of the application."));
        Bind(wxEVT_MENU, &MainWindow::DoShowHistory, this, tmpId);
        menuHelp->AppendSeparator();
    }

    wxString help = _("Visit GitHub page to get more information.");
    item = menuHelp->Append(wxID_ANY, _("Get Source from GitHub"), help);
    static const wxString githubUrl{"https://github.com/x-tools-author/wx-tools"};
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(githubUrl); }, item->GetId());

    help = _("Visit Gitee page to get more information.");
    item = menuHelp->Append(wxID_ANY, _("Get Source from Gitee"), help);
    static const wxString giteeUrl{"https://gitee.com/x-tools-author/wx-tools"};
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(giteeUrl); }, item->GetId());

    help = _("Visit GitHub home page of author.");
    item = menuHelp->Append(wxID_ANY, _("GitHub Home Page"), help);
    static const wxString githubHomeUrl{"https://github.com/x-tools-author"};
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(githubHomeUrl); }, item->GetId());

    help = _("Visit Gitee home page of author.");
    item = menuHelp->Append(wxID_ANY, _("Gitee Home Page"), help);
    static const wxString giteeHomeUrl{"https://gitee.com/x-tools-author"};
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(giteeHomeUrl); }, item->GetId());

#if defined(WIN32)
    menuHelp->AppendSeparator();

    help = _("Visit Microsoft Store page to buy wxTools.");
    item = menuHelp->Append(wxID_ANY, _("Supporting Author"), help);
    static const wxString storeUrl{"https://apps.microsoft.com/detail/9nx1d0ccv9t7"};
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(storeUrl); }, item->GetId());

    help = _("Visit Microsoft Store home page of author.");
    item = menuHelp->Append(wxID_ANY, _("Other Applications"), help);
    static wxString storeHomeUrl{"https://apps.microsoft.com/search/publisher?name=x-tools-author"};
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxLaunchDefaultBrowser(storeHomeUrl); }, item->GetId());
#endif
}

void MainWindow::InitStatusBar()
{
    m_statusBar = CreateStatusBar(2);
    int widths[] = {-1, 60};
    SetStatusWidths(2, widths);

    m_updateTimeTimer.Start(1000);
}

std::string GetPageParameterFileName(LinkType type)
{
    wxString name = GetLinkRawName(type);
    name.Replace(" ", "");
    return name.ToStdString() + ".json";
}

void MainWindow::DoLoad(wxString fileName)
{
    if (fileName.IsEmpty()) {
        fileName = GetSettingsFileName();
    }

    if (!wxFileName::Exists(fileName)) {
        return;
    }

    std::ifstream ifs(fileName.ToStdString());
    wxtJson json;
    ifs >> json;

    if (!json.is_object()) {
        return;
    }

    for (auto it = m_pageMap.begin(); it != m_pageMap.end(); ++it) {
        Page* page = it->second;
        wxString name = GetPageParameterFileName(it->first);
        if (!json.contains(name.ToStdString())) {
            continue;
        }

        wxtJson pageJson = json[name.ToStdString()];
        if (pageJson.is_object()) {
            page->DoLoad(pageJson);
        }
    }

    int tabIndex = wxtConfig->Read("MainWindow/tabIndex", long(0));
    if (tabIndex >= 0 && tabIndex < m_notebook->GetPageCount()) {
        m_notebook->SetSelection(tabIndex);
    }
}

void MainWindow::DoSave(wxString fileName)
{
    wxtConfig->Write("MainWindow/tabIndex", m_notebook->GetSelection());

    wxtJson wxTools = wxtJson::object();
    for (auto it = m_pageMap.begin(); it != m_pageMap.end(); ++it) {
        Page* page = it->second;
        wxtJson json = page->DoSave();
        wxString name = GetPageParameterFileName(it->first);
        wxTools[name.ToStdString()] = json;
    }

    // Write json to file
    if (fileName.IsEmpty()) {
        fileName = GetSettingsFileName();
    }

    std::ofstream ofs(fileName.ToStdString());
    ofs << wxTools.dump(4);
    ofs.close();
}

void MainWindow::DoCheckForUpdates(wxCommandEvent&)
{
#if defined(WXT_PORTABLE_EDITION)
    wxLaunchDefaultBrowser("https://github.com/x-tools-author/wx-tools/releases");
#else
#if defined(WIN32)
    wxExecute(wxString("cmd /C start ms-windows-store://pdp/?productid=9NX1D0CCV9T7"));
#endif
#endif
}

void MainWindow::DoShowHistory(wxCommandEvent&)
{
    wxString path = GetHistoryFileName();
    if (!wxFileExists(path)) {
        wxMessageBox(_("History file not found!"), _("Error"), wxOK | wxICON_ERROR);
        return;
    }

    wxDialog dlg(this, wxID_ANY, _("wxTools Release History"), wxDefaultPosition, wxSize(600, 400));
    dlg.SetIcon(wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, wxSize(64, 64)));
    wxTextCtrl* textCtrl = new wxTextCtrl(&dlg,
                                          wxID_ANY,
                                          wxEmptyString,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_READONLY);
    textCtrl->LoadFile(path);
    dlg.ShowModal();
}

wxString MainWindow::GetHistoryFileName() const
{
#if defined(__WINDOWS__)
    wxString path = wxStandardPaths::Get().GetDataDir();
    path += wxFileName::GetPathSeparator() + wxString("files");
    path += wxFileName::GetPathSeparator() + wxString("history.txt");
    return path;
#elif defined(__LINUX__)
    return wxString("/usr/share/doc/wx-tools/history.txt");
#elif defined(__APPLE__)
    return wxString("/Applications/wx-tools/history.txt");
#endif

    return wxEmptyString;
}