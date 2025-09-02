/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/thread.h>
#include <wx/wx.h>

#include "Common/wxTools.h"

#define wxtIgnoreCloseErrorPopup 1
enum wxtID {
    wxtID_LINK_TX = wxID_HIGHEST + 1,
    wxtID_LINK_RX,
    wxtID_LINK_ERROR,
    wxtID_LINK_NEW,
    wxtID_LINK_DELETE,
    wxtID_LINK_OPENED,
    wxtID_LINK_CLOSED,
    wxtID_LINK_RESOLVED
};

class LinkPrivate;
class Link : public wxThread
{
public:
    Link(LinkPrivate *d);
    virtual ~Link();

    bool Open();
    void Close();
    void Write(std::shared_ptr<char> bytes, int len);
    void SetEvtHandler(wxEvtHandler *handler);

    virtual void DoLoad(const wxtJson &parameters) = 0;
    virtual wxtJson DoSave() = 0;

public:
    template<typename T>
    T GetD() const
    {
        return dynamic_cast<T>(this->d);
    }

protected:
    void *Entry() override;

protected:
    LinkPrivate *d;
};
