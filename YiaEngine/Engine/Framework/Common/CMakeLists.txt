# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)




include_directories("${PROJECT_SOURCE_DIR}/Framework/Common/IO")


# 添加可执行
aux_source_directory(. SRC_LIST)
#aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Sample Sample_LIST)
set(Sample_LIST ${PROJECT_SOURCE_DIR}/Framework/Sample/Dx12Sample.cpp)

aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Windows Window_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Common/AssetLoder ASSETLODER_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Common/ImageParser PARSER_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Core Core_LIST)

add_executable(${PROJECT_NAME} WIN32 
	${SRC_LIST} 
	${Sample_LIST} 
	${ASSETLODER_LIST} 
	${Core_LIST}
	${PROJECT_SOURCE_DIR}/Framework/Windows/WindowBase.cpp
	${PARSER_LIST}
	${ExternalDir}/crossguid/guid.cpp "MacroHelp.h"
	${PROJECT_SOURCE_DIR}/Framework/Common/Allocator.cpp
	${PROJECT_SOURCE_DIR}/Framework/Common/MemoryManager.cpp
	${CMAKE_SOURCE_DIR}/YiaEngine/External/crossguid/guid.cpp
	${CMAKE_SOURCE_DIR}/YiaEngine/External/ImGui/backend/imgui_impl_dx12.cpp
	${CMAKE_SOURCE_DIR}/YiaEngine/External/ImGui/backend/imgui_impl_win32.cpp
	)

file(COPY ${CMAKE_SOURCE_DIR}/YiaEngine/External/assimp/assimp-vc142-mtd.dll
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/YiaEngine/External/freeImage/FreeImage.dll
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(YiaMath  YiaScene IMGUI)

target_link_libraries(${PROJECT_NAME} 
	YiaMath
	YiaScene
	IMGUI
	${CMAKE_SOURCE_DIR}/YiaEngine/External/assimp/assimp-vc142-mtd.lib 
	${CMAKE_SOURCE_DIR}/YiaEngine/External/freeImage/FreeImage.lib
)

#add_subdirectory(AssetLoder)
add_subdirectory(Test)

#target_link_libraries(${PROJECT_NAME} PRIVATE  WindowDLL)



# 添加可执行
#aux_source_directory(. SRC_LIST)

#add_executable(YiaEngine ${SRC_LIST} )
