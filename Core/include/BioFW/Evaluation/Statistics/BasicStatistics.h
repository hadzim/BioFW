/*
 * BasicStatistics.h
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#ifndef BASICSTATISTICS_H_
#define BASICSTATISTICS_H_
#include "BioFW/Evaluation/MethodResults.h"
#include "Extremes.h"

namespace BioFW {
	namespace Statistics {

		struct Averager {
		   double currentAverage;
		   size_t count;

		   Averager() : currentAverage(0), count(0){

		   }

		   void addData (double value) {
			   this->currentAverage += (value - this->currentAverage) / ++count;
		   }
		};

		class BasicStatistics {

			public:
				BasicStatistics(const MethodResults & results);

				const Extremes & getGlobalExtremes() const;
				const Extremes & getGenuineExtremes() const;
				const Extremes & getImpostorExtremes() const;

				double getGenuineMean() const;
				double getImpostorMean() const;
				double getGlobalMean() const;

				int getGenuineCount() const;
				int getImpostorCount() const;
				int getTotalCount() const;

				const MethodResults & getMethodResults() const;

				void refresh();
				void clear();
			protected:
				void addScoreToStatistics(const ComparisonScore & ms, bool isGenuine);

			private:
				Extremes globalExtremes;

				Extremes genuineExtremes;
				Extremes impostorExtremes;

				MethodResults methodResults;

				Averager genuineAvg;
				Averager impostorAvg;
				Averager totalAvg;
		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* BASICSTATISTICS_H_ */
