﻿/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "SocketBase.h"

class SocketServerPrivate;
class SocketServer : public SocketBase
{
public:
    SocketServer(SocketServerPrivate *d);
    ~SocketServer();

    wxtSignal<std::string, uint16_t> newClientSignal;
    wxtSignal<std::string, uint16_t> deleteClientSignal;

    void SetCurrentClient(const std::string &ip, uint16_t port);
    virtual void DoClearClients();
};
