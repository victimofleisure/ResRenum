# ResRenum
Renumber resource IDs in Visual Studio projects

ResRenum renumbers resource IDs in Visual Studio C++ projects. It's a console app, and it's intended to be integrated into the IDE as an external tool. It's compatible with any version of Visual Studio from MFC 6.0 onwards. It's been tested in 6.0, VS2008, VS2010, and VS2012.

The resources are sorted in alphabetical order within each resource type. The resource types are sorted in compliance with Windows rules. Unicode is fully supported: the app automatically detects if either input file contains Unicode text and does the right thing.

ResRenum takes two arguments: the path to the resource header (usually resource.h), and the path to the resource file (.rc). The second argument is optional, but if it's specified, ResRenum deletes unused identifiers from the resource header. This is usually what you want.

Features:

    Supports Unicode resource files
    Easily integrated into IDE as external tool
    Optionally deletes unused resource identifiers
    Compatible with any version from MFC 6.0 onward
    Simple to use and free, unlike Riverblade ResOrg
    Avoids many problems caused by duplicate resource IDs

