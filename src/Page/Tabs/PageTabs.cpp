/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageTabs.h"

#include "Common/wxTools.h"
#include "Lua/LuaRunner.h"
#include "Lua/LuaTab.h"
#include "Page/Page.h"

IMPLEMENT_ABSTRACT_CLASS(PageTabs, wxNotebook)
BEGIN_EVENT_TABLE(PageTabs, wxNotebook)
EVT_THREAD(wxtID_LUA_RUNNER_INVOKE_WRITE, PageTabs::OnInvokeWrite)
END_EVENT_TABLE()

PageTabs::PageTabs(Page *parent)
    : wxNotebook(parent, wxID_ANY)
    , m_luaTab(nullptr)
    , m_page(parent)
{
    m_luaTab = new LuaTab(this);
    AddPage(m_luaTab, _("Lua"));
}

PageTabs::~PageTabs() {}

void PageTabs::OnBytesRead(std::shared_ptr<char> data, int size)
{
    if (m_luaTab == nullptr) {
        return;
    }

    m_luaTab->OnBytesRead(data, size);
}

void PageTabs::OnInvokeWrite(wxThreadEvent &event)
{
    wxString str = event.GetString();
    if (str.IsEmpty()) {
        return;
    }

    if (m_page == nullptr) {
        return;
    }

    m_page->DoWrite(str);
}