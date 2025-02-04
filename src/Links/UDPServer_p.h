﻿/***************************************************************************************************
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
};

static void OnMgEvPoll(struct mg_connection *c, void *ev_data, UDPServer *q)
{
    auto *d = q->GetD<UDPServerPrivate>();
    if (d == nullptr) {
        return;
    }

    size_t len = 0;
    std::string ip = d->mg_addr_to_ipv4(&c->rem);
    uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
    std::string to = DoEncodeFlag(ip, port);

    if (port == 0) {
        return;
    }

    for (auto &ctx : d->txBytes) {
        len = mg_send(c, ctx.first.get(), ctx.second);
        if (len > 0) {
            q->bytesTxSignal(ctx.first, ctx.second, to);
        } else {
            wxtInfo() << fmt::format("Send bytes to {} failed!", to);
            q->deleteClientSignal(ip, port);
        }
    }

    d->txBytes.clear();
}

static void OnMgEvOpen(struct mg_connection *c, void *ev_data, UDPServer *q)
{
    auto *d = q->GetD<UDPServerPrivate>();
    const std::string remIp = d->mg_addr_to_ipv4(&c->rem);
    const uint16_t remPort = DoReverseByteOrder<uint16_t>(c->rem.port);
    const std::string locIp = d->mg_addr_to_ipv4(&c->loc);
    const uint16_t locPort = DoReverseByteOrder<uint16_t>(c->loc.port);
    const std::string from = DoEncodeFlag(remIp, remPort);
    const std::string to = DoEncodeFlag(locIp, locPort);

    wxtInfo() << fmt::format("UDP client connected from {0} to {1}", from, to);
}

static void OnMgEvRead(struct mg_connection *c, void *ev_data, UDPServer *q)
{
    if (c->recv.len <= 0) {
        return;
    }

    std::shared_ptr<char> bytes(new char[c->recv.len], [](char *p) { delete[] p; });
    memcpy(bytes.get(), c->recv.buf, c->recv.len);

    auto *d = q->GetD<UDPServerPrivate>();
    const std::string ip = d->mg_addr_to_ipv4(&c->rem);
    const uint16_t port = DoReverseByteOrder<uint16_t>(c->rem.port);
    const std::string from = DoEncodeFlag(ip, port);

    auto client = std::make_pair(ip, port);
    auto clients = d->clients;
    if (std::find(clients.begin(), clients.end(), client) == clients.end()) {
        d->clients.push_back(client);
        q->newClientSignal(ip, port);
    }

    q->bytesRxSignal(std::move(bytes), c->recv.len, from);
    c->recv.len = 0;
}

static void UDPServerHandler(struct mg_connection *c, int ev, void *ev_data)
{
    auto *q = reinterpret_cast<UDPServer *>(c->mgr->userdata);
    if (ev == MG_EV_OPEN) {
        OnMgEvOpen(c, ev_data, q);
    } else if (ev == MG_EV_READ) {
        OnMgEvRead(c, ev_data, q);
    } else if (ev == MG_EV_POLL) {
        OnMgEvPoll(c, ev_data, q);
    }
}
