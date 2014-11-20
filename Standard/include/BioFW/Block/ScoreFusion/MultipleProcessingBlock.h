/*
 * MultipleProcessingBlock.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef MULTIPLEPROCESSINGBLOCK_H_
#define MULTIPLEPROCESSINGBLOCK_H_
#include "IScoreFusion.h"
#include "BioFW/Block/ScoreNormalization/ZScoreNormalizer.h"

namespace BioFW {

	template<class TBioSample>
	class MultipleProcessingBlock: public IProcessingBlock<TBioSample> {

		public:
			typedef std::shared_ptr<MultipleProcessingBlock> Ptr;

			typedef IProcessingBlock<TBioSample> Block;
			typedef typename Block::Ptr BlockPtr;

			typedef size_t TemplateIndex;

			MultipleProcessingBlock(std::string name, IScoreFusion::Ptr fusionModule) : fusion(fusionModule), name(name) {

			}
			MultipleProcessingBlock(IScoreFusion::Ptr fusionModule)  : fusion(fusionModule) {

			}
			virtual ~MultipleProcessingBlock() {

			}

			void addBlock(BlockPtr b){

				BlockPtr nb(
						new BioFW::IScoreNormalizingProcessingBlock<TBioSample>(
								b,
								this->fusion->scoreNormalizer()
						)
				);

				//this->blocks.push_back(b);
				this->blocks.push_back(nb);


			}

			void resetTemplates() {
				check();
				templateNumbers.clear();
				for (auto b : blocks) {
					b->resetTemplates();
				}
			}

			TemplateIndex pushTemplate(IPersistentTemplate::Ptr t) {
				try {
					check();
					int index = templateNumbers.size();
					templateNumbers.push_back(TemplateMap());
					for (auto b : blocks) {
						int subindex = b->pushTemplate(t->getSubtemplate(b->getName()));
						templateNumbers[index][b->getName()] = subindex;
					}
					return index;
				} catch (BioFWException & e){
					std::cout << "error in push template of " << this->getName() << std::endl;
					throw;
				}
			}

			void setInputData(const TBioSample & inputData) {
				check();
				for (auto b : blocks) {
					b->setInputData(inputData);
				}
			}

			ComparisonScore computeComparisonScore(TemplateIndex templateIndex) {
				check();
				ComparisonScore::Subscores comparisonScoresToBeResolved;

				for (auto b : blocks) {
					ComparisonScore ms = b->computeComparisonScore(templateNumbers[templateIndex][b->getName()]);
					comparisonScoresToBeResolved[b->getName()] = ms.getScore();
				}

				return fusion->resolve(comparisonScoresToBeResolved);
			}

			IPersistentTemplate::Ptr extract(const TBioSample & biometricSample) {
				check();
				//TODO maybe something else then Mem p template
				IPersistentTemplate::Ptr pt(new MemoryPersistentTemplate());

				for (auto b : blocks) {
					pt->addSubtemplate(b->getName(), b->extract(biometricSample));
				}
				return pt;
			}

			std::string getName() const {
				if (name.empty()) {
					name.append("{");
					for (auto b : blocks) {
						name.append(b->getName());
						name.append(" ");
					}
					name.append("fus:");
					name.append(this->fusion->getName());
					name.append("}");
				}
				return name;
			}

			IScoreFusion::Ptr getScoreFusion() {
				return this->fusion;
			}

			typedef typename ITrainingBlock<TBioSample>::Samples Samples;

			void train(const Samples & biometricSamples, const Labels & labels) {
				check();
				for (auto b : blocks) {
					b->train(biometricSamples, labels);
				}

				Results r = evaluateBlocks<TBioSample>(blocks, biometricSamples, labels);

				this->fusion->train(r);
			}

		private:
			void check() {
				if (blocks.empty()) {
					throw std::runtime_error("Blocks are empty in MultipleProcessingBlock");
				}
			}
		private:
			std::vector<BlockPtr> blocks;
			typedef std::map<std::string, int> TemplateMap;
			std::vector<TemplateMap> templateNumbers;
			IScoreFusion::Ptr fusion;
			mutable std::string name;


	};

} /* namespace BioFW */
#endif /* MULTIPLEPROCESSINGBLOCK_H_ */
