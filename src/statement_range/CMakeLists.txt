cmake_minimum_required(VERSION 3.10)

# 项目名称和版本号
project(ASTAAnayer VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 LLVM 和 Clang 安装
find_package(LLVM 12 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
set(Clang_DIR "/usr/lib/llvm-12/lib/cmake/clang")
find_package(Clang REQUIRED CONFIG)

# 打印 LLVM 和 Clang 信息，帮助调试

message(STATUS "Found Clang: ${Clang_FOUND}")
message(STATUS "Clang include dirs: ${CLANG_INCLUDE_DIRS}")

# 包含 LLVM 和 Clang 的头文件路径
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

# 链接 LLVM 和 Clang 库
link_directories(${LLVM_LIBRARY_DIRS})
link_directories(${CLANG_LIBRARY_DIRS})

# 定义编译目标
add_executable(get_statement_range get_statement_range.cpp)

# 链接所需的库
target_link_libraries(get_statement_range
  PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangTooling
    LLVM
    tinyxml2
)

# 设置编译选项
target_compile_options(get_statement_range PRIVATE 
    ${LLVM_COMPILE_FLAGS}
    -Wno-unused-parameter
    -Wno-error
)

# 安装规则：将可执行文件安装到上一级目录的 bin 文件夹
install(TARGETS get_statement_range
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../../bin
)

