cmake_minimum_required (VERSION 3.8)
project(LZO)


FILE(GLOB LZX_HEADERS "Include/*.h")
FILE(GLOB_RECURSE LZX_SOURCES "Source/*.c")


include_directories("Include")
include_directories("Source") 

#add_compile_definitions(LZO_BUILDING_LIB)

add_library(LZX STATIC ${LZX_SOURCES})