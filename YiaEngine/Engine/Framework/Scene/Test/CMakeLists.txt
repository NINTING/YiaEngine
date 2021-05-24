# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)




aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Scene SceneList)

add_executable(SceneTest test.cpp ${SceneList})

target_link_libraries(SceneTest ${assimpLib})

add_test(NAME Test_Scene COMMAND SceneTest)