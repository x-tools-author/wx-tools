/***************************************************************************************************
 * Copyright 2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "IpComboBox.h"

#include <asio.hpp>

IpComboBox::IpComboBox(wxWindow* parent)
    : wxComboBox(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, nullptr)
{
    DoRefresh();
#if 0
    SetMinSize(wxSize(130, -1));
#endif
}

void IpComboBox::DoRefresh()
{
    Clear();
    Append(wxString("127.0.0.1"));

#if ASIO_VERSION > 103200 //1.32.0
    asio::io_context io_context;
    asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(asio::ip::host_name(), "");
    for (const auto& ep : endpoints) {
        auto addr = ep.endpoint().address();
        if (addr.is_v4()) {
            Append(addr.to_string());
        } else if (addr.is_v6()) {
#if defined(WXT_ENABLE_IPV6)
            Append(addr.to_string());
#endif
        }
    }
#else
    asio::io_service ioService;
    asio::ip::tcp::resolver resolver(ioService);
    asio::ip::tcp::resolver::query query(asio::ip::host_name(), "");
    asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    asio::ip::tcp::resolver::iterator end;
    while (iter != end) {
        asio::ip::tcp::endpoint ep = *iter++;
        if (ep.protocol() == asio::ip::tcp::v4()) {
            Append(ep.address().to_string());
        } else {
#if defined(WXT_ENABLE_IPV6)
            Append(ep.address().to_string());
#endif
        }
    }
#endif

    SetSelection(0);
}
