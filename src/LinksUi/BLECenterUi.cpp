/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "BLECenterUi.h"

#include <wx/gbsizer.h>
#include <wx/string.h>

#include "Utilities/BLECenterManager.h"
#include "Utilities/BaudRateComboBox.h"
#include "Utilities/DataBitsComboBox.h"
#include "Utilities/FlowBitsComboBox.h"
#include "Utilities/ParityComboBox.h"
#include "Utilities/PortNameComboBox.h"
#include "Utilities/StopBitsComboBox.h"

#include "Links/BLECenter.h"

class BLECenterUiPrivate
{
public:
    BLECenterUiPrivate(BLECenterUi *q_ptr)
        : q(q_ptr)
    {}

    ~BLECenterUiPrivate() = default;

public:
    BLECenterManager *m_bleMgr{nullptr};

private:
    BLECenterUi *q{nullptr};
};

BLECenterUi::BLECenterUi(wxWindow *parent)
    : LinkUi(parent)
{
    d = new BLECenterUiPrivate(this);
    d->m_bleMgr = new BLECenterManager(parent);
    Add(d->m_bleMgr, wxGBPosition(0, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
#if 0
    m_portNameComboBox = new PortNameComboBox(parent);
    m_baudRateComboBox = new BaudRateComboBox(parent);
    m_dataBitsComboBox = new DataBitsComboBox(parent);
    m_stopBitsComboBox = new StopBitsComboBox(parent);
    m_flowBitsComboBox = new FlowBitsComboBox(parent);
    m_parityComboBox = new ParityComboBox(parent);

    SetupComboBox(m_portNameComboBox, _("Port name"), 1, parent);
    SetupComboBox(m_baudRateComboBox, _("Baud rate"), 2, parent);
    SetupComboBox(m_dataBitsComboBox, _("Data bits"), 3, parent);
    SetupComboBox(m_stopBitsComboBox, _("Stop bits"), 4, parent);
    SetupComboBox(m_parityComboBox, _("Parity"), 5, parent);
    SetupComboBox(m_flowBitsComboBox, _("Flow bits"), 6, parent);
#endif

    AddGrowableCol(1);
}

BLECenterUi::~BLECenterUi()
{
    delete dynamic_cast<BLECenter *>(GetLink());
}

void BLECenterUi::Disable() {}

void BLECenterUi::Enable() {}

wxtJson BLECenterUi::DoSave() const
{
    nlohmann::json json = nlohmann::json(nlohmann::json::object());
    BLECenterParameterKeys keys;
    return json;
}

void BLECenterUi::DoLoad(const wxtJson &json)
{
    BLECenterParameterKeys keys;
}

void BLECenterUi::DoRefreshDevice()
{
    if (GetLink()) {
        return;
    }
}

Link *BLECenterUi::NewLink()
{
    return new BLECenter();
}
