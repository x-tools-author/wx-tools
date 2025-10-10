/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of wxTools project.
 *
 * wxTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "LuaTab.h"

#include <wx/artprov.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/textdlg.h>

#include "Common/wxTools.h"
#include "LuaRunner.h"

IMPLEMENT_ABSTRACT_CLASS(LuaTab, wxPanel)
BEGIN_EVENT_TABLE(LuaTab, wxPanel)
EVT_THREAD(wxtID_LUA_RUNNER_FINISHED, LuaTab::OnThreadFinished)
EVT_THREAD(wxtID_LUA_RUNNER_ERROR, LuaTab::OnThreadError)
EVT_THREAD(wxtID_LUA_RUNNER_INVOKE_WRITE, LuaTab::OnThreadInvokeWrite)
END_EVENT_TABLE()

wxString GetMsStoreLuaDir()
{
    wxString dir = wxtMsAppLocalDataPath("LocalState");
#if 0
    dir += wxString(wxFileName::GetPathSeparator()) + wxString("scripts");
    dir += wxString(wxFileName::GetPathSeparator()) + wxString("lua");
#endif
    wxtInfo() << "GetMsStoreLuaDir: " << dir;
    if (!wxDirExists(dir)) {
#if defined(WIN32)
        wxMkDir(dir);
#else
        wxMkDir(dir, 777);
#endif
    }

    return dir;
}

wxString GetAppLuaDir()
{
    wxString appDir = wxStandardPaths::Get().GetExecutablePath();
    wxFileName appFileName(appDir);
    wxString appPath = appFileName.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
#if defined(__APPLE__)
    appPath += wxString("..");
    appPath += wxFileName::GetPathSeparator();
    appPath += wxString("Resources");
    appPath += wxFileName::GetPathSeparator();
#endif
    wxString luaDir = appPath + wxString("scripts");
    luaDir += wxFileName::GetPathSeparator();
    luaDir += wxString("lua");
    if (!wxDirExists(luaDir)) {
        wxMkdir(luaDir, wxS_DIR_DEFAULT);
    }

    return luaDir;
}

LuaTab::LuaTab(wxWindow *parent)
    : wxPanel(parent, wxID_ANY)
    , m_luaRunner(nullptr)
    , m_controllerBoxSizer(nullptr)
    , m_mainSizer(nullptr)
    , m_luaTextCtrl(nullptr)
    , m_logTextCtrl(nullptr)
    , m_fileComboBox(nullptr)
    , m_newButton(nullptr)
    , m_openDirButton(nullptr)
    , m_refreshButton(nullptr)
    , m_helpButton(nullptr)
{
    m_controllerBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    m_mainSizer = new wxGridBagSizer(0, 0);

    DoSetupUi();
    DoLoadLuaFileList();
    OnLuaFileComboBoxSelected();

    SetSizer(m_mainSizer);
    Layout();
}

LuaTab::~LuaTab() {}

wxtJson LuaTab::DoSave() const
{
    wxtJson json;
    LuaTabParameterKeys keys;
    json[keys.script] = m_fileComboBox->GetValue().ToStdWstring();
    return json;
}

void LuaTab::DoLoad(const wxtJson &parameters)
{
    LuaTabParameterKeys keys;
    wxString script = wxtGetJsonObjValue<std::wstring>(parameters, keys.script, std::wstring(L""));
    int index = m_fileComboBox->FindString(script);
    if (index == wxNOT_FOUND) {
        return;
    }

    m_fileComboBox->SetSelection(index);
    OnLuaFileComboBoxSelected();
}

void LuaTab::OnBytesRead(std::shared_ptr<char> data, int size)
{
    if (data && size > 0 && m_luaRunner) {
        m_luaRunner->OnBytesRead(data, size);
    }
}

void LuaTab::DoSetupUi()
{
    DoSetupUiControllers();
    DoSetupUiTextCtrl();
}

