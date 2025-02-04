﻿/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <nlohmann/json.hpp>
#include <sigslot/signal.hpp>

#include <wx/wx.h>

#include "Common/wxTools.h"

class LinkPrivate;
class Link : wxObject
{
public:
    Link(LinkPrivate *d);
    ~Link();

    // clang-format off
    wxtSignal<std::shared_ptr<char> /*bytes*/, int /*length of bytes*/, std::string /*to  */> bytesTxSignal;
    wxtSignal<std::shared_ptr<char> /*bytes*/, int /*length of bytes*/, std::string /*from*/> bytesRxSignal;
    sigslot::signal<std::string /*error message*/> errorOccurredSignal;
    // clang-format on

    bool Open();
    void Close();
    void Write(std::shared_ptr<char> bytes, int len);

    virtual void Load(const wxtJson &parameters) = 0;
    virtual wxtJson Save() = 0;

public:
    template<typename T>
    T *GetD() const
    {
        return reinterpret_cast<T *>(this->d);
    }

private:
    // The function is called in a separate thread...
    virtual void Loop();

protected:
    LinkPrivate *d;
};
