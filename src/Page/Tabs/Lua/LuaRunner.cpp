/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "LuaRunner.h"

#include "Common/wxTools.h"

LuaRunner::LuaRunner(const wxString &fileName, wxEvtHandler *handler)
    : wxThread()
    , m_fileName(fileName)
    , m_handler(handler)
{}

LuaRunner::~LuaRunner() {}

LuaRunner::ExitCode LuaRunner::Entry()
{
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);
    while (!TestDestroy()) {
        wxtInfo() << "LuaRunner thread is running...";
        wxThread::Sleep(1000);
    }

    lua_close(m_lua);
    return 0;
}
