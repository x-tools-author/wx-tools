/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageSettingsInput.h"

#include <wx/gbsizer.h>
#include <wx/wx.h>

#include "PageSettingsInputPopup.h"
#include "Utilities/TextFormatComboBox.h"

PageSettingsInput::PageSettingsInput(wxWindow* parent)
    : wxStaticBoxSizer(wxVERTICAL, parent, _("Input Settings"))
    , m_parent{parent}
{
    auto cycleText = new wxStaticText(GetStaticBox(), wxID_ANY, _("Cycle"));
    m_interval = InitCycleIntervalComboBox();
    auto formatText = new wxStaticText(GetStaticBox(), wxID_ANY, _("Format"));
    m_format = new TextFormatComboBox(GetStaticBox());
    m_settings = new wxButton(GetStaticBox(), wxID_ANY, _("Settings"));
    m_send = new wxButton(GetStaticBox(), wxID_ANY, _("Send"));
    m_popup = new PageSettingsInputPopup(m_settings);

    auto* sizer = new wxGridBagSizer(4, 4);
    sizer->Add(cycleText, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sizer->Add(m_interval, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    sizer->Add(formatText, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sizer->Add(m_format, wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    sizer->AddGrowableCol(1);
    Add(sizer, 0, wxEXPAND | wxALL, 0);

    AddSpacer(4);

    auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
#if defined(__WXOSX__)
    buttonSizer->Add(m_settings, 1, wxEXPAND | wxALL, 4);
    buttonSizer->Add(m_send, 1, wxEXPAND | wxALL, 4);
#else
    buttonSizer->Add(m_settings, 1, wxEXPAND | wxALL, 0);
    buttonSizer->Add(m_send, 1, wxEXPAND | wxALL, 0);
#endif
    Add(buttonSizer, 0, wxEXPAND | wxALL, 0);

    m_interval->Bind(wxEVT_COMBOBOX_CLOSEUP, &PageSettingsInput::OnIntervalChanged, this);
    m_send->Bind(wxEVT_BUTTON, &PageSettingsInput::OnSendButtonClicked, this);
    m_format->Bind(wxEVT_COMBOBOX_CLOSEUP, &PageSettingsInput::OnInputFormatChangehd, this);
    m_timer.Bind(wxEVT_TIMER, &PageSettingsInput::OnTimer, this);
}

void PageSettingsInput::DoLoad(const wxtJson& parameters)
{
    PageSettingsInputParameterKeys keys;
    // clang-format off
    int cycleInterval = wxtGetJsonObjValue<int>(parameters, keys.cycleInterval, -1);
    int textFormat = wxtGetJsonObjValue<int>(parameters, keys.textFormat, static_cast<int>(TextFormat::Hex));
    wxtJson popup = wxtGetJsonObjValue<wxtJson>(parameters, keys.popup, wxtJson::object());
    // clang-format on

    SetComboBoxSectionByIntClientData(m_interval, cycleInterval);
    SetComboBoxSectionByIntClientData(m_format, textFormat);
    m_popup->DoLoad(popup);
}

wxtJson PageSettingsInput::DoSave() const
{
    PageSettingsInputParameterKeys keys;
    wxtJson parameters;

    int selection = m_interval->GetSelection();
    void* clientData = m_interval->GetClientData(selection);
    if (clientData) {
        int cycleInterval = *static_cast<int*>(clientData);
        parameters[keys.cycleInterval] = cycleInterval;
    }

    selection = m_format->GetSelection();
    clientData = m_format->GetClientData(selection);
    if (clientData) {
        int textFormat = *static_cast<int*>(clientData);
        parameters[keys.textFormat] = textFormat;
    }

    parameters[keys.popup] = m_popup->DoSave();
    return parameters;
}

int PageSettingsInput::GetTextFormat() const
{
    int selection = m_format->GetSelection();
    void* clientData = m_format->GetClientData(selection);
    if (clientData) {
        return *static_cast<int*>(clientData);
    }

    return static_cast<int>(TextFormat::Hex);
}

void PageSettingsInput::DoStopTimer()
{
    m_timer.Stop();
    m_interval->SetSelection(0);
}

PageSettingsInputPopup* PageSettingsInput::GetPopup()
{
    return m_popup;
}

int PageSettingsInput::GetInterval() const
{
    int selection = m_interval->GetSelection();
    void* clientData = m_interval->GetClientData(selection);
    if (clientData) {
        return *static_cast<int*>(clientData);
    }

    return -1;
}

void PageSettingsInput::OnSendButtonClicked(wxCommandEvent&)
{
    wxPostEvent(m_parent, wxCommandEvent(wxtEVT_SETTINGS_INPUT_WRITE));
}

void PageSettingsInput::OnInputFormatChangehd(wxCommandEvent&)
{
    wxCommandEvent event(wxtEVT_SETTINGS_INPUT_FORMAT);
    event.SetInt(GetTextFormat());
    wxPostEvent(m_parent, event);
}

void PageSettingsInput::OnIntervalChanged(wxCommandEvent&)
{
    int interval = GetInterval();
    if (interval == -1) {
        m_timer.Stop();
    } else {
        m_timer.Start(interval);
    }
}

void PageSettingsInput::OnTimer(wxTimerEvent&)
{
    wxPostEvent(m_parent, wxCommandEvent(wxtEVT_SETTINGS_INPUT_WRITE));
}

wxComboBox* PageSettingsInput::InitCycleIntervalComboBox()
{
    auto cb = new wxComboBox(GetStaticBox(),
                             wxID_ANY,
                             wxEmptyString,
                             wxDefaultPosition,
                             wxDefaultSize,
                             0,
                             nullptr,
                             wxCB_READONLY);

    cb->Append(_("Disabled"), new int(-1));
    std::vector<int> items;
    for (int i = 10; i <= 100; i += 10) {
        items.push_back(i);
    }
    for (int i = 200; i <= 1000; i += 100) {
        items.push_back(i);
    }
    for (int i = 2000; i <= 10000; i += 1000) {
        items.push_back(i);
    }
    for (auto it = items.begin(); it != items.end(); ++it) {
        cb->Append(wxString::Format("%d ms", *it), new int(*it));
    }

    cb->SetSelection(0);
    return cb;
}
