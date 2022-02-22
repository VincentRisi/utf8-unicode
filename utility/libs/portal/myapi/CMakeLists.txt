project (libs-portal-myapi)

include_directories (
  ${LIBS_SOURCE_DIR}/portal/myapi
  ${LIBS_SOURCE_DIR}/portal/myapi/src
  )

add_library (myapi
    src/myapi.cpp 
    src/myapi.h
  )
  
set_target_properties(myapi PROPERTIES PROJECT_LABEL libs-portal-myapi)
set_property(TARGET myapi PROPERTY FOLDER ${LIBS_FOLDER}/portal/myapi)
