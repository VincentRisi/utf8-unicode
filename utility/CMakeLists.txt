project (utility)

include (logging.cmake)

cmake_minimum_required(VERSION 3.20)
option (USE_FOLDERS "Use Folders" ON)
set_property (GLOBAL PROPERTY USE_FOLDERS ${USE_FOLDERS})

if (WIN32)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set (WIN_SIZE 64 CACHE STRING "Window Size")
  else ()  
    set (WIN_SIZE 32 CACHE STRING "Window Size")
  endif ()
endif ()

if (UNIX)
  exec_program (uname OUTPUT_VARIABLE SYSTEM_NAME)
  set (SYSTEM_NAME "${SYSTEM_NAME}" CACHE INTERNAL "")
  if (SYSTEM_NAME STREQUAL "Linux")
    set (LINUX ON)
  elseif (SYSTEM_NAME STREQUAL "FreeBSD")
    set (FREEBSD ON)
  else ()
    set (AIX ON)
  endif ()
endif ()

if (LINUX)
  log_info ("LINUX")
  set (switches " -g -fPIC -Wno-write-strings -fpermissive")
  list (APPEND CMAKE_C_FLAGS "${switches}")
  list (APPEND CMAKE_CXX_FLAGS "${switches}")
  string (REPLACE ";" " " CMAKE_C_FLAGS   ${CMAKE_C_FLAGS})
  string (REPLACE ";" " " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set (switches " -pthread")
  list (APPEND CMAKE_EXE_LINKER_FLAGS "${switches}")
  list (APPEND CMAKE_MODULE_LINKER_FLAGS "${switches}")
  list (APPEND CMAKE_SHARED_LINKER_FLAGS "${switches}")
  string (REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS    ${CMAKE_EXE_LINKER_FLAGS})
  string (REPLACE ";" " " CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS})
  string (REPLACE ";" " " CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS})
