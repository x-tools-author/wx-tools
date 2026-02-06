/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "LinkUi.h"

class BLECenterUiPrivate;
class BLECenterUi : public LinkUi
{
    BLECenterUiPrivate *d{nullptr};

public:
    BLECenterUi(wxWindow *parent = nullptr);
    ~BLECenterUi();

    void Disable() override;
    void Enable() override;
    wxtJson DoSave() const override;
    void DoLoad(const wxtJson &json) override;
    void DoRefreshDevice() override;

protected:
    Link *NewLink() override;
};
