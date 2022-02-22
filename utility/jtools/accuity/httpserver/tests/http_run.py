accuityContact_delete = "accuityContact/Id/2"
accuityContact_get = "accuityContact/Id/2"
accuityContact_patch = '''\
{
  "status": 0,
  "usId": "tests"
}
'''
accuityContact_post = '''\
{
  "newRec": {
    "Id": 2,
    "LocationId": 10000010,
    "Department": "MAIN",
    "ContactType": "TEL",
    "ContactInfo": "1-479-271-1363",
    "Status": 0,
    "USId": "##er",
    "TmStamp": "20200701"
  }
}
'''
accuityContact_put = '''\
{
  "newRec": {
    "Id": 2,
    "LocationId": 10000010,
    "Department": "MAIN",
    "ContactType": "TEL",
    "ContactInfo": "1-479-127-1363 EXT 124",
    "Status": 0,
    "USId": "##er",
    "TmStamp": "20200701"
  }
}
'''
## -----------------------------------------------------
accuityCorrespondent_delete = "accuityCorrespondent/Id/8862"
accuityCorrespondentGet_get = "accuityCorrespondentGet/FinId/0/CorrCurrency/0"
accuityCorrespondent_get = "accuityCorrespondent/Id/8862"
accuityCorrespondentSwiftList_get = "accuityCorrespondentSwiftList/FinId/0/CorrCurrency/0"
accuityCorrespondent_patch = '''\
{
  "status": 0,
  "usId": "tests"
}
'''
accuityCorrespondent_post = '''\
{
  "newRec": {
    "Id": 8862,
    "LocationId": 96785103,
    "ClearingSystem": "COM",
    "CurrencyCode": "USD",
    "OwnerSwift": "ALPEAT22XXX",
    "OwnerSwiftWOPadding": "ALPEAT22",
    "OwnerSSIAccountNo": "",
    "ClearingAccuityId": "26542590",
    "ClearingSwift": "RZBAATWWXXX",
    "ClearingSwiftWOPadding": "RZBAATWW",
    "HolderAccuityId": "",
    "HolderSwift": "",
    "HolderSwiftWOPadding": "",
    "HolderSSIAccountNo": "",
    "PreferredSSIInd": "N",
    "FurtherAccuityId": "",
    "FurtherSwift": "",
    "FurtherSwiftWOPadding": "",
    "Further2AccuityId": "",
    "Further2Swift": "",
    "Further2SwiftWOPadding": "",
    "CorrespondentEffectiveDate": "",
    "CorrespondentDeactivationDate": "",
    "CorrespondentUpdateDate": "2019/10/01",
    "SSINotes": "",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029162316"
  }
}
'''
accuityCorrespondent_put = '''\
{
  "newRec": {
    "Id": 8862,
    "LocationId": 96785103,
    "ClearingSystem": "COM",
    "CurrencyCode": "USD",
    "OwnerSwift": "ALPEAT22XXX",
    "OwnerSwiftWOPadding": "ALPEAT22",
    "OwnerSSIAccountNo": "",
    "ClearingAccuityId": "26542590",
    "ClearingSwift": "RZBAATWWXXX",
    "ClearingSwiftWOPadding": "RZBAATWW",
    "HolderAccuityId": "",
    "HolderSwift": "",
    "HolderSwiftWOPadding": "",
    "HolderSSIAccountNo": "",
    "PreferredSSIInd": "N",
    "FurtherAccuityId": "",
    "FurtherSwift": "",
    "FurtherSwiftWOPadding": "",
    "Further2AccuityId": "",
    "Further2Swift": "",
    "Further2SwiftWOPadding": "",
    "CorrespondentEffectiveDate": "",
    "CorrespondentDeactivationDate": "",
    "CorrespondentUpdateDate": "2019/10/01",
    "SSINotes": "",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029162316"
  }
}
'''
## -----------------------------------------------------
accuityCorrespondentSub_delete = "accuityCorrespondentSub/Id/5911218"
accuityCorrespondentSub_get = "accuityCorrespondentSub/Id/5911218"
accuityCorrespondentSub_patch = '''\
{
  "status": 0,
  "usId": "tests"
}
'''
accuityCorrespondentSub_post = '''\
{
"newrec":
  {
    "Id": 5911218,
    "CorrespondentId": 5573559,
    "SubCategory": "MYM",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029162316"
  }
}
'''
accuityCorrespondentSub_put = '''\
{
"newrec":
  {
    "Id": 5911218,
    "CorrespondentId": 5573559,
    "SubCategory": "MYM",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029162316"
  }
}
'''
## -----------------------------------------------------
#accuityLocationAccuityBank_get = 'accuityLocationAccuityBank'
#accuityLocationAccuityCorrespondent_get = 'accuityLocationAccuityCorrespondent'
accuityLocationBankCode_get = 'accuityLocationBankCode/FinId/0'
accuityLocationCheck_get = 'accuityLocationCheck/FinId/0/NationalBankCode/0'
accuityLocationdelete_sh = 'accuityLocation/Id/10002640'
accuityLocationExists_get = 'accuityLocationExists/LocationId/10002640'
#accuityLocationFinIdLookup_get = 'accuityLocationFinIdLookup/SwiftAddress/FNCOVECA'
accuityLocationGet_get = 'accuityLocationGet/FinId/0'
accuityLocation_get = 'accuityLocation/Id/10002640'
accuityLocationReqBankByName_get = 'accuityLocationReqBankByName/Country/VENEZUELA/BankName/BANCO'
accuityLocationReqBankByNameANDTown_get ='accuityLocationReqBankByNameANDTown/Country/VENEZUELA/BankName/BANCO/Town/CARACAS'
accuityLocationReqBankByNameOnly_get = 'accuityLocationReqBankByNameOnly/BankName/BANCO'
accuityLocationReqBankByNameOnlyANDTown_get = 'accuityLocationReqBankByNameOnlyANDTown/BankName/BANCO/Town/CARACAS'
accuityLocationReqBankBySWIFT_get = 'accuityLocationReqBankBySWIFT/SwiftAddress/FNCOVECA'
accuityLocationReqBankBySWIFTANDTown_get = 'accuityLocationReqBankBySWIFTANDTown/SwiftAddress/FNCOVECA/Town/CARACAS'
accuityLocationReqCountries_get = 'accuityLocationReqCountries'
#accuityLocationReqDraftBanks_get ='accuityLocationReqDraftBanks/CurrId/EUR/Country/ITALY'
accuityLocationReqPresentationCountries_get = 'accuityLocationReqPresentationCountries/CurrId/EUR/Country/ITALY'
accuityLocationSwiftCountryLookup_get = 'accuityLocationSwiftCountryLookup/SwiftAddress/BKCHDEFF/Country/AU'
accuityLocationSwiftLookup_get = 'accuityLocationSwiftLookup/SwiftAddress/BKCHDEFF/Currency/AU'
accuityLocation_patch = '''\
{
  "status": 0,
  "usId": "tests"
}
'''
accuityLocation_post = '''\
{
  "newRec": {
    "LocationId": 10002640,
    "CountryCode": "US",
    "InstitutionType": "01",
    "OfficeType": "06",
    "NameAbbrev": "TRADERS AND FARMERS BANK",
    "NameFull": "TRADERS AND FARMERS BANK",
    "BranchName": "",
    "AddressLine1": "10 BLAKE DR",
    "AddressLine2": "",
    "City": "DOUBLE SPRINGS",
    "StateAbbr": "AL",
    "StateFull": "ALABAMA",
    "PostalCode": "35553",
    "CountryName": "UNITED STATES",
    "EmployerTaxId": "",
    "DateOfFinancial": "00000000",
    "CurrentAssets": "             0",
    "HeadOfficeId": 10004340,
    "InstitutionId": 10004340,
    "CorrLocationType": 0,
    "CorrLocationId": 0,
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029161234"
  }
}
'''
accuityLocation_put = '''\
{
  "newRec": {
    "LocationId": 10002640,
    "CountryCode": "US",
    "InstitutionType": "01",
    "OfficeType": "06",
    "NameAbbrev": "TRADERS AND FARMERS BANK",
    "NameFull": "TRADERS AND FARMERS BANK",
    "BranchName": "",
    "AddressLine1": "10 BLAKE DR",
    "AddressLine2": "",
    "City": "DOUBLE SPRINGS",
    "StateAbbr": "AL",
    "StateFull": "ALABAMA",
    "PostalCode": "35553",
    "CountryName": "UNITED STATES",
    "EmployerTaxId": "",
    "DateOfFinancial": "00000000",
    "CurrentAssets": "             0",
    "HeadOfficeId": 10004340,
    "InstitutionId": 10004340,
    "CorrLocationType": 0,
    "CorrLocationId": 0,
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029161234"
  }
}
'''
## -----------------------------------------------------
accuityOfficer_delete = "accuityOfficer/Id/1428369"
accuityOfficer_get = "accuityOfficer/Id/1428369"
accuityOfficer_patch = '''\
{
  "status": 0,
  "usId": "tests"
}
'''
accuityOfficer_post = '''\
{
  "newRec": {
    "Id": 1428369,
    "LocationId": 20423300,
    "Department": "520",
    "OfficerName": "ROBERT ALMEIDA",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029161234"
  }
}
'''
accuityOfficer_put = '''\
{
  "newRec": {
    "Id": 1428369,
    "LocationId": 20423300,
    "Department": "520",
    "OfficerName": "ROBERT ALMEIDA",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029161234"
  }
}
'''
## -----------------------------------------------------
accuityRoutingAtt_delete = "accuityRoutingAtt/Id/113478"
accuityRoutingAtt_get = "accuityRoutingAtt/Id/113478"
accuityRoutingAtt_patch = '''\
{
  "status": 0,
  "usId": "tests"
}
'''
accuityRoutingAtt_post = '''\
{
  "newRec":
  {
    "Id": 113478,
    "RoutingType": "ABA",
    "RoutingCode": "011500010",
    "RoutingCodeAlt": "011500010",
    "OverrideRoutingCode": "026009593",
    "OverrideRoutingCodeAlt": "026009593",
    "ClearingSystem": "FEDWIRE",
    "MembershipType": "",
    "RoutingTypeInfo": "R",
    "RoutingCodeStatus": "A",
    "ACHFlag": "Y",
    "FedwireFundStatus": "I",
    "FedwireSecurityStatus": "I",
    "WireTranCode": "",
    "EffectiveDate": "",
    "DeactivatedDate": "",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029161234"
  }
}
'''
accuityRoutingAtt_put = '''\
{
  "newRec":
  {
    "Id": 113478,
    "RoutingType": "ABA",
    "RoutingCode": "011500010",
    "RoutingCodeAlt": "011500010",
    "OverrideRoutingCode": "026009593",
    "OverrideRoutingCodeAlt": "026009593",
    "ClearingSystem": "FEDWIRE",
    "MembershipType": "",
    "RoutingTypeInfo": "R",
    "RoutingCodeStatus": "A",
    "ACHFlag": "Y",
    "FedwireFundStatus": "I",
    "FedwireSecurityStatus": "I",
    "WireTranCode": "",
    "EffectiveDate": "",
    "DeactivatedDate": "",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029161234"
  }
}
'''
## -----------------------------------------------------
accuityRoutingCode_delete = "accuityRoutingCode/Id/694362"
accuityRoutingCode_get = "accuityRoutingCode/Id/694362"
accuityRoutingCode_patch = '''\
{
  "status": 0,
  "usId": "tests"
}
'''
accuityRoutingCode_post = '''\
{
  "newRec": {
    "Id": 694362,
    "LocationId": 98008550,
    "Department": "MAIN",
    "Rank": "1",
    "RoutingType": "ARRN",
    "RoutingCode": "017 0291",
    "RoutingCodeAlt": "0170291",
    "FederalDistrict": "",
    "SwiftPairingCode": "BFRPARBAXXX",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029161234"
  }
}
'''
accuityRoutingCode_put = '''\
{
  "newRec": {
    "Id": 694362,
    "LocationId": 98008550,
    "Department": "MAIN",
    "Rank": "1",
    "RoutingType": "ARRN",
    "RoutingCode": "017 0291",
    "RoutingCodeAlt": "0170291",
    "FederalDistrict": "",
    "SwiftPairingCode": "BFRPARBAXXX",
    "Status": 0,
    "USId": "accuityimport",
    "TmStamp": "20191029161234"
  }
}
'''
## -----------------------------------------------------
pang_get = "pang/given/14/add/3/and/1.4"
ping_get = "ping/given/13/add/12/add/2"
pong_get = "pong/given/12/add/2"
