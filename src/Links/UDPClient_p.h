/***************************************************************************************************
 * Copyright 2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <fmt/format.h>

#include "SocketClient_p.h"

class UDPClientPrivate : public SocketClientPrivate
{
public:
    std::string GetProtocolName() const override { return std::string("udp"); }
    mg_connection *DoConnection(struct mg_mgr *mgr, const char *url, mg_event_handler_t fn) override
    {
        return mg_connect(mgr, url, fn, nullptr);
    }
    bool GetIsClient() const override { return true; }

public:
    void OnMgEvPoll(struct mg_connection *c)
    {
        txBytesLock.lock();
        for (auto &ctx : txBytes) {
            if (mg_send(c, ctx.first.get(), ctx.second)) {
                std::string ip = DoMgAddressToIpV4(&c->rem);
                uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
                std::string to = DoEncodeFlag(ip, port);
                DoQueueTxBytes(ctx.first, ctx.second, to);
            } else {
                DoQueueError(_("Client write bytes failed."));
                break;
            }
        }

        txBytes.clear();
        txBytesLock.unlock();
    }

    void DoPoll(struct mg_connection *c, int ev, void *ev_data) override
    {
        SocketClientPrivate::DoPoll(c, ev, ev_data);

        if (ev == MG_EV_POLL) {
            OnMgEvPoll(c);
        }
    }
};
