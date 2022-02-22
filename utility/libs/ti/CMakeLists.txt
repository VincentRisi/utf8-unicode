project (ti)

set (source
  src/acb.h src/alertrpc.h src/appkeys.h src/aqueuepairs.h
  src/ascify.cpp src/ascify.h
  src/assign.cpp src/assign.h 
  src/binio.cpp src/binio.h
  src/buffer.cpp src/buffer.h
  src/bytebuffer.h
  src/cbbcast.cpp  src/cbbcast.h
  src/ccsocket.h src/cifprsok.h src/cliserv.h src/clisock.h
  src/codepage.h src/condprnt.h src/console.h src/cs.h
  src/cxcept.h src/datablck.h
  src/date.cpp src/date.h
  src/ddc.h src/ddcix.h src/ddcom.h src/ddcwin.h src/dds.h
  src/dict.cpp src/dict.h
  src/dlapi.h src/dmrpc.h src/dumyvers.h src/errbase.h
  src/errnomsg.h src/error.h src/errstr.h src/extract.h
  src/flap.h src/flapqin.h
  src/fndir.cpp src/fnext.cpp src/fnfind.cpp src/fnmerge.cpp src/fnname.cpp 
  src/fnsplit.cpp src/fortify.cpp src/fortify.h
  src/genclnt.h src/gencserr.h src/gennet.h src/genpackr.h src/genrpc.h
  src/getargs.cpp src/getargs.h
  src/getch.h src/gsrv_ipc.h src/gsrv_net.h src/gsvr_ipc.h
  src/ini.cpp src/ini.h
  src/inistr.cpp src/inistr.h
  src/init.cpp src/lc_enum.h
  src/lhcbexi.h src/lhcbnew.h src/lhcbtrin.h src/linghead.h src/list.h
  src/list.hpp src/log.h
  src/lstream.h src/mcs.h src/message.h src/metricdata.h src/mformats.h
  src/mqclient.h src/mqseries.h
  src/nederror.cpp src/nederror.h
  src/net.h src/newdebug.h src/nsw_fmt.h src/nswgetsi.h src/nswswift.h
  src/nxlcrpc.h src/obbcast.h src/ociapr_.h src/ocidfn_.h src/oracle.h src/osfuncs.h
  src/param.cpp src/param.h
  src/parent.h src/rateserverrpc.h src/report.h src/request.h src/rmrates.h
  src/rpcstream.cpp src/rpcstream.h
  src/rpcti.h src/rscli.h src/sarb.h src/secadm.h src/sfapi.h src/sfapp.h
  src/sfcommon.h src/sferr.cpp src/sferr.h src/sqlapi80.h
  src/sqlbin.cpp src/sqlbin.h
  src/sqlcons.h src/sqlhnd.h
  src/sqlodbc.h src/sqlodbc2.h
  src/sqltok.cpp src/sqltok.h
  src/store.h src/ti.h src/timer.h src/tllist.h src/tmqueue.h src/tools.h
  src/tp2p.h src/tpipe.h src/tpostsock.h src/tpqueue.h src/tpresock.h src/tqueue.h
  src/trace.cpp src/trace.h
  src/tranmsg.h src/tsem.h src/tsem2.h src/tshmseg.h src/tstreams.h
  src/tstring.cpp src/tstring.h
  src/tstringlist.h src/tunixser.h src/ufortify.h
  src/vsocket.cpp src/vsocket.h
  src/waste.h src/winsock.h src/wintools.h src/xassert.h src/xdir.h
  src/xl_ipc.h src/xlnet.h src/xmldef.h src/xmlfile.h src/xmlreader.h
  src/xmltchar.h src/xtalloc.cpp src/xtalloc.h
  src/xtime.cpp src/xtime.h src/yyerror.h
  src/zcompress.cpp src/zcompress.h
  src/zdcidsnd.h
)

add_library(ti
  ${source}
)

target_link_libraries (ti zlib)

set_target_properties(ti PROPERTIES PROJECT_LABEL libs-ti)
set_property(TARGET ti PROPERTY FOLDER ${LIBS_FOLDER}/ti)
