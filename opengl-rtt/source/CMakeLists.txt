# Author: Lauri Kortevaara
# Date 11 April 2015

# Set the compilation flags to report all warnings and enable debugging in the generated
# object files and executables

set( CMAKE_C_FLAGS "-Wall -g" )

# The following GLOB command will search for all files within the current directory
# that match the specified expressions (*.c, *.cpp, *.mm and *.h), and assign the resulting files
# to the variable SRCS. Thus, SRCS is a list of all C source and header files in the
# current directory.

file( GLOB SRCS *.c *.cpp *.h *.mm )

# Define the target application executable (named opengl_rtt) and the list of C source
# and header files needed for the executable.

set( WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin" )

add_executable( opengl_rtt ${SRCS} )

target_link_libraries(opengl_rtt "-framework CoreFoundation")
target_link_libraries(opengl_rtt "-framework OpenGL")
target_link_libraries(opengl_rtt ${CMAKE_BINARY_DIR}/lib/Debug/libglewd.a)
target_link_libraries(opengl_rtt ${CMAKE_BINARY_DIR}/lib/Debug/libglfw.dylib)
set_target_properties(opengl_rtt PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
