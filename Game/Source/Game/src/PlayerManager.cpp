#pragma once

#include <PlayerManager.h>
#include <json\json.h>
#include <FileUtils.h>
#include <Log.h>
#include <Step.h>

PlayerManager::PlayerManager() :
	momentDelay(0.5f),
	momentTimer(0)
{
	loadDefaults();
}

PlayerManager::~PlayerManager(){

}

void PlayerManager::save(const std::string & _path){
	Json::Value jsonStatistics;
	for(auto k : statistics){
		jsonStatistics[k.first] = k.second;
	}	
	Json::Value saveFile;
	saveFile["statistics"] = jsonStatistics;
	std::ofstream log("data/"+_path, std::ios_base::out);
	log << saveFile;
	log.close();
}

void PlayerManager::load(const std::string & _path){
	Json::Value root;
	Json::Reader reader;
	std::string jsonLoaded = FileUtils::readFile("data/" + _path);
	bool parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		 Json::Value resourcesJson = root["statistics"];
		 Json::Value::Members keys = resourcesJson.getMemberNames();
		 for(auto key : keys) {
			 statistics[key] = resourcesJson[key].asFloat();
		 }
	}
}

void PlayerManager::loadDefaults(){
	load("../assets/statisticsDefaults.json");
}

void PlayerManager::update(Step * _step){
	// increase the timer
	momentTimer += _step->deltaTime;
	
	// call moment only when enough time has passed
	while(momentTimer > momentDelay){
		moment();
		momentTimer -= momentDelay;
	}
}

void PlayerManager::moment(){
	// update statistics
	statistics["progress"] += statistics["speed"];
	statistics["wool"] += statistics["herdSize"];
	if(statistics["food"] <= 0){
		statistics["rations"] = 0;
	}else{
		statistics["food"] -= statistics["rations"] * statistics["herdSize"];
	}
	statistics["health"] += (statistics["rations"]-1)*1.5f - (statistics["speed"] - 1);

	// TODO: check for events
	//randomEventCheck();
	//lossEventCheck();
	//destinationEventCheck();
}