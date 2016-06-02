; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "SRTool"
#define MyAppVersion "1.0.0.0.6"
#define MyAppPublisher "sisec"
#define MyAppURL "http://www.sisec.com.cn/"
#define DesktopName "¼������"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId=0FFA9F1A-5CF9-4E73-B571-B9F9AAB69B4E
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputBaseFilename=SRTool{#MyAppVersion}
Compression=lzma
SolidCompression=yes

[Languages]
Name: "ch"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags:checkablealone;
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Files]
Source: "..\bin\SRTool.exe"; DestDir: "{app}"; Flags: ignoreversion 
Source: "..\bin\DuiLib_u.dll"; DestDir: "{app}"; Flags: ignoreversion 
Source: "..\bin\msvcr120.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\msvcp120.dll"; DestDir: "{app}"; Flags: ignoreversion  

Source: "..\bin\OBSApi.dll"; DestDir: "{app}"; Flags: ignoreversion 
Source: "..\bin\OBSdll.dll"; DestDir: "{app}"; Flags: ignoreversion 
Source: "..\bin\ObsNvenc.dll"; DestDir: "{app}"; Flags: ignoreversion 
Source: "..\bin\PocoFoundation.dll"; DestDir: "{app}"; Flags: ignoreversion 
Source: "..\bin\QSVHelper.exe"; DestDir: "{app}"; Flags: ignoreversion 
Source: "..\bin\libx264-146.dll"; DestDir: "{app}"; Flags: ignoreversion 
;
Source: "..\bin\skin\record\*"; DestDir: "{app}\skin\record\"; Flags: ignoreversion recursesubdirs createallsubdirs
;
Source: "..\bin\locale\*"; DestDir: "{app}\locale\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\OBS\*"; DestDir: "{app}\OBS\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\pluginData\*"; DestDir: "{app}\pluginData\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\plugins\*"; DestDir: "{app}\plugins\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\sceneCollection\*"; DestDir: "{app}\sceneCollection\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\shaders\*"; DestDir: "{app}\shaders\"; Flags: ignoreversion recursesubdirs createallsubdirs

;Source: "..\bin\skin\menu.xml"; DestDir: "{app}\skin\"; Flags: ignoreversion 
Source: "..\bin\skin\record.xml"; DestDir: "{app}\skin\"; Flags: ignoreversion 
Source: "..\bin\skin\SettingDlg.xml"; DestDir: "{app}\skin\"; Flags: ignoreversion 
Source: "..\bin\skin\Capture.xml"; DestDir: "{app}\skin\"; Flags: ignoreversion 
Source: "..\bin\skin\AboutDlg.xml"; DestDir: "{app}\skin\"; Flags: ignoreversion 
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commondesktop}\{#DesktopName}"; Filename: "{app}\{#MyAppName}"; Tasks: desktopicon

[Run]
;Filename: "{app}\{#MyAppName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files;          Name: "{%HOMEPATH}\*.json"
Type: filesandordirs; Name: "{app}"
Type: filesandordirs; Name: "{%HOMEPATH}\OBS"