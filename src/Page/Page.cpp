/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Page.h"

#include <fmt/format.h>
#include <wx/gbsizer.h>

#include "Common/wxTools.h"
#include "Links/Link.h"
#include "LinksUi/LinkUi.h"
#include "LinksUi/SocketClientUi.h"
#include "LinksUi/SocketServerUi.h"
#include "LinksUi/UDPServerUi.h"

#include "PageIO.h"
#include "PageIOInput.h"
#include "PageIOOutput.h"
#include "PageSettings.h"
#include "PageSettingsInput.h"
#include "PageSettingsInputPopup.h"
#include "PageSettingsLink.h"
#include "PageSettingsOutput.h"
#include "PageSettingsOutputPopup.h"

IMPLEMENT_ABSTRACT_CLASS(Page, wxPanel)
BEGIN_EVENT_TABLE(Page, wxPanel)
EVT_THREAD(wxtID_LINK_RX, Page::OnBytesRx)
EVT_THREAD(wxtID_LINK_TX, Page::OnBytesTx)
EVT_THREAD(wxtID_LINK_ERROR, Page::OnErrorOccurred)
EVT_THREAD(wxtID_LINK_NEW, Page::OnNewClient)
EVT_THREAD(wxtID_LINK_DELETE, Page::OnDeleteClient)
EVT_THREAD(wxtID_LINK_OPENED, Page::OnLinkOpened)
EVT_THREAD(wxtID_LINK_CLOSED, Page::OnLinkClosed)
EVT_THREAD(wxtID_LINK_RESOLVED, Page::OnLinkResolve)
EVT_COMMAND(wxID_ANY, wxtEVT_SETTINGS_LINK_OPEN, Page::OnOpen)
EVT_COMMAND(wxID_ANY, wxtEVT_SETTINGS_LINK_POPUP_REFRESH, Page::OnRefresh)
EVT_COMMAND(wxID_ANY, wxtEVT_SETTINGS_OUTPUT_CLEAR, Page::OnClear)
EVT_COMMAND(wxID_ANY, wxtEVT_SETTINGS_OUTPUT_WRAP, Page::OnWrap)
EVT_COMMAND(wxID_ANY, wxtEVT_SETTINGS_INPUT_WRITE, Page::OnWrite)
EVT_COMMAND(wxID_ANY, wxtEVT_SETTINGS_INPUT_FORMAT, Page::OnInputTextFormatChanged)
END_EVENT_TABLE()

Page::Page(LinkType type, wxWindow *parent)
    : wxPanel(parent, wxID_ANY)
    , m_pageSettings(nullptr)
    , m_pageIO(nullptr)
{
    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizerAndFit(sizer);

    m_pageSettings = new PageSettings(type, this);
    sizer->Add(m_pageSettings, 0, wxEXPAND | wxALL, 4);

    m_pageIO = new PageIO(this);
    sizer->Add(m_pageIO, 1, wxEXPAND | wxALL, 4);

    Layout();
}

void Page::DoLoad(const wxtJson &json)
{
    if (!json.is_object()) {
        return;
    }

    PageParameterKeys keys;
    m_pageSettings->DoLoad(wxtGetJsonObjValue<wxtJson>(json, keys.settings, wxtJson::object()));
    m_pageIO->DoLoad(wxtGetJsonObjValue<wxtJson>(json, keys.io, wxtJson::object()));

    int format = m_pageSettings->GetInputSettings()->GetTextFormat();
    bool wrap = m_pageSettings->GetOutputSettings()->GetWrap();
    m_pageIO->GetInput()->SetTextFormat(static_cast<TextFormat>(format));
    m_pageIO->GetOutput()->SetWrap(wrap);
}

wxtJson Page::DoSave() const
{
    wxtJson json;
    PageParameterKeys keys;
    json[keys.settings] = m_pageSettings->DoSave();
    json[keys.io] = m_pageIO->DoSave();
    return json;
}

void Page::OnOpen(wxCommandEvent &)
{
    PageSettingsLink *linkSettings = m_pageSettings->GetLinkSettings();
    LinkUi *linkUi = linkSettings->GetLinkUi();

    wxButton *btn = linkSettings->GetOpenButton();
    btn->Disable();

    if (linkUi->IsOpen()) {
        DoClose(true);
    } else {
        DoOpen();
    }
}

void Page::OnRefresh(wxCommandEvent &)
{
    auto linkSettings = m_pageSettings->GetLinkSettings();
    auto linkUi = linkSettings->GetLinkUi();
    linkUi->DoRefreshDevice();
}

void Page::OnBytesRx(wxThreadEvent &e)
{
    wxtDataItem item = e.GetPayload<wxtDataItem>();
    DoOutputText(item.data, item.len, item.flag, true);
}

void Page::OnBytesTx(wxThreadEvent &e)
{
    auto item = e.GetPayload<wxtDataItem>();
    DoOutputText(item.data, item.len, item.flag, false);
}

