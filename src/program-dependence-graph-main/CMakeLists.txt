cmake_minimum_required(VERSION 3.4.3)
project(pdg)
# 明确指定查找 LLVM 12 版本
find_package(LLVM 12 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
# 使用标准的 LLVM_DIR 变量来显示路径
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

message("LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}
  Targets     ${LLVM_TARGETS_TO_BUILD}"
)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Use the same C++ standard as LLVM does
set(CMAKE_CXX_STANDARD 14 CACHE STRING "")

# Build type
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE
      STRING "Build type (default Debug):" FORCE)
endif()

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall\
    -fdiagnostics-color=always -Wno-everything")


include_directories("include")
include_directories("src")
# include_directories("sea-dsa/include")

# Common source files, excluding the mutually exclusive QPGGraph implementations
set(COMMON_SOURCES
    src/CallWrapper.cpp
    src/ControlDependencyGraph.cpp
    src/DataDependencyGraph.cpp
    src/DebugInfoUtils.cpp
    src/FunctionWrapper.cpp
    src/GlobalVarUsage.cpp
    src/Graph.cpp
    src/GraphWriter.cpp
    src/parserxml.cpp
    src/PDGCallGraph.cpp
    src/PDGNode.cpp
    src/PDGUtils.cpp
    src/ProgramDependencyGraph.cpp
    src/count_ir_stats.cpp
)

file(GLOB HEADERS "include/*.hh")

# --- Create separate libraries for each variant ---

# Variant 'b'
add_library(pdg_b MODULE
    ${HEADERS}
    ${COMMON_SOURCES}
    src/QPGGraph_b.cpp
)
target_link_libraries(pdg_b PRIVATE pugixml z3)
if(APPLE)
    set_target_properties(pdg_b PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)


# Variant 'u'
add_library(pdg_u MODULE
    ${HEADERS}
    ${COMMON_SOURCES}
    src/QPGGraph_u.cpp
)
target_link_libraries(pdg_u PRIVATE pugixml z3)
if(APPLE)
    set_target_properties(pdg_u PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)


# Variant 'noz3'
add_library(pdg_noz3 MODULE
    ${HEADERS}
    ${COMMON_SOURCES}
    src/QPGGraph_noz3.cpp
)
# This target does not link against z3
target_link_libraries(pdg_noz3 PRIVATE pugixml)
if(APPLE)
    set_target_properties(pdg_noz3 PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)


# --- Original static library, if still needed ---
# This creates a static library with only the common files.
# The specific QPGGraph implementations are excluded to avoid conflicts.
add_library(pdg_shared STATIC 
    ${COMMON_SOURCES}
)

# Link pugixml library. Note: z3 is linked to specific targets above.
# target_link_libraries(pdg PRIVATE pugixml z3) # This is now handled per-target

#
# Turn on C++11, turn off C++ RTTI.
#
# target_compile_features(pdg PRIVATE cxx_range_for cxx_auto_type)
# set_target_properties(pdg PROPERTIES COMPILE_FLAGS "-fno-rtti")

#
# OS X-specific configuration
#
# This is now handled per-target
# if(APPLE)
#     set_target_properties(pdg PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
# endif(APPLE)

# --- Installation ---
# This will install the .so files to the ../../lib directory relative to the source directory
install(TARGETS pdg_b pdg_u pdg_noz3
    LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/../../lib
)

