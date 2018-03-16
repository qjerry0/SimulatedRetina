#include "stdafx.h"
#include "Opponent.h"
#include "photoreceptor.h"
#include <vector>
#include "point.h"
#include "color.h"

class Opponent {
protected:
	float SURROUND_CONSTANT = 1;
	Point location;
	enum OpponentType {Luminance, RedGreen, BlueYellow};
	OpponentType oType;
	std::vector<Photoreceptor*> inputCenter;
	std::vector<Photoreceptor*> inputSurround;

public:

	Opponent(){
	}


	Opponent(OpponentType ot){
		oType = ot;
	}

	Opponent(OpponentType ot, Point p){
		oType = ot;
		location = p;
	}

	void setCenterConnections(std::vector<Photoreceptor*> v){
		inputCenter
	}

	void setSurroundConnections(std::vector<Photoreceptor*> v);

	float process(){
		float sum = 0;
		for (Photoreceptor &p : inputCenter){
			sum += p.process();
		}
		sum/= inputCenter.size();
		
		float subtract = 0;
		for (Photoreceptor &p : inputSurround){
			subtract += p.process();
		}
		subtract/= inputSurround.size();
		subtract/= SURROUND_CONSTANT;

		return sum - subtract;
	}

	Point getPoint(){
		return location;
	}

	void acquireInputCenter(std::vector<float> v){
		inputCenter = v;
	}
	
	void acquireInputSurround(std::vector<float> v){
		inputSurround = v;
	}

	void resetConnections();

	OpponentChannelType Opponent::getChannelType(){
		return ocType;
	}	

	OpponentFieldType Opponent::getFieldType(){
		return ofType;
	}

	void Opponent::resetConnections(){
		inputCenterConnectedCells = NULL;
		inputSurroundConnectedCells = NULL;
	}
}