void LuaTab::DoSetupUiControllers()
{
    m_fileComboBox = new wxComboBox(this,
                                    wxID_ANY,
                                    wxEmptyString,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    0,
                                    nullptr,
                                    wxCB_READONLY);
    // clang-format off
    m_controllerBoxSizer->Add(m_fileComboBox, 1, wxEXPAND | wxALL, 4);
    m_runButton = new wxBitmapButton(this, wxID_ANY, wxNullBitmap);
    m_runButton->Bind(wxEVT_BUTTON, &LuaTab::OnRunButtonClicked, this);
    m_fileComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &) { OnLuaFileComboBoxSelected(); });
    DoUpdateRunButtonState();
    // clang-format on

    auto newIcon = wxArtProvider::GetBitmapBundle(wxART_PLUS, wxART_BUTTON);
    m_newButton = new wxBitmapButton(this, wxID_ANY, newIcon);
    m_newButton->SetToolTip(_("New Lua Script"));
    m_newButton->Bind(wxEVT_BUTTON, &LuaTab::OnNewButtonClicked, this);

    auto openDirIcon = wxArtProvider::GetBitmapBundle(wxART_FOLDER_OPEN, wxART_BUTTON);
    m_openDirButton = new wxBitmapButton(this, wxID_ANY, openDirIcon);
    m_openDirButton->SetToolTip(_("Open Lua Script Directory"));
    m_openDirButton->Bind(wxEVT_BUTTON, &LuaTab::OnOpenDirButtonClicked, this);

    auto refreshIcon = wxArtProvider::GetBitmapBundle(wxART_REFRESH, wxART_BUTTON);
    m_refreshButton = new wxBitmapButton(this, wxID_ANY, refreshIcon);
    m_refreshButton->SetToolTip(_("Refresh Lua Script List"));
    m_refreshButton->Bind(wxEVT_BUTTON, &LuaTab::OnRefreshButtonClicked, this);

    auto helpIcon = wxArtProvider::GetBitmapBundle(wxART_HELP, wxART_BUTTON);
    m_helpButton = new wxBitmapButton(this, wxID_ANY, helpIcon);
    m_helpButton->SetToolTip(_("Visit Online Manual"));
    m_helpButton->Bind(wxEVT_BUTTON, &LuaTab::OnHelpButtonClicked, this);

    m_controllerBoxSizer->Add(m_runButton, 0, wxEXPAND | wxALL, 4);
    m_controllerBoxSizer->Add(m_newButton, 0, wxEXPAND | wxALL, 4);
    m_controllerBoxSizer->Add(m_openDirButton, 0, wxEXPAND | wxALL, 4);
    m_controllerBoxSizer->Add(m_refreshButton, 0, wxEXPAND | wxALL, 4);
    m_controllerBoxSizer->Add(m_helpButton, 0, wxEXPAND | wxALL, 4);
}

