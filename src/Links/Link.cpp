/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Link.h"
#include "Link_p.h"

Link::Link(LinkPrivate *dPtr)
    : d(dPtr)
{
    d->q = this;
}

Link::~Link()
{

}

bool Link::Open()
{
    if (Create() != wxTHREAD_NO_ERROR) {
        wxtWarning() << "Failed to create thread";
        return false;
    }

    if (Run() != wxTHREAD_NO_ERROR) {
        wxtWarning() << "Failed to run thread";
        return false;
    }

    return true;
}

void Link::Close()
{
    d->enableExitThread.store(true);
    Delete();
}

void Link::Write(std::shared_ptr<char> bytes, int len)
{
    if (len > 0) {
        d->txBytesLock.lock();
        d->txBytes.push_back(std::make_pair(std::move(bytes), len));
        d->txBytesLock.unlock();
    }
}

void Link::SetEvtHandler(wxEvtHandler *handler)
{
    d->evtHandler = handler;
}

void *Link::Entry()
{
    d->Poll();

    while (!d->enableExitThread.load()) {
        // Waiting for the ui thread to set the flag to true(Call Close()).
    }

    return nullptr;
}
