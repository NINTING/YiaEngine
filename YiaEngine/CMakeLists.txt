# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(YiaEngine)

# 包含子项目。

include_directories("${CMAKE_SOURCE_DIR}/YiaEngine")


include_directories("${CMAKE_SOURCE_DIR}/YiaEngine/External")
include_directories("${CMAKE_SOURCE_DIR}/YiaEngine/External/crossguid")
include_directories("${CMAKE_SOURCE_DIR}/YiaEngine/External/ImGui")

add_subdirectory("External")
add_subdirectory ("Engine")

# TODO: 如有需要，请添加测试并安装目标。
