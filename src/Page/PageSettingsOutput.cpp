/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageSettingsOutput.h"

#include <wx/gbsizer.h>

#include "PageSettingsOutputPopup.h"
#include "Utilities/TextFormatComboBox.h"

PageSettingsOutput::PageSettingsOutput(wxWindow *parent)
    : wxStaticBoxSizer(wxHORIZONTAL, parent, _("Output Settings"))
    , m_textFormatComboBox(nullptr)
    , m_showDate(nullptr)
    , m_showTime(nullptr)
    , m_showMs(nullptr)
    , m_showRx(nullptr)
    , m_showTx(nullptr)
    , m_showFlag(nullptr)
    , m_wrap(nullptr)
    , m_terminalMode(nullptr)
    , m_parent(parent)
{
    auto formatText = new wxStaticText(GetStaticBox(), wxID_ANY, _("Format"));
    m_textFormatComboBox = new TextFormatComboBox(GetStaticBox());
    auto optionsSizer = new wxGridBagSizer(4, 4);
    auto AddCheckBox = [=, this](const wxString &label, int row, int col) -> wxCheckBox * {
        auto cb = new wxCheckBox(GetStaticBox(), wxID_ANY, label);
        optionsSizer->Add(cb, wxGBPosition(row, col), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
        return cb;
    };

    m_showRx = AddCheckBox(_("Rx"), 0, 0);
    m_showTx = AddCheckBox(_("Tx"), 0, 1);
    m_showFlag = AddCheckBox(_("Flags"), 0, 2);
    m_showDate = AddCheckBox(_("Date"), 1, 0);
    m_showTime = AddCheckBox(_("Time"), 1, 1);
    m_showMs = AddCheckBox(_("MS"), 1, 2);

    m_showRx->SetValue(true);
    m_showTx->SetValue(true);
    m_showDate->SetValue(false);
    m_showTime->SetValue(true);
    m_showMs->SetValue(false);
    m_showFlag->SetValue(true);

    auto showModeSizer = new wxGridBagSizer(4, 4);
    m_wrap = new wxCheckBox(GetStaticBox(), wxID_ANY, _("Auto Wrap"));
    m_terminalMode = new wxCheckBox(GetStaticBox(), wxID_ANY, _("Terminal Mode"));
    showModeSizer->Add(m_wrap, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    showModeSizer->Add(m_terminalMode, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);

    auto settingsButton = new wxButton(GetStaticBox(), wxID_ANY, _("Settings"));
    auto clearButton = new wxButton(GetStaticBox(), wxID_ANY, _("Clear"));
    auto buttonsSizer = new wxBoxSizer(wxHORIZONTAL);
#if defined(__WXOSX__)
    buttonsSizer->Add(settingsButton, 1, wxEXPAND | wxALL, 4);
    buttonsSizer->Add(clearButton, 1, wxEXPAND | wxALL, 4);
#else
    buttonsSizer->Add(settingsButton, 1, wxEXPAND | wxALL, 0);
    buttonsSizer->Add(clearButton, 1, wxEXPAND | wxALL, 0);
#endif

    m_popup = new PageSettingsOutputPopup(settingsButton);

    auto *sizer = new wxGridBagSizer(4, 4);
    sizer->Add(formatText, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    sizer->Add(m_textFormatComboBox, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    sizer->Add(optionsSizer, wxGBPosition(1, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sizer->Add(showModeSizer, wxGBPosition(2, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    sizer->Add(buttonsSizer, wxGBPosition(3, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    Add(sizer, 1, wxEXPAND | wxALL, 0);
    sizer->AddGrowableCol(1);

    m_wrap->Bind(wxEVT_CHECKBOX, &PageSettingsOutput::OnWrapModeStateChanged, this);
    m_terminalMode->Bind(wxEVT_CHECKBOX, &PageSettingsOutput::OnTerminalModeStateChanged, this);
    clearButton->Bind(wxEVT_BUTTON, &PageSettingsOutput::OnClear, this);
}

void PageSettingsOutput::DoLoad(const wxtJson &parameters)
{
    PageSettingsOutputParameterKeys keys;
    int defaultFormat = static_cast<int>(TextFormat::Hex);
    int format = wxtGetJsonObjValue<int>(parameters, keys.textFormat, defaultFormat);
    bool showDate = wxtGetJsonObjValue<bool>(parameters, keys.showDate, false);
    bool showTime = wxtGetJsonObjValue<bool>(parameters, keys.showTime, true);
    bool showMs = wxtGetJsonObjValue<bool>(parameters, keys.showMs, false);
    bool showRx = wxtGetJsonObjValue<bool>(parameters, keys.showRx, true);
    bool showTx = wxtGetJsonObjValue<bool>(parameters, keys.showTx, true);
    bool showFlag = wxtGetJsonObjValue<bool>(parameters, keys.showFlag, true);
    bool wrap = wxtGetJsonObjValue<bool>(parameters, keys.wrap, false);
    bool terminalMode = wxtGetJsonObjValue<bool>(parameters, keys.terminalMode, false);

    SetComboBoxSectionByIntClientData(m_textFormatComboBox, format);
    m_showDate->SetValue(showDate);
    m_showTime->SetValue(showTime);
    m_showMs->SetValue(showMs);
    m_showRx->SetValue(showRx);
    m_showTx->SetValue(showTx);
    m_showFlag->SetValue(showFlag);
    m_wrap->SetValue(wrap);
    m_terminalMode->SetValue(terminalMode);

    DoUpdateCheckBoxesState();
}

wxtJson PageSettingsOutput::DoSave() const
{
    wxtJson parameters;
    PageSettingsOutputParameterKeys keys;

    int selection = m_textFormatComboBox->GetSelection();
    void *clientData = m_textFormatComboBox->GetClientData(selection);
    if (clientData) {
        parameters[keys.textFormat] = *static_cast<int *>(clientData);
    }

    parameters[keys.showDate] = m_showDate->GetValue();
    parameters[keys.showTime] = m_showTime->GetValue();
    parameters[keys.showMs] = m_showMs->GetValue();
    parameters[keys.showRx] = m_showRx->GetValue();
    parameters[keys.showTx] = m_showTx->GetValue();
    parameters[keys.showFlag] = m_showFlag->GetValue();
    parameters[keys.wrap] = m_wrap->GetValue();
    parameters[keys.terminalMode] = m_terminalMode->GetValue();

    return parameters;
}

TextFormat PageSettingsOutput::GetTextFormat() const
{
    return m_textFormatComboBox->GetSelectedFormat();
}

bool PageSettingsOutput::GetShowDate() const
{
    return m_showDate->GetValue();
}

bool PageSettingsOutput::GetShowTime() const
{
    return m_showTime->GetValue();
}

bool PageSettingsOutput::GetShowMs() const
{
    return m_showMs->GetValue();
}

bool PageSettingsOutput::GetShowRx() const
{
    return m_showRx->GetValue();
}

bool PageSettingsOutput::GetShowTx() const
{
    return m_showTx->GetValue();
}

bool PageSettingsOutput::GetShowFlag() const
{
    return m_showFlag->GetValue();
}

bool PageSettingsOutput::GetWrap() const
{
    return m_wrap->GetValue();
}

bool PageSettingsOutput::GetTerminalMode() const
{
    return m_terminalMode->GetValue();
}

PageSettingsOutputPopup *PageSettingsOutput::GetPopup()
{
    return m_popup;
}

void PageSettingsOutput::DoUpdateCheckBoxesState()
{
    bool checked = m_terminalMode->GetValue();
    m_showRx->Enable(!checked);
    m_showTx->Enable(!checked);
    m_showFlag->Enable(!checked);
    m_showDate->Enable(!checked);
    m_showTime->Enable(!checked);
    m_showMs->Enable(!checked);
}

void PageSettingsOutput::OnTerminalModeStateChanged(wxCommandEvent &)
{
    DoUpdateCheckBoxesState();
}

void PageSettingsOutput::OnClear(wxCommandEvent &)
{
    wxPostEvent(m_parent, wxCommandEvent(wxtEVT_SETTINGS_OUTPUT_CLEAR));
}

void PageSettingsOutput::OnWrapModeStateChanged(wxCommandEvent &)
{
    wxCommandEvent event(wxtEVT_SETTINGS_OUTPUT_WRAP);
    event.SetInt(m_wrap->GetValue() ? 1 : 0);
    wxPostEvent(m_parent, wxCommandEvent(wxtEVT_SETTINGS_OUTPUT_WRAP));
}
