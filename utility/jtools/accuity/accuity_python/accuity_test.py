from INTRINSICS import *
from HTTPSERVER_UTIL import *
from ACCUITY_IMPL import *

import random, sys
random.seed()

pragmas = '''\
ping:ping/given/add get
pong:pong/{given}/add/{add} get
pang:pang/{given}/add/{add}/and/{gotten} get
banana:banana options
accuityContactInsert:accuityContact post
accuityContactUpdate:accuityContact put
accuityContactUpdateAllStatus:accuityContact patch
accuityContactSelectOne:accuityContact/Id get
accuityContactDeleteOne:accuityContact/Id delete
accuityContactExists:accuityContact-Exists/Id get
accuityCorrespondentInsert:accuityCorrespondent post 
accuityCorrespondentUpdate:accuityCorrespondent put 
accuityCorrespondentUpdateAllStatus:accuityCorrespondent patch
accuityCorrespondentSelectOne:accuityCorrespondent/Id get
accuityCorrespondentDeleteOne:accuityCorrespondent/Id delete
accuityCorrespondentExists:accuityCorrespondent-Exists/Id get
accuityCorrespondentGet:accuityCorrespondent-Get/FinId/CorrCurrency get
accuityCorrespondentGetSwiftList:accuityCorrespondent-SwiftList/FinId/CorrCurrency get
accuityCorrespondentSubInsert:accuityCorrespondentSub post 
accuityCorrespondentSubUpdate:accuityCorrespondentSub put 
accuityCorrespondentSubUpdateAllStatus:accuityCorrespondentSub patch
accuityCorrespondentSubSelectOne:accuityCorrespondentSub/Id get
accuityCorrespondentSubDeleteOne:accuityCorrespondentSub/Id delete
accuityCorrespondentSubExists:accuityCorrespondentSub-Exists/Id get
accuityLocationInsert:accuityLocation post
accuityLocationUpdate:accuityLocation put
accuityLocationUpdateAllStatus:accuityLocation patch
accuityLocationSelectOne:accuityLocation/Id get 
accuityLocationDeleteOne:accuityLocation/Id delete
accuityLocationExists:accuityLocation-Exists/Id get
accuityLocationGet:accuityLocation-Get/FinId get
accuityLocationGetBankCode:accuityLocation-BankCode/FinId get
accuityLocationFinIdBankCodeCheck:accuityLocation-Check/FinId/NationalBankCode get
accuityLocationSwiftLookup:accuityLocation-SwiftLookup/SwiftAddress/Currency get
accuityLocationSwiftCountryLookup:accuityLocation-SwiftCountryLookup/SwiftAddress/Country get
accuityLocationFinIdLookup:accuityLocation-FinIdLookup/FinId/BranchId get
accuityLocationAccuityBank:accuityLocation-AccuityBank get
accuityLocationAccuityCorrespondent:accuityLocation-AccuityCorrespondent get
accuityLocationReqBankByName:accuityLocation-ReqBankByName/Country/BankName get
accuityLocationReqBankByNameANDTown:accuityLocation-ReqBankByNameANDTown/Country/BankName/Town get
accuityLocationReqBankByNameOnly:accuityLocation-ReqBankByNameOnly/BankName get
accuityLocationReqBankByNameOnlyANDTown:accuityLocation-ReqBankByNameOnlyANDTown/BankName/Town get
accuityLocationReqBankBySWIFT:accuityLocation-ReqBankBySWIFT/SwiftAddress get
accuityLocationReqBankBySWIFTANDTown:accuityLocation-ReqBankBySWIFTANDTown/SwiftAddress/Town get
accuityLocationReqDraftBanks:accuityLocation-ReqDraftBanks/CurrId/Country get
accuityLocationReqCountries:accuityLocation-ReqCountries get
accuityLocationReqPresentationCountries:accuityLocation-ReqPresentationCountries/CurrId/Country get
accuityOfficerInsert:accuityOfficer post
accuityOfficerUpdate:accuityOfficer put
accuityOfficerUpdateAllStatus:accuityOfficer patch
accuityOfficerSelectOne:accuityOfficer/Id get
accuityOfficerDeleteOne:accuityOfficer/Id delete
accuityOfficerExists:accuityOfficer-Exists/Id get
accuityRoutingAttInsert:accuityRoutingAtt post
accuityRoutingAttUpdate:accuityRoutingAtt put
accuityRoutingAttUpdateAllStatus:accuityRoutingAtt patch
accuityRoutingAttSelectOne:accuityRoutingAtt/Id get
accuityRoutingAttDeleteOne:accuityRoutingAtt/Id delete
accuityRoutingAttExists:accuityRoutingAtt-Exists/Id get
accuityRoutingCodeInsert:accuityRoutingCode post 
accuityRoutingCodeUpdate:accuityRoutingCode put
accuityRoutingCodeUpdateAllStatus:accuityRoutingCode patch
accuityRoutingCodeSelectOne:accuityRoutingCode/Id get
accuityRoutingCodeDeleteOne:accuityRoutingCode/Id delete
accuityRoutingCodeExists:accuityRoutingCode-Exists/Id get
'''.splitlines()

