cmake_minimum_required(VERSION 3.4.1)
#在librtmp目录中已经定义编译出静态库rtmp
add_subdirectory(librtmp)
add_subdirectory(log)
add_library( # Sets the name of the library.
        hw-rtmp

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        rtmp_jni.cpp
        rtmp_wrap.cpp)


target_link_libraries( # Specifies the target library.
        hw-rtmp
        myLog
        rtmp)