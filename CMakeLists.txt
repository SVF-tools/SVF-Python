cmake_minimum_required(VERSION 3.23)

project(SVF-PYTHON
        VERSION 0.1
        DESCRIPTION "SVF Python bindings"
        HOMEPAGE_URL "https://github.com/SVF-tools/SVF-Python"
        LANGUAGES C CXX
)

# 查找 Python
find_package(Python REQUIRED COMPONENTS Interpreter Development)
add_subdirectory(SVF)

# 启用 PYBIND11_FINDPYTHON
set(PYBIND11_FINDPYTHON ON)

# 查找 pybind11
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
include_directories(SYSTEM ${CMAKE_BINARY_DIR}/SVF/include)
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Check if LLVM was built generating the single libLLVM.so shared library file or as separate static libraries
if(LLVM_LINK_LLVM_DYLIB)
    message(STATUS "Linking to LLVM dynamic shared library object")
    set(llvm_libs LLVM)

    # Set which components to include in the dynamic library to include the new SvfLLVM
    if (LLVM_DYLIB_COMPONENTS)
        message(STATUS "Appending SvfLLVM to LLVM dynamic library components")
        list(APPEND LLVM_DYLIB_COMPONENTS SvfLLVM)
    else()
        message(STATUS "Adding all;SvfLLVM to LLVM dynamic library components (was unset)")
        set(LLVM_DYLIB_COMPONENTS all;SvfLLVM)
    endif()
else()
    message(STATUS "Linking to separate LLVM static libraries")
    llvm_map_components_to_libnames(
            llvm_libs
            analysis
            bitwriter
            core
            instcombine
            instrumentation
            ipo
            irreader
            linker
            scalaropts
            support
            target
            transformutils
    )
endif()

# Make the "add_llvm_library()" command available and configure LLVM/CMake
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)



# 添加编译目标
add_library(pysvf MODULE pybind/svf_pybind.cpp)

# 连接 pybind11 库
target_link_libraries(pysvf PRIVATE pybind11::module SvfLLVM SvfCore)

# 设置 C++ 标准
set_target_properties(pysvf PROPERTIES CXX_STANDARD 17)