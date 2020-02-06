#!/usr/bin/env python

import serial
import json
import time
import MySQLdb as mariadb
from firebase import firebase

archivo = open("fichero.txt", "r")
datos=[]
for linea in archivo.readlines():
    print(linea)
    datos.append(linea[:-1])
archivo.close()
#print(datos)
hh= float(datos[0])
hhgr= float(datos[1])
ll=int(datos[2])
lg=float(datos[3])
tt=float(datos[4])
vs=float(datos[5])

while True:
	try: 
		db = mariadb.connect("localhost","root","root","temperatura")
		#print("Conexion Exitosa a la base de datos")
	except: 
        	print("Can't connect to database") 
	if datos != '\n':
		try:
			#f=open("get.txt","w")
			#f.write(str(result['HumedadSuelo'])+";"+str(result['Humedad'])+";"+str(result['Lluvia'])+";"+str(result['Luz'])+";"+str(result['Temperatura']))
			#f.close()
			print(datos)
			#print(result['Humedad'])
			#print(result['Lluvia'])
			#print(result['Luz'])
			#print(result['Temperatura'])
			cursor = db.cursor()
			cursor.execute("INSERT INTO pred(afecha,tptn)values(now(),"+str(vs)+")")
			db.commit()
		except ValueError:
			print "Error al escribir "
	time.sleep(29)
