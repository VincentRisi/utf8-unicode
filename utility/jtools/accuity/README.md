# Notes on usage of idl2 http using python3 code and pragma for restful generation.

## Other Notes

### from idl2 CMakeLists.txt

We use genned which is in .gitignore for the BINARY_DIR.

``` cmake
...
  set (ACCUITY_SOURCE_DIR ${PUTTY3_SOURCE_DIR}/idl2/accuity)
  set (ACCUITY_BINARY_DIR ${PUTTY3_SOURCE_DIR}/idl2/accuity/genned)
...
```

### from idl2/accuity CMakeLists.txt

We only do the swagger_build if in Windows

``` cmake
...
add_subdirectory (sql_build)

add_subdirectory (idl2_build)

if (WIN32)
...
  add_subdirectory (swagger_build)
endif ()

add_subdirectory (py_zip)

add_subdirectory (httpserver)
...
```

### idl2/accuity/sql_build CMakeLists.txt

We only use the swagger and openapi switches if in Windows

``` cmake
project (putty3.idl2.accuity.sql_build)

pathed(iiDir       ${ACCUITY_BINARY_DIR}/ii)
pathed(shDir       ${ACCUITY_BINARY_DIR}/sh)
pathed(soDir       ${ACCUITY_BINARY_DIR}/so)
pathed(sqlDir      ${ACCUITY_BINARY_DIR}/sql)
pathed(binDir      ${ACCUITY_BINARY_DIR}/bin)
pathed(logDir      ${ACCUITY_BINARY_DIR}/log)
pathed(pyDir       ${ACCUITY_BINARY_DIR}/py)
pathed(pyDBADir    ${ACCUITY_BINARY_DIR}/pydba)
pathed(piDir       ${ACCUITY_BINARY_DIR}/pi)

set (switches 
  "ConnReqd=0"
  "OneSQLScript=1"
  "Internal=1"
  "I3Gen=0"
  "Python3=1"
  "XmlValue=1"
  "ControlDB=npud00"
  "UConnect=npu"
  "C|${shDir}|.sh|SH Files"
  "SO|${soDir}|.so"
  "SQL|${sqlDir}|.sql|SQL Files"
  "LogDir=${logDir}"
  "Python|${pyDir}|.py|PY_Files"
  "PyDBApi|${pyDBADir}|.py|DB_Api"
  "ParmsDir=${piDir}"
)

if (WIN32)
...  
  pathed(swaggerDir  ${ACCUITY_BINARY_DIR}/yaml2)
  pathed(openapiDir  ${ACCUITY_BINARY_DIR}/yaml3)
  list (APPEND switches
...
    "Swagger|${swaggerDir}|.yaml|Swagger"
    "Openapi|${openapiDir}|.yaml|OpenApi"
    )
endif ()

set (accuity_list
  accuityroutingcode.si
  accuitycontact.si
  accuitycorrespondent.si
  accuitycorrespondentsub.si
  accuitylocation.si
  accuityofficer.si
  accuityroutingatt.si
  audits.si
  bankfile.si
)

set (accuity_sifiles)
foreach (name ${accuity_list})
  list (APPEND accuity_sifiles ${MCPE_SOURCE_DIR}/sql/si/${name})
endforeach()

dbportal2(accuity_sql_build accuity "${accuity_sifiles}" ${switches})

set_property(TARGET accuity_sql_build  PROPERTY FOLDER "putty3/idl2/accuity/sql_build")
```

### idl2/accuity/idl2_build CMakeLists.txt

We use PopHTTPServerFull for restful, also if windows PopHTTPSwaggerFull and PopHTTPOpenApiFull.
We write a config file for the PopHTTPSwaggerFull and PopHTTPOpenApiFull generators for finding the sql generated files.
Note the pragmas for the config file in the accuity.idl2 file below.

``` cmake
project (putty3.idl2.accuity.idl2_build)

pathed (genServerDir     ${ACCUITY_BINARY_DIR}/server/idl)
pathed (genPythonDir     ${ACCUITY_BINARY_DIR}/server/py)
pathed (genPythonTreeDir ${ACCUITY_BINARY_DIR}/tree)
pathed (genHttpServerDir ${ACCUITY_BINARY_DIR}/http)

set (switches
  "${genServerDir}|PopGenServer|Server Files"
  "${genHttpServerDir}|PopHTTPServerFull|HTTP Server Files"
  "${genPythonDir}|PopGenPython|PY_Files"
  "${genPythonTreeDir}|PopGenPythonTree|PY_Files"
)

if (WIN32)
  pathed (genCSharpDir     ${ACCUITY_BINARY_DIR}/client/cs)
  list (APPEND switches
    "${genCSharpDir}|PopGenCSharp|CSharp Files"
    "${genHttpServerDir}|PopHTTPSwaggerFull|HTTP Swagger Files"
    "${genHttpServerDir}|PopHTTPOpenApiFull|HTTP Swagger Files"
    )
  set (configFile ${genHttpServerDir}/accuity.properties)
  file (WRITE ${configFile})
  file (APPEND ${configFile} "urlPrefix=${ACCUITY_BINARY_DIR}\n")
  file (APPEND ${configFile} "defSqlSub=/yaml2\n")
  file (APPEND ${configFile} "compSqlSub=/yaml3\n")
endif ()

set (idlFile ${CMAKE_CURRENT_SOURCE_DIR}/accuity.idl2)
idl2_simple(accuity_idl2_build ${idlFile} ${switches})

add_dependencies (accuity_idl2_build accuity_sql_build)

set_property(TARGET accuity_idl2_build PROPERTY FOLDER "putty3/idl2/accuity/idl2_build")
```

