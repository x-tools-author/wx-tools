/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageTabs.h"

#include "Lua/LuaTab.h"

PageTabs::PageTabs(wxWindow* parent)
    : wxNotebook(parent, wxID_ANY)
    , m_luaTab(nullptr)
{
    m_luaTab = new LuaTab(this);
    AddPage(m_luaTab, _("Lua"));
}

PageTabs::~PageTabs() {}