cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SeaChain)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("libaries/sfml")
set(SFML_INCS "libaries/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/libaries/sfml/lib")

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/resources"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/resources
)

add_subdirectory("libaries/b2d/Box2D")
set(B2D_INCS "libaries/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/libaries/sfml/lib")


##Engine Lib
file(GLOB_RECURSE SOURCE_FILES engine/*.cpp engine/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm lib_logger Box2D sfml-graphics sfml-audio)

# Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader lib_maths lib_logger sfml-graphics)

##ECM Lib
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm" )
target_link_libraries(lib_ecm lib_logger lib_maths)

##JSON Tag Lib
file(GLOB_RECURSE SOURCE_FILES lib_tag/*.cpp lib_tag/*.h)
add_library(lib_tag STATIC ${SOURCE_FILES})
target_include_directories(lib_tag INTERFACE "${CMAKE_SOURCE_DIR}/lib_tag" )
target_link_libraries(lib_tag lib_logger)

##Logger lib
file(GLOB_RECURSE SOURCE_FILES lib_logger/*.cpp lib_logger/*.h)
add_library(lib_logger STATIC ${SOURCE_FILES})
target_include_directories(lib_logger INTERFACE "${CMAKE_SOURCE_DIR}/lib_logger" )
target_link_libraries(lib_logger lib_engine)

file(GLOB_RECURSE SOURCES Sea_Chain/*.cpp Sea_Chain/*.h)
file(GLOB_RECURSE CMPNTS Sea_Chain/components/*.cpp Sea_Chain/components/*.h)
file(GLOB_RECURSE SCENES Sea_Chain/scenes/*.cpp Sea_Chain/scenes/*.h)
file(GLOB_RECURSE HLPRS Sea_Chain/helpers/*.cpp Sea_Chain/helpers/*.h)
add_executable(Sea_Chain ${SOURCES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("scenes" FILES ${SCENES})
source_group("helpers" FILES ${HLPRS})
source_group("resources" FILES ${RESOURCE_FILES})
#target_include_directories(Sea_Chain SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(Sea_Chain lib_engine)
set(EXECUTABLES ${EXECUTABLES} Sea_Chain)

set_target_properties(Sea_Chain
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)

add_dependencies(Sea_Chain copy_resources)
