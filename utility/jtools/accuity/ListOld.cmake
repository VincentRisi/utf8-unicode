project (putty3.idl2.accuity)

set (ACCUITY_PYD_DIR "${ACCUITY_BINARY_DIR}/pyd")
file(MAKE_DIRECTORY ${ACCUITY_PYD_DIR})

if (EXISTS ${CMAKE_BINARY_DIR}/pyd/prod/pyportal.pyd)
  configure_file (${CMAKE_BINARY_DIR}/pyd/prod/pyportal.pyd ${ACCUITY_PYD_DIR}/pyportal.pyd COPYONLY)
  configure_file (${CMAKE_BINARY_DIR}/pyd/prod/pyportal.pdb ${ACCUITY_PYD_DIR}/pyportal.pdb COPYONLY)
elseif (EXISTS ${CMAKE_BINARY_DIR}/pyd/prod/pyportal.so)
  configure_file (${CMAKE_BINARY_DIR}/pyd/prod/pyportal.so ${ACCUITY_PYD_DIR}/pyportal.so COPYONLY)
endif ()  

add_subdirectory (sql_build)
add_subdirectory (idl2_build)

if (WIN32)
  include_external_msproject (
          accuity_python 
          ${ACCUITY_SOURCE_DIR}/accuity_python/accuity_python.pyproj
          TYPE "645a0c82-85bc-45ac-9b2a-098f517dbafb"
          PLATFORM "Any CPU"
          )
  set_property(TARGET accuity_python PROPERTY FOLDER "putty3/idl2/accuity/accuity_python")
  add_subdirectory (swagger_build)
endif ()

add_subdirectory (py_zip)

option (BUILD_ACCUITY_IDL2_SERVER "Use ACCUITY IDL2 SERVER Build" ON)
if (BUILD_ACCUITY_IDL2_SERVER)
  add_subdirectory (idl2server)
endif ()

add_subdirectory (httpserver)
