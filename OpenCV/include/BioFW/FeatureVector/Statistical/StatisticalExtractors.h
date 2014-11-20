/*
 * EigenFacesExtractor.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef EIGENFACESEXTRACTOR_H_
#define EIGENFACESEXTRACTOR_H_

/*
 * Copyright (c) 2011,2012. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include <set>
#include <opencv2/opencv.hpp>
#include "BioFW/BiometricSample/MatSample.h"

namespace BioFW {
	template<class Samples>
	static cv::Mat asRowMatrix(const Samples & src, int rtype, double alpha = 1, double beta = 0) {

		// number of samples
		size_t n = src.size();
		// return empty matrix if no matrices given
		if (n == 0)
			return cv::Mat();
		// dimensionality of (reshaped) samples
		size_t d = src[0].mat().total();
		// create data matrix
		cv::Mat data((int) n, (int) d, rtype);
		// now copy data
		for (unsigned int i = 0; i < n; i++) {
			// make sure data can be reshaped, throw exception if not!
			if (src[i].mat().total() != d) {
				std::string error_message = cv::format("Wrong number of elements in matrix #%d! Expected %d was %d.",
						i, d,
						src[i].mat().total());
				CV_Error(CV_StsBadArg, error_message);
			}
			// get a hold of the current row
			cv::Mat xi = data.row(i);
			// make reshape happy by cloning for non-continuous matrices
			if (src[i].mat().isContinuous()) {
				src[i].mat().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
			} else {
				src[i].mat().clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
			}
		}
		return data;
	}

// Removes duplicate elements in a given vector.
	template<typename _Tp>
	inline std::vector<_Tp> remove_dups(const std::vector<_Tp>& src) {
		typedef typename std::set<_Tp>::const_iterator constSetIterator;
		typedef typename std::vector<_Tp>::const_iterator constVecIterator;
		std::set<_Tp> set_elems;
		for (constVecIterator it = src.begin(); it != src.end(); ++it)
			set_elems.insert(*it);
		std::vector<_Tp> elems;
		for (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
			elems.push_back(*it);
		return elems;
	}

// Turk, M., and Pentland, A. "Eigenfaces for recognition.". Journal of
// Cognitive Neuroscience 3 (1991), 71–86.
	class EigenfacesExtractor: public IFeatureVectorExtractor<MatSample, MatFV> {
		private:
			double _reduction;
			int _num_components;
			double _threshold;
			std::vector<cv::Mat> _projections;
			cv::Mat _labels;
			cv::Mat _eigenvectors;
			cv::Mat _eigenvalues;
			cv::Mat _mean;

			cv::PCA cvPca;

		public:

			typedef NormComparator<cv::NORM_L2, MatFV, MatFV> DefaultComparator;

			// Initializes an empty Eigenfaces model.
			EigenfacesExtractor(double _reduction = 1.0, int num_components = 0, double threshold = DBL_MAX) :
				_reduction(_reduction), _num_components(num_components), _threshold(threshold) {
			}

			virtual FeatureVector extractFeatureVector(const Sample & input) {
				// get data
				cv::Mat src = input.mat();
				// make sure the user is passing correct data
				if (_projections.empty()) {
					// throw error if no data (or simply return -1?)
					std::string error_message = "This Eigenfaces model is not computed yet. Did you call Eigenfaces::train?";
					CV_Error(CV_StsError, error_message);
				} else if (_eigenvectors.rows != static_cast<int>(src.total())) {
					// check data alignment just for clearer exception messages
					std::string error_message = cv::format("Wrong input image size. Reason: Training and Test images must be of equal size! Expected an image with %d elements, but got %d.", _eigenvectors.rows, src.total());
					CV_Error(CV_StsBadArg, error_message);
				}

				cv::Mat q = cvPca.project(src.reshape(1, 1));

				// project into PCA subspace
				//cv::Mat q = cv::subspaceProject(_eigenvectors, _mean, src.reshape(1, 1));
				MatFV fv;
				fv.mat() = q;
				return fv;
				/*
				double minDist;
				minDist = DBL_MAX;
				minClass = -1;
						for (size_t sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++) {
							double dist = norm(_projections[sampleIdx], q, NORM_L2);
							if ((dist < minDist) && (dist < _threshold)) {
								minDist = dist;
								minClass = _labels.at<int>((int) sampleIdx);
							}
						}*/
			}

			virtual void train(const Samples & inputs, const Labels & labels) {
				int total = inputs.size();
				if (total == 0) {
					throw std::runtime_error("Empty training data was given. You'll need more than one sample to learn a model.");
				}
				// make sure data has correct size
				if (total > 1) {
					for (int i = 1; i < total; i++) {
						if (inputs[i - 1].mat().total() != inputs[i].mat().total()) {
							std::string error_message = cv::format("In the Eigenfaces method all input samples (training images) must be of equal size! Expected %d pixels, but was %d pixels.",
									inputs[i - 1].mat().total(), inputs[i].mat().total());
							CV_Error(CV_StsUnsupportedFormat, error_message);
						}
					}
				}
				// observations in row
				cv::Mat data = asRowMatrix<Samples>(inputs, CV_64FC1);

				// number of samples
				int n = data.rows;

				// clear existing model data
				_projections.clear();
				// clip number of components to be valid
				if ((_num_components <= 0) || (_num_components > n))
					_num_components = n;

				// perform the PCA
				cvPca = cv::PCA(data, cv::Mat(), CV_PCA_DATA_AS_ROW, _num_components);
				// copy the PCA results
				_mean = cvPca.mean.reshape(1, 1); // store the mean vector
				_eigenvalues = cvPca.eigenvalues.clone(); // eigenvalues by row
				cv::transpose(cvPca.eigenvectors, _eigenvectors); // eigenvectors by column
				// save projections
				for (int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++) {
					cv::Mat p = cv::subspaceProject(_eigenvectors, _mean, data.row(sampleIdx));
					_projections.push_back(p);
				}

				reduction();
			}

			void reduction()
				{
					if (_reduction >= 1) return; // nothing to do here

				    double sum = 0.0;
				    int r;
				    for (r = 0; r < cvPca.eigenvalues.rows; r++)
				    {
				        sum += cvPca.eigenvalues.at<double>(r);
				    }

				    double actualSum = 0.0;
				    for (r = 0; r < cvPca.eigenvalues.rows; r++)
				    {
				        actualSum += cvPca.eigenvalues.at<double>(r);

				        if (actualSum/sum > _reduction)
				            break;
				    }

				    cvPca.eigenvectors = cvPca.eigenvectors.rowRange(0, r+1);
				    cvPca.eigenvalues = cvPca.eigenvalues.rowRange(0, r+1);
				}

	};