#### accuity.idl2

Note the openapi: pragmas as discussed above.
Also note the use PYTHON: in the code ... endcode blocks flagging we are using Python code.
The idl2 methods ping and pong do the same thing. ping uses Python code and pong use C code.

**pragma ping:get ping/given/add Utility** flags using a restful **get** with *given* and *add* in *"GET ping/given/add"* format.

**pragma pong:get pong/{given}/add/{add} Utility** flags using a restful **get** with *given* and *add* in *"GET pong/?/add/?"* format.

**pragma accuityContactInsert:post accuityContact Contact**

**pragma accuityContactUpdate:put accuityContact Contact**

**pragma accuityContactUpdateAllStatus:patch accuityContact Contact**

**pragma accuityContactSelectOne:get accuityContact/Id Contact**

**pragma accuityContactDeleteOne:delete accuityContact/Id Contact**

**pragma accuityContactExists:get accuityContact-Exists/Id Contact**

``` python
module Accuity;
version "19.07.13.0";

pragma openapi: description: Ingress3 ATP Server
pragma openapi: urlPrefix: $urlPrefix
pragma openapi: defSqlSub: $defSqlSub
pragma openapi: compSqlSub: $compSqlSub
pragma openapi: idl2Sub: $idl2Sub
pragma JSON

code
BOTH:using namespace std;
#include "date.h"
#include "xmlrecord.h"
#include "putty3shell.h"
endcode

message
{
  DB_ERROR        "DB Exception Error."
  XCEPT_ERROR     "Generic Exception Error."
  SCRIPT_ERROR    "Script Error."
  PYTHON_ERROR    "Python Exception Error."
  NOT_YET_DONE    "This has not yet been done."
}

private struct Accuity
{
  TBChar errors;
  Putty3Shell *putty3;
  char binFileName[256];
  char connectString[64];
code
  Connect.CB()->isVB = 0;
  LoadConfig();
  putty3 = new Putty3Shell(&LogFile, "AccuityServer", ConfigFile, "ACCUITY");
  runPython = putty3Runner;
onshutdown:
  delete putty3;
endcode
}

struct tBananaRama
{
   int count;
   double whipped;
}

code
template <size_t N>
inline char* zzcopy(char(&target)[N], const char *source)
{ 
  char* result = strncpy(target, source, N-1); 
  target[N-1] = 0;
  return result;
}

inline void queryValue(char* work, int len, tINI &ini, const char* key, const char* section)
{
  tString temp;
  ini.QueryValue(key, temp, section);
  strncpy(work, temp.c_str(), len-1);
  work[len-1] = 0;
}
endcode

private void LoadConfig()
{
code
  tINI ini(ConfigFile);
  const int len=256;
  char work[len];
  queryValue(work, len, ini, "{BinFile}",    "DataBase");  zzcopy(binFileName, work);
  queryValue(work, len, ini, "{Connection}", "DataBase");  zzcopy(connectString, work);
endcode
}

code
PYTHON:
from INTRINSICS import *
from DB_AUDITS import DBAudits

actions = ['ADD', 'CHANGE', 'DELETE']
ADD, CHANGE, DELETE = 0, 1, 2

def amp_fix(value, asis=False):
    ''' 
      asis=True - leaves previous escapes 
    '''
    escapes = [('&', '&amp;'), ('<','&lt;'), ('>', '&gt;'), ('"','&quot;'), ("'",'&apos;')]
    if asis == False:
        for pair in escapes:
            value = value.replace(pair[1], pair[0])
    for pair in escapes:
        value = value.replace(pair[0], pair[1])
    return value

def make_xml(name, action, fields, values):
    usid = ''
    xml = '<%s action="%s">\r\n' % (name, actions[action])
    for i, field in enumerate(fields):
        value = str(values[i]) if len(values) == len(fields) else ''
        xml += '  <%s value="%s"/>\r\n' % (field, amp_fix(value))
        if field.lower() == 'usid':
            usid = value
    xml += '</%s>' % (name)
    return usid, xml

def write_audit(name, action, fields, old, new):
    if actions[action] != 'ADD':
        usid, xmlOld = make_xml(name, action, fields, old)
    else:
        xmlOld = ' '
    if actions[action] != 'DELETE':
        usid, xmlNew = make_xml(name, action, fields, new)
    else:
        xmlNew = ' '
    audit = DBAudits(connect)
    tmStamp = '20190101000000'
    audit.runInsert(0, name, actions[action], xmlOld, xmlNew, usid, tmStamp)
endcode

pragma ping:get ping/given/add Utility
int ping(int given, int add)
{
message: #
input 
    given;
    add;
code
PYTHON: 
    return given + add
endcode
}

pragma pong:get pong/{given}/add/{add} Utility
int pong(int given, int add)
{
message: #
input 
    given;
    add;
code
    return given + add;
endcode
}

pragma pang:get pang/{given}/add/{add}/and/{gotten} Utility
int pang(int given, int add, string gotten)
{
message: #
input 
    given;
    add;
    gotten;
code
PYTHON:
    log_info(gotten)
    return given + add;
endcode
}

// get post put patch delete head options trace
pragma banana:options banana Utility
void banana(tBananaRama *splits)
{
message: #
input
    splits;
code
PYTHON:
    return
endcode
}

pragma bananaFor:options banana/{opt} Utility
void bananaFor(int opt, tBananaRama *splits)
{
message: #

input
    opt;
output
    splits;
code
PYTHON:
    return
endcode
}

//------------------------------------------------------------------------------

struct tAccuityContact "accuitycontact.sh"

pragma accuityContactInsert:post accuityContact Contact
void accuityContactInsert(tAccuityContact *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        write_audit('AccuityContact', ADD, newRec._fields(), None, newRec._data())
        newRec.execInsert()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

pragma accuityContactUpdate:put accuityContact Contact
void accuityContactUpdate(tAccuityContact *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        oldRec = DBAccuityContact(connect)
        oldRec.Id = newRec.Id
        oldRec.execSelectOne()
        write_audit('AccuityContact', CHANGE, newRec._fields(), oldRec._data(), newRec._data())
        newRec.execUpdate()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

struct tAccuityContactUpdateAllStatus "accuitycontact.sh"
pragma accuityContactUpdateAllStatus:patch accuityContact Contact
void accuityContactUpdateAllStatus(int status, char* usId)
{
message: #
input
  status;
  usId;
code
PYTHON:
    try:
        rec = DBAccuityContactUpdateAllStatus(connect)
        rec.runUpdateAllStatus(status, usId)
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

pragma accuityContactSelectOne:get accuityContact/Id Contact
void accuityContactSelectOne(int Id, tAccuityContact *rec)
{
message: #
input
    Id;
output
    rec;
code
PYTHON:
    try:
        rec = DBAccuityContact(connect)
        rec.Id = Id
        rec.execSelectOne()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
    return rec
endcode
}

struct tAccuityContactDeleteOne "accuitycontact.sh"

pragma accuityContactDeleteOne:delete accuityContact/Id Contact
void accuityContactDeleteOne(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityContactDeleteOne(connect)
        rec.Id = Id
        rec.execDeleteOne()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityContactExists "accuitycontact.sh"
pragma accuityContactExists:get accuityContact-Exists/Id Contact
int accuityContactExists(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityContactExists(connect)
        if rec.readExists(Id) == 1:
            return rec.Count
        else:
            return 0
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

//------------------------------------------------------------------------------

struct tAccuityCorrespondent "accuitycorrespondent.sh"

pragma accuityCorrespondentInsert:post accuityCorrespondent Correspondent
void accuityCorrespondentInsert(tAccuityCorrespondent *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        write_audit('AccuityCorrespondent', ADD, newRec._fields(), None, newRec._data())
        newRec.execInsert()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

pragma accuityCorrespondentUpdate:put accuityCorrespondent Correspondent
void accuityCorrespondentUpdate(tAccuityCorrespondent *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        oldRec = DBAccuityCorrespondent(connect)
        oldRec.Id = newRec.Id
        oldRec.execSelectOne()
        write_audit('AccuityCorrespondent', CHANGE, newRec._fields(), oldRec._data(), newRec._data())
        newRec.execUpdate()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

struct tAccuityCorrespondentUpdateAllStatus "accuitycorrespondent.sh"
pragma accuityCorrespondentUpdateAllStatus:patch accuityCorrespondent Correspondent
void accuityCorrespondentUpdateAllStatus(int status, char* usId)
{
message: #
input
  status;
  usId;
code
PYTHON:
    try:
        rec = DBAccuityCorrespondentUpdateAllStatus(connect)
        rec.runUpdateAllStatus(status, usId)
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

pragma accuityCorrespondentSelectOne:get accuityCorrespondent/Id Correspondent
void accuityCorrespondentSelectOne(int Id, tAccuityCorrespondent *rec)
{
message: #
input
    Id;
output
    rec;
code
PYTHON:
    try:
        rec = DBAccuityCorrespondent(connect)
        rec.Id = Id
        rec.execSelectOne()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
    return rec
endcode
}

struct tAccuityCorrespondentDeleteOne "accuitycorrespondent.sh"

pragma accuityCorrespondentDeleteOne:delete accuityCorrespondent/Id Correspondent
void accuityCorrespondentDeleteOne(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityCorrespondentDeleteOne(connect)
        rec.Id = Id
        rec.execDeleteOne()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityCorrespondentExists "accuitycorrespondent.sh"
pragma accuityCorrespondentExists:get accuityCorrespondent-Exists/Id Correspondent
int accuityCorrespondentExists(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityCorrespondentExists(connect)
        if rec.readExists(Id) == 1:
            return rec.Count
        else:
            return 0
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityCorrespondentGet "accuitycorrespondent.sh"
pragma accuityCorrespondentGet:get accuityCorrespondent-Get/FinId/CorrCurrency Correspondent
int accuityCorrespondentGet(int FinId, char* CorrCurrency, tAccuityCorrespondentGet* rec)
{
message: #
input
    FinId;
    CorrCurrency;
output
    rec;    
code
PYTHON:
    try:   
        rec = DBAccuityCorrespondentGet(connect)
        rec.FinId = FinId
        rec.CorrCurrency = CorrCurrency;
        return rec.readGet(FinId, CorrCurrency), rec
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityCorrespondentGetSwiftList "accuitycorrespondent.sh"
pragma accuityCorrespondentGetSwiftList:get accuityCorrespondent-SwiftList/FinId/CorrCurrency Correspondent
int accuityCorrespondentGetSwiftList(int FinId, char* CorrCurrency, int* NoOf, tAccuityCorrespondentGetSwiftList*& Recs)
{
message: #
input
    FinId;
    CorrCurrency;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityCorrespondentGetSwiftList(connect)
        rec.FinId = FinId
        rec.CorrCurrency = CorrCurrency;
        recs = rec.loadGetSwiftList()
        noOf = len(recs);
        rc = 1 if noOf > 0 else 0
        return rc, noOf, recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

//------------------------------------------------------------------------------

struct tAccuityCorrespondentSub "accuitycorrespondentsub.sh"

pragma accuityCorrespondentSubInsert:post accuityCorrespondentSub CorrespondentSub
void accuityCorrespondentSubInsert(tAccuityCorrespondentSub *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        write_audit('AccuityCorrespondentSub', ADD, newRec._fields(), None, newRec._data())
        newRec.execInsert()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

pragma accuityCorrespondentSubUpdate:put accuityCorrespondentSub CorrespondentSub
void accuityCorrespondentSubUpdate(tAccuityCorrespondentSub *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        oldRec = DBAccuityCorrespondentSub(connect)
        oldRec.Id = newRec.Id
        oldRec.execSelectOne()
        write_audit('AccuityCorrespondentSub', CHANGE, newRec._fields(), oldRec._data(), newRec._data())
        newRec.execUpdate()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

struct tAccuityCorrespondentSubUpdateAllStatus "accuitycorrespondentsub.sh"
pragma accuityCorrespondentSubUpdateAllStatus:patch accuityCorrespondentSub CorrespondentSub
void accuityCorrespondentSubUpdateAllStatus(int status, char* usId)
{
message: #
input
  status;
  usId;
code
PYTHON:
    try:
        rec = DBAccuityCorrespondentSubUpdateAllStatus(connect)
        rec.runUpdateAllStatus(status, usId)
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

pragma accuityCorrespondentSubSelectOne:get accuityCorrespondentSub/Id CorrespondentSub
void accuityCorrespondentSubSelectOne(int Id, tAccuityCorrespondentSub *rec)
{
message: #
input
    Id;
output
    rec;
code
PYTHON:
    try:
        rec = DBAccuityCorrespondentSub(connect)
        rec.Id = Id
        rec.execSelectOne()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
    return rec
endcode
}

struct tAccuityCorrespondentSubDeleteOne "accuitycorrespondentsub.sh"

pragma accuityCorrespondentSubDeleteOne:delete accuityCorrespondentSub/Id CorrespondentSub
void accuityCorrespondentSubDeleteOne(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityCorrespondentSubDeleteOne(connect)
        rec.Id = Id
        rec.execDeleteOne()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityCorrespondentSubExists "accuitycorrespondentsub.sh"
pragma accuityCorrespondentSubExists:get accuityCorrespondentSub-Exists/Id CorrespondentSub
int accuityCorrespondentSubExists(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityCorrespondentSubExists(connect)
        if rec.readExists(Id) == 1:
            return rec.Count
        else:
            return 0
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

//------------------------------------------------------------------------------

struct tAccuityLocation "accuitylocation.sh"

pragma accuityLocationInsert:post accuityLocation Location
void accuityLocationInsert(tAccuityLocation *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        write_audit('AccuityLocation', ADD, newRec._fields(), None, newRec._data())
        newRec.execInsert()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

pragma accuityLocationUpdate:put accuityLocation Location
void accuityLocationUpdate(tAccuityLocation *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        oldRec = DBAccuityLocation(connect)
        oldRec.LocationId = newRec.LocationId
        oldRec.execSelectOne()
        write_audit('AccuityLocation', CHANGE, newRec._fields(), oldRec._data(), newRec._data())
        newRec.execUpdate()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

struct tAccuityLocationUpdateAllStatus "accuitylocation.sh"
pragma accuityLocationUpdateAllStatus:patch accuityLocation Location
void accuityLocationUpdateAllStatus(int status, char* usId)
{
message: #
input
  status;
  usId;
code
PYTHON:
    try:
        rec = DBAccuityLocationUpdateAllStatus(connect)
        rec.runUpdateAllStatus(status, usId)
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

pragma accuityLocationSelectOne:get accuityLocation/Id Location
void accuityLocationSelectOne(int Id, tAccuityLocation *rec)
{
message: #
input
    Id;
output
    rec;
code
PYTHON:
    try:
        rec = DBAccuityLocation(connect)
        log_debug(f'LocationSelecetOne Id:{Id}')
        rec.LocationId = Id
        rec.execSelectOne()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
    return rec
endcode
}

struct tAccuityLocationDeleteOne "accuitylocation.sh"

pragma accuityLocationDeleteOne:delete accuityLocation/Id Location
void accuityLocationDeleteOne(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityLocationDeleteOne(connect)
        log_debug(f'LocationDeleteOne Id:{Id}')
        rec.LocationId = Id
        rec.execDeleteOne()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationExists "accuitylocation.sh"
pragma accuityLocationExists:get accuityLocation-Exists/Id Location
int accuityLocationExists(int LocationId)
{
message: #
input
    LocationId;
code
PYTHON:
    try:
        rec = DBAccuityLocationExists(connect)
        log_debug(f'LocationExists LocationId:{LocationId}')
        if rec.readExists(LocationId) == 1:
            return rec.Count
        else:
            return 0
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationGet "accuitylocation.sh"
pragma accuityLocationGet:get accuityLocation-Get/FinId Location
int accuityLocationGet(int FinId, tAccuityLocationGet* rec)
{
message: #
input
    FinId;
output
    rec;    
code
PYTHON:
    try:
        rec = DBAccuityLocationGet(connect)
        log_debug(f'LocationGet FinId:{FinId}')
        rec.FinId = FinId
        return rec.readGet(FinId), rec
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationGetBankCode "accuitylocation.sh"
pragma accuityLocationGetBankCode:get accuityLocation-BankCode/FinId Location
int accuityLocationGetBankCode(int FinId, tAccuityLocationGetBankCode* rec)
{
message: #
input
    FinId;
output
    rec;    
code
PYTHON:
    try:
        rec = DBAccuityLocationGetBankCode(connect)
        log_debug(f'LocationGetBankCode FinId:{FinId}')
        rec.FinId = FinId
        return rec.readGetBankCode(FinId), rec
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationFinIdBankCodeCheck "accuitylocation.sh"
pragma accuityLocationFinIdBankCodeCheck:get accuityLocation-Check/FinId/NationalBankCode Location
int accuityLocationFinIdBankCodeCheck(int FinId, char* NationalBankCode, tAccuityLocationFinIdBankCodeCheck* rec)
{
message: #
input
    FinId;
    NationalBankCode;
output
    rec;    
code
PYTHON:
    try:
        rec = DBAccuityLocationFinIdBankCodeCheck(connect)
        log_debug(f'LocationFinIdBankCodeCheck FinId:{FinId} NationalBankCode:{NationalBankCode}')
        rec.FinId = FinId
        rec.NationalBankCode = NationalBankCode
        return rec.readFinIdBankCodeCheck(FinId, NationalBankCode), rec
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationSwiftLookup "accuitylocation.sh"
pragma accuityLocationSwiftLookup:get accuityLocation-SwiftLookup/SwiftAddress/Currency Location
int accuityLocationSwiftLookup(char* SwiftAddress, char* Currency, int *NoOf, tAccuityLocationSwiftLookup*& Recs)
{
message: #
input
    SwiftAddress;
    Currency;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationSwiftLookup(connect)
        log_debug(f'LocationSwiftLookup SwiftAddress:{SwiftAddress} Currency:{Currency}')
        rec.SwiftAddress = SwiftAddress
        rec.Currency = Currency
        Recs = rec.loadSwiftLookup()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationSwiftCountryLookup "accuitylocation.sh"
pragma accuityLocationSwiftCountryLookup:get accuityLocation-SwiftCountryLookup/SwiftAddress/Country Location
int accuityLocationSwiftCountryLookup(char* SwiftAddress, char* Country, int *NoOf, tAccuityLocationSwiftCountryLookup*& Recs)
{
message: #
input
    SwiftAddress;
    Country;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationSwiftCountryLookup(connect)
        log_debug(f'LocationSwiftCountryLookup SwiftAddress:{SwiftAddress} Country:{Country}')
        rec.SwiftAddress = SwiftAddress
        rec.Country = Country
        Recs = rec.loadSwiftCountryLookup()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationFinIdLookup "accuitylocation.sh"
pragma accuityLocationFinIdLookup:get accuityLocation-FinIdLookup/SwiftAddress Location
int accuityLocationFinIdLookup(char* SwiftAddress, int *NoOf, tAccuityLocationFinIdLookup*& Recs)
{
message: #
input
    SwiftAddress;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationFinIdLookup(connect)
        log_debug(f'LocationFinIdLookup SwiftAddress:{SwiftAddress}')
        rec.SwiftAddress = SwiftAddress
        Recs = rec.loadFinIdLookup()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationAccuityBank "accuitylocation.sh"
pragma accuityLocationAccuityBank:get accuityLocation-AccuityBank Location
int accuityLocationAccuityBank(int *NoOf, tAccuityLocationAccuityBank*& Recs)
{
message: #
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationAccuityBank(connect)
        Recs = rec.loadAccuityBank()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationAccuityCorrespondent "accuitylocation.sh"
pragma accuityLocationAccuityCorrespondent:get accuityLocation-AccuityCorrespondent Location
int accuityLocationAccuityCorrespondent(int *NoOf, tAccuityLocationAccuityCorrespondent*& Recs)
{
message: #
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationAccuityCorrespondent(connect)
        Recs = rec.loadAccuityCorrespondent()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqBankByName "accuitylocation.sh"
pragma accuityLocationReqBankByName:get accuityLocation-ReqBankByName/Country/BankName Location
int accuityLocationReqBankByName(char* Country, char* BankName, int *NoOf, tAccuityLocationReqBankByName*& Recs)
{
message: #
input
    Country;
    BankName;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqBankByName(connect)
        log_debug (f'LocationReqBankByName Country:{Country} BankName:{BankName}')
        rec.Country = Country
        rec.BankName = BankName
        Recs = rec.loadReqBankByName()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqBankByNameANDTown "accuitylocation.sh"
pragma accuityLocationReqBankByNameANDTown:get accuityLocation-ReqBankByNameANDTown/Country/BankName/Town Location
int accuityLocationReqBankByNameANDTown(char* Country, char* BankName, char* Town, int *NoOf, tAccuityLocationReqBankByNameANDTown*& Recs)
{
message: #
input
    Country;
    BankName;
    Town;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqBankByNameANDTown(connect)
        log_debug (f'LocationReqBankByNameANDTown Country:{Country} BankName:{BankName} Town:{Town}')
        rec.Country = Country
        rec.BankName = BankName
        rec.Town = Town
        Recs = rec.loadReqBankByNameANDTown()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqBankByNameOnly "accuitylocation.sh"
pragma accuityLocationReqBankByNameOnly:get accuityLocation-ReqBankByNameOnly/BankName Location
int accuityLocationReqBankByNameOnly(char* BankName, int *NoOf, tAccuityLocationReqBankByNameOnly*& Recs)
{
message: #
input
    BankName;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqBankByNameOnly(connect)
        log_debug (f'LocationReqBankByNameOnly BankName:{BankName}')
        rec.BankName = BankName
        Recs = rec.loadReqBankByNameOnly()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqBankByNameOnlyANDTown "accuitylocation.sh"
pragma accuityLocationReqBankByNameOnlyANDTown:get accuityLocation-ReqBankByNameOnlyANDTown/BankName/Town Location
int accuityLocationReqBankByNameOnlyANDTown(char* BankName, char* Town, int *NoOf, tAccuityLocationReqBankByNameOnlyANDTown*& Recs)
{
message: #
input
    BankName;
    Town;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqBankByNameOnlyANDTown(connect)
        log_debug (f'LocationReqBankByNameOnlyANDTown BankName:{BankName} Town:{Town}')
        rec.BankName = BankName
        rec.Town = Town
        Recs = rec.loadReqBankByNameOnlyANDTown()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqBankBySWIFT "accuitylocation.sh"
pragma accuityLocationReqBankBySWIFT:get accuityLocation-ReqBankBySWIFT/SwiftAddress Location
int accuityLocationReqBankBySWIFT(char* SwiftAddress, int *NoOf, tAccuityLocationReqBankBySWIFT*& Recs)
{
message: #
input
    SwiftAddress;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqBankBySWIFT(connect)
        log_debug (f'LocationReqBankBySWIFT SwiftAddress:{SwiftAddress}')
        rec.SwiftAddress = SwiftAddress
        Recs = rec.loadReqBankBySWIFT()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqBankBySWIFTANDTown "accuitylocation.sh"
pragma accuityLocationReqBankBySWIFTANDTown:get accuityLocation-ReqBankBySWIFTANDTown/SwiftAddress/Town Location
int accuityLocationReqBankBySWIFTANDTown(char* SwiftAddress, char* Town, int *NoOf, tAccuityLocationReqBankBySWIFTANDTown*& Recs)
{
message: #
input
    SwiftAddress;
    Town;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqBankBySWIFTANDTown(connect)
        log_debug (f'LocationReqBankBySWIFTANDTown SwiftAddress:{SwiftAddress} Town:{Town}')
        rec.SwiftAddress = SwiftAddress
        rec.Town = Town
        Recs = rec.loadReqBankBySWIFTANDTown()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqDraftBanks "accuitylocation.sh"
pragma accuityLocationReqDraftBanks:get accuityLocation-ReqDraftBanks/CurrId/Country Location
int accuityLocationReqDraftBanks(char* CurrId, char* Country, int *NoOf, tAccuityLocationReqDraftBanks*& Recs)
{
message: #
input
    CurrId;
    Country;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqDraftBanks(connect)
        log_debug (f'LocationReqBankDraftBanks CurrId:{CurrId} Country:{Country}')
        rec.CurrId = CurrId
        rec.Country = Country
        Recs = rec.loadReqDraftBanks()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqCountries "accuitylocation.sh"
pragma accuityLocationReqCountries:get accuityLocation-ReqCountries Location
int accuityLocationReqCountries(int *NoOf, tAccuityLocationReqCountries*& Recs)
{
message: #
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqCountries(connect)
        Recs = rec.loadReqCountries()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityLocationReqPresentationCountries "accuitylocation.sh"
pragma accuityLocationReqPresentationCountries:get accuityLocation-ReqPresentationCountries/CurrId Location
int accuityLocationReqPresentationCountries(char* CurrId, int *NoOf, tAccuityLocationReqPresentationCountries*& Recs)
{
message: #
input
    CurrId;
output
    NoOf;
    Recs size(NoOf);    
code
PYTHON:
    try:
        rec = DBAccuityLocationReqPresentationCountries(connect)
        log_debug (f'LocationReqPresentationCountries CurrId:{CurrId}')
        rec.CurrId = CurrId
        Recs = rec.loadReqPresentationCountries()
        NoOf = len(Recs)
        rc = 1 if NoOf > 0 else 0
        return rc, NoOf, Recs
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

//------------------------------------------------------------------------------

struct tAccuityOfficer "accuityofficer.sh"

pragma accuityOfficerInsert:post accuityOfficer Officer
void accuityOfficerInsert(tAccuityOfficer *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        write_audit('AccuityOfficer', ADD, newRec._fields(), None, newRec._data())
        newRec.execInsert()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

pragma accuityOfficerUpdate:put accuityOfficer Officer
void accuityOfficerUpdate(tAccuityOfficer *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        oldRec = DBAccuityOfficer(connect)
        oldRec.Id = newRec.Id
        oldRec.execSelectOne()
        write_audit('AccuityOfficer', CHANGE, newRec._fields(), oldRec._data(), newRec._data())
        newRec.execUpdate()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

struct tAccuityOfficerUpdateAllStatus "accuityofficer.sh"
pragma accuityOfficerUpdateAllStatus:patch accuityOfficer Officer
void accuityOfficerUpdateAllStatus(int status, char* usId)
{
message: #
input
  status;
  usId;
code
PYTHON:
    try:
        rec = DBAccuityOfficerUpdateAllStatus(connect)
        rec.runUpdateAllStatus(status, usId)
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

pragma accuityOfficerSelectOne:get accuityOfficer/Id Officer
void accuityOfficerSelectOne(int Id, tAccuityOfficer *rec)
{
message: #
input
    Id;
output
    rec;
code
PYTHON:
    try:
        rec = DBAccuityOfficer(connect)
        rec.Id = Id
        rec.execSelectOne()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
    return rec
endcode
}

struct tAccuityOfficerDeleteOne "accuityofficer.sh"

pragma accuityOfficerDeleteOne:delete accuityOfficer/Id Officer
void accuityOfficerDeleteOne(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityOfficerDeleteOne(connect)
        rec.Id = Id
        rec.execDeleteOne()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityOfficerExists "accuityofficer.sh"

pragma accuityOfficerExists:get accuityOfficer-Exists/Id Officer
int accuityOfficerExists(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityOfficerExists(connect)
        if rec.readExists(Id) == 1:
            return rec.Count
        else:
            return 0
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

//------------------------------------------------------------------------------

struct tAccuityRoutingAtt "accuityroutingatt.sh"

pragma accuityRoutingAttInsert:post accuityRoutingAtt RoutingAtt
void accuityRoutingAttInsert(tAccuityRoutingAtt *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        write_audit('AccuityRoutingAtt', ADD, newRec._fields(), None, newRec._data())
        newRec.execInsert()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

pragma accuityRoutingAttUpdate:put accuityRoutingAtt RoutingAtt
void accuityRoutingAttUpdate(tAccuityRoutingAtt *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        oldRec = DBAccuityRoutingAtt(connect)
        oldRec.Id = newRec.Id
        oldRec.execSelectOne()
        write_audit('AccuityRoutingAtt', CHANGE, newRec._fields(), oldRec._data(), newRec._data())
        newRec.execUpdate()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

struct tAccuityRoutingAttUpdateAllStatus "accuityroutingatt.sh"
pragma accuityRoutingAttUpdateAllStatus:patch accuityRoutingAtt RoutingAtt
void accuityRoutingAttUpdateAllStatus(int status, char* usId)
{
message: #
input
  status;
  usId;
code
PYTHON:
    try:
        rec = DBAccuityRoutingAttUpdateAllStatus(connect)
        rec.runUpdateAllStatus(status, usId)
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

pragma accuityRoutingAttSelectOne:get accuityRoutingAtt/Id RoutingAtt
void accuityRoutingAttSelectOne(int Id, tAccuityRoutingAtt *rec)
{
message: #
input
    Id;
output
    rec;
code
PYTHON:
    try:
        rec = DBAccuityRoutingAtt(connect)
        rec.Id = Id
        rec.execSelectOne()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
    return rec
endcode
}

struct tAccuityRoutingAttDeleteOne "accuityroutingatt.sh"
pragma accuityRoutingAttDeleteOne:delete accuityRoutingAtt/Id RoutingAtt
void accuityRoutingAttDeleteOne(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityRoutingAttDeleteOne(connect)
        rec.Id = Id
        rec.execDeleteOne()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityRoutingAttExists "accuityroutingatt.sh"
pragma accuityRoutingAttExists:get accuityRoutingAtt-Exists/Id RoutingAtt
int accuityRoutingAttExists(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityRoutingAttExists(connect)
        if rec.readExists(Id) == 1:
            return rec.Count
        else:
            return 0
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

//------------------------------------------------------------------------------

struct tAccuityRoutingCode          "accuityroutingcode.sh"

pragma accuityRoutingCodeInsert:post accuityRoutingCode RoutingCode
void accuityRoutingCodeInsert(tAccuityRoutingCode *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        write_audit('AccuityRoutingCode', ADD, newRec._fields(), '[]', newRec._data())
        newRec.execInsert()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

pragma accuityRoutingCodeUpdate:put accuityRoutingCode RoutingCode
void accuityRoutingCodeUpdate(tAccuityRoutingCode *newRec)
{
message: #
input
    newRec;
output
    newRec;
code
PYTHON:
    try:
        oldRec = DBAccuityRoutingCode(connect)
        oldRec.Id = newRec.Id
        oldRec.execSelectOne()
        write_audit('AccuityRoutingCode', CHANGE, newRec._fields(), oldRec._data(), newRec._data())
        newRec.execUpdate()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
        connect.rollback()
    return newRec
endcode
}

struct tAccuityRoutingCodeUpdateAllStatus "accuityroutingcode.sh"
pragma accuityRoutingCodeUpdateAllStatus:patch accuityRoutingCode RoutingCode
void accuityRoutingCodeUpdateAllStatus(int status, char* usId)
{
message: #
input
  status;
  usId;
code
PYTHON:
    try:
        rec = DBAccuityRoutingCodeUpdateAllStatus(connect)
        rec.runUpdateAllStatus(status, usId)
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

pragma accuityRoutingCodeSelectOne:get accuityRoutingCode/Id RoutingCode
void accuityRoutingCodeSelectOne(int Id, tAccuityRoutingCode *rec)
{
message: #
input
    Id;
output
    rec;
code
PYTHON:
    try:
        rec = DBAccuityRoutingCode(connect)
        rec.Id = Id
        rec.execSelectOne()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
    return rec
endcode
}

struct tAccuityRoutingCodeDeleteOne "accuityroutingcode.sh"
pragma accuityRoutingCodeDeleteOne:delete accuityRoutingCode/Id RoutingCode
void accuityRoutingCodeDeleteOne(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityRoutingCodeDeleteOne(connect)
        rec.Id = Id
        rec.execDeleteOne()
        connect.commit()
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}

struct tAccuityRoutingCodeExists "accuityroutingcode.sh"
pragma accuityRoutingCodeExists:get accuityRoutingCode-Exists/Id RoutingCode
int accuityRoutingCodeExists(int Id)
{
message: #
input
    Id;
code
PYTHON:
    try:
        rec = DBAccuityRoutingCodeExists(connect)
        if rec.readExists(Id) == 1:
            return rec.Count
        else:
            return 0
    except DBError as db:
        log_error ('DBError: value:{0} rc:{1} ociErr:{2}'.format(db.value, db.rc, db.ociErr))
endcode
}
```

