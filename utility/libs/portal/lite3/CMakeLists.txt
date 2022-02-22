project (libs-portal-lite3)

include_directories (
  ${LIBS_SOURCE_DIR}/portal/lite3
  ${LIBS_SOURCE_DIR}/portal/lite3/src
  )

add_library (lite3
  src/lite3api.cpp
  src/lite3api.h
  src/sqlite3.c
  src/sqlite3.h
  src/sqlite3ext.h
  )

set_target_properties(lite3 PROPERTIES PROJECT_LABEL libs-portal-lite3)
set_property(TARGET lite3 PROPERTY FOLDER ${LIBS_FOLDER}/portal/lite3)


