# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#


# 包含子项目。
aux_source_directory(. IMGUI_LIST)
aux_source_directory(./backend IMPL_TEST_LIST)

add_library(IMGUI STATIC ${IMGUI_LIST})


add_executable(ImGuiTest ${IMPL_TEST_LIST})

target_link_libraries(ImGuiTest IMGUI)
 

add_test(NAME Test_IMGUI COMMAND ImGuiTest)



#ADD_SUBDIRECTORY("assimp")
# TODO: 如有需要，请添加测试并安装目标。
