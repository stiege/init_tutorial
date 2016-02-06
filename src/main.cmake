
set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")
add_executable(out
    "${CMAKE_CURRENT_LIST_DIR}/main.c"
    "${CMAKE_CURRENT_LIST_DIR}/requires_init.c")