elseif (WIN32)
  log_info ("WIN32")
  add_definitions (
    -D_CRT_SECURE_NO_WARNINGS
    -D_SCL_SECURE_NO_WARNINGS
    )
  string (REPLACE "/MD" "/MT" CMAKE_C_FLAGS_DEBUG            ${CMAKE_C_FLAGS_DEBUG})
  string (REPLACE "/MD" "/MT" CMAKE_C_FLAGS_MINSIZEREL       ${CMAKE_C_FLAGS_MINSIZEREL})
  string (REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE          ${CMAKE_C_FLAGS_RELEASE})
  string (REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELWITHDEBINFO   ${CMAKE_C_FLAGS_RELWITHDEBINFO})
  string (REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG})
  string (REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL     ${CMAKE_CXX_FLAGS_MINSIZEREL})
  string (REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE})
  string (REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
elseif (AIX)
  log_info ("AIX")
  set (CMAKE_C_FLAGS                  "-qthreaded -qalias=noansi -qhalt=e")
  set (CMAKE_C_FLAGS_DEBUG            "-g -qfullpath -qdbxextra -bloadmap:loadmap.map")
  set (CMAKE_C_FLAGS_MINSIZEREL       "-O -DNDEBUG")
  set (CMAKE_C_FLAGS_RELEASE          "-O -DNDEBUG")
  set (CMAKE_C_FLAGS_RELWITHDEBINFO   "-g")
  set (CMAKE_CXX_FLAGS                "-+ -g -qfullpath -qdbxextra -bloadmap:loadmap.map -qthreaded -qhalt=e -qstaticinline -qfuncsect -qrtti -qro -qroconst -qunique -qcheck=all -qformat=all -qinfo=ini")
  set (CMAKE_CXX_FLAGS_MINSIZEREL     "-O -DNDEBUG")
  set (CMAKE_CXX_FLAGS_RELEASE        "-O -DNDEBUG")
  set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g")
endif ()

if (WIN32)
  set (PYTHON_HOME           "C:/Python37")
  set (PYTHON_SCRIPTS        "${PYTHON_HOME}/Scripts")
  set (PYTHON_INCLUDE_PATH   "${SUPPORT_DIR}/python37/include")
  link_directories (${SUPPORT_DIR}/python37/libs)
  set (PYTHON_LIBRARIES       python37 python37_d)
  set (PYTHON_LIB             "${PYTHON_HOME}/Lib")
  set (PYTHON_BUILD_LIBRARIES ${PYTHON_LIBRARIES})
  set (PYTHON_EXECUTABLE     "${PYTHON_HOME}/python.exe")
elseif (LINUX)
  set (PYTHON_HOME            "$ENV{PYTHONHOME}")
  set (PYTHON_INCLUDE_PATH    "${PYTHON_HOME}/include/python3.7m")
  set (PYTHON_LIBRARIES       "${PYTHON_HOME}/lib/python3.7/config-3.7m-x86_64-linux-gnu/libpython3.7m.a")
  list (APPEND PYTHON_LIBRARIES ${LIB_UTIL})
  set (PYTHON_LIB             "${PYTHON_HOME}/Lib")
  set (PYTHON_PIC_LIBRARIES   "${PYTHON_HOME}/lib/libpython3.7m.so")
  set (PYTHON_BUILD_LIBRARIES ${PYTHON_LIBRARIES})
  set (PYTHON_EXECUTABLE      "${PYTHON_HOME}/bin/python3")
  set (PYTHON_CONFIG_DIR      "${PYTHON_HOME}/lib/python3.7/config-3.7m-x86_64-linux-gnu")
elseif (AIX)
  set (PYTHON_HOME            "$ENV{PYTHONHOME}")
  set (PYTHON_INCLUDE_PATH    "${PYTHON_HOME}/include/python3.7m")
  set (PYTHON_LIBRARIES       "${PYTHON_HOME}/lib/python3.7/config-3.7m/libpython3.7m.a")
  set (PYTHON_BUILD_LIBRARIES ${PYTHON_LIBRARIES})
  set (PYTHON_LIB             "${PYTHON_HOME}/Lib")
  set (PYTHON_EXECUTABLE      "${PYTHON_HOME}/bin/python")
  set (PYTHON_CONFIG_DIR      "${PYTHON_HOME}/lib/python3.7/config-3.7m")
  add_definitions (-D_LARGE_FILES)
endif ()

set (DBPORTAL_SOURCE_DIR ${CMAKE_SOURCE_DIR}/dbportal)
set (DBPORTAL_BINARY_DIR ${CMAKE_SOURCE_DIR}/dbportal/build)
set (LIBS_SOURCE_DIR     ${CMAKE_SOURCE_DIR}/libs)
set (LIBS_BINARY_DIR     ${CMAKE_SOURCE_DIR}/libs/build)
set (JTOOLS_SOURCE_DIR   ${CMAKE_SOURCE_DIR}/jtools)
set (JTOOLS_BINARY_DIR   ${CMAKE_SOURCE_DIR}/jtools/build)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin"   CACHE FILEPATH "Single output directory for building all executeables")
set(LIBRARY_OUTPUT_PATH    "${CMAKE_BINARY_DIR}/lib"   CACHE FILEPATH "Single output directory for building all libraries")

include_directories (
  ${LIBS_SOURCE_DIR}/expat
  ${LIBS_SOURCE_DIR}/expat/src
  ${LIBS_SOURCE_DIR}/zlib
  ${LIBS_SOURCE_DIR}/zlib/src
  ${LIBS_SOURCE_DIR}/ti
  ${LIBS_SOURCE_DIR}/ti/src
  ${LIBS_SOURCE_DIR}/main
  ${LIBS_SOURCE_DIR}/main/src
  ${LIBS_SOURCE_DIR}/http
  ${LIBS_SOURCE_DIR}/http/src
  ${LIBS_SOURCE_DIR}/jsoncpp
  ${LIBS_SOURCE_DIR}/jsoncpp/src
  )

add_subdirectory (dbportal)
add_subdirectory (libs)
add_subdirectory (tests)
add_subdirectory (jtools)
