#include "stdafx.h"
#include "Opponent.h"
#include "Photoreceptor.h"
#include <vector>
#include "point.h"
#include "color.h"

Opponent::Opponent() {}

Opponent::Opponent(OpponentChannelType ot, OpponentFieldType oft){
	ocType = ot;
	ofType = oft;
}

Opponent::Opponent(OpponentChannelType ot, OpponentFieldType oft, Point p){
	ocType = ot;
	ofType = oft;
	location = p;
}

void Opponent::setCenterConnections(std::vector<Photoreceptor*> v){
	inputCenterConnectedCells = v;
}

void Opponent::setSurroundConnections(std::vector<Photoreceptor*> v) {
	inputSurroundConnectedCells = v;
};

double Opponent::process(){
	double sum = 0;
	for (Photoreceptor* p : inputCenterConnectedCells){
		sum += p->getOutput();
	}
	if(inputCenterConnectedCells.size() != 0)
		sum/= inputCenterConnectedCells.size();
		
	double subtract = 0;
	for (Photoreceptor* p : inputSurroundConnectedCells){
		subtract += p->getOutput();
	}
	if(inputSurroundConnectedCells.size() != 0)
		subtract/= inputSurroundConnectedCells.size();
	subtract/= SURROUND_CONSTANT;

	return sum - subtract;
}

Point Opponent::getPoint(){
	return location;
}

Opponent::OpponentChannelType Opponent::getChannelType(){
	return ocType;
}	

Opponent::OpponentFieldType Opponent::getFieldType(){
	return ofType;
}

void Opponent::resetConnections(){
	inputCenterConnectedCells.clear();
	inputSurroundConnectedCells.clear();
}
