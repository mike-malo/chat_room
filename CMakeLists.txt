cmake_minimum_required(VERSION 3.5)

project(chat_room LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets WebSockets Network REQUIRED)

add_executable(chat_room
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    clientcore.cpp
    clientcore.h
    chatlist.ui
    chatlist.cpp
    chatlist.h
    itemwidget.h
    itemwidget.cpp
    itemwidget.ui
    wstest.cpp
    wstest.h
    res.qrc
)

target_link_libraries(chat_room PRIVATE
    Qt5::Widgets
    Qt5::Network
    Qt5::WebSockets
)
