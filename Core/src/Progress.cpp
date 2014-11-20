#include "BioFW/Progress.h"
/*
 * Progress.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include "BioFW/Progress.h"

namespace BioFW {

	ProgressReport::ProgressReport(Message m, Progress p) : message(m), progress(p) {
	}
	ProgressReport::ProgressReport(Message m, Progress item, Progress total) : message(m), progress(item * 100 / total) {

	}

	ProgressReport::Progress ProgressReport::getProgress() const {
		return progress;
	}
	ProgressReport::Message ProgressReport::getMessage() const {
		return message;
	}

}  // namespace BioFW
