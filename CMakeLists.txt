cmake_minimum_required(VERSION 3.14)
project(Perevodchik)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_VERSION 6)
    set(QT_LIBRARIES Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_VERSION 5)
    set(QT_LIBRARIES Qt5::Widgets)
endif()

set(SOURCES
    main.cpp
    MainWindow.cpp
    LayoutFixer.cpp
)
set(HEADERS
    MainWindow.h
    LayoutFixer.h
)

add_executable(Perevodchik ${SOURCES} ${HEADERS})
if (WIN32)
    set_target_properties(Perevodchik PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
target_link_libraries(Perevodchik ${QT_LIBRARIES})
if(QT_VERSION EQUAL 6)
    target_compile_definitions(Perevodchik PRIVATE USE_QT6)
endif()