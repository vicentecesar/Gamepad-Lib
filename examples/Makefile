ALL:
	@echo Chame "mingw32-make c++" ou "mingw32-make c"
c++:
	@echo Construindo a biblioteca C++...
	cd .. && mingw32-make c++
	@echo Construindo exemplo em C++...
	g++ "Example C++.cpp" -o "Example C++.exe" -I ../src/C++ -L ../lib -lgampadlib-c++ -lhid