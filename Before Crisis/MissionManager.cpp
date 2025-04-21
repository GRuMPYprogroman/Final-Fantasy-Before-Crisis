#include "MissionManager.h"

MissionManager::MissionManager()
{
    missions.push_back({ "Defeat the Goblin King", false, 100 });
    missions.push_back({ "Explore the Forest", false, 50 });
}

void MissionManager::AddMission(const Mission& mission)
{
    missions.push_back(mission);
}

void MissionManager::CompleteMission(const std::string& missionName)
{
	for (auto& mission : missions)
	{
		if (mission.name == missionName)
		{
			mission.completed = true;
			break;
		}
	}
}

std::vector<Mission> MissionManager::GetActiveMissions() const {
	std::vector<Mission> active;
	for (const auto& mission : missions) {
		if (!mission.completed) active.push_back(mission);
	}
	return active;
}