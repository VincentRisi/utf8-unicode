set (OCI_CLIENT C:/oracle/client${WIN_SIZE}-21.3)
set (ORACLE_LIBRARY_DIR ${OCI_CLIENT}/sdk/lib/msvc)
set (ORACLE_INCLUDE_DIR ${OCI_CLIENT}/sdk/include)
set (ocilibs 
  ${ORACLE_LIBRARY_DIR}/oci.lib
  ${ORACLE_LIBRARY_DIR}/ociw32.lib
  )
