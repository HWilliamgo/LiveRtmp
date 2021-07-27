cmake_minimum_required(VERSION 3.4.1)
message("构建类型：${CMAKE_BUILD_TYPE}")
message("构建目录：${CMAKE_SOURCE_DIR}")

add_subdirectory(librtmp)
add_subdirectory(log)

#如果在x264目录下新建一个CMakeLists.txt的方法，行不通，只能在这里直接写。
add_library(myx264
        STATIC
        IMPORTED)
set_target_properties(
        myx264
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/x264/lib/${ANDROID_ABI}/libx264.a
)
include_directories(${CMAKE_SOURCE_DIR}/x264/include)

include_directories(include)

add_library( # Sets the name of the library.
        hw-rtmp

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        rtmp_jni.cpp
        rtmp_wrap.cpp)


target_link_libraries( # Specifies the target library.
        hw-rtmp
        rtmp
        myLog
        myx264)