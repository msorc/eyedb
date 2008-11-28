#!/usr/bin/python
# Usage:
# ./openoffice-report.py DOCUMENT SHEET LABEL ROW ROW...
# ./openoffice-report.py /home/francois/projects/eyedb/eyedb-bench-svn/results/poleposition.ods 'Barcelona' 'EyeDB_java' '0,1,2,3,4,5' '10,11,12,13,14,15' '20,21,22,23,24,25' '30,31,32,33,34,35' 
### insertion of headers does not work with setValue()
# ./mytest.py '/home/francois/projects/eyedb/eyedb-bench-svn/results/poleposition.ods' 'Barcelona' 'EyeDB_java' 'selects,objects,write (ms),read (ms),query (ms),delete (ms)' '0,1,2,3,4,5' '10,11,12,13,14,15' '20,21,22,23,24,25' '30,31,32,33,34,35' 
# Before, launch:
# oocalc "-accept=socket,host=localhost,port=8100;urp;"
# oocalc -nologo -nodefault -accept='socket,host=localhost,port=8100;urp;'

import string
import uno
import unohelper
import os.path
import sys

class CalcDocument:
    def __init__( self, file, host='localhost', port=8100):
        localContext = uno.getComponentContext()
        resolver = localContext.ServiceManager.createInstanceWithContext( 'com.sun.star.bridge.UnoUrlResolver', localContext)
        context = resolver.resolve( 'uno:socket,host=' + host + ',port=' + str(port) + ';urp;StarOffice.ComponentContext' )
        manager=context.ServiceManager
        desktop = manager.createInstance( 'com.sun.star.frame.Desktop' )
        # empty document
        #doc = desktop.loadComponentFromURL( 'private:factory/scalc', '_blank', 0, () )
        # existing document
        url = unohelper.systemPathToFileUrl( os.path.abspath(file));
        self.__doc = desktop.loadComponentFromURL( url, '_blank', 0, ())

    def sheet( self, name):
         return Sheet(self.__doc.getSheets().getByName( name))

class Sheet:
    def __init__(self,s):
        self.__sheet = s

    def range( self, w):
        if isinstance(w,str):
            return Range(self.__sheet.getCellRangeByName( w))
        elif isinstance(w,list) or isinstance(w,tuple):
            return Range(self.__sheet.getCellRangeByPosition(w[0],w[1],w[2],w[3]))

class Range:
    def __init__(self,r):
        self.__range = r
        a = self.__range.getRangeAddress()
        self.__address = (a.StartColumn,a.StartRow,a.EndColumn,a.EndRow)

    def address(self):
        return self.__address

    def set(self,value):
        if isinstance(value,list) or isinstance(value,tuple):
            count = 0
            row = self.__address[0] == self.__address[2]
            for v in value:
                if row:
                    self.__range.getCellByPosition(0,count).setValue(v)
                else:
                    self.__range.getCellByPosition(count,0).setValue(v)
                count = count + 1
        else:
            self.__range.getCellByPosition(0,0).setValue(value)

docName = sys.argv[1]
sheetName = sys.argv[2]
rangeName = sys.argv[3]

d = CalcDocument( docName)
s = d.sheet( sheetName)
r = s.range( rangeName)
a = r.address()

count = 0
offset = 2
for v in sys.argv[4:]:
    t = v.split(',')
    r = s.range((a[0],a[1]+offset+count,a[0]+len(t),a[1]+offset+count))
    r.set(t)
    count = count + 1
