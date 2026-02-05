/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
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
#if WXT_ENABLE_BLE
#include "LinksUi/BLECenterUi.h"
#include "LinksUi/BLEPeripheralUi.h"
#endif

PageSettingsLink::PageSettingsLink(LinkType type, wxWindow *parent)
    : wxStaticBoxSizer(wxVERTICAL, parent, _("Link Settings"))
    , m_linkUi(nullptr)
    , m_openButton(nullptr)
    , m_refreshButton(nullptr)
    , m_extButton(nullptr)
{
    m_linkUi = CreateLinkUi(type, GetStaticBox());
    Add(m_linkUi, 0, wxEXPAND, 0);

    AddSpacer(4);

    m_refreshButton = new wxButton(GetStaticBox(), wxID_ANY, _("Refresh"));
    m_openButton = new wxButton(GetStaticBox(), wxID_ANY, _("Open"));
    m_extButton = new wxToggleButton(GetStaticBox(), wxID_ANY, "");
    DoUpdateExtButtonText();
    m_extButton->SetValue(false);

    auto buttonSizer = new wxGridBagSizer(4, 4);
    buttonSizer->Add(m_refreshButton, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    buttonSizer->Add(m_openButton, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    buttonSizer->Add(m_extButton, wxGBPosition(1, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
    Add(buttonSizer, 0, wxEXPAND, 0);
    buttonSizer->AddGrowableCol(0);
    buttonSizer->AddGrowableCol(1);
}

void PageSettingsLink::DoLoad(const wxtJson &parameters)
{
    bool toggled = wxtGetJsonObjValue(parameters, "extPanelToggled", false);
    m_linkUi->DoLoad(parameters);
    m_extButton->SetValue(toggled);
}

wxtJson PageSettingsLink::DoSave() const
{
    wxtJson json = m_linkUi->DoSave();
    json["extPanelToggled"] = m_extButton->GetValue();
    return json;
}

wxButton *PageSettingsLink::GetOpenButton() const
{
    return m_openButton;
}

wxButton *PageSettingsLink::GetRefreshButton() const
{
    return m_refreshButton;
}

wxToggleButton *PageSettingsLink::GetExtButton() const
{
    return m_extButton;
}

LinkUi *PageSettingsLink::GetLinkUi() const
{
    return m_linkUi;
}

void PageSettingsLink::DoUpdateExtButtonText()
{
    if (m_extButton->GetValue()) {
        m_extButton->SetLabel(_("Hide External Panel"));
    } else {
        m_extButton->SetLabel(_("Show External Panel"));
    }
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
#if WXT_ENABLE_BLE
    else if (type == LinkType::BLECenter) {
        return new BLECenterUi(parent);
    } else if (type == LinkType::BLEPeripheral) {
        return new BLEPeripheralUi(parent);
    }
#endif
    return nullptr;
}
