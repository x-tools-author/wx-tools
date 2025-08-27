/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "BaudRateComboBox.h"

BaudRateComboBox::BaudRateComboBox(wxWindow* parent)
    : wxComboBox(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, nullptr)
{
    Append("110");
    Append("300");
    Append("600");
    Append("1200");
    Append("2400");
    Append("4800");
    Append("9600");
    Append("14400");
    Append("19200");
    Append("38400");
    Append("56000");
    Append("57600");
    Append("115200");
    Append("921600");

    SetEditable(true);
    SetSelection(6);
}

int BaudRateComboBox::GetBaudRate() const
{
    return wxAtoi(GetStringSelection());
}

void BaudRateComboBox::SetBaudRate(int baudRate)
{
    SetStringSelection(wxString::Format("%d", baudRate));
}
