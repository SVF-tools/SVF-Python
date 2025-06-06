cmake_minimum_required(VERSION 3.23)

project(SVF-PYTHON
        VERSION 0.1
        DESCRIPTION "SVF Python bindings"
        HOMEPAGE_URL "https://github.com/SVF-tools/SVF-Python"
        LANGUAGES C CXX
)
# print SVF_DIR, LLVM_DIR, and Z3_DIR
message(STATUS "SVF_DIR: ${SVF_DIR}")
message(STATUS "LLVM_DIR: ${LLVM_DIR}")
message(STATUS "Z3_DIR: ${Z3_DIR}")

find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)

message(STATUS "Using Python3: ${Python3_EXECUTABLE}")
message(STATUS "Python3 Include Dir: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3 Library: ${Python3_LIBRARIES}")

include_directories(${Python3_INCLUDE_DIRS})

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
    if(EXISTS "${SVF_DIR}/Release-build")
        set(SVF_CUSTOM_DIR "${SVF_DIR}/Release-build")
        set(SVF_INCLUDE_DIR "${SVF_CUSTOM_DIR}/include" "${SVF_DIR}/svf/include" "${SVF_DIR}/svf-llvm/include")
        set(SVF_LIB_DIR "${SVF_CUSTOM_DIR}/lib")
        message(STATUS "Using Release-build directory for SVF.")
    elseif(EXISTS "${SVF_DIR}/Debug-build")
        set(SVF_CUSTOM_DIR "${SVF_DIR}/Debug-build")
        set(SVF_INCLUDE_DIR "${SVF_CUSTOM_DIR}/include" "${SVF_DIR}/svf/include" "${SVF_DIR}/svf-llvm/include")
        set(SVF_LIB_DIR "${SVF_CUSTOM_DIR}/lib")
        message(STATUS "Using Debug-build directory for SVF.")
    else()
        set(SVF_CUSTOM_DIR "${SVF_DIR}")
        set(SVF_INCLUDE_DIR "${SVF_CUSTOM_DIR}/include")
        set(SVF_LIB_DIR "${SVF_CUSTOM_DIR}/lib")
        message(STATUS "Using Installed directory for SVF.")
    endif()
else()
    message(FATAL_ERROR "No SVF_DIR environment variable found; using OS-specific defaults.")
endif()

# ------------------------------------------------------------------------------
# 2) Add include directories and link directories for the pysvf target
# ------------------------------------------------------------------------------
set(SVF_LIB_EXT ${CMAKE_SHARED_LIBRARY_SUFFIX})
if(EXISTS "${Z3_DIR}/bin/libz3${SVF_LIB_EXT}")
    set(Z3_INCLUDE_DIR "${Z3_DIR}/include")
    set(Z3_LIB_FILE "${Z3_DIR}/bin/libz3${SVF_LIB_EXT}")
elseif(EXISTS "${Z3_DIR}/lib/libz3${SVF_LIB_EXT}")
    set(Z3_INCLUDE_DIR "${Z3_DIR}/include")
    set(Z3_LIB_FILE "${Z3_DIR}/lib/libz3${SVF_LIB_EXT}")
else()
    message(FATAL_ERROR "Z3 library not found in expected locations.")
endif()

message("SVF_INCLUDE_DIR: ${SVF_INCLUDE_DIR}")
include_directories(SYSTEM "${SVF_INCLUDE_DIR}" "${Z3_INCLUDE_DIR}")

file(GLOB_RECURSE PYBIND_CPP pybind/*.cpp)
add_library(pysvf MODULE ${PYBIND_CPP})
target_link_directories(pysvf PRIVATE "${SVF_LIB_DIR}")

# ------------------------------------------------------------------------------
# 3) Link against SVF libraries and pybind11
# ------------------------------------------------------------------------------

message("Z3_LIB_FILE: ${Z3_LIB_FILE}")
target_link_libraries(pysvf PRIVATE
        Python3::Module
        pybind11::module
        ${SVF_LIB_DIR}/libSvfLLVM${SVF_LIB_EXT}
        ${SVF_LIB_DIR}/libSvfCore${SVF_LIB_EXT}
        ${Z3_LIB_FILE}
)

# Set C++ standard to 17 (adjust as needed)
set_target_properties(pysvf PROPERTIES CXX_STANDARD 17)
