# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
IF(${WIN32})
	SET(GEOMMATH_LIB_FILE ${CMAKE_CURRENT_BINARY_DIR}/GeomMath.lib)
	SET(ISPC_COMPILER ${CMAKE_SOURCE_DIR}/YiaEngine/External/ispc/bin/ispc.exe)
	SET(ISPC_COMPILER_PATH ${CMAKE_SOURCE_DIR}/YiaEngine/External/ispc/bin/)
	
	SET(ISPC_OPTIONS --arch = x86-64)
	SET(ISPC_LIBRARIAN_OPTIONS lib /MACHINE:X64 /OUT:${GEOMMATH_LIB_FILE})
ENDIF(${WIN32})

SET(GEOMMATH_LIB_HEADER_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/operation)

#add_custom_command(OUTPUT ${GEOMMATH_LIB_FILE} COMMAND ${ISPC_COMPILER} ${ISPC_OPTIONS} -o )

add_library(GeomMath STATIC IMPORTED GLOBAL)
add_dependencies(GeomMath GeomMathLib)
set_target_properties(GeomMath 
					PROPERTIES
					IMPORTED_LOCATION ${GEOMMATH_LIB_FILE}
					INTERFACE_INCLUDE_DIRECTORIES ${GEOMMATH_LIB_HEADER_FOLDER}
					)
add_subdirectory(ispc)
add_subdirectory(test)