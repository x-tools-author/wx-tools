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
    const std::string textFormat{"textFormat"};
    const std::string showDate{"showDate"};
    const std::string showTime{"showTime"};
    const std::string showMs{"showMs"};
    const std::string showRx{"showRx"};
    const std::string showTx{"showTx"};
    const std::string showFlag{"showFlag"};
    const std::string wrap{"wrap"};
    const std::string terminalMode{"terminalMode"};
    const std::string filter{"filter"};
};

class TextFormatComboBox;
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
    wxCheckBox *GetWrapCheckBox() const;
    wxCheckBox *GetTerminalModeCheckBox() const;
    wxButton *GetSaveButton() const;
    wxButton *GetClearButton() const;
    wxArrayString GetFilter() const;

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
    wxTextCtrl *m_filterTextCtrl;
    wxButton *m_saveButton;
    wxButton *m_clearButton;

private:
    void DoUpdateCheckBoxesState();
    void OnTerminalModeStateChanged(wxCommandEvent &);
};
