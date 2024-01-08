# Copyright 2023 WorldCourier. All rights reserved.
#
# Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

cmake_minimum_required(VERSION 3.16)

# Arrival project.
project(Arrival VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Packages needed.
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

# Require Qt 6.5.
qt_standard_project_setup(REQUIRES 6.5)

set_source_files_properties(qml/Style.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

set(ARRIVAL_APP_INCLUDE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/include/app.h

    ${CMAKE_CURRENT_LIST_DIR}/include/data/csvdocument.h
    ${CMAKE_CURRENT_LIST_DIR}/include/data/csvhandling.h
    ${CMAKE_CURRENT_LIST_DIR}/include/data/jobtable.h
    ${CMAKE_CURRENT_LIST_DIR}/include/data/jobtablerow.h
    ${CMAKE_CURRENT_LIST_DIR}/include/data/selectedheaderstemplate.h
    ${CMAKE_CURRENT_LIST_DIR}/include/data/selectedheaderstemplatelist.h

    ${CMAKE_CURRENT_LIST_DIR}/include/ui/appmodel.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ui/headerlistmodel.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ui/jobtablemodel.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ui/selectedheaderstemplatemodel.h)

set(ARRIVAL_APP_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/app.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/data/csvdocument.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/data/csvhandling.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/data/jobtable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/data/selectedheaderstemplate.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/data/selectedheaderstemplatelist.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/ui/appmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ui/headerlistmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ui/jobtablemodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ui/selectedheaderstemplatemodel.cpp)

set(ARRIVAL_APP_QML_FILES
    qml/App.qml
    qml/ArrivalBusyIndicator.qml
    qml/ArrivalButton.qml
    qml/ArrivalCheckBox.qml
    qml/ArrivalHeaderSelector.qml
    qml/ArrivalJobTable.qml
    qml/FileSelectionArea.qml
    qml/InfoLabelArea.qml
    qml/Main.qml
    qml/Style.qml)

set(ARRIVAL_APP_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resources/Arrival.rc")

# Executable and module.
qt_add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/resources/resources.qrc
    ${ARRIVAL_APP_INCLUDE_FILES}
    ${ARRIVAL_APP_SOURCE_FILES}
    src/main.cpp
    ${ARRIVAL_APP_RESOURCE_FILE}
)
qt_add_qml_module(${PROJECT_NAME}
    URI Arrival
    VERSION 1.0
    QML_FILES ${ARRIVAL_APP_QML_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER worldcourier.arrival.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick Qt6::QuickControls2 QtCSV QXlsx
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
