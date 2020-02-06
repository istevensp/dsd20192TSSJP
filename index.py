#!/usr/bin/env python
import cgi,cgitb
import MySQLdb as mariadb

print "Content-Type: text/html;charset=utf-8"
print "\n\n"
print "<html>"
print "<head>"
#print '<link href= "control.css" rel="stylesheet" type="text/css">'
print "<title>"
print "RT Graph"
print "</title>"

print "<script type=\"text/javascript\"src=\"https://www.google.com/jsapi\"></script>"
print "<script type=\"text/javascript\">"
print "google.load('visualization', '1', {packages:['corechart']});"
print "google.setOnLoadCallback(drawChart);"

db = mariadb.connect("localhost","root","root","temperatura")
curs = db.cursor()
curs.execute("SELECT * FROM dsdrt order By fecha DESC limit 0,100")

print "	     function drawChart() {"
print "	       var data = google.visualization.arrayToDataTable(["
print "		 ['Fecha', 'Humedad', 'Lluvia', 'Luz', 'Humedad de la tierra', 'Temperatura']"
for data in curs.fetchall():
	print ",['"+str(data[1])+"',"+str(data[3])+","+str(data[4])+","+str(data[5])+","+str(data[2])+","+str(data[6])+"]"
print		 "]);"
db.close()

print "		   var options = {"
print "		     title: 'Variables medidas'	 };"
print "		   var chart = new google.visualization.LineChart(document.getElementById('chart_div'));"
print "		   chart.draw(data, options);	   }"
print "	       </script>"

print "</head>"
print "<body>"
print "<h1><center>Grafica en tiempo real</center></h1>"
print "<center>"
print '<div id="chart_div" style="width: 1200px; height: 500px;"></div>'
print "<h2></h2>"
print '<a href="index.py"><img src="principal.png" width="200" height="70" alt="Achatada" border="0"></a>'
print "<h3></h3>"
print '<a href="humedad.py"><img src="humedad.png" width="200" height="70" alt="Achatada" border="0"></a>'
print '<a href="lluvia.py"><img src="lluvia.png" width="200" height="70" alt="Achatada" border="0"></a>'
print '<a href="luz.py"><img src="luz.png" width="200" height="70" alt="Achatada" border="0"></a>'
print '<a href="humedadgr.py"><img src="humedadgr.png" width="200" height="70" alt="Achatada" border="0"></a>'
print '<a href="temperatura.py"><img src="temperatura.png" width="200" height="70" alt="Achatada" border="0"></a>'
print "<h4></h4>"
print '<a href="index.html"><img src="back2.png" width="250" height="70" alt="Achatada" border="0"></a>'
print "</center>"
print '<meta http-equiv="Content-Type" content="text/html; charset=utf-8">'
print '<meta http-equiv="refresh" content="28;URL=http://192.168.2.100/index.py">'
#print '<meta http-equiv="Content-Type" content="text/html; charset=utf-8">'
#print '<meta http-equiv="refresh" content="2;URL=http://192.168.2.231/control.html">'
print "</body>"
print "</html>"

