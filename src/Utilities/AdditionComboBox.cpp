/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "AdditionComboBox.h"

#include "Common/wxTools.h"

AdditionComboBox::AdditionComboBox(wxWindow *parent)
    : wxComboBox(parent,
                 wxID_ANY,
                 wxEmptyString,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 nullptr,
                 wxCB_READONLY)
{
    auto types = GetSuportedAdditionTypes();
    for (auto &type : types) {
        Append(GetAdditionName(type), new int(static_cast<int>(type)));
    }

    SetSelection(0);
}