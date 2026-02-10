cmake_minimum_required(VERSION 3.5)

project(myGatekeeper VERSION 0.1 LANGUAGES CXX)

# Habilitar AutoUic, AutoMoc y AutoRcc para gestionar automáticamente los recursos de Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Configuración del estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuración flexible para Qt5 o Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Multimedia)

# Archivos fuente del proyecto
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    VibeCheck.cpp
    VibeCheck.h
    Juego.cpp Juego.h
    LevelConfig.cpp LevelConfig.h
    Logger.cpp Logger.h
    Persona.cpp Persona.h
)

# Configuración para agregar el ejecutable en función de la versión de Qt
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(myGatekeeper
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        mujeres.qrc
        hombres.qrc
        sonidos.qrc
        otros.qrc










    )
else()
    if(ANDROID)
        add_library(myGatekeeper SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(myGatekeeper
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Enlazar las bibliotecas de Qt necesarias
target_link_libraries(myGatekeeper PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

# Configuración de propiedades para iOS/macOS y Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.myGatekeeper)
endif()
set_target_properties(myGatekeeper PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Configuración de instalación
include(GNUInstallDirs)
install(TARGETS myGatekeeper
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalización del ejecutable para Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(myGatekeeper)
endif()
