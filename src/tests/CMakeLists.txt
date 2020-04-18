# Target: test_helloWorld
add_executable(test_helloWorld test_helloWorld.cpp)
add_dependencies(test_helloWorld ${PROJECT_NAME})
target_link_libraries(test_helloWorld ${PROJECT_NAME})
