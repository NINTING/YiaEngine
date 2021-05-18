# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)


# 包含子项目。
aux_source_directory(. SRC_LIST)

add_library(WindowDLL SHARED ${SRC_LIST} )

target_include_directories(WindowDLL PUBLIC "${PROJECT_SOURCE_DIR}/Sample/Dx12Sample.h")
#add_subdirectory("Interface")

target_link_libraries(WindowDLL PUBLIC Dx12Sample)