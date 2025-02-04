﻿/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <nlohmann/json.hpp>
#include <wx/gbsizer.h>
#include <wx/wx.h>

#include "Links/Link.h"

class LinkUi : public wxGridBagSizer
{
public:
    LinkUi(wxWindow *parent = nullptr);
    ~LinkUi();
    Link *GetLink() const;

    bool Open();
    void Close();
    bool IsOpen() const;

    virtual void Disable();
    virtual void Enable();
    virtual wxtJson Save() const;
    virtual void Load(const wxtJson &json);
    virtual void Refresh();

protected:
    virtual Link *NewLink();
    virtual void DeleteLink(Link *link);

private:
    Link *m_link;
};
