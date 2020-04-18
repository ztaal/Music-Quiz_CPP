
#Target: Jeopardy
SET(UisFiles JeopardyGUI.ui)
SET(RccSrcFiles)
QT5_ADD_RESOURCES(RccSrcFiles jeopardy_resources.qrc)
QT5_WRAP_UI(UiHeaders ${UisFiles})

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

add_executable(JeopardyGUI main.cpp JeopardyGUI.cpp ${RccSrcFiles} ${UiHeaders})
add_dependencies(JeopardyGUI ${PROJECT_NAME})
target_link_libraries(JeopardyGUI ${PROJECT_NAME})

