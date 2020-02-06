#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <unistd.h>
#define TAM 6
/*=======  niNN vector de entrada =========*/
//float niNN[]={59.0,30.0,757.0,2491.5,1.0}; //% in: 'Humedad Aire','Temperatura','Humedad_Suelo_ant','Luz','Lluvia'
// out: Humedad_Suelo_Sig
/*=======  funcion mapminmax =========*/
float *mapminmax(float input[])
{
    int i;
    float ymax=1,ymin=-1;
    float xmin[]={42,27.1000000000000014210854715202,434,119.760000000000005115907697473,0};
    float xmax[]={69,32.8999999999999985789145284798,1001,9581.9899999999997817212715745,2};
    static float output[5];				//<--------------segun el numero de entradas
    for(i=0;i<5;i++)					//<--------------segun el numero de entradas
    {
        output[i]=((ymax-ymin)/(xmax[i]-xmin[i]))*(input[i]-xmin[i])+ymin;
    }
    return output;
}
/*=======  producto punto =========*/
float DotProduct(float input1[], float input2[])
{
    int i=0;
    float acumulador=0;
    for (i=0;i!=5;i++)					//<--------------segun el numero de entradas
    {
        acumulador=input1[i]*input2[i]+acumulador;
    }
    return acumulador;
}
/*=======  tangente sigmoidal =========*/
float tansig(float input1)
{
    float tansigout=(2/(1+exp(-2*input1))-1);
    return tansigout;
}
/*=======  hidden layer =========*/
float *layer1(float *input1)
{
    float static output[10];
    float w0[]={1.6742464298832022696,-0.64369128181948809253,2.7481944040470684953,1.4534233297716174782,-3.6660204906480786669};   
    float w1[]={5.42263202332974803,0.50880012865517587617,-2.8165819701696293009,-0.7935129884565619296,-0.52945188740556148321};
    float w2[]={-2.449845777812604819,1.175344588457386541,0.27098489348108201202,1.3400667787978695067,0.95881799721686422622};
    float w3[]={-0.83683103268900393967,0.183117815817271129,-0.44184948769975773697,0.03851891976583136229,0.1533404320177076674};
    float w4[]={1.8123234556808858198,0.63509576270517897889,3.4001553120878962311,-0.49320283720538699024,-0.17741088640083546579};
    float w5[]={-1.5298645782845856633,0.12334200449075957695,0.93204639580521642106,0.63057775715769825986,-1.1696894424033175142};
    float w6[]={0.48040535847814680448,0.095734395978936126514,1.2563168685136798519,-0.089696330121251763301,-0.083387429075690619462};
    float w7[]={-0.40545696340345238617,-0.56646707076413094217,-0.84958190873238137453,0.27103945108406402031,0.49102724158735222559};
	float w8[]={0.60410927371538358788,1.5764506030732143849,0.89096152544066853363,3.3640321133287325317,2.1283011329933896327};
	float w9[]={1.3263619316254648961,-1.4829120061071523473,-3.8700986612241488061,-2.3387011196671583235,2.2687520745834666158};
	output[0]=tansig(DotProduct(input1,w0)+(-7.1443664812724518853));
    output[1]=tansig(DotProduct(input1,w1)+(-3.0529923521068846881));
    output[2]=tansig(DotProduct(input1,w2)+(0.48389118508769340954));
    output[3]=tansig(DotProduct(input1,w3)+(-0.027863107139115286803));
    output[4]=tansig(DotProduct(input1,w4)+(-1.0864097478391869789));
    output[5]=tansig(DotProduct(input1,w5)+(0.5184890599939740996));
    output[6]=tansig(DotProduct(input1,w6)+(-0.32184179938335366611));
    output[7]=tansig(DotProduct(input1,w7)+(1.9156214453960740407));
    output[8]=tansig(DotProduct(input1,w8)+(3.5409243826392216015));
	output[9]=tansig(DotProduct(input1,w9)+(1.7562291985174833808));
    return output;
}
/*=======  producto punto =========*/
float DotProduct1(float input1[], float input2[])
{
    int i=0;
    float acumulador=0;
    for (i=0;i!=10;i++)									//<--------Segun el numero de neuronas en el hidden layer
    {
        acumulador=input1[i]*input2[i]+acumulador;
    }
    return acumulador;
}
/*=======  funcion mapminreverse =========*/
float mapminreverse(float input)
{
    float ymax=1,ymin=-1;
    float xmin=434;										//<--------min salida
    float xmax=1001;										//<--------max salida
    static float output;
    output=((xmax-xmin)/(ymax-ymin))*(input-ymin)+xmin;
    return output;
}
/*=======  output layer =========*/
float layer2(float *input1)
{
    float static output1;
    float w0[]={-1.0119111081504676974,-0.022717177547684645755,-0.016683283469536673571,0.40900434704795740615,-0.2797596020890583679,-0.010276895019064693701,1.7076411461150402538,0.72947285518374693769,-0.038717707787515279194,-0.027961011196908611837};
    output1=DotProduct1(input1,w0)+(-1.3883333581630632469);
	return mapminreverse(output1);
}

