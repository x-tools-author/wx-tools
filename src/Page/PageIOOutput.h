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

class PageIOOutput : public wxStaticBoxSizer
{
public:
    PageIOOutput(wxWindow *parent);

    void AppendText(const wxString &text, bool newLine);
    void SetWrap(bool wrap);
    void Clear();

private:
    wxTextCtrl *m_textCtrl;
    wxWindow *m_parent;
};