debug_list = []

def test_ping(p):
    given, add = p
    return (ping(given, add))

def test_pang(p):
    a,b,c = p
    return (pang(a, b, c))

## --------------------------------------------------------------

def test_accuityContactExists(Id: int):
    return accuityContactExists(Id)

def test_accuityContactSelectOne(Id: int):
    return accuityContactSelectOne(Id)

def test_accuityContactDeleteOne(Id: int):    
    return accuityContactDeleteOne(Id)

def test_accuityContactInsert(newRec: DBAccuityContact):
    newRec.Id += 1
    return accuityContactInsert(newRec)

def test_accuityContactUpdate(newRec: DBAccuityContact):
    return accuityContactUpdate(newRec)

def test_accuityContactUpdateAllStatus(args):
    Id, usId = args
    accuityContactUpdateAllStatus(Id, usId)
    return

## --------------------------------------------------------------

def test_accuityCorrespondentExists(Id: int):
    return accuityCorrespondentExists(Id)
    
def test_accuityCorrespondentSelectOne(Id: int):
    return accuityCorrespondentSelectOne(Id)

def test_accuityCorrespondentDeleteOne(Id: int):    
    return accuityCorrespondentDeleteOne(Id)

def test_accuityCorrespondentInsert(newRec: DBAccuityCorrespondent):
    newRec.Id += 1
    return accuityCorrespondentInsert(newRec)

def test_accuityCorrespondentUpdate(newRec: DBAccuityCorrespondent):
    return accuityCorrespondentUpdate(newRec)

def test_accuityCorrespondentUpdateAllStatus(args):
    Id, usId = args
    accuityCorrespondentUpdateAllStatus(Id, usId)
    return

def test_accuityCorrespondentGet(args):
    #accuityCorrespondent-Get/FinId/CorrCurrency get
    FinId, CorrCurrency = args
    return accuityCorrespondentGet(FinId, CorrCurrency)

def test_accuityCorrespondentGetSwiftList(args):
    #accuityCorrespondent-SwiftList/FinId/CorrCurrency get
    FinId, CorrCurrency = args
    return accuityCorrespondentGetSwiftList(FinId, CorrCurrency)

## --------------------------------------------------------------

def test_accuityCorrespondentSubExists(Id: int):
    return accuityCorrespondentSubExists(Id)
    
def test_accuityCorrespondentSubSelectOne(Id: int):
    return accuityCorrespondentSubSelectOne(Id)

def test_accuityCorrespondentSubDeleteOne(Id: int):    
    return accuityCorrespondentSubDeleteOne(Id)

def test_accuityCorrespondentSubInsert(newRec: DBAccuityCorrespondentSub):
    newRec.Id += 1
    return accuityCorrespondentSubInsert(newRec)

def test_accuityCorrespondentSubUpdate(newRec: DBAccuityCorrespondentSub):
    return accuityCorrespondentSubUpdate(newRec)

def test_accuityCorrespondentSubUpdateAllStatus(args):
    Id, usId = args
    accuityCorrespondentSubUpdateAllStatus(Id, usId)
    return

## --------------------------------------------------------------

def test_accuityLocationExists(Id: int):
    return accuityLocationExists(Id)
    
def test_accuityLocationSelectOne(Id: int):
    return accuityLocationSelectOne(Id)

def test_accuityLocationDeleteOne(Id: int):    
    return accuityLocationDeleteOne(Id)

