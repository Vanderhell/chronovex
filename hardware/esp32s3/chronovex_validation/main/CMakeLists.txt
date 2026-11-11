idf_component_register(SRCS "main.c" REQUIRES chronovex)
set_source_files_properties("${CMAKE_CURRENT_LIST_DIR}/main.c" PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
