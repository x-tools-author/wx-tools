/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "LinkUi.h"

class wxSpinCtrl;
class IpComboBox;
class DataChannelComboBox;
class SocketBaseUi : public LinkUi
{
public:
    SocketBaseUi(wxWindow *parent = nullptr);
    ~SocketBaseUi();

    void Disable() override;
    void Enable() override;
    wxtJson DoSave() const override;
    void DoLoad(const wxtJson &json) override;
    void DoRefreshDevice() override;

    wxTextCtrl *GetClientInfoTextCtrl() const;

protected:
    // clang-format off
    void InitUiComponents(const std::vector<void (SocketBaseUi::*)(int, wxWindow *)> &funcs, wxWindow *parent);
    void InitServerComboBox(int row, wxWindow *parent);
    void InitServerPortCtrl(int row, wxWindow *parent);
    void InitClientsComboBox(int row, wxWindow *parent);
    void InitClientInfoTextCtrl(int row, wxWindow *parent);
    void InitDataChannelComboBox(int row, wxWindow *parent);
    // clang-format on

protected:
    wxStaticText *m_serverAddressLabel{nullptr};
    IpComboBox *m_serverComboBox{nullptr};
    wxStaticText *m_serverPortLabel{nullptr};
    wxSpinCtrl *m_serverPortCtrl{nullptr};

    wxStaticText *m_clientsLabel{nullptr};
    wxComboBox *m_clientsComboBox{nullptr};

    wxStaticText *m_dataChannelLabel{nullptr};
    DataChannelComboBox *m_dataChannelComboBox{nullptr};

    wxTextCtrl *m_clientInfoTextCtrl{nullptr};
};
