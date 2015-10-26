#pragma once

#include <PlayerResources.h>
#include <json\json.h>
#include <FileUtils.h>
#include <Log.h>

PlayerResources::PlayerResources(){
	loadDefaults();
}

PlayerResources::~PlayerResources(){

}

void PlayerResources::save(const std::string & _path){
	Json::Value jsonResources;
	for(auto k : resources){
		jsonResources[k.first] = k.second;
	}	
	Json::Value saveFile;
	saveFile["resources"] = jsonResources;
	std::ofstream log("data/"+_path, std::ios_base::out);
	log << saveFile;
	log.close();
}

void PlayerResources::load(const std::string & _path){
	Json::Value root;
	Json::Reader reader;
	std::string jsonLoaded = FileUtils::readFile("data/" + _path);
	bool parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		 Json::Value resourcesJson = root["resources"];
		 Json::Value::Members keys = resourcesJson.getMemberNames();
		 for(auto key : keys) {
			 resources[key] = resourcesJson[key].asFloat();
		 }
	}
}

void PlayerResources::loadDefaults(){
	load("../assets/resourceDefaults.json");
}