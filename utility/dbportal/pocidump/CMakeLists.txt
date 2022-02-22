project (dbportal_pocidump)

include_directories (
  ${DBPORTAL_SOURCE_DIR}/code 
)

set (source
  ${DBPORTAL_SOURCE_DIR}/code/dumpmain.cpp
  )

add_executable(pocidump ${source})

target_link_libraries (pocidump poci)

