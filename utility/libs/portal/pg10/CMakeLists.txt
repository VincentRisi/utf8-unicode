project (misc_tests.portal.pg10)

include_directories (
  ${PORTAL_SOURCE_DIR}/pg10/pgsql/include
  ${PORTAL_SOURCE_DIR}/pg10/src
)
  
add_library (pg10
  ${PORTAL_SOURCE_DIR}/pg10/src/pgapi.cpp
  ${PORTAL_SOURCE_DIR}/pg10/src/pgaspect.cpp
  ${PORTAL_SOURCE_DIR}/pg10/src/pgapi.h
  ${PORTAL_SOURCE_DIR}/pg10/src/pgaspect.h
)
  
set_target_properties (pg10 PROPERTIES PROJECT_LABEL misc_tests_portal_pg10)
set_property(TARGET pg10 PROPERTY FOLDER "misc/tests/portal/pg10")
  