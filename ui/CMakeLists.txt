#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE JOYSTICK-UI EXECUTABLE")

FIND_PACKAGE(Qt4 COMPONENTS QtGui QtCore REQUIRED)
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(GUI_HEADERS_FILES
    v_joystick_main_window.h
    v_joystick_about_dialog.h)

SET(GUI_SOURCE_FILES
    v_joystick_main_window.cpp
    v_joystick_about_dialog.cpp)

SET(QT_FORMS
    v_joystick_main_window.ui
    v_joystick_about_dialog.ui)

SET(MAIN_FILE
    main.cpp)

QT4_WRAP_CPP(GUI_MOC_SOURCES ${GUI_HEADERS_FILES})
QT4_WRAP_UI(QT_FORMS_HEADERS ${QT_FORMS})

SET(RESOURCES_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources)

SET(IMAGE_FILES
    ${RESOURCES_DIRECTORY}/logo.png)

QT4_ADD_RESOURCES(RESOURCES_FILES
    ${RESOURCES_DIRECTORY}/resources.qrc)

ADD_EXECUTABLE(vjoystick-ui
    ${GUI_HEADER_FILES}
    ${GUI_SOURCE_FILES}
    ${GUI_MOC_SOURCES}
    ${QT_FORMS_HEADERS}
    ${RESOURCES_FILES}
    ${IMAGE_FILES}
    ${MAIN_FILE})

ADD_DEPENDENCIES(vjoystick-ui vcore)

TARGET_LINK_LIBRARIES(vjoystick-ui ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(vjoystick-ui ${SDL_LIBRARY})
TARGET_LINK_LIBRARIES(vjoystick-ui vjoyqt)

#==============================================================================

IF(INSTALL_LIB)
    INSTALL(TARGETS vjoystick-ui DESTINATION bin)
ENDIF(INSTALL_LIB)
