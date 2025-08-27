/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "UDPClient.h"
#include "UDPClient_p.h"

#include <fmt/format.h>

UDPClient::UDPClient()
    : SocketClient(new UDPClientPrivate)
{}

UDPClient::~UDPClient()
{
    delete GetD<UDPClientPrivate *>();
}
