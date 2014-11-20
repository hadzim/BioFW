/*
 * ProcessingBlock.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef PROCESSINGBLOCK_H_
#define PROCESSINGBLOCK_H_
#include "BioFW/Block/IComparisonBlock.h"
#include "BioFW/FeatureVector/IFeatureVectorExtractor.h"
#include "BioFW/Template/ITemplateCreator.h"
#include "BioFW/Comparison/IComparator.h"
#include "BioFW/Template/ITemplateSerializer.h"
#include <vector>
#include "BioFW/Block/IProcessingBlock.h"

namespace BioFW {

		template<class TBiometricSample, class TTemplate, class TPersistentTemplate>
		class ProcessingBlock: public IProcessingBlock<TBiometricSample> {
			public:
				typedef std::shared_ptr <ProcessingBlock> Ptr;

				typedef typename IComparisonBlock<TBiometricSample>::TemplateIndex TemplateIndex;
				typedef typename TTemplate::FeatureVector FeatureVector;
				typedef TBiometricSample Sample;
				typedef TTemplate Template;
				typedef TPersistentTemplate PersistentTemplate;

				typedef IFeatureVectorExtractor<Sample, FeatureVector> Extractor;
				typedef IComparator<FeatureVector, Template> Comparator;
				typedef ITemplateCreator<Template> TemplateCreator;
				typedef ITemplateSerializer<Template, PersistentTemplate> TemplateSerializer;

				typedef std::vector<TBiometricSample> Samples;

				ProcessingBlock(
						std::string name,
						typename Extractor::Ptr featureVectorExtractor,
						typename Comparator::Ptr comparator,
						typename TemplateCreator::Ptr templateCreator,
						typename TemplateSerializer::Ptr serializer) :

						name(name),
						featureVectorExtractor(featureVectorExtractor),
						comparator(comparator),
						templateCreator(templateCreator),
						serializer(serializer) {
				}

				virtual ~ProcessingBlock() {

				}

				virtual std::string getName() const {
					return this->name;
				}

				virtual void train(const Samples & biometricSamples, const Labels & labels) {
					featureVectorExtractor->train(biometricSamples, labels);
				}

				virtual IPersistentTemplate::Ptr extract(const TBiometricSample & input) {

					std::string message = "Template extraction: ";
					message += ".";

					this->reportProgress(ProgressReport(message));

					//create feature vector
					FeatureVector featureVector = featureVectorExtractor->extractFeatureVector(input);

					message += ".";
					this->reportProgress(ProgressReport(message));

					//create template
					TTemplate templ = this->templateCreator->createTemplate(featureVector);

					message += ".";
					this->reportProgress(ProgressReport(message));

					//convert to binary form
					IPersistentTemplate::Ptr pt = serializer->serialize(templ);

					message += ".";
					this->reportProgress(ProgressReport(message));

					return pt;
				}

				virtual void resetTemplates() {
					this->referenceList.clear();
				}

				/// push template to actual buffer -> returns template index that is then used for matching
				virtual TemplateIndex pushTemplate(IPersistentTemplate::Ptr t) {
					typename PersistentTemplate::Ptr pers = std::dynamic_pointer_cast<PersistentTemplate>(t);
					if (!pers){
						throw BioFWException("template cannot be casted");
					}
					TTemplate bufferedTemplate(serializer->deserialize(pers));
					referenceList.push_back(bufferedTemplate);
					return referenceList.size() - 1;
				}

				/// set actual input data
				virtual void setInputData(const TBiometricSample & inputData) {
					probeFeatureVector = ProbePtr(new FeatureVector(this->featureVectorExtractor->extractFeatureVector(inputData)));
				}

				/// compute matching score of actual input and template in buffer with given index
				virtual ComparisonScore computeComparisonScore(TemplateIndex templateIndex) {
					if (templateIndex < 0 || templateIndex >= referenceList.size()) {
						throw BioFWException("Compute comaprasion score: Template index ouf of buffer range");
					}
					return this->comparator->computeComparisonScore(*probeFeatureVector.get(), referenceList.at(templateIndex));
				}

			private:
				void reportProgress(const ProgressReport & report){
					ProgressReport r = report;
					this->ProgressChanged.notify(this, r);
				}
			private:
				//block name
				std::string name;
				//feature extracto
				typename Extractor::Ptr featureVectorExtractor;

				//comparator
				typename Comparator::Ptr comparator;

				//template creator
				typename TemplateCreator::Ptr templateCreator;

				//template serializer
				typename TemplateSerializer::Ptr serializer;

			private:
				//internals
				typedef std::vector<TTemplate> References;
				References referenceList;
				typedef std::shared_ptr<FeatureVector> ProbePtr;
				ProbePtr probeFeatureVector;

		};

		template<class TBiometricSample, class TProbeFeatureVector, class TTemplate, class TPersistentTemplate>
				class DifferentExtractorProcessingBlock: public IComparisonBlock<TBiometricSample>, public ITrainingBlock <TBiometricSample> {
					public:
						typedef std::shared_ptr <DifferentExtractorProcessingBlock> Ptr;

						typedef typename IComparisonBlock<TBiometricSample>::TemplateIndex TemplateIndex;
						typedef typename TTemplate::FeatureVector ReferenceFeatureVector;
						typedef TProbeFeatureVector ProbeFeatureVector;
						typedef TBiometricSample Sample;
						typedef TTemplate Template;
						typedef TPersistentTemplate PersistentTemplate;

						typedef IFeatureVectorExtractor<Sample, ReferenceFeatureVector> ReferenceExtractor;
						typedef IFeatureVectorExtractor<Sample, ProbeFeatureVector> ProbeExtractor;
						typedef IComparator<ProbeFeatureVector, Template> Comparator;
						typedef ITemplateCreator<Template> TemplateCreator;
						typedef ITemplateSerializer<Template, PersistentTemplate> TemplateSerializer;

						typedef std::vector<TBiometricSample> Samples;

						DifferentExtractorProcessingBlock(
								std::string name,
								typename ReferenceExtractor::Ptr referenceFeatureVectorExtractor,
								typename ProbeExtractor::Ptr probeFeatureVectorExtractor,
								typename Comparator::Ptr comparator,
								typename TemplateCreator::Ptr templateCreator,
								typename TemplateSerializer::Ptr serializer) :

								name(name),
								referenceFeatureVectorExtractor(referenceFeatureVectorExtractor),
								probeFeatureVectorExtractor(probeFeatureVectorExtractor),
								comparator(comparator),
								templateCreator(templateCreator),
								serializer(serializer) {
						}

						virtual ~DifferentExtractorProcessingBlock() {

						}

						virtual std::string getName() const {
							return this->name;
						}

						virtual void train(const Samples & biometricSamples, const Labels & labels) {
							//both needs to be trained?
							referenceFeatureVectorExtractor->train(biometricSamples, labels);
							//both needs to be trained?
							probeFeatureVectorExtractor->train(biometricSamples, labels);
						}

						virtual IPersistentTemplate::Ptr extract(const TBiometricSample & input) {

							std::string message = "Template extraction: ";
							message += ".";

							this->reportProgress(ProgressReport(message));

							//create feature vector
							ReferenceFeatureVector featureVector = referenceFeatureVectorExtractor->extractFeatureVector(input);

							message += ".";
							this->reportProgress(ProgressReport(message));

							//create template
							TTemplate templ = this->templateCreator->createTemplate(featureVector);

							message += ".";
							this->reportProgress(ProgressReport(message));

							//convert to binary form
							IPersistentTemplate::Ptr pt = serializer->serialize(templ);

							message += ".";
							this->reportProgress(ProgressReport(message));

							return pt;
						}

						virtual void resetTemplates() {
							this->referenceList.clear();
						}

						/// push template to actual buffer -> returns template index that is then used for matching
						virtual TemplateIndex pushTemplate(IPersistentTemplate::Ptr t) {
							typename PersistentTemplate::Ptr pers = std::dynamic_pointer_cast<PersistentTemplate>(t);
							if (!pers){
								throw BioFWException("template cannot be casted");
							}
							TTemplate bufferedTemplate(serializer->deserialize(pers));
							referenceList.push_back(bufferedTemplate);
							return referenceList.size() - 1;
						}

						/// set actual input data
						virtual void setInputData(const TBiometricSample & inputData) {
							probeFeatureVector = ProbePtr(new TProbeFeatureVector(this->probeFeatureVectorExtractor->extractFeatureVector(inputData)));
						}

						/// compute matching score of actual input and template in buffer with given index
						virtual ComparisonScore computeComparisonScore(TemplateIndex templateIndex) {
							if (templateIndex < 0 || templateIndex >= referenceList.size()) {
								throw BioFWException("Compute comaprasion score: Template index ouf of buffer range");
							}
							return this->comparator->computeComparisonScore(*probeFeatureVector.get(), referenceList.at(templateIndex));
						}
					private:
						void reportProgress(const ProgressReport & report){
							ProgressReport r = report;
							this->ProgressChanged.notify(this, r);
						}
					private:
						//block name
						std::string name;
						//Reference feature vector extractor
						typename ReferenceExtractor::Ptr referenceFeatureVectorExtractor;

						//Probe feature vector extractor
						typename ProbeExtractor::Ptr probeFeatureVectorExtractor;

						//comparator
						typename Comparator::Ptr comparator;

						//template creator
						typename TemplateCreator::Ptr templateCreator;

						//template serializer
						typename TemplateSerializer::Ptr serializer;

					private:
						//internals
						typedef std::vector<TTemplate> References;
						References referenceList;
						typedef std::shared_ptr<TProbeFeatureVector> ProbePtr;
						ProbePtr probeFeatureVector;

				};



}  // namespace BioFW

#endif /* PROCESSINGBLOCK_H_ */
