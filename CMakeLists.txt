cmake_minimum_required(VERSION 3.20)
project(chronovex VERSION 0.1.0 LANGUAGES C)

option(CHRONOVEX_BUILD_TESTS "Build Chronovex tests" ON)
option(CHRONOVEX_STRICT_WARNINGS "Treat compiler warnings as errors" ON)

set(CHRONOVEX_SOURCES
    src/pc_anim_duration_scale_u32.c
    src/pc_motion_duration_u32.c
    src/pc_time_deadline_from_now_u64.c
    src/pc_time_until_deadline_i64.c)

add_library(chronovex STATIC ${CHRONOVEX_SOURCES})
target_include_directories(chronovex
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/chronovex/timing)
set_target_properties(chronovex PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO)
target_compile_features(chronovex PUBLIC c_std_99)

if(CHRONOVEX_STRICT_WARNINGS)
    if(MSVC)
        target_compile_options(chronovex PRIVATE /W4 /WX)
    else()
        target_compile_options(chronovex PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endif()

if(CHRONOVEX_BUILD_TESTS)
    enable_testing()
    foreach(test_name IN ITEMS test_basic test_implementation test_audit)
        add_executable(chronovex_${test_name} tests/${test_name}.c)
        target_link_libraries(chronovex_${test_name} PRIVATE chronovex)
        set_target_properties(chronovex_${test_name} PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        if(CHRONOVEX_STRICT_WARNINGS)
            if(MSVC)
                target_compile_options(chronovex_${test_name} PRIVATE /W4 /WX)
            else()
                target_compile_options(chronovex_${test_name} PRIVATE -Wall -Wextra -Wpedantic -Werror)
            endif()
        endif()
        add_test(NAME ${test_name} COMMAND chronovex_${test_name})
    endforeach()
endif()
