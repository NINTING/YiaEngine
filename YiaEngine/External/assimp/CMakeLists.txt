# CMakeList.txt: YiaEngine 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(YiaEngine)

# 包含子项目。

#assimp-vc142-mt.dll

Set(assimpLib assimp-vc142-mt.lib  CACHE INTERNAL "assimpLib")
# TODO: 如有需要，请添加测试并安装目标。
