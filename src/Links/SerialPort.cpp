/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SerialPort.h"
#include "SerialPort_p.h"

#include <fmt/format.h>

SerialPort::SerialPort()
    : Link(new SerialPortPrivate())
{}

SerialPort::~SerialPort()
{
    delete GetD<SerialPortPrivate *>();
}

void SerialPort::DoLoad(const wxtJson &parameters)
{
    auto d = GetD<SerialPortPrivate *>();
    SerialPortParameterKeys keys;
    // clang-format off
    d->portName = wxtGetJsonObjValue<std::string>(parameters, keys.portName, std::string(""));
    d->baudRate = wxtGetJsonObjValue<int>(parameters, keys.baudRate, 9600);
    d->flowControl = static_cast<itas109::FlowControl>(wxtGetJsonObjValue<int>(parameters, keys.flowControl, static_cast<int>(itas109::FlowNone)));
    d->parity = static_cast<itas109::Parity>(wxtGetJsonObjValue<int>(parameters, keys.parity, static_cast<int>(itas109::ParityNone)));
    d->stopBits = static_cast<itas109::StopBits>(wxtGetJsonObjValue<int>(parameters, keys.stopBits, static_cast<int>(itas109::StopOne)));
    d->dataBits = static_cast<itas109::DataBits>(wxtGetJsonObjValue<int>(parameters, keys.dataBits, static_cast<int>(itas109::DataBits8)));
    // clang-format on
}

wxtJson SerialPort::DoSave()
{
    auto d = GetD<SerialPortPrivate *>();
    wxtJson parameters;
    SerialPortParameterKeys keys;
    parameters[keys.portName] = d->portName;
    parameters[keys.baudRate] = d->baudRate;
    parameters[keys.flowControl] = static_cast<int>(d->flowControl);
    parameters[keys.parity] = static_cast<int>(d->parity);
    parameters[keys.stopBits] = static_cast<int>(d->stopBits);
    parameters[keys.dataBits] = static_cast<int>(d->dataBits);
    return parameters;
}
