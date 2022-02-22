project (misc_tests.portal.pg13)

include_directories (
  ${PORTAL_SOURCE_DIR}/pg13/pgsql/include
  ${PORTAL_SOURCE_DIR}/pg13/src
)
  
add_library (pg13
  ${PORTAL_SOURCE_DIR}/pg13/src/pgapi.cpp
  ${PORTAL_SOURCE_DIR}/pg13/src/pgaspect.cpp
  ${PORTAL_SOURCE_DIR}/pg13/src/pgapi.h
  ${PORTAL_SOURCE_DIR}/pg13/src/pgaspect.h
)
  
set_target_properties (pg13 PROPERTIES PROJECT_LABEL misc_tests_portal_pg13)
set_property(TARGET pg13 PROPERTY FOLDER "misc/tests/portal/pg13")
  