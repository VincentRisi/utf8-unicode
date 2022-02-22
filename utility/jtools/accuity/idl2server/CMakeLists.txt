project (putty3.idl2.accuity.idl2server)

include_directories (
  ${PYTHON_DIR}
  ${PYTHON_INCLUDE_DIR}
  ${ACCUITY_SOURCE_DIR}
  ${ACCUITY_BINARY_DIR}/server/idl
  ${ACCUITY_BINARY_DIR}/sh
  ${PUTTY3_SOURCE_DIR}/idl2/src
  ${LIBS_SOURCE_DIR}
  ${LIBS_SOURCE_DIR}/loki/include
  ${LIBS_SOURCE_DIR}/idl2/src
  ${LIBS_SOURCE_DIR}/nsc/src
  ${LIBS_SOURCE_DIR}/nsc/sh
  ${LIBS_SOURCE_DIR}/jsoncpp
  ${LIBS_SOURCE_DIR}/jsoncpp/src
  ${PUTTY3_INCLUDE_DIR}
  )

include (${CMAKE_CURRENT_SOURCE_DIR}/inifile.cmake)

set (source
  ${ACCUITY_BINARY_DIR}/server/idl/accuity.h
  ${ACCUITY_BINARY_DIR}/server/idl/accuityimpl.cpp
  ${ACCUITY_BINARY_DIR}/server/idl/accuityReqID.txt
  ${ACCUITY_BINARY_DIR}/server/idl/accuityserver.cpp
  ${ACCUITY_BINARY_DIR}/server/py/ACCUITY_IMPL.py
  ${ACCUITY_BINARY_DIR}/server/py/ACCUITY_JSON.py
  ${ACCUITY_BINARY_DIR}/server/py/ACCUITY_STRUCTS.py
  ${ACCUITY_BINARY_DIR}/server/py/INTRINSICS.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYCONTACT.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYCORRESPONDENT.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYCORRESPONDENTSUB.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYLOCATION.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYOFFICER.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYROUTINGATT.py
  ${ACCUITY_BINARY_DIR}/py/DB_ACCUITYROUTINGCODE.py
  ${ACCUITY_BINARY_DIR}/py/DB_AUDITS.py
  ${ACCUITY_BINARY_DIR}/py/DB_BANKFILE.py
  ${outfile}
)

set_source_files_properties (${source} PROPERTIES GENERATED TRUE)

list (APPEND source 
  ${PUTTY3_SOURCE_DIR}/idl2/src/putty3shell.cpp
  ${PUTTY3_SOURCE_DIR}/idl2/src/putty3shell.h
  ${PUTTY3_SOURCE_DIR}/idl2/src/HTTPSERVER_UTIL.py 
  )

if (WIN32)
  set (winlibs ws2_32 winmm)
else ()  
  set (winlibs)
endif ()

add_executable (accuityserver ${source})
add_dependencies (accuityserver accuity_idl2_build)

target_link_libraries (accuityserver ${bbdlibs} ${ocilibs} ${winlibs} ${bbdextra} idl2 nsc jsoncpp ${PYTHONLIBS})

set_target_properties (accuityserver PROPERTIES PROJECT_LABEL putty3_idl2_accuity_idl2server)
set_property(TARGET accuityserver PROPERTY FOLDER "putty3/idl2/accuity/idl2server")
