project (libs-portal-cli9)

include_directories(
  ${LIBS_SOURCE_DIR}/portal/cli9
  ${LIBS_SOURCE_DIR}/portal/cli9/src
  ${LIBS_SOURCE_DIR}/portal/cli9/db2
  ${LIBS_SOURCE_DIR}/portal/cli9/db2/win
  )

set (source 
  src/cliapi.cpp
  src/cliapi.h
  src/climachine.h
)

add_library(cli9 ${source})

target_link_libraries (cli9 main)

set_target_properties(cli9 PROPERTIES PROJECT_LABEL libs-portal-cli9)
set_property(TARGET cli9 PROPERTY FOLDER ${LIBS_FOLDER}/portal/cli9)
