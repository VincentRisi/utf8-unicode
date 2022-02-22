project (libs-portal-oci_19)

include (oci.cmake)

include_directories(
  ${LIBS_SOURCE_DIR}/portal/oci_19/oracle
  ${LIBS_SOURCE_DIR}/portal/oci_19/src
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

add_library(oci_19 SHARED
  src/ociapi.cpp
  src/ocinumber.cpp
  ${headers}
)

target_link_libraries (oci_19 main ${ocilibs})

set_target_properties(oci_19 PROPERTIES PROJECT_LABEL libs-portal-oci_19)
set_property(TARGET oci_19 PROPERTY FOLDER ${LIBS_FOLDER}/portal/oci_19)
