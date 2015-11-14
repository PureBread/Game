#pragma once

#include <node\NodeUpdatable.h>
#include <map>


class PlayerManager : public NodeUpdatable{
public:
	std::map<std::string, float> statistics;


	// saves resources to a json file at data/_path
	void save(const std::string & _path);
	// loads resources from a json file at data/_path
	void load(const std::string & _path);
	// loads the default resources
	void loadDefaults();

	PlayerManager();
	~PlayerManager();

	virtual void update(Step * _step) override;


	float momentDelay;
	float momentTimer;

	void moment();
};