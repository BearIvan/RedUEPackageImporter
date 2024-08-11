cmake_minimum_required (VERSION 3.8)
project(LZO)


FILE(GLOB LZO_HEADERS "Include/*.h")
FILE(GLOB_RECURSE LZO_SOURCES "Source/*.c")


include_directories("Include")
include_directories("Source") 

#add_compile_definitions(LZO_BUILDING_LIB)

add_library(LZO STATIC ${LZO_SOURCES})