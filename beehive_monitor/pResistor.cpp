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
int t1 = 0, t2 = 0;
const int t_dur = 100;		// FIXME
const double var_thresh = 100;	// FIXME
volatile int new_pResistorReading = 0;
vector<int> pReadings;

double computeMean(vector<int> &v);
double computeVar(vector<int> &v, double mean);
void isBlocked();
void isNotBlocked();

double computeMean(vector<int> &v) {
	double sum = 0.0;
	double mean = 0.0;
	int n = v.size();
	for (int i = 0; i < n; i++)
		sum += v[i];
	return mean = sum / n;
}

double computeVar(vector<int> &v, double mean) {
	double sum = 0.0;
	double temp = 0.0;
	double var = 0.0;
	int n = v.size();
	for (int i = 0; i < n; i++) {
		temp = pow((v[i] - mean), 2);
		sum += temp;
	}
	return var = sum / n;
}

int readPhotoResistors(const int pResistor_pin) {
	return analogRead(pResistor_pin);
}

void isBlocked() {
	t1 = now();
	detachInterrupt(pResistor1);
	detachInterrupt(pResistor2);
	for (int i = 0; i < 10; ++i) {
		pReadings.push_back(readPhotoResistors(pResistor1));
		pReadings.push_back(readPhotoResistors(pResistor2));
		delay(200);
	}
	if (computeVar(pReadings) < var_thresh) {	// fake info
		new_pResistorReading = (readPhotoResistors(pResistor1) + readPhotoResistors(pResistor1)) / 2;
	}
	attachInterrupt(pResistor1, isNotBlocked, RISING);
	attachInterrupt(pResistor1, isBlocked, FALLING);
}

void isNotBlocked() {
	detachInterrupt(pResistor1);
	detachInterrupt(pResistor2);
	for (int i = 0; i < 10; ++i) {
		pReadings.push_back(readPhotoResistors(pResistor1));
		pReadings.push_back(readPhotoResistors(pResistor2));
		delay(500);
	}
	if (computeVar(pReadings) < var_thresh) {	// end high traffic
		t2 = now();
		if (t2 - t1 > t_dur) {
			// TODO: send signal
		}
	}
	attachInterrupt(pResistor1, isNotBlocked, RISING);
	attachInterrupt(pResistor1, isBlocked, FALLING);
}
