all: build cook run

build: 
	"D:\ieu\apps\UnrealEngine\UE_4.27\Engine\Build\BatchFiles\Build.bat" AvinationViewer Win64 Development "D:\ieu\develop\opensim-viewer\AvinationViewer.uproject" -waitmutex -NoHotReload
	
edit:
	"D:\ieu\apps\UnrealEngine\UE_4.27\Engine\Binaries\Win64\UE4Editor.exe" "D:\ieu\develop\opensim-viewer\AvinationViewer.uproject" -game -log -windowed -resx=1200 -resy=720
	
cook:
	"D:\ieu\apps\UnrealEngine\UE_4.27\Engine\Binaries\Win64\UE4Editor-cmd.exe" "D:\ieu\develop\opensim-viewer\AvinationViewer.uproject" -run=cook -targetplatform=WindowsNoEditor
	
run:
	"D:\ieu\develop\opensim-viewer\Binaries\Win64\AvinationViewer.exe" -log -windowed -resx=1200 -resy=720
	
