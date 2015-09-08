# Microsoft Developer Studio Project File - Name="TrackEye" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TrackEye - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TrackEye.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TrackEye.mak" CFG="TrackEye - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TrackEye - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TrackEye - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TrackEye - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\OpenCV\cv\include" /I "C:\Program Files\OpenCV\otherlibs\cvcam\include" /I "C:\Program Files\OpenCV\otherlibs\highgui" /I "C:\Program Files\OpenCV\cvaux\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41f /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41f /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 cv.lib cvcam.lib highgui.lib cvaux.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Program Files\OpenCV\lib"

!ELSEIF  "$(CFG)" == "TrackEye - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\OpenCV\cv\include" /I "C:\Program Files\OpenCV\otherlibs\cvcam\include" /I "C:\Program Files\OpenCV\otherlibs\highgui" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41f /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41f /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cv.lib cvcam.lib highgui.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\OpenCV\lib"

!ENDIF 

# Begin Target

# Name "TrackEye - Win32 Release"
# Name "TrackEye - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Calibration4Pupil.cpp
# End Source File
# Begin Source File

SOURCE=.\Calibration4Snake.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\DefineEye4Calibration.cpp
# End Source File
# Begin Source File

SOURCE=.\Eye.cpp
# End Source File
# Begin Source File

SOURCE=.\EyeDatabaseExplorer.cpp
# End Source File
# Begin Source File

SOURCE=.\Face.cpp
# End Source File
# Begin Source File

SOURCE=.\FilenameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IconButton.cpp
# End Source File
# Begin Source File

SOURCE=.\PCASettings.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TrackEye.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackEye.rc
# End Source File
# Begin Source File

SOURCE=.\TrackEyeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackingParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackingSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\TrajectoryExtractor2D.cpp
# End Source File
# Begin Source File

SOURCE=.\USBCam.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Calibration4Pupil.h
# End Source File
# Begin Source File

SOURCE=.\Calibration4Snake.h
# End Source File
# Begin Source File

SOURCE=.\ColorSelector.h
# End Source File
# Begin Source File

SOURCE=.\DefineEye4Calibration.h
# End Source File
# Begin Source File

SOURCE=.\Eye.h
# End Source File
# Begin Source File

SOURCE=.\EyeDatabaseExplorer.h
# End Source File
# Begin Source File

SOURCE=.\Face.h
# End Source File
# Begin Source File

SOURCE=.\FilenameDlg.h
# End Source File
# Begin Source File

SOURCE=.\IconButton.h
# End Source File
# Begin Source File

SOURCE=.\PCASettings.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TrackEye.h
# End Source File
# Begin Source File

SOURCE=.\TrackEyeDlg.h
# End Source File
# Begin Source File

SOURCE=.\TrackingParameters.h
# End Source File
# Begin Source File

SOURCE=.\TrackingSettings.h
# End Source File
# Begin Source File

SOURCE=.\TrajectoryExtractor2D.h
# End Source File
# Begin Source File

SOURCE=.\USBCam.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\avi.ico
# End Source File
# Begin Source File

SOURCE=.\res\Control.ico
# End Source File
# Begin Source File

SOURCE=.\res\dx3500.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\mc3.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrackEye.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrackEye.rc2
# End Source File
# Begin Source File

SOURCE=.\res\zs.BMP
# End Source File
# End Group
# Begin Group "Libraries"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Hough.h
# End Source File
# Begin Source File

SOURCE=.\Hough2.h
# End Source File
# Begin Source File

SOURCE=.\TezLibrary_1.h
# End Source File
# Begin Source File

SOURCE=.\Tracking_Library.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
