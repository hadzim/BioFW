
/*
 * main.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include "BioFW/Database/Record.h"
#include "BioFW/Database/IDatabaseCreator.h"
#include "BioFW/Database/Database.h"

#include <iostream>
#include <memory>

#include "BioFW/Utils/Database.h"
#include <Poco/Glob.h>
#include "BioFW/Database/BiometricID.h"
#include <Poco/StringTokenizer.h>
#include <Poco/Delegate.h>
#include "BioFW/Database/Subset/ReferenceAndProbeDatabaseSubsetCreator.h"

#include <opencv2/opencv.hpp>
#include "BioFW/BiometricSample/IBiometricSampleLoader.h"
#include "BioFW/FeatureVector/IFeatureVectorExtractor.h"
#include "BioFW/Comparison/IFeatureVectorComparator.h"
#include "BioFW/Template/Template.h"
#include "BioFW/Template/TemplateCreator.h"
#include "BioFW/Template/MemoryPersistentTemplate.h"
#include "BioFW/Template/ITemplateSerializer.h"
#include "BioFW/Block/ProcessingBlock.h"
#include "BioFW/Comparsion/Comparator.h"
#include "BioFW/Evaluation/BlockEvaluator.h"
#include "BioFW/Database/Reference/BiometricReferenceDataRecord.h"
#include "BioFW/Utils/ResultsOverview.h"


template <
	class TInputRecord,
	class TBiometricSample
>
class ProjectTypes {
	public:
		typedef TInputRecord InputRecord;
		typedef TBiometricSample BiometricSample;
};

typedef BioFW::Record FaceRecord;



class JaffeCreator : public BioFW::IDatabaseCreator <FaceRecord> {
	public:
		virtual Db createDatabase(){
			std::string path = "c:/Data/JAFFE/";
			std::set<std::string> records;
			Poco::Glob::glob(path + "*.tiff", records);

			Db db;
			for (std::set<std::string>::iterator i = records.begin(); i != records.end(); i++){
				Poco::StringTokenizer st(*i, ".");

				BioFW::BiometricID bid(st[0], "Face");

				FaceRecord fr(*i, bid);
				db.addRecord(fr);
			}
			return db;
		}
};

class FaceSample {
	public:
		cv::Mat image;
};

class FaceSampleLoader : public BioFW::IBiometricSampleLoader<FaceRecord, FaceSample> {
	public:
		virtual Sample createBiometricSample(const Record & record){
			FaceSample s;
			s.image = cv::imread(record.getSampleID());
			return s;
		}
};

class FaceFeatureVector {
	public:
		cv::Mat fv;
};

class FaceFeatureVectorExtractor : public BioFW::IFeatureVectorExtractor <FaceSample, FaceFeatureVector> {
	public:
		virtual FeatureVector extractFeatureVector(const Sample & input){
			FeatureVector fv;
			fv.fv = input.image;
			return fv;
		}
};

class FaceFeatureVectorComparator : public BioFW::IFeatureVectorComparator <FaceFeatureVector> {
	public:
		virtual BioFW::ComparisonScore computeComparisonScore(const ProbeFeatureVector & probe, const ReferenceFeatureVector & ref){
			cv::Mat diff;
			cv::absdiff(probe.fv, ref.fv, diff);
			return BioFW::ComparisonScore(cv::mean(diff).val[0]);
		}
};


typedef BioFW::Template<FaceFeatureVector> FaceTemplate;
typedef BioFW::TemplateCreator<FaceTemplate> FaceTemplateCreator;

typedef BioFW::Comparator<FaceFeatureVector, FaceTemplate> FaceComparator;

class FacePersTemplate : public BioFW::MemoryPersistentTemplate {
	public:
		FacePersTemplate(){

		}
		typedef std::shared_ptr <FacePersTemplate> Ptr;
		FaceTemplate faceTemplate;
};

class FaceTempalteSerializer : public BioFW::ITemplateSerializer <FaceTemplate, FacePersTemplate> {
	public:

		virtual PersistentTemplatePtr serialize(const Template & from){

			FacePersTemplate::Ptr fpt(new FacePersTemplate());
			fpt->faceTemplate = from;
			return fpt;
		}
		virtual Template deserialize(PersistentTemplatePtr fptr){
			return fptr->faceTemplate;
		}
};

void printDb(const JaffeCreator::Db & db, std::string name){
	std::cout << "-----------" << std::endl;
	std::cout << name << ":" << std::endl;
	BioFW::Utils::dumpDatabaseOverview(std::cout, db);
	std::cout << "-----------" << std::endl;
}

typedef BioFW::ProcessingBlock <FaceSample, FaceTemplate, FacePersTemplate> Block;

void onProgress(BioFW::ProgressReport & report){
	std::cout << "\r";
	std::cout << "progress: " << report.getProgress() << "% " << report.getMessage() << "                ";
	std::cout << std::flush; //()<< std::endl;
}

int main(int argc, char **argv) {

	JaffeCreator creator;
	JaffeCreator::Db db = creator.createDatabase();

	printDb(db, "Full database");

	//create database subsets
	BioFW::ReferenceAndProbeDatabaseSubsetCreator<FaceRecord> referenceAndProbeSubset(db, 1);

	//template subset
	JaffeCreator::Db referenceDbSubset = referenceAndProbeSubset.getDatabaseSubset(BioFW::ReferenceAndProbeDatabaseSubsetCreator<FaceRecord>::ReferenceSubsetName());
	printDb(referenceDbSubset, "Reference database");

	JaffeCreator::Db probeDbSubset = referenceAndProbeSubset.getDatabaseSubset(BioFW::ReferenceAndProbeDatabaseSubsetCreator<FaceRecord>::ProbeSubsetName());
	printDb(probeDbSubset, "Probe database");


	Block::Ptr processingBlock(new Block(
			"simple",
			std::make_shared<FaceFeatureVectorExtractor>(),
			std::make_shared<FaceComparator>(std::make_shared<FaceFeatureVectorComparator>()),
			std::make_shared<FaceTemplateCreator>(),
			std::make_shared<FaceTempalteSerializer>()
	));

	BioFW::BlockEvaluator<FaceRecord, FaceSample> evaluator(std::make_shared<FaceSampleLoader>());
	evaluator.addBlock(processingBlock);

	 //database for templates
	BioFW::BiometricReferenceDatabase referenceDatabase;
	//progress report event
	evaluator.ProgressChanged += Poco::delegate(&onProgress);

	evaluator.extractTemplates(
		referenceDbSubset.getCollections().getRecords(),
		referenceDatabase
	);

	std::cout << std::endl;
	std::cout << "---------------------------------" << std::endl;
	std::cout << "All reference templates extracted" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	std::cout << std::endl;

	BioFW::Results results;

	evaluator.evaluateRecords(
		probeDbSubset.getCollections().getRecords(),
		referenceDatabase,
		results
	);

	std::cout << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << "All results computed" << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << std::endl;

	evaluator.ProgressChanged -= Poco::delegate(&onProgress);


	cv::Mat image = BioFW::Utils::renderGenuineImpostorGraph(
			results.getMethodResult(processingBlock->getName()));
	cv::imshow("gi", image);
	cv::waitKey(0);






	return 0;
}
