[Setup]
AppName=Perevodchik
AppVersion=1.0
DefaultDirName={pf}\Perevodchik
DefaultGroupName=Perevodchik
OutputBaseFilename=PerevodchikSetup
Compression=lzma
SolidCompression=yes

[Files]
Source: "C:\Data\plugins\IdeaProjects\Perevodchik\build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\Perevodchik"; Filename: "{app}\Perevodchik.exe"
Name: "{userdesktop}\Perevodchik"; Filename: "{app}\Perevodchik.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Создать ярлык на рабочем столе"; GroupDescription: "Дополнительные задачи:" 