### idl2/accuity/swagger_build CMakeLists.txt

``` cmake
project (putty3.idl2.accuity.swagger_build)

if (BUILD_NSWAG)
  pathed (outDir ${ACCUITY_BINARY_DIR}/nswag/cs)
  file (TO_NATIVE_PATH ${ACCUITY_BINARY_DIR}/http/accuity_swagger.yaml inFile)
  file (TO_NATIVE_PATH ${outDir}/Accuity.cs outFile)
  source_group (Built FILES ${outDir}/Accuity.cs)
  set (command "nswag openapi2csclient /input:${inFile} /output:${outFile} /classname:AccuityClient /namespace:AccuityNSwag /generatesyncmethods:true /UseTitleAsTypeName:true")
  message (STATUS ${command})
  add_custom_target (accuity_nswag_build ALL
    nswag openapi2csclient /input:${inFile} /output:${outFile} /classname:AccuityClient /namespace:AccuityNSwag /generatesyncmethods:true /UseTitleAsTypeName:true
    DEPENDS ${ACCUITY_BINARY_DIR}/http/accuity_swagger.yaml
    SOURCES ${ACCUITY_BINARY_DIR}/http/accuity_swagger.yaml
  )
  set_property(TARGET accuity_nswag_build PROPERTY FOLDER "putty3/idl2/accuity/swagger_nswag_build")
endif ()
```

