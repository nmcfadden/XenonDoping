/*
 * MedianOmatic.cxx
 *
 *  Created on: May 4, 2012
 *      Author: fraenkle
 */

#include "MedianOmatic.h"

MedianOmatic::MedianOmatic() {

}

void MedianOmatic::Clear() {

	binLimits.clear();
	x.clear();
	y.clear();
}

void MedianOmatic::AddBinLimit(double binLimit) {

	binLimits.push_back(binLimit);
}

void MedianOmatic::AddValues(double xvalue, double yvalue) {

	x.push_back(xvalue);
	y.push_back(yvalue);
}

double MedianOmatic::GetLowerMedian(double binLimit) {

	int indexBinLimit = 0;
	int indexLowerLimit = 0;
	int indexUpperLimit = 0;
	double upperLimit, lowerLimit;

	while (binLimit != binLimits[indexBinLimit] && indexBinLimit < binLimits.size()) {
		indexBinLimit++;
	}

	upperLimit = binLimits[indexBinLimit];
	if (indexBinLimit > 0) {
		lowerLimit = binLimits[indexBinLimit - 1];
	} else {
		return 0.0;
	}

	while (x[indexLowerLimit] < lowerLimit && indexLowerLimit < x.size()) {
		indexLowerLimit++;
	}
	indexUpperLimit = indexLowerLimit;
	while (x[indexUpperLimit] < upperLimit && indexUpperLimit < x.size()) {
		indexUpperLimit++;
	}

	return CalculateMedian(indexLowerLimit, indexUpperLimit);
}

double MedianOmatic::GetUpperMedian(double binLimit) {

	int indexBinLimit = 0;
	int indexLowerLimit = 0;
	int indexUpperLimit = 0;
	double upperLimit, lowerLimit;

	while (binLimit != binLimits[indexBinLimit] && indexBinLimit < binLimits.size()) {
		indexBinLimit++;
	}

	lowerLimit = binLimits[indexBinLimit];
	if (indexBinLimit + 1 < binLimits.size()) {
		upperLimit = binLimits[indexBinLimit + 1];
	} else {
		return 0.0;
	}

	while (x[indexLowerLimit] < lowerLimit && indexLowerLimit < x.size()) {
		indexLowerLimit++;
	}
	indexUpperLimit = indexLowerLimit;
	while (x[indexUpperLimit] < upperLimit && indexUpperLimit < x.size()) {
		indexUpperLimit++;
	}

	return CalculateMedian(indexLowerLimit, indexUpperLimit);
}

double MedianOmatic::CalculateMedian(int indexLowerLimit, int indexUpperLimit) {

	double median = 0;
	double normFactor = x[indexUpperLimit] - x[indexLowerLimit];

	for (int i = indexLowerLimit; i < indexUpperLimit; i++) {
		median += (x[i + 1] - x[i]) / normFactor * y[i];
	}

	return median;
}

MedianOmatic::~MedianOmatic() {

	Clear();
}

