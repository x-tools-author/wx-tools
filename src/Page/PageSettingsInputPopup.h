/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "Common/wxTools.h"
#include "Utilities/BaseSettingsPopup.h"

struct PageSettingsInputPopupParameterKeys
{
    std::string prefix = {"prefix"};
    std::string suffix = {"suffix"};
    std::string escIndex = {"esc"};

    std::string startIndex = {"startIndex"};
    std::string endIndex = {"endIndex"};
    std::string algorithm = {"algorithm"};
    std::string addCrc = {"addCrc"};
    std::string bigEndian = {"bigEndian"};
};

class wxSpinCtrl;
class PageSettingsInputPopup : public BaseSettingsPopup
{
public:
    PageSettingsInputPopup(wxButton *controlButton);

    void DoLoad(const wxtJson &json);
    wxtJson DoSave() const;

private:
    wxComboBox *m_prefixComboBox{nullptr};
    wxComboBox *m_suffixComboBox{nullptr};
    wxComboBox *m_escComboBox{nullptr};

    wxSpinCtrl *m_startIndexSpinCtrl{nullptr};
    wxSpinCtrl *m_endIndexSpinCtrl{nullptr};
    wxComboBox *m_algorithmComboBox{nullptr};
    wxCheckBox *m_addCrcCheckBox{nullptr};
    wxCheckBox *m_bigEndianCheckBox{nullptr};

private:
    void OnParametersChanged(wxCommandEvent &event);
};
