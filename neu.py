#!/usr/bin/env python
import cgi,cgitb
import MySQLdb as mariadb

print "Content-Type: text/html;charset=utf-8"
print "\n\n"
print "<html>"
print "<head>"
#print '<link href= "control.css" rel="stylesheet" type="text/css">'
print "<title>"
print "Red Neuronal"
print "</title>"

print "<script type=\"text/javascript\"src=\"https://www.google.com/jsapi\"></script>"
print "<script type=\"text/javascript\">"
print "google.load('visualization', '1', {packages:['corechart']});"
print "google.setOnLoadCallback(drawChart);"

db = mariadb.connect("localhost","root","root","temperatura")
curs = db.cursor()
curs.execute("SELECT * FROM pred order By afecha DESC limit 0,150")

print "	     function drawChart() {"
print "	       var data = google.visualization.arrayToDataTable(["
print "		 ['Fecha', 'Humedad del Suelo']"
for data in curs.fetchall():
	print ",['"+str(data[0])+"',"+str(data[1])+"]"
print		 "]);"
db.close()

print "		   var options = {"
print "		     title: 'Humedad del suelo Posible'	 };"
print "		   var chart = new google.visualization.LineChart(document.getElementById('chart_div'));"
print "		   chart.draw(data, options);	   }"
print "	       </script>"

print "</head>"
print "<body>"
print "<h1><center>Prediccion de humedad del suelo</center></h1>"
print "<center>"
print '<div id="chart_div" style="width: 1200px; height: 500px;"></div>'
print "<h2></h2>"
print '<a href="neu.py"><img src="reload.png" width="60" height="60" alt="Achatada" border="0"></a>'
print "<h3></h3>"
print '<a href="index.html"><img src="back2.png" width="250" height="70" alt="Achatada" border="0"></a>'
print "</center>"
print '<meta http-equiv="Content-Type" content="text/html; charset=utf-8">'
print '<meta http-equiv="refresh" content="28;URL=http://192.168.2.100/neu.py">'
#print '<meta http-equiv="Content-Type" content="text/html; charset=utf-8">'
#print '<meta http-equiv="refresh" content="2;URL=http://192.168.2.231/control.html">'
print "</body>"
print "</html>"

