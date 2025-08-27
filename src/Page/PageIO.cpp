/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PageIO.h"

#include "PageIOInput.h"
#include "PageIOOutput.h"

PageIO::PageIO(wxWindow *parent)
    : wxPanel(parent, wxID_ANY)
    , m_input(nullptr)
    , m_output(nullptr)
{
    auto* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizerAndFit(sizer);

    m_output = new PageIOOutput(this);
    sizer->Add(m_output, 1, wxEXPAND | wxALL, 0);

    m_input = new PageIOInput(this);
    sizer->Add(m_input, 0, wxEXPAND | wxALL, 0);
}

PageIOInput *PageIO::GetInput() const
{
    return m_input;
}

PageIOOutput *PageIO::GetOutput() const
{
    return m_output;
}

wxtJson PageIO::DoSave() const
{
    return wxtJson::object();
}

void PageIO::DoLoad(const wxtJson &parameters)
{
    wxUnusedVar(parameters);
}
