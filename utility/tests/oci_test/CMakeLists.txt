project (tests_ocitest)

set (OCI_DIR ${BUILD_SQL_DIR}/oci/sh)

include_directories(
  ${LIBS_SOURCE_DIR}/portal/oci_12/oracle
  ${LIBS_SOURCE_DIR}/portal/oci_12/src
  ${LIBS_SOURCE_DIR}/utfpp
  ${OCI_DIR}
)

set (generated 
  ${OCI_DIR}/areacode.cpp
  ${OCI_DIR}/areacode.sh
  ${OCI_DIR}/areacode_snips.h
  ${OCI_DIR}/assignhist.cpp
  ${OCI_DIR}/assignhist.sh
  ${OCI_DIR}/assignhist_snips.h
  ${OCI_DIR}/attachment.cpp
  ${OCI_DIR}/attachment.sh
  ${OCI_DIR}/attachmentlocation.cpp
  ${OCI_DIR}/attachmentlocation.sh
  ${OCI_DIR}/attachmentlocation_snips.h
  ${OCI_DIR}/attachmenttype.cpp
  ${OCI_DIR}/attachmenttype.sh
  ${OCI_DIR}/attachmenttype_snips.h
  ${OCI_DIR}/attachment_snips.h
  ${OCI_DIR}/availablestep.cpp
  ${OCI_DIR}/availablestep.sh
  ${OCI_DIR}/availablestep_snips.h
  ${OCI_DIR}/caseactivity.cpp
  ${OCI_DIR}/caseactivity.sh
  ${OCI_DIR}/caseactivity_snips.h
  ${OCI_DIR}/casedependency.cpp
  ${OCI_DIR}/casedependency.sh
  ${OCI_DIR}/casedependency_snips.h
  ${OCI_DIR}/casereference.cpp
  ${OCI_DIR}/casereference.sh
  ${OCI_DIR}/casereferencetype.cpp
  ${OCI_DIR}/casereferencetype.sh
  ${OCI_DIR}/casereferencetype_snips.h
  ${OCI_DIR}/casereference_snips.h
  ${OCI_DIR}/cases.cpp
  ${OCI_DIR}/cases.sh
  ${OCI_DIR}/casesarch.cpp
  ${OCI_DIR}/casesarch.sh
  ${OCI_DIR}/casesarch_snips.h
  ${OCI_DIR}/cases_snips.h
  ${OCI_DIR}/caseuser.cpp
  ${OCI_DIR}/caseuser.sh
  ${OCI_DIR}/caseuser_snips.h
  ${OCI_DIR}/channeltransaction.cpp
  ${OCI_DIR}/channeltransaction.sh
  ${OCI_DIR}/channeltransaction_snips.h
  ${OCI_DIR}/datamapper.cpp
  ${OCI_DIR}/datamapper.sh
  ${OCI_DIR}/datamapper_snips.h
  ${OCI_DIR}/diarytype.cpp
  ${OCI_DIR}/diarytype.sh
  ${OCI_DIR}/diarytype_snips.h
  ${OCI_DIR}/discardreason.cpp
  ${OCI_DIR}/discardreason.sh
  ${OCI_DIR}/discardreason_snips.h
  ${OCI_DIR}/gaterule.cpp
  ${OCI_DIR}/gaterule.sh
  ${OCI_DIR}/gaterule_snips.h
  ${OCI_DIR}/gatetype.cpp
  ${OCI_DIR}/gatetype.sh
  ${OCI_DIR}/gatetype_snips.h
  ${OCI_DIR}/interfacetype.cpp
  ${OCI_DIR}/interfacetype.sh
  ${OCI_DIR}/interfacetype_snips.h
  ${OCI_DIR}/locks.cpp
  ${OCI_DIR}/locks.sh
  ${OCI_DIR}/locks_snips.h
  ${OCI_DIR}/message.cpp
  ${OCI_DIR}/message.sh
  ${OCI_DIR}/message_snips.h
  ${OCI_DIR}/moggy.cpp
  ${OCI_DIR}/moggy.sh
  ${OCI_DIR}/moggy_snips.h
  ${OCI_DIR}/note.cpp
  ${OCI_DIR}/note.sh
  ${OCI_DIR}/notetype.cpp
  ${OCI_DIR}/notetype.sh
  ${OCI_DIR}/notetype_snips.h
  ${OCI_DIR}/note_snips.h
  ${OCI_DIR}/objecttype.cpp
  ${OCI_DIR}/objecttype.sh
  ${OCI_DIR}/objecttype_snips.h
  ${OCI_DIR}/orchdef.cpp
  ${OCI_DIR}/orchdef.sh
  ${OCI_DIR}/orchdefendpoint.cpp
  ${OCI_DIR}/orchdefendpoint.sh
  ${OCI_DIR}/orchdefendpoint_snips.h
  ${OCI_DIR}/orchdef_snips.h
  ${OCI_DIR}/orchinst.cpp
  ${OCI_DIR}/orchinst.sh
  ${OCI_DIR}/orchinst_snips.h
  ${OCI_DIR}/pendreason.cpp
  ${OCI_DIR}/pendreason.sh
  ${OCI_DIR}/pendreasonrule.cpp
  ${OCI_DIR}/pendreasonrule.sh
  ${OCI_DIR}/pendreasonrulecondition.cpp
  ${OCI_DIR}/pendreasonrulecondition.sh
  ${OCI_DIR}/pendreasonrulecondition_snips.h
  ${OCI_DIR}/pendreasonrule_snips.h
  ${OCI_DIR}/pendreasonusergroup.cpp
  ${OCI_DIR}/pendreasonusergroup.sh
  ${OCI_DIR}/pendreasonusergroup_snips.h
  ${OCI_DIR}/pendreason_snips.h
  ${OCI_DIR}/proccategory.cpp
  ${OCI_DIR}/proccategory.sh
  ${OCI_DIR}/proccategory_snips.h
  ${OCI_DIR}/procdef.cpp
  ${OCI_DIR}/procdef.sh
  ${OCI_DIR}/procdefusergroup.cpp
  ${OCI_DIR}/procdefusergroup.sh
  ${OCI_DIR}/procdefusergroup_snips.h
  ${OCI_DIR}/procdef_snips.h
  ${OCI_DIR}/process.cpp
  ${OCI_DIR}/process.sh
  ${OCI_DIR}/processarch.cpp
  ${OCI_DIR}/processarch.sh
  ${OCI_DIR}/processarch_snips.h
  ${OCI_DIR}/processdata.cpp
  ${OCI_DIR}/processdata.sh
  ${OCI_DIR}/processdatadef.cpp
  ${OCI_DIR}/processdatadef.sh
  ${OCI_DIR}/processdatadef_snips.h
  ${OCI_DIR}/processdataext.cpp
  ${OCI_DIR}/processdataext.sh
  ${OCI_DIR}/processdataext_snips.h
  ${OCI_DIR}/processdata_snips.h
  ${OCI_DIR}/process_snips.h
  ${OCI_DIR}/proctype.cpp
  ${OCI_DIR}/proctype.sh
  ${OCI_DIR}/proctypegroup.cpp
  ${OCI_DIR}/proctypegroup.sh
  ${OCI_DIR}/proctypegrouping.cpp
  ${OCI_DIR}/proctypegrouping.sh
  ${OCI_DIR}/proctypegrouping_snips.h
  ${OCI_DIR}/proctypegroup_snips.h
  ${OCI_DIR}/proctype_snips.h
  ${OCI_DIR}/pubsublist.cpp
  ${OCI_DIR}/pubsublist.sh
  ${OCI_DIR}/pubsublist_snips.h
  ${OCI_DIR}/queryattributes.cpp
  ${OCI_DIR}/queryattributes.sh
  ${OCI_DIR}/queryattributes_snips.h
  ${OCI_DIR}/queryclassifications.cpp
  ${OCI_DIR}/queryclassifications.sh
  ${OCI_DIR}/queryclassifications_snips.h
  ${OCI_DIR}/queryproducts.cpp
  ${OCI_DIR}/queryproducts.sh
  ${OCI_DIR}/queryproducts_snips.h
  ${OCI_DIR}/reclassify.cpp
  ${OCI_DIR}/reclassify.sh
  ${OCI_DIR}/reclassify_snips.h
  ${OCI_DIR}/relatedcases.cpp
  ${OCI_DIR}/relatedcases.sh
  ${OCI_DIR}/relatedcases_snips.h
  ${OCI_DIR}/relatereason.cpp
  ${OCI_DIR}/relatereason.sh
  ${OCI_DIR}/relatereason_snips.h
  ${OCI_DIR}/relationtype.cpp
  ${OCI_DIR}/relationtype.sh
  ${OCI_DIR}/relationtype_snips.h
  ${OCI_DIR}/routehistory.cpp
  ${OCI_DIR}/routehistory.sh
  ${OCI_DIR}/routehistory_snips.h
  ${OCI_DIR}/searchcase.cpp
  ${OCI_DIR}/searchcase.sh
  ${OCI_DIR}/searchcasearch.cpp
  ${OCI_DIR}/searchcasearch.sh
  ${OCI_DIR}/searchcasearch_snips.h
  ${OCI_DIR}/searchcase_snips.h
  ${OCI_DIR}/securitygroup.cpp
  ${OCI_DIR}/securitygroup.sh
  ${OCI_DIR}/securitygrouptasklink.cpp
  ${OCI_DIR}/securitygrouptasklink.sh
  ${OCI_DIR}/securitygrouptasklink_snips.h
  ${OCI_DIR}/securitygroup_snips.h
  ${OCI_DIR}/serviceclient.cpp
  ${OCI_DIR}/serviceclient.sh
  ${OCI_DIR}/serviceclienttaskmapping.cpp
  ${OCI_DIR}/serviceclienttaskmapping.sh
  ${OCI_DIR}/serviceclienttaskmapping_snips.h
  ${OCI_DIR}/serviceclient_snips.h
  ${OCI_DIR}/singletrans.cpp
  ${OCI_DIR}/singletrans.sh
  ${OCI_DIR}/singletrans_snips.h
  ${OCI_DIR}/status.cpp
  ${OCI_DIR}/status.sh
  ${OCI_DIR}/status_snips.h
  ${OCI_DIR}/step.cpp
  ${OCI_DIR}/step.sh
  ${OCI_DIR}/stepactivity.cpp
  ${OCI_DIR}/stepactivity.sh
  ${OCI_DIR}/stepactivity_snips.h
  ${OCI_DIR}/stepdiary.cpp
  ${OCI_DIR}/stepdiary.sh
  ${OCI_DIR}/stepdiary_snips.h
  ${OCI_DIR}/stepescalation.cpp
  ${OCI_DIR}/stepescalation.sh
  ${OCI_DIR}/stepescalation_snips.h
  ${OCI_DIR}/stepusergroup.cpp
  ${OCI_DIR}/stepusergroup.sh
  ${OCI_DIR}/stepusergroup_snips.h
  ${OCI_DIR}/step_snips.h
  ${OCI_DIR}/task.cpp
  ${OCI_DIR}/task.sh
  ${OCI_DIR}/taskgate.cpp
  ${OCI_DIR}/taskgate.sh
  ${OCI_DIR}/taskgatein.cpp
  ${OCI_DIR}/taskgatein.sh
  ${OCI_DIR}/taskgatein_snips.h
  ${OCI_DIR}/taskgate_snips.h
  ${OCI_DIR}/taskparameters.cpp
  ${OCI_DIR}/taskparameters.sh
  ${OCI_DIR}/taskparametersconfig.cpp
  ${OCI_DIR}/taskparametersconfig.sh
  ${OCI_DIR}/taskparametersconfig_snips.h
  ${OCI_DIR}/taskparameters_snips.h
  ${OCI_DIR}/tasktype.cpp
  ${OCI_DIR}/tasktype.sh
  ${OCI_DIR}/tasktype_snips.h
  ${OCI_DIR}/taskusergroup.cpp
  ${OCI_DIR}/taskusergroup.sh
  ${OCI_DIR}/taskusergroup_snips.h
  ${OCI_DIR}/task_snips.h
  ${OCI_DIR}/usedsecuritygroup.cpp
  ${OCI_DIR}/usedsecuritygroup.sh
  ${OCI_DIR}/usedsecuritygroup_snips.h
  ${OCI_DIR}/userarealink.cpp
  ${OCI_DIR}/userarealink.sh
  ${OCI_DIR}/userarealink_snips.h
  ${OCI_DIR}/usergroup.cpp
  ${OCI_DIR}/usergroup.sh
  ${OCI_DIR}/usergrouplink.cpp
  ${OCI_DIR}/usergrouplink.sh
  ${OCI_DIR}/usergrouplink_snips.h
  ${OCI_DIR}/usergroup_snips.h
  ${OCI_DIR}/userhierarchy.cpp
  ${OCI_DIR}/userhierarchy.sh
  ${OCI_DIR}/userhierarchy_snips.h
  ${OCI_DIR}/users.cpp
  ${OCI_DIR}/users.sh
  ${OCI_DIR}/users_snips.h
  ${OCI_DIR}/wfmessagetracing.cpp
  ${OCI_DIR}/wfmessagetracing.sh
  ${OCI_DIR}/wfmessagetracing_snips.h
  )

source_group("generated" FILES ${generated})

set (source
  main.cpp
  )

add_executable (oci_test ${source} ${generated})

link_directories(${ORACLE_LIBRARY_DIR})
target_link_libraries (oci_test oci_12)

set_target_properties(oci_test PROPERTIES PROJECT_LABEL tests_oci_test)
set_property(TARGET oci_test PROPERTY FOLDER ${TESTS_FOLDER}/oci_test)

set (generated
  ${OCI_DIR}/moggy2.cpp
  ${OCI_DIR}/moggy2.sh
  ${OCI_DIR}/moggy2_snips.h
  )

source_group("generated" FILES ${generated})

set (source
  main_blob.cpp
  )

add_executable (oci_blob_test ${source} ${generated})

link_directories(${ORACLE_LIBRARY_DIR})
target_link_libraries (oci_blob_test oci_12)

set_target_properties(oci_blob_test PROPERTIES PROJECT_LABEL tests_oci_blob_test)
set_property(TARGET oci_blob_test PROPERTY FOLDER ${TESTS_FOLDER}/oci_blob_test)
