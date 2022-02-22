project (tests)

set (TESTS_FOLDER tests)

set (BUILD_SQL_DIR C:/vanguard/build/sql)

add_subdirectory(oci_test)
add_subdirectory(odbc_oracle_test)
add_subdirectory(odbc_postgre_test)
add_subdirectory(db2_test)
