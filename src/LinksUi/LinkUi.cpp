/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "LinkUi.h"

#include <wx/event.h>
#include <wx/wx.h>

#include "Links/Link_p.h"

LinkUi::LinkUi(wxWindow *parent)
    : wxGridBagSizer(4, 4)
    , m_link(nullptr)
{}

LinkUi::~LinkUi()
{

}

Link *LinkUi::GetLink() const
{
    return m_link;
}

bool LinkUi::Open(wxEvtHandler *evtHandler)
{
    m_link = NewLink();
    if (!m_link) {
        return false;
    }

    m_link->DoLoad(DoSave());
    m_link->SetEvtHandler(evtHandler);
    return m_link->Open();
}

void LinkUi::Close(bool ignoredCloseError)
{
    if (m_link) {
        // m_link will be deleted when the thread exits...
        m_link->GetD<LinkPrivate *>()->ignoreCloseError.store(ignoredCloseError);
        m_link->Close();
    }

    m_link = nullptr;
}

bool LinkUi::IsOpen() const
{
    return m_link != nullptr;
}

void LinkUi::Disable() {}

void LinkUi::Enable() {}

wxtJson LinkUi::DoSave() const
{
    wxtJson json;
    return json;
}

void LinkUi::DoLoad(const wxtJson &json)
{
    wxUnusedVar(json);
}

void LinkUi::DoRefreshDevice()
{
    // Nothing to do yet...
}

Link *LinkUi::NewLink()
{
    return nullptr;
}
