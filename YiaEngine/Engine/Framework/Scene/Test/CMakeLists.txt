# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)




aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Scene SceneList)


link_directories(${CMAKE_SOURCE_DIR}/YiaEngine/External/assimp)



add_executable(SceneTest test.cpp ${SceneList} 
	${PROJECT_SOURCE_DIR}/Framework/Common/AssetLoder/AssetLoder.cpp
	${PROJECT_SOURCE_DIR}/Framework/Common/MemoryManager.cpp
	${PROJECT_SOURCE_DIR}/Framework/Common/Allocator.cpp)

#install (FILES  "${CMAKE_SOURCE_DIR}/YiaEngine/External/assimp/assimp-vc142-mt.dll" DESTINATION bin)

file(COPY ${CMAKE_SOURCE_DIR}/YiaEngine/External/assimp/assimp-vc142-mtd.dll
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#set_target_properties(SceneTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG} )

target_link_libraries(SceneTest 
	${CMAKE_SOURCE_DIR}/YiaEngine/External/assimp/assimp-vc142-mtd.lib 
		)

add_test(NAME Test_Scene COMMAND SceneTest)