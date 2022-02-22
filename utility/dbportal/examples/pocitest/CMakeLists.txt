project (dbportal_pocitest)

include(${CMAKE_SOURCE_DIR}/cmake/dbportal.cmake)

pathed(binDir      ${CMAKE_CURRENT_BINARY_DIR}/bin)
pathed(csDir       ${CMAKE_CURRENT_BINARY_DIR}/cs)
pathed(csadoDir    ${CMAKE_CURRENT_BINARY_DIR}/csado)
pathed(csnet7Dir   ${CMAKE_CURRENT_BINARY_DIR}/csnet7)
pathed(csrwDir     ${CMAKE_CURRENT_BINARY_DIR}/csrw)
pathed(iiDir       ${CMAKE_CURRENT_BINARY_DIR}/ii)
pathed(logDir      ${CMAKE_CURRENT_BINARY_DIR}/log)
pathed(piDir       ${CMAKE_CURRENT_BINARY_DIR}/pi)
pathed(pyDir       ${CMAKE_CURRENT_BINARY_DIR}/py)
pathed(pyDBApiDir  ${CMAKE_CURRENT_BINARY_DIR}/pyDBApi)
pathed(shDir       ${CMAKE_CURRENT_BINARY_DIR}/sh)
pathed(soDir       ${CMAKE_CURRENT_BINARY_DIR}/so)
pathed(sqlDir      ${CMAKE_CURRENT_BINARY_DIR}/sql)

set (switches
  "OneSQLScript=1"
  "Internal=1"
  "UseCSFields=1"
  "ControlDB=nard00"
  "UConnect=nar"
  "Python3=1"
  "LogDir=${logDir}"
  "C|${shDir}|.sh|SH Files|.*[.]sh$"
  "IDL|${iiDir}|.ii|IDL Files|.*[.]ii$"
  "SO|${soDir}|.so"
  "SQL|${sqlDir}|.sql|SQL Files|.*[.]sql$"
  "Python|${pyDir}|.py|Python"
  "PyDBApi|${pyDBApiDir}|.py|PyDBApi"
  "CSIDL2|${csDir}|.cs|CSIdl2"
  "CSAdoNet|${csadoDir}|.cs|CSAdoNet"
  "CSNet7|${csnet7Dir}|.cs|CSNet7"
  "CSRW|${csrwDir}|.cs|CSRW"
  "PARMS|${piDir}|.pi|PI Files|.*[.]pi$"
)

set (list
  queue nednotes render report zedzed audittrail comments
)

set (sifiles)
foreach (name ${list})
  list (APPEND sifiles ${CMAKE_CURRENT_SOURCE_DIR}/${name}.si)
endforeach ()

dbportal(dbportal_pocitest agile "${sifiles}" ${switches})

add_dependencies (dbportal_pocitest pocioci pocilink)

if (WIN32)
  set (DEBUG "/DEBUG")
else ()
  set (DEBUG)
endif ()

#add_custom_target (dbportal_pocitest_pocidump ALL
#  DEPENDS pocidump
#  COMMAND ${pocidump} ${soDir}/zedzed.so
#  )
