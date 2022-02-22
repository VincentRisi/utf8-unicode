project (libs_portal_oci_18)

include (oci.cmake)

include_directories(
  ${LIBS_SOURCE_DIR}/portal/oci_18/oracle
  ${LIBS_SOURCE_DIR}/portal/oci_18/src
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

add_library(oci_18
  src/ociapi.cpp
  src/ocinumber.cpp
  ${headers}
)

target_link_libraries (oci_18 main ${ocilibs})

set_target_properties(oci_18 PROPERTIES PROJECT_LABEL libs-portal-oci_18)
set_property(TARGET oci_18 PROPERTY FOLDER ${LIBS_FOLDER}/portal/oci_18)
