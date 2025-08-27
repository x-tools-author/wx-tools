/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketServerUi.h"

SocketServerUi::SocketServerUi(wxWindow *parent)
    : SocketBaseUi(parent)
{}

SocketServerUi::~SocketServerUi() {}

void SocketServerUi::DoNewClient(const std::string &ip, uint16_t port)
{
    if (!m_clientsComboBox) {
        return;
    }

    if (m_clientsComboBox->FindString(ip) == wxNOT_FOUND) {
        m_clientsComboBox->Append(DoEncodeFlag(ip, port));
    }
}

void SocketServerUi::DoDeleteClient(const std::string &ip, uint16_t port)
{
    if (!m_clientsComboBox) {
        return;
    }

    int index = m_clientsComboBox->FindString(DoEncodeFlag(ip, port));
    if (index != wxNOT_FOUND) {
        if (index == m_clientsComboBox->GetSelection()) {
            m_clientsComboBox->SetSelection(0);
        }
        m_clientsComboBox->Delete(index);
    }
}

void SocketServerUi::DoClearClients()
{
    if (!m_clientsComboBox) {
        return;
    }

    while (m_clientsComboBox->GetCount() > 1) {
        m_clientsComboBox->Delete(1);
    }
}
