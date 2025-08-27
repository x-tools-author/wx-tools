/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "ParityComboBox.h"

ParityComboBox::ParityComboBox(wxWindow* parent)
    : wxComboBox(parent,
                 wxID_ANY,
                 wxEmptyString,
                 wxDefaultPosition,
                 wxDefaultSize,
                 0,
                 nullptr,
                 wxCB_READONLY)
{
    Append(_("None"), new int(static_cast<int>(itas109::ParityNone)));
    Append(_("Odd"), new int(static_cast<int>(itas109::ParityOdd)));
    Append(_("Even"), new int(static_cast<int>(itas109::ParityEven)));
    Append(_("Mark"), new int(static_cast<int>(itas109::ParityMark)));
    Append(_("Space"), new int(static_cast<int>(itas109::ParitySpace)));
    SetSelection(0);
}

itas109::Parity ParityComboBox::GetParity() const
{
    void* ptr = GetClientData(GetSelection());
    if (ptr == nullptr) {
        return itas109::ParityNone;
    }

    return static_cast<itas109::Parity>(*static_cast<int*>(ptr));
}

void ParityComboBox::SetParity(itas109::Parity parity)
{
    for (size_t i = 0; i < GetCount(); i++) {
        void* clientData = GetClientData(i);
        if (clientData == nullptr) {
            continue;
        }

        if (parity == static_cast<itas109::Parity>(*static_cast<int*>(clientData))) {
            SetSelection(i);
            return;
        }
    }

    SetSelection(0);
}
