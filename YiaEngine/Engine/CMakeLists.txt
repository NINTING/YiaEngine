# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(YiaEngine)

# 包含子项目。

include_directories("${PROJECT_SOURCE_DIR}/Framework")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Common")
include_directories("${PROJECT_SOURCE_DIR}/Framework/EditorWindow")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Editor")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Interface")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Sample")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Windows")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Shader")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Math")

include_directories("${PROJECT_SOURCE_DIR}/Framework/Common/AssetLoder")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Common/ImageParser")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Scene")
add_subdirectory ("Framework")
IF(WIN32)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS") 
ENDIF(WIN32)
# TODO: 如有需要，请添加测试并安装目标。
