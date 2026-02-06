/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "BLECenterManager.h"

#include <wx/artprov.h>
#include <wx/bmpbuttn.h>
#include <wx/combobox.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>
#include <wx/thread.h>

#include <simpleble/SimpleBLE.h>

#include "Common/wxTools.h"

IMPLEMENT_ABSTRACT_CLASS(BLECenterManager, wxPanel)
BEGIN_EVENT_TABLE(BLECenterManager, wxPanel)
EVT_THREAD(wxtID_SCAN_START, BLECenterManager::onScanStart)
EVT_THREAD(wxtID_SCAN_FOUND, BLECenterManager::onScanFound)
EVT_THREAD(wxtID_SCAN_UPDATED, BLECenterManager::onScanUpdated)
EVT_THREAD(wxtID_SCAN_STOP, BLECenterManager::onScanStop)
END_EVENT_TABLE()

class BLECenterManagerThread : public wxThread
{
public:
    BLECenterManagerThread(wxPanel *panel)
        : wxThread(wxTHREAD_JOINABLE)
        , m_panel(panel)
    {}

public:
    static SimpleBLE::Adapter getAdapter()
    {
        if (!SimpleBLE::Adapter::bluetooth_enabled()) {
            wxtWarning() << "Bluetooth is not enabled!";
            return SimpleBLE::Adapter();
        }

        std::vector<SimpleBLE::Adapter> adapter_list = SimpleBLE::Adapter::get_adapters();

        // no adapter found
        if (adapter_list.empty()) {
            wxtWarning() << "No BLE adapter found!";
            return SimpleBLE::Adapter();
        }

        // only one found, returning directly
        if (adapter_list.size() >= 1) {
            SimpleBLE::Adapter adapter = adapter_list.at(0);
            wxString id = wxString::FromUTF8(adapter.identifier());
            wxString address = wxString::FromUTF8(adapter.address());
            wxtInfo() << "------------------------------------------------------------------------";
            wxtInfo() << "Adapter: " << id << " [" << address << "]";
            wxtInfo() << "------------------------------------------------------------------------";
            return adapter;
        }

        // TODO: multiple adapters found, let user choose which one to use
        // But for now, just return the first one, most of the time there is only one adapter.
        return SimpleBLE::Adapter();
    }

protected:
    ExitCode Entry() override
    {
        SimpleBLE::Adapter adapter = BLECenterManagerThread::getAdapter();
        if (!adapter.initialized()) {
            wxtWarning() << "No BLE adapter found, scanning thread will exit.";
            return nullptr;
        }

        adapter.set_callback_on_scan_found([=](SimpleBLE::Peripheral peripheral) {
            if (!peripheral.is_connectable()) {
                return;
            }

            auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_SCAN_FOUND);
            wxtBlePeripheralItem item;
            item.identifier = wxString::FromUTF8(peripheral.identifier());
            item.address = wxString::FromUTF8(peripheral.address());
            item.rssi = peripheral.rssi();
            evt->SetPayload<wxtBlePeripheralItem>(item);
            m_panel->GetEventHandler()->QueueEvent(evt);
        });

        adapter.set_callback_on_scan_updated([=](SimpleBLE::Peripheral peripheral) {
            if (!peripheral.is_connectable()) {
                return;
            }

            auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_SCAN_UPDATED);
            wxtBlePeripheralItem item;
            item.identifier = wxString::FromUTF8(peripheral.identifier());
            item.address = wxString::FromUTF8(peripheral.address());
            item.rssi = peripheral.rssi();
            evt->SetPayload<wxtBlePeripheralItem>(item);
            m_panel->GetEventHandler()->QueueEvent(evt);
#if 0
            auto map = peripheral.manufacturer_data();
            for (const auto &it : map) {
                SimpleBLE::ByteArray bytes = it.second;
                const char *data = reinterpret_cast<const char *>(bytes.data());
                size_t len = bytes.size();
                wxtInfo() << "  Manufacturer ID: " << static_cast<int>(it.first)
                          << " Data: " << wxString::From8BitData(data, len);
            }
#endif
        });

        adapter.set_callback_on_scan_start([=]() {
            auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_SCAN_START);
            m_panel->GetEventHandler()->QueueEvent(evt);
        });

        adapter.set_callback_on_scan_stop([=]() {
            auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_SCAN_STOP);
            m_panel->GetEventHandler()->QueueEvent(evt);
        });

#if 1
        // Scan for 30 seconds.
        adapter.scan_for(30000);
#else
        adapter.scan_start();
#endif

        std::vector<SimpleBLE::Peripheral> peripherals = adapter.scan_get_results();
        std::cout << "The following devices were found:" << std::endl;
        for (size_t i = 0; i < peripherals.size(); i++) {
            if (!peripherals[i].is_connectable()) {
                continue;
            }

            std::string connectable_string = peripherals[i].is_connectable() ? "Connectable"
                                                                             : "Non-Connectable";
            std::string peripheral_string = peripherals[i].identifier() + " ["
                                            + peripherals[i].address() + "] "
                                            + std::to_string(peripherals[i].rssi()) + " dBm";

            std::cout << "[" << i << "] " << peripheral_string << " " << connectable_string
                      << std::endl;

            std::cout << "    Tx Power: " << std::dec << peripherals[i].tx_power() << " dBm"
                      << std::endl;
            std::cout << "    Address Type: " << peripherals[i].address_type() << std::endl;

            std::vector<SimpleBLE::Service> services = peripherals[i].services();
            for (auto &service : services) {
                std::cout << "    Service UUID: " << service.uuid() << std::endl;
                std::cout << "    Service data: " << service.data() << std::endl;
                auto rawData = service.data();
                const char *data = reinterpret_cast<const char *>(rawData.data());
                size_t len = rawData.size();
                wxtInfo() << "    Service name: " << wxString::FromUTF8(data, len);

                auto characteristics = service.characteristics();
                for (auto &characteristic : characteristics) {
                    std::cout << "        Characteristic UUID: " << characteristic.uuid()
                              << std::endl;
                }
            }

            std::map<uint16_t, SimpleBLE::ByteArray> manufacturer_data = peripherals[i]
                                                                             .manufacturer_data();
            for (auto &[manufacturer_id, data] : manufacturer_data) {
                std::cout << "    Manufacturer ID: " << manufacturer_id << std::endl;
                std::cout << "    Manufacturer data: " << data << std::endl;
            }
        }

        wxtWarning() << "Scan complete, thread will exit.";
        return nullptr;
    }

