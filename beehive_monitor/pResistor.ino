/*
*
* pResistor.ino
* Description: this page analyze the data from photoresistors
* Created on Apr 26, 2018
*
* bigger adc reading, more blocked
-------------------------------------
 			R_ref		PhotoResistors
+3.3    o---/\/\/---.---/\/\/---o GND
 					|
Pin 	o-----------.
-------------------------------------
*/
//****************** structs ********************
struct PhotoResistors {
	const int pin;	// photoresistors pin
	double vol;
	double res;
	double res_log;
};

struct PhotoResistors pResistor[pResistor_UNIT_NUM] = {
	{31, 0.0, 0.0, 0.0},
	{32, 0.0, 0.0, 0.0},
	{33, 0.0, 0.0, 0.0},
};

struct PhotoResistors prev_pResistor[pResistor_UNIT_NUM] = {
	{31, 0.0, 0.0, 0.0},
	{32, 0.0, 0.0, 0.0},
	{33, 0.0, 0.0, 0.0},
};

//***************** Variables *******************
double res_sum = 0.0;
double diff_sum = 0.0;
const double R_ref = 100000;	// FIXME
const double var_thresh = 100;	// FIXME

//***************** Functions *******************
int computeTraffic(void) {
	return (int)res_sum % 5;
}

double vol2Res(double vol) {
	return (vol * R_ref) / (3.3 - vol);
}

double computeMean(void) {
	double sum = 0.0;
	double mean = 0.0;
	for (int i = 0; i < pResistor_UNIT_NUM; i++)
		sum += pResistor[i].res;
	return mean = sum / pResistor_UNIT_NUM;
}

double computeVar(double mean) {
	double sum = 0.0;
	double temp = 0.0;
	double var = 0.0;
	for (int i = 0; i < pResistor_UNIT_NUM; i++) {
		temp = pow((pResistor[i].res - mean), 2);
		sum += temp;
	}
	return var = sum / pResistor_UNIT_NUM;
}

double computeDiffAbs(double a, double b) {
	return abs(a - b);
}

void updateReadings() {
	int value = 0;
	for (int i = 0; i < pResistor_UNIT_NUM; i++) {
		value = adc->adc0->analogRead(pResistor[i].pin); // read a new value, will return ADC_ERROR_VALUE if the comparison is false
		pResistor[i].vol = value * 3.3 / adc->getMaxValue(ADC_0);
		pResistor[i].res = vol2Res(pResistor[i].vol);
		pResistor[i].res_log = log10(pResistor[i].res);
		Serial.print(i);
		Serial.print(" th res_log = ");
		Serial.println(pResistor[i].res_log);
		// TODO: how to use diff_sum?
		diff_sum += computeDiffAbs(prev_pResistor[i].res, prev_pResistor[i].res);
		prev_pResistor[i].vol = pResistor[i].vol;
		prev_pResistor[i].res = pResistor[i].res;
		prev_pResistor[i].res_log = pResistor[i].res_log;
		res_sum += pResistor[i].res_log;
	}
}

void judgeTraffic(void) {
	double mean = computeMean();
	double var = computeVar(mean);
	if (var > var_thresh) {
		// TODO: calculate possibility
		// highly possible that high traffic
	}
}

void pResistorMonitor(void) {
	delay(1000);
	updateReadings();
	int traffic = computeTraffic();
	// TODO: send data
	// traffic, diff_value, resistence, possibility,
}
