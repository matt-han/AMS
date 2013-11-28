//Gruppe 2-8
//Matthias Hansert 791744
//Max Knodel 798479
//Niklas Knauer 798163

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <vmc232.h>
#include <sys/time.h>
#include "robi.h"
#include "velocity.h"

int main(int argc, char **argv)
{
	float vBahn, omega;
	int zeit;
  VMC::CVmc    vmc(CNTRL_DEV);

  if(!vmc.isConnected())
  {
     printf ("Error connecting to VMC\n");
     return -1;
  }

	if(argc != 4)
	{
		printf("Zu wenig Parameter...\nUsage: %s vBahn(m/s) Omega(2Pi/s) Fahrzeit(ms)\n", argv[0]);
		return -1;
	}

	vBahn = atof(argv[1]);
	omega = atof(argv[2]);
	zeit = atoi(argv[3]);

  printf("Angegebene Bahnwerte:\nvBahn: %4.4f m/s\nOmega: %4.4f 2Pi/s\nZeit :  %u ms\n",
					vBahn, omega, zeit); 

  vmc.setMaximumRPM(MAX_RPM);
  vmc.setCycleTime(30);
  vmc.setSpeedFactor(MOTOR_ID_LEFT,SPEED_FACTOR_LEFT);
  vmc.setSpeedFactor(MOTOR_ID_RIGHT,SPEED_FACTOR_RIGHT);
  vmc.clearResponseList();
  vmc.addStateToResponseList(vmc.MOTOR_TICKS_ABSOLUTE);

  vmc.setMotorLinearPart(MOTOR_ID_LEFT,		PL);
  vmc.setMotorLinearPart(MOTOR_ID_RIGHT,	PR);
  vmc.setMotorIntegralPart(MOTOR_ID_LEFT,	IL);
  vmc.setMotorIntegralPart(MOTOR_ID_RIGHT, IL);

	vmc.setWheelRadius(MOTOR_ID_LEFT, RAD_DURCHMESSER/2);
  vmc.setWheelRadius(MOTOR_ID_RIGHT, RAD_DURCHMESSER/2);
  vmc.setWheelDistance(RAD_ABSTAND);

  vmc.setTicksPerTurn(MOTOR_ID_LEFT, ENCODER_COUNT);
  vmc.setTicksPerTurn(MOTOR_ID_RIGHT, ENCODER_COUNT);

  vmc.resetMotorTicks();
  vmc.wait(1000);

  velocity(vBahn, omega, zeit, &vmc); // Fährt 1 Meter geradeaus, hoffentlich
  
  return 0;
}
