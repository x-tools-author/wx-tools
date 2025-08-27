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
#include "Links/WSClient.h"
#include "SocketClient_p.h"

class WSClientPrivate : public SocketClientPrivate
{
public:
    std::string GetProtocolName() const override { return std::string("ws"); }
    mg_connection *DoConnection(struct mg_mgr *mgr, const char *url, mg_event_handler_t fn) override
    {
        return mg_ws_connect(mgr, url, fn, nullptr, nullptr);
    }
    bool GetIsClient() const override { return true; }

public:
    void OnMgEvPoll(struct mg_connection *c)
    {
        std::string op;
        size_t len = 0;
        std::string ip = DoMgAddressToIpV4(&c->rem);
        uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
        std::string to = DoEncodeFlag(ip, port);

        txBytesLock.lock();
        for (auto &ctx : txBytes) {
            if (dataChannel == WEBSOCKET_OP_TEXT) {
                op = std::string("(T)");
                len = mg_ws_send(c, ctx.first.get(), ctx.second, WEBSOCKET_OP_TEXT);
            } else {
                op = std::string("(B)");
                len = mg_ws_send(c, ctx.first.get(), ctx.second, WEBSOCKET_OP_BINARY);
            }

            if (len > 0) {
                DoQueueTxBytes(ctx.first, ctx.second, to + op);
            } else {
                DoQueueError(_("WS server send bytes error."));
                break;
            }
        }

        txBytes.clear();
        txBytesLock.unlock();
    }

    void OnMgEvWsMsg(struct mg_connection *c, void *ev_data)
    {
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        std::string op;
        if ((wm->flags & WEBSOCKET_OP_TEXT) == WEBSOCKET_OP_TEXT) {
            op = std::string("(T)");
        } else if ((wm->flags & WEBSOCKET_OP_BINARY) == WEBSOCKET_OP_BINARY) {
            op = std::string("(B)");
        } else {
            wxtWarning() << "Unknown data channel: " << wm->flags;
            return;
        }

        auto *d = q->GetD<WSClientPrivate *>();
        std::string ip = q->GetD<WSClientPrivate *>()->DoMgAddressToIpV4(&c->rem);
        uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
        std::string from = DoEncodeFlag(ip, port) + op;
        std::shared_ptr<char> bytes(new char[wm->data.len], [](char *p) { delete[] p; });
        memcpy(bytes.get(), wm->data.buf, wm->data.len);
        d->DoQueueRxBytes(bytes, wm->data.len, from);
    }

    void DoPoll(struct mg_connection *c, int ev, void *ev_data) override
    {
        SocketClientPrivate::DoPoll(c, ev, ev_data);
        if (ev == MG_EV_POLL) {
            OnMgEvPoll(c);
        } else if (ev == MG_EV_WS_MSG) {
            OnMgEvWsMsg(c, ev_data);
        }
    }
};
