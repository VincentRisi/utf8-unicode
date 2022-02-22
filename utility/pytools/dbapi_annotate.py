import zlib
from datetime import datetime, date, time

class Binary():
    def __init__(self, length, null=False):
        self.name = 'Binary'
        self.length = length
        self.null = null

class Blob():
    def __init__(self, length, null=False):
        self.name = 'Blob'
        self.length = length
        self.null = null

class Boolean():
    def __init__(self, length, null=False):
        self.name = 'Boolean'
        self.length = length
        self.null = null

class Char():
    def __init__(self, length, null=False):
        self.name = 'Char'
        self.length = length
        self.null = null

class Clob():
    def __init__(self, length, null=False):
        self.name = 'Clob'
        self.length = length
        self.null = null

class Date():
    def __init__(self, null=False):
        self.name = 'Date'
        self.null = null

class DateTime():
    def __init__(self, null=False):
        self.name = 'DateTime'
        self.null = null

class Float():
    def __init__(self, length, scale, null=False):
        self.name = 'Float'
        self.length = length
        self.scale = scale
        self.null = null

class HugeCHAR():
    def __init__(self, length, null=False):
        self.name = 'HugeCHAR'
        self.length = length
        self.null = null

class Image():
    def __init__(self, length, null=False):
        self.name = 'Image'
        self.length = length
        self.null = null

class Int():
    def __init__(self, length, null=False):
        self.name = 'Int'
        self.length = length
        self.null = null

class LongInt():
    def __init__(self, length, null=False):
        self.name = 'LongInt'
        self.length = length
        self.null = null

class Money():
    def __init__(self, length, null=False):
        self.name = 'Money'
        self.length = length
        self.null = null

class NChar():
    def __init__(self, length, null=False):
        self.name = 'NChar'
        self.length = length
        self.null = null

class SmallInt():
    def __init__(self, length, null=False):
        self.name = 'SmallInt'
        self.length = length
        self.null = null

class Status():
    def __init__(self, length, null=False):
        self.name = 'Status'
        self.length = length
        self.null = null

class Time():
    def __init__(self, null=False):
        self.name = 'Time'
        self.null = null

class TimeStamp():
    def __init__(self, null=False):
        self.name = 'TimeStamp'
        self.null = null

class TinyInt():
    def __init__(self, length, null=False):
        self.name = 'TinyInt'
        self.length = length
        self.null = null

class Unhandled():
    def __init__(self, name, length, null=False):
        self.name = name
        self.length = length
        self.null = null

class UserStamp():
    def __init__(self, length, null=False):
        self.name = 'UserStamp'
        self.length = length
        self.null = null

class VarNum():
    def __init__(self, length, null=False):
        self.name = 'VarNum'
        self.length = length
        self.null = null

class XMLTYPE():
    def __init__(self, length, null=False):
        self.name = 'XMLTYPE'
        self.length = length
        self.null = null