### idl2/accuity/httpserver CMakeLists.txt

``` cmake
project (putty3.idl2.accuity.httpserver)

add_definitions(-DACCUITY_STANDALONE)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PYTHON_DIR}
  ${PYTHON_INCLUDE_DIR}
  ${ACCUITY_SOURCE_DIR}
  ${ACCUITY_BINARY_DIR}/http
  ${ACCUITY_BINARY_DIR}/server/idl
  ${ACCUITY_BINARY_DIR}/server/http
  ${ACCUITY_BINARY_DIR}/sh
  ${PUTTY3_SOURCE_DIR}/idl2/src
  ${LIBS_SOURCE_DIR}
  ${LIBS_SOURCE_DIR}/http/src
  ${LIBS_SOURCE_DIR}/jsoncpp
  ${LIBS_SOURCE_DIR}/jsoncpp/src
  ${PUTTY3_INCLUDE_DIR}
  )

include (${CMAKE_CURRENT_SOURCE_DIR}/inifile.cmake)

set (source
  ${ACCUITY_BINARY_DIR}/http/accuityserver_http.h
  ${ACCUITY_BINARY_DIR}/http/accuityserver_http.cpp
  ${ACCUITY_BINARY_DIR}/server/idl/accuity.h
  ${ACCUITY_BINARY_DIR}/server/idl/accuityimpl.cpp
  ${ACCUITY_BINARY_DIR}/http/accuity_main.cpp
  ${outfile}
  ${pyfile}
  )

set_source_files_properties (${source} PROPERTIES GENERATED TRUE)

if (WIN32)
  set (winlibs ws2_32 winmm)
else ()  
  set (winlibs)
endif ()

set (added_source
  ${PUTTY3_SOURCE_DIR}/idl2/src/putty3shell.cpp
  ${PUTTY3_SOURCE_DIR}/idl2/src/putty3shell.h
  )

add_executable(accuityhttp ${source} ${added_source})
target_link_libraries (accuityhttp ${bbdlibs} ${winlibs} ${bbdextra} jsoncpp http ${PYTHONLIBS})
add_dependencies (accuityhttp accuity_idl2_build pyportal)
if (WIN32)
  add_dependencies (accuityhttp Accuityhttp_cs_build)
  add_custom_command (TARGET accuityhttp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${ACCUITY_BINARY_DIR}/bin/accuity.bin ${CMAKE_BINARY_DIR}/bin/accuity.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${ACCUITY_BINARY_DIR}/bin/accuityhttp.ini ${CMAKE_BINARY_DIR}/bin/accuityhttp.ini
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/Debug/accuityhttp.exe ${ACCUITY_BINARY_DIR}/bin/accuityhttp.exe
    COMMAND ${CMAKE_COMMAND} -E copy ${ACCUITY_BINARY_DIR}/bin/accuityhttp.cmd ${CMAKE_BINARY_DIR}/bin/accuityhttp.cmd
    )
else ()
  add_custom_command (TARGET accuityhttp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${ACCUITY_BINARY_DIR}/bin/accuity.bin ${CMAKE_BINARY_DIR}/bin/accuity.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/accuityhttp.ini ${ACCUITY_BINARY_DIR}/bin/accuityhttp.ini
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/accuityhttp ${ACCUITY_BINARY_DIR}/bin/accuityhttp
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/accuityhttp.sh ${ACCUITY_BINARY_DIR}/bin/accuityhttp.sh
    )
endif ()

set_target_properties (accuityhttp PROPERTIES PROJECT_LABEL putty3_idl2_accuity_httpserver)
set_property(TARGET accuityhttp PROPERTY FOLDER "putty3/idl2/accuity/httpserver")
```

#### inifile.cmake

``` cmake
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
```
