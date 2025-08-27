/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/popupwin.h>
#include <wx/wx.h>

class BaseSettingsPopup : public wxPopupTransientWindow
{
public:
    BaseSettingsPopup(wxButton *controlButton, bool showOnBottom = true);

protected:
    bool ProcessLeftDown(wxMouseEvent &event) override;

private:
    wxButton *m_settingsButton;
    bool m_showOnBottom;

private:
    void OnSettingsButtonClicked(wxMouseEvent &event);
    void OnGlobalMouseClick(wxMouseEvent &event);
};
