/*
 * MedianOmatic.h
 *
 *  Created on: May 4, 2012
 *      Author: fraenkle
 */

#ifndef MEDIANOMATIC_H_
#define MEDIANOMATIC_H_

#include <vector>

class MedianOmatic {
public:
	MedianOmatic();
	void Clear();
	void AddBinLimit(double binLimit);
	void AddValues(double xvalue, double yvalue);
	double GetUpperMedian(double binLimit);
	double GetLowerMedian(double binLimit);
	virtual ~MedianOmatic();
private:
	std::vector<double> binLimits;
	std::vector<double> x, y;
	double CalculateMedian(int indexLowerLimit, int indexUpperLimit);
};

#endif /* MEDIANOMATIC_H_ */
