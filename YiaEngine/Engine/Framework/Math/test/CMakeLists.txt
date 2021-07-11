# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#



add_executable(GeomMathTest test.cpp )
add_dependencies(GeomMathTest YiaMath)
#target_link_libraries(GeomMathTest  GeomMath 
#	CommonMath)
 target_link_libraries(GeomMathTest YiaMath)
add_test(NAME Test_GeomMath COMMAND GeomMathTest)