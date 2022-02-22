project (utility_dbportal_pocioci)

add_definitions (-DYYERROR_VERBOSE)

set (source_dir ${DBPORTAL_SOURCE_DIR}/code)
set (binary_dir ${DBPORTAL_BINARY_DIR})

include_directories (
  ${source_dir} 
)

set (sqldefIn
  ${source_dir}/build.py    
  ${source_dir}/sqldef.y 
)

set (sqldefOut      
  ${source_dir}/sqldef.cpp 
  ${source_dir}/sqldef.h
  ${source_dir}/yytok.cpp
)
set_source_files_properties(${sqldefOut} PROPERTIES GENERATED TRUE)

set (source
  ${sqldefIn}   
  ${sqldefOut}
  )

set (cpp_list
  cleanup genarchive gencommn gencsadonet gencsidl2 gencsnet7 gencsrw genidl genjportal
  genoci genopenapi genparm genpydbapi genpython genvbcode3 genvbforador genvbforidl
  genvbnet7 genswagger lex
)

foreach (name ${cpp_list})
  list (APPEND source ${source_dir}/${name}.cpp)
endforeach ()

add_executable (pocioci ${source} ${source_dir}/pocimain.cpp)
set_target_properties (pocioci PROPERTIES PROJECT_LABEL dbportal_pocioci)
target_link_libraries (pocioci poci)

if (WIN32)
  log_info ("COMMAND ${PYTHON_EXECUTABLE} ${source_dir}/build.py ${source_dir} ${source_dir}")
  add_custom_command (TARGET pocioci PRE_BUILD
    COMMAND ${PYTHON_EXECUTABLE} ${source_dir}/build.py ${source_dir} ${source_dir}
    BYPRODUCTS ${sqldefOut}
    )
endif ()  
