/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <CSerialPort/SerialPort.h>
#include <wx/wx.h>

class StopBitsComboBox : public wxComboBox
{
public:
    StopBitsComboBox(wxWindow* parent = nullptr);
    ~StopBitsComboBox();

    itas109::StopBits GetStopBits() const;
    void SetStopBits(itas109::StopBits stopBits);
};
