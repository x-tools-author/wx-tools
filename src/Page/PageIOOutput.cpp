/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageIOOutput.h"

PageIOOutput::PageIOOutput(wxWindow *parent)
    : wxStaticBoxSizer(wxVERTICAL, parent, _("Output"))
    , m_parent(parent)
    , m_textCtrl(nullptr)
{
    SetWrap(false);
}

void PageIOOutput::AppendText(const wxString &text, bool newLine)
{
    if (!m_textCtrl) {
        return;
    }

    if (newLine) {
        m_textCtrl->AppendText(text + "\n");
    } else {
        m_textCtrl->AppendText(text);
    }
}

void PageIOOutput::SetWrap(bool wrap)
{
    wxString text = m_textCtrl ? m_textCtrl->GetValue() : wxString("");
    if (m_textCtrl) {
        Remove(0);
        m_textCtrl->Destroy();
        m_textCtrl = nullptr;
    }

    long wrapMode = wrap ? (wxTE_MULTILINE | wxTE_CHARWRAP) : (wxTE_MULTILINE | wxTE_DONTWRAP);
    m_textCtrl = new wxTextCtrl(m_parent,
                                wxID_ANY,
                                wxEmptyString,
                                wxDefaultPosition,
                                wxDefaultSize,
                                wrapMode);

    Add(m_textCtrl, 1, wxEXPAND | wxALL, 0);
    m_textCtrl->SetEditable(false);
    m_textCtrl->SetValue(text);
    Layout();
}

void PageIOOutput::Clear()
{
    if (m_textCtrl) {
        m_textCtrl->Clear();
    }
}
