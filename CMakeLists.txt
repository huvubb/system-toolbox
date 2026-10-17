cmake_minimum_required(VERSION 3.16)
project(system_toolbox LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "System Toolbox requires Windows")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(Threads REQUIRED)
include(CTest)

add_executable(system-toolbox system-toolbox.cpp)
add_custom_command(TARGET system-toolbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/music-converter/music_unlock" "$<TARGET_FILE_DIR:system-toolbox>/music-converter/music_unlock"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/music-converter/assets" "$<TARGET_FILE_DIR:system-toolbox>/music-converter/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/music-converter/unlocker.py" "$<TARGET_FILE_DIR:system-toolbox>/music-converter/unlocker.py"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/music-converter/LICENSE" "$<TARGET_FILE_DIR:system-toolbox>/music-converter/LICENSE"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/music-converter/UPSTREAM.md" "$<TARGET_FILE_DIR:system-toolbox>/music-converter/UPSTREAM.md")
set(toolbox_targets system-toolbox)
if(BUILD_TESTING)
    add_executable(toolbox-tests tests/toolbox-tests.cpp)
    list(APPEND toolbox_targets toolbox-tests)
    add_test(NAME toolbox-regression COMMAND toolbox-tests)
    set_tests_properties(toolbox-regression PROPERTIES TIMEOUT 60)
endif()

foreach(target IN LISTS toolbox_targets)
    target_link_libraries(${target} PRIVATE Threads::Threads ws2_32 shell32 advapi32
        ole32 crypt32 opengl32 d3d11 dxgi gdi32 user32)
    if(MSVC)
        target_compile_options(${target} PRIVATE /utf-8 /W3)
    else()
        target_compile_options(${target} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8
            -Wall -Wextra -Wno-unknown-pragmas -Wno-missing-field-initializers)
        if(MINGW)
            target_link_options(${target} PRIVATE -static)
        endif()
    endif()
endforeach()
