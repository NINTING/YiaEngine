# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

aux_source_directory(${PROJECT_SOURCE_DIR}/Framework/Common/AssetLoder AssetLoder_LIST)

set(testObj ${PROJECT_SOURCE_DIR}/Framework/Common/Allocator.cpp
	${PROJECT_SOURCE_DIR}/Framework/Common/MemoryManager.cpp
	${CMAKE_SOURCE_DIR}/YiaEngine/External/crossguid/guid.cpp	
	)

add_executable(allocTest test.cpp ${testObj} ${AssetLoder_LIST})
target_link_libraries(allocTest GeomMath)

file(COPY ${CMAKE_SOURCE_DIR}/YiaEngine/External/assimp/assimp-vc142-mtd.dll
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(allocTest
	${CMAKE_SOURCE_DIR}/YiaEngine/External/assimp/assimp-vc142-mtd.lib 
		)

add_test(NAME Test_Alloc COMMAND allocTest)