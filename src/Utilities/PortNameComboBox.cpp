/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PortNameComboBox.h"

#include <algorithm>

#include <CSerialPort/SerialPortInfo.h>

PortNameComboBox::PortNameComboBox(wxWindow* parent)
    : wxComboBox(parent,
                 wxID_ANY,
                 wxEmptyString,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 nullptr,
                 wxCB_READONLY)
{
    DoRefresh();
}

wxString PortNameComboBox::GetPortName() const
{
    return GetValue();
}

void PortNameComboBox::SetPortName(const wxString& portName)
{
    int selection = FindString(portName);
    if (selection != wxNOT_FOUND) {
        SetSelection(selection);
    }
}

void PortNameComboBox::DoRefresh()
{
    Clear();

    std::vector<itas109::SerialPortInfo> infos = itas109::CSerialPortInfo::availablePortInfos();
    std::sort(infos.begin(),
              infos.end(),
              [](const itas109::SerialPortInfo& a, const itas109::SerialPortInfo& b) {
                  std::string aStr = a.portName;
                  std::string bStr = b.portName;
                  if (aStr.length() < bStr.length()) {
                      return true;
                  } else {
                      return false;
                  }
              });

    for (auto& info : infos) {
        Append(info.portName, new wxString(info.portName));
    }

    if (!infos.empty()) {
        SetSelection(0);
    }
}
