/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "FlowBitsComboBox.h"

FlowBitsComboBox::FlowBitsComboBox(wxWindow* parent)
    : wxComboBox(parent,
                 wxID_ANY,
                 wxEmptyString,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 nullptr,
                 wxCB_READONLY)
{
    Append(_("None"), new int(static_cast<int>(itas109::FlowNone)));
    Append(_("Hardware"), new int(static_cast<int>(itas109::FlowHardware)));
    Append(_("Software"), new int(static_cast<int>(itas109::FlowSoftware)));
    SetSelection(0);
}

FlowBitsComboBox::~FlowBitsComboBox()
{
    for (size_t i = 0; i < GetCount(); i++) {
        delete static_cast<int*>(GetClientData(i));
    }
}

itas109::FlowControl FlowBitsComboBox::GetFlowBits() const
{
    void* ptr = GetClientData(GetSelection());
    if (ptr == nullptr) {
        return itas109::FlowNone;
    }

    return static_cast<itas109::FlowControl>(*static_cast<int*>(ptr));
}

void FlowBitsComboBox::SetFlowBits(itas109::FlowControl flowBits)
{
    for (size_t i = 0; i < GetCount(); i++) {
        void* clientData = GetClientData(i);
        if (clientData == nullptr) {
            continue;
        }

        if (flowBits == static_cast<itas109::FlowControl>(*static_cast<int*>(clientData))) {
            SetSelection(i);
            return;
        }
    }

    SetSelection(0);
}
