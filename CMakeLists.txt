cmake_minimum_required(VERSION 3.19)
project(opus_tryout)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(opus_tryout main.cpp sound_encoder.cpp sound_encoder.h sound_decoder.cpp sound_decoder.h audio_pipeline.cpp audio_pipeline.h audio_opus_pipeline.cpp audio_opus_pipeline.h)
target_link_libraries(opus_tryout ${OPUS_LIB_PATH})
find_package(Qt5 COMPONENTS Core Multimedia REQUIRED)
target_link_libraries(opus_tryout Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Multimedia)
