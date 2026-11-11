set(CHRONOVEX_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../..")
set(CHRONOVEX_SOURCES
    "${CHRONOVEX_ROOT}/src/pc_anim_duration_scale_u32.c"
    "${CHRONOVEX_ROOT}/src/pc_motion_duration_u32.c"
    "${CHRONOVEX_ROOT}/src/pc_time_deadline_from_now_u64.c"
    "${CHRONOVEX_ROOT}/src/pc_time_until_deadline_i64.c")
idf_component_register(SRCS ${CHRONOVEX_SOURCES}
    INCLUDE_DIRS "${CHRONOVEX_ROOT}/include"
    PRIV_INCLUDE_DIRS "${CHRONOVEX_ROOT}/include/chronovex/timing")
target_compile_options(${COMPONENT_LIB} PRIVATE -std=c99)
set_source_files_properties(${CHRONOVEX_SOURCES} PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
