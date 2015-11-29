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
			 Scenario * s = new Scenario("assets/events/random/" + randomEventsJson[i].asString() + ".json");
			 randomEvents.push_back(s);
		 }
		 Json::Value lossEventsJson = root["lossEvents"];
		 for(Json::Value::ArrayIndex i = 0; i < lossEventsJson.size(); ++i) {
			 Scenario * s = new Scenario("assets/events/loss/" + lossEventsJson[i].asString() + ".json");
			 lossEvents.push_back(s);
		 }
		 Json::Value destinationEventsJson = root["destinationEvents"];
		 for(Json::Value::ArrayIndex i = 0; i < destinationEventsJson.size(); ++i) {
			 Scenario * s = new Scenario("assets/events/destination/" + destinationEventsJson[i].asString() + ".json");
			 destinationEvents.push_back(s);
		 }
	}

	scenario = new Scenario("assets/scenarioGlobal.json");
	resources.push_back(scenario);

}