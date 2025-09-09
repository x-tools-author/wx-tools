/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>

#include <wx/event.h>
#include <wx/thread.h>

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

enum wxtLuaRunnerID {
    wxtID_LUA_RUNNER_FINISHED = wxID_HIGHEST + 1,
    wxtID_LUA_RUNNER_ERROR,
    wxtID_LUA_RUNNER_INVOKE_WRITE
};

class LuaRunner : public wxThread
{
public:
    enum ParameterType { ParameterTypeTable, ParameterTypeString };

public:
    explicit LuaRunner(const wxString &fileName, wxEvtHandler *handler = nullptr);
    ~LuaRunner();

    void CloseLuaState();

protected:
    ExitCode Entry() override;

private:
    static int DoWrite(lua_State *L);
    static int DoLuaPrint(lua_State *L);
    static int DoSleep(lua_State *L);
    static int GetIsInterruptionRequested(lua_State *L);
    void DoOutput(const wxString &text);

private:
    lua_State *m_lua{nullptr};
    wxString m_fileName;
    wxEvtHandler *m_handler{nullptr};
    std::atomic_bool m_isInterruptionRequested{false};
};