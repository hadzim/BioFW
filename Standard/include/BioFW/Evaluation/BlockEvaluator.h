/*
 * BlockEvaluator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef BLOCKEVALUATOR_H_
#define BLOCKEVALUATOR_H_

#include <vector>
#include <Poco/NumberFormatter.h>

#include "BioFW/Evaluation/IBlockEvaluator.h"
#include "BioFW/Block/IComparisonBlock.h"
#include "BioFW/BiometricSample/IBiometricSampleLoader.h"
#include "BioFW/Template/IPersistentTemplateCreator.h"
#include "BioFW/Template/MemoryPersistentTemplate.h"

#include <iostream>
#include "BioFW/Database/Subset/ReferenceAndProbeDatabaseSubsetCreator.h"
#include "BioFW/Evaluation/MethodResults.h"
#include "BioFW/Utils/Database.h"

#include <Poco/Random.h>

namespace BioFW {

	template<class TRecord, class TBiometricSample>
	class BlockEvaluator: public IBlockEvaluator<TRecord> {

		public:

			typedef typename IComparisonBlock<TBiometricSample>::Ptr BlockPtr;
			typedef std::vector<BlockPtr> Blocks;

			typedef IBiometricSampleLoader<TRecord, TBiometricSample> SampleLoader;
			typedef typename SampleLoader::Ptr SampleLoaderPtr;

			BlockEvaluator(SampleLoaderPtr sampleLoader
			) :
					sampleLoader(sampleLoader){

			}

			virtual ~BlockEvaluator() {

			}

			void addBlock(BlockPtr block) {
				this->blocks.push_back(block);
			}

			typedef typename TRecord::List Records;
			typedef typename Records::const_iterator RecordsCIt;

			/// extract tempaltes
			/// get list of database records and create templates from each of them
			virtual void extractTemplates(const typename TRecord::List & inputDatabase, BiometricReferenceDatabase & referenceDatabase) {
				int errors = 0;

				this->reportProgress(ProgressReport("Template extraction started"));

				int total = inputDatabase.size();
				int act = 0;
				for (RecordsCIt r = inputDatabase.begin(); r != inputDatabase.end(); r++) {
					act++;

					std::string message = "Template extraction ";
					message += ".";
					this->reportProgress(ProgressReport(message, act, total));

					//create input data
					TBiometricSample inputData = this->sampleLoader->createBiometricSample(*r);

					message += ".";
					this->reportProgress(ProgressReport(message, act, total));

					MemoryPersistentTemplate::Ptr mt(new MemoryPersistentTemplate());
					bool passed = false;
					int cnt = 0;
					for (typename Blocks::iterator block = blocks.begin(); block != blocks.end(); block++) {

						try {
							IPersistentTemplate::Ptr persistentTemplate = (*block)->extract(inputData);
							mt->addSubtemplate((*block)->getName(), persistentTemplate);

							if ((cnt++ % 5) == 0){
								message += ".";
								this->reportProgress(ProgressReport(message, act, total));
							}

							passed = true;
						} catch (...) {
							//TODO
							errors++;
						}
					}

					if (passed) {
						//store template
						referenceDatabase.addRecord(BiometricReferenceDataRecord(r->getSampleID(), r->getBiometricID(), mt));
					} else {

					}
					message += ".";

					message += " errors: " + Poco::NumberFormatter::format(errors);

					this->reportProgress(ProgressReport(message, act, total));
				}
			}

			/// evaluate records - compare each template against each input and save results
			virtual void evaluateRecords(const typename TRecord::List & probeDatabase, const BiometricReferenceDatabase & referenceDatabase, Results & results) {
				if (this->blocks.size() == 0) {
					throw BioFWException("No block to evaluate");
				}

				this->reportProgress(ProgressReport("Records evaluation started"));

				std::map<std::string, std::map<int, BiometricReferenceDataRecord> > cache;

				for (BlockPtr & block : this->blocks) {

					block->resetTemplates();

					for (auto & templateRecord : referenceDatabase.getCollections().getRecords()) {
						size_t index = block->pushTemplate(templateRecord.getPersistentTemplate()->getSubtemplate(block->getName()));
						cache[block->getName()].insert(std::make_pair(index, templateRecord));
					}
				}

				int errors = 0;
				int total = probeDatabase.size() * blocks.size();
				int act = 0;

				Poco::Random rnd;

				for (const TRecord & probeRecord : probeDatabase) {

					try {
						std::string message = "Records evaluation ";
						message += ".";
						this->reportProgress(ProgressReport(message, act, total));

						//create input data
						TBiometricSample probeInputData = this->sampleLoader->createBiometricSample(probeRecord);

						message += ".";
						this->reportProgress(ProgressReport(message, act, total));

						for (BlockPtr & block : this->blocks) {
							act++;

							block->setInputData(probeInputData);

							for (auto & referenceTemplate : cache[block->getName()]) {
								try {

									Result result(Record(referenceTemplate.second), Record(probeRecord));

									//TO reduce amount of impostor comparisons
									if (!result.isGenuine() && (rnd.next() % 8 != 0)){
										//std::cout << "skip" << std::endl;
										//continue;
									}
									//std::cout << "noskip" << std::endl;
									//compare
									ComparisonScore comparisonScore = block->computeComparisonScore(referenceTemplate.first);

									//store result

									result.setComparisonScore(block->getName(), comparisonScore);
									results.addResult(result);
								} catch (Poco::Exception & exc) {
									errors++;
								} catch (std::exception & exc) {
									errors++;
								}
							}

							message += ".";
							this->reportProgress(ProgressReport(message, act, total));

						}

					} catch (...) {
						errors++;
					}

				}
				this->reportProgress(ProgressReport("Done -> errors: " + Poco::NumberFormatter::format(errors), 100));
			}

			void reportProgress(const ProgressReport & report) {
				ProgressReport r = report;
				this->ProgressChanged.notify(this, r);
			}



		private:

			Blocks blocks;
			SampleLoaderPtr sampleLoader;

	};

	template <class TBiometricSample>
	class SampleRecord : public Record {
		public:
			typedef std::vector <SampleRecord> List;
			SampleRecord(const SampleID & sampleID, const BiometricID & bioID, const TBiometricSample & sample)
				: Record(sampleID, bioID), sample(sample){
			}
		public:
			TBiometricSample sample;
	};

	template <class TBiometricSample>
	class SampleRecordSelfLoader : public IBiometricSampleLoader < SampleRecord<TBiometricSample>, TBiometricSample>{
		public:
			virtual ~SampleRecordSelfLoader(){

			}
			virtual TBiometricSample createBiometricSample(const SampleRecord<TBiometricSample> & record){
				return record.sample;
			}
	};



	template <class TBiometricSample>
	Results evaluateBlocks(std::vector<typename IProcessingBlock<TBiometricSample>::Ptr> blocks, const std::vector <TBiometricSample> & biometricSamples, const Labels & labels){

			typedef SampleRecord<TBiometricSample> SelfRecord;
			typedef Database<SelfRecord> SelfDb;

			//create DB
			SelfDb db;
			int index = 0;
			for (auto s : biometricSamples){
				SelfRecord rec(labels[index].getSampleID(), labels[index].getBiometricID(), s);
				db.addRecord(rec);
				index++;
			}

			ReferenceAndProbeDatabaseSubsetCreator<SelfRecord> creator(db, 1);

			BlockEvaluator<SelfRecord, TBiometricSample > evaluator(
					std::make_shared<SampleRecordSelfLoader<TBiometricSample> >()
			);

			for (auto b : blocks){
				evaluator.addBlock(b);
			}

			BioFW::BiometricReferenceDatabase referenceDatabase;

			evaluator.extractTemplates(
				creator.getReferenceSubset().getCollections().getRecords(),
				referenceDatabase
			);

			BioFW::Results results;

			evaluator.evaluateRecords(
				creator.getProbeSubset().getCollections().getRecords(),
				referenceDatabase,
				results
			);

			return results;

	}


	template <class TBiometricSample>
	MethodResults evaluateBlock(typename IProcessingBlock<TBiometricSample>::Ptr block, const std::vector <TBiometricSample> & biometricSamples, const Labels & labels){

			std::vector <typename IProcessingBlock<TBiometricSample>::Ptr> blocks;
			blocks.push_back(block);

			Results results = evaluateBlocks<TBiometricSample>(blocks, biometricSamples, labels);

			return results.getMethodResult(block->getName());

	}


} /* namespace BioFW */
#endif /* BLOCKEVALUATOR_H_ */
