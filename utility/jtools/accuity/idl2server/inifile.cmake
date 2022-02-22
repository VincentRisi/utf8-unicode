set (service 32133)
file (TO_NATIVE_PATH ${ACCUITY_BINARY_DIR}/pyd PYD_DIR)
file (TO_NATIVE_PATH ${ACCUITY_BINARY_DIR}/bin/accuityserver.ini INI_FILE)
file (TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} SRC_DIR)

set (configure_source
  ${CMAKE_CURRENT_SOURCE_DIR}/accuityserver.vcxproj.user.in
  ${CMAKE_CURRENT_SOURCE_DIR}/accuityserver.cmd.in
  ${CMAKE_CURRENT_SOURCE_DIR}/accuityserver.sh.in
  )

if (WIN32)
  set (bindir ${ACCUITY_BINARY_DIR}/bin)
  set (logdir ${ACCUITY_BINARY_DIR}/bin)
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/accuityserver.vcxproj.user.in 
    ${CMAKE_CURRENT_BINARY_DIR}/accuityserver.vcxproj.user
    )
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/accuityserver.cmd.in 
    ${bindir}/accuityserver.cmd
    )
else ()
  set (bindir ${CMAKE_BINARY_DIR}/bin)
  set (logdir ${CMAKE_BINARY_DIR}/bin)
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/accuityserver.sh.in 
    ${bindir}/accuityserver.sh
    )
  execute_process(COMMAND chmod +x ${bindir}/accuityserver.sh)
endif ()

set (connect npu/npuqa@qn29)
set (outfile ${bindir}/accuityserver.ini)

log_info ("file (WRITE ${outfile}")
file (WRITE ${outfile}
  "[Server Options]\n"
  "{Service}${service}\n"
  "{Timeout}30000\n"
  "{Metrics}$(Metrics)\n"
  "{NoProcs}$(NoProcs)\n"
  "{Debug}$(Debug)\n\n"
  )

if (AIX OR LINUX)
  file (APPEND ${outfile}
    "{WaitForChildren}$(WaitForChildren)\n"
    "{RestartCount}$(RestartCount)\n"
    )
endif()

if (USE_OPENSSL)
  file (APPEND ${outfile}
    "{UseSSL}0\n"
    "{CertFile}\n"
    "{CAPath}\n"
    "{CAFile}\n"
    "{KeyFile}\n"
    "{KeyPassword}\n"
    )
endif ()

file (APPEND ${outfile}
  "{Log}${logdir}/accuityserver.log\n"
  "{LogLevel}$(LogLevel)\n"
  "{LogReceive}$(LogReceive)\n"
  "{LogTransmit}$(LogTransmit)\n"
  "{LogDisplay}$(LogDisplay)\n\n"
  )

file (APPEND ${outfile}
  "[DataBase]\n"
  "{BinFile}${logdir}/accuity.bin\n"
  "{Connection}${connect}\n"
  "{Timeout}30000\n\n"
  )

file (APPEND ${outfile}
  "[UPM]\n"
  "{User ID}ACCUITY\n"
  "{HostName}$(HN)\n\n"
  )

file (APPEND ${outfile}
  "[npu]\n"
  "{BinFile}${logdir}/accuity.bin\n"
  "{Connection}${connect}\n\n"
  )
