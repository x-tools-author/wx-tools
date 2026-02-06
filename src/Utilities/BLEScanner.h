/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/panel.h>
#include <wx/wx.h>

#define wxtID_SCAN_START wxID_HIGHEST + 0
#define wxtID_SCAN_FOUND wxID_HIGHEST + 1
#define wxtID_SCAN_UPDATED wxID_HIGHEST + 2
#define wxtID_SCAN_STOP wxID_HIGHEST + 3

struct wxtBlePeripheralItem
{
    wxString identifier;
    wxString address;
    int rssi;
};

class BLEScannerPrivate;
class BLEScanner : public wxPanel
{
    BLEScannerPrivate *d{nullptr};

public:
    BLEScanner(wxWindow *parent);
    ~BLEScanner() override;

    void onScanStart(wxThreadEvent &e);
    void onScanFound(wxThreadEvent &e);
    void onScanUpdated(wxThreadEvent &e);
    void onScanStop(wxThreadEvent &e);

private:
    DECLARE_DYNAMIC_CLASS(BLEScanner);
    DECLARE_EVENT_TABLE();
};