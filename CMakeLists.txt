PROJECT(Quinect)

SET(CMAKE_MODULE_PATH "${Quinect_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)
ADD_DEFINITIONS(-DUSE_QT4)
FIND_PACKAGE(Qt4 REQUIRED) # find and setup Qt4 for this project
SET(QT_USE_OPENGL TRUE)
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(OpenKinect REQUIRED)

OPTION(DEBUG_BUILD "Compile in debug mode" OFF)

IF(DEBUG_BUILD)
  SET(CMAKE_BUILD_TYPE "Debug")
  SET(LIBS
    ${QT_QTGUI_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
	${QT_QTMULTIMEDIA_LIBRARY}
	${QT_QTNETWORK_LIBRARY}
	${QT_QTWEBKIT_LIBRARY}
	${OpenCV_LIBS}
	${OPENKINECT_LIBRARY_DEBUG}
  )
ELSE(DEBUG_BUILD)
  SET(CMAKE_BUILD_TYPE "Release")
  SET(LIBS
    ${QT_QTGUI_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
	${QT_QTMULTIMEDIA_LIBRARY}
	${QT_QTNETWORK_LIBRARY}
	${QT_QTWEBKIT_LIBRARY}
	${OpenCV_LIBS}
	${OPENKINECT_LIBRARY}
  )
ENDIF(DEBUG_BUILD)

INCLUDE_DIRECTORIES(
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
  ${BRGMUTIL_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ICONV_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}/QtGui
  ${QT_INCLUDE_DIR}/QtCore
  ${QT_INCLUDE_DIR}/Qt
  ${QT_INCLUDE_DIR}/QtNetwork
  ${QT_INCLUDE_DIR}/QtOpenGL
  ${QT_INCLUDE_DIR}/QtXml
  ${QT_INCLUDE_DIR}/QtWebKit
  ${QT_INCLUDE_DIR}/QtMultimedia
  ${OpenCV_INCLUDE_DIR}
  ${OPENKINECT_INCLUDE_DIR}
  ./src
  ./
)

SET(PRG_SRCS
  src/main.cpp
  
)
 
SET(PRGS_HEADERS
  src/globals.h
  
)

SET(PRGS_UIS
  ui/MainWindow.ui
)

SET(WIDGET_SRCS
  src/MainWindow.cpp
  src/videowidget.cpp
  src/videowidgetsurface.cpp
  src/MyKinectListener.cpp
  src/FrameWindow.cpp
)

SET(WIDGET_MOC_HDRS
  src/MainWindow.h
  src/videowidget.h
  src/videowidgetsurface.h
  src/MyKinectListener.h
  src/FrameWindow.h
)  

# so that we have our application icon
SET(PRGS_RCS
     resources.rc
)

SET(PRGS_RCCS 
	resources.qrc
)


# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )

# compile resources
QT4_ADD_RESOURCES(Quinect_RCC_SRCS ${PRGS_RCCS} )

QT4_WRAP_UI(SOURCES_UIS_H ${PRGS_UIS} )

# After this call, foo_MOC_SRCS = moc_Class1.cxx moc_Class2.cxx moc_Class3.cxx.
QT4_WRAP_CPP(WIDGET_MOC_SRCS ${WIDGET_MOC_HDRS} ${SOURCES_UIS_H})

# To have executable with consoles (on windows OS)
ADD_EXECUTABLE(Quinect ${PRG_SRCS} ${PRGS_HEADERS} ${WIDGET_SRCS} ${WIDGET_MOC_SRCS} ${WIDGET_MOC_HDRS} ${PRGS_RCS} ${Quinect_RCC_SRCS} ${SOURCES_UIS_H} )

TARGET_LINK_LIBRARIES(Quinect ${LIBS})

