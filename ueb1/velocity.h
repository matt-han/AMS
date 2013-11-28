//Gruppe 2-8
//Matthias Hansert 791744
//Max Knodel 798479
//Niklas Knauer 798163

#ifndef VELOCITY_H
#define VELOCITY_H

/*
 * Die Funktion fährt den Roboter abhängig von Übergabeparametern entweder
 * gerade aus oder in einem Kreis, abhängig von Omega.
 *
 * Parameter:	float vBahn:		Bahngeschwindigkeit m/s
 *						float omega:		Winkelgeschwindigkeit 2pi/s
 *						int zeit:				Fahrzeit in Millisekunden
 *						VMC::CVmc *vmc:	Zeiger auf CVmc-Objekt
 * 
*/
void velocity(float vBahn, float omega, int zeit, VMC::CVmc *vmc);

#endif
