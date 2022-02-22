import sys, os

def pathed(dir):
    os.makedirs(dir, exist_ok=True)
    return dir

build_sets = '''\
example|GPSPayment|GPS_PAYMENT
example|pacs.008.001.08|PACS_008
example|pain.001.001.09|PAIN_001
messageorch/Common|file_message|FILE_MESSAGE
aeoi|SARSForeignTaxpayerInfoDeclarationV2.3.1|SARS_FOREIGNTAXPAYERINFO_DECLARATION_V231
aeoi|SARSForeignTaxpayerInfoDeclarationV2.3|SARS_FOREIGNTAXPAYERINFO_DECLARATION_V23
aeoi|SARSForeignTaxpayerInfoSupportingDataSubmissionV1.1|SARS_FOREIGNTAXPAYERINFO_SUPPORTINGDATASUBMISSION_V11
aeoi|SARSForeignTaxpayerInfoSupportingDataV1.1|SARS_FOREIGNTAXPAYERINFO_SUPPORTINGDATA
aeoi|SARSRequestForeignTaxpayerInfoDeclarationV2.3|SARS_REQUEST_FOREIGNTAXPAYERINFO_DECLARATION_V23
aeoi|SARSRequestForeignTaxpayerInfoDeclarationV2.3.1|SARS_REQUEST_FOREIGNTAXPAYERINFO_DECLARATION_V231
aeoi|SARSSubmitForeignTaxpayerInfoDeclarationV2.3|SARS_SUBMIT_FOREIGNTAXPAYERINFO_DECLARATION_V23
aeoi|SARSSubmitForeignTaxpayerInfoDeclarationV2.3.1|SARS_SUBMIT_FOREIGNTAXPAYERINFO_DECLARATION_V231
aeoi|SARSThirdPartyCorrespondenceV1.10|SARS_THIRDPARTYCORRESPONDENCE_V110
aeoi|SARSThirdPartySubmissionHeaderV1.11|SARS_THIRDPARTYSUBMISSION_HEADER_V111
'''.splitlines()

CMAKE_CURRENT_SOURCE_DIR = 'C:/sarsUtil/pytools/xsd_py_gen'
CMAKE_CURRENT_BINARY_DIR = 'C:/sarsUtil/pytools/xsd_py_gen/build'

def process(build_set):
    dir_name, base_name, make_name = build_set.split('|')
    outPath = pathed(f'{CMAKE_CURRENT_BINARY_DIR}/{dir_name}/py')
    logPath = pathed(f'{CMAKE_CURRENT_BINARY_DIR}/{dir_name}/log')
    inFile  = f'{CMAKE_CURRENT_SOURCE_DIR}/xsds/{dir_name}/{base_name}.xsd'
    inPath  = f'{CMAKE_CURRENT_SOURCE_DIR}/xsds/{dir_name}'
    outFile = f'{outPath}/XSD_{make_name}.py'
    logFile = f'{logPath}/{base_name}.log'
    sys.argv = ['xsd_py_gen.py', '-i', f'{inFile}', '-I', f'{inPath}', '-o', f'{outFile}', '-l', f'{logPath}']

process(build_sets[4])
import xsd_py_gen
xsd_py_gen.main()