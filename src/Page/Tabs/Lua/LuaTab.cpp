/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "LuaTab.h"

#include <wx/artprov.h>

#include "Common/wxTools.h"
#include "LuaRunner.h"

LuaTab::LuaTab(wxWindow *parent)
    : wxPanel(parent, wxID_ANY)
    , m_luaRunner(nullptr)
    , m_controllerBoxSizer(nullptr)
    , m_mainSizer(nullptr)
    , m_textCtrl(nullptr)
    , m_fileComboBox(nullptr)
    , m_newButton(nullptr)
    , m_openDirButton(nullptr)
    , m_refreshButton(nullptr)
    , m_helpButton(nullptr)
{
    m_controllerBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    m_mainSizer = new wxGridBagSizer(0, 0);

    DoSetupUi();

    SetSizer(m_mainSizer);
    Layout();
}

LuaTab::~LuaTab() {}

void LuaTab::DoSetupUi()
{
    DoSetupUiControllers();
    DoSetupUiTextCtrl();
}

void LuaTab::DoSetupUiControllers()
{
    m_fileComboBox = new wxComboBox(this,
                                    wxID_ANY,
                                    wxEmptyString,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    0,
                                    nullptr,
                                    wxCB_READONLY);
    m_controllerBoxSizer->Add(m_fileComboBox, 1, wxEXPAND | wxALL, 4);
    m_runButton = new wxBitmapButton(this, wxID_ANY, wxNullBitmap);
    m_runButton->Bind(wxEVT_BUTTON, &LuaTab::OnRunButtonClicked, this);
    DoUpdateRunButtonState();

    auto newIcon = wxArtProvider::GetBitmapBundle(wxART_PLUS, wxART_BUTTON);
    m_newButton = new wxBitmapButton(this, wxID_ANY, newIcon);
    m_newButton->SetToolTip(_("New Lua Script"));

    auto openDirIcon = wxArtProvider::GetBitmapBundle(wxART_FOLDER_OPEN, wxART_BUTTON);
    m_openDirButton = new wxBitmapButton(this, wxID_ANY, openDirIcon);
    m_openDirButton->SetToolTip(_("Open Lua Script Directory"));

    auto refreshIcon = wxArtProvider::GetBitmapBundle(wxART_REFRESH, wxART_BUTTON);
    m_refreshButton = new wxBitmapButton(this, wxID_ANY, refreshIcon);
    m_refreshButton->SetToolTip(_("Refresh Lua Script List"));

    auto helpIcon = wxArtProvider::GetBitmapBundle(wxART_HELP, wxART_BUTTON);
    m_helpButton = new wxBitmapButton(this, wxID_ANY, helpIcon);
    m_helpButton->SetToolTip(_("Visit Online Manual"));

    m_controllerBoxSizer->Add(m_runButton, 0, wxEXPAND | wxALL, 4);
    m_controllerBoxSizer->Add(m_newButton, 0, wxEXPAND | wxALL, 4);
    m_controllerBoxSizer->Add(m_openDirButton, 0, wxEXPAND | wxALL, 4);
    m_controllerBoxSizer->Add(m_refreshButton, 0, wxEXPAND | wxALL, 4);
    m_controllerBoxSizer->Add(m_helpButton, 0, wxEXPAND | wxALL, 4);
}

void LuaTab::DoSetupUiTextCtrl()
{
    m_textCtrl = new wxTextCtrl(this,
                                wxID_ANY,
                                wxEmptyString,
                                wxDefaultPosition,
                                wxDefaultSize,
                                wxTE_MULTILINE | wxTE_DONTWRAP);

    m_mainSizer->Add(m_controllerBoxSizer, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    m_mainSizer->Add(m_textCtrl, wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 4);
    m_mainSizer->AddGrowableCol(0);
    m_mainSizer->AddGrowableRow(1);
}

void LuaTab::DoUpdateRunButtonState()
{
    if (m_luaRunner) {
        const wxString stopIconPath = GetIconPath("Stop.svg");
        auto stopIcon = wxBitmapBundle::FromSVGFile(stopIconPath, wxSize(16, 16));
        m_runButton->SetBitmap(stopIcon);
        m_runButton->SetToolTip(_("Stop Running Lua Script"));
    } else {
        const wxString runIconPath = GetIconPath("Play.svg");
        auto runIcon = wxBitmapBundle::FromSVGFile(runIconPath, wxSize(16, 16));
        m_runButton->SetBitmap(runIcon);
        m_runButton->SetToolTip(_("Start Running Lua Script"));
    }
}

void LuaTab::OnRunButtonClicked(wxCommandEvent &event)
{
    if (m_luaRunner) {
        m_luaRunner->Delete();
        m_luaRunner = nullptr;
    } else {
        m_luaRunner = new LuaRunner("test.lua", this);
        m_luaRunner->Run();
    }

    DoUpdateRunButtonState();
}