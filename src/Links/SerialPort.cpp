﻿/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SerialPort.h"
#include "SerialPort_p.h"

SerialPort::SerialPort()
    : Link(new SerialPortPrivate())
{}

SerialPort::~SerialPort()
{
    delete GetD<SerialPortPrivate>();
}

void SerialPort::Loop()
{
    auto d = GetD<SerialPortPrivate>();
    d->invokedInterrupted.store(false);
    auto *serialPortIml = new itas109::CSerialPort();
    serialPortIml->init(d->portName.c_str(),
                        d->baudRate,
                        d->parity,
                        d->dataBits,
                        d->stopBits,
                        d->flowControl);

    d->isRunning.store(true);
    if (serialPortIml->open()) {
        while (!d->invokedInterrupted.load()) {
            // Read data....
            ReadBytes(serialPortIml, this);

            // Write data...
            WriteBytes(serialPortIml, this);

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    } else {
        std::string errorString(serialPortIml->getLastErrorMsg());
        errorOccurredSignal(std::string("Open port failed.") + errorString);
    }

    serialPortIml->close();
    delete serialPortIml;
    d->isRunning.store(false);
    wxtInfo() << "Serial port loop exit.";
}

void SerialPort::Load(const wxtJson &parameters)
{
    auto d = GetD<SerialPortPrivate>();
    SerialPortParameterKeys keys;
    d->portName = parameters[keys.portName].get<std::string>();
    d->baudRate = parameters[keys.baudRate].get<int>();
    d->flowControl = static_cast<itas109::FlowControl>(parameters[keys.flowControl].get<int>());
    d->parity = static_cast<itas109::Parity>(parameters[keys.parity].get<int>());
    d->stopBits = static_cast<itas109::StopBits>(parameters[keys.stopBits].get<int>());
    d->dataBits = static_cast<itas109::DataBits>(parameters[keys.dataBits].get<int>());
}

wxtJson SerialPort::Save()
{
    auto d = GetD<SerialPortPrivate>();
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
