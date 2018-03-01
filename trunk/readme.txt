ResRenum renumbers resource IDs in Visual Studio C++ projects. It's a console app, and it's intended to be integrated into the IDE as an external tool. It's compatible with any version of Visual Studio from MFC 6.0 onwards. It's been tested in 6.0, VS2008, VS2010, and VS2012.

Here's the recommended external tool setup. Note that C:\foo is a placeholder, for which you should substitute the path to ResRenum.exe on your PC.

Title: Renumber Resources
Command: C:\foo\ResRenum.exe
Arguments: Resource.h $(TargetName).rc
Initial directory: $(ProjectDir)
Use Output Window: Yes
Prompt for arguments: No
Treat output as Unicode: No

ResRenum takes two arguments: the path to the resource header (usually resource.h), and the path to the resource file (.rc). The second argument is optional, but if it's specified, ResRenum deletes unused identifiers from the resource header. This is usually what you want.

If the IDE has the resource file open when you run ResRenum, the IDE prompts you as follows: "This resource has been modified outside of Microsoft Visual Studio. Do you want to reload ... .rc?" You should select "Yes" at this prompt to avoid confusing the IDE.

Note that Visual Studio 2008 tends to crash when the resources are reloaded, alas. Later versions of Visual Studio don't have this problem.
 
The resources are sorted in alphabetical order within each resource type. The resource types are sorted in compliance with Windows rules. Unicode is fully supported: the app automatically detects if either input file contains Unicode text and does the right thing.

The executable depends on the MFC shared DLL. This shouldn't be a problem, because anyone using this tool will have Visual Studio installed, in which case the DLL will be available.

The project is written in C++ and can be built in Visual Studio 2008 or any later version.
 
LICENSE:

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.  This program is distributed in the hope that
it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
the GNU General Public License for more details.  You should have
received a copy of the GNU General Public License along with this
program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111 USA.

END
