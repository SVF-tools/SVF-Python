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

# Ensure that LLVM was built with RTTI enabled
if(NOT LLVM_ENABLE_RTTI)
    message(FATAL_ERROR "LLVM was not built with RTTI enabled. Please rebuild LLVM with -DLLVM_ENABLE_RTTI=ON.")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
    set(CMAKE_BUILD_RPATH "@loader_path/SVF/z3.obj/bin;@loader_path/SVF/z3.obj/lib;@loader_path/SVF/Release-build/lib")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
    set(CMAKE_BUILD_RPATH "$ORIGIN/SVF/z3.obj/bin;$ORIGIN/SVF/z3.obj/lib;$ORIGIN/SVF/Release-build/lib;")
endif ()
# Check if building in Debug mode vs. LLVM's build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT ${LLVM_BUILD_TYPE} STREQUAL "Debug")
    message(NOTICE "Building SVF in debug-mode but LLVM was not built in debug-mode; "
            "debug information could be incomplete when using SVF from LLVM")
endif()

# Add LLVM's include directories for all targets defined hereafter
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

# ------------------------------------------------------------------------------
# 1) Decide where SVF headers/libraries come from
# ------------------------------------------------------------------------------
if(SVF_DIR)
    message(STATUS "Detected SVF_DIR environment variable: ${SVF_DIR}")
    set(SVF_CUSTOM_DIR ${SVF_DIR})
    set(SVF_INCLUDE_DIR "${SVF_CUSTOM_DIR}/Release-build/include" "${SVF_CUSTOM_DIR}/svf/include" "${SVF_CUSTOM_DIR}/svf-llvm/include")
    set(SVF_LIB_DIR     "${SVF_CUSTOM_DIR}/Release-build/lib")
else()
    message(STATUS "No SVF_DIR environment variable found; using OS-specific defaults.")
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(SVF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SVF-npm/SVF-osx/Release-build/include")
        set(SVF_LIB_DIR     "${CMAKE_SOURCE_DIR}/SVF-npm/SVF-osx/Release-build/lib")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(SVF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SVF-npm/SVF-linux/Release-build/include")
        set(SVF_LIB_DIR     "${CMAKE_SOURCE_DIR}/SVF-npm/SVF-linux/Release-build/lib")
    else()
        message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}. Please set SVF_DIR manually.")
    endif()
endif()

# ------------------------------------------------------------------------------
# 2) Add include directories and link directories for the pysvf target
# ------------------------------------------------------------------------------
message("SVF_INCLUDE_DIR: ${SVF_INCLUDE_DIR}")
include_directories(SYSTEM "${SVF_INCLUDE_DIR}")

add_library(pysvf MODULE pybind/svf_pybind.cpp)
target_link_directories(pysvf PRIVATE "${SVF_LIB_DIR}")

# ------------------------------------------------------------------------------
# 3) Link against SVF libraries and pybind11
# ------------------------------------------------------------------------------

set(SVF_LIB_EXT ${CMAKE_SHARED_LIBRARY_SUFFIX})
if(EXISTS "${Z3_DIR}/bin/libz3${SVF_LIB_EXT}")
    set(Z3_LIB_FILE "${Z3_DIR}/bin/libz3${SVF_LIB_EXT}")
elseif(EXISTS "${Z3_DIR}/lib/libz3${SVF_LIB_EXT}")
    set(Z3_LIB_FILE "${Z3_DIR}/lib/libz3${SVF_LIB_EXT}")
else()
    message(FATAL_ERROR "Z3 library not found in expected locations.")
endif()
message("Z3_LIB_FILE: ${Z3_LIB_FILE}")
target_link_libraries(pysvf PRIVATE
        pybind11::module
        ${SVF_LIB_DIR}/libSvfLLVM${SVF_LIB_EXT}
        ${SVF_LIB_DIR}/libSvfCore${SVF_LIB_EXT}
        ${Z3_LIB_FILE}
)

# Set C++ standard to 17 (adjust as needed)
set_target_properties(pysvf PROPERTIES CXX_STANDARD 17)