def test_accuityLocationInsert(newRec: DBAccuityLocation):
    newRec.LocationId += 1
    return accuityLocationInsert(newRec)

def test_accuityLocationUpdate(newRec: DBAccuityLocation):
    return accuityLocationUpdate(newRec)

def test_accuityLocationUpdateAllStatus(args):
    Id, usId = args
    accuityLocationUpdateAllStatus(Id, usId)
    return

def test_accuityLocationGet(args):
    # accuityLocation-Get/FinId get
    FinId = args
    response = accuityLocationGet(FinId)
    return response

def test_accuityLocationGetBankCode(args):
    # accuityLocation-BankCode/FinId get
    FinId = args
    response = accuityLocationGetBankCode(FinId)
    return response

def test_accuityLocationFinIdBankCodeCheck(args):
    # accuityLocation-Check/FinId/NationalBankCode get
    Finid, NationalBankCode = args
    response = accuityLocationFinIdBankCodeCheck(Finid, NationalBankCode)
    return response

def test_accuityLocationSwiftLookup(args):
    # accuityLocation-SwiftLookup/SwiftAddress/Currency get
    SwiftAddress, Currency = args
    response = accuityLocationSwiftLookup(SwiftAddress, Currency)
    return response

def test_accuityLocationSwiftCountryLookup(args):
    # accuityLocation-SwiftCountryLookup/SwiftAddress/Country get
    SwiftAddress, Country = args
    response = accuityLocationSwiftCountryLookup(SwiftAddress, Country)
    return response

def test_accuityLocationFinIdLookup(args):
    # accuityLocation-FinIdLookup/SwiftAddress get
    SwiftAddress = args
    response = accuityLocationFinIdLookup(SwiftAddress)
    return response

def test_accuityLocationAccuityBank(args):
    # accuityLocation-AccuityBank get
    response = accuityLocationAccuityBank()
    return response

def test_accuityLocationAccuityCorrespondent(args):
    # accuityLocation-AccuityCorrespondent get
    response = accuityLocationAccuityCorrespondent()
    return response

def test_accuityLocationReqBankByName(args):
    # accuityLocation-ReqBankByName/Country/BankName get
    Country, BankName = args
    response = accuityLocationReqBankByName(Country, BankName)
    return response

def test_accuityLocationReqBankByNameANDTown(args):
    # accuityLocation-ReqBankByNameANDTown/Country/BankName/Town get
    Country, BankName, Town = args
    response = accuityLocationReqBankByNameANDTown(Country, BankName, Town)
    return response

def test_accuityLocationReqBankByNameOnly(args):
    # accuityLocation-ReqBankByNameOnly/BankName get
    BankName = args
    response = accuityLocationReqBankByNameOnly(BankName)
    return response

def test_accuityLocationReqBankByNameOnlyANDTown(args):
    # accuityLocation-ReqBankByNameOnlyANDTown/BankName/Town get
    BankName, Town = args
    response = accuityLocationReqBankByNameOnlyANDTown(BankName, Town)
    return response

def test_accuityLocationReqBankBySWIFT(args):
    # accuityLocation-ReqBankBySWIFT/SwiftAddress get
    SwiftAddress = args
    response = accuityLocationReqBankBySWIFT(SwiftAddress)
    return response

def test_accuityLocationReqBankBySWIFTANDTown(args):
    # accuityLocation-ReqBankBySWIFTANDTown/SwiftAddress/Town get
    SwiftAddress, Town = args
    response = accuityLocationReqBankBySWIFTANDTown(SwiftAddress, Town)
    return response

def test_accuityLocationReqDraftBanks(args):
    # accuityLocation-ReqDraftBanks/CurrId/Country get
    CurrId, Country = args
    response = accuityLocationReqDraftBanks(CurrId, Country)
    return response

def test_accuityLocationReqCountries(args):
    # accuityLocation-ReqCountries get
    response = accuityLocationReqCountries()
    return response

def test_accuityLocationReqPresentationCountries(args)    :
    # accuityLocation-ReqPresentationCountries/CurrId get
    CurrId = args
    response = accuityLocationReqPresentationCountries(CurrId)
    return response

## --------------------------------------------------------------

def test_accuityOfficerExists(Id: int):
    return accuityOfficerExists(Id)
    
def test_accuityOfficerSelectOne(Id: int):
    return accuityOfficerSelectOne(Id)

