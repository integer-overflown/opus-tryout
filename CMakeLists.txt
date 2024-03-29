cmake_minimum_required(VERSION 3.19)
project(opus_tryout)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(lib/opus)
add_executable(opus_tryout main.cpp audio_pipeline.cpp audio_pipeline.h audio_opus_pipeline.cpp audio_opus_pipeline.h buffered_device.cpp buffered_device.h transforming_device.cpp transforming_device.h pipe_device.h pipe_device.cpp)

target_link_libraries(opus_tryout Opus::opus)
target_include_directories(opus_tryout PUBLIC ${CMAKE_SOURCE_DIR}/lib/opus/include)

find_package(Qt5 COMPONENTS Core Multimedia REQUIRED)
target_link_libraries(opus_tryout Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Multimedia)
