project (libs_portal_odbc_general)

include_directories (
  ${LIBS_SOURCE_DIR}/portal/odbc_general
  ${LIBS_SOURCE_DIR}/portal/odbc_general/include
  )

if (WIN32)
  include_directories (${LIBS_SOURCE_DIR}/odbc_general/win/include)
else ()
  include_directories (${LIBS_SOURCE_DIR}/odbc_general/include)
endif ()

add_library (odbc_general
  src/odbcapi.cpp
  include/dbapi.h
  include/odbcapi.h
  include/odbcmachine.h
  include/ubibasevars.h
  )

target_link_libraries (odbc_general main odbc32)

set_target_properties(odbc_general PROPERTIES PROJECT_LABEL libs-portal-odbc_general)
set_property(TARGET odbc_general PROPERTY FOLDER ${LIBS_FOLDER}/portal/odbc_general)
