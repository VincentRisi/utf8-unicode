project (tests_odbc_oracle_test)

set (ODBC_ORACLE_DIR ${BUILD_SQL_DIR}/odbc/oracle)

include_directories(
  ${LIBS_SOURCE_DIR}/portal/odbc_general/include
  ${LIBS_SOURCE_DIR}/utfpp
  ${ODBC_ORACLE_DIR}
)

add_definitions (
  -DDRIVER="Oracle 12 ODBC 64"
  -DUSERID="bbdmsg"
  -DPWD="oracle"
  )

set (generated 
  ${ODBC_ORACLE_DIR}/areacode.cpp
  ${ODBC_ORACLE_DIR}/areacode.sh
  ${ODBC_ORACLE_DIR}/areacode_snips.h
  ${ODBC_ORACLE_DIR}/assignhist.cpp
  ${ODBC_ORACLE_DIR}/assignhist.sh
  ${ODBC_ORACLE_DIR}/assignhist_snips.h
  ${ODBC_ORACLE_DIR}/attachment.cpp
  ${ODBC_ORACLE_DIR}/attachment.sh
  ${ODBC_ORACLE_DIR}/attachmentlocation.cpp
  ${ODBC_ORACLE_DIR}/attachmentlocation.sh
  ${ODBC_ORACLE_DIR}/attachmentlocation_snips.h
  ${ODBC_ORACLE_DIR}/attachmenttype.cpp
  ${ODBC_ORACLE_DIR}/attachmenttype.sh
  ${ODBC_ORACLE_DIR}/attachmenttype_snips.h
  ${ODBC_ORACLE_DIR}/attachment_snips.h
  ${ODBC_ORACLE_DIR}/availablestep.cpp
  ${ODBC_ORACLE_DIR}/availablestep.sh
  ${ODBC_ORACLE_DIR}/availablestep_snips.h
  ${ODBC_ORACLE_DIR}/caseactivity.cpp
  ${ODBC_ORACLE_DIR}/caseactivity.sh
  ${ODBC_ORACLE_DIR}/caseactivity_snips.h
  ${ODBC_ORACLE_DIR}/casedependency.cpp
  ${ODBC_ORACLE_DIR}/casedependency.sh
  ${ODBC_ORACLE_DIR}/casedependency_snips.h
  ${ODBC_ORACLE_DIR}/casereference.cpp
  ${ODBC_ORACLE_DIR}/casereference.sh
  ${ODBC_ORACLE_DIR}/casereferencetype.cpp
  ${ODBC_ORACLE_DIR}/casereferencetype.sh
  ${ODBC_ORACLE_DIR}/casereferencetype_snips.h
  ${ODBC_ORACLE_DIR}/casereference_snips.h
  ${ODBC_ORACLE_DIR}/cases.cpp
  ${ODBC_ORACLE_DIR}/cases.sh
  ${ODBC_ORACLE_DIR}/casesarch.cpp
  ${ODBC_ORACLE_DIR}/casesarch.sh
  ${ODBC_ORACLE_DIR}/casesarch_snips.h
  ${ODBC_ORACLE_DIR}/cases_snips.h
  ${ODBC_ORACLE_DIR}/caseuser.cpp
  ${ODBC_ORACLE_DIR}/caseuser.sh
  ${ODBC_ORACLE_DIR}/caseuser_snips.h
  ${ODBC_ORACLE_DIR}/channeltransaction.cpp
  ${ODBC_ORACLE_DIR}/channeltransaction.sh
  ${ODBC_ORACLE_DIR}/channeltransaction_snips.h
  ${ODBC_ORACLE_DIR}/datamapper.cpp
  ${ODBC_ORACLE_DIR}/datamapper.sh
  ${ODBC_ORACLE_DIR}/datamapper_snips.h
  ${ODBC_ORACLE_DIR}/diarytype.cpp
  ${ODBC_ORACLE_DIR}/diarytype.sh
  ${ODBC_ORACLE_DIR}/diarytype_snips.h
  ${ODBC_ORACLE_DIR}/discardreason.cpp
  ${ODBC_ORACLE_DIR}/discardreason.sh
  ${ODBC_ORACLE_DIR}/discardreason_snips.h
  ${ODBC_ORACLE_DIR}/gaterule.cpp
  ${ODBC_ORACLE_DIR}/gaterule.sh
  ${ODBC_ORACLE_DIR}/gaterule_snips.h
  ${ODBC_ORACLE_DIR}/gatetype.cpp
  ${ODBC_ORACLE_DIR}/gatetype.sh
  ${ODBC_ORACLE_DIR}/gatetype_snips.h
  ${ODBC_ORACLE_DIR}/interfacetype.cpp
  ${ODBC_ORACLE_DIR}/interfacetype.sh
  ${ODBC_ORACLE_DIR}/interfacetype_snips.h
  ${ODBC_ORACLE_DIR}/locks.cpp
  ${ODBC_ORACLE_DIR}/locks.sh
  ${ODBC_ORACLE_DIR}/locks_snips.h
  ${ODBC_ORACLE_DIR}/message.cpp
  ${ODBC_ORACLE_DIR}/message.sh
  ${ODBC_ORACLE_DIR}/message_snips.h
  ${ODBC_ORACLE_DIR}/moggy_snips.h
  ${ODBC_ORACLE_DIR}/moggy.cpp
  ${ODBC_ORACLE_DIR}/moggy.sh
  ${ODBC_ORACLE_DIR}/moggyutf8_snips.h
  ${ODBC_ORACLE_DIR}/moggyutf8.cpp
  ${ODBC_ORACLE_DIR}/moggyutf8.sh
  ${ODBC_ORACLE_DIR}/moggyunicode_snips.h
  ${ODBC_ORACLE_DIR}/moggyunicode.cpp
  ${ODBC_ORACLE_DIR}/moggyunicode.sh
  ${ODBC_ORACLE_DIR}/note.cpp
  ${ODBC_ORACLE_DIR}/note.sh
  ${ODBC_ORACLE_DIR}/notetype.cpp
  ${ODBC_ORACLE_DIR}/notetype.sh
  ${ODBC_ORACLE_DIR}/notetype_snips.h
  ${ODBC_ORACLE_DIR}/note_snips.h
  ${ODBC_ORACLE_DIR}/objecttype.cpp
  ${ODBC_ORACLE_DIR}/objecttype.sh
  ${ODBC_ORACLE_DIR}/objecttype_snips.h
  ${ODBC_ORACLE_DIR}/orchdef.cpp
  ${ODBC_ORACLE_DIR}/orchdef.sh
  ${ODBC_ORACLE_DIR}/orchdefendpoint.cpp
  ${ODBC_ORACLE_DIR}/orchdefendpoint.sh
  ${ODBC_ORACLE_DIR}/orchdefendpoint_snips.h
  ${ODBC_ORACLE_DIR}/orchdef_snips.h
  ${ODBC_ORACLE_DIR}/orchinst.cpp
  ${ODBC_ORACLE_DIR}/orchinst.sh
  ${ODBC_ORACLE_DIR}/orchinst_snips.h
  ${ODBC_ORACLE_DIR}/pendreason.cpp
  ${ODBC_ORACLE_DIR}/pendreason.sh
  ${ODBC_ORACLE_DIR}/pendreasonrule.cpp
  ${ODBC_ORACLE_DIR}/pendreasonrule.sh
  ${ODBC_ORACLE_DIR}/pendreasonrulecondition.cpp
  ${ODBC_ORACLE_DIR}/pendreasonrulecondition.sh
  ${ODBC_ORACLE_DIR}/pendreasonrulecondition_snips.h
  ${ODBC_ORACLE_DIR}/pendreasonrule_snips.h
  ${ODBC_ORACLE_DIR}/pendreasonusergroup.cpp
  ${ODBC_ORACLE_DIR}/pendreasonusergroup.sh
  ${ODBC_ORACLE_DIR}/pendreasonusergroup_snips.h
  ${ODBC_ORACLE_DIR}/pendreason_snips.h
  ${ODBC_ORACLE_DIR}/proccategory.cpp
  ${ODBC_ORACLE_DIR}/proccategory.sh
  ${ODBC_ORACLE_DIR}/proccategory_snips.h
  ${ODBC_ORACLE_DIR}/procdef.cpp
  ${ODBC_ORACLE_DIR}/procdef.sh
  ${ODBC_ORACLE_DIR}/procdefusergroup.cpp
  ${ODBC_ORACLE_DIR}/procdefusergroup.sh
  ${ODBC_ORACLE_DIR}/procdefusergroup_snips.h
  ${ODBC_ORACLE_DIR}/procdef_snips.h
  ${ODBC_ORACLE_DIR}/process.cpp
  ${ODBC_ORACLE_DIR}/process.sh
  ${ODBC_ORACLE_DIR}/processarch.cpp
  ${ODBC_ORACLE_DIR}/processarch.sh
  ${ODBC_ORACLE_DIR}/processarch_snips.h
  ${ODBC_ORACLE_DIR}/processdata.cpp
  ${ODBC_ORACLE_DIR}/processdata.sh
  ${ODBC_ORACLE_DIR}/processdatadef.cpp
  ${ODBC_ORACLE_DIR}/processdatadef.sh
  ${ODBC_ORACLE_DIR}/processdatadef_snips.h
  ${ODBC_ORACLE_DIR}/processdataext.cpp
  ${ODBC_ORACLE_DIR}/processdataext.sh
  ${ODBC_ORACLE_DIR}/processdataext_snips.h
  ${ODBC_ORACLE_DIR}/processdata_snips.h
  ${ODBC_ORACLE_DIR}/process_snips.h
  ${ODBC_ORACLE_DIR}/proctype.cpp
  ${ODBC_ORACLE_DIR}/proctype.sh
  ${ODBC_ORACLE_DIR}/proctypegroup.cpp
  ${ODBC_ORACLE_DIR}/proctypegroup.sh
  ${ODBC_ORACLE_DIR}/proctypegrouping.cpp
  ${ODBC_ORACLE_DIR}/proctypegrouping.sh
  ${ODBC_ORACLE_DIR}/proctypegrouping_snips.h
  ${ODBC_ORACLE_DIR}/proctypegroup_snips.h
  ${ODBC_ORACLE_DIR}/proctype_snips.h
  ${ODBC_ORACLE_DIR}/pubsublist.cpp
  ${ODBC_ORACLE_DIR}/pubsublist.sh
  ${ODBC_ORACLE_DIR}/pubsublist_snips.h
  ${ODBC_ORACLE_DIR}/queryattributes.cpp
  ${ODBC_ORACLE_DIR}/queryattributes.sh
  ${ODBC_ORACLE_DIR}/queryattributes_snips.h
  ${ODBC_ORACLE_DIR}/queryclassifications.cpp
  ${ODBC_ORACLE_DIR}/queryclassifications.sh
  ${ODBC_ORACLE_DIR}/queryclassifications_snips.h
  ${ODBC_ORACLE_DIR}/queryproducts.cpp
  ${ODBC_ORACLE_DIR}/queryproducts.sh
  ${ODBC_ORACLE_DIR}/queryproducts_snips.h
  ${ODBC_ORACLE_DIR}/reclassify.cpp
  ${ODBC_ORACLE_DIR}/reclassify.sh
  ${ODBC_ORACLE_DIR}/reclassify_snips.h
  ${ODBC_ORACLE_DIR}/relatedcases.cpp
  ${ODBC_ORACLE_DIR}/relatedcases.sh
  ${ODBC_ORACLE_DIR}/relatedcases_snips.h
  ${ODBC_ORACLE_DIR}/relatereason.cpp
  ${ODBC_ORACLE_DIR}/relatereason.sh
  ${ODBC_ORACLE_DIR}/relatereason_snips.h
  ${ODBC_ORACLE_DIR}/relationtype.cpp
  ${ODBC_ORACLE_DIR}/relationtype.sh
  ${ODBC_ORACLE_DIR}/relationtype_snips.h
  ${ODBC_ORACLE_DIR}/routehistory.cpp
  ${ODBC_ORACLE_DIR}/routehistory.sh
  ${ODBC_ORACLE_DIR}/routehistory_snips.h
  ${ODBC_ORACLE_DIR}/searchcase.cpp
  ${ODBC_ORACLE_DIR}/searchcase.sh
  ${ODBC_ORACLE_DIR}/searchcasearch.cpp
  ${ODBC_ORACLE_DIR}/searchcasearch.sh
  ${ODBC_ORACLE_DIR}/searchcasearch_snips.h
  ${ODBC_ORACLE_DIR}/searchcase_snips.h
  ${ODBC_ORACLE_DIR}/securitygroup.cpp
  ${ODBC_ORACLE_DIR}/securitygroup.sh
  ${ODBC_ORACLE_DIR}/securitygrouptasklink.cpp
  ${ODBC_ORACLE_DIR}/securitygrouptasklink.sh
  ${ODBC_ORACLE_DIR}/securitygrouptasklink_snips.h
  ${ODBC_ORACLE_DIR}/securitygroup_snips.h
  ${ODBC_ORACLE_DIR}/serviceclient.cpp
  ${ODBC_ORACLE_DIR}/serviceclient.sh
  ${ODBC_ORACLE_DIR}/serviceclienttaskmapping.cpp
  ${ODBC_ORACLE_DIR}/serviceclienttaskmapping.sh
  ${ODBC_ORACLE_DIR}/serviceclienttaskmapping_snips.h
  ${ODBC_ORACLE_DIR}/serviceclient_snips.h
  ${ODBC_ORACLE_DIR}/singletrans.cpp
  ${ODBC_ORACLE_DIR}/singletrans.sh
  ${ODBC_ORACLE_DIR}/singletrans_snips.h
  ${ODBC_ORACLE_DIR}/status.cpp
  ${ODBC_ORACLE_DIR}/status.sh
  ${ODBC_ORACLE_DIR}/status_snips.h
  ${ODBC_ORACLE_DIR}/step.cpp
  ${ODBC_ORACLE_DIR}/step.sh
  ${ODBC_ORACLE_DIR}/stepactivity.cpp
  ${ODBC_ORACLE_DIR}/stepactivity.sh
  ${ODBC_ORACLE_DIR}/stepactivity_snips.h
  ${ODBC_ORACLE_DIR}/stepdiary.cpp
  ${ODBC_ORACLE_DIR}/stepdiary.sh
  ${ODBC_ORACLE_DIR}/stepdiary_snips.h
  ${ODBC_ORACLE_DIR}/stepescalation.cpp
  ${ODBC_ORACLE_DIR}/stepescalation.sh
  ${ODBC_ORACLE_DIR}/stepescalation_snips.h
  ${ODBC_ORACLE_DIR}/stepusergroup.cpp
  ${ODBC_ORACLE_DIR}/stepusergroup.sh
  ${ODBC_ORACLE_DIR}/stepusergroup_snips.h
  ${ODBC_ORACLE_DIR}/step_snips.h
  ${ODBC_ORACLE_DIR}/task.cpp
  ${ODBC_ORACLE_DIR}/task.sh
  ${ODBC_ORACLE_DIR}/taskgate.cpp
  ${ODBC_ORACLE_DIR}/taskgate.sh
  ${ODBC_ORACLE_DIR}/taskgatein.cpp
  ${ODBC_ORACLE_DIR}/taskgatein.sh
  ${ODBC_ORACLE_DIR}/taskgatein_snips.h
  ${ODBC_ORACLE_DIR}/taskgate_snips.h
  ${ODBC_ORACLE_DIR}/taskparameters.cpp
  ${ODBC_ORACLE_DIR}/taskparameters.sh
  ${ODBC_ORACLE_DIR}/taskparametersconfig.cpp
  ${ODBC_ORACLE_DIR}/taskparametersconfig.sh
  ${ODBC_ORACLE_DIR}/taskparametersconfig_snips.h
  ${ODBC_ORACLE_DIR}/taskparameters_snips.h
  ${ODBC_ORACLE_DIR}/tasktype.cpp
  ${ODBC_ORACLE_DIR}/tasktype.sh
  ${ODBC_ORACLE_DIR}/tasktype_snips.h
  ${ODBC_ORACLE_DIR}/taskusergroup.cpp
  ${ODBC_ORACLE_DIR}/taskusergroup.sh
  ${ODBC_ORACLE_DIR}/taskusergroup_snips.h
  ${ODBC_ORACLE_DIR}/task_snips.h
  ${ODBC_ORACLE_DIR}/usedsecuritygroup.cpp
  ${ODBC_ORACLE_DIR}/usedsecuritygroup.sh
  ${ODBC_ORACLE_DIR}/usedsecuritygroup_snips.h
  ${ODBC_ORACLE_DIR}/userarealink.cpp
  ${ODBC_ORACLE_DIR}/userarealink.sh
  ${ODBC_ORACLE_DIR}/userarealink_snips.h
  ${ODBC_ORACLE_DIR}/usergroup.cpp
  ${ODBC_ORACLE_DIR}/usergroup.sh
  ${ODBC_ORACLE_DIR}/usergrouplink.cpp
  ${ODBC_ORACLE_DIR}/usergrouplink.sh
  ${ODBC_ORACLE_DIR}/usergrouplink_snips.h
  ${ODBC_ORACLE_DIR}/usergroup_snips.h
  ${ODBC_ORACLE_DIR}/userhierarchy.cpp
  ${ODBC_ORACLE_DIR}/userhierarchy.sh
  ${ODBC_ORACLE_DIR}/userhierarchy_snips.h
  ${ODBC_ORACLE_DIR}/users.cpp
  ${ODBC_ORACLE_DIR}/users.sh
  ${ODBC_ORACLE_DIR}/users_snips.h
  ${ODBC_ORACLE_DIR}/wfmessagetracing.cpp
  ${ODBC_ORACLE_DIR}/wfmessagetracing.sh
  ${ODBC_ORACLE_DIR}/wfmessagetracing_snips.h
  )

source_group("generated" FILES ${generated})

set (source
  main.cpp
  )

add_executable (odbc_oracle_test ${source} ${generated})

target_link_libraries (odbc_oracle_test odbc_general winmm)

set_target_properties(odbc_oracle_test PROPERTIES PROJECT_LABEL tests_odbc_oracle_test)
set_property(TARGET odbc_oracle_test PROPERTY FOLDER ${TESTS_FOLDER}/odbc_oracle_test)

set (generated
  ${ODBC_ORACLE_DIR}/moggy2.cpp
  ${ODBC_ORACLE_DIR}/moggy2.sh
  ${ODBC_ORACLE_DIR}/moggy2_snips.h
  )

source_group("generated" FILES ${generated})

set (source
  main_blob.cpp
  )

add_executable (odbc_oracle_blob_test ${source} ${generated})

target_link_libraries (odbc_oracle_blob_test odbc_general winmm)

set_target_properties(odbc_oracle_blob_test PROPERTIES PROJECT_LABEL tests_odbc_oracle_blob_test)
set_property(TARGET odbc_oracle_blob_test PROPERTY FOLDER ${TESTS_FOLDER}/odbc_oracle_blob_test)
