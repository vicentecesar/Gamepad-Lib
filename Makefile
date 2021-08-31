obj_dir = build
c++_src_dir = src/C++
c++_src_files := $(wildcard $(c++_src_dir)/*.cpp)
c++_obj_files := $(patsubst $(c++_src_dir)/%.cpp,$(obj_dir)/%.o,$(c++_src_files))

ALL:
	@echo Chame "mingw32-make c++" ou "mingw32-make c"

c++: $(c++_obj_files)
	@echo Construindo biblioteca C++...
	ar rvs lib/libgampadlib-c++.a build/*.o

$(obj_dir)/%.o: $(c++_src_dir)/%.cpp
	g++ -c -o $@ $<