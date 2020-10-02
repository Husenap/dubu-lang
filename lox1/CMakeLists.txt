set(target_name "lox1")

set(src_precompiled
    "src/precompiled.h")

set(src_interpreter
    "src/lox1/interpreter/Expression.h"
    "src/lox1/interpreter/Interpreter.cpp"
    "src/lox1/interpreter/Interpreter.h"
    "src/lox1/interpreter/Token.cpp"
    "src/lox1/interpreter/Token.h"
    "src/lox1/interpreter/Parser.cpp"
    "src/lox1/interpreter/Parser.h"
    "src/lox1/interpreter/Lexer.cpp"
    "src/lox1/interpreter/Lexer.h"
    "src/lox1/interpreter/Lox.cpp"
    "src/lox1/interpreter/Lox.h")

set(src_lox1
    "src/lox1/Definitions.h"
    "src/lox1/lox1.h")

set(src_files
    ${src_precompiled}
    ${src_interpreter}
    ${src_lox1})

# Project
add_library(${target_name} STATIC ${src_files})

target_link_libraries(${target_name}
    compiler_features
    compiler_warnings)

target_precompile_headers(${target_name} PUBLIC ${src_precompiled})

source_group("src" FILES ${src_lox1})
source_group("src/interpreter" FILES ${src_interpreter})
source_group("src/precompiled" FILES ${src_precompiled})
source_group("src/precompiled" REGULAR_EXPRESSION "cmake_pch\.[ch]xx")

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(${target_name} PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})
