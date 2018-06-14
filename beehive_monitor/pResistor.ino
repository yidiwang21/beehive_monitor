/*
*
* pResistor.ino
* Description: this page analyze the data from photoresistors
* Created on Apr 27, 2018
*
* bigger adc reading, more blocked
-------------------------------------
 			R_ref		PhotoResistors
+3.3    o---/\/\/---.---/\/\/---o GND
 					|
Pin 	o-----------.
-------------------------------------
*/

//***************** Variables *******************
double res_sum = 0.0;
double res_sum_log = 0.0;
double diff_sum = 0.0;
double baseline = 0.0;
const double R_ref = 1000000;	// FIXME: 100k for testing, need precise value
const double var_thresh = 100;	// FIXME

//***************** Functions *******************
double getBaseline(void) {
	int n = 3600;
	for (int i = 0; i < n; ++i) {
		delay(1000);
		baseline += computeMean();
	}
	baseline /= n;
	return baseline;
}

double computeTraffic(void) {
	if (res_sum_log - 5 < 0)
		return 0;
	else
		return res_sum_log - 5;
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

void updateReadings(void) {
	double value = 0.0;
	res_sum = 0.0;
	res_sum_log = 0.0;
	diff_sum = 0.0;
	for (int i = 0; i < pResistor_UNIT_NUM; i++) {
		prev_pResistor[i].vol = pResistor[i].vol;
		prev_pResistor[i].res = pResistor[i].res;
		prev_pResistor[i].res_log = pResistor[i].res_log;
		value = adc->analogRead(pResistor[i].pin, ADC_1); // read a new value, will return ADC_ERROR_VALUE if the comparison is false
		pResistor[i].vol = value * 3.3 / adc->getMaxValue(ADC_1);
		pResistor[i].res = vol2Res(pResistor[i].vol);
		pResistor[i].res_log = log10(pResistor[i].res);
		Serial.print(i);
		Serial.print(" th res_log = ");
		Serial.println(pResistor[i].res_log);
		Serial.print(i);
		Serial.print(" th res = ");
		Serial.println(pResistor[i].res);
		Serial.print(i);
		Serial.print(" th vol = ");
		Serial.println(pResistor[i].vol);
		// TODO: how to use diff_sum?
		res_sum += pResistor[i].res;
		diff_sum += computeDiffAbs(pResistor[i].res, prev_pResistor[i].res);
		res_sum_log += pResistor[i].res_log;
	}
}

void judgeTraffic(void) {
	double mean = computeMean();
	double var = computeVar(mean);
	if (var > var_thresh) {
		// TODO: calculate possibility
		// highly possible at high traffic
	}
}

void pResistorMonitor(void) {
	updateReadings();
	sendData();
	printData();
}

void sendData(void) {
	// sending data: traffic, diff_value, resistence, possibility,
	Bluetooth.println("traffic: ");
	Bluetooth.println(computeTraffic());
	Bluetooth.println("resistance: ");
	Bluetooth.println(res_sum);
	Bluetooth.println("resistance difference: ");
	Bluetooth.println(diff_sum);
	flag = 1;
}

void printData() {
	Serial.print("traffic: ");
	Serial.println(computeTraffic());
	Serial.print("resistance: ");
	Serial.println(res_sum);
	Serial.print("resistance difference: ");
	Serial.println(diff_sum);
	Serial.println("============================================");
}
