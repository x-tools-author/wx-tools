/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/textctrl.h>

#include "Common/wxTools.h"
#include "Utilities/BaseSettingsPopup.h"

struct PageSettingsOutputPopupParameterKeys
{
    std::string fillter = {"fillter"};
};

class PageSettingsOutputPopup : public BaseSettingsPopup
{
public:
    PageSettingsOutputPopup(wxButton *controlButton);

    void Load(const wxtJson &parameters);
    wxtJson DoSave() const;

    wxArrayString GetFilter() const;

private:
    wxTextCtrl *m_filterTextCtrl;
};
