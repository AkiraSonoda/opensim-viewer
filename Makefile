all: build-game cook run

build-editor: 
	"D:\ieu\apps\UnrealEngine\UE_4.27\Engine\Build\BatchFiles\Build.bat" PotamosEditor Win64 Development "D:\ieu\develop\opensim-viewer\Potamos.uproject" -waitmutex -NoHotReload

build-game: 
	"D:\ieu\apps\UnrealEngine\UE_4.27\Engine\Build\BatchFiles\Build.bat" Potamos Win64 Development "D:\ieu\develop\opensim-viewer\Potamos.uproject" -waitmutex -NoHotReload
	
editor:
	"D:\ieu\apps\UnrealEngine\UE_4.27\Engine\Binaries\Win64\UE4Editor.exe" "D:\ieu\develop\opensim-viewer\Potamos.uproject" -log

editor-run:
	"D:\ieu\apps\UnrealEngine\UE_4.27\Engine\Binaries\Win64\UE4Editor.exe" "D:\ieu\develop\opensim-viewer\Potamos.uproject" -game -log -windowed -resx=1200 -resy=720
	
cook:
	"D:\ieu\apps\UnrealEngine\UE_4.27\Engine\Binaries\Win64\UE4Editor-cmd.exe" "D:\ieu\develop\opensim-viewer\Potamos.uproject" -run=cook -targetplatform=WindowsNoEditor
	
run:
	"D:\ieu\develop\opensim-viewer\Binaries\Win64\Potamos.exe" -log -windowed -resx=1200 -resy=720
	
