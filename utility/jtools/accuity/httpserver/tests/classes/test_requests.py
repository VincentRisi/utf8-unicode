import sys
import requests
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-H', '--host',    type=str, default='localhost')
parser.add_argument('-P', '--port',    type=str, default='32135')
parser.add_argument('-b', '--build',   type=str, default=r'C:\nedbank\ownfronts\mcpe-single\source\putty3\idl2\accuity\genned\http')
parser.add_argument('-s', '--support', type=str, default=r'C:\nedbank\ownfronts\mcpe-single\build\tools\pyscripts')
args = parser.parse_args()
host = args.host
port = args.port
build = args.build
support = args.support

sys.path.append(build)
sys.path.append(support)
import accuity
HttpError = accuity.HttpError

def do_ping():
    print ('ping')
    print ('****')
    req = accuity.ping_request()
    req.given = 11
    req.add = 1
    try:
       response = req.get(host, port)
       rsp = accuity.ping_response()
       rsp.load(response)
       accuity.display(rsp)
    except HttpError as e:
        print (f'{e.url}: {e.error}')        

def do_pong():
    print ('pong')
    print ('****')
    req = accuity.pong_request()
    req.given = 11
    req.add = 1
    try:
       response = req.get(host, port)
       rsp = accuity.pong_response()
       rsp.load(response)
       accuity.display(rsp)
    except HttpError as e:
        print (f'{e.url}: {e.error}')        

def do_pang():
    print ('pang')
    print ('****')
    req = accuity.pang_request()
    req.given = 11
    req.add = 1
    req.gotten = 'Mein'
    try:
        response = req.get(host, port)
        rsp = accuity.pang_response()
        rsp.load(response)
        accuity.display(rsp)
    except HttpError as e:
        print (f'{e.url}: {e.error}')  
        
def do_Contact_post():
    print ('Contact_post')
    print ('************')
    req = accuity.accuityContactInsert_request()
    rsp = accuity.accuityContactInsert_response()
    req.newRec = accuity.tAccuityContact()
    req.newRec.Id = 2
    req.newRec.LocationId = 10000010
    req.newRec.Department = 'MAIN'
    req.newRec.ContactType = 'TEL'
    req.newRec.ContactInfo = '1-479-271-1363'
    req.newRec.Status = 0
    req.newRec.USId = 'TESTREQ'
    req.newRec.TmStamp = '20200701'
    try:
        response = req.post(host, port)
        rsp.load(response)
        accuity.display(rsp)
    except HttpError as e:
        print (f'{e.url}: {e.error}')  

def do_Contact_put():
    print ('Contact_put')
    print ('***********')
    req = accuity.accuityContactUpdate_request()
    rsp = accuity.accuityContactUpdate_response()
    req.newRec = accuity.tAccuityContact()
    req.newRec.Id = 2
    req.newRec.LocationId = 10000010
    req.newRec.Department = 'MAINY'
    req.newRec.ContactType = 'TEL'
    req.newRec.ContactInfo = '1-479-271-1363'
    req.newRec.Status = 0
    req.newRec.USId = 'TESTREQ'
    req.newRec.TmStamp = '20200701'
    try:
        response = req.put(host, port)
        rsp.load(response)
        accuity.display(rsp)
    except HttpError as e:
        print (f'{e.url}: {e.error}')

def do_Contact_patch():
    print ('Contact_patch')
    print ('*************')
    req = accuity.accuityContactUpdateAllStatus_request()
    req.status = 0
    req.usId = 'TESTREQ'
    try:
        response = req.patch(host, port)
        print (response.status_code)
    except HttpError as e:
        print (f'{e.url}: {e.error}')

def do_Contact_get():
    print ('Contact_get')
    print ('***********')
    req = accuity.accuityContactSelectOne_request()
    rsp = accuity.accuityContactSelectOne_response()
    req.Id = 2
    try:
        response = req.get(host, port)
        rsp.load(response)
        accuity.display(rsp)
    except HttpError as e:
        print (f'{e.url}: {e.error}')

def do_Contact_delete():
    print ('Contact_delete')
    print ('**************')
    req = accuity.accuityContactDeleteOne_request()
    req.Id = 2
    try:
        response = req.delete(host, port)
        print (response.status_code)
    except HttpError as e:
        print (f'{e.url}: {e.error}')

def do_ContactExists_get():
    print ('ContactExists_get')
    print ('*****************')
    req = accuity.accuityContactExists_request()
    rsp = accuity.accuityContactExists_response()
    req.Id = 2
    try:
        response = req.get(host, port)
        rsp.load(response)
        accuity.display(rsp)
    except HttpError as e:
        print (f'{e.url}: {e.error}')

def main():
    do_ping()
    do_pong()
    do_pang()
    do_Contact_post()
    do_Contact_put()
    do_Contact_patch()
    do_ContactExists_get()
    do_Contact_get()
    do_Contact_delete()

if __name__ == '__main__':
    main()
