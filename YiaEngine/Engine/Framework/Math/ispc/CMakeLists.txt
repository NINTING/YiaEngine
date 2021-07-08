# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#


set(FUNCTIONS simple AddByElement DCT8X8 MatrixOperation SubtractByElement TestFunc Dot Cross)

foreach(FUNC IN LISTS FUNCTIONS)
	add_custom_command(OUTPUT ${FUNC}.o 
		COMMAND ${CMAKE_COMMAND} -E env "PATH=${ISPC_COMPILER_PATH}" ${ISPC_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/${FUNC}.ispc  -h ${GEOMMATH_LIB_HEADER_FOLDER}/${FUNC}.h -o ${FUNC}.o 
		MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${FUNC}.ispc
		COMMENT "BUILDING ${FUNC}"
		)
		list(APPEND OBJECTS ${FUNC}.o)

endforeach(FUNC)

IF(${WIN32})
add_custom_command(OUTPUT ${GEOMMATH_LIB_FILE}
					COMMAND ${ISPC_LIBRARIAN_OPTIONS} ${OBJECTS}
					COMMAND del  ${OBJECTS}
					DEPENDS ${OBJECTS}
					)
ENDIF(${WIN32})
add_custom_target(GeomMathLib
	DEPENDS ${GEOMMATH_LIB_FILE}
)