/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/tglbtn.h>
#include <wx/wx.h>

#include "Common/wxTools.h"

class LinkUi;
class PageSettingsLink : public wxStaticBoxSizer
{
public:
    PageSettingsLink(LinkType type, wxWindow *parent);

    void DoLoad(const wxtJson &parameters);
    wxtJson DoSave() const;

    wxButton *GetOpenButton() const;
    wxButton *GetRefreshButton() const;
    wxToggleButton *GetExtButton() const;
    LinkUi *GetLinkUi() const;

    void DoUpdateExtButtonText();

private:
    LinkUi *m_linkUi;
    wxButton *m_openButton;
    wxButton *m_refreshButton;
    wxToggleButton *m_extButton;

private:
    LinkUi *CreateLinkUi(LinkType type, wxWindow *parent);
};
