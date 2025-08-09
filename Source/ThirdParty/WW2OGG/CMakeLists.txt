cmake_minimum_required (VERSION 3.8..4.3)
project(WW2OGG)


FILE(GLOB WW2OGG_HEADERS "Include/*.h")
FILE(GLOB_RECURSE WW2OGG_SOURCES "Source/*.c*")


include_directories("Include")
include_directories("Source") 

#add_compile_definitions(WW2OGG_BUILDING_LIB)

add_library(WW2OGG STATIC ${WW2OGG_SOURCES})