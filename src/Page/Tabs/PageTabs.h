/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <memory>

#include <wx/notebook.h>
#include <wx/wx.h>

class Page;
class LuaTab;
class PageTabs : public wxNotebook
{
public:
    PageTabs(Page *parent);
    ~PageTabs() override;

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