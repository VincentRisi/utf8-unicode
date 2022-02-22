project (utility_dbportal)

set (EXECUTABLE_OUTPUT_PATH "${DBPORTAL_BINARY_DIR}/bin")
set (LIBRARY_OUTPUT_PATH    "${DBPORTAL_BINARY_DIR}/lib")

add_subdirectory (pocilib)
add_subdirectory (pocioci)
add_subdirectory (pocilink)
add_subdirectory (pocidump)
