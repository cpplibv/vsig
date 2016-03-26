#

cmake_minimum_required (VERSION 3.1)
project (libv)
include(ExternalProject)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor count: ${PROCESSOR_COUNT}")
message(STATUS "CXX compiler id: ${CMAKE_CXX_COMPILER_ID}")

# --------------------------------------------------------------------------------------------------

# TODO P2: use cmake copy function instead of "cp"
# TODO P2: flag or build options to turn on/off sanitizes
# TODO P2: debug symbol automated turn on/off
# TODO P3: backward compatibility for pre gcc 5.1 because suggest-override
# TODO P3: test clang build
# TODO P5: test msvc build

# --------------------------------------------------------------------------------------------------

#set(CMAKE_VERBOSE_MAKEFILE off)
#set(CMAKE_BUILD_TYPE "DEBUG")
set(CMAKE_BUILD_TYPE "RELEASE")

#Definitions
if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
	add_definitions(-O0)
	add_definitions(-ggdb3)
else() #elseif(CMAKE_BUILD_TYPE EQUAL "RELEASE")
	add_definitions(-Ofast)
endif()

add_definitions(-m64)
add_definitions(-std=c++14)

#Macro
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLM_FORCE_RADIANS)

# --------------------------------------------------------------------------------------------------

#Paths
set(PATH_BIN ${PROJECT_SOURCE_DIR}/bin)
set(PATH_BLD build)
set(PATH_LIB ${PROJECT_SOURCE_DIR}/lib)
set(PATH_PRJ ${CMAKE_CURRENT_SOURCE_DIR})
set(PATH_EXT ${PATH_PRJ}/ext)
set(PATH_EXT_SRC ${PATH_PRJ}/ext_src)
set(PATH_INC include)
set(PATH_SRC src)
set(PATH_EXP example)
set(PATH_SNB sandbox)
set(PATH_TST test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PATH_LIB})

#Variables
set(TOOLSET)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

	add_compile_options(/Wall)
	set(TOOLSET msvc)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")

	add_compile_options(-Weverything -Wconversion -Wmissing-override -Wno-comment)
	set(TOOLSET clang)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")

	add_compile_options(-Wall -Wextra -Wconversion -Wpedantic -Wno-comment)

	if (NOT(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1))
		add_compile_options(-Wsuggest-override)
	endif()

	if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
		#add_compile_options(-fsanitize=address)
		#add_compile_options(-fsanitize=kernel-address)
		#add_compile_options(-fsanitize=leak)
		#add_compile_options(-fsanitize=thread)
		#add_compile_options(-fsanitize=undefined)
	endif()
	set(TOOLSET gcc)

endif()

# --------------------------------------------------------------------------------------------------

#if(NOT WIN32)
#	link_libraries(pthread)
#endif()

# --------------------------------------------------------------------------------------------------

set(EXTERNAL_PROJECT_LIST)

list(APPEND EXTERNAL_PROJECT_LIST ext_unittest)
ExternalProject_Add(ext_unittest
	GIT_REPOSITORY https://github.com/philsquared/Catch.git
	GIT_TAG master
	PREFIX ${PATH_EXT_SRC}/unittest
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		mkdir -p ${PATH_EXT}/unittest/ &&
		cp -r -f ${PATH_EXT_SRC}/unittest/src/ext_unittest/include/ ${PATH_EXT}/unittest/
	EXCLUDE_FROM_ALL 1
)

# --------------------------------------------------------------------------------------------------

#Includes
include_directories (${PATH_INC})

#Externals
include_directories(SYSTEM ${PATH_EXT}/unittest/include)

#Test Sources
file(GLOB_RECURSE vsig_test_src   RELATIVE ${PATH_PRJ} ${PATH_TST}/sig/*.cpp)

#Tests
add_executable(vsig_test   ${vsig_test_src})

#Sandbox - Executables
add_executable(vsig_main   ${PATH_SNB}/vsig_main.cpp)

#Example - Executables
#add_executable(vsig_main   ${PATH_EXP}/example0_signal_doing_this.cpp)
#add_executable(vsig_main   ${PATH_EXP}/example1_signal_doing_this.cpp)
#add_executable(vsig_main   ${PATH_EXP}/example2_signal_doing_this.cpp)
#add_executable(vsig_main   ${PATH_EXP}/example3_signal_doing_this.cpp)

#Custom Targets
add_custom_target(externals DEPENDS ${EXTERNAL_PROJECT_LIST})
add_custom_target(ext DEPENDS externals)

#Link - Tests
target_link_libraries(vsig_test   -static )

#Link - Executables
#target_link_libraries(vsig_main   -static vlog cppformat)