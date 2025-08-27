/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/wx.h>

#include "Common/wxTools.h"

struct PageSettingsParameterKeys
{
    std::string linkSettings = {"cycleInterval"};
    std::string outputSettings = {"outputSettings"};
    std::string inputSettings = {"inputSettings"};
};

class PageSettingsLink;
class PageSettingsOutput;
class PageSettingsInput;
class PageSettings : public wxBoxSizer
{
public:
    PageSettings(LinkType type, wxWindow *parent);

    void DoLoad(const wxtJson &parameters);
    wxtJson DoSave() const;

    PageSettingsLink *GetLinkSettings() const;
    PageSettingsOutput *GetOutputSettings() const;
    PageSettingsInput *GetInputSettings() const;

private:
    PageSettingsLink *m_linkSettings;
    PageSettingsOutput *m_outputSettings;
    PageSettingsInput *m_inputSettings;
};
