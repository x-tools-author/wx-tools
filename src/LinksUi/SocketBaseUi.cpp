/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketBaseUi.h"

#include <wx/spinctrl.h>
#include <wx/textctrl.h>

#include "Common/wxTools.h"
#include "Links/SocketBase.h"
#include "Links/SocketBase_p.h"
#include "Links/SocketServer.h"
#include "Utilities/DataChannelComboBox.h"
#include "Utilities/IpComboBox.h"

SocketBaseUi::SocketBaseUi(wxWindow *parent)
    : LinkUi(parent)
    , m_serverComboBox{nullptr}
    , m_serverPortCtrl{nullptr}
    , m_dataChannelComboBox{nullptr}
{}

SocketBaseUi::~SocketBaseUi() {}

void SocketBaseUi::Disable()
{
    if (m_serverComboBox) {
        m_serverComboBox->Disable();
    }
    if (m_serverPortCtrl) {
        m_serverPortCtrl->Disable();
    }
}

void SocketBaseUi::Enable()
{
    if (m_serverComboBox) {
        m_serverComboBox->Enable();
    }
    if (m_serverPortCtrl) {
        m_serverPortCtrl->Enable();
    }
}

wxtJson SocketBaseUi::DoSave() const
{
    wxtJson json;
    SocketBaseParameterKeys keys;
    const wxString localhost = "127.0.0.1";
    // clang-format off
    json[keys.serverAddress] = m_serverComboBox ? m_serverComboBox->GetValue() : localhost;
    json[keys.serverPort] = m_serverPortCtrl ? m_serverPortCtrl->GetValue() : 51234;
    if (m_dataChannelComboBox) {
        int selection = m_dataChannelComboBox->GetSelection();
        int dataChannel = *reinterpret_cast<int*>(m_dataChannelComboBox->GetClientData(selection));
        json[keys.dataChannel] = dataChannel;
    }
    // clang-format on

    return json;
}

void SocketBaseUi::DoLoad(const wxtJson &json)
{
    SocketBaseParameterKeys keys;
    if (m_serverComboBox) {
        wxString serverAddress = wxtGetJsonObjValue(json,
                                                    keys.serverAddress,
                                                    std::string("127.0.0.1"));
        int section = m_serverComboBox->FindString(serverAddress);
        if (section != wxNOT_FOUND) {
            m_serverComboBox->SetSelection(section);
        }
    }
    if (m_serverPortCtrl) {
        int serverPort = wxtGetJsonObjValue<int>(json, keys.serverPort, 51234);
        m_serverPortCtrl->SetValue(serverPort);
    }
    if (m_dataChannelComboBox) {
        int dataChannel = wxtGetJsonObjValue<int>(json, keys.dataChannel, WEBSOCKET_OP_TEXT);
        m_dataChannelComboBox->SetDataChannel(dataChannel);
    }
}

void SocketBaseUi::DoRefreshDevice()
{
    if (GetLink()) {
        return;
    }

    if (m_serverComboBox) {
        m_serverComboBox->DoRefresh();
    }
}

wxTextCtrl *SocketBaseUi::GetClientInfoTextCtrl() const
{
    return m_clientInfoTextCtrl;
}

void SocketBaseUi::InitUiComponents(
    const std::vector<void (SocketBaseUi::*)(int, wxWindow *)> &funcs, wxWindow *parent)
{
    for (int i = 0; i < funcs.size(); i++) {
        (this->*funcs[i])(i, parent);
    }
    AddGrowableCol(1);
}

void SocketBaseUi::InitServerComboBox(int row, wxWindow *parent)
{
    m_serverAddressLabel = new wxStaticText(parent, wxID_ANY, _("Server IP"));
    Add(m_serverAddressLabel,
        wxGBPosition(row, 0),
        wxGBSpan(1, 1),
        wxALIGN_CENTER_VERTICAL | wxALL,
        0);

    m_serverComboBox = new IpComboBox(parent);
    Add(m_serverComboBox, wxGBPosition(row, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
}

void SocketBaseUi::InitServerPortCtrl(int row, wxWindow *parent)
{
    m_serverPortLabel = new wxStaticText(parent, wxID_ANY, _("Server port"));
    Add(m_serverPortLabel, wxGBPosition(row, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);

    m_serverPortCtrl = new wxSpinCtrl(parent, wxID_ANY);
    Add(m_serverPortCtrl, wxGBPosition(row, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    m_serverPortCtrl->SetRange(1, 65535);
    m_serverPortCtrl->SetValue(51234);
}

void SocketBaseUi::InitClientsComboBox(int row, wxWindow *parent)
{
    m_clientsLabel = new wxStaticText(parent, wxID_ANY, _("Write to"));
    Add(m_clientsLabel, wxGBPosition(row, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);

    m_clientsComboBox = new wxComboBox(parent,
                                       wxID_ANY,
                                       wxEmptyString,
                                       wxDefaultPosition,
                                       wxDefaultSize,
                                       0,
                                       nullptr,
                                       wxCB_READONLY);
    Add(m_clientsComboBox, wxGBPosition(row, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    m_clientsComboBox->Append(_("All Clients"), new wxString(""));
    m_clientsComboBox->SetSelection(0);

    m_clientsComboBox->Bind(wxEVT_COMBOBOX_CLOSEUP, [this](wxCommandEvent &) {
        Link *link = GetLink();
        SocketServer *socketServer = dynamic_cast<SocketServer *>(link);
        if (!socketServer) {
            return;
        }

        wxString str = m_clientsComboBox->GetValue();
        auto ctx = DoDecodeFlag(str.ToStdString());
        socketServer->SetCurrentClient(ctx.first, ctx.second);
    });
}

void SocketBaseUi::InitClientInfoTextCtrl(int row, wxWindow *parent)
{
    auto label = new wxStaticText(parent, wxID_ANY, _("Client Info"));
    m_clientInfoTextCtrl = new wxTextCtrl(parent,
                                          wxID_ANY,
                                          "0.0.0.0:0",
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxTE_READONLY | wxTE_CENTRE);
    Add(label, wxGBPosition(row, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    Add(m_clientInfoTextCtrl, wxGBPosition(row, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
}

void SocketBaseUi::InitDataChannelComboBox(int row, wxWindow *parent)
{
    m_dataChannelLabel = new wxStaticText(parent, wxID_ANY, _("Tx Channel"));
    Add(m_dataChannelLabel, wxGBPosition(row, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);

    m_dataChannelComboBox = new DataChannelComboBox(parent);
    Add(m_dataChannelComboBox, wxGBPosition(row, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);

    m_dataChannelComboBox->Bind(wxEVT_COMBOBOX_CLOSEUP, [this](wxCommandEvent &) {
        Link *link = GetLink();
        SocketBase *socket = dynamic_cast<SocketBase *>(link);
        if (!socket) {
            return;
        }

        int selection = m_dataChannelComboBox->GetSelection();
        int dataChannel = *reinterpret_cast<int *>(m_dataChannelComboBox->GetClientData(selection));
        socket->GetD<SocketBasePrivate *>()->dataChannel = dataChannel;
    });
}
