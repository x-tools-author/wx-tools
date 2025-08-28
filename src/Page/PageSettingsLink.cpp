/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageSettingsLink.h"

#include "LinksUi/LinkUi.h"
#include "LinksUi/SerialPortUi.h"
#include "LinksUi/TCPClientUi.h"
#include "LinksUi/TCPServerUi.h"
#include "LinksUi/UDPClientUi.h"
#include "LinksUi/UDPServerUi.h"
#include "LinksUi/WSClientUi.h"
#include "LinksUi/WSServerUi.h"

PageSettingsLink::PageSettingsLink(LinkType type, wxWindow *parent)
    : wxStaticBoxSizer(wxVERTICAL, parent, _("Link Settings"))
    , m_linkUi(nullptr)
    , m_openButton(nullptr)
    , m_refreshButton(nullptr)
{
    m_linkUi = CreateLinkUi(type, GetStaticBox());
    Add(m_linkUi, 0, wxEXPAND, 0);

    AddSpacer(4);

    m_refreshButton = new wxButton(GetStaticBox(), wxID_ANY, _("Refresh"));
    m_openButton = new wxButton(GetStaticBox(), wxID_ANY, _("Open"));

    auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
#if defined(__WXOSX__)
    buttonSizer->Add(m_refreshButton, 1, wxEXPAND | wxALL, 4);
    buttonSizer->Add(m_openButton, 1, wxEXPAND | wxALL, 4);
#else
    buttonSizer->Add(m_refreshButton, 1, wxEXPAND | wxALL, 0);
    buttonSizer->Add(m_openButton, 1, wxEXPAND | wxALL, 0);
#endif
    Add(buttonSizer, 0, wxEXPAND, 0);
}

void PageSettingsLink::DoLoad(const wxtJson &parameters)
{
    m_linkUi->DoLoad(parameters);
}

wxtJson PageSettingsLink::DoSave() const
{
    return m_linkUi->DoSave();
}

wxButton *PageSettingsLink::GetOpenButton() const
{
    return m_openButton;
}

wxButton *PageSettingsLink::GetRefreshButton() const
{
    return m_refreshButton;
}

LinkUi *PageSettingsLink::GetLinkUi() const
{
    return m_linkUi;
}

LinkUi *PageSettingsLink::CreateLinkUi(LinkType type, wxWindow *parent)
{
    if (type == LinkType::SerialPort) {
        return new SerialPortUi(parent);
    } else if (type == LinkType::UDPClient) {
        return new UDPClientUi(parent);
    } else if (type == LinkType::UDPServer) {
        return new UDPServerUi(parent);
    } else if (type == LinkType::TCPClient) {
        return new TCPClientUi(parent);
    } else if (type == LinkType::TCPServer) {
        return new TCPServerUi(parent);
    } else if (type == LinkType::WSClient) {
        return new WSClientUi(parent);
    } else if (type == LinkType::WSServer) {
        return new WSServerUi(parent);
    }

    return nullptr;
}