/*=======for error MariaDB=======*/
void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

/*=======  main =========*/
int main()
{
  MYSQL *con = mysql_init(NULL);
  if (con == NULL)
  {      fprintf(stderr, "mysql_init() failed\n");
      exit(1);  }
  if (mysql_real_connect(con, "localhost", "root", "root", "temperatura", 0, NULL, 0) == NULL)
  {      finish_with_error(con);}
  if (mysql_query(con, "SELECT * FROM dsdrt ORDER BY fecha DESC LIMIT 1"))
  {      finish_with_error(con);}
  MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL)
  {      finish_with_error(con);}

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  char *token;
  row = mysql_fetch_row(result);
  printf("%s ", row[num_fields-7]);
  printf("%s ", row[num_fields-6]);
  printf("%s ", row[num_fields-5]);
  printf("%s ", row[num_fields-4]);
  printf("%s ", row[num_fields-3]);
  printf("%s ", row[num_fields-2]);
  printf("%s ", row[num_fields-1]);

  float hm= atof(row[num_fields-4]);
  float hgr= atof(row[num_fields-5]);
  int rn= atoi(row[num_fields-3]);
  float tpt= atof(row[num_fields-1]);
  float l= atof(row[num_fields-2]);
  
	float niNN[]={hm,tpt,hgr,l,rn}; //% in: 'Humedad Aire','Temperatura','Humedad_Suelo_ant','Luz','Lluvia'
	int n;
	float *GPP1=mapminmax(niNN);
	float input1[5];									//<--------------segun el numero de entradas
	float input2[10];									//<--------Segun el numero de neuronas en el hidden layer
	for(n=0;n<4;n++)									//<--------------segun el numero de entradas
	{
	    input1[n]=GPP1[n];
	}
	float *GPP2=layer1(input1);
	for(n=0;n<10;n++)									//<--------Segun el numero de neuronas en el hidden layer
	{
		input2[n]=GPP2[n];
	}
	float output=layer2(input2);
	printf("%f\n",output);
	FILE *fp;
  char buf[100];
  char buf1[100];
  char buf2[100];
  char buf3[100];
  char buf4[100];
  char buf5[100];
  gcvt(hm,6,buf);
  gcvt(hgr,6,buf1);
  gcvt(rn,6,buf2);
  gcvt(l,6,buf3);
  gcvt(tpt,6,buf4);
  gcvt(output,6,buf5);
  fp = fopen ( "fichero.txt", "wt" ); //parámetro para escritura al final y para file tipo texto
  fputs(buf,fp);
  fputs("\n",fp);
  fputs(buf1,fp);
  fputs("\n",fp); 
  fputs(buf2,fp);
  fputs("\n",fp);
  fputs(buf3,fp);
  fputs("\n",fp);
  fputs(buf4,fp);
  fputs("\n",fp);
  fputs(buf5,fp);
  fclose ( fp );
  mysql_free_result(result);
  mysql_close(con);
return 0;
}


