import requests
import argparse
import http_run

parser = argparse.ArgumentParser()
parser.add_argument('-H', '--host', type=str, default='localhost')
parser.add_argument('-P', '--port', type=str, default='32135')
parser.add_argument('rest', nargs='*', type=str)
args = parser.parse_args()
rest = args.rest
host = args.host
port = args.port
headers = {'Accept': 'application/json', 'Content-Type': 'application/json'}

routines = []
for routine in dir(http_run):
    if routine[0] != '_':
        routines.append(routine)

def split_path(data):
    params = {}
    path = data
    p1 = data.split('?')
    if len(p1) == 2:
        path, parms = p1
        p2 = parms.split('&')
        for parm in p2:
            p3 = parm.split('=')
            if len(p3) == 2:
                params[p3[0]] = p3[1]
    return path, params

def get(data):
    path, params = split_path(data)
    print (path, params)
    r = requests.get(f'http://{host}:{port}/{path}', params=params, headers=headers)
    print (r.text)
    print (r.url)
    #rec = r.json()
    #print (rec)

def delete(data):
    path, params = split_path(data)
    print (path, params)
    r = requests.delete(f'http://{host}:{port}/{path}', params=params, headers=headers)
    print (r.text)

def options(path, data):
    if '{' in data:
        print (path, data)
        r = requests.options(f'http://{host}:{port}/{path}', data=data, headers=headers)
        print (r.text)
    else:
        path, params = split_path(data)
        print (path, params)
        r = requests.options(f'http://{host}:{port}/{path}', params=params, headers=headers)
        print (r.text)

def head(path, data):
    if '{' in data:
        print (path, data)
        r = requests.head(f'http://{host}:{port}/{path}', data=data, headers=headers)
        print (r.text)
    else:
        path, params = split_path(data)
        print (path, params)
        r = requests.head(f'http://{host}:{port}/{path}', params=params, headers=headers)
        print (r.text)

def post(path, data):
    if '{' in data:
        print (path, data)
        r = requests.post(f'http://{host}:{port}/{path}', data=data, headers=headers)
        print (r.text)

def put(path, data):
    if '{' in data:
        print (path, data)
        r = requests.put(f'http://{host}:{port}/{path}', data=data, headers=headers)
        print (r.text)

def patch(path, data):
    if '{' in data:
        print (path, data)
        r = requests.patch(f'http://{host}:{port}/{path}', data=data, headers=headers)
        print (r.text)

def main():
    for routine in routines:
        count = 0  
        for opt in rest:
            if not opt in routine:
                continue
            count += 1
        if count == len(rest):
            parts = routine.split('_')
            type = parts[-1]
            path = '_'.join(parts[0:-1])
            data = eval(f'http_run.{routine}')
            try:
                if type == 'get':
                    get(data)
                elif type == 'delete':
                    delete(data)
                elif type == 'options':
                    options(path, data)
                elif type == 'head':
                    head(path, data)
                elif type == 'post':
                    post(path, data)
                elif type == 'put':
                    put(path, data)
                elif type == 'patch':
                    patch(path, data)
            except Exception as ex:
                print (type, path, data, ex.args, "FAILED")
                pass

if __name__ == '__main__':
    main()

