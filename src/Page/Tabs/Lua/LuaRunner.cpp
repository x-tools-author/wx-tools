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
    m_isInterruptionRequested = true;
}

LuaRunner::ExitCode LuaRunner::Entry()
{
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);
    lua_pushlightuserdata(m_lua, this);
    lua_setglobal(m_lua, "LUA_RUNNER_INSTANCE");
    lua_register(m_lua, "print", &LuaRunner::DoLuaPrint);
    lua_register(m_lua, "wxt_write", &LuaRunner::DoWrite);
    lua_register(m_lua, "wxt_sleep", &LuaRunner::DoSleep);
    lua_register(m_lua, "wxt_is_interruption_requested", &LuaRunner::GetIsInterruptionRequested);

    int ret = luaL_dofile(m_lua, m_fileName.mb_str().data());
    if (ret != LUA_OK) {
        const char *errorMsg = lua_tostring(m_lua, -1);
        lua_pop(m_lua, 1); // Remove error message from the stack

        auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_LUA_RUNNER_ERROR);
        evt->SetString(wxString::FromUTF8(errorMsg));
        m_handler->QueueEvent(evt);
    }

    lua_close(m_lua);
    m_lua = nullptr;

    m_handler->QueueEvent(new wxThreadEvent(wxEVT_THREAD, wxtID_LUA_RUNNER_FINISHED));
    wxtInfo() << "[Lua] Finished.";
    return (ExitCode) 0;
}

int LuaRunner::DoWrite(lua_State *L)
{
    const char *data = luaL_checkstring(L, 1);
    if (data) {
        lua_getglobal(L, "LUA_RUNNER_INSTANCE");
        LuaRunner *runner = static_cast<LuaRunner *>(lua_touserdata(L, -1));
        lua_pop(L, 1);

        if (runner) {
            auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_LUA_RUNNER_INVOKE_WRITE);
            evt->SetString(wxString::FromUTF8(data));
            runner->m_handler->QueueEvent(evt);
        } else {
            wxtInfo() << "[Lua]" << data;
        }
    }

    return 0;
}

int LuaRunner::DoLuaPrint(lua_State *L)
{
    int n = lua_gettop(L);
    wxString output;

    for (int i = 1; i <= n; i++) {
        if (lua_isstring(L, i)) {
            output += lua_tostring(L, i);
        } else if (lua_isnumber(L, i)) {
            output += wxString::Format("%f", lua_tonumber(L, i));
        } else if (lua_isboolean(L, i)) {
            output += lua_toboolean(L, i) ? "true" : "false";
        } else if (lua_isnil(L, i)) {
            output += "nil";
        } else {
            output += wxString::Format("(%1)", luaL_typename(L, i));
        }

        if (i < n) {
            output += "\t";
        }
    }

    lua_getglobal(L, "LUA_RUNNER_INSTANCE");
    LuaRunner *runner = static_cast<LuaRunner *>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (runner) {
        runner->DoOutput(output);
    } else {
        wxtInfo() << "[Lua]" << output;
    }

    return 0;
}

int LuaRunner::DoSleep(lua_State *L)
{
    int ms = (int) luaL_checkinteger(L, 1);
    if (ms > 0) {
        wxThread::Sleep(ms);
    }
    return 0;
}

int LuaRunner::GetIsInterruptionRequested(lua_State *L)
{
    lua_getglobal(L, "LUA_RUNNER_INSTANCE");
    LuaRunner *runner = static_cast<LuaRunner *>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (runner) {
        return runner->m_isInterruptionRequested;
    } else {
        return 1;
    }
}

void LuaRunner::DoOutput(const wxString &text)
{
    auto *evt = new wxThreadEvent(wxEVT_THREAD, wxtID_LUA_RUNNER_ERROR);
    evt->SetString(text);
    m_handler->QueueEvent(evt);
}