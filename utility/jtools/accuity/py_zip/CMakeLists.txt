project (putty3_idl2_accuity_py_zip)

#we use DBPORTAL.py as INTRINSICS.py
set (ACCUITY_OUTPUT_PATH "${ACCUITY_BINARY_DIR}/pyd")
file(MAKE_DIRECTORY ${ACCUITY_OUTPUT_PATH})

configure_file (${PUTTY3_SOURCE_DIR}/router3/intrsys/DBPORTAL.py ${ACCUITY_BINARY_DIR}/server/py/INTRINSICS.py COPYONLY)

if (WIN32)
  if (EXISTS ${PUTTY3_SOURCE_DIR}/build/pyd/prod/pyportal.pyd)
    configure_file (${PUTTY3_SOURCE_DIR}/build/pyd/prod/pyportal.pyd ${ACCUITY_OUTPUT_PATH}/pyportal.pyd COPYONLY)
  endif ()  
endif ()  

set (source
  ${PUTTY3_SOURCE_DIR}/idl2/src/HTTPSERVER_UTIL.py 
  ${ACCUITY_BINARY_DIR}/server/py/INTRINSICS.py
  )

set (genned
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYROUTINGCODE.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYCONTACT.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYCORRESPONDENT.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYCORRESPONDENTSUB.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYLOCATION.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYOFFICER.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYROUTINGATT.py
  ${ACCUITY_BINARY_DIR}/py/DB_AUDITS.py
  ${ACCUITY_BINARY_DIR}/py/DB_BANKFILE.py
  ${ACCUITY_BINARY_DIR}/server/py/ACCUITY_IMPL.py
  ${ACCUITY_BINARY_DIR}/server/py/ACCUITY_JSON.py
  ${ACCUITY_BINARY_DIR}/server/py/ACCUITY_STRUCTS.py
  )

set_source_files_properties (${genned} PROPERTIES GENERATED TRUE)

list (APPEND source ${genned})

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/input.lst "${source}")

source_group ("Python Files" FILES ${source})

message (STATUS "${PYTHON_EXECUTABLE}")
set (logFile ${CMAKE_CURRENT_BINARY_DIR}/accuity_py_zip.log)
add_custom_command (
  OUTPUT ${logFile}
  COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_DIR}/pyscripts/python_zip_maker.py
          -i ${CMAKE_CURRENT_BINARY_DIR}/input.lst
          -l ${logFile}
          -n accuity 
          -o ${ACCUITY_OUTPUT_PATH} 
          -s -a 
  COMMAND ${CMAKE_COMMAND} -E copy
          ${ACCUITY_OUTPUT_PATH}/accuity.zip
          ${CMAKE_BINARY_DIR}/pyd/accuity.zip     
  DEPENDS ${source} accuity_sql_build # pyportal
  )
add_custom_target (putty3_accuity_zip ALL
  DEPENDS ${logFile}
  SOURCES ${source} ${logFile}
  )

set_property(TARGET putty3_accuity_zip PROPERTY FOLDER "putty3/idl2/accuity/py_zip")
