project (libs-http)

set (source
  src/httpserver.cpp
  src/httpserver.h
  src/httprequest.cpp
  src/httprequest.h
  src/httpxcept.cpp
  src/httpxcept.h
  )

ADD_LIBRARY(http
   ${source}
)
target_link_libraries (http jsoncpp)

set_target_properties(http PROPERTIES PROJECT_LABEL libs-http)
set_property(TARGET http PROPERTY FOLDER ${LIBS_FOLDER}/http)
