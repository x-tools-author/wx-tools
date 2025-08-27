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

class BaudRateComboBox;
class DataBitsComboBox;
class FlowBitsComboBox;
class ParityComboBox;
class PortNameComboBox;
class StopBitsComboBox;
class SerialPortUi : public LinkUi
{
public:
    SerialPortUi(wxWindow *parent = nullptr);
    ~SerialPortUi();

    void Disable() override;
    void Enable() override;
    wxtJson DoSave() const override;
    void DoLoad(const wxtJson &json) override;
    void DoRefreshDevice() override;

protected:
    Link *NewLink() override;

private:
    void SetupComboBox(wxComboBox *cb, const wxString &label, int row, wxWindow *parent);

private:
    PortNameComboBox *m_portNameComboBox;
    BaudRateComboBox *m_baudRateComboBox;
    DataBitsComboBox *m_dataBitsComboBox;
    StopBitsComboBox *m_stopBitsComboBox;
    FlowBitsComboBox *m_flowBitsComboBox;
    ParityComboBox *m_parityComboBox;
};
