
*** Tested under Xcode 16.1, macOS Sonoma 14.7 ***

1.1 Open Game.xcodeproj
1.2 Click the "play button" (compile and run)
1.3 Quit application by pressing Esc on the keyboard

Done! :-)

2. Else, if required, change Game->General->Game->Minimum Deployments->macOS to your present OS version  

3. As an alternative, the project could also be built from scratch by the following steps:

3.1 Open Xcode and select "Create New Project"
3.2 Select "App" and "Next"
3.3 Set product name to "SGE", Interface to "XIB", Language to "Swift", and select "Next"
3.4 Select or create a project folder
3.6 Delete AppDelegate.swift from within Xcode, since it will else create a dupicate entry point to Main
3.7 Copy the files under /Mac - complete OpenGL libs/Game/ to the local Game folder of the new project
3.8 Import the files to the project
3.9 Under SGE->General->TARGETS->SGE->"FrameWorks, Libraries, and Embedded Content", click "+" and add the following two frameworks:
OpenGL.framework
GLUT.framework
3.10 Compile and run SGE

Done! =)
