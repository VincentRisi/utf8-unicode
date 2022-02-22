project (libs-ubi)

include_directories (
  ${LIBS_SOURCE_DIR}/ubi
  ${LIBS_SOURCE_DIR}/ubi/src
  ${LIBS_SOURCE_DIR}/main
  ${LIBS_SOURCE_DIR}/main/src
  )

set (source
  src/metrics.cpp
  src/metrics.h
  src/ubiclient.cpp
  src/ubiclient.h
  src/ubirpcclient.cpp
  src/ubirpcclient.h
  src/ubirpcheader.h
  src/ubidata.cpp
  src/ubidata.h
  src/ubixcept.h
  )

ADD_LIBRARY(ubi
   ${source}
)
target_link_libraries (ubi main)

set_target_properties(ubi PROPERTIES PROJECT_LABEL libs-ubi)
set_property(TARGET ubi PROPERTY FOLDER ${LIBS_FOLDER}/ubi)
