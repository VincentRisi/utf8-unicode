project (libs-jsoncpp)

set (jsoncpp_source
  json/allocator.h
  json/assertions.h
  json/autolink.h
  json/config.h
  json/features.h
  json/forwards.h
  json/json.h
  json/reader.h
  json/value.h
  json/version.h
  json/writer.h
  src/json_tool.h
  src/json_reader.cpp
  src/json_valueiterator.inl
  src/json_value.cpp
  src/json_writer.cpp
  src/json_databuild.cpp
  src/json_databuild.h
  src/json_databuild2.cpp
  src/json_databuildjp.cpp
  )

add_library(jsoncpp ${jsoncpp_source})
target_link_libraries (jsoncpp main)

set_target_properties(jsoncpp PROPERTIES PROJECT_LABEL libs-jsoncpp)
set_property(TARGET jsoncpp PROPERTY FOLDER ${LIBS_FOLDER}/jsoncpp)

