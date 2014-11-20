/*
 * ResultsOverview.cpp
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#include "BioFW/Utils/ResultsOverview.h"
#include "BioFW/Evaluation/Statistics/BasicStatistics.h"
#include "kplot/opencvGraphics.h"
#include "kplot/PlotData.h"
#include "kplot/koolplot.h"

namespace BioFW {
	namespace Utils {

		static int cnt = 50;

		static int getBin(Statistics::BasicStatistics & s, double score){
			double minDiff = score - s.getGlobalExtremes().getMin();
			return (int)(minDiff * (cnt-1) / s.getGlobalExtremes().getDiff());
		}



		cv::Mat renderGenuineImpostorGraph(const MethodResults & r){


			std::vector <double> x;
			std::vector <double> g;
			std::vector <double> i;

			Statistics::BasicStatistics statistics(r);

			int cnt = 50;
			double binSize = statistics.getGlobalExtremes().getDiff() / cnt;

			for (int it = 0; it < cnt; it++){
				x.push_back(it * binSize);
				g.push_back(0);
				i.push_back(0);
			}
			for (auto score : r.getGenuineScores()){
				g[getBin(statistics, score.getScore())]++;
			}
			for (auto score : r.getImpostorScores()){
				i[getBin(statistics, score.getScore())]++;
			}

			for (int it = 0; it < cnt; it++){
				g[it] = g[it] * 100 / statistics.getGenuineCount();
				i[it] = i[it] * 100 / statistics.getImpostorCount();
			}


			Plot::plotdata xx(x);
			Plot::plotdata gg(g);


			Plot::setColor(xx,gg, RED);

			xx << x;
			gg << i;


			Plot::Plotstream ps(640,320);
			cv::Mat m = ps.plot(xx, gg);

			return m;

		}

	}
}
