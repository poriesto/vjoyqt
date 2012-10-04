#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE VJOYQT LIBRARY")

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

FIND_PACKAGE(SDL)
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})

SET(LIB_HEADER_FILES
    v_joystick_adapter.h)

SET(LIB_SOURCE_FILES
    v_joystick_adapter.cpp)

QT4_WRAP_CPP(MOC_SOURCES ${LIB_HEADER_FILES})

ADD_LIBRARY(vjoyqt ${LIB_HEADER_FILES} ${LIB_SOURCE_FILES} ${MOC_SOURCES})
TARGET_LINK_LIBRARIES(vjoyqt ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(vjoyqt ${SDL_LIBRARY})

INSTALL(TARGETS vjoyqt DESTINATION lib)
