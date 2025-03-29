project(nitroinspireface)
cmake_minimum_required(VERSION 3.9.0)

set(PACKAGE_NAME nitroinspireface)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)

# Define C++ library and add all sources
add_library(${PACKAGE_NAME} SHARED src/main/cpp/cpp-adapter.cpp ../cpp/NitroInspireFace.cpp ../cpp/NitroSession.cpp ../cpp/ImageStream.cpp)

add_library(inspireface SHARED IMPORTED)
set_target_properties(inspireface
  PROPERTIES
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libInspireFace.so )

# Add Nitrogen specs :)
include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/nitroinspireface+autolinking.cmake)

# Set up local includes
include_directories("src/main/cpp" "../cpp" "libs/include")

find_library(LOG_LIB log)

# Link all libraries together
target_link_libraries(
        ${PACKAGE_NAME}
        ${LOG_LIB}
        android # <-- Android core
        inspireface # <-- InspireFace
)
