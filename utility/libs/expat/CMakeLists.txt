project (libs-expat)

add_library(expat
  src/xmldef.h
  src/xmlparse.h 
  src/xmlparse.c 
  src/xmlrole.c 
  src/xmlrole.h
  src/xmltok.c 
  src/xmltok.h
)

set_target_properties(expat PROPERTIES PROJECT_LABEL libs-expat)
set_property(TARGET expat PROPERTY FOLDER ${LIBS_FOLDER}/exfat)