void LuaTab::DoSetupUiTextCtrl()
{
    auto *luaBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Lua Script"));
    m_luaTextCtrl = new wxStyledTextCtrl(luaBoxSizer->GetStaticBox(),
                                         wxID_ANY,
                                         wxDefaultPosition,
                                         wxDefaultSize,
                                         wxTE_MULTILINE | wxTE_DONTWRAP);
    DoSetupStyledTextCtrl(m_luaTextCtrl);
    luaBoxSizer->Add(m_luaTextCtrl, 1, wxEXPAND | wxALL, 0);
    m_luaTextCtrl->Bind(wxEVT_TEXT, &LuaTab::OnLuaTextCtrlChanged, this);

    auto *logBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Log"));
    m_logTextCtrl = new wxTextCtrl(logBoxSizer->GetStaticBox(),
                                   wxID_ANY,
                                   wxEmptyString,
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_READONLY);
    logBoxSizer->Add(m_logTextCtrl, 1, wxEXPAND | wxALL, 0);

    m_mainSizer->Add(m_controllerBoxSizer, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    m_mainSizer->Add(luaBoxSizer, wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 4);
    m_mainSizer->Add(logBoxSizer, wxGBPosition(2, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 4);
    m_mainSizer->AddGrowableCol(0);
    m_mainSizer->AddGrowableRow(1);
    m_mainSizer->SetItemMinSize(logBoxSizer, -1, 200);
}

void LuaTab::DoUpdateRunButtonState()
{
    if (m_luaRunner) {
        const wxString stopIconPath = GetIconPath("Stop.svg");
        auto stopIcon = wxBitmapBundle::FromSVGFile(stopIconPath, wxSize(16, 16));
        m_runButton->SetBitmap(stopIcon);
        m_runButton->SetToolTip(_("Stop Running Lua Script"));
    } else {
        const wxString runIconPath = GetIconPath("Play.svg");
        auto runIcon = wxBitmapBundle::FromSVGFile(runIconPath, wxSize(16, 16));
        m_runButton->SetBitmap(runIcon);
        m_runButton->SetToolTip(_("Start Running Lua Script"));
    }
}

void LuaTab::DoLoadLuaFileList()
{
    int count = m_fileComboBox->GetCount();
    for (int i = count - 1; i >= 0; --i) {
        void *ret = m_fileComboBox->GetClientData(i);
        if (ret) {
            wxString *clientData = static_cast<wxString *>(ret);
            delete clientData;
        }

        m_fileComboBox->Delete(i);
    }

    m_fileComboBox->Clear();
    if (m_fileComboBox->GetCount() > 0) {
        m_fileComboBox->SetSelection(0);
    }

    DoLoadLuaFileListApp();
#if defined(__WINDOWS__) && !defined(WXT_PORTABLE_EDITION)
    DoLoadLuaFileListStore();
#endif
    if (m_fileComboBox->GetCount() > 0) {
        m_fileComboBox->SetSelection(0);
    }
}

void LuaTab::DoLoadLuaFileListApp()
{
    wxString luaDir = GetAppLuaDir();
    if (!wxDirExists(luaDir)) {
        return;
    }

    DoLoadLuaFileList(luaDir);
}

void LuaTab::DoLoadLuaFileListStore()
{
    wxString luaDir = GetMsStoreLuaDir();
    if (!wxDirExists(luaDir)) {
        return;
    }

    DoLoadLuaFileList(luaDir);
}

void LuaTab::DoLoadLuaFileList(const wxString &luaDir)
{
    wxArrayString fileArray;
    wxString wildcard = "*.lua";
    wxDir::GetAllFiles(luaDir, &fileArray, wildcard, wxDIR_FILES);

    for (const auto &filePath : fileArray) {
        DoAddLuaFileToList(filePath);
    }
}

void LuaTab::DoAddLuaFileToList(const wxString &filePath)
{
    wxFileName fileName(filePath);
    wxString baseName = fileName.GetName();
    if (m_fileComboBox->FindString(baseName) == wxNOT_FOUND) {
        m_fileComboBox->Append(baseName, new wxString(filePath));
    }
}

void LuaTab::DoOpenLuaRunner()
{
    int section = m_fileComboBox->GetSelection();
    void *ret = m_fileComboBox->GetClientData(section);
    if (ret == nullptr) {
        wxMessageBox(_("Please select a Lua script file first."), wxtErrorStr, wxOK | wxICON_ERROR);
        return;
    }

    wxString *clientData = static_cast<wxString *>(ret);
    if (clientData == nullptr) {
        wxMessageBox(_("Please select a Lua script file first."), wxtErrorStr, wxOK | wxICON_ERROR);
        return;
    }

    wxString fileName = *clientData;
    if (!wxFileExists(fileName)) {
        wxMessageBox(_("The selected Lua script file does not exist."),
                     wxtErrorStr,
                     wxOK | wxICON_ERROR);
        return;
    }

    m_logTextCtrl->Clear();
    DoUpdateUiEnabledState(false);
    m_luaRunner = new LuaRunner(fileName, this);
    m_luaRunner->Run();
}

void LuaTab::DoCloseLuaRunner()
{
    if (m_luaRunner == nullptr) {
        return;
    }

    m_luaRunner->CloseLuaState();
    m_luaRunner = nullptr;
    DoUpdateUiEnabledState(false);
}

void LuaTab::DoUpdateUiEnabledState(bool enabled)
{
    m_fileComboBox->Enable(enabled);
    m_newButton->Enable(enabled);
    m_refreshButton->Enable(enabled);
    m_luaTextCtrl->Enable(enabled);
}

void LuaTab::OnRunButtonClicked(wxCommandEvent &event)
{
    if (m_luaRunner) {
        DoCloseLuaRunner();
    } else {
        DoOpenLuaRunner();
    }

    DoUpdateRunButtonState();
}

void LuaTab::OnLuaFileComboBoxSelected()
{
    int selection = m_fileComboBox->GetSelection();
    if (selection == wxNOT_FOUND) {
        wxtWarning() << "No file is selected in the combo box.";
        return;
    }

    void *ret = m_fileComboBox->GetClientData(selection);
    if (ret == nullptr) {
        wxtWarning() << "Client data for the selected item is null.";
        return;
    }

    wxString *clientData = static_cast<wxString *>(ret);
    if (clientData == nullptr) {
        wxtWarning() << "Failed to cast client data to wxString.";
        return;
    }

    wxString filePath = *clientData;
    if (!wxFileExists(filePath)) {
        wxtWarning() << "The selected Lua script file does not exist: " << filePath;
        return;
    }

    wxFile file(filePath);
    if (!file.IsOpened()) {
        wxtWarning() << "Failed to open the selected Lua script file: " << filePath;
        return;
    }

    wxString fileContent;
    file.ReadAll(&fileContent);
    file.Close();
    m_luaTextCtrl->SetValue(fileContent);
}

void LuaTab::OnLuaTextCtrlChanged(wxCommandEvent &)
{
    wxString fileName = GetCurrentLuaFilePath();
    if (fileName.IsEmpty()) {
        return;
    }

    wxFile file(fileName, wxFile::write);
    if (!file.IsOpened()) {
        wxtWarning() << "Failed to open the Lua script file for writing: " << fileName;
        return;
    }

    wxString fileContent = m_luaTextCtrl->GetValue();
    file.Write(fileContent);
    file.Close();
}

void LuaTab::OnOpenDirButtonClicked(wxCommandEvent &)
{
#if defined(__WINDOWS__) && !defined(WXT_PORTABLE_EDITION)
    wxString dirPath = GetMsStoreLuaDir();
#else
    wxFileName fileName(GetCurrentLuaFilePath());
    wxString dirPath = GetAppLuaDir();
    if (!wxDirExists(dirPath)) {
        return;
    }
#endif
    wxLaunchDefaultApplication(dirPath);
}

void LuaTab::OnHelpButtonClicked(wxCommandEvent &event)
{
    wxString url = "https://x-tools-author.github.io/wx-tools/docs/12.lua/";
    wxLaunchDefaultBrowser(url);
}

void LuaTab::OnNewButtonClicked(wxCommandEvent &event)
{
    wxTextEntryDialog dlg(nullptr,
                          _("Enter the name of the new Lua script file:"),
                          _("New Lua Script"),
                          "LuaScript",
                          wxOK | wxCANCEL);
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }

    wxString newFileName = dlg.GetValue().Trim(true).Trim(false);
#if defined(__WINDOWS__) && !defined(WXT_PORTABLE_EDITION)
    wxString dirPath = GetMsStoreLuaDir();
#else
    wxString dirPath = GetAppLuaDir();
#endif
    wxString newFilePath = dirPath + wxFileName::GetPathSeparator() + newFileName;
    if (!newFileName.EndsWith(".lua")) {
        newFilePath += ".lua";
    }

    if (wxFileExists(newFilePath)) {
        wxMessageBox(_("A file with the same name already exists."),
                     wxtErrorStr,
                     wxOK | wxICON_ERROR);
        return;
    }

    wxFile newFile(newFilePath, wxFile::write);
    if (!newFile.IsOpened()) {
        wxMessageBox(_("Failed to create the new Lua script file."),
                     wxtErrorStr,
                     wxOK | wxICON_ERROR);
        return;
    }

    newFile.Write("-- New Lua script file\n");
    newFile.Close();

    DoAddLuaFileToList(newFilePath);
    int index = m_fileComboBox->FindString(newFileName);
    if (index != wxNOT_FOUND) {
        m_fileComboBox->SetSelection(index);
        OnLuaFileComboBoxSelected();
    }
}

void LuaTab::OnRefreshButtonClicked(wxCommandEvent &event)
{
    DoLoadLuaFileList();
}

void LuaTab::OnThreadFinished(wxThreadEvent &event)
{
    m_luaRunner = nullptr;
    DoUpdateRunButtonState();
    DoUpdateUiEnabledState(true);
}

void LuaTab::OnThreadError(wxThreadEvent &event)
{
    wxString errorMsg = event.GetString();
    wxString dt = GetDateTimeString();
    dt += " ";
    dt += event.GetString();
    dt += "\n";
    m_logTextCtrl->AppendText(dt);
}

void LuaTab::OnThreadInvokeWrite(wxThreadEvent &event)
{
    auto parent = GetParent();
    if (parent == nullptr) {
        return;
    }

    auto evtHandler = parent->GetEventHandler();
    evtHandler->QueueEvent(event.Clone());
}

wxString LuaTab::GetCurrentLuaFilePath()
{
    int section = m_fileComboBox->GetSelection();
    if (section == wxNOT_FOUND) {
        return wxEmptyString;
    }

    void *ret = m_fileComboBox->GetClientData(section);
    if (ret == nullptr) {
        return wxEmptyString;
    }

    wxString *clientData = static_cast<wxString *>(ret);
    if (clientData == nullptr) {
        return wxEmptyString;
    }

    return *clientData;
}

void LuaTab::DoSetupStyledTextCtrl(wxStyledTextCtrl *textCtrl)
{
    textCtrl->SetLexer(wxSTC_LEX_LUA);
    textCtrl->SetKeyWords(0, "function end local if then else for while do return break");
    textCtrl->StyleSetForeground(wxSTC_LUA_DEFAULT, wxColor("#000000"));       // 默认文本颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_COMMENT, wxColor("#737773ff"));     // 注释颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_COMMENTLINE, wxColor("#737773ff")); // 注释颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_COMMENTDOC, wxColor("#737773ff"));  // 注释颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_NUMBER, wxColor("#0000FF"));        // 数字颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_WORD, wxColor("#0000FF"));          // 关键字颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_STRING, wxColor("#A31515"));        // 字符串颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_CHARACTER, wxColor("#A31515"));     // 字符颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_LITERALSTRING, wxColor("#A31515")); // 字符串颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_PREPROCESSOR, wxColor("#FF00FF"));  // 预处理器颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_OPERATOR, wxColor("#000000"));      // 操作符颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_IDENTIFIER, wxColor("#000000"));    // 标识符颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_STRINGEOL, wxColor("#FF0000"));     // 字符串未闭合颜色
    textCtrl->StyleSetForeground(wxSTC_LUA_LABEL, wxColor("#A31515"));         // 标签颜色
    textCtrl->SetMarginType(0, wxSTC_MARGIN_NUMBER); // 设置行号栏类型为数字
    textCtrl->SetMarginWidth(0, 40);                 // 设置行号栏宽度
    textCtrl->SetTabWidth(4);                        // 设置Tab宽度为4个空格
    textCtrl->SetUseTabs(true);                      // 使用空格代替Tab
    textCtrl->SetWrapMode(wxSTC_WRAP_NONE);          // 自动换行

    // 自动提示
    textCtrl->AutoCompSetIgnoreCase(true);   // 自动提示忽略大小写
    textCtrl->AutoCompSetSeparator(' ');     // 自动提示分隔符
    textCtrl->AutoCompSetAutoHide(true);     // 自动提示自动隐藏
    textCtrl->AutoCompSetChooseSingle(true); // 自动提示单一选择时自动完成
}