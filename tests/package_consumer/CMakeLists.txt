cmake_minimum_required(VERSION 3.20)
project(ChronovexInstalledConsumer LANGUAGES C)
enable_testing()

find_package(Chronovex 0.2 CONFIG REQUIRED)

add_executable(chronovex_installed_consumer main.c)
target_link_libraries(chronovex_installed_consumer PRIVATE Chronovex::Chronovex)
set_target_properties(chronovex_installed_consumer PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
add_test(NAME chronovex_installed_consumer COMMAND chronovex_installed_consumer)
