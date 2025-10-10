/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageIOInput.h"

#include "Utilities/LineEdit.h"

PageIOInput::PageIOInput(wxWindow *parent)
    : wxStaticBoxSizer(wxVERTICAL, parent, _("Input"))
{
    m_lineEdit = new LineEdit(GetStaticBox());
    Add(m_lineEdit, 1, wxEXPAND | wxALL, 0);
}

void PageIOInput::Load(const wxtJson &parameters)
{
    if (parameters.is_null() || !parameters.is_object()) {
        return;
    }

    Parameters keys;
    m_loadedText = wxString("");
    if (parameters.contains(keys.inputText)) {
        wxString txt = parameters[keys.inputText].get<std::wstring>();
        m_loadedText = txt;
#if 0
        m_lineEdit->SetValue(txt);
#endif
    }
}

wxtJson PageIOInput::DoSave() const
{
    wxtJson parameters{wxtJson::object()};
    Parameters keys;
    wxString txt = m_lineEdit->GetValue();
    if (!txt.IsEmpty()) {
        parameters[keys.inputText] = txt.ToStdWstring();
    }

    return parameters;
}

void PageIOInput::SetInputText(const wxString &text)
{
    m_lineEdit->SetValue(text);
}

wxString PageIOInput::GetInputText() const
{
    wxString txt = m_lineEdit->GetValue();
    if (txt.IsEmpty()) {
        txt = m_lineEdit->GetHint();
    }

    return txt;
}

void PageIOInput::SetTextFormat(TextFormat format)
{
    m_lineEdit->SetTextFormat(format);
}

wxString PageIOInput::GetLoadedText() const
{
    return m_loadedText;
}
