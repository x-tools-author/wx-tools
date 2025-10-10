/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <memory>

#include <wx/notebook.h>
#include <wx/wx.h>

#include "Common/wxTools.h"

struct PageTabsParameterKeys
{
    const std::string luaTab{"luaTab"};
};

class Page;
class LuaTab;
class PageTabs : public wxNotebook
{
public:
    PageTabs(Page *parent);
    ~PageTabs() override;

    wxtJson DoSave() const;
    void DoLoad(const wxtJson &parameters);
    void OnBytesRead(std::shared_ptr<char> data, int size);

private:
    void OnInvokeWrite(wxThreadEvent &event);

private:
    LuaTab *m_luaTab;
    Page *m_page;

private:
    DECLARE_DYNAMIC_CLASS(PageTabs);
    DECLARE_EVENT_TABLE();
};