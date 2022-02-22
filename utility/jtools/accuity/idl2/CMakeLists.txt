project (putty3.idl2.accuity.idl2_build)

pathed (genServerDir     ${ACCUITY_BINARY_DIR}/server/idl)
pathed (genPythonDir     ${ACCUITY_BINARY_DIR}/server/py)
pathed (genCSharpDir     ${ACCUITY_BINARY_DIR}/client/cs)
pathed (genHttpServerDir ${ACCUITY_BINARY_DIR}/http)

set (switches
  "${genServerDir}|PopGenServer|Server Files"
  "${genCSharpDir}|PopGenCSharp|CSharp Files"
  "${genHttpServerDir}|PopHTTPServer|HTTP Server Files"
  "${genHttpServerDir}|PopHTTPSwagger|HTTP Swagger Files"
  "${genPythonDir}|PopGenPython|PY_Files"
)

set (idlFile ${CMAKE_CURRENT_SOURCE_DIR}/accuity.idl2)

idl2_simple(accuity_idl2_build ${idlFile} ${switches})

add_dependencies (accuity_idl2_build accuity_sql_build)
set_property(TARGET accuity_idl2_build PROPERTY FOLDER "putty3/idl2/accuity/idl2_build")
