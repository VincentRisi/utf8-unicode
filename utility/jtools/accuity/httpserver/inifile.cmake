set (service 32135)
file (TO_NATIVE_PATH ${ACCUITY_BINARY_DIR}/pyd PYD_DIR)
file (TO_NATIVE_PATH ${ACCUITY_BINARY_DIR}/bin/accuityhttp.ini INI_FILE)
file (TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} SRC_DIR)

set (configure_source
  ${CMAKE_CURRENT_SOURCE_DIR}/accuityhttp.vcxproj.user.in
  ${CMAKE_CURRENT_SOURCE_DIR}/accuityhttp.cmd.in
  ${CMAKE_CURRENT_SOURCE_DIR}/accuityhttp.sh.in
  )

if (WIN32)
  set (bindir ${ACCUITY_BINARY_DIR}/bin)
  set (logdir ${ACCUITY_BINARY_DIR}/bin)
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/accuityhttp.vcxproj.user.in 
    ${CMAKE_CURRENT_BINARY_DIR}/accuityhttp.vcxproj.user
    )
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/accuityhttp.cmd.in 
    ${bindir}/accuityhttp.cmd
    )
else ()
  set (bindir ${CMAKE_BINARY_DIR}/bin)
  set (logdir ${CMAKE_BINARY_DIR}/bin)
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/accuityhttp.sh.in 
    ${bindir}/accuityhttp.sh
    )
  execute_process(COMMAND chmod +x ${bindir}/accuityhttp.sh)
endif ()

set (connect npu/npudev@dn29)
set (outfile ${bindir}/accuityhttp.ini)

log_info ("file (WRITE ${outfile}")
file (WRITE ${outfile}
  "[Server Options]\n"
  "{Service}${service}\n"
  "{Timeout}30000\n"
  "{Metrics}$(Metrics)\n"
  )

if (AIX OR LINUX)
  file (APPEND ${outfile}
    "{NoProcs}$(NoProcs)\n"
    "{Debug}$(Debug)\n\n"
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
  "{Log}${logdir}/accuityhttp.log\n"
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
  "[npu]\n"
  "{BinFile}${logdir}/accuity.bin\n"
  "{Connection}${connect}\n\n"
  )

if (WIN32)
  set (bindir ${ACCUITY_BINARY_DIR}/bin)
  set (logdir ${ACCUITY_BINARY_DIR}/log)

  set (pyfile ${bindir}/accuitypy.ini)
  log_info ("file (WRITE ${pyfile}")
  file (WRITE ${pyfile}
    "[Server Options]\n"
    "{Service}${service}\n"
    "{Log}{logdir}/accuityhttp.log\n"
    "{LogLevel}$(LogLevel)\n"
    "{LogReceive}$(LogReceive)\n"
    "{LogTransmit}$(LogTransmit)\n"
    "{LogDisplay}$(LogDisplay)\n\n"
    "[npu]\n"
    "{BinFile}${bindir}/accuity.bin\n"
    "{Connection}npu/npudev@dn29\n\n"
    )
endif ()

