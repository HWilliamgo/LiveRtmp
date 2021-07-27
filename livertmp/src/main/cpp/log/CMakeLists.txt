AUX_SOURCE_DIRECTORY(. log_source)
include_directories(include)
find_library( # Sets the name of the path variable.
        android-log-lib
        log)
add_library(
        myLog
        STATIC
        ${log_source})

target_link_libraries( # Specifies the target library.
        myLog
        ${android-log-lib})