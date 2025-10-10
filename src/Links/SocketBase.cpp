/***************************************************************************************************
 * Copyright 2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketBase.h"
#include "SocketBase_p.h"

#include "Common/wxTools.h"

SocketBase::SocketBase(SocketBasePrivate *d)
    : Link(d)
{}

SocketBase::~SocketBase() {}

void SocketBase::DoLoad(const wxtJson &parameters)
{
    auto *d = GetD<SocketBasePrivate *>();
    SocketBaseParameterKeys keys;

    d->serverAddress = wxtGetJsonObjValue<std::string>(parameters, keys.serverAddress, "127.0.0.1");
    d->serverPort = wxtGetJsonObjValue<int>(parameters, keys.serverPort, 51234);
    d->dataChannel = wxtGetJsonObjValue<int>(parameters, keys.dataChannel, WEBSOCKET_OP_TEXT);
}

wxtJson SocketBase::DoSave()
{
    auto *d = GetD<SocketBasePrivate *>();
    SocketBaseParameterKeys keys;
    wxtJson parameters;
    parameters[keys.serverAddress] = d->serverAddress.ToStdString();
    parameters[keys.serverPort] = d->serverPort;
    parameters[keys.dataChannel] = d->dataChannel;
    return parameters;
}
