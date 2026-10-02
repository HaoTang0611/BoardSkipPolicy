# Microsoft Developer Studio Project File - Name="JET8000" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=JET8000 - Win32 Debug_Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JET8000.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JET8000.mak" CFG="JET8000 - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JET8000 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "JET8000 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "JET8000 - Win32 Release_Unicode" (based on "Win32 (x86) Application")
!MESSAGE "JET8000 - Win32 Debug_Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JET8000 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x32\Release"
# PROP Intermediate_Dir "x32\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "AOI_Kernel" /I "Camera_Device" /I "Motion_Device" /I "Barcode_Device" /I "MiM" /I "eVision" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\JET8000_Library\Runtime_x32\JET8000.exe"

!ELSEIF  "$(CFG)" == "JET8000 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x32\Debug"
# PROP Intermediate_Dir "x32\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "AOI_Kernel" /I "Camera_Device" /I "Motion_Device" /I "Barcode_Device" /I "MiM" /I "eVision" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\JET8000_Library\Runtime_x32\JET8000(d).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "JET8000 - Win32 Release_Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "JET8000___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "JET8000___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x32\Release"
# PROP Intermediate_Dir "x32\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "AOI_Kernel" /I "Camera_Device" /I "Motion_Device" /I "Barcode_Device" /I "MiM" /I "eVision" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG"
# ADD RSC /l 0x404 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\JET8000_Library\Runtime_x86\JET8000.exe"
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"..\JET8000_Library\Runtime_x32\JET8000.exe"

!ELSEIF  "$(CFG)" == "JET8000 - Win32 Debug_Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "JET8000___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "JET8000___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x32\Debug"
# PROP Intermediate_Dir "x32\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "AOI_Kernel" /I "Camera_Device" /I "Motion_Device" /I "Barcode_Device" /I "MiM" /I "eVision" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\JET8000_Library\Runtime_x86D\JET8000.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\JET8000_Library\Runtime_x32\JET8000(d).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "JET8000 - Win32 Release"
# Name "JET8000 - Win32 Debug"
# Name "JET8000 - Win32 Release_Unicode"
# Name "JET8000 - Win32 Debug_Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3DUnWrapping.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgBarcodeRecognizeWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgBinaryParam.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgBlobCountWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgCharVerifyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgImageCompareWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgImageEdgeEnhanceWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgImageSourceWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AlgParam.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgPatternEditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgPatternListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgPatternTextWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgSolderWettingWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AOIDataCollect.cpp
# End Source File
# Begin Source File

SOURCE=.\AOIDataCollect_OnlineProc.cpp
# End Source File
# Begin Source File

SOURCE=.\AOIDataCollect_Param.cpp
# End Source File
# Begin Source File

SOURCE=.\AOIDataCollect_Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\AOIDataDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\AOIFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\AOISystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrayPasteWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BarcodeConfirmWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BarcodeDeviceWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BarcodeGeneralWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BarcodeInputWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BarcodeListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BlobAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\BoardConfigWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BoardListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CalibrationWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CaliPaneAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\CaliPaneDynamic.cpp
# End Source File
# Begin Source File

SOURCE=.\CaliPaneStage.cpp
# End Source File
# Begin Source File

SOURCE=.\CaliPaneTargetSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\CaliWndDynamicTune.cpp
# End Source File
# Begin Source File

SOURCE=.\CameraCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\ColorGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorGroupSet.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\ColorRGBV.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorRGBVWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentAgentListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentConfigWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentDefectAlarmListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentDefectAlarmWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CudaCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DefectListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DIB.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw3DWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DtkBarcode.cpp
# End Source File
# Begin Source File

SOURCE=.\EditBarcodeView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditFdView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImageBinaryWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImageBlobWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImageColorFilterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImagePatternWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImageProcessPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImageView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImageView3DPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLibraryWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditMainView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditModelListPaneBar.cpp
# End Source File
# Begin Source File

SOURCE=.\EditModelView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditPartNumberListPaneBar.cpp
# End Source File
# Begin Source File

SOURCE=.\EditResultPaneBar.cpp
# End Source File
# Begin Source File

SOURCE=.\EditWndView.cpp
# End Source File
# Begin Source File

