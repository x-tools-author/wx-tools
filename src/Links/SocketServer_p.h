/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "Link.h"
#include "SocketBase_p.h"
#include "SocketServer.h"

class SocketServerPrivate : public SocketBasePrivate
{
public:
    void OnMgEvRead(struct mg_connection *c) override
    {
        if (c->recv.len <= 0) {
            return;
        }

        SocketBasePrivate::OnMgEvRead(c);
    }

    void OnMgEvClose(struct mg_connection *c, void *ev_data)
    {
        if (c->is_client) {
            std::string ip = DoMgAddressToIpV4(&c->rem);
            uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
            DoTryToDeleteClient(ip, port);
        } else {
            DoQueueError(_("Server has been closed."));
            DoQueueLinkClosed();
        }
    }

    void OnMgEvAccept(struct mg_connection *c)
    {
        c->is_client = true;
        const std::string remIp = DoMgAddressToIpV4(&c->rem);
        const uint16_t remPort = DoReverseByteOrder<uint16_t>(c->rem.port);
        wxtInfo() << fmt::format("New client socket opened({0}:{1}).", remIp, remPort);

        DoTryToNewClient(remIp, remPort);
    }

    void DoPoll(struct mg_connection *c, int ev, void *ev_data) override
    {
        SocketBasePrivate::DoPoll(c, ev, ev_data);
        if (ev == MG_EV_CLOSE) {
            OnMgEvClose(c, ev_data);
        } else if (ev == MG_EV_ACCEPT) {
            OnMgEvAccept(c);
        }
    }

public:
    std::vector<std::pair<std::string, uint16_t>> clients;
    std::pair<std::string, uint16_t> selection;

public:
    void DoTryToDeleteClient(const std::string &address, uint16_t port)
    {
        if (clients.empty()) {
            return;
        }

        auto it = std::find_if(clients.begin(),
                               clients.end(),
                               [&](const std::pair<std::string, uint16_t> &client) {
                                   return client.first == address && client.second == port;
                               });

        if (it != clients.end()) {
            clients.erase(it);
        }

        // If the client is the current selection, clear the selection
        if (address == selection.first && port == selection.second) {
            selection = std::make_pair("", 0);
        }

        if (evtHandler) {
            wxThreadEvent *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_LINK_DELETE);
            evt->SetString(address);
            evt->SetInt(port);
            evtHandler->QueueEvent(evt);
        }
    }

    void DoTryToNewClient(const std::string &address, uint16_t port)
    {
        auto client = std::make_pair(address, port);
        if (std::find(clients.begin(), clients.end(), client) == clients.end()) {
            clients.push_back(client);

            if (evtHandler) {
                wxThreadEvent *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_LINK_NEW);
                evt->SetString(address);
                evt->SetInt(port);
                evtHandler->QueueEvent(evt);
            }
        }
    }

    void DoTryToSendBytes(struct mg_connection *c)
    {
        std::string ip = DoMgAddressToIpV4(&c->rem);
        uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
        std::string to = DoEncodeFlag(ip, port);

        txBytesLock.lock();
        for (auto &ctx : txBytes) {
            if (mg_send(c, ctx.first.get(), ctx.second)) {
                DoQueueTxBytes(ctx.first, ctx.second, to);
            } else {
                wxtInfo() << fmt::format("Failed to send bytes to {0}, it will be removed.", to);
                DoTryToDeleteClient(ip, port);
            }
        }
        txBytesLock.unlock();
    }
};
