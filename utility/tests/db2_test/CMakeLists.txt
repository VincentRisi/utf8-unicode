project (tests_ocitest)

set (DB2_DIR ${BUILD_SQL_DIR}/db2/sh)

include_directories(
  ${LIBS_SOURCE_DIR}/portal/cli9/db2/win
  ${LIBS_SOURCE_DIR}/portal/cli9/src
  ${DB2_DIR}
)

set (generated 
  ${DB2_DIR}/attachmenttype.cpp
  ${DB2_DIR}/attachmenttype.sh
  ${DB2_DIR}/attachmenttype_snips.h
  ${DB2_DIR}/channeltransaction.cpp
  ${DB2_DIR}/channeltransaction.sh
  ${DB2_DIR}/channeltransaction_snips.h
  ${DB2_DIR}/diarytype.cpp
  ${DB2_DIR}/diarytype.sh
  ${DB2_DIR}/diarytype_snips.h
  ${DB2_DIR}/gatetype.cpp
  ${DB2_DIR}/gatetype.sh
  ${DB2_DIR}/gatetype_snips.h
  ${DB2_DIR}/interfacetype.cpp
  ${DB2_DIR}/interfacetype.sh
  ${DB2_DIR}/interfacetype_snips.h
  ${DB2_DIR}/message.cpp
  ${DB2_DIR}/message.sh
  ${DB2_DIR}/message_snips.h
  ${DB2_DIR}/moggy_snips.h
  ${DB2_DIR}/moggy.cpp
  ${DB2_DIR}/moggy.sh
  ${DB2_DIR}/notetype.cpp
  ${DB2_DIR}/notetype.sh
  ${DB2_DIR}/notetype_snips.h
  ${DB2_DIR}/orchdef.cpp
  ${DB2_DIR}/orchdef.sh
  ${DB2_DIR}/orchdef_snips.h
  ${DB2_DIR}/orchdefendpoint.cpp
  ${DB2_DIR}/orchdefendpoint.sh
  ${DB2_DIR}/orchdefendpoint_snips.h
  ${DB2_DIR}/orchinst.cpp
  ${DB2_DIR}/orchinst.sh
  ${DB2_DIR}/orchinst_snips.h
  ${DB2_DIR}/routehistory.cpp
  ${DB2_DIR}/routehistory.sh
  ${DB2_DIR}/routehistory_snips.h
  ${DB2_DIR}/singletrans.cpp
  ${DB2_DIR}/singletrans.sh
  ${DB2_DIR}/singletrans_snips.h
  ${DB2_DIR}/status.cpp
  ${DB2_DIR}/status.sh
  ${DB2_DIR}/status_snips.h
  ${DB2_DIR}/taskparametersconfig.cpp
  ${DB2_DIR}/taskparametersconfig.sh
  ${DB2_DIR}/taskparametersconfig_snips.h
  ${DB2_DIR}/tasktype.cpp
  ${DB2_DIR}/tasktype.sh
  ${DB2_DIR}/tasktype_snips.h
  )

source_group("generated" FILES ${generated})

set (source
  main.cpp
  )

add_executable (db2_test ${source} ${generated})

link_directories(${ORACLE_LIBRARY_DIR})
#target_link_libraries (db2_test oci_12)

set_target_properties(db2_test PROPERTIES PROJECT_LABEL tests-db2-test)
set_property(TARGET db2_test PROPERTY FOLDER ${TESTS_FOLDER}/db2_test)

