/*
 * Extremes.cpp
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Statistics/Extremes.h"

namespace BioFW {
	namespace Statistics {

		Extremes::Extremes() : isInited(true), min(0.0), max(0.0) {

		}
		Extremes::Extremes(double min, double max) : isInited(true), min(min), max(max) {

		}

		double Extremes::getMin() const {
			return this->min;
		}
		double Extremes::getMax() const {
			return this->max;
		}
		double Extremes::getDiff() const {
			return this->max - this->min;
		}


		void Extremes::update(double val) {
			if (!isInited) {
				isInited = true;
				min = val;
				max = val;
			}
			if (val < min) min = val;
			if (val > max) max = val;
		}

	} /* namespace Statistics */
} /* namespace BioFW */
