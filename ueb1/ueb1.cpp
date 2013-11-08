#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <vmc232.h>
#include <sys/time.h>  // NEW
#include "robi.h"

double time_shot()    // NEW
{

   struct timeval tv;                       // Sys clock struct

   gettimeofday(&tv, NULL);                 // Systemzeit abrufen
   return ((double)((int)tv.tv_sec)+
          (double)tv.tv_usec/1000000.0);    // Zeit in Sekunden
}


velo velocity(float vBahn, float omega)
{
  float b = 34.36 ;
  float vl, vr;
  float radius;
  
  velo ret;
  
  ret.l = vBahn - omega*(b/2);
  ret.r = vBahn + omega*(b/2);
  
  return ret;  
}


void drive(float vBahn, float omega, int zeit, VMC::CVmc *vmc)
{
  int schritte = 0;
  velo velos;
  float rampe = vBahn/100;
  float sb = 0;
  float beschleunigung = 0;
  float gStrecke = 0;
  float zeit2 = 0;
  
  
  
  sb = pow(vBahn,2) / (2 * vBahn / 0.5);
  printf("sb: %f\n", sb);
  
  gStrecke = vBahn * zeit;
  printf("gStrecke: %f\n", gStrecke);
  gStrecke -= 2 * sb;
  printf("gStrecke: %f\n", gStrecke);
  
  
  
  zeit2 = gStrecke / vBahn;
  printf("zeit2: %f\n", zeit2);
  
  schritte = (zeit2 * 1000) / TEILSCHRITTE;
  printf("schritte: %f\n", schritte);
  
  // Sanft losfahren
  for (float i = rampe; i <= vBahn; i += rampe)
  {
    velos = velocity(i, omega);
    printf("l: %f, lk: %i\n", velos.l, umrechne(velos.l));
    vmc->setMotors( umrechne(velos.l), umrechne(velos.r) );
    vmc->wait(5);
  }
  
  printf("---------------------------\n");
  
  for(int i = 0; i <= schritte; i++)
  {
    velos = velocity(vBahn, omega);
    
    printf("l: %f, lk: %i\n", velos.l, umrechne(velos.l));
    
    vmc->setMotors(umrechne(velos.l), umrechne(velos.r));
    vmc->wait(TEILSCHRITTE);    
  }
  
   printf("---------------------------\n");
   
  // Sanft bremsen
  for (float i = vBahn; i >= 0; i -= rampe)
  {
    velos = velocity(i, omega);
    printf("l: %f, lk: %i\n", velos.l, umrechne(velos.l));
    vmc->setMotors( umrechne(velos.l), umrechne(velos.r) );
    vmc->wait(50);
  }
}

/**
  return float, Motergeschwindigkeit in Prozent
*/
int umrechne(float v)
{
  float tmp;
  //Radumfang
  //float umfang = RAD_DURCHMESSER * M_PI / 100;
  //getriebeumrechung
  tmp = v * 14;
  tmp = tmp * 0.207; //umfang // Wie 207???
  //in prozent von MAX_RPM
  tmp = (3000 * tmp) / 100;
  return (int)tmp;
  
  
}



int main(int argc, char **argv)
{
  
  int i;
  double startTime;         // Zeitstempel am Beginn der Messung (zum Subtrahieren)
  double times_l[MAX_MEAS]; // Mess-Zeitpunkte fuer linkes Rad
  double times_r[MAX_MEAS]; // Mess-Zeitpunkte fuer rechtes Rad
  double enc_l;             // Abgeholte Encoderticks linkes Rad
  double enc_r;             // Abgeholte Encoderticks rechtes Rad
  int    encs_l[MAX_MEAS];  // Encoder-Messwerte linkes Rad
  int    encs_r[MAX_MEAS];  // Encoder-Messwerte rechtes Rad
  int    pf;                // Plotfile
  char   filename[50];      // Dateiname
  char   buffer[80];        // line buffer
  double timestamp_l;       // Zeitstempel fuer Encoderticks Links
  double timestamp_r;       // Zeitstempel fuer Encoderticks Rechts
  double old_timestamp_l;   // Vorherg. Zeitstempel fuer Encoderticks Links
  double old_timestamp_r;   // Vorherg. Zeitstempel fuer Encoderticks Rechts

  VMC::CVmc    vmc("/dev/ttyS0");

  if(!vmc.isConnected())
  {
     printf ("Error connecting to VMC\n");
     return 1;
  }

  printf("------------------------------------------------------------\n"); 

  vmc.setMaximumRPM(MAX_RPM);
  vmc.setCycleTime(30);
  vmc.setSpeedFactor(1,1);
  vmc.setSpeedFactor(2,1);
  vmc.clearResponseList();
  vmc.addStateToResponseList(vmc.MOTOR_TICKS_ABSOLUTE);

  vmc.setMotorLinearPart(MOTOR_ID_LEFT,		pL);
  vmc.setMotorLinearPart(MOTOR_ID_RIGHT,	pR);
  vmc.setMotorIntegralPart(MOTOR_ID_LEFT,	iL);
  vmc.setMotorIntegralPart(MOTOR_ID_RIGHT,	iL);

  vmc.resetMotorTicks();
  vmc.wait(1000);
  vmc.getMotorState(MOTOR_ID_LEFT,vmc.MOTOR_TICKS_ABSOLUTE,NULL,&startTime);

  /*// Sanft losfahren
  for (i=0;i<=20;i++)
  {
    vmc.wait(50);
    vmc.setMotors(i, i);
  }*/
  
  //vl and vr
  //vmc.setMotors(20, 20);
  //float vBahn, float omega, int zeit, VMC *vmc
  drive(0.2, 0, 10, &vmc); // Fährt 1 Meter geradeaus, hoffentlich
  
  //vmc.wait(1000);
  
  //drive(20, 1.5, 10, &vmc); //punktdrehung
  //drive(20, 0.7853, 10, &vmc); // 45 grad
  
  
 /*  
  for(i=0; i<MAX_MEAS; i++)
  {
    printf("ts: %0.3f lR: %5d rR: %d \n", times_l[i], encs_l[i], encs_r[i]);
  }  

 // Sanft abbremsen
  for (i=20;i>=0;i--)
  {
    vmc.wait(50);
    vmc.setMotors(i, i);
  }
 
  vmc.setMotorLinearPart(MOTOR_ID_LEFT,100);
  vmc.setMotorLinearPart(MOTOR_ID_RIGHT,100);
  vmc.setMotorIntegralPart(MOTOR_ID_LEFT,12000);
  vmc.setMotorIntegralPart(MOTOR_ID_RIGHT,12000);
  vmc.wait(1000);
  printf("Zurueckfahren\n");
  vmc.setMotors(-20, -20);
  vmc.wait(5000);
  printf("Einlenken\n");
  vmc.setMotors(-10, -10);
  vmc.wait(1000);
  vmc.setMotors(-10, 0);
  vmc.wait(500);
  vmc.setMotors(0, -10);
  vmc.wait(500);
  printf("Vorfahren\n");
  vmc.setMotors(10, 0);
  vmc.wait(500);
  vmc.setMotors(0, 10);
  vmc.wait(1000);
  vmc.setMotors(10, 0);
  vmc.wait(500);
  vmc.setMotors(5, 5);
  vmc.wait(2000);
  printf("Anhalten\n");
  vmc.setMotors(0, 0);
  vmc.wait(1000);
  */
  
  return 0;
}
