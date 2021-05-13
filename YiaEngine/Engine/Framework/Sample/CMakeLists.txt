# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)



# 添加可执行
aux_source_directory(. SRC_LIST)

add_library(Dx12Sample STATIC ${SRC_LIST} )
target_include_directories(Dx12Sample PUBLIC Dx12Sample.h)