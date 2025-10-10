/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/timer.h>
#include <wx/wx.h>

#include "Common/wxTools.h"

struct PageSettingsInputParameterKeys
{
    std::string cycleInterval = {"cycleInterval"};
    std::string textFormat = {"textFormat"};
    std::string popup = {"popup"};
};

class PageSettingsInputPopup;
class TextFormatComboBox;
class PageSettingsInput : public wxStaticBoxSizer
{
public:
    PageSettingsInput(wxWindow *parent);

    void DoLoad(const wxtJson &parameters);
    wxtJson DoSave() const;
    int GetInterval() const;
    int GetTextFormat() const;
    void DoStopTimer();
    wxTimer *GetTimer() const;
    wxButton *GetSendButton() const;
    wxComboBox *GetTextFormatComboBox() const;
    TextFormatComboBox *GetFormatComboBox() const;
    PageSettingsInputPopup *GetPopup();

private:
    wxButton *m_settings{nullptr};
    wxButton *m_send{nullptr};
    wxComboBox *m_interval{nullptr};
    TextFormatComboBox *m_format{nullptr};
    PageSettingsInputPopup *m_popup{nullptr};
    wxTimer m_timer;

private:
    void OnIntervalChanged(wxCommandEvent &);

    wxComboBox *InitCycleIntervalComboBox();
};
