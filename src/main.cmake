
set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")
# set(CMAKE_EXE_LINKER_FLAGS "-nostdlib")
# set(CMAKE_C_FLAGS "-static")
add_executable(out "${CMAKE_CURRENT_LIST_DIR}/main.c")
