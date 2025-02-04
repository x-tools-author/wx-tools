﻿/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TCPServerUi.h"

#include "Links/TCPServer.h"

TCPServerUi::TCPServerUi(wxWindow *parent)
    : SocketServerUi(parent)
{
    std::vector<void (SocketBaseUi::*)(int, wxWindow *)> funcs;
    funcs.push_back(&TCPServerUi::InitServerComboBox);
    funcs.push_back(&TCPServerUi::InitServerPortCtrl);
    funcs.push_back(&TCPServerUi::InitClientsComboBox);

    InitUiComponents(funcs, parent);
}

TCPServerUi::~TCPServerUi() {}

Link *TCPServerUi::NewLink()
{
    auto *tcpServer = new TCPServer();
    tcpServer->newClientSignal.connect(&TCPServerUi::DoNewClient, this);
    tcpServer->deleteClientSignal.connect(&TCPServerUi::DoDeleteClient, this);

    return tcpServer;
}

void TCPServerUi::DeleteLink(Link *link)
{
    auto *tcpServer = dynamic_cast<TCPServer *>(link);
    if (tcpServer) {
        tcpServer->newClientSignal.disconnect(&TCPServerUi::DoNewClient, this);
        tcpServer->deleteClientSignal.disconnect(&TCPServerUi::DoDeleteClient, this);
        delete tcpServer;
    }
}