def test_accuityOfficerDeleteOne(Id: int):    
    return accuityOfficerDeleteOne(Id)

def test_accuityOfficerInsert(newRec: DBAccuityOfficer):
    newRec.Id += 1
    return accuityOfficerInsert(newRec)

def test_accuityOfficerUpdate(newRec: DBAccuityOfficer):
    return accuityOfficerUpdate(newRec)

def test_accuityOfficerUpdateAllStatus(args):
    Id, usId = args
    accuityOfficerUpdateAllStatus(Id, usId)
    return

## --------------------------------------------------------------

def test_accuityRoutingAttExists(Id: int):
    return accuityRoutingAttExists(Id)
    
def test_accuityRoutingAttSelectOne(Id: int):
    return accuityRoutingAttSelectOne(Id)

def test_accuityRoutingAttDeleteOne(Id: int):    
    return accuityRoutingAttDeleteOne(Id)

def test_accuityRoutingAttInsert(newRec: DBAccuityRoutingAtt):
    newRec.Id += 1
    return accuityRoutingAttInsert(newRec)

def test_accuityRoutingAttUpdate(newRec: DBAccuityRoutingAtt):
    return accuityRoutingAttUpdate(newRec)

def test_accuityRoutingAttUpdateAllStatus(args):
    Id, usId = args
    accuityRoutingAttAllStatus(Id, usId)
    return

## --------------------------------------------------------------

def test_accuityRoutingCodeExists(Id: int):
    return accuityRoutingCodeExists(Id)
    
def test_accuityRoutingCodeSelectOne(Id: int):
    return accuityRoutingCodeSelectOne(Id)

def test_accuityRoutingCodeDeleteOne(Id: int):    
    return accuityRoutingCodeDeleteOne(Id)

def test_accuityRoutingCodeInsert(newRec: DBAccuityRoutingCode):
    newRec.Id += 1
    return accuityRoutingCodeInsert(newRec)

def test_accuityRoutingCodeUpdate(newRec: DBAccuityRoutingCode):
    return accuityRoutingCodeUpdate(newRec)

def test_accuityRoutingCodeUpdateAllStatus(args):
    Id, usId = args
    accuityRoutingCodeUpdateAllStatus(Id)
    return

## --------------------------------------------------------------

def run(proc, arg=()):
    try:
        log_debug ('proc:%s %s' % (proc.__name__, arg))
        return proc(arg)
    except DBError as dberr:
        log_error ('%s %s' % (proc.__name__, dberr.ociErr))
    except Exception as ex:
        log_error ('%s %s' % (proc.__name__, ex.args))
    return arg

def display_recs(descr, result):
    if result != None and len(result) == 3:
        rc, noOf, recs = result
        if rc == 1 and noOf > 0:
             display_dbrec(f'{descr}:{noOf}', recs[0])

def display_rec(descr, result):
    if result != None and len(result) == 2:
        rc, rec = result
        if rc == 1:
            display_dbrec(f'{descr}', rec)

def do_ping(): 
    ping_no = run(test_ping, (12, 1))
    log_info (f'{ping_no}')
    pang_no = run(test_pang, (13, 2, 'arg'))
    log_info (f'{pang_no}')

def do_contact():
    result = run(test_accuityContactExists, (1675608))
    accuityContactRec = run(test_accuityContactSelectOne, (1675608))
    accuityContactRec = run(test_accuityContactInsert, (accuityContactRec))
    accuityContactRec = run(test_accuityContactUpdate, (accuityContactRec))
    display_dbrec('Contact', accuityContactRec)
    result = run(test_accuityContactDeleteOne, (accuityContactRec.Id))
    #run(test_accuityContactUpdateAllStatus, (0, 'accuity_test'))

def do_correspondent():
    result = run(test_accuityCorrespondentExists, (550000004))
    accuityCorrespondentRec = run(test_accuityCorrespondentSelectOne, (550000004))
    accuityCorrespondentRec = run(test_accuityCorrespondentInsert, (accuityCorrespondentRec))
    accuityCorrespondentRec = run(test_accuityCorrespondentUpdate, (accuityCorrespondentRec))
    display_dbrec('Correspondent', accuityCorrespondentRec)
    result = run(test_accuityCorrespondentDeleteOne, (accuityCorrespondentRec.Id))
    #run(test_accuityCorrespondentUpdateAllStatus, (0, 'accuity_test'))    
    FinId = 10385510
    CorrCurrency = 'ZAR'
    result = run(test_accuityCorrespondentGet, (FinId, CorrCurrency))
    display_rec('CorrespondentGet', result)
    result = run(test_accuityCorrespondentGetSwiftList, (FinId, CorrCurrency))
    display_recs('CorrespondentGetSwiftList', result)

