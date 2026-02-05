/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "BLECenter.h"
#include "BLECenter_p.h"

#include <fmt/format.h>

BLECenter::BLECenter()
    : Link(new BLECenterPrivate())
{}

BLECenter::~BLECenter()
{
    delete GetD<BLECenterPrivate *>();
}

void BLECenter::DoLoad(const wxtJson &parameters)
{
    auto d = GetD<BLECenterPrivate *>();
    BLECenterParameterKeys keys;
    // clang-format off
    d->portName = wxtGetJsonObjValue<std::string>(parameters, keys.portName, std::string(""));
    d->baudRate = wxtGetJsonObjValue<int>(parameters, keys.baudRate, 9600);
    d->flowControl = static_cast<itas109::FlowControl>(wxtGetJsonObjValue<int>(parameters, keys.flowControl, static_cast<int>(itas109::FlowNone)));
    d->parity = static_cast<itas109::Parity>(wxtGetJsonObjValue<int>(parameters, keys.parity, static_cast<int>(itas109::ParityNone)));
    d->stopBits = static_cast<itas109::StopBits>(wxtGetJsonObjValue<int>(parameters, keys.stopBits, static_cast<int>(itas109::StopOne)));
    d->dataBits = static_cast<itas109::DataBits>(wxtGetJsonObjValue<int>(parameters, keys.dataBits, static_cast<int>(itas109::DataBits8)));
    // clang-format on
}

wxtJson BLECenter::DoSave()
{
    auto d = GetD<BLECenterPrivate *>();
    wxtJson parameters;
    BLECenterParameterKeys keys;
    parameters[keys.portName] = d->portName;
    parameters[keys.baudRate] = d->baudRate;
    parameters[keys.flowControl] = static_cast<int>(d->flowControl);
    parameters[keys.parity] = static_cast<int>(d->parity);
    parameters[keys.stopBits] = static_cast<int>(d->stopBits);
    parameters[keys.dataBits] = static_cast<int>(d->dataBits);
    return parameters;
}
