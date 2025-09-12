/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Application.h"

#include <mongoose.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>

#include "Common/wxTools.h"
#include "MainWindow.h"

Application::~Application()
{
    DoShutdownLogging();
}

bool Application::OnInit()
{
    long theme = long(static_cast<int>(wxAppBase::Appearance::System));
    int appAppearance = wxtConfig->Read("Application/Theme", theme);
    SetAppearance(static_cast<wxAppBase::Appearance>(appAppearance));

    SetAppName("wxTools");
    SetVendorName("xTools");
    mg_log_set(MG_LL_NONE);

    DoInitLogging("wxTools");

    wxString appPath = wxStandardPaths::Get().GetExecutablePath();
#if defined(__WXOSX__)
    // Get the path of i18nDir
    wxString i18nDir = wxFileName(appPath).GetPath();
    i18nDir += wxFileName::GetPathSeparator();
    i18nDir += wxString("../Resources");
#else
    wxString i18nDir = wxFileName(appPath).GetPath();
#endif
    i18nDir += wxFileName::GetPathSeparator();
    i18nDir += wxString("i18n");

    m_locale.AddCatalogLookupPathPrefix(i18nDir);
    m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT);
    m_locale.AddCatalog(wxString("wxTools"));
    m_locale.AddCatalog(wxString("wxWidgets"));
    wxtInfo() << "Application booting...";
    wxtInfo() << "--------------------------------------------------------------------------------";
    wxtInfo() << "The I18N directory is:" << i18nDir;
    wxtInfo() << "Settings file is:" << GetSettingsFileName();

    m_mainWindow = new MainWindow();
#if defined(WXT_MAGIC)
    m_mainWindow->SetTransparent(50);
#endif
    m_mainWindow->Show(true);
    m_mainWindow->DoUpdateWindowFrameWork();
    return true;
}

int Application::OnExit()
{
    return wxApp::OnExit();
}