#pragma once

#include <Markers.h>
#include <FileUtils.h>
#include <Log.h>

Marker::Marker(Json::Value _json){
	position = _json["position"].asInt();
	Json::Value coloursJson = _json["colours"];

	for(auto layer = 0; layer < coloursJson.size(); ++layer){
		for(unsigned long int channel = 0; channel < 3; ++channel){
			coloursReplaceWhite[layer] = glm::vec3(coloursJson[layer][0][0].asFloat(), coloursJson[layer][0][1].asFloat(), coloursJson[layer][0][2].asFloat());
			coloursReplaceBlack[layer] = glm::vec3(coloursJson[layer][1][0].asFloat(), coloursJson[layer][1][1].asFloat(), coloursJson[layer][1][2].asFloat());
		}
	}
}

Markers::Markers() :
	currentPosition(0),
	currentMarker(0),
	nextMarker(0)
{
	Json::Value root;
	Json::Reader reader;
	std::string jsonLoaded = FileUtils::readFile("assets/levelMarkers.json");
	bool parsingSuccessful = reader.parse(jsonLoaded, root);
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		Json::Value markersJson = root["markers"];
		for(auto i = 0; i < markersJson.size(); ++i){
			markers.push_back(Marker(markersJson[i]));
		}
	}
}

void Markers::update(Step * _step){
	if(currentPosition > nextMarker){
		++currentMarker;
		if(currentMarker >= markers.size()){
			currentMarker = markers.size()-1;
		}
		nextMarker = markers.at(currentMarker).position;
	}if(currentPosition < markers.at(currentMarker).position){
		--currentMarker;
		if(currentMarker > markers.size()){
			currentMarker = 0;
		}
		nextMarker = markers.at(currentMarker).position;
	}
	if(currentPosition < 0){
		currentPosition = 0;
	}
	unsigned long int nm = currentMarker+1 >= markers.size() ? currentMarker : (currentMarker + 1);
	float d = (nm == currentMarker) ? 0 : ((currentPosition - markers.at(currentMarker).position) / (markers.at(nm).position - markers.at(currentMarker).position));

	for(unsigned long int i = 0; i < NUM_LAYERS; ++i){
		coloursReplaceBlack[i] = markers.at(currentMarker).coloursReplaceBlack[i] + (markers.at(nm).coloursReplaceBlack[i] - markers.at(currentMarker).coloursReplaceBlack[i]) * d;
		coloursReplaceWhite[i] = markers.at(currentMarker).coloursReplaceWhite[i] + (markers.at(nm).coloursReplaceWhite[i] - markers.at(currentMarker).coloursReplaceWhite[i]) * d;
	}
}