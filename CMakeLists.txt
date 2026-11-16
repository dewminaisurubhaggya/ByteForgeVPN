cmake_minimum_required(VERSION 3.21)
project(ByteForgeVPN VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Widgets Sql)

qt_add_executable(ByteForgeVPN
    app/main.cpp
    app/MainWindow.cpp
    app/MainWindow.h
    core/Database.cpp
    core/Database.h
    core/ProfileManager.cpp
    core/ProfileManager.h
    ui/Pages.cpp
    ui/Pages.h
)

target_include_directories(ByteForgeVPN PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ByteForgeVPN PRIVATE Qt6::Widgets Qt6::Sql)

if(MSVC)
    target_compile_options(ByteForgeVPN PRIVATE /W4 /utf-8)
endif()

if(WIN32)
    set_target_properties(ByteForgeVPN PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

include(GNUInstallDirs)
install(TARGETS ByteForgeVPN
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
)
