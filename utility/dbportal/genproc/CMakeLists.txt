project (dbportal_genproc)

set (SRC_DIR ${PORTAL_SOURCE_DIR})
set (BIN_DIR ${CMAKE_BINARY_DIR}/pocioci)

include_directories (
  ${SRC_DIR} 
  ${BIN_DIR}
)

set (source
  ${SRC_DIR}/loadso.cpp
  ${SRC_DIR}/genproc.cpp
  )

add_executable(genproc ${source})

target_link_libraries (genproc poci)