SOURCE=.\FdConfirmWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FdListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FdSortWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FilenameSyntaxWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageCombineWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageConfigWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDebugWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageMaskWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImagePhaseAllWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImagePhaseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBoxWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\InputComboxWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDateTimeWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\InputListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ITSCommWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\JET8000.cpp
# End Source File
# Begin Source File

SOURCE=.\JET8000.rc
# End Source File
# Begin Source File

SOURCE=.\JetAPIUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\JetBlob.cpp
# End Source File
# Begin Source File

SOURCE=.\JetFieldDivider.cpp
# End Source File
# Begin Source File

SOURCE=.\JetImage.cpp
# End Source File
# Begin Source File

SOURCE=.\JETListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\JetMatch.cpp
# End Source File
# Begin Source File

SOURCE=.\JETMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\JetMemDC.cpp
# End Source File
# Begin Source File

SOURCE=.\JetMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\JetSerial.cpp
# End Source File
# Begin Source File

SOURCE=.\JETTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\JetUSBCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\JetZip.cpp
# End Source File
# Begin Source File

SOURCE=.\Jpegfile.cpp
# End Source File
# Begin Source File

SOURCE=.\KNNClassify.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiDLP.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiDLP_v3.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiDLP_v4.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiDLPWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiFrmw.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiFrmw_v3.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiFrmw_v4.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiUsb.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiUsb_v3.cpp
# End Source File
# Begin Source File

SOURCE=.\Light3DTiUsb_v4.cpp
# End Source File
# Begin Source File

SOURCE=.\LightCtrlBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\LightCtrlBoardWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBomWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadCadxyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LogManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LogNode.cpp
# End Source File
# Begin Source File

SOURCE=.\LogOperViewerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MachineStatusWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MapCoordinate.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageBoxWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelAddAllWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGroupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelPropertyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelUpdateToGroupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionParamPane.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionStatusPane.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPanelWizardWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectFiducialPane.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectIntrodPane.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectLoadCadxyPane.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectLoadLibraryPane.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectPaneDivideDistrict.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectPanelAlignPane.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectRegionImagePane.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectWizardWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineFormView_Dual.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGLWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PageDialogbarPartNumber.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamUni.cpp
# End Source File
# Begin Source File

SOURCE=.\PatternParam.cpp
# End Source File
# Begin Source File

SOURCE=.\Plc_Basic.cpp
# End Source File
# Begin Source File

SOURCE=.\Plc_Vigor.cpp
# End Source File
# Begin Source File

SOURCE=.\PlcCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PlcLaneAdjustPane.cpp
# End Source File
# Begin Source File

SOURCE=.\PlcLaneStatusPane.cpp
# End Source File
# Begin Source File

SOURCE=.\PlcNodeListPane.cpp
# End Source File
# Begin Source File

SOURCE=.\PlcParamListPane.cpp
# End Source File
# Begin Source File

SOURCE=.\PlcTowerLightPane.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectCodeListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectColorWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectCompareWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectDivideDistrictWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectFieldConfigWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectGroupConfigWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectLibraryWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectMapMaskWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectMapPane.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectMapWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectMarkWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectParamBasicPane.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneBarcode.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneRepair.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneSave.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneSpecTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneVersionCode.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectParamWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectRegionMapWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteParamWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RulerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketClientWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SortObj.cpp
# End Source File
# Begin Source File

SOURCE=.\SpaceBaseParamWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SpaceNoiseFilterParamWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SystemAdvancePane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemAIPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemAuthorizationPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemBasicPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemColorPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemConfigWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemConvertWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemDebugPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemDefaultPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemFolderPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemInitialWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemLogPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemM2MPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemMESPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOfflinePane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOnlinePane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemParamPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemThreadPane.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlDef.cpp
# End Source File
# Begin Source File

SOURCE=.\UserLevelWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UserRegisterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\WndDefectItemWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3DUnWrapping.h
# End Source File
# Begin Source File

SOURCE=.\AlgBarcodeRecognizeWnd.h
# End Source File
# Begin Source File

SOURCE=.\AlgBinaryParam.h
# End Source File
# Begin Source File

SOURCE=.\AlgBlobCountWnd.h
# End Source File
# Begin Source File

SOURCE=.\AlgCharVerifyWnd.h
# End Source File
# Begin Source File

