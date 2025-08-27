/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/textctrl.h>
#include <wx/validate.h>
#include <wx/wx.h>

#include "Common/wxTools.h"

class LineEdit : public wxTextCtrl
{
public:
    LineEdit(wxWindow *parent);

    void SetTextFormat(TextFormat format);

private:
    int m_format;
    wxString m_tmp;

private:
    void OnText(wxCommandEvent &);

    bool Matches();
};
