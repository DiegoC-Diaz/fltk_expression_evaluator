## To run this porject:
The following libraries from FLTK are required, if you are using visual studio 2020. 
[https://courses.cs.washington.edu/courses/cse557/14au/tools/fltk_install.html]
2. Configure MSVC (Microsoft Visual C++)

Open MSVC, and click Tools->Options.
In the left pane, expland "Projects and Solutions"
Choose "VC++ Directories"
In the right pane, change the drop-down to "Include Files"
Add the item "\"
Add the item "\\png"
Add the item "\\jpeg"
Add the item "\\zlib"
Change the drop-down to "Library Files"
Add the item "\\lib"
3. Verify that your project is linking with the correct library

In MSVC, click Project->Settings...
Click the "C++" tab.
Change the "Category" drop-down to "Code Generation".
If the project configuration is Release, change the "Use run-time library" drop down to "Multithreaded DLL".
If the project configuration is Debug, change the "Use run-time library" drop down to "Debug Multithreaded DLL".
Click the "Link" tab.
If the project configuration is Release, make sure that "fltk.lib" (and not "fltkd.lib") is present in edit box labeled "Object/Library modules".
If the project configuration is Debug, make sure that "fltkd.lib" (and not "fltk.lib") is present in edit box labeled "Object/Library modules".
That's it!
