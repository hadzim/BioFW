/*
 * ComparisonScore.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef COMPARISONSCORE_H_
#define COMPARISONSCORE_H_
#include <map>
#include <string>
#include <vector>

namespace BioFW {

	class ComparisonScore {

		public:

			typedef double Score;
			typedef std::map <std::string, Score> Subscores;
			typedef std::vector <ComparisonScore> List;


			ComparisonScore(Score score);
			ComparisonScore(Score score, Subscores subscores);

			Score getScore() const;

			bool isValid() const;

			const Subscores & getSubscores() const;

			static ComparisonScore invalid();


		private:
			Score score;
			Subscores subscores;
	};

} /* namespace BioFW */
#endif /* COMPARISONSCORE_H_ */
