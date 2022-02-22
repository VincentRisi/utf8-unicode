project (dbportal_pocilink)

include_directories (
  ${DBPORTAL_SOURCE_DIR}/code
)

set (cpp_list
  linkmain slink
  )

foreach (name ${cpp_list})
  list (APPEND source ${DBPORTAL_SOURCE_DIR}/code/${name}.cpp)
endforeach ()

add_executable(pocilink ${source})
target_link_libraries (pocilink poci)

set_target_properties (pocilink PROPERTIES PROJECT_LABEL dbportal_pocilink)
