import os
import os.path
import zlib
import base64
from INTRINSICS import *

from datetime import datetime, date, time

def base64_encode(data):
    return base64.standard_b64encode(data).decode('ascii')

def base64_decode(data):
    return base64.standard_b64decode(data)

def decompress(length, data):
    sign = data[:4]
    if sign == b'\xED\xAC\xAC\xED':
        result = ''.join(chr(x) for x in zlib.decompress(data[4:]))
    else:        
        result = ''.join(chr(x) for x in data)
    return len(result), result

def to_date14(data, format='%Y%m%d%H%M%S'):
    return datetime.strptime(data, format)

def to_date8(data, format='%Y%m%d'):
    return datetime.strptime(data, format)

def to_char14(data, format='%Y%m%d%H%M%S'):
    return datetime.strftime(data, format)

def to_char8(data, format='%Y%m%d'):
    return datetime.strftime(data, format)

def expand(value: str) -> str:
    while True:
        p = value.find('$(')
        if p < 0: break
        e = value.find(')', p)
        if e < 0: break
        var = value[p+2:e]
        env=os.getenv(var,'')
        value = '%s%s%s' % (value[:p], env, value[e+1:])
    if value.find('..') == 0:
        value = os.path.abspath(value)
    return value;

def load_config(config_file):
    if os.path.exists(config_file) == False:
        return False, None
    infile = open(config_file, 'rt')
    lines = infile.readlines()
    infile.close()
    config = dict()
    START, IN_CONTROL = 0, 1
    state = START
    config['ulist'] = []
    for line in lines:
        line=line.strip() 
        p = line.find('#')
        if p >= 0:
            line=line[:p].strip()
        if len(line) == 0:
            continue    
        p = line.find('[')
        if p == 0:
            e = line.find(']')
            if e > 0:
                section = line[1:e].replace(' ','')
                if len(section) == 3 and section[0] == 'n':
                    config['ulist'].append(section)
            continue
        p = line.find('{')
        if p == 0:
            e = line.find('}')
            if e > 0:
                key = f'{section}.{line[1:e]}'.replace(' ','')
                value = line[e+1:]
                config[key] = expand(value)
    return True, config

def do_logons(config_file):
    rc, config = load_config(config_file)
    if rc == True:
        filename = config['ServerOptions.Log']
        level = config['ServerOptions.LogLevel']
        display = config['ServerOptions.LogDisplay']
        receive = config['ServerOptions.LogReceive']
        transmit = config['ServerOptions.LogTransmit']
        log_open(filename)
        log_level(level)
        log_display(display)
        log_receive(receive)
        log_transmit(transmit)
        log_info(f'{filename} opened')
        for u in config['ulist']:
            user, where = config[f'{u}.Connection'].split('/')
            password, server = where.split('@')
            binfile = config[f'{u}.BinFile']
            log_info ('%s %s %s' % (user, password, server))
            connId = logon(binfile, user, password, server)
    return rc, config

def display_dbrec(name, rec):
    log_info (f'{name}:')
    if rec is not None:
        data = rec._data()
        for i, field in enumerate(rec._fields()):
            log_info (f'  {field}: {data[i]}')
    else:
        log_warn(f'{name}: rec passed in is None')
