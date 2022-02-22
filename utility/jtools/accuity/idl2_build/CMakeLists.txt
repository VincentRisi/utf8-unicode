project (putty3.idl2.accuity.idl2_build)

pathed (genServerDir     ${ACCUITY_BINARY_DIR}/server/idl)
pathed (genPythonDir     ${ACCUITY_BINARY_DIR}/server/py)
pathed (genPythonTreeDir ${ACCUITY_BINARY_DIR}/tree)
pathed (genHttpServerDir ${ACCUITY_BINARY_DIR}/http)

set (switches
  "${genServerDir}|PopGenServer|Server Files"
  "${genHttpServerDir}|PopHTTPServerFull|HTTP Server Files"
  "${genHttpServerDir}|PopHTTPSwaggerFull|HTTP Swagger Files"
  "${genHttpServerDir}|PopHTTPOpenApiFull|HTTP Swagger Files"
  "${genHttpServerDir}|PopHTTPPython|HTTP Swagger Files"
  "${genPythonDir}|PopGenPython|PY_Files"
  "${genPythonTreeDir}|PopGenPythonTree|PY_Files"
)

if (WIN32)
  pathed (genCSharpDir     ${ACCUITY_BINARY_DIR}/client/cs)
  list (APPEND switches
    "${genCSharpDir}|PopGenCSharp|CSharp Files"
    )
endif ()

set (configFile ${genHttpServerDir}/accuity.properties)
file (WRITE ${configFile})
file (APPEND ${configFile} "urlPrefix=${ACCUITY_BINARY_DIR}\n")
file (APPEND ${configFile} "defSqlSub=/yaml2\n")
file (APPEND ${configFile} "compSqlSub=/yaml3\n")

set (idlFile ${CMAKE_CURRENT_SOURCE_DIR}/accuity.idl2)
idl2_simple(accuity_idl2_build ${idlFile} ${switches})

add_dependencies (accuity_idl2_build accuity_sql_build)
set_property(TARGET accuity_idl2_build PROPERTY FOLDER "putty3/idl2/accuity/idl2_build")
