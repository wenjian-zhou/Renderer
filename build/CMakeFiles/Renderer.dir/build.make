# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/wenjian_zhou/Program/cpp/Renderer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/wenjian_zhou/Program/cpp/Renderer/build

# Include any dependencies generated for this target.
include CMakeFiles/Renderer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Renderer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Renderer.dir/flags.make

CMakeFiles/Renderer.dir/src/core/main.cpp.o: CMakeFiles/Renderer.dir/flags.make
CMakeFiles/Renderer.dir/src/core/main.cpp.o: ../src/core/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wenjian_zhou/Program/cpp/Renderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Renderer.dir/src/core/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Renderer.dir/src/core/main.cpp.o -c /Users/wenjian_zhou/Program/cpp/Renderer/src/core/main.cpp

CMakeFiles/Renderer.dir/src/core/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Renderer.dir/src/core/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wenjian_zhou/Program/cpp/Renderer/src/core/main.cpp > CMakeFiles/Renderer.dir/src/core/main.cpp.i

CMakeFiles/Renderer.dir/src/core/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Renderer.dir/src/core/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wenjian_zhou/Program/cpp/Renderer/src/core/main.cpp -o CMakeFiles/Renderer.dir/src/core/main.cpp.s

# Object files for target Renderer
Renderer_OBJECTS = \
"CMakeFiles/Renderer.dir/src/core/main.cpp.o"

# External object files for target Renderer
Renderer_EXTERNAL_OBJECTS =

Renderer: CMakeFiles/Renderer.dir/src/core/main.cpp.o
Renderer: CMakeFiles/Renderer.dir/build.make
Renderer: CMakeFiles/Renderer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wenjian_zhou/Program/cpp/Renderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Renderer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Renderer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Renderer.dir/build: Renderer

.PHONY : CMakeFiles/Renderer.dir/build

CMakeFiles/Renderer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Renderer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Renderer.dir/clean

CMakeFiles/Renderer.dir/depend:
	cd /Users/wenjian_zhou/Program/cpp/Renderer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wenjian_zhou/Program/cpp/Renderer /Users/wenjian_zhou/Program/cpp/Renderer /Users/wenjian_zhou/Program/cpp/Renderer/build /Users/wenjian_zhou/Program/cpp/Renderer/build /Users/wenjian_zhou/Program/cpp/Renderer/build/CMakeFiles/Renderer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Renderer.dir/depend
