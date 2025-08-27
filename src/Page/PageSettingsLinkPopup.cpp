/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageSettingsLinkPopup.h"

#include <wx/gbsizer.h>
#include <wx/statline.h>
#include <wx/wx.h>

PageSettingsLinkPopup::PageSettingsLinkPopup(wxButton *controlButton, wxWindow *parent)
    : BaseSettingsPopup(controlButton)
    , m_parent(parent)
    , m_refresh(nullptr)
{
    wxPanel *panel = new wxPanel(this);
    m_refresh = new wxButton(panel, wxID_ANY, _("Refresh"));

    wxGridBagSizer *sizer = new wxGridBagSizer(4, 4);
    sizer->Add(m_refresh, wxGBPosition(0, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);

    sizer->AddGrowableCol(1);
    panel->SetSizerAndFit(sizer);

    int padding = 8;
    wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(panel, 1, wxEXPAND | wxALL, padding);
    panel->SetMinSize(wxSize(152, -1));
    SetSizerAndFit(panelSizer);

    m_refresh->Bind(wxEVT_BUTTON, &PageSettingsLinkPopup::OnRefresh, this);
}

void PageSettingsLinkPopup::Load(const wxtJson &parameters)
{
    wxUnusedVar(parameters);
}

wxtJson PageSettingsLinkPopup::DoSave() const
{
    wxtJson json = wxtJson::object();
    PageSettingsLinkPopupParameterKeys keys;
    wxUnusedVar(keys);
    return json;
}

void PageSettingsLinkPopup::OnRefresh(wxCommandEvent &)
{
    wxPostEvent(m_parent, wxCommandEvent(wxtEVT_SETTINGS_LINK_POPUP_REFRESH));
    Hide();
}