private:
    wxPanel *m_panel{nullptr};
};

class BLECenterManagerPrivate
{
public:
    BLECenterManagerPrivate(BLECenterManager *q_ptr)
        : q(q_ptr)
    {}
    ~BLECenterManagerPrivate() = default;

public:
    wxComboBox *m_blePeripheralComboBox{nullptr};
    wxComboBox *m_bleServiceComboBox{nullptr};
    wxComboBox *m_bleCharacteristicComboBox{nullptr};
    wxComboBox *m_bleDescriptorComboBox{nullptr};
    BLECenterManagerThread *m_thread{nullptr};

public:
    void DoRefreshDevice()
    {
        SimpleBLE::Adapter adapter = BLECenterManagerThread::getAdapter();
        if (!adapter.initialized()) {
            //wxMessageBox(_("No BLE adapter found."), _("Error"), wxOK | wxICON_ERROR);
            return;
        }

        if (m_thread && m_thread->IsRunning()) {
            m_thread->Delete();
            m_thread = nullptr;
            return;
        }

        m_thread = new BLECenterManagerThread(q);
        m_thread->Create();
        m_thread->Run();
    }

    void DoAddComboBox(wxGridBagSizer *sizer, const wxString &label, wxComboBox *control)
    {
        auto text = new wxStaticText(q, wxID_ANY, label);
        sizer->Add(text,
                   wxGBPosition(m_rowCount, 0),
                   wxGBSpan(1, 1),
                   wxALIGN_CENTER_VERTICAL | wxALL,
                   0);
        m_rowCount++;
        sizer->Add(control, wxGBPosition(m_rowCount, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
        m_rowCount++;
    }

private:
    BLECenterManager *q{nullptr};
    int m_rowCount{0};
};

#define BLEComboBox \
    wxComboBox(this, \
               wxID_ANY, \
               wxEmptyString, \
               wxDefaultPosition, \
               wxDefaultSize, \
               0, \
               nullptr, \
               wxCB_READONLY)

BLECenterManager::BLECenterManager(wxWindow *parent)
    : wxPanel(parent)
{
    wxGridBagSizer *sizer = new wxGridBagSizer(0, 0);
    SetSizer(sizer);

    d = new BLECenterManagerPrivate(this);
    d->m_blePeripheralComboBox = new BLEComboBox;
    d->m_bleServiceComboBox = new BLEComboBox;
    d->m_bleCharacteristicComboBox = new BLEComboBox;
    d->m_bleDescriptorComboBox = new BLEComboBox;

    d->DoAddComboBox(sizer, _("Peripheral"), d->m_blePeripheralComboBox);
    d->DoAddComboBox(sizer, _("Service"), d->m_bleServiceComboBox);
    d->DoAddComboBox(sizer, _("Characteristic"), d->m_bleCharacteristicComboBox);
    d->DoAddComboBox(sizer, _("Descriptor"), d->m_bleDescriptorComboBox);

    sizer->AddGrowableCol(0);
    d->DoRefreshDevice();
}

BLECenterManager::~BLECenterManager()
{
    // Nothing to do yet...
}

void BLECenterManager::onScanStart(wxThreadEvent &e)
{
    wxtInfo() << "Scan started.";
}

void BLECenterManager::onScanFound(wxThreadEvent &e)
{
    wxtBlePeripheralItem item = e.GetPayload<wxtBlePeripheralItem>();
    if (item.identifier.IsEmpty()) {
        return;
    }

    const wxString txt = item.identifier.IsEmpty() ? item.address : item.identifier;
    d->m_blePeripheralComboBox->Append(txt);
    int index = d->m_blePeripheralComboBox->GetCount() - 1;
    d->m_blePeripheralComboBox->SetClientData(index, new wxtBlePeripheralItem(item));
    if (d->m_blePeripheralComboBox->GetSelection() == wxNOT_FOUND) {
        d->m_blePeripheralComboBox->SetSelection(0);
    }
}

void BLECenterManager::onScanUpdated(wxThreadEvent &e)
{
    // Update existing item
    wxtBlePeripheralItem item = e.GetPayload<wxtBlePeripheralItem>();
    int count = d->m_blePeripheralComboBox->GetCount();
    for (int i = 0; i < count; ++i) {
        void *rawData = d->m_blePeripheralComboBox->GetClientData(i);
        wxtBlePeripheralItem *data = static_cast<wxtBlePeripheralItem *>(rawData);
        if (data && data->address == item.address) {
            const wxString txt = item.identifier.IsEmpty() ? item.address : item.identifier;
            d->m_blePeripheralComboBox->SetString(i, txt);
            data->identifier = item.identifier;
            data->rssi = item.rssi;
            return;
        }
    }

    // Not found, add new item
    onScanFound(e);
}

void BLECenterManager::onScanStop(wxThreadEvent &e)
{
    wxtInfo() << "Scan stopped.";
}