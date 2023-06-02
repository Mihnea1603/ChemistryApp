ifeq ($(OS),Windows_NT)
release:
	if not exist build\release mkdir build\release
	copy assets\* build\release\ /Y
	copy dependencies\windows\release\* build\release\ /Y
	g++ -O2 -I"$(WX)\include" -I"$(WX)\lib\gcc_dll\mswu" -L"$(WX)\lib\gcc_dll" App.cpp MainFrame.cpp MainPanel.cpp ReactionPanel.cpp TheoryPanel.cpp -lwxbase32u -lwxmsw32u_core -o build\release\ChemistryApp -mwindows
debug:
	if not exist build\debug mkdir build\debug
	copy assets\* build\debug\ /Y
	copy dependencies\windows\debug\* build\debug\ /Y
	g++ -g -I"$(WX)\include" -I"$(WX)\lib\gcc_dll\mswud" -L"$(WX)\lib\gcc_dll" App.cpp MainFrame.cpp MainPanel.cpp ReactionPanel.cpp TheoryPanel.cpp -lwxbase32ud -lwxmsw32ud_core -o build\debug\ChemistryApp -mwindows
clean:
	rmdir /s /q build
else
release:
	mkdir -p build/release
	rsync -av --exclude="ChemistryApp.exe.manifest" assets/ build/release/
	cp dependencies/ubuntu/release/* build/release/
	g++ -O2 -I"$(WX)/include" -I"$(WX)/lib/wx/include/gtk3-unicode-3.2" -L"$(WX)/lib" App.cpp MainFrame.cpp MainPanel.cpp ReactionPanel.cpp TheoryPanel.cpp -lwx_baseu-3.2 -lwx_gtk3u_core-3.2 -o build/release/ChemistryApp -D__WXGTK__ -Wl,-rpath,'$$ORIGIN'
clean:
	rm -rf build
endif