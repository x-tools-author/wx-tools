/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include <wx/timer.h>

#include "Link.h"

class LinkPrivate
{
public:
    LinkPrivate()
        : q(nullptr)
        , evtHandler(nullptr)
    {}

    Link *q;

    std::atomic_bool ignoreCloseError{false};
    std::atomic<bool> enableExitThread{false}; // The flag must be set to true on ui thread.
    wxEvtHandler *evtHandler; // You must set this to receive events if you want to use them.

    std::mutex txBytesLock;
    std::vector<std::pair<std::shared_ptr<char> /*data*/, int /*len*/>> txBytes;
    void DoClrearTxBytes()
    {
        txBytesLock.lock();
        txBytes.clear();
        txBytesLock.unlock();
    }

    void DoQueueRxBytes(std::shared_ptr<char> bytes, int len, const wxString &from)
    {
        DoQueueBytes(bytes, len, from, wxtID_LINK_RX);
    }

    void DoQueueTxBytes(std::shared_ptr<char> bytes, int len, const wxString &to)
    {
        DoQueueBytes(bytes, len, to, wxtID_LINK_TX);
    }

    void DoQueueError(const wxString &error)
    {
        if (evtHandler) {
            auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_LINK_ERROR);
            if (ignoreCloseError) {
                evt->SetInt(wxtIgnoreCloseErrorPopup);
            } else {
                evt->SetInt(0);
            }
            evt->SetString(error);
            evtHandler->QueueEvent(evt);
        }
    }

    void DoQueueLinkOpened() { DoQueueLinkState(wxtID_LINK_OPENED); }

    void DoQueueLinkClosed() { DoQueueLinkState(wxtID_LINK_CLOSED); }

public:
    virtual void Poll() = 0;

    template<typename T>
    T GetQ()
    {
        return reinterpret_cast<T>(this->q);
    }

private:
    void DoQueueBytes(std::shared_ptr<char> bytes, int len, const wxString &flag, int id)
    {
        if (bytes && evtHandler && len > 0) {
            auto *evt = new wxThreadEvent(wxEVT_THREAD, id);
            evt->SetPayload<wxtDataItem>(wxtDataItem{bytes, len, flag.ToStdString()});
            evtHandler->QueueEvent(evt);
        }
    }

    void DoQueueLinkState(int id)
    {
        if (evtHandler) {
            auto *evt = new wxThreadEvent(wxEVT_THREAD, id);
            evtHandler->QueueEvent(evt);
        }
    }
};
