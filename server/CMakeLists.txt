cmake_minimum_required(VERSION 3.16)
project(DeviceMonitorServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Network)

set(SERVER_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/server_worker.cpp
    src/server_worker.h
    src/client_handler.cpp
    src/client_handler.h
    src/client_config.h
    src/data_models.h
    src/protocol.h
)

add_executable(DeviceMonitorServer ${SERVER_SOURCES})

target_link_libraries(DeviceMonitorServer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

target_include_directories(DeviceMonitorServer PRIVATE src)