// Belhumeur, P. N., Hespanha, J., and Kriegman, D. "Eigenfaces vs. Fisher-
// faces: Recognition using class specific linear projection.". IEEE
// Transactions on Pattern Analysis and Machine Intelligence 19, 7 (1997),
// 711–720.
	class FisherfacesExtractor : public IFeatureVectorExtractor<MatSample, MatFV> {
		private:
			int _num_components;
			double _threshold;
			cv::Mat _eigenvectors;
			cv::Mat _eigenvalues;
			cv::Mat _mean;
			std::vector<cv::Mat> _projections;
			cv::Mat _labels;

		public:

			typedef NormComparator<cv::NORM_L2, MatFV, MatFV> DefaultComparator;

			// Initializes an empty Fisherfaces model.
			FisherfacesExtractor(int num_components = 0, double threshold = DBL_MAX) :
					_num_components(num_components), _threshold(threshold) {
			}

			virtual FeatureVector extractFeatureVector(const Sample & input) {
				cv::Mat src = input.mat();
				// check data alignment just for clearer exception messages
				if (_projections.empty()) {
					// throw error if no data (or simply return -1?)
					std::string error_message = "This Fisherfaces model is not computed yet. Did you call Fisherfaces::train?";
					CV_Error(CV_StsBadArg, error_message);
				} else if (src.total() != (size_t) _eigenvectors.rows) {
					std::string error_message = cv::format("Wrong input image size. Reason: Training and Test images must be of equal size! Expected an image with %d elements, but got %d.", _eigenvectors.rows, src.total());
					CV_Error(CV_StsBadArg, error_message);
				}
				// project into LDA subspace
				cv::Mat q = subspaceProject(_eigenvectors, _mean, src.reshape(1, 1));
				MatFV fv;
				fv.mat() = q;
				return fv;
			}

			virtual void train(const Samples & inputs, const Labels & labels) {
				int total = inputs.size();

				if (total == 0) {
					std::string error_message = cv::format("Empty training data was given. You'll need more than one sample to learn a model.");
					CV_Error(CV_StsBadArg, error_message);
				}
				// make sure data has correct size

				if (total > 1) {
					for (int i = 1; i < total; i++) {
						if (inputs[i - 1].mat().total() != inputs[i].mat().total()) {
							std::string error_message = cv::format("In the Fisherefaces method all input samples (training images) must be of equal size! Expected %d pixels, but was %d pixels.",
									inputs[i - 1].mat().total(), inputs[i].mat().total());
							CV_Error(CV_StsUnsupportedFormat, error_message);
						}
					}
				}
				// get data
				cv::Mat data = asRowMatrix<Samples>(inputs, CV_64FC1);
				// number of samples
				int N = data.rows;

				// clear existing model data
				_projections.clear();
				// safely copy from cv::Mat to std::vector
				std::vector<int> ll;
				Labels2Int l2i(labels);
				for (auto l : labels){
					ll.push_back(l2i.index(l));
				}


				// get the number of unique classes
				int C = (int) remove_dups(ll).size();
				// clip number of components to be a valid number
				if ((_num_components <= 0) || (_num_components > (C - 1)))
					_num_components = (C - 1);
				// perform a PCA and keep (N-C) components
				cv::PCA pca(data, cv::Mat(), CV_PCA_DATA_AS_ROW, (N - C));
				// project the data and perform a LDA on it
				cv::LDA lda(pca.project(data), ll, _num_components);
				// store the total mean vector
				_mean = pca.mean.reshape(1, 1);
				// store the eigenvalues of the discriminants
				lda.eigenvalues().convertTo(_eigenvalues, CV_64FC1);
				// Now calculate the projection matrix as pca.eigenvectors * lda.eigenvectors.
				// Note: OpenCV stores the eigenvectors by row, so we need to transpose it!
				cv::gemm(pca.eigenvectors, lda.eigenvectors(), 1.0, cv::Mat(), 0.0, _eigenvectors, cv::GEMM_1_T);
				// store the projections of the original data
				for (int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++) {
					cv::Mat p = cv::subspaceProject(_eigenvectors, _mean, data.row(sampleIdx));
					_projections.push_back(p);
				}
			}
	};

