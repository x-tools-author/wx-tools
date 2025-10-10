/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SerialPortUi.h"

#include <wx/gbsizer.h>
#include <wx/string.h>

#include "Utilities/BaudRateComboBox.h"
#include "Utilities/DataBitsComboBox.h"
#include "Utilities/FlowBitsComboBox.h"
#include "Utilities/ParityComboBox.h"
#include "Utilities/PortNameComboBox.h"
#include "Utilities/StopBitsComboBox.h"

#include "Links/SerialPort.h"

SerialPortUi::SerialPortUi(wxWindow *parent)
    : LinkUi(parent)
    , m_portNameComboBox(nullptr)
    , m_baudRateComboBox(nullptr)
    , m_dataBitsComboBox(nullptr)
    , m_stopBitsComboBox(nullptr)
    , m_parityComboBox(nullptr)
    , m_flowBitsComboBox(nullptr)
{
    m_portNameComboBox = new PortNameComboBox(parent);
    m_baudRateComboBox = new BaudRateComboBox(parent);
    m_dataBitsComboBox = new DataBitsComboBox(parent);
    m_stopBitsComboBox = new StopBitsComboBox(parent);
    m_flowBitsComboBox = new FlowBitsComboBox(parent);
    m_parityComboBox = new ParityComboBox(parent);

    SetupComboBox(m_portNameComboBox, _("Port name"), 0, parent);
    SetupComboBox(m_baudRateComboBox, _("Baud rate"), 1, parent);
    SetupComboBox(m_dataBitsComboBox, _("Data bits"), 2, parent);
    SetupComboBox(m_stopBitsComboBox, _("Stop bits"), 3, parent);
    SetupComboBox(m_parityComboBox, _("Parity"), 4, parent);
    SetupComboBox(m_flowBitsComboBox, _("Flow bits"), 5, parent);

    AddGrowableCol(1);
}

SerialPortUi::~SerialPortUi()
{
    delete dynamic_cast<SerialPort *>(GetLink());
}

void SerialPortUi::Disable()
{
    m_portNameComboBox->Disable();
    m_baudRateComboBox->Disable();
    m_dataBitsComboBox->Disable();
    m_stopBitsComboBox->Disable();
    m_flowBitsComboBox->Disable();
    m_parityComboBox->Disable();
}

void SerialPortUi::Enable()
{
    m_portNameComboBox->Enable();
    m_baudRateComboBox->Enable();
    m_dataBitsComboBox->Enable();
    m_stopBitsComboBox->Enable();
    m_flowBitsComboBox->Enable();
    m_parityComboBox->Enable();
}

wxtJson SerialPortUi::DoSave() const
{
    nlohmann::json json = nlohmann::json(nlohmann::json::object());
    SerialPortParameterKeys keys;
    json[keys.portName] = m_portNameComboBox->GetPortName().ToStdString();
    json[keys.baudRate] = static_cast<int>(m_baudRateComboBox->GetBaudRate());
    json[keys.dataBits] = static_cast<int>(m_dataBitsComboBox->GetDataBits());
    json[keys.stopBits] = static_cast<int>(m_stopBitsComboBox->GetStopBits());
    json[keys.flowControl] = static_cast<int>(m_flowBitsComboBox->GetFlowBits());
    json[keys.parity] = static_cast<int>(m_parityComboBox->GetParity());
    return json;
}

void SerialPortUi::DoLoad(const wxtJson &json)
{
    SerialPortParameterKeys keys;
    // clang-format off
    wxString portName = wxtGetJsonObjValue<std::string>(json, keys.portName, std::string(""));
    int baudRate = wxtGetJsonObjValue<int>(json, keys.baudRate, 9600);
    int dataBits = wxtGetJsonObjValue<int>(json, keys.dataBits, static_cast<int>(itas109::DataBits8));
    int stopBits = wxtGetJsonObjValue<int>(json, keys.stopBits, static_cast<int>(itas109::StopOne));
    int flowBits = wxtGetJsonObjValue<int>(json, keys.flowControl, static_cast<int>(itas109::FlowNone));
    int parity = wxtGetJsonObjValue<int>(json, keys.parity, static_cast<int>(itas109::ParityNone));
    // clang-format on

    m_portNameComboBox->SetPortName(portName);
    m_baudRateComboBox->SetBaudRate(baudRate);
    m_dataBitsComboBox->SetDataBits(static_cast<itas109::DataBits>(dataBits));
    m_stopBitsComboBox->SetStopBits(static_cast<itas109::StopBits>(stopBits));
    m_flowBitsComboBox->SetFlowBits(static_cast<itas109::FlowControl>(flowBits));
    m_parityComboBox->SetParity(static_cast<itas109::Parity>(parity));
}

void SerialPortUi::DoRefreshDevice()
{
    if (GetLink()) {
        return;
    }

    m_portNameComboBox->DoRefresh();
}

Link *SerialPortUi::NewLink()
{
    return new SerialPort();
}

void SerialPortUi::SetupComboBox(wxComboBox *cb, const wxString &label, int row, wxWindow *parent)
{
    auto text = new wxStaticText(parent, wxID_ANY, label);
    Add(text, wxGBPosition(row, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    Add(cb, wxGBPosition(row, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
}