SOURCE=.\AlgImageCompareWnd.h
# End Source File
# Begin Source File

SOURCE=.\AlgImageEdgeEnhanceWnd.h
# End Source File
# Begin Source File

SOURCE=.\AlgImageSourceWnd.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AlgParam.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AlgParamDef.h
# End Source File
# Begin Source File

SOURCE=.\AlgPatternEditWnd.h
# End Source File
# Begin Source File

SOURCE=.\AlgPatternListWnd.h
# End Source File
# Begin Source File

SOURCE=.\AlgPatternTextWnd.h
# End Source File
# Begin Source File

SOURCE=.\AlgSolderWettingWnd.h
# End Source File
# Begin Source File

SOURCE=.\AOIDataCollect.h
# End Source File
# Begin Source File

SOURCE=.\AOIDataDefine.h
# End Source File
# Begin Source File

SOURCE=.\AOIFileIO.h
# End Source File
# Begin Source File

SOURCE=.\AOIFileIODef.h
# End Source File
# Begin Source File

SOURCE=.\AOIFileSpcDef.h
# End Source File
# Begin Source File

SOURCE=.\AOISystem.h
# End Source File
# Begin Source File

SOURCE=.\ArrayPasteWnd.h
# End Source File
# Begin Source File

SOURCE=.\BarcodeConfirmWnd.h
# End Source File
# Begin Source File

SOURCE=.\BarcodeDeviceWnd.h
# End Source File
# Begin Source File

SOURCE=.\BarcodeGeneralWnd.h
# End Source File
# Begin Source File

SOURCE=.\BarcodeInputWnd.h
# End Source File
# Begin Source File

SOURCE=.\BarcodeListWnd.h
# End Source File
# Begin Source File

SOURCE=.\BlobAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\BoardConfigWnd.h
# End Source File
# Begin Source File

SOURCE=.\BoardListWnd.h
# End Source File
# Begin Source File

SOURCE=.\CalibrationDef.h
# End Source File
# Begin Source File

SOURCE=.\CalibrationWnd.h
# End Source File
# Begin Source File

SOURCE=.\CaliPaneAlign.h
# End Source File
# Begin Source File

SOURCE=.\CaliPaneDynamic.h
# End Source File
# Begin Source File

SOURCE=.\CaliPaneStage.h
# End Source File
# Begin Source File

SOURCE=.\CaliPaneTargetSetting.h
# End Source File
# Begin Source File

SOURCE=.\CaliWndDynamicTune.h
# End Source File
# Begin Source File

SOURCE=.\CameraCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\ColorGroup.h
# End Source File
# Begin Source File

SOURCE=.\ColorGroupSet.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\ColorRGBV.h
# End Source File
# Begin Source File

SOURCE=.\ColorRGBVWnd.h
# End Source File
# Begin Source File

SOURCE=.\ComponentAgentListWnd.h
# End Source File
# Begin Source File

SOURCE=.\ComponentConfigWnd.h
# End Source File
# Begin Source File

SOURCE=.\ComponentDefectAlarmListWnd.h
# End Source File
# Begin Source File

SOURCE=.\ComponentDefectAlarmWnd.h
# End Source File
# Begin Source File

SOURCE=.\ComponentListWnd.h
# End Source File
# Begin Source File

SOURCE=.\CudaCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\DebugFormView.h
# End Source File
# Begin Source File

SOURCE=.\DebugWnd.h
# End Source File
# Begin Source File

SOURCE=.\DefectListWnd.h
# End Source File
# Begin Source File

SOURCE=.\DialogBase.h
# End Source File
# Begin Source File

SOURCE=.\DIB.h
# End Source File
# Begin Source File

SOURCE=.\Draw3DWnd.h
# End Source File
# Begin Source File

SOURCE=.\DtkBarcode.h
# End Source File
# Begin Source File

SOURCE=.\EditBarcodeView.h
# End Source File
# Begin Source File

SOURCE=.\EditFdView.h
# End Source File
# Begin Source File

SOURCE=.\EditFormView.h
# End Source File
# Begin Source File

SOURCE=.\EditImageBinaryWnd.h
# End Source File
# Begin Source File

SOURCE=.\EditImageBlobWnd.h
# End Source File
# Begin Source File

SOURCE=.\EditImageColorFilterWnd.h
# End Source File
# Begin Source File

