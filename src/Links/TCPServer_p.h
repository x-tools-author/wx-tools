/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <fmt/format.h>

#include "SocketServer_p.h"
#include "TCPServer.h"

class TCPServerPrivate : public SocketServerPrivate
{
public:
    std::string GetProtocolName() const override { return std::string("tcp"); }
    mg_connection *DoConnection(struct mg_mgr *mgr, const char *url, mg_event_handler_t fn) override
    {
        return mg_listen(mgr, url, fn, nullptr);
    }
    bool GetIsClient() const override { return false; }

public:
    void OnMgEvPoll(struct mg_connection *c)
    {
        if (selection.first.empty() && selection.second == 0) {
            for (auto conns = c; conns != nullptr; conns = conns->next) {
                if (conns->is_client) {
                    DoTryToSendBytes(c);
                }
            }
        } else {
            for (auto conns = c; conns != nullptr; conns = conns->next) {
                std::string ip = DoMgAddressToIpV4(&conns->rem);
                uint16_t port = DoReverseByteOrder<uint16_t>(conns->rem.port);
                if (conns->is_client && selection.first == ip && selection.second == port) {
                    DoTryToSendBytes(c);
                    break;
                }
            }
        }

        DoClrearTxBytes();
    }

    void DoPoll(struct mg_connection *c, int ev, void *ev_data) override
    {
        SocketServerPrivate::DoPoll(c, ev, ev_data);
        if (ev == MG_EV_POLL) {
            OnMgEvPoll(c);
        }
    }
};
