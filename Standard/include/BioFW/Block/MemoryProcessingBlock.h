/*
 * ProcessingBlock.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef MEMPROCESSINGBLOCK_H_
#define MEMPROCESSINGBLOCK_H_
#include "BioFW/Block/IComparisonBlock.h"
#include "BioFW/FeatureVector/IFeatureVectorExtractor.h"
#include "BioFW/Template/ITemplateCreator.h"
#include "BioFW/Comparison/IComparator.h"

namespace BioFW {

	template<class TBiometricSample, class TTemplate>
	class MemoryProcessingBlock: public IMemoryComparisonBlock<TBiometricSample, TTemplate>, public ITrainingBlock<TBiometricSample> {
		public:
			typedef std::shared_ptr <MemoryProcessingBlock> Ptr;

			typedef typename IComparisonBlock<TBiometricSample>::TemplateIndex TemplateIndex;
			typedef typename TTemplate::FeatureVector FeatureVector;
			typedef TBiometricSample Sample;
			typedef TTemplate Template;


			typedef IFeatureVectorExtractor<Sample, FeatureVector> Extractor;
			typedef IComparator<FeatureVector, Template> Comparator;
			typedef ITemplateCreator<Template> TemplateCreator;

			typedef std::vector<TBiometricSample> Samples;

			MemoryProcessingBlock(
					std::string name,
					typename Extractor::Ptr featureVectorExtractor,
					typename Comparator::Ptr comparator,
					typename TemplateCreator::Ptr templateCreator) :
						name(name),
						featureVectorExtractor(featureVectorExtractor),
						comparator(comparator),
						templateCreator(templateCreator) {
			}

			virtual ~MemoryProcessingBlock() {

			}

			virtual std::string getName() const {
				return this->name;
			}

			virtual void train(const Samples & biometricSamples, const Labels & labels) {
				featureVectorExtractor->train(biometricSamples, labels);
			}

			virtual Template extract(const TBiometricSample & input) {

				std::string message = "Template extraction: ";
				message += ".";

				this->ProgressChanged(ProgressReport(message));

				//create feature vector
				FeatureVector featureVector = featureVectorExtractor->extractFeatureVector(input);

				message += ".";
				this->ProgressChanged(ProgressReport(message));

				//create template
				Template templ = this->templateCreator->createTemplate(featureVector);

				message += ".";
				this->ProgressChanged(ProgressReport(message));

				return templ;
			}

			virtual void resetTemplates() {
				this->referenceList.clear();
			}

			/// push template to actual buffer -> returns template index that is then used for matching
			virtual TemplateIndex pushTemplate(Template t) {
				referenceList.Add(t);
				return referenceList.size() - 1;
			}

			/// set actual input data
			virtual void setInputData(const Sample & inputData) {
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



			//block name
			std::string name;
			//Reference feature vector extractor
			typename Extractor::Ptr featureVectorExtractor;

			//comparator
			typename Comparator::Ptr comparator;

			//template creator
			typename TemplateCreator::Ptr templateCreator;

		private:
			//internals
			typedef std::vector<TTemplate> References;
			References referenceList;
			typedef std::shared_ptr<FeatureVector> ProbePtr;
			ProbePtr probeFeatureVector;

	};

	template<class TBiometricSample, class TProbeFeatureVector, class TTemplate>
	class DifferentExtractorMemoryProcessingBlock: public IMemoryComparisonBlock<TBiometricSample, TTemplate>, public ITrainingBlock<TBiometricSample> {
		public:
			typedef std::shared_ptr <DifferentExtractorMemoryProcessingBlock> Ptr;


			typedef typename TTemplate::FeatureVector ReferenceFeatureVector;
			typedef TProbeFeatureVector ProbeFeatureVector;

			typedef typename IComparisonBlock<TBiometricSample>::TemplateIndex TemplateIndex;
			typedef TBiometricSample Sample;
			typedef TTemplate Template;


			typedef IFeatureVectorExtractor<Sample, ReferenceFeatureVector> ReferenceExtractor;
			typedef IFeatureVectorExtractor<Sample, ProbeFeatureVector> ProbeExtractor;
			typedef IComparator<ProbeFeatureVector, Template> Comparator;
			typedef ITemplateCreator<Template> TemplateCreator;

			typedef std::vector<TBiometricSample> Samples;


			DifferentExtractorMemoryProcessingBlock(
					std::string name,
					typename ReferenceExtractor::Ptr referenceFeatureVectorExtractor,
					typename ProbeExtractor::Ptr probeFeatureVectorExtractor,
					typename Comparator::Ptr comparator,
					typename TemplateCreator::Ptr templateCreator) :

					name(name), referenceFeatureVectorExtractor(referenceFeatureVectorExtractor), probeFeatureVectorExtractor(probeFeatureVectorExtractor), comparator(comparator), templateCreator(templateCreator) {
			}

			virtual ~DifferentExtractorMemoryProcessingBlock() {

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

			virtual Template extract(const TBiometricSample & input) {

				std::string message = "Template extraction: ";
				message += ".";

				this->ProgressChanged(ProgressReport(message));

				//create feature vector
				ReferenceFeatureVector featureVector = referenceFeatureVectorExtractor->extractFeatureVector(input);

				message += ".";
				this->ProgressChanged(ProgressReport(message));

				//create template
				Template templ = this->templateCreator->createTemplate(featureVector);

				message += ".";
				this->ProgressChanged(ProgressReport(message));

				return templ;
			}

			virtual void resetTemplates() {
				this->referenceList.clear();
			}

			/// push template to actual buffer -> returns template index that is then used for matching
			virtual TemplateIndex pushTemplate(Template t) {
				referenceList.Add(t);
				return referenceList.size() - 1;
			}

			/// set actual input data
			virtual void setInputData(const Sample & inputData) {
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
			//block name
			std::string name;
			//Reference feature vector extractor
			typename ReferenceExtractor::Ptr::Ptr referenceFeatureVectorExtractor;

			//Probe feature vector extractor
			typename ProbeExtractor::Ptr::Ptr probeFeatureVectorExtractor;

			//comparator
			typename Comparator::Ptr comparator;

			//template creator
			typename TemplateCreator::Ptr templateCreator;

		private:
			//internals
			typedef std::vector<TTemplate> References;
			References referenceList;
			typedef std::shared_ptr<TProbeFeatureVector> ProbePtr;
			ProbePtr probeFeatureVector;

	};

}  // namespace BioFW

#endif /* PROCESSINGBLOCK_H_ */
