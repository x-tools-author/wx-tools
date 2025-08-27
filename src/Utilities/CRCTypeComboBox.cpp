/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "CRCTypeComboBox.h"

CRCTypeComboBox::CRCTypeComboBox(wxWindow *parent)
    : wxComboBox(parent,
                 wxID_ANY,
                 wxtNoneStr,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 nullptr,
                 wxCB_READONLY)
{
    for (auto type : GetSuportedCrcTypes()) {
        Append(GetCrcName(type), new int(static_cast<int>(type)));
    }

    SetSelection(0);
}

CRCType CRCTypeComboBox::GetCrcType() const
{
    auto index = GetSelection();
    if (index == wxNOT_FOUND) {
        return CRCType::CRC_8;
    }

    return *reinterpret_cast<CRCType *>(GetClientData(index));
}

void CRCTypeComboBox::SetCrcType(CRCType type)
{
    auto index = FindString(GetCrcName(type));
    if (index != wxNOT_FOUND) {
        SetSelection(index);
    }
}