/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "TCPClientUi.h"

#include "Links/TCPClient.h"

TCPClientUi::TCPClientUi(wxWindow *parent)
    : SocketClientUi(parent)
{
    std::vector<void (SocketBaseUi::*)(int, wxWindow *)> funcs;
    funcs.push_back(&TCPClientUi::InitServerComboBox);
    funcs.push_back(&TCPClientUi::InitServerPortCtrl);
    funcs.push_back(&TCPClientUi::InitClientInfoTextCtrl);

    InitUiComponents(funcs, parent);
}

TCPClientUi::~TCPClientUi()
{
    delete dynamic_cast<TCPClient *>(GetLink());
}

Link *TCPClientUi::NewLink()
{
    return new TCPClient();
}
