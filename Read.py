#!/usr/bin/env python

import serial
import json
import time
import MySQLdb as mariadb
import os
import subprocess
from firebase import firebase

firebase = firebase.FirebaseApplication('https://dsd-agriculture.firebaseio.com/')
result= firebase.get('prueba',None)

while True:
	try: 
		db = mariadb.connect("localhost","root","root","temperatura")
	except: 
        	print("Can't connect to database") 
	if result != '\n':
		try:
			print("\t Datos de Firebase:")
			print(result['HumedadSuelo'])
			print(result['Humedad'])
			print(result['Lluvia'])
			print(result['Luz'])
			print(result['Temperatura'])
			cursor = db.cursor()
			cursor.execute("INSERT INTO dsdrt(fecha,humgr,hum,rain,light,temp)values(now(),"+str(result['HumedadSuelo'])+","+str(result['Humedad'])+","+str(result['Lluvia'])+","+str(result['Luz'])+","+str(result['Temperatura'])+")")
			db.commit()
		except ValueError:
			print "Error al escribir "
	print("\t Ejecuto Subproceso en C")
	subprocess.call(["./agri",""])
	print("\n Leo valor proximo")
	time.sleep(2)
	archivo = open("fichero.txt", "r")
	datos=[]
	for linea in archivo.readlines():
    		datos.append(linea[:-1])
	archivo.close()
	vs=float(datos[5])
        try:
                db = mariadb.connect("localhost","root","root","temperatura")
        except:
                print("Can't connect to database")
        if vs != '\0':
                try:
                        print("Valor proximo: ")
                        print(vs)
                        cursor = db.cursor()
                        cursor.execute("INSERT INTO pred(afecha,tptn)values(now(),"+str(vs)+")")
                        db.commit()
                except ValueError:
                        print "Error al escribir "
        time.sleep(20)
