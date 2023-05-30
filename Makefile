debug:
	if not exist build mkdir build
	if not exist build\Debug mkdir build\Debug
	copy assets\* build\Debug\ /Y
	copy dlls\debug\* build\Debug\ /Y
	g++ -I"$(WX)\include" -I"$(WX)\lib\gcc_dll\mswud" -L"$(WX)\lib\gcc_dll" App.cpp MainFrame.cpp MainPanel.cpp ReactionPanel.cpp TheoryPanel.cpp -lwxbase32ud -lwxmsw32ud_core -o build\Debug\ChemistryApp -mwindows
release:
	if not exist build mkdir build
	if not exist build\Release mkdir build\Release
	copy assets\* build\Release\ /Y
	copy dlls\release\* build\Release\ /Y
	g++ -O2 -I"$(WX)\include" -I"$(WX)\lib\gcc_dll\mswu" -L"$(WX)\lib\gcc_dll" App.cpp MainFrame.cpp MainPanel.cpp ReactionPanel.cpp TheoryPanel.cpp -lwxbase32u -lwxmsw32u_core -o build\Release\ChemistryApp -mwindows
clean:
	rmdir /s /q build