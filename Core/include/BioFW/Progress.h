/*
 * IProgressReporter.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IPROGRESSREPORTER_H_
#define IPROGRESSREPORTER_H_

#include <string>
#include <Poco/BasicEvent.h>

namespace BioFW {

	class ProgressReport {
		public:
			typedef int Progress;
			typedef std::string Message;

			ProgressReport(Message m, Progress p = 0);
			ProgressReport(Message m,  Progress item, Progress total);

			Progress getProgress() const;
			Message getMessage() const;


		private:
			Message message;
			Progress progress;

	};

	class IProgressReporter {
		public:
			Poco::BasicEvent <ProgressReport> ProgressChanged;
	};

} /* namespace BioFW */
#endif /* IPROGRESSREPORTER_H_ */
