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

void LuaRunner::CloseLuaState()
{
    if (m_lua) {
        lua_close(m_lua);
        m_lua = nullptr;
    }
}

LuaRunner::ExitCode LuaRunner::Entry()
{
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);
    while (!TestDestroy()) {
        int ret = luaL_dofile(m_lua, m_fileName.mb_str().data());
        if (ret != LUA_OK) {
            const char *errorMsg = lua_tostring(m_lua, -1);
            lua_pop(m_lua, 1); // Remove error message from the stack

            auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_LUA_RUNNER_ERROR);
            evt->SetString(wxString::FromUTF8(errorMsg));
            m_handler->QueueEvent(evt);
        }
        break; // Exit after one execution
    }

    lua_close(m_lua);
    m_lua = nullptr;

    wxtInfo() << "Lua script execution finished.";

    m_handler->QueueEvent(new wxThreadEvent(wxEVT_THREAD, wxtID_LUA_RUNNER_FINISHED));
    return (ExitCode) 0;
}
