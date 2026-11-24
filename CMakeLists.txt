cmake_minimum_required(VERSION 3.20)
project(Chronovex VERSION 0.2.0 LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(CHRONOVEX_BUILD_TESTS "Build Chronovex host tests" OFF)
option(CHRONOVEX_BUILD_EXAMPLES "Build Chronovex examples" OFF)
option(CHRONOVEX_BUILD_CXX_HEADER_TEST "Build optional C++ header/link smoke test" ON)
option(CHRONOVEX_STRICT_WARNINGS "Treat Chronovex target warnings as errors" ON)

set(CHRONOVEX_SOURCES
    src/anim_duration_scale_u32.c
    src/motion_duration_u32.c
    src/time_deadline_from_now_u64.c
    src/time_until_deadline_i64.c)

add_library(chronovex STATIC ${CHRONOVEX_SOURCES})
add_library(Chronovex::Chronovex ALIAS chronovex)
set_target_properties(chronovex PROPERTIES
    EXPORT_NAME Chronovex
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO)
target_compile_features(chronovex PUBLIC c_std_99)
target_include_directories(chronovex
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(CHRONOVEX_STRICT_WARNINGS)
    if(MSVC)
        target_compile_options(chronovex PRIVATE /W4 /WX)
    else()
        target_compile_options(chronovex PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endif()

install(TARGETS chronovex
    EXPORT ChronovexTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ChronovexTargets
    FILE ChronovexTargets.cmake
    NAMESPACE Chronovex::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Chronovex)

configure_package_config_file(
    cmake/ChronovexConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ChronovexConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Chronovex)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ChronovexConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ChronovexConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ChronovexConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Chronovex)

if(CHRONOVEX_BUILD_TESTS)
    enable_testing()
    foreach(test_name IN ITEMS test_basic test_implementation test_audit test_compat)
        add_executable(chronovex_${test_name} tests/${test_name}.c)
        target_link_libraries(chronovex_${test_name} PRIVATE Chronovex::Chronovex)
        set_target_properties(chronovex_${test_name} PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        if(CHRONOVEX_STRICT_WARNINGS)
            if(MSVC)
                target_compile_options(chronovex_${test_name} PRIVATE /W4 /WX /UNDEBUG)
            else()
                target_compile_options(chronovex_${test_name} PRIVATE -Wall -Wextra -Wpedantic -Werror -UNDEBUG)
            endif()
        endif()
        add_test(NAME chronovex_${test_name} COMMAND chronovex_${test_name})
    endforeach()

    set(CHRONOVEX_PUBLIC_HEADERS
        chronovex/timing/anim_duration_scale_u32.h
        chronovex/timing/motion_duration_u32.h
        chronovex/timing/time_deadline_from_now_u64.h
        chronovex/timing/time_until_deadline_i64.h)
    foreach(header IN LISTS CHRONOVEX_PUBLIC_HEADERS)
        string(REPLACE "/" "_" header_id "${header}")
        string(REPLACE ".h" "" header_id "${header_id}")
        set(smoke_source "${CMAKE_CURRENT_BINARY_DIR}/generated/${header_id}.c")
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")
        file(WRITE "${smoke_source}" "#include <${header}>\nint main(void) { return 0; }\n")
        add_executable("chronovex_header_${header_id}" "${smoke_source}")
        target_include_directories("chronovex_header_${header_id}" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        set_target_properties("chronovex_header_${header_id}" PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        if(CHRONOVEX_STRICT_WARNINGS)
            if(MSVC)
                target_compile_options("chronovex_header_${header_id}" PRIVATE /W4 /WX)
            else()
                target_compile_options("chronovex_header_${header_id}" PRIVATE -Wall -Wextra -Wpedantic -Werror)
            endif()
        endif()
        add_test(NAME "chronovex_header_${header_id}" COMMAND "chronovex_header_${header_id}")
    endforeach()

    if(CHRONOVEX_BUILD_CXX_HEADER_TEST)
        include(CheckLanguage)
        check_language(CXX)
        if(CMAKE_CXX_COMPILER)
            enable_language(CXX)
            add_executable(chronovex_test_cpp_header tests/test_cpp_header.cpp)
            target_link_libraries(chronovex_test_cpp_header PRIVATE Chronovex::Chronovex)
            set_target_properties(chronovex_test_cpp_header PROPERTIES CXX_STANDARD 98 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
            if(CHRONOVEX_STRICT_WARNINGS)
                if(MSVC)
                    target_compile_options(chronovex_test_cpp_header PRIVATE /W4 /WX)
                else()
                    target_compile_options(chronovex_test_cpp_header PRIVATE -Wall -Wextra -Wpedantic -Werror)
                endif()
            endif()
            add_test(NAME chronovex_test_cpp_header COMMAND chronovex_test_cpp_header)
        endif()
    endif()
endif()

if(CHRONOVEX_BUILD_EXAMPLES)
    enable_testing()
    add_executable(chronovex_example_quick_start examples/quick_start.c)
    target_link_libraries(chronovex_example_quick_start PRIVATE Chronovex::Chronovex)
    set_target_properties(chronovex_example_quick_start PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    if(CHRONOVEX_STRICT_WARNINGS)
        if(MSVC)
            target_compile_options(chronovex_example_quick_start PRIVATE /W4 /WX)
        else()
            target_compile_options(chronovex_example_quick_start PRIVATE -Wall -Wextra -Wpedantic -Werror)
        endif()
    endif()
    add_test(NAME chronovex_example_quick_start COMMAND chronovex_example_quick_start)
endif()
