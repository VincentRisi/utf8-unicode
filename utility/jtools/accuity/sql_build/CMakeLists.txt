project (putty3.idl2.accuity.sql_build)

pathed(iiDir       ${ACCUITY_BINARY_DIR}/ii)
pathed(shDir       ${ACCUITY_BINARY_DIR}/dsh)
pathed(soDir       ${ACCUITY_BINARY_DIR}/so)
pathed(sqlDir      ${ACCUITY_BINARY_DIR}/sql)
pathed(binDir      ${ACCUITY_BINARY_DIR}/bin)
pathed(logDir      ${ACCUITY_BINARY_DIR}/log)
pathed(pyDir       ${ACCUITY_BINARY_DIR}/py)
pathed(pyDBADir    ${ACCUITY_BINARY_DIR}/pydba)
pathed(piDir       ${ACCUITY_BINARY_DIR}/pi)

set (switches 
  "ConnReqd=0"
  "OneSQLScript=1"
  "Internal=1"
  "I3Gen=0"
  "Python3=1"
  "XmlValue=1"
  "ControlDB=npud00"
  "UConnect=npu"
  "C|${shDir}|.sh|SH Files"
  "SO|${soDir}|.so"
  "SQL|${sqlDir}|.sql|SQL Files"
  "LogDir=${logDir}"
  "Python|${pyDir}|.py|PY_Files"
  "PyDBApi|${pyDBADir}|.py|DB_Api"
  "ParmsDir=${piDir}"
)

if (WIN32)
  pathed(csDir       ${ACCUITY_BINARY_DIR}/cs)
  pathed(swaggerDir  ${ACCUITY_BINARY_DIR}/yaml2)
  pathed(openapiDir  ${ACCUITY_BINARY_DIR}/yaml3)
  list (APPEND switches
    "CSIDL2|${csDir}|.cs|CS_Files"
    "CSNet7DIR=${csDir}"
    "Swagger|${swaggerDir}|.yaml|Swagger"
    "Openapi|${openapiDir}|.yaml|OpenApi"
    )
endif ()

set (accuity_list
  accuityroutingcode.si
  accuitycontact.si
  accuitycorrespondent.si
  accuitycorrespondentsub.si
  accuitylocation.si
  accuityofficer.si
  accuityroutingatt.si
  audits.si
  bankfile.si
)

set (accuity_sifiles)
foreach (name ${accuity_list})
  list (APPEND accuity_sifiles ${ACCUITY_SOURCE_DIR}/dsi/${name})
endforeach()

dbportal2(accuity_sql_build accuity "${accuity_sifiles}" ${switches})

set_property(TARGET accuity_sql_build  PROPERTY FOLDER "putty3/idl2/accuity/sql_build")
