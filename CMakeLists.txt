cmake_minimum_required(VERSION 3.14)
project(DicomViewer VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#1. Raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(raylib)

# 2. TinyFileDialogs
message(STATUS "Baixando tinyfiledialogs...")
file(DOWNLOAD 
    https://raw.githubusercontent.com/native-toolkit/tinyfiledialogs/master/tinyfiledialogs.c
    ${CMAKE_BINARY_DIR}/tinyfiledialogs.c
)
file(DOWNLOAD 
    https://raw.githubusercontent.com/native-toolkit/tinyfiledialogs/master/tinyfiledialogs.h
    ${CMAKE_BINARY_DIR}/tinyfiledialogs.h
)

find_package(DCMTK CONFIG REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/view.cpp
    src/archive_service.cpp
    src/dicom_loader.cpp
    ${CMAKE_BINARY_DIR}/tinyfiledialogs.c
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    src 
    ${CMAKE_BINARY_DIR} 
    ${DCMTK_INCLUDE_DIRS} 
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    raylib 
    dcmjpeg ijg8 ijg12 ijg16
    ${DCMTK_LIBRARIES}
)

# Para evitar que o windows abra um terminal atrás
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()