/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "BaseSettingsPopup.h"

BaseSettingsPopup::BaseSettingsPopup(wxButton *controlButton, bool showOnBottom)
#if 1
    : wxPopupTransientWindow(controlButton, wxBORDER_THEME | wxPU_CONTAINS_CONTROLS)
#else
#if defined(__WXMSW__)
    : wxPopupTransientWindow(controlButton, wxBORDER_THEME | wxPU_CONTAINS_CONTROLS)
#else
    : wxPopupTransientWindow(controlButton, wxBORDER_THEME)
#endif
#endif
    , m_settingsButton(controlButton)
    , m_showOnBottom(showOnBottom)
{
    if (controlButton) {
        controlButton->Bind(wxEVT_LEFT_DOWN, &BaseSettingsPopup::OnSettingsButtonClicked, this);
    }

    Bind(wxEVT_LEFT_DOWN, &BaseSettingsPopup::OnGlobalMouseClick, this);
}

bool BaseSettingsPopup::ProcessLeftDown(wxMouseEvent &event)
{
    return wxPopupTransientWindow::ProcessLeftDown(event);
}

void BaseSettingsPopup::OnSettingsButtonClicked(wxMouseEvent &event)
{
    wxPoint p = m_settingsButton->ClientToScreen(wxPoint(0, 0));
    if (m_showOnBottom) {
        SetPosition(wxPoint(p.x, p.y + m_settingsButton->GetSize().GetHeight()));
    } else {
        SetPosition(wxPoint(p.x, p.y - GetSize().GetHeight()));
    }

    if (IsShown()) {
        Dismiss();
    } else {
        Popup();
    }
}

void BaseSettingsPopup::OnGlobalMouseClick(wxMouseEvent &event)
{
    if (!GetScreenRect().Contains(event.GetPosition())) {
        Dismiss();
    } else {
        event.Skip();
    }
}
