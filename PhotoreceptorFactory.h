#pragma once
#include "stdafx.h"
#include <vector>
#include "point.h"
#include "photoreceptor.h"
#ifndef PHOTORECEPTORFACTORY
#define PHOTORECEPTORFACTORY


class PhotoreceptorFactory {

public:
	/**
		Generates all photoreceptors in the model

		@return The photoreceptors generated
	*/
	std::vector<Photoreceptor*> createPhotoreceptors();
};

#endif 