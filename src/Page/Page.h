/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/wx.h>

#include "Common/wxTools.h"

struct PageParameterKeys
{
    std::string io = {"io"};
    std::string settings = {"settings"};
};

class PageIO;
class PageSettings;
class Page : public wxPanel
{
public:
    Page(LinkType type, wxWindow *parent);
    Page() {}

    void DoLoad(const wxtJson &json);
    wxtJson DoSave() const;

private:
    PageSettings *m_pageSettings;
    PageIO *m_pageIO;

private:
    void OnOpen(wxCommandEvent &);
    void OnRefresh(wxCommandEvent &);
    void OnBytesRx(wxThreadEvent &e);
    void OnBytesTx(wxThreadEvent &e);
    void OnErrorOccurred(wxThreadEvent &e);
    void OnNewClient(wxThreadEvent &e);
    void OnDeleteClient(wxThreadEvent &e);
    void OnLinkOpened(wxThreadEvent &e);
    void OnLinkClosed(wxThreadEvent &e);
    void OnLinkResolve(wxThreadEvent &e);
    void OnWrap(wxCommandEvent &event);
    void OnClear(wxCommandEvent &);
    void OnInputTextFormatChanged(wxCommandEvent &);
    void OnWrite(wxCommandEvent &);

    void DoOutputText(std::shared_ptr<char> bytes, int len, std::string &fromTo, bool isRx);
    void DoOpen();
    void DoClose(bool ignoredCloseError);
    void DoClearClients();
    void DoWrite();

private:
    DECLARE_DYNAMIC_CLASS(Page);
    DECLARE_EVENT_TABLE();
};
