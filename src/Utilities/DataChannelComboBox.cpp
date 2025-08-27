/***************************************************************************************************
 * Copyright 2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "DataChannelComboBox.h"

#include <mongoose.h>

DataChannelComboBox::DataChannelComboBox(wxWindow* parent)
    : wxComboBox(parent,
                 wxID_ANY,
                 wxEmptyString,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 nullptr,
                 wxCB_READONLY)
{
    Append(_("Text"), new int(WEBSOCKET_OP_TEXT));
    Append(_("Binary"), new int(WEBSOCKET_OP_BINARY));

    SetSelection(0);
}

DataChannelComboBox::~DataChannelComboBox()
{
    for (int i = 0; i < GetCount(); i++) {
        void* clientData = GetClientData(i);
        if (clientData != nullptr) {
            delete static_cast<int*>(clientData);
        }
    }
}

int DataChannelComboBox::GetDataChannel() const
{
    void* ptr = GetClientData(GetSelection());
    if (ptr == nullptr) {
        return WEBSOCKET_OP_TEXT;
    }

    return static_cast<int>(*static_cast<int*>(ptr));
}

void DataChannelComboBox::SetDataChannel(int dataChannel)
{
    for (size_t i = 0; i < GetCount(); i++) {
        void* clientData = GetClientData(i);
        if (clientData == nullptr) {
            continue;
        }

        if (dataChannel == static_cast<int>(*static_cast<int*>(clientData))) {
            SetSelection(i);
            return;
        }
    }

    SetSelection(0);
}
