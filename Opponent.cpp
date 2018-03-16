#include "stdafx.h"
#include "Opponent.h"
#include "Photoreceptor.h"

Opponent::Opponent() {

}

Opponent::Opponent(OpponentType ot) {
	oType = ot;
}

Opponent::Opponent(OpponentType ot) {
	oType = ot;
}

Opponent::Opponent(OpponentType ot, Point p) {
	oType = ot;
	location = p;
}

void Opponent::setCenterConnection(std::vector<Photoreceptor*> inputs){
	inputCenterConnectedCells = inputs;
}

void Opponent::setSurroundConnections(std::vector<Photoreceptor*> inputs){
	inputSurroundConnectedCells = inputs;
}


float Opponent::process(){
	return 0;
}

float Opponent::output(){
	return output;
}

Point Opponent::getPoint(){
	return location;
}

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
