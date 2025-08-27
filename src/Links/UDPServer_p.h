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

#include "Common/wxTools.h"
#include "SocketServer_p.h"
#include "UDPServer.h"

class UDPServerPrivate : public SocketServerPrivate
{
public:
    std::string GetProtocolName() const override { return std::string("udp"); }
    mg_connection *DoConnection(struct mg_mgr *mgr, const char *url, mg_event_handler_t fn) override
    {
        return mg_listen(mgr, url, fn, nullptr);
    }
    bool GetIsClient() const override { return false; }

public:
    void OnMgEvRead(struct mg_connection *c) override
    {
        if (c->recv.len <= 0) {
            return;
        }

        SocketServerPrivate::OnMgEvRead(c);
        std::string ip = DoMgAddressToIpV4(&c->rem);
        uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
        DoTryToNewClient(ip, port);
    }

    void OnMgEvPoll(struct mg_connection *c)
    {
        if (selection.first.empty() && selection.second == 0) {
            for (auto &client : clients) {
                if (DoIpV4ToMgAddress(client.first, &c->rem)) {
                    (&c->rem)->port = DoReverseByteOrder(client.second);
                    DoTryToSendBytes(c);
                }
            }
        } else {
            if (DoIpV4ToMgAddress(selection.first, &c->rem)) {
                (&c->rem)->port = DoReverseByteOrder(selection.second);
                DoTryToSendBytes(c);
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
