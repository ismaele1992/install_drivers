#pragma once
#include "Logger.h"
class GraphicCard
{
	const char * classname = "GraphicCard";
	const char* reg_query = "";

public:
	GraphicCard();
	const char * GetModelGraphicCard();
	~GraphicCard();
};

