# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)


include_directories("${PROJECT_SOURCE_DIR}/Framework/Common/IO")

# 添加可执行
aux_source_directory(. SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Sample Sample_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Windows Window_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Common/AssetLoder ASSETLODER_LIST)
add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST} ${Sample_LIST} ${ASSETLODER_LIST} ${PROJECT_SOURCE_DIR}/Framework/Windows/WindowBase.cpp)

add_subdirectory(Test)
#target_link_libraries(${PROJECT_NAME} PRIVATE  WindowDLL)



# 添加可执行
#aux_source_directory(. SRC_LIST)

#add_executable(YiaEngine ${SRC_LIST} )