def do_correspondent_sub():
    result = run(test_accuityCorrespondentSubExists, (5911220))
    accuityCorrespondentSubRec = run(test_accuityCorrespondentSubSelectOne, (5911220))
    accuityCorrespondentSubRec = run(test_accuityCorrespondentSubInsert, (accuityCorrespondentSubRec))
    accuityCorrespondentSubRec = run(test_accuityCorrespondentSubUpdate, (accuityCorrespondentSubRec))
    display_dbrec('CorrespondentSub', accuityCorrespondentSubRec)
    result = (test_accuityCorrespondentSubDeleteOne, (accuityCorrespondentSubRec.Id))
    #run(test_accuityCorrespondentSubUpdateAllStatus, (0, 'accuity_test'))    

def do_location(LocationId=10002640):
    #  result = run(test_accuityLocationExists, (LocationId))
    #  accuityLocationRec = run(test_accuityLocationSelectOne, (LocationId))
    #  accuityLocationRec = run(test_accuityLocationInsert, (accuityLocationRec))
    #  accuityLocationRec = run(test_accuityLocationUpdate, (accuityLocationRec))
    #  display_dbrec('Location', accuityLocationRec)
    #  result = run(test_accuityLocationDeleteOne, (accuityLocationRec.LocationId))
    #run(test_accuityLocationUpdateAllStatus, (0, 'accuity_test'))    
    #accuityLocation-Get/FinId get
    result = run(test_accuityLocationGet, (LocationId))
    display_rec('LocationGet', result)
    #accuityLocation-BankCode/FinId get
    result = run(test_accuityLocationGetBankCode, (LocationId))
    display_rec('LocationGetBankCode', result)
    #accuityLocation-Check/FinId/NationalBankCode
    finId = LocationId
    nationalBankCode = '062201999'
    result = run(test_accuityLocationFinIdBankCodeCheck, (LocationId, nationalBankCode))
    display_rec('LocationFinIdBankCodeCheck', result)
    #accuityLocation-SwiftLookup/SwiftAddress/Currency get
    swiftAddress = 'BKCHRUMM'
    currency = 'EUR'
    result = run(test_accuityLocationSwiftLookup, (swiftAddress, currency))
    display_recs('LocationSwiftLookup', result)
    #accuityLocation-SwiftCountryLookup/SwiftAddress/Country get
    swiftAddress = 'BKCHDEFF'
    country = 'AU'
    result = run(test_accuityLocationSwiftCountryLookup, (swiftAddress, country))
    display_recs('LocationSwiftCountryLookup', result)
    #accuityLocation-FinIdLookup/SwiftAddress get
    swiftAddress = 'BKCHDEFF'
    result = run(test_accuityLocationFinIdLookup, (swiftAddress))
    display_recs('LocationFinidLookup', result)
    ###accuityLocation-AccuityBank get
    ###response = run(test_accuityLocationAccuityBank, ())
    ###if response is not None:
    ###    rc, noOf, recs = response
    ###    if rc == 1 and noOf > 0:
    ###        display_dbrec(f'LocationAccuityBank:{noOf}', recs[0])
    ###accuityLocation-AccuityCorrespondent get
    ###result = run(test_accuityLocationAccuityCorrespondent, ())
    #accuityLocation-ReqBankByName/Country/BankName get
    Country = 'VENEZUELA'
    BankName = '100 00 BANCO'
    result = run(test_accuityLocationReqBankByName, (Country, BankName))
    display_recs('LocationReqBankByName', result)
    #accuityLocation-ReqBankByNameANDTown/Country/BankName/Town get
    Country = 'VENEZUELA'
    BankName = '100 00 BANCO'
    Town = 'CARACAS'
    result = run(test_accuityLocationReqBankByNameANDTown, (Country, BankName, Town))
    display_recs('LocationReqBankByNameANDTown', result)
    #accuityLocation-ReqBankByNameOnly/BankName get
    BankName = '100 00 BANCO'
    result = run(test_accuityLocationReqBankByNameOnly, (BankName))
    display_recs('LocationReqBankByNameOnly', result)
    #accuityLocation-ReqBankByNameOnlyANDTown/BankName/Town get
    BankName = '100 00 BANCO'
    Town = 'CARACAS'
    result = run(test_accuityLocationReqBankByNameOnlyANDTown, (BankName, Town))
    display_recs('LocationReqBankByNameOnlyANDTown', result)
    #accuityLocation-ReqBankBySWIFT/SwiftAddress get
    SwiftAddress = 'FNCOVECA'
    result = run(test_accuityLocationReqBankBySWIFT, (SwiftAddress))
    display_recs('LocationReqBankBySWIFT', result)
    #accuityLocation-ReqBankBySWIFTANDTown/SwiftAddress/Town get
    SwiftAddress = 'BDCPITTT'
    Town = 'TORINO'
    result = run(test_accuityLocationReqBankBySWIFTANDTown, (SwiftAddress, Town))
    display_recs('LocationReqBankBySWIFTANDTown', result)
    #accuityLocation-ReqDraftBanks/CurrId/Country get
    CurrId = 'EUR'
    Country = 'ITALY'
    result = run(test_accuityLocationReqDraftBanks, (CurrId, Country))
    display_recs('LocationReqDraftBanks', result)
    #accuityLocation-ReqCountries get
    result = run(test_accuityLocationReqCountries, ())
    display_recs('LocationReqCountries', result)
    #accuityLocation-ReqPresentationCountries/CurrId get
    CurrId = 'EUR'
    Country = 'VENEZUELA'
    result = run(test_accuityLocationReqPresentationCountries, (CurrId))
    display_recs('LocationReqPresentationCountries', result)

