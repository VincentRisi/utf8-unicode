import zlib
from datetime import datetime, date, time

def decompress(length, data):
    sign = data[:4]
    if sign == b'\xED\xAC\xAC\xED':
        return "".join(chr(x) for x in zlib.decompress(data[4:]))
    return "".join(chr(x) for x in data)

class OracleReturning():
    def __init__(self, table, field):
        self.head = ''
        self.output = ''
        self.sequence = f'  {table}Seq.NextVal,' 
        self.tail = f'  returning {field} into :{field}'; 
        self.dropField = '';
        self.doesGeneratedKeys = False;
        self.usesPlSql = True
    def check_use(self, value):
        return value

def to_date14(data, format='%Y%m%d%H%M%S'):
    return datetime.strptime(data, format)

def to_date8(data, format='%Y%m%d'):
    return datetime.strptime(data, format)

def to_char14(data, format='%Y%m%d%H%M%S'):
    return datetime.strftime(data, format)

def to_char8(data, format='%Y%m%d'):
    return datetime.strftime(data, format)

##proc _sequence
##output (single)
##   seq int
##sqlcode
##   select &tableSeq(128).NextVal from dual
##endcode

class Dutil_sequence(object):
    def _make(self): return Dutil_sequence()
    __slots__ = ['seq', 'tableSeq']
    def __init__(self):
        self.seq = ''
        self.tableSeq = ''
    def _fields(self):
        return Dutil_sequence.__slots__

class util_sequence(Dutil_sequence):
    def _get_output(self, _result):
        self.seq = _result[0]
        return 1
    def _copy_input(self, record):
        record.tableSeq = self.tableSeq
    def execute(self, connect): # _sequence
        _command = f'select {self.tableSeq}.NextVal from dual' 
        cursor = connect.cursor()
        cursor.execute(_command)
        record = util_sequence()
        self._copy_input(record)
        result = cursor.fetchone()
        if result == None:
            return None
        record._get_output(result)
        return record

def get_sequence(connect, table):
    query = util_sequence()
    query.tableSeq = f'{table}Seq'
    query.execute(connect)
    return query.seq

def get_timestamp():
    return datetime
