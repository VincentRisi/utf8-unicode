project (zlib)

add_library(zlib
  src/adler32.c
  src/compress.c 
  src/crc32.c 
  src/crc32.h
  src/deflate.c 
  src/deflate.h
  src/example.c 
  src/gzio.c 
  src/infback.c 
  src/inffast.c 
  src/inffast.h
  src/inflate.c 
  src/inflate.h
  src/inftrees.c 
  src/inftrees.h
  src/minigzip.c 
  src/trees.c 
  src/trees.h
  src/uncompr.c 
  src/zlib.h
  src/zutil.c 
  src/zutil.h
)

set_target_properties(zlib PROPERTIES PROJECT_LABEL libs-zlib)
set_property(TARGET zlib PROPERTY FOLDER ${LIBS_FOLDER}/zlib)