void Page::OnErrorOccurred(wxThreadEvent &e)
{
    DoClose(e.GetInt() == wxtIgnoreCloseErrorPopup);
    wxButton *btn = m_pageSettings->GetLinkSettings()->GetOpenButton();
    btn->Enable(true);
    DoClearClients();

    if (e.GetString().IsEmpty()) {
        return;
    }

    if (!(e.GetInt() == wxtIgnoreCloseErrorPopup)) {
        wxLogWarning(e.GetString());
    }
}

void Page::OnNewClient(wxThreadEvent &e)
{
    auto linkUi = m_pageSettings->GetLinkSettings()->GetLinkUi();
    if (!dynamic_cast<SocketServerUi *>(linkUi)) {
        return;
    }

    auto serverUi = dynamic_cast<SocketServerUi *>(linkUi);
    wxString ip = e.GetString();
    int port = e.GetInt();
    serverUi->DoNewClient(ip.ToStdString(), port);
}

void Page::OnDeleteClient(wxThreadEvent &e)
{
    auto linkUi = m_pageSettings->GetLinkSettings()->GetLinkUi();
    if (!dynamic_cast<SocketServerUi *>(linkUi)) {
        return;
    }

    auto serverUi = dynamic_cast<SocketServerUi *>(linkUi);
    wxString ip = e.GetString();
    int port = e.GetInt();
    serverUi->DoDeleteClient(ip.ToStdString(), port);
}

void Page::OnLinkOpened(wxThreadEvent &e)
{
    wxButton *btn = m_pageSettings->GetLinkSettings()->GetOpenButton();
    btn->Enable(true);
}

void Page::OnLinkClosed(wxThreadEvent &e)
{
    wxButton *btn = m_pageSettings->GetLinkSettings()->GetOpenButton();
    btn->Enable(true);

    e.SetString("0.0.0.0");
    e.SetInt(0);
    OnLinkResolve(e);
    DoClearClients();
}

void Page::OnLinkResolve(wxThreadEvent &e)
{
    LinkUi *linkUi = m_pageSettings->GetLinkSettings()->GetLinkUi();
    if (dynamic_cast<SocketClientUi *>(linkUi)) {
        auto clientUi = dynamic_cast<SocketClientUi *>(linkUi);
        auto textCtrl = clientUi->GetClientInfoTextCtrl();
        wxString ip = e.GetString();
        int port = e.GetInt();
        textCtrl->SetLabelText(DoEncodeFlag(ip.ToStdString(), port));
    }
}

void Page::OnWrap(wxCommandEvent &event)
{
    m_pageIO->GetOutput()->SetWrap(event.GetInt());
}

void Page::OnClear(wxCommandEvent &)
{
    m_pageIO->GetOutput()->Clear();
}

void Page::OnInputTextFormatChanged(wxCommandEvent &)
{
    PageSettingsInput *inputSettings = m_pageSettings->GetInputSettings();
    int format = inputSettings->GetTextFormat();
    m_pageIO->GetInput()->SetTextFormat(static_cast<TextFormat>(format));
}

void Page::OnWrite(wxCommandEvent &)
{
    PageSettingsLink *linkSettings = m_pageSettings->GetLinkSettings();
    LinkUi *linkUi = linkSettings->GetLinkUi();
    if (!linkUi->IsOpen()) {
        PageSettingsInput *inputSettings = m_pageSettings->GetInputSettings();
        inputSettings->DoStopTimer();
        wxMessageBox("Link is not open!", "Error", wxICON_ERROR);
        return;
    }

    DoWrite();
}

std::string dateTimeString(bool showDate, bool showTime, bool showMs)
{
    std::string text;
    if (showDate) {
        if (showTime) {
            text = GetDateTimeString("%Y-%m-%d %H:%M:%S", showMs) + " " + text;
        } else {
            if (showMs) {
                text = GetDateTimeString("%Y-%m-%d", showMs) + " " + text;
            }
        }
    } else {
        if (showTime) {
            text = GetDateTimeString("%H:%M:%S", showMs) + " " + text;
        } else {
            if (showMs) {
                text = GetDateTimeString("%H:%M:%S", showMs) + " " + text;
            }
        }
    }

    return text;
}

std::string flagString(bool isRx, const std::string &fromTo, bool showFlag)
{
    if (!showFlag) {
        return isRx ? "Rx" : "Tx";
    }

    std::stringstream stringStream;
    if (isRx) {
        stringStream << "Rx" << ("<-") << fromTo;

    } else {
        stringStream << "Tx" << ("->") << fromTo;
    }

    return stringStream.str();
}

