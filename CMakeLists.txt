cmake_minimum_required(VERSION 3.16)

project(Microchat VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 6.2 REQUIRED COMPONENTS Network Widgets)

qt_add_executable(microchat MANUAL_FINALIZATION
    main.cpp
    microchat.cpp
    microchat.h
    microchat.ui
    qaesencryption.cpp
    qaesencryption.h
)

set_target_properties(microchat PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    CXX_EXTENSIONS OFF
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(microchat PRIVATE
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060000
)

target_link_libraries(microchat PRIVATE
    Qt6::Network
    Qt6::Widgets
)

include(GNUInstallDirs)

install(TARGETS microchat
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(microchat)
