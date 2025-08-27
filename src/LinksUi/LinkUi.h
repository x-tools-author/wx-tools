/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/gbsizer.h>
#include <wx/wx.h>

#include "Links/Link.h"

class LinkUi : public wxGridBagSizer
{
public:
    LinkUi(wxWindow *parent = nullptr);
    ~LinkUi();
    Link *GetLink() const;

    bool Open(wxEvtHandler *evtHandler);
    void Close(bool ignoredCloseError);
    bool IsOpen() const;

    virtual void Disable();
    virtual void Enable();
    virtual wxtJson DoSave() const;
    virtual void DoLoad(const wxtJson &json);
    virtual void DoRefreshDevice();

protected:
    virtual Link *NewLink();

private:
    Link *m_link;
};
