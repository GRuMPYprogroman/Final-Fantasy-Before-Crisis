#pragma once
#include <string>
#include <vector>

struct Mission
{
	std::string name;
	bool completed;
	int rewardExp;
};

class MissionManager
{
private:
	std::vector<Mission> missions;
public:
	MissionManager();
	void AddMission(const Mission& mission);
	void CompleteMission(const std::string& missionName);
	std::vector<Mission> GetActiveMissions() const;
};

