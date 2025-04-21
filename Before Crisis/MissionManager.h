#pragma once
#include "IMissionManager.h"

class MissionManager : public IMissionManager
{
private:
	std::vector<Mission> missions;
public:
	MissionManager();
	void AddMission(const Mission& mission) override;
	void CompleteMission(const std::string& missionName) override;
	std::vector<Mission> GetActiveMissions() const override;
};

