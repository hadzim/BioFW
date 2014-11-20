/*
 * Extremes.h
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#ifndef EXTREMES_H_
#define EXTREMES_H_

namespace BioFW {
	namespace Statistics {
		class Extremes {
			public:
				Extremes();
				Extremes(double min, double max);

				double getMin() const;
				double getMax() const;
				double getDiff() const;

				void update(double val);
			private:
				bool isInited;
				double min;
				double max;

		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* EXTREMES_H_ */
