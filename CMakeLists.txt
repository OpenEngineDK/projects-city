# Project name. Should be the same as the project directory name and
# will become the name of the binary target.
SET( PROJECT_NAME "city")

# Project source code list
SET( PROJECT_SOURCES
  # Add all the cpp source files here
  main.cpp
  RenderStateHandler.h
  City.cpp
  City.h
  CityGrid.cpp
  CityGrid.h
  Building.cpp
  Building.h
  CityAnimator.cpp
  CityAnimator.h
  Echo.h
  Echo.cpp
  MainUI.cpp
)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

QT4_WRAP_UI(UI_SOURCE
  MainUI.ui
)
QT4_WRAP_CPP(MOC_SOURCE
  MainUI.h
)



# Include needed to use SDL under Mac OS X
IF(APPLE)
  SET(PROJECT_SOURCES ${PROJECT_SOURCES}  ${SDL_MAIN_FOR_MAC})
ENDIF(APPLE)

# Project executable
ADD_EXECUTABLE(${PROJECT_NAME}
  ${PROJECT_SOURCES}
  ${MOC_SOURCE}
  ${UI_SOURCE}
)

# Project dependencies
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  # Core library dependencies
  OpenEngine_Core
  OpenEngine_Logging
  OpenEngine_Display
  OpenEngine_Devices
  OpenEngine_Scene
  # Extension dependencies
  Extensions_SDL
  Extensions_Qt
  Extensions_SetupHelpers
  Extensions_GenericHandlers
  Extensions_FaceBuilder
  Extensions_TinyScheme
  Extensions_Sockets
  Extensions_IRC
)
