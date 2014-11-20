/*
 * Training.h
 *
 *  Created on: 10.3.2014
 *      Author: JV
 */

#ifndef TRAINING_H_
#define TRAINING_H_

#include "BioFW/Database/BiometricID.h"
#include "BioFW/Database/Record.h"
#include <map>


namespace BioFW {

	typedef Record::List Labels;

	class Labels2Int {
		public:
			Labels2Int(const Labels & lbls){
				int number = 1;
				for (auto l : lbls){
					if (labels.find(l.getBiometricID()) == labels.end()){
						labels[l.getBiometricID()] = number++;
					}
				}
			}

			int index(const Record & r){
				return labels[r.getBiometricID()];
			}
		private:
			std::map<BiometricID, int> labels;
	};

}  // namespace BioFW

#endif /* TRAINING_H_ */
