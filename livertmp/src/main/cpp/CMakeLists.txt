cmake_minimum_required(VERSION 3.4.1)
message("构建类型：${CMAKE_BUILD_TYPE}")
message("构建目录：${CMAKE_SOURCE_DIR}")

#rtmp库
add_subdirectory(librtmp)
#日志库
add_subdirectory(log)

#x264库
#在x264目录下新建一个CMakeLists.txt的方法，行不通，只能在这里直接写。
add_library(myx264
        STATIC
        IMPORTED)
set_target_properties(
        myx264
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/x264/lib/${ANDROID_ABI}/libx264.a
)

aux_source_directory(core myCore)
#核心逻辑库

#头文件
include_directories(
        ${CMAKE_SOURCE_DIR}/x264/include
        ${CMAKE_SOURCE_DIR}/core/include
        ${CMAKE_SOURCE_DIR}/log/include
        ${CMAKE_SOURCE_DIR}/librtmp
)


#定义hw-rtmp库的源文件
add_library(
        hw-rtmp
        SHARED
        ${myCore}
        api_hw_jni.cpp
)

#连接
target_link_libraries(
        hw-rtmp
        rtmp
        myLog
        myx264)