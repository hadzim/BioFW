#include "opencvGraphics.h"

namespace Plot {

typedef std::vector <Color> GlobalColors;
GlobalColors globalColors;


bool isAColor(int code){
	int transferCode = -code - 10;
	return transferCode >= 0 && transferCode < globalColors.size();
}

int color2Code(Color c){
	for (int i = 0; i < globalColors.size(); i++){
		if (globalColors[i] == c){
			return -10 - i;
		}
	}
	globalColors.push_back(c);
	int index = globalColors.size() - 1;
	return -10 - index;
}
Color code2Color(int code){
	int transferCode = -code - 10;
	return globalColors.at(transferCode);
}

}
