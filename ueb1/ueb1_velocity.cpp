#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <vmc232.h>
#include "robi.h"
#include <sys/time.h>  // NEW

double time_shot();  // NEW
void velocity(float vBahn, float omega);
float calculateRadius(float distance, float vl, float vr);
#define MAX_RPM 6000

// USER

#define pL 1
#define pR 1
#define iL 1
#define iR 1

void velocity(float vBahn, float omega)
{
  float b = 34.36 ;
  float vl, vr;
  float radius;
  
  vl = vBahn - omega*(b/2);
  vr = vBahn + omega*(b/2);
  
  vmc.setMotors(vl, vr);
  
   //radius = calculateRadius(b, vl, vr);
  
}

/**float calculateRadius(float distance, float vl, float vr)
{
   
  return distance*((vr+vl)/(vr-vl));
  
}**/




double time_shot()    // NEW
{
   struct timeval tv;                       // Sys clock struct

   gettimeofday(&tv, NULL);                 // Systemzeit abrufen
   return ((double)((int)tv.tv_sec)+
          (double)tv.tv_usec/1000000.0);    // Zeit in Sekunden
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
  int    i_p;               // Zaehlschleifenindex fuer P-Anteil
  int    i_i;               // Zaehlschleifenindex fuer I-Anteil
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
  
  vmc.setMaximumRPM(MAX_RPM);
  vmc.setCycleTime(30);
  vmc.setSpeedFactor(1,1);
  vmc.setSpeedFactor(2,1);
  vmc.clearResponseList();
  vmc.addStateToResponseList(vmc.MOTOR_TICKS_ABSOLUTE);

  //parameter for the pi-regulator
  vmc.setMotorLinearPart(MOTOR_ID_LEFT,pL);
  vmc.setMotorLinearPart(MOTOR_ID_RIGHT,pR);
  vmc.setMotorIntegralPart(MOTOR_ID_LEFT,iL);
  vmc.setMotorIntegralPart(MOTOR_ID_RIGHT,iR);
  

  
  vmc.resetMotorTicks();
  vmc.wait(1000);
  vmc.getMotorState(MOTOR_ID_LEFT,vmc.MOTOR_TICKS_ABSOLUTE,NULL,&startTime);

  //USER
  velocity(vBahn, omega);
  //*******
  
  old_timestamp_l=-1;
  old_timestamp_r=-1;
  
  for (i=20;i>=0;i--)
  {
    // Sanft abbremsen
    vmc.wait(50);
    vmc.setMotors(i, i);
  }

}