/*
 * BlockEvaluator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef BLOCKTRAINING_H_
#define BLOCKTRAINING_H_

#include <vector>
#include <Poco/NumberFormatter.h>

#include "BioFW/Evaluation/IBlockEvaluator.h"
#include "BioFW/Block/IComparisonBlock.h"
#include "BioFW/BiometricSample/IBiometricSampleLoader.h"
#include "BioFW/Template/IPersistentTemplateCreator.h"
#include "BioFW/Template/MemoryPersistentTemplate.h"
#include "BioFW/Evaluation/IBlockTraining.h"

#include <iostream>
#include "BioFW/Block/ITemplateExtractingBlock.h"
#include "BioFW/Database/Database.h"

namespace BioFW {

	template<class TRecord, class TBiometricSample>
	class BlockTraining: public IBlockTraining<TRecord> {

		public:

			typedef typename IProcessingBlock<TBiometricSample>::Ptr BlockPtr;
			typedef std::vector<BlockPtr> Blocks;
			typedef std::vector <TBiometricSample> Samples;

			typedef IBiometricSampleLoader<TRecord, TBiometricSample> SampleLoader;
			typedef typename SampleLoader::Ptr SampleLoaderPtr;

			BlockTraining(SampleLoaderPtr sampleLoader
			) :
					sampleLoader(sampleLoader){

			}

			virtual ~BlockTraining() {

			}

			void addBlock(BlockPtr block) {
				this->blocks.push_back(block);
			}

			typedef typename TRecord::List Records;
			typedef typename Records::const_iterator RecordsCIt;

			/// extract tempaltes
			/// get list of database records and create templates from each of them
			virtual void train(const typename TRecord::List & inputDatabase) {
				int errors = 0;

				this->reportProgress(ProgressReport("Template extraction started"));

				int total = inputDatabase.size();
				int act = 0;

				Samples samples;

				Labels labels;


				for (RecordsCIt r = inputDatabase.begin(); r != inputDatabase.end(); r++) {
					act++;
					std::string message = "Sample loading ";
					message += ".";
					this->reportProgress(ProgressReport(message, act, total));

					//create input data
					TBiometricSample inputData = this->sampleLoader->createBiometricSample(*r);
					samples.push_back(inputData);
					labels.push_back(Record(r->getSampleID(), r->getBiometricID()));
					message += ".";
				}


				act = 0;
				int blockCnt = blocks.size();
				for (typename Blocks::iterator block = blocks.begin(); block != blocks.end(); block++) {

						try {

							act++;
							std::string message = "Training " + (*block)->getName();
							message += ".";
							this->reportProgress(ProgressReport(message, act, blockCnt));

							(*block)->train(samples, labels);

							message += ".";
							this->reportProgress(ProgressReport(message, act, blockCnt));


						} catch (...) {
							//TODO
							errors++;
						}
				}
			}

			void reportProgress(const ProgressReport & report) {
				ProgressReport r = report;
				this->ProgressChanged.notify(this, r);
			}
		private:

			Blocks blocks;
			SampleLoaderPtr sampleLoader;

	};

} /* namespace BioFW */
#endif /* BLOCKEVALUATOR_H_ */