void Page::DoOutputText(std::shared_ptr<char> bytes, int len, std::string &fromTo, bool isRx)
{
    PageSettingsOutput *outputSettings = m_pageSettings->GetOutputSettings();
    TextFormat outputFormat = outputSettings->GetTextFormat();
    bool showDate = outputSettings->GetShowDate();
    bool showTime = outputSettings->GetShowTime();
    bool showMs = outputSettings->GetShowMs();
    bool showRx = outputSettings->GetShowRx();
    bool showTx = outputSettings->GetShowTx();
    bool showFlag = outputSettings->GetShowFlag();
    bool terminalMode = outputSettings->GetTerminalMode();
    std::string text = DoDecodeBytes(bytes, len, static_cast<int>(outputFormat));
    std::string dateTimeString = ::dateTimeString(showDate, showTime, showMs);
    std::string flagString = ::flagString(isRx, fromTo, showFlag);

    if (terminalMode) {
        if (isRx) {
            m_pageIO->GetOutput()->AppendText(text, false);
        }

        return;
    }

    if (isRx && !showRx) {
        return;
    }

    if (!isRx && !showTx) {
        return;
    }

    // Replace '\n' with '\\n' and replace '\r' with '\\r'
    wxString textCopy = text;
    textCopy.Replace("\n", "\\n");
    textCopy.Replace("\r", "\\r");
    text = textCopy.ToStdString();

    wxString str;
    if (dateTimeString.empty()) {
        str = wxString::Format("[%s] %s", flagString, text);
    } else {
        str = wxString::Format("[%s %s] %s", dateTimeString, flagString, text);
    }

    // Filter
    PageSettingsOutputPopup *outputPopup = outputSettings->GetPopup();
    wxArrayString filter = outputPopup->GetFilter();
    if (filter.IsEmpty()) {
        m_pageIO->GetOutput()->AppendText(str, true);
    } else {
        for (size_t i = 0; i < filter.GetCount(); i++) {
            if (str.Contains(filter[i])) {
                m_pageIO->GetOutput()->AppendText(str, true);
                break;
            }
        }
    }
}

void Page::DoOpen()
{
    PageSettingsLink *linkSettings = m_pageSettings->GetLinkSettings();
    LinkUi *linkUi = linkSettings->GetLinkUi();
    if (linkUi->Open(GetEventHandler())) {
        linkUi->Disable();
        auto btn = linkSettings->GetOpenButton();
        btn->SetLabel(_("Close"));
    } else {
        wxMessageBox(_("Failed to open link."), _("Error"), wxICON_ERROR);
    }
}

void Page::DoClose(bool ignoredCloseError)
{
    PageSettingsLink *linkSettings = m_pageSettings->GetLinkSettings();
    LinkUi *linkUi = linkSettings->GetLinkUi();

    PageSettingsInput *inputSettings = m_pageSettings->GetInputSettings();
    inputSettings->DoStopTimer();

    linkUi->Close(ignoredCloseError);
    linkUi->Enable();

    auto btn = linkSettings->GetOpenButton();
    btn->SetLabel(_("Open"));
}

void Page::DoClearClients()
{
    auto linkUi = m_pageSettings->GetLinkSettings()->GetLinkUi();
    UDPServerUi *serverUi = dynamic_cast<UDPServerUi *>(linkUi);
    if (serverUi) {
        serverUi->DoClearClients();
    }
}

void Page::DoWrite()
{
    PageSettingsLink *linkSettings = m_pageSettings->GetLinkSettings();
    LinkUi *linkUi = linkSettings->GetLinkUi();
    if (!linkUi->IsOpen()) {
        wxMessageBox(_("Link is not opened."), _("Error"), wxICON_ERROR);
        return;
    }

    PageSettingsInput *inputSettings = m_pageSettings->GetInputSettings();
    PageSettingsInputPopup *popup = inputSettings->GetPopup();
    wxtJson inputParameters = popup->DoSave();

    PageSettingsInputPopupParameterKeys keys;
    int prefix = inputParameters[keys.prefix].get<int>();
    int suffix = inputParameters[keys.suffix].get<int>();
    int escIndex = inputParameters[keys.escIndex].get<int>();
    int startIndex = inputParameters[keys.startIndex].get<int>();
    int endIndex = inputParameters[keys.endIndex].get<int>();
    int algorithm = inputParameters[keys.algorithm].get<int>();
    bool addCrc = inputParameters[keys.addCrc].get<bool>();
    bool bigEndian = inputParameters[keys.bigEndian].get<bool>();

    Link *link = linkUi->GetLink();
    wxString text = m_pageIO->GetInput()->GetInputText();
    if (text.IsEmpty()) {
        return;
    }

    text = GetEscapeString(text.ToStdString(), escIndex);
    int len = 0;
    int format = inputSettings->GetTextFormat();
    auto bytes = DoEncodeBytes(text.ToStdString(), len, format);
    if (len <= 0) {
        return;
    }

    std::vector<char> prefixChars = GetAdditionChars(prefix);
    std::vector<char> crcChars;
    if (addCrc) {
        crcChars = DoCalculateCRC(bytes, len, algorithm, startIndex, endIndex, bigEndian);
    }
    std::vector<char> suffixChars = GetAdditionChars(suffix);

    std::vector<char> tmp = prefixChars;
    for (int i = 0; i < len; i++) {
        tmp.push_back(bytes.get()[i]);
    }
    tmp.insert(tmp.end(), crcChars.begin(), crcChars.end());
    tmp.insert(tmp.end(), suffixChars.begin(), suffixChars.end());

    std::shared_ptr<char> allBytes(new char[tmp.size()], [](char *p) { delete[] p; });
    memcpy(allBytes.get(), tmp.data(), tmp.size());
    link->Write(std::move(allBytes), tmp.size());
}
