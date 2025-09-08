﻿/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <wx/notebook.h>
#include <wx/wx.h>

class LuaTab;
class PageTabs : public wxNotebook
{
public:
    PageTabs(wxWindow *parent);
    ~PageTabs() override;

private:
    LuaTab *m_luaTab;
};