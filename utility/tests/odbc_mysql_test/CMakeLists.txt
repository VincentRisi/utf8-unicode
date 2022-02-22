project (tests_odbc_postgre_test)

set (ODBC_POSTGRE_DIR ${BUILD_SQL_DIR}/odbc/postgre)

include_directories(
  ${LIBS_SOURCE_DIR}/portal/odbc_general/include
  ${ODBC_POSTGRE_DIR}
)

add_definitions (
  -DDRIVER="Postgre 14 ODBC 64"
  -DUSERID="bbdmsg"
  -DPWD="postgre"
  )

set (generated 
  ${ODBC_POSTGRE_DIR}/areacode.cpp
  ${ODBC_POSTGRE_DIR}/areacode.sh
  ${ODBC_POSTGRE_DIR}/areacode_snips.h
  ${ODBC_POSTGRE_DIR}/assignhist.cpp
  ${ODBC_POSTGRE_DIR}/assignhist.sh
  ${ODBC_POSTGRE_DIR}/assignhist_snips.h
  ${ODBC_POSTGRE_DIR}/attachment.cpp
  ${ODBC_POSTGRE_DIR}/attachment.sh
  ${ODBC_POSTGRE_DIR}/attachmentlocation.cpp
  ${ODBC_POSTGRE_DIR}/attachmentlocation.sh
  ${ODBC_POSTGRE_DIR}/attachmentlocation_snips.h
  ${ODBC_POSTGRE_DIR}/attachmenttype.cpp
  ${ODBC_POSTGRE_DIR}/attachmenttype.sh
  ${ODBC_POSTGRE_DIR}/attachmenttype_snips.h
  ${ODBC_POSTGRE_DIR}/attachment_snips.h
  ${ODBC_POSTGRE_DIR}/availablestep.cpp
  ${ODBC_POSTGRE_DIR}/availablestep.sh
  ${ODBC_POSTGRE_DIR}/availablestep_snips.h
  ${ODBC_POSTGRE_DIR}/caseactivity.cpp
  ${ODBC_POSTGRE_DIR}/caseactivity.sh
  ${ODBC_POSTGRE_DIR}/caseactivity_snips.h
  ${ODBC_POSTGRE_DIR}/casedependency.cpp
  ${ODBC_POSTGRE_DIR}/casedependency.sh
  ${ODBC_POSTGRE_DIR}/casedependency_snips.h
  ${ODBC_POSTGRE_DIR}/casereference.cpp
  ${ODBC_POSTGRE_DIR}/casereference.sh
  ${ODBC_POSTGRE_DIR}/casereferencetype.cpp
  ${ODBC_POSTGRE_DIR}/casereferencetype.sh
  ${ODBC_POSTGRE_DIR}/casereferencetype_snips.h
  ${ODBC_POSTGRE_DIR}/casereference_snips.h
  ${ODBC_POSTGRE_DIR}/cases.cpp
  ${ODBC_POSTGRE_DIR}/cases.sh
  ${ODBC_POSTGRE_DIR}/casesarch.cpp
  ${ODBC_POSTGRE_DIR}/casesarch.sh
  ${ODBC_POSTGRE_DIR}/casesarch_snips.h
  ${ODBC_POSTGRE_DIR}/cases_snips.h
  ${ODBC_POSTGRE_DIR}/caseuser.cpp
  ${ODBC_POSTGRE_DIR}/caseuser.sh
  ${ODBC_POSTGRE_DIR}/caseuser_snips.h
  ${ODBC_POSTGRE_DIR}/channeltransaction.cpp
  ${ODBC_POSTGRE_DIR}/channeltransaction.sh
  ${ODBC_POSTGRE_DIR}/channeltransaction_snips.h
  ${ODBC_POSTGRE_DIR}/datamapper.cpp
  ${ODBC_POSTGRE_DIR}/datamapper.sh
  ${ODBC_POSTGRE_DIR}/datamapper_snips.h
  ${ODBC_POSTGRE_DIR}/diarytype.cpp
  ${ODBC_POSTGRE_DIR}/diarytype.sh
  ${ODBC_POSTGRE_DIR}/diarytype_snips.h
  ${ODBC_POSTGRE_DIR}/discardreason.cpp
  ${ODBC_POSTGRE_DIR}/discardreason.sh
  ${ODBC_POSTGRE_DIR}/discardreason_snips.h
  ${ODBC_POSTGRE_DIR}/gaterule.cpp
  ${ODBC_POSTGRE_DIR}/gaterule.sh
  ${ODBC_POSTGRE_DIR}/gaterule_snips.h
  ${ODBC_POSTGRE_DIR}/gatetype.cpp
  ${ODBC_POSTGRE_DIR}/gatetype.sh
  ${ODBC_POSTGRE_DIR}/gatetype_snips.h
  ${ODBC_POSTGRE_DIR}/interfacetype.cpp
  ${ODBC_POSTGRE_DIR}/interfacetype.sh
  ${ODBC_POSTGRE_DIR}/interfacetype_snips.h
  ${ODBC_POSTGRE_DIR}/locks.cpp
  ${ODBC_POSTGRE_DIR}/locks.sh
  ${ODBC_POSTGRE_DIR}/locks_snips.h
  ${ODBC_POSTGRE_DIR}/message.cpp
  ${ODBC_POSTGRE_DIR}/message.sh
  ${ODBC_POSTGRE_DIR}/message_snips.h
  ${ODBC_POSTGRE_DIR}/moggy.cpp
  ${ODBC_POSTGRE_DIR}/moggy.sh
  ${ODBC_POSTGRE_DIR}/moggy_snips.h
  ${ODBC_POSTGRE_DIR}/note.cpp
  ${ODBC_POSTGRE_DIR}/note.sh
  ${ODBC_POSTGRE_DIR}/notetype.cpp
  ${ODBC_POSTGRE_DIR}/notetype.sh
  ${ODBC_POSTGRE_DIR}/notetype_snips.h
  ${ODBC_POSTGRE_DIR}/note_snips.h
  ${ODBC_POSTGRE_DIR}/objecttype.cpp
  ${ODBC_POSTGRE_DIR}/objecttype.sh
  ${ODBC_POSTGRE_DIR}/objecttype_snips.h
  ${ODBC_POSTGRE_DIR}/orchdef.cpp
  ${ODBC_POSTGRE_DIR}/orchdef.sh
  ${ODBC_POSTGRE_DIR}/orchdefendpoint.cpp
  ${ODBC_POSTGRE_DIR}/orchdefendpoint.sh
  ${ODBC_POSTGRE_DIR}/orchdefendpoint_snips.h
  ${ODBC_POSTGRE_DIR}/orchdef_snips.h
  ${ODBC_POSTGRE_DIR}/orchinst.cpp
  ${ODBC_POSTGRE_DIR}/orchinst.sh
  ${ODBC_POSTGRE_DIR}/orchinst_snips.h
  ${ODBC_POSTGRE_DIR}/pendreason.cpp
  ${ODBC_POSTGRE_DIR}/pendreason.sh
  ${ODBC_POSTGRE_DIR}/pendreasonrule.cpp
  ${ODBC_POSTGRE_DIR}/pendreasonrule.sh
  ${ODBC_POSTGRE_DIR}/pendreasonrulecondition.cpp
  ${ODBC_POSTGRE_DIR}/pendreasonrulecondition.sh
  ${ODBC_POSTGRE_DIR}/pendreasonrulecondition_snips.h
  ${ODBC_POSTGRE_DIR}/pendreasonrule_snips.h
  ${ODBC_POSTGRE_DIR}/pendreasonusergroup.cpp
  ${ODBC_POSTGRE_DIR}/pendreasonusergroup.sh
  ${ODBC_POSTGRE_DIR}/pendreasonusergroup_snips.h
  ${ODBC_POSTGRE_DIR}/pendreason_snips.h
  ${ODBC_POSTGRE_DIR}/proccategory.cpp
  ${ODBC_POSTGRE_DIR}/proccategory.sh
  ${ODBC_POSTGRE_DIR}/proccategory_snips.h
  ${ODBC_POSTGRE_DIR}/procdef.cpp
  ${ODBC_POSTGRE_DIR}/procdef.sh
  ${ODBC_POSTGRE_DIR}/procdefusergroup.cpp
  ${ODBC_POSTGRE_DIR}/procdefusergroup.sh
  ${ODBC_POSTGRE_DIR}/procdefusergroup_snips.h
  ${ODBC_POSTGRE_DIR}/procdef_snips.h
  ${ODBC_POSTGRE_DIR}/process.cpp
  ${ODBC_POSTGRE_DIR}/process.sh
  ${ODBC_POSTGRE_DIR}/processarch.cpp
  ${ODBC_POSTGRE_DIR}/processarch.sh
  ${ODBC_POSTGRE_DIR}/processarch_snips.h
  ${ODBC_POSTGRE_DIR}/processdata.cpp
  ${ODBC_POSTGRE_DIR}/processdata.sh
  ${ODBC_POSTGRE_DIR}/processdatadef.cpp
  ${ODBC_POSTGRE_DIR}/processdatadef.sh
  ${ODBC_POSTGRE_DIR}/processdatadef_snips.h
  ${ODBC_POSTGRE_DIR}/processdataext.cpp
  ${ODBC_POSTGRE_DIR}/processdataext.sh
  ${ODBC_POSTGRE_DIR}/processdataext_snips.h
  ${ODBC_POSTGRE_DIR}/processdata_snips.h
  ${ODBC_POSTGRE_DIR}/process_snips.h
  ${ODBC_POSTGRE_DIR}/proctype.cpp
  ${ODBC_POSTGRE_DIR}/proctype.sh
  ${ODBC_POSTGRE_DIR}/proctypegroup.cpp
  ${ODBC_POSTGRE_DIR}/proctypegroup.sh
  ${ODBC_POSTGRE_DIR}/proctypegrouping.cpp
  ${ODBC_POSTGRE_DIR}/proctypegrouping.sh
  ${ODBC_POSTGRE_DIR}/proctypegrouping_snips.h
  ${ODBC_POSTGRE_DIR}/proctypegroup_snips.h
  ${ODBC_POSTGRE_DIR}/proctype_snips.h
  ${ODBC_POSTGRE_DIR}/pubsublist.cpp
  ${ODBC_POSTGRE_DIR}/pubsublist.sh
  ${ODBC_POSTGRE_DIR}/pubsublist_snips.h
  ${ODBC_POSTGRE_DIR}/queryattributes.cpp
  ${ODBC_POSTGRE_DIR}/queryattributes.sh
  ${ODBC_POSTGRE_DIR}/queryattributes_snips.h
  ${ODBC_POSTGRE_DIR}/queryclassifications.cpp
  ${ODBC_POSTGRE_DIR}/queryclassifications.sh
  ${ODBC_POSTGRE_DIR}/queryclassifications_snips.h
  ${ODBC_POSTGRE_DIR}/queryproducts.cpp
  ${ODBC_POSTGRE_DIR}/queryproducts.sh
  ${ODBC_POSTGRE_DIR}/queryproducts_snips.h
  ${ODBC_POSTGRE_DIR}/reclassify.cpp
  ${ODBC_POSTGRE_DIR}/reclassify.sh
  ${ODBC_POSTGRE_DIR}/reclassify_snips.h
  ${ODBC_POSTGRE_DIR}/relatedcases.cpp
  ${ODBC_POSTGRE_DIR}/relatedcases.sh
  ${ODBC_POSTGRE_DIR}/relatedcases_snips.h
  ${ODBC_POSTGRE_DIR}/relatereason.cpp
  ${ODBC_POSTGRE_DIR}/relatereason.sh
  ${ODBC_POSTGRE_DIR}/relatereason_snips.h
  ${ODBC_POSTGRE_DIR}/relationtype.cpp
  ${ODBC_POSTGRE_DIR}/relationtype.sh
  ${ODBC_POSTGRE_DIR}/relationtype_snips.h
  ${ODBC_POSTGRE_DIR}/routehistory.cpp
  ${ODBC_POSTGRE_DIR}/routehistory.sh
  ${ODBC_POSTGRE_DIR}/routehistory_snips.h
  ${ODBC_POSTGRE_DIR}/searchcase.cpp
  ${ODBC_POSTGRE_DIR}/searchcase.sh
  ${ODBC_POSTGRE_DIR}/searchcasearch.cpp
  ${ODBC_POSTGRE_DIR}/searchcasearch.sh
  ${ODBC_POSTGRE_DIR}/searchcasearch_snips.h
  ${ODBC_POSTGRE_DIR}/searchcase_snips.h
  ${ODBC_POSTGRE_DIR}/securitygroup.cpp
  ${ODBC_POSTGRE_DIR}/securitygroup.sh
  ${ODBC_POSTGRE_DIR}/securitygrouptasklink.cpp
  ${ODBC_POSTGRE_DIR}/securitygrouptasklink.sh
  ${ODBC_POSTGRE_DIR}/securitygrouptasklink_snips.h
  ${ODBC_POSTGRE_DIR}/securitygroup_snips.h
  ${ODBC_POSTGRE_DIR}/serviceclient.cpp
  ${ODBC_POSTGRE_DIR}/serviceclient.sh
  ${ODBC_POSTGRE_DIR}/serviceclienttaskmapping.cpp
  ${ODBC_POSTGRE_DIR}/serviceclienttaskmapping.sh
  ${ODBC_POSTGRE_DIR}/serviceclienttaskmapping_snips.h
  ${ODBC_POSTGRE_DIR}/serviceclient_snips.h
  ${ODBC_POSTGRE_DIR}/singletrans.cpp
  ${ODBC_POSTGRE_DIR}/singletrans.sh
  ${ODBC_POSTGRE_DIR}/singletrans_snips.h
  ${ODBC_POSTGRE_DIR}/status.cpp
  ${ODBC_POSTGRE_DIR}/status.sh
  ${ODBC_POSTGRE_DIR}/status_snips.h
  ${ODBC_POSTGRE_DIR}/step.cpp
  ${ODBC_POSTGRE_DIR}/step.sh
  ${ODBC_POSTGRE_DIR}/stepactivity.cpp
  ${ODBC_POSTGRE_DIR}/stepactivity.sh
  ${ODBC_POSTGRE_DIR}/stepactivity_snips.h
  ${ODBC_POSTGRE_DIR}/stepdiary.cpp
  ${ODBC_POSTGRE_DIR}/stepdiary.sh
  ${ODBC_POSTGRE_DIR}/stepdiary_snips.h
  ${ODBC_POSTGRE_DIR}/stepescalation.cpp
  ${ODBC_POSTGRE_DIR}/stepescalation.sh
  ${ODBC_POSTGRE_DIR}/stepescalation_snips.h
  ${ODBC_POSTGRE_DIR}/stepusergroup.cpp
  ${ODBC_POSTGRE_DIR}/stepusergroup.sh
  ${ODBC_POSTGRE_DIR}/stepusergroup_snips.h
  ${ODBC_POSTGRE_DIR}/step_snips.h
  ${ODBC_POSTGRE_DIR}/task.cpp
  ${ODBC_POSTGRE_DIR}/task.sh
  ${ODBC_POSTGRE_DIR}/taskgate.cpp
  ${ODBC_POSTGRE_DIR}/taskgate.sh
  ${ODBC_POSTGRE_DIR}/taskgatein.cpp
  ${ODBC_POSTGRE_DIR}/taskgatein.sh
  ${ODBC_POSTGRE_DIR}/taskgatein_snips.h
  ${ODBC_POSTGRE_DIR}/taskgate_snips.h
  ${ODBC_POSTGRE_DIR}/taskparameters.cpp
  ${ODBC_POSTGRE_DIR}/taskparameters.sh
  ${ODBC_POSTGRE_DIR}/taskparametersconfig.cpp
  ${ODBC_POSTGRE_DIR}/taskparametersconfig.sh
  ${ODBC_POSTGRE_DIR}/taskparametersconfig_snips.h
  ${ODBC_POSTGRE_DIR}/taskparameters_snips.h
  ${ODBC_POSTGRE_DIR}/tasktype.cpp
  ${ODBC_POSTGRE_DIR}/tasktype.sh
  ${ODBC_POSTGRE_DIR}/tasktype_snips.h
  ${ODBC_POSTGRE_DIR}/taskusergroup.cpp
  ${ODBC_POSTGRE_DIR}/taskusergroup.sh
  ${ODBC_POSTGRE_DIR}/taskusergroup_snips.h
  ${ODBC_POSTGRE_DIR}/task_snips.h
  ${ODBC_POSTGRE_DIR}/usedsecuritygroup.cpp
  ${ODBC_POSTGRE_DIR}/usedsecuritygroup.sh
  ${ODBC_POSTGRE_DIR}/usedsecuritygroup_snips.h
  ${ODBC_POSTGRE_DIR}/userarealink.cpp
  ${ODBC_POSTGRE_DIR}/userarealink.sh
  ${ODBC_POSTGRE_DIR}/userarealink_snips.h
  ${ODBC_POSTGRE_DIR}/usergroup.cpp
  ${ODBC_POSTGRE_DIR}/usergroup.sh
  ${ODBC_POSTGRE_DIR}/usergrouplink.cpp
  ${ODBC_POSTGRE_DIR}/usergrouplink.sh
  ${ODBC_POSTGRE_DIR}/usergrouplink_snips.h
  ${ODBC_POSTGRE_DIR}/usergroup_snips.h
  ${ODBC_POSTGRE_DIR}/userhierarchy.cpp
  ${ODBC_POSTGRE_DIR}/userhierarchy.sh
  ${ODBC_POSTGRE_DIR}/userhierarchy_snips.h
  ${ODBC_POSTGRE_DIR}/users.cpp
  ${ODBC_POSTGRE_DIR}/users.sh
  ${ODBC_POSTGRE_DIR}/users_snips.h
  ${ODBC_POSTGRE_DIR}/wfmessagetracing.cpp
  ${ODBC_POSTGRE_DIR}/wfmessagetracing.sh
  ${ODBC_POSTGRE_DIR}/wfmessagetracing_snips.h
  )

source_group("generated" FILES ${generated})

set (source
  main.cpp
  )

add_executable (odbc_postgre_test ${source} ${generated})

target_link_libraries (odbc_postgre_test odbc_general winmm)

set_target_properties(odbc_postgre_test PROPERTIES PROJECT_LABEL tests_odbc_postgre_test)
set_property(TARGET odbc_postgre_test PROPERTY FOLDER ${TESTS_FOLDER}/odbc_postgre_test)

set (generated
  ${ODBC_POSTGRE_DIR}/moggy2.cpp
  ${ODBC_POSTGRE_DIR}/moggy2.sh
  ${ODBC_POSTGRE_DIR}/moggy2_snips.h
  )

source_group("generated" FILES ${generated})

set (source
  main_blob.cpp
  )

add_executable (odbc_postgre_blob_test ${source} ${generated})

target_link_libraries (odbc_postgre_blob_test odbc_general winmm)

set_target_properties(odbc_postgre_blob_test PROPERTIES PROJECT_LABEL tests_odbc_postgre_blob_test)
set_property(TARGET odbc_postgre_blob_test PROPERTY FOLDER ${TESTS_FOLDER}/odbc_postgre_blob_test)
