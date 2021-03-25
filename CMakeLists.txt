cmake_minimum_required(VERSION 3.5)

project(nautilus-pdf-converter LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include (FindPkgConfig)
include (GNUInstallDirs)

pkg_check_modules(nautilus-extension libnautilus-extension REQUIRED)

include_directories(${nautilus-extension_INCLUDE_DIRS})
link_directories(${nautilus-extension_LIBRARY_DIRS})

add_library(${PROJECT_NAME} SHARED nautilus-pdf-converter.cpp nautilus-pdf-converter.h)

target_link_libraries(${PROJECT_NAME} PRIVATE ${nautilus-extension_LIBRARIES})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/nautilus/extensions-3.0)
