cmake_minimum_required(VERSION 3.16)
project(os_thread_library)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/test TEST_FILES)

find_package(Threads REQUIRED)


add_library(libcpu STATIC IMPORTED)
if (APPLE)
    add_compile_options(-D_XOPEN_SOURCE -Wno-deprecated-declarations)
    set_target_properties(libcpu PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libcpu_macos.o)
else ()
    set_target_properties(libcpu PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libcpu_linux.o)
endif ()

# Thread library build target (customize this with the source files for your thread library)
add_library(mythread STATIC ${SRC_FILES})

# Add a build target for each test program
# Find all test cpp files
file(GLOB TEST_FILES "${PROJECT_SOURCE_DIR}/test/*.cpp")

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} libcpu mythread ${CMAKE_DL_LIBS} Threads::Threads)
endforeach()
