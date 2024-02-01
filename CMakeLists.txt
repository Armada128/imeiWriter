cmake_minimum_required(VERSION 3.21)

project(imeiWriter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick SerialBus Qml QuickControls2 Positioning Widgets)

qt_standard_project_setup(REQUIRES 6.5)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set(WIN_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/logo.rc)
endif()

set(myheaders ${PROJECT_SOURCE_DIR}/*.h)
set(mysources ${PROJECT_SOURCE_DIR}/*.cpp)
set(myqmls ${PROJECT_SOURCE_DIR}/*.qml)
set(myresources ${PROJECT_SOURCE_DIR}/assets/*.svg ${PROJECT_SOURCE_DIR}/assets/*.png ${PROJECT_SOURCE_DIR}/assets/*.jpg)

file(GLOB_RECURSE MYHEADERS ${myheaders})
file(GLOB_RECURSE MYSOURCES ${mysources})
file(GLOB_RECURSE MYQMLS RELATIVE ${PROJECT_SOURCE_DIR} ${myqmls})
file(GLOB_RECURSE MYRESOURCES RELATIVE ${PROJECT_SOURCE_DIR} ${myresources})

qt_add_executable(appimeiWriter ${MYHEADERS} ${MYSOURCES} qrclogo.qrc logo.rc)

#foreach(filename ${MYHEADERS})
#    get_filename_component(filetemp ${filename} ABSOLUTE)
#    list(APPEND MODULE_HEADERS ${filetemp})
#endforeach()

#foreach(filename ${MYSOURCES})
#    get_filename_component(filetemp ${filename} ABSOLUTE)
#    list(APPEND MODULE_SOURCES ${filetemp})
#endforeach()

#foreach(filename ${MYQMLS})
#    get_filename_component(filetemp ${filename} NAME)
#    list(APPEND MODULE_QMLS ${filetemp})
#endforeach()

#foreach(filename ${MYRESOURCES})
#    get_filename_component(filetemp ${filename} NAME)
#    list(APPEND MODULE_RESOURCES "assets/${filetemp}")
#endforeach()

##message("MODULE_HEADERS: ${MODULE_HEADERS}")
##message("MODULE_RESOURCES: ${MODULE_RESOURCES}")
#message("MYRESOURCES: ${MYRESOURCES}")
#message("MYQMLS: ${MYQMLS}")

##qt_add_qml_module(appimeiWriter
##    URI imeiWriter
##    VERSION 1.0
##    SOURCES
##        ${MODULE_HEADERS}
##        ${MODULE_SOURCES}
##    QML_FILES
##        ${MODULE_QMLS}
##    RESOURCES
##        ${MODULE_RESOURCES}
##)

qt_add_qml_module(appimeiWriter
    URI imeiWriter
    VERSION 1.0
    SOURCES
        ${MYHEADERS}
        ${MYSOURCES}
    QML_FILES
        ${MYQMLS}
    RESOURCES
        ${MYRESOURCES}
)

set_target_properties(appimeiWriter PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appimeiWriter
    PRIVATE Qt6::Quick Qt6::SerialBus Qt6::Qml Qt6::QuickControls2 Qt6::Positioning Qt6::Widgets
)

install(TARGETS appimeiWriter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
