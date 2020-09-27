set(target_name "lox1_test")

set(src_files
    "unit/ExpressionTests.cpp"
    "unit/Tests.cpp")

add_executable(${target_name} ${src_files})

target_link_libraries(${target_name}
    gmock_main
    lox1)

set_target_properties(${target_name} PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER}/test)

add_custom_command(
    TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/
    $<TARGET_FILE_DIR:${target_name}>)

gtest_discover_tests(${target_name}
	WORKING_DIRECTORY $<TARGET_FILE_DIR:${target_name}>
	VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${target_name}>)