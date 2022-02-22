project (putty3.idl2.accuity.swagger_build)

if (BUILD_NSWAG)
  pathed (outDir ${ACCUITY_BINARY_DIR}/nswag/cs)
  set_source_files_properties (${ACCUITY_BINARY_DIR}/http/accuity_swagger.yaml PROPERTIES GENERATED TRUE)
  file (TO_NATIVE_PATH ${ACCUITY_BINARY_DIR}/http/accuity_swagger.yaml inFile)
  file (TO_NATIVE_PATH ${outDir}/Accuity.cs outFile)
  source_group (Built FILES ${outDir}/Accuity.cs)
  set (command "nswag openapi2csclient /input:${inFile} /output:${outFile} /classname:AccuityClient /namespace:AccuityNSwag /generatesyncmethods:true /UseTitleAsTypeName:true")
  message (STATUS ${command})
  add_custom_target (accuity_nswag_build ALL
    nswag openapi2csclient /input:${inFile} /output:${outFile} /classname:AccuityClient /namespace:AccuityNSwag /generatesyncmethods:true /UseTitleAsTypeName:true
    DEPENDS ${ACCUITY_BINARY_DIR}/http/accuity_swagger.yaml
    SOURCES ${ACCUITY_BINARY_DIR}/http/accuity_swagger.yaml
  )
  set_property(TARGET accuity_nswag_build PROPERTY FOLDER "putty3/idl2/accuity/swagger_nswag_build")
endif ()