SOURCE=.\EditImagePatternWnd.h
# End Source File
# Begin Source File

SOURCE=.\EditImageProcessPage.h
# End Source File
# Begin Source File

SOURCE=.\EditImageView.h
# End Source File
# Begin Source File

SOURCE=.\EditImageView3DPage.h
# End Source File
# Begin Source File

SOURCE=.\EditLibraryWnd.h
# End Source File
# Begin Source File

SOURCE=.\EditMainView.h
# End Source File
# Begin Source File

SOURCE=.\EditModelListPaneBar.h
# End Source File
# Begin Source File

SOURCE=.\EditModelView.h
# End Source File
# Begin Source File

SOURCE=.\EditPartNumberListPaneBar.h
# End Source File
# Begin Source File

SOURCE=.\EditResultPaneBar.h
# End Source File
# Begin Source File

SOURCE=.\EditWndView.h
# End Source File
# Begin Source File

SOURCE=.\FdConfirmWnd.h
# End Source File
# Begin Source File

SOURCE=.\FdListWnd.h
# End Source File
# Begin Source File

SOURCE=.\FdSortWnd.h
# End Source File
# Begin Source File

SOURCE=.\FilenameSyntaxWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImageAPI.h
# End Source File
# Begin Source File

SOURCE=.\ImageCombineWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImageConfigWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImageDebugWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImageMaskWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImagePhaseAllWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImagePhaseWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImageWnd.h
# End Source File
# Begin Source File

SOURCE=.\InputBoxWnd.h
# End Source File
# Begin Source File

SOURCE=.\InputComboxWnd.h
# End Source File
# Begin Source File

SOURCE=.\InputDateTimeWnd.h
# End Source File
# Begin Source File

SOURCE=.\InputListWnd.h
# End Source File
# Begin Source File

SOURCE=.\ITSCommWnd.h
# End Source File
# Begin Source File

SOURCE=.\JET8000.h
# End Source File
# Begin Source File

SOURCE=.\JET8000MSGDef.h
# End Source File
# Begin Source File

SOURCE=.\JetAPIUtility.h
# End Source File
# Begin Source File

SOURCE=.\JetBlob.h
# End Source File
# Begin Source File

SOURCE=.\JetFieldDivider.h
# End Source File
# Begin Source File

SOURCE=.\JetImage.h
# End Source File
# Begin Source File

SOURCE=.\JETListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\JetLoadDll.h
# End Source File
# Begin Source File

SOURCE=.\JetMatch.h
# End Source File
# Begin Source File

SOURCE=.\JETMatrix.h
# End Source File
# Begin Source File

SOURCE=.\JetMemDC.h
# End Source File
# Begin Source File

SOURCE=.\JetMemory.h
# End Source File
# Begin Source File

SOURCE=.\JetSerial.h
# End Source File
# Begin Source File

SOURCE=.\JETTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\JetUSBCtrl.h
# End Source File
# Begin Source File

SOURCE=.\JetZip.h
# End Source File
# Begin Source File

SOURCE=.\Jpegfile.h
# End Source File
# Begin Source File

SOURCE=.\KNNClassify.h
# End Source File
# Begin Source File

SOURCE=.\Light3DCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Light3DDef.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiDLP.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiDLP_v3.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiDLP_v4.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiDLPWnd.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiFrmw.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiFrmw_v3.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiFrmw_v4.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiUsb.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiUsb_v3.h
# End Source File
# Begin Source File

SOURCE=.\Light3DTiUsb_v4.h
# End Source File
# Begin Source File

SOURCE=.\LightCtrlBoard.h
# End Source File
# Begin Source File

SOURCE=.\LightCtrlBoardWnd.h
# End Source File
# Begin Source File

SOURCE=.\LoadBomWnd.h
# End Source File
# Begin Source File

SOURCE=.\LoadCadxyWnd.h
# End Source File
# Begin Source File

SOURCE=.\LogManager.h
# End Source File
# Begin Source File

SOURCE=.\LogNode.h
# End Source File
# Begin Source File

SOURCE=.\LogOperViewerWnd.h
# End Source File
# Begin Source File

SOURCE=.\MachineStatusWnd.h
# End Source File
# Begin Source File

