cmake_minimum_required(VERSION 3.11)
# Require modern C++
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
elseif(APPLE)
  message("hello apple")
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9 CACHE STRING "")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SeaChain)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("libaries/sfml" EXCLUDE_FROM_ALL)
set(SFML_INCS "libaries/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/libaries/sfml/lib")

add_subdirectory("libaries/b2d/Box2D" EXCLUDE_FROM_ALL)
set(B2D_INCS "libaries/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/libaries/sfml/lib")

file(GLOB_RECURSE RESOURCE_FILES "resources/*.*")

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

#### Target for copying resources to build dir####
if(APPLE)
  add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/lib/sfml/extlibs/libs-osx/Frameworks" ${CMAKE_BINARY_DIR}/lib/sfml/Frameworks
  )
else()
  add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/resources
  )
endif()

#### Link Dependencies ####
foreach (exe ${EXECUTABLES})
  #set working directory to build dir
  set_target_properties(${exe} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
  )

  target_link_libraries(${exe}
    lib_maths
    optimized sfml-audio debug sfml-audio-d
    optimized sfml-window debug sfml-window-d
    optimized sfml-system debug sfml-system-d
    optimized sfml-graphics debug sfml-graphics-d
    optimized sfml-main
  )

  if(MSVC) # Hide Console in release builds
    set_target_properties(${exe} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
  endif(MSVC)

  add_dependencies(${exe} sfml-graphics sfml-audio copy_resources)
endforeach ()


#Install directory within build folder, so it doesn't go into /lib or program-files
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/")
#Installer.exe/zip built into /installer
SET(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/installer/")

#Install component for runtime dlls and stuff
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE) #stop this happening on non-release buildss
INCLUDE(InstallRequiredSystemLibraries)
install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "bin/" CONFIGURATIONS Release COMPONENT System )
#Install component for resources
install(DIRECTORY "${PROJECT_SOURCE_DIR}/res" DESTINATION "bin/" CONFIGURATIONS Release COMPONENT resources_cmpt)
#We disabled SFML's INSTALL component as it brings along junk, so we gotta make one ourselves
install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/" DESTINATION "bin" CONFIGURATIONS Release COMPONENT libs_cmpt FILES_MATCHING PATTERN "*.dll" )

#Tell CPack about the libs and resources install components
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} resources_cmpt libs_cmpt)
#CPACK_INSTALL_CMAKE_PROJECTS IS only necessary if not using CPACK_COMPONENTS_ALL
#set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};resources_cmpt;/)
#set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};libs_cmpt;/)

#Install component for all labs, also tell CPack about them
foreach (exe ${EXECUTABLES})
  install(TARGETS ${exe} DESTINATION "bin" CONFIGURATIONS Release COMPONENT ${exe}_cmpt )
  #set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};${exe}_cmpt;/)
  set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} ${exe}_cmpt)
  set(CPACK_COMPONENT_${exe}_CMPT_DEPENDS resources_cmpt libs_cmpt)
  set(CPACK_COMPONENT_${exe}_CMPT_GROUP "Labs")
endforeach ()


IF(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_NSIS_COMPONENT_INSTALL ON)
  set(CPACK_NSIS_HELP_LINK "https://github.com/edinburgh-napier/set09121")
  set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/edinburgh-napier/set09121")
  set(CPACK_NSIS_CONTACT "https://github.com/edinburgh-napier")
ELSE()
  set(CPACK_GENERATOR ZIP)
ENDIF(WIN32)


set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "set09119")
set(CPACK_PACKAGE_VENDOR "Edinburgh Napier University")
find_file (LICENSE NAMES LICENSE LICENSE.txt PATHS  "${CMAKE_SOURCE_DIR}/../" "${CMAKE_SOURCE_DIR}" NO_DEFAULT_PATH)
if(${LICENSE})
  set(CPACK_RESOURCE_FILE_LICENSE ${LICENSE})
endif(${LICENSE})
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

INCLUDE(CPack)
