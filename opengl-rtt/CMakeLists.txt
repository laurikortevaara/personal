# Author: Lauri Kortevaara
# Date 11 April 2015

# Main CMake project file for opengl_rtt

cmake_minimum_required(VERSION 2.6)
SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

macro(ADD_FRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname}
    NAMES ${fwname}
    PATHS ${CMAKE_OSX_SYSROOT}/System/Library
    PATH_SUFFIXES Frameworks
    NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} "${FRAMEWORK_${fwname}}/${fwname}")
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_FRAMEWORK)

# Define GLM to use radians instead of degrees
add_definitions( -DGLM_FORCE_RADIANS )

# Define include directories
include_directories( thirdparty )
include_directories( thirdparty/glfw/include )
include_directories( thirdparty/glew_cmake/include )
include_directories( thirdparty/glm )
include_directories( thirdparty/fbx/include )

# Define the project name

project (opengl_rtt)

# Add a sub directory containing our source files
add_subdirectory( source )

add_subdirectory( thirdparty/glew_cmake )

SET( GLFW_BUILD_EXAMPLES 0 )
SET( GLFW_BUILD_TESTS 0 )
SET( GLFW_BUILD_DOCS 0 )
add_subdirectory( thirdparty/glfw )

file( GLOB THIRD_PARTY thirdparty/glfw/*.h )

# Finally print out all accessible variables
get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
