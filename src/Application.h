/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/wx.h>

class MainWindow;
class Application : public wxApp
{
public:
    ~Application() override;
    bool OnInit() override;
    int OnExit() override;

private:
    wxLocale m_locale;
    MainWindow* m_mainWindow;
};
