/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/spinctrl.h>
#include <wx/wx.h>

#include "Common/wxTools.h"
#include "Utilities/BaseSettingsPopup.h"

struct PageSettingsLinkPopupParameterKeys
{

};

class PageSettingsLinkPopup : public BaseSettingsPopup
{
public:
    PageSettingsLinkPopup(wxButton *controlButton, wxWindow *parent);

    void Load(const wxtJson &parameters);
    wxtJson DoSave() const;

private:
    void OnRefresh(wxCommandEvent &);

private:
    wxWindow *m_parent;
    wxButton *m_refresh;
};
