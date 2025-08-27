/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "StopBitsComboBox.h"

StopBitsComboBox::StopBitsComboBox(wxWindow* parent)
    : wxComboBox(parent,
                 wxID_ANY,
                 wxEmptyString,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 nullptr,
                 wxCB_READONLY)
{
    Append("1", new int(static_cast<int>(itas109::StopOne)));
#ifdef _WIN32
    Append("1.5", new int(static_cast<int>(itas109::StopOneAndHalf)));
#endif
    Append("2", new int(static_cast<int>(itas109::StopTwo)));
    SetSelection(0);
}

StopBitsComboBox::~StopBitsComboBox()
{
    for (size_t i = 0; i < GetCount(); i++) {
        delete static_cast<int *>(GetClientData(i));
    }
}

itas109::StopBits StopBitsComboBox::GetStopBits() const
{
    void *ptr = GetClientData(GetSelection());
    if (ptr == nullptr) {
        return itas109::StopOne;
    }

    return static_cast<itas109::StopBits>(*static_cast<int *>(ptr));
}

void StopBitsComboBox::SetStopBits(itas109::StopBits stopBits)
{
    for (size_t i = 0; i < GetCount(); i++) {
        void *clientData = GetClientData(i);
        if (clientData == nullptr) {
            continue;
        }

        if (stopBits == static_cast<itas109::StopBits>(*static_cast<int *>(clientData))) {
            SetSelection(i);
            return;
        }
    }

    SetSelection(0);
}
