/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Application.h"

#if 0
int main(int argc, char* argv[])
{
    Application* app = new Application();
    wxApp::SetInstance(app);

    return wxEntry(argc, argv);
}
#else
wxIMPLEMENT_APP(Application);
#endif