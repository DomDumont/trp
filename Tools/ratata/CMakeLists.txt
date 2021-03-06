project (Ratata)

cmake_minimum_required (VERSION 2.8.3)

#cmake_policy(SET CMP0020 OLD)

if (WIN32)
	set(CMAKE_PREFIX_PATH C:/Qt/5.6/msvc2015_64/lib/cmake)
	SET(MY_QT_DLL_DIR C:/Qt/5.6/msvc2015_64/bin)
endif(WIN32)

if (APPLE)
	set(CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
	#set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt5/5.5.1/lib/cmake")
	# /usr/local/Cellar/qt5/5.5.1_1
endif(APPLE)

if (UNIX)
	set(QMAKESPEC linux-clang)
endif(UNIX)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Xml)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Xml_INCLUDE_DIRS})

# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})
add_definitions(${Qt5Xml_DEFINITIONS})

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Wml_EXECUTABLE_COMPILE_FLAGS}")



set(RATATA_CLASSES
    highlighter.h
    mdimainwindow.h
    mdichild.h
    projectwidget.h
    watchwidget.h
    callstackwidget.h
    outputwidget.h
    consolewidget.h
    dlgnewproject.h
    tcpclient.h
    errorwidget.h
    prefdialog.h
    breakpoint.h
    breakpointwidget.h
    myapp.h
    grepwidget.h
	)


set(RATATA_SRC
	main.cpp 
	mdimainwindow.cpp
	mdichild.cpp
	myapp.cpp
	projectwidget.cpp
	watchwidget.cpp
	errorwidget.cpp
	breakpointwidget.cpp
	outputwidget.cpp
	callstackwidget.cpp
	
	breakpoint.cpp
	grepwidget.cpp
	highlighter.cpp
	prefdialog.cpp
	tcpclient.cpp
	dlgnewproject.cpp
	consolewidget.cpp
	)


QT5_WRAP_CPP(RATATA_MOC_SRC ${RATATA_CLASSES})

# Tell CMake to create the helloworld executable
add_executable(Ratata ${RATATA_SRC}	${RATATA_MOC_SRC})

#Link the helloworld executable to the Qt 5 widgets library.
target_link_libraries(Ratata Qt5::Core Qt5::Widgets Qt5::Network Qt5::Xml)



IF(WIN32)
    INSTALL(TARGETS Ratata DESTINATION Ratata)
	INSTALL(FILES
    "${MY_QT_DLL_DIR}/Qt5Cored.dll"
	"${MY_QT_DLL_DIR}/Qt5Widgetsd.dll"
	"${MY_QT_DLL_DIR}/Qt5Guid.dll"
	"${MY_QT_DLL_DIR}/Qt5Networkd.dll"
	"${MY_QT_DLL_DIR}/Qt5Xmld.dll"
    DESTINATION Ratata
    )
	INSTALL(FILES
    "${MY_QT_DLL_DIR}/Qt5Core.dll"
	"${MY_QT_DLL_DIR}/Qt5Widgets.dll"
	"${MY_QT_DLL_DIR}/Qt5Gui.dll"
	"${MY_QT_DLL_DIR}/Qt5Network.dll"
	"${MY_QT_DLL_DIR}/Qt5Xml.dll"
    DESTINATION Ratata
    )


ENDIF(WIN32)