SOURCE=.\MainDoc.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MapCoordinate.h
# End Source File
# Begin Source File

SOURCE=.\MessageBoxWnd.h
# End Source File
# Begin Source File

SOURCE=.\ModelAddAllWnd.h
# End Source File
# Begin Source File

SOURCE=.\ModelGroupWnd.h
# End Source File
# Begin Source File

SOURCE=.\ModelPropertyWnd.h
# End Source File
# Begin Source File

SOURCE=.\ModelUpdateToGroupWnd.h
# End Source File
# Begin Source File

SOURCE=.\ModelWnd.h
# End Source File
# Begin Source File

SOURCE=.\MotionCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\MotionParamPane.h
# End Source File
# Begin Source File

SOURCE=.\MotionStatusPane.h
# End Source File
# Begin Source File

SOURCE=.\MultiLanguageDef.h
# End Source File
# Begin Source File

SOURCE=.\NewPanelWizardWnd.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectFiducialPane.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectIntrodPane.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectLoadCadxyPane.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectLoadLibraryPane.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectPaneDivideDistrict.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectPanelAlignPane.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectRegionImagePane.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectWizardWnd.h
# End Source File
# Begin Source File

SOURCE=.\OnlineFormView.h
# End Source File
# Begin Source File

SOURCE=.\OnlineFormView_Dual.h
# End Source File
# Begin Source File

SOURCE=.\OpenGLWnd.h
# End Source File
# Begin Source File

SOURCE=.\PageDialogbarPartNumber.h
# End Source File
# Begin Source File

SOURCE=.\PageSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\PanelListWnd.h
# End Source File
# Begin Source File

SOURCE=.\ParamUni.h
# End Source File
# Begin Source File

SOURCE=.\PatternParam.h
# End Source File
# Begin Source File

SOURCE=.\Plc_Basic.h
# End Source File
# Begin Source File

SOURCE=.\Plc_Define.h
# End Source File
# Begin Source File

SOURCE=.\Plc_Vigor.h
# End Source File
# Begin Source File

SOURCE=.\PlcCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\PlcLaneAdjustPane.h
# End Source File
# Begin Source File

SOURCE=.\PlcLaneStatusPane.h
# End Source File
# Begin Source File

SOURCE=.\PlcNodeListPane.h
# End Source File
# Begin Source File

SOURCE=.\PlcParamListPane.h
# End Source File
# Begin Source File

SOURCE=.\PlcTowerLightPane.h
# End Source File
# Begin Source File

SOURCE=.\ProjectCodeListWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectColorWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectCompareWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectDivideDistrictWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectFieldConfigWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectGroupConfigWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectLibraryWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectListWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectMapMaskWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectMapPane.h
# End Source File
# Begin Source File

SOURCE=.\ProjectMapWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectMarkWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParamBasicPane.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParameterDef.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneAlarm.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneBarcode.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneRepair.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneSave.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneSpecTest.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParamPaneVersionCode.h
# End Source File
# Begin Source File

SOURCE=.\ProjectParamWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProjectRegionMapWnd.h
# End Source File
# Begin Source File

SOURCE=.\RemoteParamWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RulerWnd.h
# End Source File
# Begin Source File

SOURCE=.\SocketClientWnd.h
# End Source File
# Begin Source File

SOURCE=.\SortObj.h
# End Source File
# Begin Source File

SOURCE=.\SpaceBaseParamWnd.h
# End Source File
# Begin Source File

SOURCE=.\SpaceNoiseFilterParamWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SystemAdvancePane.h
# End Source File
# Begin Source File

SOURCE=.\SystemAIPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemAuthorizationPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemBasicPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemColorPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemConfigWnd.h
# End Source File
# Begin Source File

SOURCE=.\SystemConvertWnd.h
# End Source File
# Begin Source File

SOURCE=.\SystemDebugPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemDefaultPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemFolderPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemInitialWnd.h
# End Source File
# Begin Source File

SOURCE=.\SystemLogPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemM2MPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemMESPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemOfflinePane.h
# End Source File
# Begin Source File

SOURCE=.\SystemOnlinePane.h
# End Source File
# Begin Source File

SOURCE=.\SystemParameterDef.h
# End Source File
# Begin Source File

SOURCE=.\SystemParamPane.h
# End Source File
# Begin Source File

