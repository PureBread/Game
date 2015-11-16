#pragma once

#include <Markers.h>
#include <FileUtils.h>
#include <Log.h>

Marker::Marker(Json::Value _json){
	position = _json["position"].asInt();
	event = _json["event"].asString();
	Json::Value coloursJson = _json["colours"];
	for(auto layer = 0; layer < coloursJson.size(); ++layer){
		for(Json::Value::ArrayIndex channel = 0; channel < 3; ++channel){
			coloursReplaceWhite[layer][channel] = coloursJson[layer][0][channel].asFloat();
			coloursReplaceBlack[layer][channel] = coloursJson[layer][1][channel].asFloat();
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
		// next marker triggered
		sweet::Event * event = new sweet::Event("destination");
		event->setStringData("scenario", markers.at(currentMarker).event);
		eventManager.triggerEvent(event);

		++currentMarker;
		if(currentMarker >= markers.size()){
			currentMarker = markers.size()-1;
		}
		nextMarker = markers.at(currentMarker).position;
	}
	if(currentMarker > 0 && currentPosition < markers.at(currentMarker-1).position){
		// previous marker triggered
		--currentMarker;
		if(currentMarker > markers.size()){
			currentMarker = 0;
		}
		nextMarker = markers.at(currentMarker).position;
	}
	if(currentPosition < 0){
		currentPosition = 0;
	}
	unsigned long int pm = currentMarker <= 0 ? currentMarker : (currentMarker - 1);
	float d = (pm == currentMarker) ? 0 : ((nextMarker - currentPosition) / (nextMarker - markers.at(currentMarker-1).position));
	std::cout << d << std::endl;
	for(unsigned long int i = 0; i < NUM_LAYERS; ++i){
		coloursReplaceBlack[i] = markers.at(currentMarker).coloursReplaceBlack[i] + (markers.at(pm).coloursReplaceBlack[i] - markers.at(currentMarker).coloursReplaceBlack[i]) * d;
		coloursReplaceWhite[i] = markers.at(currentMarker).coloursReplaceWhite[i] + (markers.at(pm).coloursReplaceWhite[i] - markers.at(currentMarker).coloursReplaceWhite[i]) * d;
	}

	eventManager.update(_step);
}