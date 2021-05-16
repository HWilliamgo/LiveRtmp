cmake_minimum_required(VERSION 3.4.1)
#在librtmp目录中已经定义编译出静态库rtmp
add_subdirectory(librtmp)
add_library( # Sets the name of the library.
        hw-rtmp

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        rtmp_jni.cpp
        rtmp_wrap.cpp)
find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        hw-rtmp

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        rtmp)