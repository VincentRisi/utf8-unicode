project (utfpp)

set (source utf.cpp utf.hpp)

add_library (utfpp ${source})

set_target_properties(utfpp PROPERTIES PROJECT_LABEL libs-utfpp)
set_property(TARGET utfpp PROPERTY FOLDER ${LIBS_FOLDER}/utfpp)