SOURCE=.\SystemThreadPane.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlComponent.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlDef.h
# End Source File
# Begin Source File

SOURCE=.\UserLevelWnd.h
# End Source File
# Begin Source File

SOURCE=.\UserRegisterWnd.h
# End Source File
# Begin Source File

SOURCE=.\WndAlgPropertyDef.h
# End Source File
# Begin Source File

SOURCE=.\WndDefectItemWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\JET8000.ico
# End Source File
# Begin Source File

SOURCE=.\res\JET8000.rc2
# End Source File
# Begin Source File

SOURCE=.\res\LED_Blue_Large.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Blue_Media.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Gray_Large.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Gray_Median.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Gray_Small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Green_Large.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Green_Median.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Green_Small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Red_Large.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Red_Median.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Red_Small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Yellow_Large.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LED_Yellow_Median.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Main_Offline.BMP
# End Source File
# Begin Source File

SOURCE=.\res\MainDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconBGA_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconBGA_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconCAE_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconCAE_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconChipC_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconChipC_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconChipL_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconChipL_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconChipLED_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconChipLED_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconChipR_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconChipR_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconCN_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconCN_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconCON_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconCON_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconDFN_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconDFN_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconDIP_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconDIP_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconEmpty_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconEmpty_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconGoldenFinger_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconGoldenFinger_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconIC_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconIC_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconLead_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconLead_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconLEDN_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconLEDN_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconMELF_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconMELF_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconNoLead_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconNoLead_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconOthers_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconOthers_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconPadComponent_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconPadComponent_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconPLCC_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconPLCC_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconQFN_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconQFN_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconQFP_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconQFP_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconRN_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconRN_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconSOJ_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconSOJ_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconSOP_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconSOP_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconSOT_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconSOT_Power_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconSOT_Power_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconSOT_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconTANT_M.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ModelIcon\ModelIconTANT_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ProjectTreeIcon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PropertyBtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RibbonGlobalImage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RibbonIconLarge.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RibbonIconSmall.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RibbonSmallImage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_BTLR.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_BTLR_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_BTRL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_BTRL_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_LRBT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_LRBT_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_LRTB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_LRTB_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_RLBT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_RLBT_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_RLTB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_RLTB_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_TBLR.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_TBLR_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_TBRL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SORT_TBRL_S.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "eVision Files"

# PROP Default_Filter ".h;.cpp"
# Begin Source File

SOURCE=.\eVision\EVisionLibDef.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsAPI.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsBarcode1D.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsBarcode1D.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsBarcodeDataMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsBarcodeDataMatrix.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsBarcodeQRCode.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsBarcodeQRCode.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsImageBW8.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsImageBW8.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsImageC24.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsImageC24.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsMatch.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsMatch.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsRoiBW8.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsRoiBW8.h
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsRoiC24.cpp
# End Source File
# Begin Source File

SOURCE=.\eVision\EvsRoiC24.h
# End Source File
# End Group
# Begin Group "iVision Files"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=.\MiM\MimAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\MiM\MimAPI.h
# End Source File
# Begin Source File

SOURCE=.\MiM\MimImageBW8.cpp
# End Source File
# Begin Source File

SOURCE=.\MiM\MimImageBW8.h
# End Source File
# Begin Source File

SOURCE=.\MiM\MimImageC24.cpp
# End Source File
# Begin Source File

SOURCE=.\MiM\MimImageC24.h
# End Source File
# Begin Source File

SOURCE=.\MiM\MimLibDef.h
# End Source File
# Begin Source File

SOURCE=.\MiM\MimMatch.cpp
# End Source File
# Begin Source File

SOURCE=.\MiM\MimMatch.h
# End Source File
# Begin Source File

SOURCE=.\MiM\MimRoiBW8.cpp
# End Source File
# Begin Source File

SOURCE=.\MiM\MimRoiBW8.h
# End Source File
# Begin Source File

SOURCE=.\MiM\MimRoiC24.cpp
# End Source File
# Begin Source File

