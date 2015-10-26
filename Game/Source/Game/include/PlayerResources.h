#pragma once

#include <node\Node.h>
#include <map>


class PlayerResources : public Node{
public:
	std::map<std::string, float> resources;


	// saves resources to a json file at data/_path
	void save(const std::string & _path);
	// loads resources from a json file at data/_path
	void load(const std::string & _path);
	// loads the default resources
	void loadDefaults();

	PlayerResources();
	~PlayerResources();
};