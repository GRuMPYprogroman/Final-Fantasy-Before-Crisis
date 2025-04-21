#pragma once
#include <map>
#include <string>

class WorldState
{
private:
	std::map<std::string, int> variables;
	int careerPoints = 0;
	int rank = 3; // START AS 3RD CLASS
	void UpdateRank()
	{
		if (careerPoints >= 100 && rank > 1)
		{
			rank--;
			careerPoints = 0;
		}
			
	}
public:
	void SetVariable(const std::string& key, int value) { variables[key] = value; }
	int GetVariable(const std::string& key) const { return variables.at(key); }
	void AddCareerPoints(int points) { careerPoints += points; UpdateRank(); }
	int GetRank() const { return rank; }
};

