
# CMakeLists.txt, generated gencmake.pl, on 2017/12/20 17:39:11
cmake_minimum_required( VERSION 2.8.8 )

# CMakeScripts or use the ones that come by default with CMake.
# set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

project( cwebpage )

# The version number.
set( cwebpage_MAJOR 3 )
set( cwebpage_MINOR 0 )
set( cwebpage_POINT 0 )

option( BUILD_EXAMPLE    "Set OFF to not build the example"     ON  )

if(CMAKE_COMPILER_IS_GNUCXX)
    set( WARNING_FLAGS -Wall )
endif(CMAKE_COMPILER_IS_GNUCXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
   set( WARNING_FLAGS "-Wall -Wno-overloaded-virtual" )
endif() 

if(WIN32 AND MSVC)
    # turn off various warnings - none needed in this compile
    # set(WARNING_FLAGS "${WARNING_FLAGS} /wd4996")
    # foreach(warning 4244 4251 4267 4275 4290 4786 4305)
    #     set(WARNING_FLAGS "${WARNING_FLAGS} /wd${warning}")
    # endforeach(warning)
    #set( MSVC_FLAGS "-DNOMINMAX -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS #-D__CRT_NONSTDC_NO_WARNINGS" )
    # to distinguish between debug and release libs/dll
    set( CMAKE_DEBUG_POSTFIX "d" )
else()
    # items for unix
endif()

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LD_FLAGS}" )

# configuration file, if needed
# configure_file( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h )
# add_definitions( -DHAVE_CONFIG_H )
# include_directories( ${CMAKE_BINARY_DIR} )

include_directories( dll )

# Total libs 1, exes 2
#################################################
##### LIBRARIES 1 #####
#################################################

# dll LIBRARY SHARED(DLL) from [dll],
# have 1 C/C++ sources, 1 headers
# NB: Since this DLL does NOT use '__declspec(dllimport|dllexport)'
#   on the EXPORTED functions, note MUST add the 'dll.def' file
#   as an alternative method to do the exports...
# NB: It is NOT intended that this DLL be 'auto' loaded by the
# 'example' app that uses it - it manually loads the DLL -
# hence it is NOT added to 'add_LIBS'...
set(name mydll)
set(dir dll)
set(${name}_SRCS
    ${dir}/dll.c
    ${dir}/dll.def
    )
set(${name}_HDRS
    ${dir}/cwebpage.h
    )
add_library( ${name} SHARED ${${name}_SRCS} ${${name}_HDRS} )
set_target_properties(${name} PROPERTIES OUTPUT_NAME "CWEBPAGE")
# list(APPEND add_LIBS ${name})
# deal with install, if any...
install( TARGETS ${name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib )
install(FILES ${${name}_HDRS} DESTINATION include)

#################################################
##### EXECUTABLE 2 #####
#################################################

# example EXECUTABLE from [dll\example],
# have 1 C/C++ sources, 0 headers
# Manually LOADS the above DLL
if (BUILD_EXAMPLE)
    set(name example)
    set(dir dll/example)
    set(${name}_SRCS
        ${dir}/example.c
        )
    add_executable( ${name}  WIN32 ${${name}_SRCS} )
    if (add_LIBS)
        target_link_libraries( ${name} ${add_LIBS} )
    endif ()
    if (MSVC)
        set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
    endif ()
    # deal with install, if any...
    #install( TARGETS ${name} DESTINATION bin )
endif (BUILD_EXAMPLE)

# CWebPage EXECUTABLE from [F:\Projects\cwebpage],
# have 1 C/C++ sources, 0 headers
# This app does NOT use the CWEBPAGE DLL above
# All the functions done by the DLL are included
# in this apps source...
set(name CWebPage)
set(dir src)
set(${name}_SRCS
    ${dir}/CWebPage.c
    )
add_executable( ${name}  WIN32 ${${name}_SRCS} )
if (add_LIBS)
    target_link_libraries( ${name} ${add_LIBS} )
endif ()
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install, if any...
#install( TARGETS ${name} DESTINATION bin )

# eof
