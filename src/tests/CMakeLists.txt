# Target: test_helloWorld
add_executable(test_file "test_file.cpp")
add_dependencies(test_file ${PROJECT_NAME})
target_link_libraries(test_file ${PROJECT_NAME})
