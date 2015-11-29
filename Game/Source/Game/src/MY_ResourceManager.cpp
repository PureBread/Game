#pragma once

#include <MY_ResourceManager.h>

Scenario * MY_ResourceManager::scenario = nullptr;
std::vector<Scenario *> MY_ResourceManager::randomEvents;
std::vector<Scenario *> MY_ResourceManager::lossEvents;
std::vector<Scenario *> MY_ResourceManager::destinationEvents;

void MY_ResourceManager::init(){
	Json::Value root;
	Json::Reader reader;
	std::string jsonLoaded = FileUtils::readFile("assets/events/scenarioListing.json");
	bool parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages()/* + "\n" + jsonLoaded*/);
	}else{
		 Json::Value randomEventsJson = root["randomEvents"];
		 for(Json::Value::ArrayIndex i = 0; i < randomEventsJson.size(); ++i) {
			 randomEvents.push_back(new Scenario("assets/events/random/" + randomEventsJson[i].asString() + ".json"));
		 }
		 Json::Value lossEventsJson = root["lossEvents"];
		 for(Json::Value::ArrayIndex i = 0; i < lossEventsJson.size(); ++i) {
			 lossEvents.push_back(new Scenario("assets/events/loss/" + lossEventsJson[i].asString() + ".json"));
		 }
		 Json::Value destinationEventsJson = root["destinationEvents"];
		 for(Json::Value::ArrayIndex i = 0; i < destinationEventsJson.size(); ++i) {
			 destinationEvents.push_back(new Scenario("assets/events/destination/" + destinationEventsJson[i].asString() + ".json"));
		 }
	}

	scenario = new Scenario("assets/scenarioGlobal.json");
	resources.push_back(scenario);
}