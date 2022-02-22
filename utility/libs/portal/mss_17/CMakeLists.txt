project (libs_portal_mss_17)

include_directories (
  ${LIBS_SOURCE_DIR}/portal/mss_17
  ${LIBS_SOURCE_DIR}/portal/mss_17/include
  )

if (WIN32)
  include_directories (${LIBS_SOURCE_DIR}/portal/mss_17/win/include)
else ()
  include_directories (${LIBS_SOURCE_DIR}/portal/mss_17/include)
endif ()

add_library (mss_17
  src/mssapi.cpp
  include/dbapi.h
  include/mssapi.h
  include/mssmachine.h
  include/ubibasevars.h
  )

set_target_properties(mss_17 PROPERTIES PROJECT_LABEL libs-portal-mss_17)
set_property(TARGET mss_17 PROPERTY FOLDER ${LIBS_FOLDER}/portal/mss_17)
