project (libs-portal-pg8)

include_directories (
  ${LIBS_SOURCE_DIR}/portal/pg8
  ${LIBS_SOURCE_DIR}/portal/pg8/src
  )

add_library (pg8
    src/pgapi.cpp
    src/pgapi.h
    src/pgaspect.cpp
    src/pgaspect.h
    src/pgmachine.h
  )
  
set_target_properties(pg8 PROPERTIES PROJECT_LABEL libs-portal-pg8)
set_property(TARGET pg8 PROPERTY FOLDER ${LIBS_FOLDER}/portal/pg8)
