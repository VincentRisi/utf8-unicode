project (Accuity_Client_AccuityMod)

include_directories (
  ${ACCUITY_SOURCE_DIR} 
  ${ACCUITY_BINARY_DIR}/idl2/client
  ${ACCUITY_BINARY_DIR}/sql/sh
  ${LIBS_SOURCE_DIR}/main
  ${LIBS_SOURCE_DIR}/main/src
  ${LIBS_SOURCE_DIR}/ubi
  ${LIBS_SOURCE_DIR}/ubi/src
  #${LIBS_SOURCE_DIR}/portal/cli9
  #${LIBS_SOURCE_DIR}/portal/cli9/src
  ${LIBS_SOURCE_DIR}/portal/jsoncpp
  ${LIBS_SOURCE_DIR}/portal/jsoncpp/json
  ${LIBS_SOURCE_DIR}/portal/jsoncpp/src
  ${LIBS_SOURCE_DIR}/portal/oci19
  ${LIBS_SOURCE_DIR}/portal/oci19/oracle
  ${LIBS_SOURCE_DIR}/portal/oci19/src
  ${PYTHON_INCLUDE_PATH}
  ${PYTHON_LIB}/site-packages/pybind11/include
  )

set (shfiles
  ${ACCUITY_BINARY_DIR}/sql/sh/accuitycontact.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/accuitycontact.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/accuitycorrespondent.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/accuitycorrespondent.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/accuitycorrespondentsub.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/accuitycorrespondentsub.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/accuitylocation.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/accuitylocation.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/accuityofficer.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/accuityofficer.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/accuityroutingatt.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/accuityroutingatt.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/accuityroutingcode.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/accuityroutingcode.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/accuityuseroverride.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/accuityuseroverride.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/audits.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/audits.sh
  ${ACCUITY_BINARY_DIR}/sql/sh/bankfile.cpp
  ${ACCUITY_BINARY_DIR}/sql/sh/bankfile.sh
  )

source_group ("SH Files"  REGULAR_EXPRESSION ".*[.]sh$")
source_group ("PY Files"  REGULAR_EXPRESSION ".*[.]py$")

message (STATUS "sh: ${shfiles}")

set (generated 
  ${ACCUITY_BINARY_DIR}/idl2/client/accuityclient.cpp
  ${ACCUITY_BINARY_DIR}/idl2/client/accuityclient.h
  ${ACCUITY_BINARY_DIR}/idl2/client/AccuityMod.cpp
  ${shfiles}
  )

set_source_files_properties (
  ${generated}
  PROPERTIES GENERATED TRUE
  )

set (source 
  #${ACCUITY_SOURCE_DIR}/runner.py
  )

add_library (AccuityMod SHARED
  ${source} 
  ${generated}
  )

if (WIN32)
  set_target_properties (AccuityMod PROPERTIES SUFFIX .pyd)
  add_definitions (/bigobj)
endif ()

target_link_libraries (AccuityMod ubi jsoncpp ws2_32 winmm ${PYTHON_BUILD_LIBRARIES})
#add_subdirectory (runpy)
