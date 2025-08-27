/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "LineEdit.h"

#include <wx/regex.h>

#include "Common/wxTools.h"

LineEdit::LineEdit(wxWindow *parent)
    : wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize)
    , m_format(static_cast<int>(TextFormat::Utf8))
{
    Bind(wxEVT_TEXT, &LineEdit::OnText, this);
}

void LineEdit::SetTextFormat(TextFormat format)
{
    static std::map<TextFormat, std::string> hintMap
        = {{TextFormat::Bin, "00101000 01101110 01110101 01101100 01101100 00101001"},
           {TextFormat::Oct, "050 156 165 154 154 051"},
           {TextFormat::Dec, "40 110 117 108 108 41"},
           {TextFormat::Hex, "28 6E 75 6C 6C 29"}};

    if (hintMap.find(format) != hintMap.end()) {
        SetHint(hintMap[format]);
    } else {
        SetHint("(null)");
    }

    Clear();
    m_tmp = wxString("");
    m_format = static_cast<int>(format);
}

void LineEdit::OnText(wxCommandEvent &)
{
    if (GetValue().IsEmpty()) {
        m_tmp = wxString("");
        return;
    }

    if (m_tmp == GetValue()) {
        return;
    }

    if (Matches()) {
        m_tmp = GetValue();
    } else {
        SetValue(m_tmp);
        SetInsertionPointEnd();
    }
}

bool LineEdit::Matches()
{
    // clang-format off
    if (m_format == static_cast<int>(TextFormat::Bin)) {
        static wxRegEx regEx("^([01]{1,8}( [01]{1,8})* ?)$");
        return regEx.Matches(GetValue());
    } else if (m_format == static_cast<int>(TextFormat::Oct)) {
        static wxRegEx regEx("^(?:[0-3]?[0-7]{1,2}|[0-7]{1,2})( (?:[0-3]?[0-7]{1,2}|[0-7]{1,2}))* ?$");
        return regEx.Matches(GetValue());
    } else if (m_format == static_cast<int>(TextFormat::Dec)) {
        static wxRegEx regEx("^(?:[0-9]{1,2}|1[0-9]{2}|2[0-4][0-9]|25[0-5])( (?:[0-9]{1,2}|1[0-9]{2}|2[0-4][0-9]|25[0-5]))* ?$");
        return regEx.Matches(GetValue());
    } else if (m_format == static_cast<int>(TextFormat::Hex)) {
        static wxRegEx regEx("^([0-9a-fA-F]{1,2}( [0-9a-fA-F]{1,2})* ?)$");
        return regEx.Matches(GetValue());
    } else {
        return true;
    }
    // clang-format on
}
