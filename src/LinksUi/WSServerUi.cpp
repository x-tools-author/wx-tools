﻿/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "WSServerUi.h"

#include <Links/WSServer.h>

WSServerUi::WSServerUi(wxWindow *parent)
    : SocketServerUi(parent)
{
    std::vector<void (SocketBaseUi::*)(int, wxWindow *)> funcs;
    funcs.push_back(&WSServerUi::InitServerComboBox);
    funcs.push_back(&WSServerUi::InitServerPortCtrl);
    funcs.push_back(&WSServerUi::InitClientsComboBox);
#if 0
    funcs.push_back(&WSServerUi::InitClearClientButton);
#endif
    funcs.push_back(&WSServerUi::InitDataChannelComboBox);

    InitUiComponents(funcs, parent);
}

WSServerUi::~WSServerUi() {}

Link *WSServerUi::NewLink()
{
    auto wsServer = new WSServer();
    wsServer->newClientSignal.connect(&WSServerUi::DoNewClient, this);
    wsServer->deleteClientSignal.connect(&WSServerUi::DoDeleteClient, this);
    return wsServer;
}

void WSServerUi::DeleteLink(Link *link)
{
    auto wsServer = dynamic_cast<WSServer *>(link);
    if (wsServer) {
        wsServer->newClientSignal.disconnect(&WSServerUi::DoNewClient, this);
        wsServer->deleteClientSignal.disconnect(&WSServerUi::DoDeleteClient, this);
        delete wsServer;
    }
}
