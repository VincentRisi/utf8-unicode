project (dbportal_poci2j)

set (SRC_DIR ${PORTAL_SOURCE_DIR})
set (LAST_DIR ${SRC_DIR}/last_built)

include_directories (
  ${SRC_DIR}
  ${LAST_DIR} 
 )

set (cpp_list
  jportal
  )

foreach (name ${cpp_list})
  list (APPEND source ${SRC_DIR}/${name}.cpp)
endforeach ()

add_executable(poci2j ${source})
target_link_libraries (poci2j poci)

set_target_properties (poci2j PROPERTIES PROJECT_LABEL dbportal_poci2j)
