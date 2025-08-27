/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "ESCBoxSizer.h"

#include "Common/wxTools.h"

ESCBoxSizer::ESCBoxSizer(wxWindow *parent)
    : wxBoxSizer(wxHORIZONTAL)
{
    auto types = GetSuportedEscapeTypes();
    for (auto type : types) {
        auto *checkBox = new wxCheckBox(parent, wxID_ANY, GetEscapeName(type));
        Add(checkBox, 1, wxEXPAND | wxALL);
    }
}