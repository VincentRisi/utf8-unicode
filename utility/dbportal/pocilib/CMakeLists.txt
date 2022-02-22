project (utility_dbportal_pocilib)

include_directories (
  ${DBPORTAL_SOURCE_DIR}/code 
)

set (source)

set (cpp_list
  binio date fndir fnext fnmerge fnname fnsplit getargs sqlbin sqltok 
  xcept xstring xtalloc yyerror
  )

foreach (name ${cpp_list})
  list (APPEND source ${DBPORTAL_SOURCE_DIR}/code/${name}.cpp)
endforeach ()

set (h_list
  addlist binio date dir getargs machine sqlbin sqlcons sqltok xcept 
  xdir xstring xtalloc yyerror
  )

foreach (name ${cpp_list})
  list (APPEND source ${DBPORTAL_SOURCE_DIR}/code/${name}.cpp)
endforeach ()

foreach (name ${h_list})
  list (APPEND source ${DBPORTAL_SOURCE_DIR}/code/${name}.h)
endforeach ()

add_library(poci ${source})
