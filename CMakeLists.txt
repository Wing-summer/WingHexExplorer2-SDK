cmake_minimum_required(VERSION 3.16)

project(WingPlugin LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(
    WingPlugin SHARED
    iwingplugin.cpp
    iwingplugin.h
    iwingpluginbase.h
    iwingpluginbase.cpp
    wingplugin_global.h
    wingcore.h
    wingplugincalls.h
    wingplugincalls.cpp
    wingplugincalls_p.h
    wingeditorviewwidget.h
    wingeditorviewwidget.cpp
    settingpage.h
    iwingdevice.h
    iwingdevice.cpp
    settingpage.cpp
    iwingpluginbasecalls.h
    iwingpluginbasecalls.cpp
    iwingplugincalls.h
    iwingplugincalls.cpp
    wingplugincallconvertor.h
    wingpluginwidget.h
    wingpluginwidget.cpp
    iwingmanager.h
    iwingmanager.cpp
    iwingangel.h
    iwingangel.cpp
    hexeditorcontext.h
    hexeditorcontext.cpp
    iwinghexeditorplugin.h
    iwinghexeditorplugin.cpp)

target_link_libraries(WingPlugin PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(WingPlugin PRIVATE Qt6::Core)

target_compile_options(
    WingPlugin
    PRIVATE
        $<$<AND:$<CONFIG:Release>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:-g1>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/Z7>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:
        -ffunction-sections
        -fdata-sections
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fno-var-tracking
        -fno-var-tracking-assignments
        >
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:
        -ffunction-sections
        -fdata-sections
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fno-var-tracking
        >)

target_link_options(
    WingPlugin
    PRIVATE
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/DEBUG
    /OPT:REF
    /OPT:ICF>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:-Wl,--gc-sections>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-Wl,--gc-sections>)

target_include_directories(WingPlugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_compile_definitions(WingPlugin PRIVATE WINGPLUGIN_LIBRARY)