def do_officer():
    result = run(test_accuityOfficerExists, (1500967))
    accuityOfficerRec = run(test_accuityOfficerSelectOne, (1500967))
    accuityOfficerRec = run(test_accuityOfficerInsert, (accuityOfficerRec))
    accuityOfficerRec = run(test_accuityOfficerUpdate, (accuityOfficerRec))
    display_dbrec('Officer', accuityOfficerRec)
    result = run(test_accuityOfficerDeleteOne, (accuityOfficerRec.Id))
    #run(test_accuityOfficerUpdateAllStatus, (0, 'accuity_test'))    

def do_routing_att():
    result = run(test_accuityRoutingAttExists, (1583539))
    accuityRoutingAttRec = run(test_accuityRoutingAttSelectOne, (1583539))
    accuityRoutingAttRec = run(test_accuityRoutingAttInsert, (accuityRoutingAttRec))
    accuityRoutingAttRec = run(test_accuityRoutingAttUpdate, (accuityRoutingAttRec))
    display_dbrec('RoutingAtt', accuityRoutingAttRec)
    result = run(test_accuityRoutingAttDeleteOne, (accuityRoutingAttRec.Id))
    #result = run(test_accuityRoutingAttUpdateAllStatus, (0, 'accuity_test'))    

def do_routing_code():
    result = run(test_accuityRoutingCodeExists, (11740946))
    accuityRoutingCodeRec = run(test_accuityRoutingCodeSelectOne, (11740946))
    accuityRoutingCodeRec = run(test_accuityRoutingCodeInsert, (accuityRoutingCodeRec))
    if accuityRoutingCodeRec is not None:
        accuityRoutingCodeRec = run(test_accuityRoutingCodeUpdate, (accuityRoutingCodeRec))
        display_dbrec('RoutingCode', accuityRoutingCodeRec)
    if accuityRoutingCodeRec is not None:
        result = run(test_accuityRoutingCodeDeleteOne, (accuityRoutingCodeRec.Id))
    #result = run(test_accuityRoutingCodeUpdateAllStatus, (0, 'accuity_test'))    

def main():
    global out_dir
    config_file = sys.argv[1]
    out_dir = sys.argv[2]
    print (config_file)
    rc, config = do_logons(config_file)
    log_display('1')
    do_ping()
    do_contact()
    do_correspondent()
    do_correspondent_sub()
    do_location()
    do_officer()
    do_routing_att()
    do_routing_code()
    return

if __name__ == '__main__':
    main()

