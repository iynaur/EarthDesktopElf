cmake_minimum_required(VERSION 3.2)

project(earth)

set(CMAKE_AUTOMOC ON) # For meta object compiler
set(CMAKE_AUTORCC ON) # Resource files
set(CMAKE_AUTOUIC ON) # UI files

# Find the QtWidgets library
find_package(Qt5 COMPONENTS
             Core
             Widgets
             Gui
             OpenGL
             #QtOpenGL 
             REQUIRED)

find_package (VTK REQUIRED)
find_package (PCL 1.8 REQUIRED)
#find_package(OpenCV 3 REQUIRED)


# Fix a compilation bug under ubuntu 16.04 (Xenial)
list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")

#PCL
include_directories (${PCL_INCLUDE_DIRS})
include_directories (${OpenCV_INCLUDE_DIRS})
link_directories    (${PCL_LIBRARY_DIRS})
add_definitions     (${PCL_DEFINITIONS})

#find_package(LibXml2 REQUIRED)
#include_directories(${LIBXML2_INCLUDE_DIR})



add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 14)
ADD_DEFINITIONS (${QT_DEFINITIONS})

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath=.")


set(CPPS

  common.cpp
  earth.cpp
  trackball.cpp
)

set(HEADS


)

include_directories(${PROJECT_SOURCE_DIR}/common/include)


include_directories(
${PROJECT_SOURCE_DIR}/include)

set(COMMON_HEAD

  )

set(COMMON_CPP


  )

IF( "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Release")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Release")
    MESSAGE( STATUS "CMAKE_BUILD_TYPE set ${CMAKE_BUILD_TYPE}" )
ELSEIF( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Debug")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Debug")
    MESSAGE( STATUS "CMAKE_BUILD_TYPE set ${CMAKE_BUILD_TYPE}" )
ELSE()
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
    MESSAGE( STATUS "CMAKE_BUILD_TYPE not set yet ${CMAKE_BUILD_TYPE}" )
ENDIF()

qt5_add_resources(RESOURCES resources.qrc)
FIND_PACKAGE(OpenGL)

add_executable(${PROJECT_NAME} main.cpp
    ${RESOURCES} ${CPPS} ${HEADERS} ${FORMS}
#    ${DEPRECATED} ${HARDWARE} ${CAM_LIST} ${HANDEYE_LIST}
    ${COMMON_CPP}
    )



target_link_libraries(  ${PROJECT_NAME} ${QT_LIBRARIES} ${PCL_LIBRARIES} ${OpenCV_LIBS}
${OPENGL_LIBRARIES}
    )


qt5_use_modules(${PROJECT_NAME} Core Widgets Gui
  OpenGL
  )


