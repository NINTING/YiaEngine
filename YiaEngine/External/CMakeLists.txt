# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(YiaEngine)

# 包含子项目。


set(ExternalDir ${CMAKE_SOURCE_DIR}/YiaEngine/External CACHE INTERNAL "ThridLibraryDirection")

include_directories("${CMAKE_SOURCE_DIR}/YiaEngine/External/crossguid")


ADD_SUBDIRECTORY("assimp")
# TODO: 如有需要，请添加测试并安装目标。
