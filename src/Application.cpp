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

bool Application::OnInit()
{
#if wxCHECK_VERSION(3, 3, 0)
    SetAppearance(Appearance::System);
#endif

    SetAppName("wxTools");
    SetVendorName("xTools");
    mg_log_set(MG_LL_NONE);

#if defined(WXT_RELEASE)
    DoInitLogging("wxTools");
#endif

    wxString i18nDir = wxGetCwd();
    i18nDir += wxFileName::GetPathSeparator();
    i18nDir += wxString("i18n");

    m_locale.AddCatalogLookupPathPrefix(i18nDir);
    m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT);
    m_locale.AddCatalog(wxString("wxTools"));
    m_locale.AddCatalog(wxString("wxWidgets"));
    wxtInfo() << "Application booting...";
    wxtInfo() << "--------------------------------------------------------------------------------";
    wxtInfo() << "Settings file is:" << GetSettingsFileName();

    auto* mainWindow = new MainWindow();
#if defined(WXT_MAGIC)
    mainWindow->SetTransparent(50);
#endif
    mainWindow->Show(true);
    return true;
}

Application::~Application()
{
#if defined(WXT_RELEASE)
    DoShutdownLogging();
#endif
}
