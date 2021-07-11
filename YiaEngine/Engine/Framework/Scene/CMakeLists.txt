# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

#aux_source_directory(${CURRENT_SOURCE_DIR}/Framework/Scene COMMON_LIST)
#aux_source_directory(${CURRENT_SOURCE_DIR}/Framework/Scene/Node NODE_LIST)
#aux_source_directory(${CURRENT_SOURCE_DIR}/Framework/Scene/Object OBJECT_LIST)

add_library(YiaScene STATIC SceneEnum.cpp)

add_subdirectory("Test")