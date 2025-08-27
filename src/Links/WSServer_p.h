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
#include "WSServer.h"

class WSServerPrivate : public SocketServerPrivate
{
public:
    std::string GetProtocolName() const override { return std::string("ws"); }
    mg_connection *DoConnection(struct mg_mgr *mgr, const char *url, mg_event_handler_t fn) override
    {
        return mg_http_listen(mgr, url, fn, nullptr);
    }
    bool GetIsClient() const override { return false; }

public:
    void OnMgEvHttpMsg(struct mg_connection *c, void *ev_data)
    {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        mg_ws_upgrade(c, hm, NULL);
    }

    void OnMgEvMsg(struct mg_connection *c, void *ev_data)
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

        auto *d = q->GetD<WSServerPrivate *>();
        std::string ip = d->DoMgAddressToIpV4(&c->rem);
        uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
        std::string from = DoEncodeFlag(ip, port) + op;
        std::shared_ptr<char> bytes(new char[wm->data.len], [](char *p) { delete[] p; });
        memcpy(bytes.get(), wm->data.buf, wm->data.len);

        d->DoQueueRxBytes(bytes, wm->data.len, from);
    }

    void DoSendBytesToClient(struct mg_connection *c)
    {
        if (!c->is_client) {
            return;
        }

        auto *d = q->GetD<WSServerPrivate *>();
        std::string op;
        size_t len = 0;
        std::string ip = d->DoMgAddressToIpV4(&c->rem);
        uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
        std::string to = DoEncodeFlag(ip, port);

        for (auto &ctx : d->txBytes) {
            if (d->dataChannel == WEBSOCKET_OP_TEXT) {
                op = std::string("(T)");
                len = mg_ws_send(c, ctx.first.get(), ctx.second, WEBSOCKET_OP_TEXT);
            } else if (d->dataChannel == WEBSOCKET_OP_BINARY) {
                op = std::string("(B)");
                len = mg_ws_send(c, ctx.first.get(), ctx.second, WEBSOCKET_OP_BINARY);
            } else {
                wxtWarning() << "Unknown data channel: " << d->dataChannel;
                break;
            }

            if (len > 0) {
                d->DoQueueTxBytes(ctx.first, ctx.second, to + op);
            } else {
                d->DoTryToDeleteClient(ip, port);
                break;
            }
        }
    }

    void OnMgEvPoll(struct mg_connection *c)
    {
        auto *d = q->GetD<WSServerPrivate *>();
        if (d->selection.first.empty() && d->selection.second == 0) {
            for (struct mg_connection *conns = c; conns != nullptr; conns = conns->next) {
                if (conns->is_client) {
                    DoSendBytesToClient(conns);
                }
            }
        } else {
            for (struct mg_connection *conns = c; conns != nullptr; conns = conns->next) {
                const std::string ip = d->DoMgAddressToIpV4(&conns->rem);
                const uint8_t port = DoReverseByteOrder<uint16_t>(conns->rem.port);
                if (ip == d->selection.first && port == d->selection.second) {
                    DoSendBytesToClient(conns);
                    break;
                }
            }
        }

        d->DoClrearTxBytes();
    }

    void DoPoll(struct mg_connection *c, int ev, void *ev_data) override
    {
        SocketServerPrivate::DoPoll(c, ev, ev_data);
        if (ev == MG_EV_HTTP_MSG) {
            OnMgEvHttpMsg(c, ev_data);
        } else if (ev == MG_EV_WS_MSG) {
            OnMgEvMsg(c, ev_data);
        } else if (ev == MG_EV_POLL) {
            OnMgEvPoll(c);
        }
    }
};
