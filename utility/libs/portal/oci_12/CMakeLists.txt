project (libs_portal_oci_12)

include (oci.cmake)

include_directories(
  ${LIBS_SOURCE_DIR}/portal/oci_12/oracle
  ${LIBS_SOURCE_DIR}/portal/oci_12/src
)

set (headers
  src/csvbuilder.h
  src/databuilder.h
  src/databuilder2.h
  src/databuilderjp.h
  src/ociapi.h
  src/ocimachine.h
  src/ocinumber.h
  src/popgen.h
  src/ubibasevars.h
)

add_library(oci_12
  src/ociapi.cpp
  src/ocinumber.cpp
  ${headers}
)

target_link_libraries (oci_12 main ${ocilibs})

set_target_properties(oci_12 PROPERTIES PROJECT_LABEL libs-portal-oci_12)
set_property(TARGET oci_12 PROPERTY FOLDER ${LIBS_FOLDER}/portal/oci_12)
