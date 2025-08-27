/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include <wx/fileconf.h>
#include <wx/wx.h>

//--------------------------------------------------------------------------------------------------
// Common
#define wxtNone -1
#define wxtOK 0
#define wxtUnknown -2

#define wxtNoneStr wxT("None")
#define wxtOkStr wxT("Ok")
#define wxtUnknownStr wxT("Unknown")
#define wxtPathSeparator wxFileName::GetPathSeparator()
#define wxtDataDir wxStandardPaths::Get().GetDataDir()

#ifdef MG_DATA_SIZE
const static int wxtDataSize = MG_DATA_SIZE;
#else
const static int wxtDataSize = 10240;
#endif

#define wxtIgnoreCloseErrorPopup 1
enum wxtID {
    wxtID_LINK_TX = wxID_HIGHEST + 1,
    wxtID_LINK_RX,
    wxtID_LINK_ERROR,
    wxtID_LINK_NEW,
    wxtID_LINK_DELETE,
    wxtID_LINK_OPENED,
    wxtID_LINK_CLOSED,
    wxtID_LINK_RESOLVED
};
const int wxtNewID();

wxDECLARE_EVENT(wxtEVT_SETTINGS_LINK_OPEN, wxCommandEvent);
wxDECLARE_EVENT(wxtEVT_SETTINGS_LINK_POPUP_REFRESH, wxCommandEvent);
wxDECLARE_EVENT(wxtEVT_SETTINGS_OUTPUT_CLEAR, wxCommandEvent);
wxDECLARE_EVENT(wxtEVT_SETTINGS_OUTPUT_WRAP, wxCommandEvent);
wxDECLARE_EVENT(wxtEVT_SETTINGS_INPUT_WRITE, wxCommandEvent);
wxDECLARE_EVENT(wxtEVT_SETTINGS_INPUT_FORMAT, wxCommandEvent);

//--------------------------------------------------------------------------------------------------
// Datastructure
struct wxtDataItem
{
    std::shared_ptr<char> data;
    int len;
    std::string flag;
};

//--------------------------------------------------------------------------------------------------
// Google Log
#define wxtLog(severity) LOG(severity)
#define wxtInfo() LOG(INFO)
#define wxtWarning() LOG(WARNING)
#define wxtError() LOG(ERROR)
std::string LogPath();
void DoInitLogging(const char *argv0);
void DoShutdownLogging();

//--------------------------------------------------------------------------------------------------
// json
typedef nlohmann::json wxtJson;
template<typename T>
T wxtGetJsonObjValue(const wxtJson &json, const std::string &key, T defaultValue)
{
    if (json.contains(key)) {
        return json[key].get<T>();
    }

    return defaultValue;
}

//--------------------------------------------------------------------------------------------------
// libcrc
enum class CRCType {
    CRC_8,
    CRC_8_ITU,
    CRC_8_ROHC,
    CRC_8_MAXIM,
    CRC_16_IBM,
    CRC_16_MAXIM,
    CRC_16_USB,
    CRC_16_MODBUS,
    CRC_16_CCITT,
    CRC_16_CCITT_FALSE,
    CRC_16_x25,
    CRC_16_XMODEM,
    CRC_16_DNP,
    CRC_32,
    CRC_32_MPEG2
};
std::vector<CRCType> GetSuportedCrcTypes();
wxString GetCrcName(CRCType type);
std::vector<char> DoCalculateCRC(std::shared_ptr<char> bytes,
                                 int len,
                                 int algorithm,
                                 int startIndex,
                                 int endIndex,
                                 bool bigEndian);

//--------------------------------------------------------------------------------------------------
// Common interface
std::string GetDateTimeString(const std::string &format = "%Y-%m-%d %H:%M:%S", bool showMs = false);

//--------------------------------------------------------------------------------------------------
// Text format
enum class TextFormat { Bin, Oct, Dec, Hex, Ascii, Utf8, Unknown = -1 };
std::vector<int> GetSuportedFormats();
std::vector<wxString> GetSuportedTextFormats();
wxString GetTextFormatName(TextFormat format);
std::string DoDecodeBytes(const std::shared_ptr<char> &bytes, int &len, int format);
std::shared_ptr<char> DoEncodeBytes(const std::string &text, int &len, int format);
std::string GetBinString(uint8_t value);
std::string GetOctString(uint8_t value);
std::string GetDecString(uint8_t value);
std::string GetHexString(uint8_t value);

//--------------------------------------------------------------------------------------------------
// Link type
enum class LinkType {
    SerialPort,
    UDPClient,
    UDPServer,
    TCPClient,
    TCPServer,
    WSClient,
    WSServer,
    Unknown = -1
};
std::vector<LinkType> GetSuportedLinkTypes();
wxString GetLinkName(LinkType type);
wxString GetLinkRawName(LinkType type);

//--------------------------------------------------------------------------------------------------
// Addition
enum class AdditionType { R, RN, N, NR, None = wxtNone };
std::vector<AdditionType> GetSuportedAdditionTypes();
wxString GetAdditionName(AdditionType type);
std::vector<char> GetAdditionChars(int type);

//--------------------------------------------------------------------------------------------------
// Escape character
enum class EscapeType { R, RN, N, NR, R_N, None = wxtNone };
std::vector<EscapeType> GetSuportedEscapeTypes();
wxString GetEscapeName(EscapeType type);
wxString GetEscapeString(const std::string &txt, int type);

//--------------------------------------------------------------------------------------------------
// wxWidgets
#define wxtConfig GetSettingsConfig()
wxFileConfig *GetSettingsConfig();

void SetComboBoxSectionByIntClientData(wxComboBox *comboBox, int clientDataValue);
wxString GetSettingsPath();
wxString GetSettingsFileName();
std::string DoEncodeFlag(const std::string &ip, uint16_t port);
std::pair<std::string, uint16_t> DoDecodeFlag(const std::string &flag);

template<typename T>
T DoReverseByteOrder(T value)
{
    T result = 0;
    for (size_t i = 0; i < sizeof(value); i++) {
        result = (result << 8) | (value & 0xff);
        value >>= 8;
    }
    return result;
}
