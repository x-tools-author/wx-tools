/***************************************************************************************************
 * Copyright 2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "Link.h"

struct SocketBaseParameterKeys
{
    const std::string serverAddress{"serverAddress"};
    const std::string serverPort{"serverPort"};
    const std::string dataChannel{"dataChannel"};
};

class SocketBasePrivate;
class SocketBase : public Link
{
public:
    SocketBase(SocketBasePrivate *d);
    ~SocketBase() override;

    void DoLoad(const wxtJson &parameters) override;
    wxtJson DoSave() override;
};
