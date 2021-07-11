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



add_library(YiaMath STATIC IMPORTED GLOBAL)
add_library(GeomMath STATIC IMPORTED GLOBAL)
add_library(CommonMath STATIC CommonMath.cpp)


add_dependencies(GeomMath GeomMathLib)


set_target_properties(GeomMath 
					PROPERTIES
					IMPORTED_LOCATION ${GEOMMATH_LIB_FILE}
					INTERFACE_INCLUDE_DIRECTORIES ${GEOMMATH_LIB_HEADER_FOLDER}
					)


set_target_properties(YiaMath
	PROPERTIES
	IMPORTED_LOCATION  ${CMAKE_CURRENT_BINARY_DIR}/math.lib
)



add_subdirectory(ispc)

find_program(MSVC_LIB_TOOL lib.exe)

if(MSVC_LIB_TOOL)
	message(${MSVC_LIB_TOOL})
	add_dependencies(YiaMath Combine GeomMath CommonMath)
#    add_custom_target(Combine
#        COMMAND ${MSVC_LIB_TOOL} /OUT:${CMAKE_CURRENT_BINARY_DIR}/math.lib
#				${CMAKE_CURRENT_BINARY_DIR}/GeomMath.lib
#				${CMAKE_CURRENT_BINARY_DIR}/CommonMath.lib
#       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#		DEPENDS ${GEOMMATH_LIB_FILE}
#    )

	add_custom_target(Combine
        COMMAND ${MSVC_LIB_TOOL} /OUT:${CMAKE_CURRENT_BINARY_DIR}/math.lib
				$<TARGET_FILE:GeomMath> 
				$<TARGET_FILE:CommonMath> 
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		
    )
	 
          
endif()
add_subdirectory(test)



