project (libs-main)

add_library(main
  src/addlist.h
  src/amutex.h
  src/appendlist.h
  src/aqueue.h
  src/ascii85.cpp src/ascii85.h
  src/athread.cpp src/athread.h
  src/autosemaphore.cpp src/autosemaphore.h
  src/base64.cpp src/base64.h
  src/basemacros.h
  src/bdatetime.cpp src/bdatetime.h
  src/blackboard.h
  src/bstring.cpp src/bstring.h
  src/cbsocket.cpp src/cbsocket.h
  src/cmdlinearg.cpp src/cmdlinearg.h
  src/collection.h
  src/config.cpp src/config.h
  src/databuilder2.h
  src/databuilderjp.h
  src/filehandler.cpp src/filehandler.h
  src/getargs.cpp src/getargs.h
  src/handles.h
  src/heapqueue.h
  src/hugechar.h
  src/logfile.cpp src/logfile.h
  src/machine.h
  src/md5.cpp src/md5.h
  src/mutex.cpp src/mutex.h
  src/nedgen.h
  src/obsocket.cpp src/obsocket.h
  src/padgen.h
  src/percent_encoding.cpp src/percent_encoding.h
  src/rdc.cpp src/rdc.h
  src/regexp.cpp src/regexpc.h
  src/splits.cpp src/splits.h
  src/swapbytes.h
  src/tbuffer.h
  src/versions.h
  src/xcept.cpp src/xcept.h
  src/xml_databuild.cpp src/xml_databuild.h
  src/xmlcore.cpp src/xmlcore.h
  src/xmlmake.cpp src/xmlmake.h
  src/xmlrecord.cpp src/xmlrecord.h
  src/xstring.cpp src/xstring.h
  src/z85.cpp src/z85.h
)

target_link_libraries (main ti expat utfpp)

set_target_properties(main PROPERTIES PROJECT_LABEL libs-main)
set_property(TARGET main PROPERTY FOLDER ${LIBS_FOLDER}/main)
