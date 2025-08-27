/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/wx.h>

#include "Common/wxTools.h"

struct PageSettingsOutputParameterKeys
{
    std::string textFormat = {"textFormat"};
    std::string showDate = {"showDate"};
    std::string showTime = {"showTime"};
    std::string showMs = {"showMs"};
    std::string showRx = {"showRx"};
    std::string showTx = {"showTx"};
    std::string showFlag = {"showFlag"};
    std::string wrap = {"wrap"};
    std::string terminalMode = {"terminalMode"};
};

class TextFormatComboBox;
class PageSettingsOutputPopup;
class PageSettingsOutput : public wxStaticBoxSizer
{
public:
    PageSettingsOutput(wxWindow *parent);

    void DoLoad(const wxtJson &parameters);
    wxtJson DoSave() const;

    TextFormat GetTextFormat() const;
    bool GetShowDate() const;
    bool GetShowTime() const;
    bool GetShowMs() const;
    bool GetShowRx() const;
    bool GetShowTx() const;
    bool GetShowFlag() const;
    bool GetWrap() const;
    bool GetTerminalMode() const;
    PageSettingsOutputPopup *GetPopup();

private:
    TextFormatComboBox *m_textFormatComboBox;
    wxCheckBox *m_showDate;
    wxCheckBox *m_showTime;
    wxCheckBox *m_showMs;
    wxCheckBox *m_showRx;
    wxCheckBox *m_showTx;
    wxCheckBox *m_showFlag;
    wxCheckBox *m_wrap;
    wxCheckBox *m_terminalMode;
    PageSettingsOutputPopup *m_popup;
    wxWindow *m_parent;

private:
    void DoUpdateCheckBoxesState();

    void OnTerminalModeStateChanged(wxCommandEvent &);
    void OnClear(wxCommandEvent &);
    void OnWrapModeStateChanged(wxCommandEvent &);
};