SOURCE=.\MiM\MimRoiC24.h
# End Source File
# End Group
# Begin Group "Camera Device"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=".\Camera Device\Camera_Basic.cpp"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_Basic.h"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_CoaxLink_Q_12A180F.cpp"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_CoaxLink_Q_12A180F.h"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_Coaxlink_VC_12MX_M180.cpp"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_Coaxlink_VC_12MX_M180.h"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_Define.h"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_DynamicModule.cpp"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_DynamicModule.h"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_GrabLinkFull_CSC6M100BMP11.cpp"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_GrabLinkFull_CSC6M100BMP11.h"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_GrabLinkFull_Q12A65Fm.cpp"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_GrabLinkFull_Q12A65Fm.h"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_USB3_TeliBU1207MCF.cpp"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\Camera_USB3_TeliBU1207MCF.h"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\CameraCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\Camera Device\CameraCtrl.h"
# End Source File
# End Group
# Begin Group "Barcode Device"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Basic.cpp"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Basic.h"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_DataLogic_Matrix210.cpp"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_DataLogic_Matrix210.h"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_DataLogic_Matrix210N.cpp"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_DataLogic_Matrix210N.h"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Define.h"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Device.h"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Device_Module.cpp"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Device_Module.h"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Handheld.cpp"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Handheld.h"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Honeywell_3310GHD.cpp"
# End Source File
# Begin Source File

SOURCE=".\Barcode Device\Barcode_Honeywell_3310GHD.h"
# End Source File
# End Group
# Begin Group "Motion Device"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=".\Motion Device\Motion_Basic.cpp"
# End Source File
# Begin Source File

SOURCE=".\Motion Device\Motion_Basic.h"
# End Source File
# Begin Source File

SOURCE=".\Motion Device\Motion_Define.h"
# End Source File
# Begin Source File

SOURCE=".\Motion Device\Motion_Lib_Module.cpp"
# End Source File
# Begin Source File

SOURCE=".\Motion Device\Motion_Lib_Module.h"
# End Source File
# Begin Source File

SOURCE=".\Motion Device\Motion_PCE_M114GL.cpp"
# End Source File
# Begin Source File

SOURCE=".\Motion Device\Motion_PCE_M114GL.h"
# End Source File
# Begin Source File

SOURCE=".\Motion Device\Motion_PCI_M114GL.cpp"
# End Source File
# Begin Source File

SOURCE=".\Motion Device\Motion_PCI_M114GL.h"
# End Source File
# End Group
# Begin Group "AOIObj Files"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=.\AOI_Kernel\AOIBarcode.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIBarcode.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIBoard.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIBox.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIBox.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIComponent.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIFd.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIFd.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIField.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIField.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIFov.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIFov.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIFovMark.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIFovMark.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIFrame.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOILand.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOILand.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOILogic.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOILogic.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIModel.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIModel.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIModelDef.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIObj.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIObj.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIObjManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIObjManager.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIPanel.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIProject.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIProject.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIRgn.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIRgn.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOISlice.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOISlice.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIWindow.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIWnd.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIWndMask.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIWndMask.h
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIWndRoi.cpp
# End Source File
# Begin Source File

SOURCE=.\AOI_Kernel\AOIWndRoi.h
# End Source File
# End Group
# Begin Group "MES Files"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=.\MES\ITSFileChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\MES\ITSFileChecker.h
# End Source File
# Begin Source File

SOURCE=.\MES\ITSLinker.cpp
# End Source File
# Begin Source File

SOURCE=.\MES\ITSLinker.h
# End Source File
# Begin Source File

SOURCE=.\MES\ITSRabbitMQ.cpp
# End Source File
# Begin Source File

SOURCE=.\MES\ITSRabbitMQ.h
# End Source File
# Begin Source File

SOURCE=.\MES\MES_Basic.cpp
# End Source File
# Begin Source File

SOURCE=.\MES\MES_Basic.h
# End Source File
# Begin Source File

SOURCE=.\MES\MES_Class.h
# End Source File
# Begin Source File

SOURCE=.\MES\MES_ITS.cpp
# End Source File
# Begin Source File

SOURCE=.\MES\MES_ITS.h
# End Source File
# Begin Source File

SOURCE=.\MES\MES_MTS.cpp
# End Source File
# Begin Source File

SOURCE=.\MES\MES_MTS.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\JET8000.reg
# End Source File
# Begin Source File

SOURCE=.\SmartChart\JETSmartChart.exe
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\res\ribbon1.mfcribbon-ms"
# End Source File
# End Target
# End Project
