/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "DataBitsComboBox.h"

DataBitsComboBox::DataBitsComboBox(wxWindow* parent)
    : wxComboBox(parent,
                 wxID_ANY,
                 wxEmptyString,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 nullptr,
                 wxCB_READONLY)
{
    Append("8", new int(static_cast<int>(itas109::DataBits8)));
    Append("7", new int(static_cast<int>(itas109::DataBits7)));
    Append("6", new int(static_cast<int>(itas109::DataBits6)));
    Append("5", new int(static_cast<int>(itas109::DataBits5)));

    SetSelection(0);
}

DataBitsComboBox::~DataBitsComboBox()
{
    for (size_t i = 0; i < GetCount(); i++) {
        delete static_cast<int*>(GetClientData(i));
    }
}

itas109::DataBits DataBitsComboBox::GetDataBits() const
{
    return static_cast<itas109::DataBits>(wxAtoi(GetStringSelection()));
}

void DataBitsComboBox::SetDataBits(itas109::DataBits dataBits)
{
    SetStringSelection(wxString::Format("%d", dataBits));
}
