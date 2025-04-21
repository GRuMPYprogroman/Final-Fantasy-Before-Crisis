#pragma once
#include <string>
#include <vector>

struct Mission
{
	std::string name;
	bool completed;
	int rewardExp;
};

class IMissionManager
{
private:
	std::vector<Mission> missions;
public:
	virtual ~IMissionManager()  = default;
	virtual void AddMission(const Mission& mission) = 0;
	virtual void CompleteMission(const std::string& missionName) = 0;
	virtual std::vector<Mission> GetActiveMissions() const = 0;
};
