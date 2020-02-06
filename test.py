#!/usr/bin/env python

import serial
import json
import time
import MySQLdb as mariadb
from firebase import firebase

try:
    from ctypes import *
except ImportError:
    print 'ERROR! La biblioteca *ctypes* para Python no esta disponible.'
    sys.exit(-1)
#firebase = firebase.FirebaseApplication('https://dsd-agriculture.firebaseio.com/')
#result= firebase.get('prueba',None)
libtest = cdll.LoadLibrary('./libpre.so')
print 'El proximo valor es:', libtest.prediccion(42,34,357,2,272)
#print(result['HumedadSuelo'])
#print(result['Humidity'])
#print(result['Lluvia'])
#print(result['Luz'])
#print(result['Temperatura'])
