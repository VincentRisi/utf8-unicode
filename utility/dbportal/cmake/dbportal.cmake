if (EXISTS ${CMAKE_BINARY_DIR}/tools/pocitools)
  set (DBPORTAL_OCI_DIR ${CMAKE_BINARY_DIR}/tools/pocitools)
elseif (EXISTS ${CMAKE_SOURCE_DIR}/tools/pocitools)
  set (DBPORTAL_OCI_DIR ${CMAKE_SOURCE_DIR}/tools/pocitools)
endif ()  

function (pathed result ext_dir)
  foreach (arg ${ARGN})
    if (${ARGN} STREQUAL REMOVE)
      file (GLOB remFiles ${ext_dir}/*.*)
      list (LENGTH remFiles count)
      if (0 LESS count)
        #message (STATUS "--info-- Removing all files in ${ext_dir}")
        file (REMOVE ${remFiles})
      endif ()
    endif ()
  endforeach ()
  file(MAKE_DIRECTORY ${ext_dir})
  set ("${result}" ${ext_dir} PARENT_SCOPE)
endfunction ()

# -- splits db portal switches based on the following rules
#  1: switch                     -  "OneSQLScript=1"
#  2: target|dir|ext             -  "SO|${soDir}|.so"
#  3: target|dir|ext|group|mask  -  "C|${shDir}|.sh|SH Files|.*[.]sh$"
#     C|${shDir}|.sh
#     is changed to TargetC=1 CDir=${shDir} CExt=.sh
function (dbportal_split target_args)
  list (LENGTH target_args count)
  if (4 LESS count)
    list (GET target_args 3 arg)
    set (title ${arg})
    list (GET target_args 4 arg)
    set (mask ${arg})
  elseif (3 LESS count)
    list (GET target_args 3 arg)
    set (title ${arg})
  endif ()
  if (2 LESS count)
    list (GET target_args 0 arg)
    set (target "Target${arg}=1")
    set (name ${arg})
    list (GET target_args 1 arg)
    set (targetdir "${name}Dir=${arg}")
    set (dir ${arg})
    list (GET target_args 2 arg)
    set (targetext "${name}Ext=${arg}")
    string (SUBSTRING ${arg} 1 -1 ext)
  endif ()
  set (groups PARENT_SCOPE)
  if (1 EQUAL count)
    set (switch ${target_args}            PARENT_SCOPE)
  elseif (3 EQUAL count)
    set (switch "${target};${targetdir};${targetext}" PARENT_SCOPE)
    set (groups "${dir};${ext}"                       PARENT_SCOPE)
  elseif (4 EQUAL count)
    set (switch "${target};${targetdir};${targetext}" PARENT_SCOPE)
    set (groups "${title};${dir};${ext}"              PARENT_SCOPE)
  elseif (5 EQUAL count)
    set (switch "${target};${targetdir};${targetext}" PARENT_SCOPE)
    set (groups "${title};${mask};${dir};${ext}"      PARENT_SCOPE)
  endif ()
endfunction ()

function (dbportal_glob dir ext)
  file (GLOB globs 
    ${dir}/*${ext} 
    )
  set (files ${globs} PARENT_SCOPE)
endfunction()

function (dbportal2 projectName binFilename siFiles)
  #if (DBPORTAL_NO_ALL)
  #  message (STATUS "--info-- DBPORTAL_NO_ALL ${projectName}")
  #endif ()
  source_group ("SI Files"  REGULAR_EXPRESSION ".*[.]si$")
  set (direxts)
  set (logFiles)
  set (allFiles)
  set (switchesList ${CMAKE_CURRENT_BINARY_DIR}/${projectName}.switches)
  file (WRITE ${switchesList} "")
  foreach (arg ${ARGN})
    string(REPLACE "|" ";" arg2 ${arg})
    dbportal_split("${arg2}")
    list (LENGTH groups noOf)
    if (3 LESS noOf)
      list (GET groups 0 title)
      list (GET groups 1 mask)
      source_group ("${title}" REGULAR_EXPRESSION "${mask}")
      list (GET groups 2 dir)
      list (GET groups 3 ext)
      list (APPEND direxts "${dir}|${ext}")
    elseif (2 LESS noOf)
      list (GET groups 0 title)
      list (GET groups 1 dir)
      list (GET groups 2 ext)
      dbportal_glob(${dir} ${ext})
      source_group ("${title}" FILES ${files})
      list (APPEND allFiles ${files})
    elseif (1 LESS noOf)
      list (GET groups 0 dir)
      list (GET groups 1 ext)
      list (APPEND direxts "${dir}|${ext}")
    endif ()
    foreach (sw ${switch})
      file (APPEND ${switchesList} "${sw}\n")
    endforeach ()
  endforeach ()
  set (pocioci ${DBPORTAL_OCI_DIR}/pocioci)
  set (soFiles)
  foreach (siFile ${siFiles})
    get_filename_component (temp ${siFile} NAME)
    string (TOLOWER ${temp} temp1)
    get_filename_component (filename ${temp1} NAME_WE)
    set (logFile ${CMAKE_CURRENT_BINARY_DIR}/${filename}.log)
    list (APPEND logFiles ${logFile})
    set (oFiles)
    set (soFile)
    foreach (arg ${direxts})
      string(REPLACE "|" ";" arg2 ${arg})
      list (GET arg2 0 dir)
      list (GET arg2 1 ext)
      set (oFile ${dir}/${filename}.${ext})
      if (ext STREQUAL "so")
        list (APPEND soFiles  "${oFile}")
        set (soFile ${oFile})
      endif ()
    endforeach()
    #message (STATUS "--info-- OUTPUT  ${logFile} ${soFile}")
    #message (STATUS "--info-- DEPENDS ${siFile}")
    add_custom_command (
      OUTPUT  ${logFile} ${soFile}
      COMMAND ${pocioci} -s${switchesList} -l${logFile} ${siFile}
      DEPENDS ${siFile}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
  endforeach()
  source_group (LogFiles FILES ${logFiles})
  set (pocilink ${DBPORTAL_OCI_DIR}/pocilink)
  list (LENGTH soFiles noOf)
  if (0 LESS noOf)
    set (soFileList ${CMAKE_CURRENT_BINARY_DIR}/${projectName}.sofiles)
    file (WRITE ${soFileList} "")
    foreach (soFile ${soFiles})
      file (APPEND ${soFileList} "${soFile}\n")
    endforeach()
    set (binFile ${binDir}/${binFilename}.bin)
    #message (STATUS "--info-- ${noOf} ${soFileList}")
    add_custom_command (
      OUTPUT  ${binFile}
      COMMAND ${pocilink} -v -f${soFileList} -b${binDir} -n${binFilename}
      DEPENDS ${siFiles}
      )
  endif ()
  add_custom_target (${projectName} ALL
    DEPENDS ${logFiles} ${soFiles} ${binFile}
    SOURCES ${siFiles} ${logFiles} ${allFiles} ${soFileList}
  )
endfunction()

function (dbportal projectName binFilename siFiles)
  if (DBPORTAL_NO_ALL)
    #message (STATUS "--info-- DBPORTAL_NO_ALL ${projectName}")
  endif ()
  source_group ("SI Files"  REGULAR_EXPRESSION ".*[.]si$")
  set (direxts)
  set (allFiles)
  # We will store switches to file to avoid over long command line
  set (switchesList ${CMAKE_CURRENT_BINARY_DIR}/${projectName}.switches)
  file (WRITE ${switchesList} "")
  foreach (arg ${ARGN})
    string(REPLACE "|" ";" arg2 ${arg})
    dbportal_split("${arg2}")
    list (LENGTH groups noOf)
    if (3 LESS noOf)
      # this case has VS source group plus dir ext files
      list (GET groups 0 title)
      list (GET groups 1 mask)
      source_group ("${title}" REGULAR_EXPRESSION "${mask}")
      list (GET groups 2 dir)
      list (GET groups 3 ext)
      list (APPEND direxts "${dir}|${ext}")
    elseif (2 LESS noOf)
      list (GET groups 0 title)
      list (GET groups 1 dir)
      list (GET groups 2 ext)
      dbportal_glob(${dir} ${ext})
      source_group ("${title}" FILES ${files})
      #message (STATUS "source_group (${title} FILES ${files})")
      list (APPEND allFiles ${files})
    elseif (1 LESS noOf)
      # this case dir ext files
      list (GET groups 0 dir)
      list (GET groups 1 ext)
      list (APPEND direxts "${dir}|${ext}")
    endif ()
    # switch may be multipart with semicolon separates
    foreach (sw ${switch})
      file (APPEND ${switchesList} "${sw}\n")
    endforeach ()
  endforeach ()
  if (DBPORTAL_OCI_DIR)
    file (TO_NATIVE_PATH ${DBPORTAL_OCI_DIR}/pocioci pocioci)
  endif ()  
  set (soFiles)
  foreach (siFile ${siFiles})
    get_filename_component (temp ${siFile} NAME)
    string (TOLOWER ${temp} temp1)
    get_filename_component (filename ${temp1} NAME_WE)
    set (oFiles)
    foreach (arg ${direxts})
      string(REPLACE "|" ";" arg2 ${arg})
      list (GET arg2 0 dir)
      list (GET arg2 1 ext)
      set (oFile ${dir}/${filename}.${ext})
      if (ext STREQUAL "so")
        list (APPEND soFiles  "${oFile}")
        list (APPEND oFiles   "${oFile}")
      elseif (DBPORTAL_NO_ALL)
        list (APPEND oFiles   "${oFile}")
      else ()
        list (APPEND oFiles   "${oFile}")
        list (APPEND allFiles "${oFile}")
      endif ()
    endforeach()
    add_custom_command (
      OUTPUT  ${oFiles}
      COMMAND ${pocioci} ${siFile} "-s${switchesList}"
      DEPENDS ${siFile}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} # added from simon
      VERBATIM
    )
  endforeach()
  set (allFileList ${CMAKE_CURRENT_BINARY_DIR}/${projectName}.allfiles)
  file (WRITE ${allFileList} "")
  foreach (allFile ${allFiles})
    file (APPEND ${allFileList} "${allFile}\n")
  endforeach()
  if (DBPORTAL_OCI_DIR)
    file (TO_NATIVE_PATH ${DBPORTAL_OCI_DIR}/pocilink pocilink)
  endif ()  
  list (LENGTH soFiles noOf)
  if (0 LESS noOf)
    set (soFileList ${CMAKE_CURRENT_BINARY_DIR}/${projectName}.sofiles)
    file (WRITE ${soFileList} "")
    foreach (soFile ${soFiles})
      file (APPEND ${soFileList} "${soFile}\n")
    endforeach()
    set (binFile ${binDir}/${binFilename}.bin)
    add_custom_command (
      OUTPUT  ${binFile}
      COMMAND ${pocilink} -f${soFileList} -b${binDir} -n${binFilename}
      DEPENDS ${soFiles} ${siFiles}
    )
    add_custom_target (${projectName} ALL
      DEPENDS ${binFile}
      SOURCES ${siFiles} ${allFiles}
    )
  else ()
    add_custom_target (${projectName} ALL
      DEPENDS ${allFiles}
      SOURCES ${siFiles} ${allFiles}
    )
  endif ()
  set_source_files_properties (
    ${allFiles}
    PROPERTIES GENERATED TRUE
  )
endfunction()

function (dbportal_genproc projectName siFiles)
  source_group ("SI Files"  REGULAR_EXPRESSION ".*[.]si$")
  set (direxts)
  set (allFiles)
  # We will store switches to file to avoid over long command line
  set (switchesList ${CMAKE_CURRENT_BINARY_DIR}/${projectName}.switches)
  file (WRITE ${switchesList} "")
  foreach (arg ${ARGN})
    string(REPLACE "|" ";" arg2 ${arg})
    dbportal_split("${arg2}")
    list (LENGTH groups noOf)
    if (3 LESS noOf)
      # this case has VS source group plus dir ext files
      list (GET groups 0 title)
      list (GET groups 1 mask)
      source_group ("${title}" REGULAR_EXPRESSION "${mask}")
      list (GET groups 2 dir)
      list (GET groups 3 ext)
      list (APPEND direxts "${dir}|${ext}")
    elseif (2 LESS noOf)
      list (GET groups 0 title)
      list (GET groups 1 dir)
      list (GET groups 2 ext)
      dbportal_glob(${dir} ${ext})
      source_group ("${title}" FILES "${files}")
      list (APPEND allFiles ${files})
    elseif (1 LESS noOf)
      # this case dir ext files
      list (GET groups 0 dir)
      list (GET groups 1 ext)
      list (APPEND direxts "${dir}|${ext}")
    endif ()
    # switch may be multipart with semicolon separates
    foreach (sw ${switch})
      file (APPEND ${switchesList} "${sw}\n")
    endforeach ()
  endforeach ()
  set (soFiles)
  set (pcFiles)
  foreach (siFile ${siFiles})
    get_filename_component (temp ${siFile} NAME)
    string (TOLOWER ${temp} temp1)
    get_filename_component (filename ${temp1} NAME_WE)
    set (oFiles)
    foreach (arg ${direxts})
      string(REPLACE "|" ";" arg2 ${arg})
      list (GET arg2 0 dir)
      list (GET arg2 1 ext)
      set (oFile ${dir}/${filename}.${ext})
      list (APPEND pcFiles ${pcDir}/${filename}.pc)
      list (APPEND pcFiles ${pcDir}/${filename}.h)
      if (ext STREQUAL "so")
        list (APPEND soFiles  "${oFile}")
        list (APPEND oFiles   "${oFile}")
      else ()
        list (APPEND allFiles "${oFile}")
        list (APPEND oFiles   "${oFile}")
      endif ()
    endforeach()
    add_custom_command (
      OUTPUT  ${oFiles}
      COMMAND ${pocioci} ${siFile} "-s${switchesList}"
      DEPENDS ${siFile}
      VERBATIM
    )
  endforeach()
  list (LENGTH soFiles noOf)
  if (0 LESS noOf)
    set (soFileList ${CMAKE_CURRENT_BINARY_DIR}/${projectName}.sofiles)
    #file (WRITE ${soFileList} "")
    foreach (soFile ${soFiles})
      #file (APPEND ${soFileList} "${soFile}\n")
      get_filename_component (temp ${soFile} NAME)
      string (TOLOWER ${temp} temp1)
      get_filename_component (filename ${temp1} NAME_WE)
      add_custom_command (
        OUTPUT  ${pcDir}/${filename}.pc ${pcDir}/${filename}.h
        COMMAND ${genproc} -o${pcDir} ${soFile}
        DEPENDS ${soFile}
      )
    endforeach()
    add_custom_target (${projectName} ALL
      DEPENDS ${pcFiles}
      SOURCES ${siFiles} ${pcFiles}
    )
  else ()
    add_custom_target (${projectName} ALL
      DEPENDS ${allFiles}
      SOURCES ${siFiles} ${allFiles}
    )
  endif ()
  set_source_files_properties (
    ${allFiles}
    PROPERTIES GENERATED TRUE
  )
endfunction()

function (proC2c name sourceDir binaryDir)
  set (iname  ${sourceDir}/${name}.pc)
  set (oname  ${binaryDir}/${name}.c)
  set (lname  ${binaryDir}/${name}.lis)
  set (includes INCLUDE=${sourceDir})
  foreach (arg ${ARGN})
    list (APPEND includes INCLUDE=${arg})
  endforeach ()
  string (REPLACE ";" " " includes "${includes}")
  set (command ${PROC} ${SQLCHECK} ${COMMON_PARSER} INAME=${iname} ONAME=${oname} LNAME=${lname} ${includes})
  separate_arguments(command)
  #message (STATUS "--info-- ${command}")
  add_custom_command (
    OUTPUT ${oname}
    COMMAND ${command}
    DEPENDS ${iname}
  )
  add_custom_target (${name}_proC2c ALL
    DEPENDS ${oname}
    SOURCES ${iname} ${ARGN}
  )
  include_directories ( ${ORACLE_INCLUDE}  )
endfunction ()

function (makewsdl output sources)
  set (switches)
  foreach (arg ${ARGN})
    list(APPEND switches "${arg}")
  endforeach ()
  #message (STATUS "--info-- switches ${switches}")
  #message (STATUS "--info-- ${WSDL2H} -o ${output} ${sources}")
  add_custom_command (
    OUTPUT ${output}
    COMMAND ${WSDL2H} -o ${output} ${sources} ${switches}
    DEPENDS ${sources}
  )
  get_filename_component (proj ${output} NAME_WE)
  add_custom_target (${proj}_makewsdl ALL
    DEPENDS ${output}
    SOURCES ${sources}
  )
endfunction ()

function (makesoap input soap_output)
  set (switches)
  foreach (arg ${ARGN})
    list(APPEND switches "${arg}")
  endforeach ()
  foreach (file in ${soap_output})
    get_filename_component (soap_dir ${file} PATH)
  endforeach ()
  #message (STATUS "--info-- ${SOAPCPP2} -C ${input} -d ${soap_dir}")
  add_custom_command (
    OUTPUT ${soap_output}
    COMMAND ${SOAPCPP2} -1 -I${GSOAP_SOURCE_DIR}/import -C ${input} -d ${soap_dir} ${switches}
    DEPENDS ${input}
  )
  get_filename_component (proj ${input} NAME_WE)
  add_custom_target (${proj}_makesoap ALL
    DEPENDS ${soap_output}
    SOURCES ${input}
  )
endfunction ()

function (gitversion)
  set (output ${EXECUTABLE_OUTPUT_PATH}/version.txt)
  add_custom_command (
    OUTPUT ${output} always
    COMMAND echo `git rev-parse --abbrev-ref HEAD`:`git rev-parse HEAD` > ${output}
  )
  add_custom_target (version ALL
    DEPENDS ${output}
  )
endfunction ()

# -- splits db portal switches based on the following rules
#  1: switch (individual switches)    -  "-o" "${genServerDir}" "PopGenServer"
#  2: dir|generator                   -  "${genServerDir}|PopGenServer"
#    "${genServerDir}|PopGenServer"
#       ==> "-o;${genServerDir};PopGenServer"
#  3: dir|generator|group             -  "${genServerDir}|PopGenServer|Server Files|.*[.]cpp$"
#    "${genServerDir}|PopGenServer|Server Files|.*[.]cpp$"

function (idl_split target_args)
  list (LENGTH target_args count)
  if (2 LESS count)
    list (GET target_args 2 arg)
    set (title ${arg})
  endif ()
  if (1 LESS count)
    list (GET target_args 0 arg)
    set (targetdir ${arg})
    list (GET target_args 1 arg)
    set (targetgen ${arg})
  endif ()
  set (groups PARENT_SCOPE)
  if (1 EQUAL count)
    set (switch ${target_args} PARENT_SCOPE)
  elseif (2 EQUAL count)
    set (switch "-o;${targetdir};${targetgen}" PARENT_SCOPE)
  elseif (3 EQUAL count)
    set (switch "-o;${targetdir};${targetgen}" PARENT_SCOPE)
    set (groups "${title};${targetdir}" PARENT_SCOPE)
  endif ()
endfunction ()

function (file_glob dir)
  file (GLOB globs 
    ${dir}/*.bas 
    ${dir}/*.cls 
    ${dir}/*.cpp 
    ${dir}/*.cs 
    ${dir}/*.def 
    ${dir}/*.h 
    ${dir}/*.idl 
    ${dir}/*.idl2 
    ${dir}/*.py 
    ${dir}/*.pty 
    ${dir}/*.sh 
    ${dir}/*.txt 
    ${dir}/*.sql
    ${dir}/*.yaml
    ${dir}/*.json
    )
  ##message (STATUS "--info-- GLOB ${globs}")
  set (files ${globs} PARENT_SCOPE)
endfunction()

function (idl2 projectName imFile iiDir ibDir)
  source_group ("IDL Source"  REGULAR_EXPRESSION ".*[.](im|ib)$")
  source_group ("IDL Built"   REGULAR_EXPRESSION ".*[.](ii|idl|idl2)$")
  set (switches)
  set (preswitches)
  set (allfiles)
  foreach (arg ${ARGN})
    string(REPLACE "|" ";" arg2 ${arg})
    idl_split("${arg2}")
    list (LENGTH switch noOf)
    if (1 EQUAL noOf)
      if ("-h" STREQUAL ${switch} OR "-s" STREQUAL ${switch})
        list (APPEND preswitches "${switch}")
      else ()
        list(APPEND switches "${switch}")
      endif ()
    else ()    
      list(APPEND switches "${switch}")
    endif ()
    list (LENGTH groups noOf)
    if (1 LESS noOf)
      list (GET groups 0 title)
      list (GET groups 1 dir)
      set (files)
      file_glob (${dir})
      #message (STATUS "--info-- ${title} ${dir} ${files}")
      source_group (${title} FILES ${files})
      list (APPEND allfiles ${files})
    endif ()  
  endforeach ()
  get_filename_component(temp ${imFile} NAME)
  get_filename_component(filename ${temp} NAME_WE)
  set (idlFile ${CMAKE_CURRENT_BINARY_DIR}/idl/${filename}.idl2)
  set (crackleJar ${TOOLS_DIR}/idl2/crackle.jar)
  file (GLOB ibFiles ${ibDir}/*.ib)
  file (GLOB iiFiles ${iiDir}/*.ii)
  set (simpleTarget ${CMAKE_CURRENT_BINARY_DIR}/${projectName})
  #message (STATUS "--info-- java -jar ${crackleJar} -i ${iiDir} -b ${ibDir} ${preswitches} -f ${idlFile} ${imFile} ${switches}")
  add_custom_command(
    OUTPUT ${idlFile}
    COMMAND java -jar ${crackleJar} -i ${iiDir} -b ${ibDir} ${preswitches} -f ${idlFile} ${imFile} ${switches}
    COMMAND echo "built" > ${simpleTarget}
    DEPENDS ${imFile} ${ibFiles} ${iiFiles}
    VERBATIM
  )
  add_custom_target (${projectName} ALL
    DEPENDS ${idlFile} ${simpleTarget}
    SOURCES ${imFile} ${ibFiles} ${iiFiles} ${idlFile} ${allfiles}
  )
endfunction()

function (idl2_simple projectName idlFile)
  source_group ("IDL Source"  REGULAR_EXPRESSION ".*[.](idl|idl2)$")
  set (crackleJar ${TOOLS_DIR}/idl2/crackle.jar)
  set (switches)
  set (preswitches)
  set (allfiles)
  foreach (arg ${ARGN})
    string(REPLACE "|" ";" arg2 ${arg})
    idl_split("${arg2}")
    if (1 EQUAL noOf)
      if ("-h" STREQUAL ${switch} OR "-s" STREQUAL ${switch})
        list (APPEND preswitches "${switch}")
      else ()
        list(APPEND switches "${switch}")
      endif ()
    else ()    
      list(APPEND switches "${switch}")
    endif ()
    list (LENGTH groups noOf)
    if (1 LESS noOf)
      list (GET groups 0 title)
      list (GET groups 1 dir)
      set (files)
      file_glob (${dir})
      source_group (${title} FILES ${files})
      list (APPEND allfiles ${files})
    endif ()  
  endforeach ()
  set (simpleTarget ${CMAKE_CURRENT_BINARY_DIR}/${projectName})
  #message (STATUS "--info-- java -jar ${crackleJar} ${preswitches} ${idlFile} ${switches}")
  add_custom_command(
    OUTPUT ${simpleTarget}
    COMMAND java -jar ${crackleJar} ${preswitches} ${idlFile} ${switches}
    COMMAND echo "built" > ${simpleTarget}
    DEPENDS ${idlFile}
    VERBATIM
  )
  add_custom_target (${projectName} ALL
    DEPENDS ${simpleTarget}
    SOURCES ${idlFile} ${allfiles}
  )
endfunction ()

function (jportal projectName siFiles)
  source_group ("SI Files"  REGULAR_EXPRESSION ".*[.]si$")
  set (logFiles)
  set (switches)
  set (allFiles)
  foreach (arg ${ARGN})
    string(REPLACE "|" ";" arg2 ${arg})
    idl_split("${arg2}")
    if (1 EQUAL noOf)
      list(APPEND switches "${switch}")
    else ()    
      list(APPEND switches "${switch}")
    endif ()
    list (LENGTH groups noOf)
    if (1 LESS noOf)
      list (GET groups 0 title)
      list (GET groups 1 dir)
      set (files)
      file_glob (${dir})
      source_group (${title} FILES ${files})
      list (APPEND allFiles ${files})
    endif ()  
  endforeach ()
  set (jportalJar ${TOOLS_DIR}/anydb/jportal.jar)
  foreach (siFile ${siFiles})
    get_filename_component (temp ${siFile} NAME)
    string (TOLOWER ${temp} temp1)
    get_filename_component (filename ${temp1} NAME_WE)
    set (logFile ${CMAKE_CURRENT_BINARY_DIR}/${filename}.log)
    list (APPEND logFiles ${logFile})
    add_custom_command (
      OUTPUT  ${logFile}
      COMMAND java -jar ${jportalJar} ${siFile} -l ${logFile} ${switches}
      DEPENDS ${siFile}
      VERBATIM
    )
  endforeach ()
  source_group (LogFiles FILES ${logFiles})
  #set (logFiles ${logFiles} PARENT_SCOPE)
  add_custom_target (${projectName} ALL
    DEPENDS ${logFiles}
    SOURCES ${siFiles} ${allFiles} ${logFiles}
  )
endfunction()

function (pickle projectName pmFile piDir prDir)
  source_group ("Source"  REGULAR_EXPRESSION ".*[.](pm|pr)$")
  source_group ("PI"      REGULAR_EXPRESSION ".*[.](pi)$")
  source_group ("Merged"  REGULAR_EXPRESSION ".*[.](pickle)$")
  set (switches)
  foreach (arg ${ARGN})
    list(APPEND switches "${arg}")
  endforeach ()
  get_filename_component(temp ${pmFile} NAME)
  get_filename_component(filename ${temp} NAME_WE)
  set (pickleFile ${CMAKE_CURRENT_BINARY_DIR}/pickle/${filename}.pickle)
  set (pickleJar ${TOOLS_DIR}/pickle/pickle.jar)
  file (GLOB prFiles ${prDir}/*.pr)
  file (GLOB piFiles ${piDir}/*.pi)
  #message (STATUS "--info-- java [-jar ${pickleJar}] [-p ${piDir}] [-r ${prDir}] [-f ${pickleFile}] ${pmFile} ${switches}")
  add_custom_command(
    OUTPUT ${pickleFile}
    COMMAND java -jar ${pickleJar} -p ${piDir} -r ${prDir} -f ${pickleFile} ${pmFile} ${switches}
    DEPENDS ${pmFile} ${piFiles} ${prFiles}
    VERBATIM
  )
  add_custom_target (${projectName} ALL
    DEPENDS ${pickleFile}
    SOURCES ${pmFile} ${piFiles} ${prFiles} ${pickleFile}
  )
endfunction()

function (lite3sql dbFile sqlFiles)
  source_group ("SQL Files"  REGULAR_EXPRESSION ".*[.](sql)$")
  get_filename_component(dbDir  ${dbFile} PATH)
  get_filename_component(dbName ${dbFile} NAME)
  set (batchFile ${dbDir}/build.bat)
  file (WRITE ${batchFile} "rem - build of lite3\n")
  set (lite3SQL ${TOOLS_DIR}/sqllite3/lite3SQL.py)
  foreach (sqlFile ${sqlFiles})
    get_filename_component(temp ${sqlFile} NAME)
    get_filename_component(tableName ${temp} NAME_WE)
    set (logName ${dbDir}/${tableName}.log)
    set (cmd "${PYTHON_EXECUTABLE} ${lite3SQL} -d ${dbName} ${sqlFile}")
    file (APPEND ${batchFile} "${cmd}\n")
  endforeach()
endfunction ()

function (install_file files destdir)
  file (INSTALL ${files} DESTINATION ${destdir} FILE_PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ GROUP_WRITE WORLD_READ)
endfunction ()

function (install_exec files destdir)
  file (INSTALL ${files} DESTINATION ${destdir} FILE_PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ)
endfunction ()
