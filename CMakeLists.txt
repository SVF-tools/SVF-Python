cmake_minimum_required(VERSION 3.23)

project(SVF-PYTHON
        VERSION 0.1
        DESCRIPTION "SVF Python bindings"
        HOMEPAGE_URL "https://github.com/SVF-tools/SVF-Python"
        LANGUAGES C CXX
)

find_package(Python REQUIRED COMPONENTS Interpreter Development)

set(PYBIND11_FINDPYTHON ON)

find_package(pybind11 REQUIRED)


# Find the LLVM instance to use for building SvfLLVM
find_package(LLVM REQUIRED CONFIG HINTS ${LLVM_DIR} $ENV{LLVM_DIR})
message(STATUS "LLVM STATUS:
  Version       ${LLVM_VERSION}
  Definitions   ${LLVM_DEFINITIONS}
  Includes      ${LLVM_INCLUDE_DIRS}
  Libraries     ${LLVM_LIBRARY_DIRS}
  Targets       ${LLVM_TARGETS_TO_BUILD}
  Build type    ${LLVM_BUILD_TYPE}
  Exceptions    ${LLVM_ENABLE_EH}
  RTTI          ${LLVM_ENABLE_RTTI}
  Dynamic lib   ${LLVM_LINK_LLVM_DYLIB}"
)

# Though not necessary, check if SVF is being built in debug mode & if that matches LLVM
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT ${LLVM_BUILD_TYPE} STREQUAL "Debug")
    message(NOTICE  "Building SVF in debug-mode but LLVM was not built in debug-mode; "
            "debug information could be incomplete when using SVF from LLVM")
endif()

# Add LLVM's include directories and link directory for all targets defined hereafter
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})


# if system is darwin or linux
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/SVF-npm/SVF-osx/Release-build/include)

elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/SVF-npm/SVF-linux/Release-build/include)

endif ()


add_library(pysvf MODULE pybind/svf_pybind.cpp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
# add target link dir
    target_link_directories(pysvf PRIVATE ${CMAKE_SOURCE_DIR}/SVF-npm/SVF-osx/Release-build/lib)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_directories(pysvf PRIVATE ${CMAKE_SOURCE_DIR}/SVF-npm/SVF-linux/Release-build/lib)
endif ()

target_link_libraries(pysvf PRIVATE pybind11::module SvfLLVM SvfCore)

set_target_properties(pysvf PROPERTIES CXX_STANDARD 17)