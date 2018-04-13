#include "stdafx.h"
#include "Opponent.h"
#include "Photoreceptor.h"
#include <vector>

const float Opponent::SURROUND_CONSTANT = 2.0f;

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
	double sum1 = 0;
	int sum1Size = 0;

	//Used For RedGreen and BlueYellow
	double sum2 = 0;
	int sum2Size = 0;

	for (Photoreceptor* p : inputCenterConnectedCells){
		if (this->ocType == OpponentChannelType::Luminance) {
			sum1 += p->getOutput();
			sum1Size++;
		}
		else {
			if (p->getType() == Photoreceptor::PhotoreceptorType::RedCone || (p->getType() == Photoreceptor::PhotoreceptorType::GreenCone && this->ocType == OpponentChannelType::BlueYellow)) {
				sum1 += p->getOutput();
				sum1Size++;
			}
			else {
				sum2 += p->getOutput();
				sum2Size++;
			}
		}
	}
	if(sum1Size != 0)
		sum1/= sum1Size;
	if (sum2Size != 0)
		sum2 /= sum2Size;

	double sub1 = 0;
	int sub1Size = 0;

	//Used For RedGreen and BlueYellow
	double sub2 = 0;
	int sub2Size = 0;

	for (Photoreceptor* p : inputSurroundConnectedCells) {
		if (this->ocType == OpponentChannelType::Luminance) {
			sub1 += p->getOutput();
			sub1Size++;
		}
		else {
			if (p->getType() == Photoreceptor::PhotoreceptorType::RedCone || (p->getType() == Photoreceptor::PhotoreceptorType::GreenCone && this->ocType == OpponentChannelType::BlueYellow)) {
				sub1 += p->getOutput();
				sub1Size++;
			}
			else {
				sub2 += p->getOutput();
				sub2Size++;
			}
		}
	}
	if (sub1Size != 0)
		sub1 /= sub1Size;
	if (sub2Size != 0)
		sub2 /= sub2Size;


	sub1 /= SURROUND_CONSTANT;
	sub2 /= SURROUND_CONSTANT;

	double channel1 = sum1 - sub1 > 0 ? sum1 - sub1 : 0;
	double channel2 = sum2 - sub2 > 0 ? sum2 - sub2 : 0;

	output = channel1 - channel2;
	return output;
}

double Opponent::getOutput() {
	return output;
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

double Opponent::getRange() {
	return 5.0 + sqrt(location.x * location.x + location.y *location.y) / 3.0;
}

bool Opponent::isCompatible(Photoreceptor* p) {
	if (p->getType() == Photoreceptor::PhotoreceptorType::BlueCone && this->ocType == OpponentChannelType::RedGreen)
		return false;
	return true;
}

void Opponent::resetConnections(){
	inputCenterConnectedCells.clear();
	inputSurroundConnectedCells.clear();
}
