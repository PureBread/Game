#pragma once

#include <node\NodeUpdatable.h>
#include <vector>
#include <glm\glm.hpp>
#include <json\json.h>
#include <EventManager.h>

#define NUM_LAYERS 6

class Marker{
public:
	float position;
	std::string event;
	Marker(Json::Value _json);
	glm::vec3 coloursReplaceWhite[NUM_LAYERS];
	glm::vec3 coloursReplaceBlack[NUM_LAYERS];
};

class Markers : public virtual NodeUpdatable{
public:

	sweet::EventManager eventManager;

	unsigned long int currentMarker;
	float nextMarker;
	float currentPosition;
	std::vector<Marker> markers;
	glm::vec3 coloursReplaceWhite[NUM_LAYERS];
	glm::vec3 coloursReplaceBlack[NUM_LAYERS];

	Markers();
	virtual void update(Step * _step) override;
};