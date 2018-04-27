/*
*
* pResistor.cpp
* Description: this page analyze the data gotten from photoresistors
* Created on Apr 26, 2018
*
-------------------------------------
 		PhotoResistors	100K
+3.3    o---/\/\/---.---/\/\/---o GND
 					|
Pin 	o-----------.
-------------------------------------
*/

int readPhotoResistors(const int pResistor_pin) {
	return analogRead(pResistor_pin);
}

// TODO: monitoring changing resistance in a period of time
void monitorResistence() {
	//
}