// Face Recognition based on Local Binary Patterns.
//
//  Ahonen T, Hadid A. and Pietikäinen M. "Face description with local binary
//  patterns: Application to face recognition." IEEE Transactions on Pattern
//  Analysis and Machine Intelligence, 28(12):2037-2041.
//
//	class LBPH {
//		private:
//			int _grid_x;
//			int _grid_y;
//			int _radius;
//			int _neighbors;
//			double _threshold;
//
//			std::vector<cv::Mat> _histograms;
//			cv::Mat _labels;
//
//			// Computes a LBPH model with images in src and
//			// corresponding labels in labels, possibly preserving
//			// old model data.
//			void train(InputArrayOfArrays src, InputArray labels, bool preserveData);
//
//		public:
//
//			// Initializes this LBPH Model. The current implementation is rather fixed
//			// as it uses the Extended Local Binary Patterns per default.
//			//
//			// radius, neighbors are used in the local binary patterns creation.
//			// grid_x, grid_y control the grid size of the spatial histograms.
//			LBPH(int radius_ = 1, int neighbors_ = 8, int gridx = 8, int gridy = 8, double threshold = DBL_MAX) :
//					_grid_x(gridx), _grid_y(gridy), _radius(radius_), _neighbors(neighbors_), _threshold(threshold) {
//			}
//
//			~LBPH() {
//			}
//
//			// Getter functions.
//			int neighbors() const {
//				return _neighbors;
//			}
//			int radius() const {
//				return _radius;
//			}
//			int grid_x() const {
//				return _grid_x;
//			}
//			int grid_y() const {
//				return _grid_y;
//			}
//
//	};
//
////------------------------------------------------------------------------------
//// Eigenfaces
////------------------------------------------------------------------------------
//	void Eigenfaces::train(InputArrayOfArrays _src, InputArray _local_labels) {
//		if (_src.total() == 0) {
//			string error_message = format("Empty training data was given. You'll need more than one sample to learn a model.");
//			CV_Error(CV_StsBadArg, error_message);
//		} else if (_local_labels.getMat().type() != CV_32SC1) {
//			string error_message = format("Labels must be given as integer (CV_32SC1). Expected %d, but was %d.", CV_32SC1, _local_labels.type());
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		// make sure data has correct size
//		if (_src.total() > 1) {
//			for (int i = 1; i < static_cast<int>(_src.total()); i++) {
//				if (_src.getMat(i - 1).total() != _src.getMat(i).total()) {
//					string error_message = format("In the Eigenfaces method all input samples (training images) must be of equal size! Expected %d pixels, but was %d pixels.", _src.getMat(i - 1).total(),
//							_src.getMat(i).total());
//					CV_Error(CV_StsUnsupportedFormat, error_message);
//				}
//			}
//		}
//		// get labels
//		Mat labels = _local_labels.getMat();
//		// observations in row
//		Mat data = asRowMatrix(_src, CV_64FC1);
//
//		// number of samples
//		int n = data.rows;
//		// assert there are as much samples as labels
//		if (static_cast<int>(labels.total()) != n) {
//			string error_message = format("The number of samples (src) must equal the number of labels (labels)! len(src)=%d, len(labels)=%d.", n, labels.total());
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		// clear existing model data
//		_labels.release();
//		_projections.clear();
//		// clip number of components to be valid
//		if ((_num_components <= 0) || (_num_components > n))
//			_num_components = n;
//
//		// perform the PCA
//		PCA pca(data, Mat(), CV_PCA_DATA_AS_ROW, _num_components);
//		// copy the PCA results
//		_mean = pca.mean.reshape(1, 1); // store the mean vector
//		_eigenvalues = pca.eigenvalues.clone(); // eigenvalues by row
//		transpose(pca.eigenvectors, _eigenvectors); // eigenvectors by column
//		// store labels for prediction
//		_labels = labels.clone();
//		// save projections
//		for (int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++) {
//			Mat p = subspaceProject(_eigenvectors, _mean, data.row(sampleIdx));
//			_projections.push_back(p);
//		}
//	}
//
//	void Eigenfaces::predict(InputArray _src, int &minClass, double &minDist) const {
//		// get data
//		Mat src = _src.getMat();
//		// make sure the user is passing correct data
//		if (_projections.empty()) {
//			// throw error if no data (or simply return -1?)
//			string error_message = "This Eigenfaces model is not computed yet. Did you call Eigenfaces::train?";
//			CV_Error(CV_StsError, error_message);
//		} else if (_eigenvectors.rows != static_cast<int>(src.total())) {
//			// check data alignment just for clearer exception messages
//			string error_message = format("Wrong input image size. Reason: Training and Test images must be of equal size! Expected an image with %d elements, but got %d.", _eigenvectors.rows, src.total());
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		// project into PCA subspace
//		Mat q = subspaceProject(_eigenvectors, _mean, src.reshape(1, 1));
//		minDist = DBL_MAX;
//		minClass = -1;
//		for (size_t sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++) {
//			double dist = norm(_projections[sampleIdx], q, NORM_L2);
//			if ((dist < minDist) && (dist < _threshold)) {
//				minDist = dist;
//				minClass = _labels.at<int>((int) sampleIdx);
//			}
//		}
//	}
//
////------------------------------------------------------------------------------
//// Fisherfaces
////------------------------------------------------------------------------------
//	void Fisherfaces::train(InputArrayOfArrays src, InputArray _lbls) {
//		if (src.total() == 0) {
//			string error_message = format("Empty training data was given. You'll need more than one sample to learn a model.");
//			CV_Error(CV_StsBadArg, error_message);
//		} else if (_lbls.getMat().type() != CV_32SC1) {
//			string error_message = format("Labels must be given as integer (CV_32SC1). Expected %d, but was %d.", CV_32SC1, _lbls.type());
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		// make sure data has correct size
//		if (src.total() > 1) {
//			for (int i = 1; i < static_cast<int>(src.total()); i++) {
//				if (src.getMat(i - 1).total() != src.getMat(i).total()) {
//					string error_message = format("In the Fisherfaces method all input samples (training images) must be of equal size! Expected %d pixels, but was %d pixels.", src.getMat(i - 1).total(),
//							src.getMat(i).total());
//					CV_Error(CV_StsUnsupportedFormat, error_message);
//				}
//			}
//		}
//		// get data
//		Mat labels = _lbls.getMat();
//		Mat data = asRowMatrix(src, CV_64FC1);
//		// number of samples
//		int N = data.rows;
//		// make sure labels are passed in correct shape
//		if (labels.total() != (size_t) N) {
//			string error_message = format("The number of samples (src) must equal the number of labels (labels)! len(src)=%d, len(labels)=%d.", N, labels.total());
//			CV_Error(CV_StsBadArg, error_message);
//		} else if (labels.rows != 1 && labels.cols != 1) {
//			string error_message = format("Expected the labels in a matrix with one row or column! Given dimensions are rows=%s, cols=%d.", labels.rows, labels.cols);
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		// clear existing model data
//		_labels.release();
//		_projections.clear();
//		// safely copy from cv::Mat to std::vector
//		vector<int> ll;
//		for (unsigned int i = 0; i < labels.total(); i++) {
//			ll.push_back(labels.at<int>(i));
//		}
//		// get the number of unique classes
//		int C = (int) remove_dups(ll).size();
//		// clip number of components to be a valid number
//		if ((_num_components <= 0) || (_num_components > (C - 1)))
//			_num_components = (C - 1);
//		// perform a PCA and keep (N-C) components
//		PCA pca(data, Mat(), CV_PCA_DATA_AS_ROW, (N - C));
//		// project the data and perform a LDA on it
//		LDA lda(pca.project(data), labels, _num_components);
//		// store the total mean vector
//		_mean = pca.mean.reshape(1, 1);
//		// store labels
//		_labels = labels.clone();
//		// store the eigenvalues of the discriminants
//		lda.eigenvalues().convertTo(_eigenvalues, CV_64FC1);
//		// Now calculate the projection matrix as pca.eigenvectors * lda.eigenvectors.
//		// Note: OpenCV stores the eigenvectors by row, so we need to transpose it!
//		gemm(pca.eigenvectors, lda.eigenvectors(), 1.0, Mat(), 0.0, _eigenvectors, GEMM_1_T);
//		// store the projections of the original data
//		for (int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++) {
//			Mat p = subspaceProject(_eigenvectors, _mean, data.row(sampleIdx));
//			_projections.push_back(p);
//		}
//	}
//
//	void Fisherfaces::predict(InputArray _src, int &minClass, double &minDist) const {
//		Mat src = _src.getMat();
//		// check data alignment just for clearer exception messages
//		if (_projections.empty()) {
//			// throw error if no data (or simply return -1?)
//			string error_message = "This Fisherfaces model is not computed yet. Did you call Fisherfaces::train?";
//			CV_Error(CV_StsBadArg, error_message);
//		} else if (src.total() != (size_t) _eigenvectors.rows) {
//			string error_message = format("Wrong input image size. Reason: Training and Test images must be of equal size! Expected an image with %d elements, but got %d.", _eigenvectors.rows, src.total());
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		// project into LDA subspace
//		Mat q = subspaceProject(_eigenvectors, _mean, src.reshape(1, 1));
//		// find 1-nearest neighbor
//		minDist = DBL_MAX;
//		minClass = -1;
//		for (size_t sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++) {
//			double dist = norm(_projections[sampleIdx], q, NORM_L2);
//			if ((dist < minDist) && (dist < _threshold)) {
//				minDist = dist;
//				minClass = _labels.at<int>((int) sampleIdx);
//			}
//		}
//	}
//
//	int Fisherfaces::predict(InputArray _src) const {
//		int label;
//		double dummy;
//		predict(_src, label, dummy);
//		return label;
//	}
//
//// See FaceRecognizer::load.
//	void Fisherfaces::load(const FileStorage& fs) {
//		//read matrices
//		fs["num_components"] >> _num_components;
//		fs["mean"] >> _mean;
//		fs["eigenvalues"] >> _eigenvalues;
//		fs["eigenvectors"] >> _eigenvectors;
//		// read sequences
//		readFileNodeList(fs["projections"], _projections);
//		fs["labels"] >> _labels;
//	}
//
//// See FaceRecognizer::save.
//	void Fisherfaces::save(FileStorage& fs) const {
//		// write matrices
//		fs << "num_components" << _num_components;
//		fs << "mean" << _mean;
//		fs << "eigenvalues" << _eigenvalues;
//		fs << "eigenvectors" << _eigenvectors;
//		// write sequences
//		writeFileNodeList(fs, "projections", _projections);
//		fs << "labels" << _labels;
//	}
//
////------------------------------------------------------------------------------
//// LBPH
////------------------------------------------------------------------------------
//
//	template<typename _Tp> static
//	void olbp_(InputArray _src, OutputArray _dst) {
//		// get matrices
//		Mat src = _src.getMat();
//		// allocate memory for result
//		_dst.create(src.rows - 2, src.cols - 2, CV_8UC1);
//		Mat dst = _dst.getMat();
//		// zero the result matrix
//		dst.setTo(0);
//		// calculate patterns
//		for (int i = 1; i < src.rows - 1; i++) {
//			for (int j = 1; j < src.cols - 1; j++) {
//				_Tp center = src.at < _Tp > (i, j);
//				unsigned char code = 0;
//				code |= (src.at < _Tp > (i - 1, j - 1) >= center) << 7;
//				code |= (src.at < _Tp > (i - 1, j) >= center) << 6;
//				code |= (src.at < _Tp > (i - 1, j + 1) >= center) << 5;
//				code |= (src.at < _Tp > (i, j + 1) >= center) << 4;
//				code |= (src.at < _Tp > (i + 1, j + 1) >= center) << 3;
//				code |= (src.at < _Tp > (i + 1, j) >= center) << 2;
//				code |= (src.at < _Tp > (i + 1, j - 1) >= center) << 1;
//				code |= (src.at < _Tp > (i, j - 1) >= center) << 0;
//				dst.at<unsigned char>(i - 1, j - 1) = code;
//			}
//		}
//	}
//
////------------------------------------------------------------------------------
//// cv::elbp
////------------------------------------------------------------------------------
//	template<typename _Tp> static
//	inline void elbp_(InputArray _src, OutputArray _dst, int radius, int neighbors) {
//		//get matrices
//		Mat src = _src.getMat();
//		// allocate memory for result
//		_dst.create(src.rows - 2 * radius, src.cols - 2 * radius, CV_32SC1);
//		Mat dst = _dst.getMat();
//		// zero
//		dst.setTo(0);
//		for (int n = 0; n < neighbors; n++) {
//			// sample points
//			float x = static_cast<float>(-radius * sin(2.0 * CV_PI * n / static_cast<float>(neighbors)));
//			float y = static_cast<float>(radius * cos(2.0 * CV_PI * n / static_cast<float>(neighbors)));
//			// relative indices
//			int fx = static_cast<int>(floor(x));
//			int fy = static_cast<int>(floor(y));
//			int cx = static_cast<int>(ceil(x));
//			int cy = static_cast<int>(ceil(y));
//			// fractional part
//			float ty = y - fy;
//			float tx = x - fx;
//			// set interpolation weights
//			float w1 = (1 - tx) * (1 - ty);
//			float w2 = tx * (1 - ty);
//			float w3 = (1 - tx) * ty;
//			float w4 = tx * ty;
//			// iterate through your data
//			for (int i = radius; i < src.rows - radius; i++) {
//				for (int j = radius; j < src.cols - radius; j++) {
//					// calculate interpolated value
//					float t = static_cast<float>(w1 * src.at < _Tp > (i + fy, j + fx) + w2 * src.at < _Tp > (i + fy, j + cx) + w3 * src.at < _Tp > (i + cy, j + fx) + w4 * src.at < _Tp > (i + cy, j + cx));
//					// floating point precision, so check some machine-dependent epsilon
//					dst.at<int>(i - radius, j - radius) += ((t > src.at < _Tp > (i, j)) || (std::abs(t - src.at < _Tp > (i, j)) < std::numeric_limits<float>::epsilon())) << n;
//				}
//			}
//		}
//	}
//
//	static void elbp(InputArray src, OutputArray dst, int radius, int neighbors) {
//		int type = src.type();
//		switch (type) {
//			case CV_8SC1:
//				elbp_<char>(src, dst, radius, neighbors);
//				break;
//			case CV_8UC1:
//				elbp_<unsigned char>(src, dst, radius, neighbors);
//				break;
//			case CV_16SC1:
//				elbp_<short>(src, dst, radius, neighbors);
//				break;
//			case CV_16UC1:
//				elbp_<unsigned short>(src, dst, radius, neighbors);
//				break;
//			case CV_32SC1:
//				elbp_<int>(src, dst, radius, neighbors);
//				break;
//			case CV_32FC1:
//				elbp_<float>(src, dst, radius, neighbors);
//				break;
//			case CV_64FC1:
//				elbp_<double>(src, dst, radius, neighbors);
//				break;
//			default:
//				string error_msg = format("Using Original Local Binary Patterns for feature extraction only works on single-channel images (given %d). Please pass the image data as a grayscale image!", type);
//				CV_Error(CV_StsNotImplemented, error_msg);
//				break;
//		}
//	}
//
//	static Mat histc_(const Mat& src, int minVal = 0, int maxVal = 255, bool normed = false) {
//		Mat result;
//		// Establish the number of bins.
//		int histSize = maxVal - minVal + 1;
//		// Set the ranges.
//		float range[] = { static_cast<float>(minVal), static_cast<float>(maxVal + 1) };
//		const float* histRange = { range };
//		// calc histogram
//		calcHist(&src, 1, 0, Mat(), result, 1, &histSize, &histRange, true, false);
//		// normalize
//		if (normed) {
//			result /= (int) src.total();
//		}
//		return result.reshape(1, 1);
//	}
//
//	static Mat histc(InputArray _src, int minVal, int maxVal, bool normed) {
//		Mat src = _src.getMat();
//		switch (src.type()) {
//			case CV_8SC1:
//				return histc_(Mat_<float>(src), minVal, maxVal, normed);
//				break;
//			case CV_8UC1:
//				return histc_(src, minVal, maxVal, normed);
//				break;
//			case CV_16SC1:
//				return histc_(Mat_<float>(src), minVal, maxVal, normed);
//				break;
//			case CV_16UC1:
//				return histc_(src, minVal, maxVal, normed);
//				break;
//			case CV_32SC1:
//				return histc_(Mat_<float>(src), minVal, maxVal, normed);
//				break;
//			case CV_32FC1:
//				return histc_(src, minVal, maxVal, normed);
//				break;
//			default:
//				CV_Error(CV_StsUnmatchedFormats, "This type is not implemented yet.");
//				break;
//		}
//		return Mat();
//	}
//
//	static Mat spatial_histogram(InputArray _src, int numPatterns, int grid_x, int grid_y, bool /*normed*/) {
//		Mat src = _src.getMat();
//		// calculate LBP patch size
//		int width = src.cols / grid_x;
//		int height = src.rows / grid_y;
//		// allocate memory for the spatial histogram
//		Mat result = Mat::zeros(grid_x * grid_y, numPatterns, CV_32FC1);
//		// return matrix with zeros if no data was given
//		if (src.empty())
//			return result.reshape(1, 1);
//		// initial result_row
//		int resultRowIdx = 0;
//		// iterate through grid
//		for (int i = 0; i < grid_y; i++) {
//			for (int j = 0; j < grid_x; j++) {
//				Mat src_cell = Mat(src, Range(i * height, (i + 1) * height), Range(j * width, (j + 1) * width));
//				Mat cell_hist = histc(src_cell, 0, (numPatterns - 1), true);
//				// copy to the result matrix
//				Mat result_row = result.row(resultRowIdx);
//				cell_hist.reshape(1, 1).convertTo(result_row, CV_32FC1);
//				// increase row count in result matrix
//				resultRowIdx++;
//			}
//		}
//		// return result as reshaped feature vector
//		return result.reshape(1, 1);
//	}
//
////------------------------------------------------------------------------------
//// wrapper to cv::elbp (extended local binary patterns)
////------------------------------------------------------------------------------
//
//	static Mat elbp(InputArray src, int radius, int neighbors) {
//		Mat dst;
//		elbp(src, dst, radius, neighbors);
//		return dst;
//	}
//
//	void LBPH::load(const FileStorage& fs) {
//		fs["radius"] >> _radius;
//		fs["neighbors"] >> _neighbors;
//		fs["grid_x"] >> _grid_x;
//		fs["grid_y"] >> _grid_y;
//		//read matrices
//		readFileNodeList(fs["histograms"], _histograms);
//		fs["labels"] >> _labels;
//	}
//
//// See FaceRecognizer::save.
//	void LBPH::save(FileStorage& fs) const {
//		fs << "radius" << _radius;
//		fs << "neighbors" << _neighbors;
//		fs << "grid_x" << _grid_x;
//		fs << "grid_y" << _grid_y;
//		// write matrices
//		writeFileNodeList(fs, "histograms", _histograms);
//		fs << "labels" << _labels;
//	}
//
//	void LBPH::train(InputArrayOfArrays _in_src, InputArray _in_labels) {
//		this->train(_in_src, _in_labels, false);
//	}
//
//	void LBPH::update(InputArrayOfArrays _in_src, InputArray _in_labels) {
//		// got no data, just return
//		if (_in_src.total() == 0)
//			return;
//
//		this->train(_in_src, _in_labels, true);
//	}
//
//	void LBPH::train(InputArrayOfArrays _in_src, InputArray _in_labels, bool preserveData) {
//		if (_in_src.kind() != _InputArray::STD_VECTOR_MAT && _in_src.kind() != _InputArray::STD_VECTOR_VECTOR) {
//			string error_message = "The images are expected as InputArray::STD_VECTOR_MAT (a std::vector<Mat>) or _InputArray::STD_VECTOR_VECTOR (a std::vector< vector<...> >).";
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		if (_in_src.total() == 0) {
//			string error_message = format("Empty training data was given. You'll need more than one sample to learn a model.");
//			CV_Error(CV_StsUnsupportedFormat, error_message);
//		} else if (_in_labels.getMat().type() != CV_32SC1) {
//			string error_message = format("Labels must be given as integer (CV_32SC1). Expected %d, but was %d.", CV_32SC1, _in_labels.type());
//			CV_Error(CV_StsUnsupportedFormat, error_message);
//		}
//		// get the vector of matrices
//		vector < Mat > src;
//		_in_src.getMatVector(src);
//		// get the label matrix
//		Mat labels = _in_labels.getMat();
//		// check if data is well- aligned
//		if (labels.total() != src.size()) {
//			string error_message = format("The number of samples (src) must equal the number of labels (labels). Was len(samples)=%d, len(labels)=%d.", src.size(), _labels.total());
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		// if this model should be trained without preserving old data, delete old model data
//		if (!preserveData) {
//			_labels.release();
//			_histograms.clear();
//		}
//		// append labels to _labels matrix
//		for (size_t labelIdx = 0; labelIdx < labels.total(); labelIdx++) {
//			_labels.push_back(labels.at<int>((int) labelIdx));
//		}
//		// store the spatial histograms of the original data
//		for (size_t sampleIdx = 0; sampleIdx < src.size(); sampleIdx++) {
//			// calculate lbp image
//			Mat lbp_image = elbp(src[sampleIdx], _radius, _neighbors);
//			// get spatial histogram from this lbp image
//			Mat p = spatial_histogram(lbp_image, /* lbp_image */
//			static_cast<int>(std::pow(2.0, static_cast<double>(_neighbors))), /* number of possible patterns */
//			_grid_x, /* grid size x */
//			_grid_y, /* grid size y */
//			true);
//			// add to templates
//			_histograms.push_back(p);
//		}
//	}
//
//	void LBPH::predict(InputArray _src, int &minClass, double &minDist) const {
//		if (_histograms.empty()) {
//			// throw error if no data (or simply return -1?)
//			string error_message = "This LBPH model is not computed yet. Did you call the train method?";
//			CV_Error(CV_StsBadArg, error_message);
//		}
//		Mat src = _src.getMat();
//		// get the spatial histogram from input image
//		Mat lbp_image = elbp(src, _radius, _neighbors);
//		Mat query = spatial_histogram(lbp_image, /* lbp_image */
//		static_cast<int>(std::pow(2.0, static_cast<double>(_neighbors))), /* number of possible patterns */
//		_grid_x, /* grid size x */
//		_grid_y, /* grid size y */
//		true /* normed histograms */);
//		// find 1-nearest neighbor
//		minDist = DBL_MAX;
//		minClass = -1;
//		for (size_t sampleIdx = 0; sampleIdx < _histograms.size(); sampleIdx++) {
//			double dist = compareHist(_histograms[sampleIdx], query, CV_COMP_CHISQR);
//			if ((dist < minDist) && (dist < _threshold)) {
//				minDist = dist;
//				minClass = _labels.at<int>((int) sampleIdx);
//			}
//		}
//	}
//
//	int LBPH::predict(InputArray _src) const {
//		int label;
//		double dummy;
//		predict(_src, label, dummy);
//		return label;
//	}
//
//	Ptr<FaceRecognizer> createEigenFaceRecognizer(int num_components, double threshold) {
//		return new Eigenfaces(num_components, threshold);
//	}
//
//	Ptr<FaceRecognizer> createFisherFaceRecognizer(int num_components, double threshold) {
//		return new Fisherfaces(num_components, threshold);
//	}
//
//	Ptr<FaceRecognizer> createLBPHFaceRecognizer(int radius, int neighbors, int grid_x, int grid_y, double threshold) {
//		return new LBPH(radius, neighbors, grid_x, grid_y, threshold);
//	}
//
//	CV_INIT_ALGORITHM(Eigenfaces, "FaceRecognizer.Eigenfaces",
//			obj.info()->addParam(obj, "ncomponents", obj._num_components);
//			obj.info()->addParam(obj, "threshold", obj._threshold);
//			obj.info()->addParam(obj, "projections", obj._projections, true);
//			obj.info()->addParam(obj, "labels", obj._labels, true);
//			obj.info()->addParam(obj, "eigenvectors", obj._eigenvectors, true);
//			obj.info()->addParam(obj, "eigenvalues", obj._eigenvalues, true);
//			obj.info()->addParam(obj, "mean", obj._mean, true));
//
//	CV_INIT_ALGORITHM(Fisherfaces, "FaceRecognizer.Fisherfaces",
//			obj.info()->addParam(obj, "ncomponents", obj._num_components);
//			obj.info()->addParam(obj, "threshold", obj._threshold);
//			obj.info()->addParam(obj, "projections", obj._projections, true);
//			obj.info()->addParam(obj, "labels", obj._labels, true);
//			obj.info()->addParam(obj, "eigenvectors", obj._eigenvectors, true);
//			obj.info()->addParam(obj, "eigenvalues", obj._eigenvalues, true);
//			obj.info()->addParam(obj, "mean", obj._mean, true));
//
//	CV_INIT_ALGORITHM(LBPH, "FaceRecognizer.LBPH",
//			obj.info()->addParam(obj, "radius", obj._radius);
//			obj.info()->addParam(obj, "neighbors", obj._neighbors);
//			obj.info()->addParam(obj, "grid_x", obj._grid_x);
//			obj.info()->addParam(obj, "grid_y", obj._grid_y);
//			obj.info()->addParam(obj, "threshold", obj._threshold);
//			obj.info()->addParam(obj, "histograms", obj._histograms, true);
//			obj.info()->addParam(obj, "labels", obj._labels, true));
//
//	bool initModule_contrib() {
//		Ptr<Algorithm> efaces = createEigenfaces(), ffaces = createFisherfaces(), lbph = createLBPH();
//		return efaces->info() != 0 && ffaces->info() != 0 && lbph->info() != 0;
//	}
//
//}

}

#endif /* EIGENFACESEXTRACTOR_H_ */
