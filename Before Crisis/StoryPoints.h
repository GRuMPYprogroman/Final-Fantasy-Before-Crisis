#pragma once
#include <map>
#include <string>
#include <fstream>

enum class story_flags_{Initial = 0, Fourth_Rank = 4, Third_rank = 3, Second_rank = 2,First_rank = 1,
	FirstClearedMission = 8, SecondClearedMission = 7,ThirdClearedMission = 6, OnBossDefeat = 5,
	number_of_flags = 9};

inline std::map<story_flags_, std::string> storyFiles = {
	{story_flags_::Initial, "../diaryNotes/initial.txt"},
	{story_flags_::Fourth_Rank, "../diaryNotes/FourthRank.txt"},
	{story_flags_::Third_rank, "../diaryNotes/ThirdRank.txt"},
	{story_flags_::Second_rank, "../diaryNotes/SecondRank.txt"},
	{story_flags_::First_rank, "../diaryNotes/FirstRank.txt"},
	{ story_flags_::FirstClearedMission, "../diaryNotes/OnFirstCleared.txt" },
	{ story_flags_::SecondClearedMission, "../diaryNotes/OnSecondCleared.txt" },
	{ story_flags_::ThirdClearedMission, "../diaryNotes/OnThirdCleared.txt" },
	{ story_flags_::OnBossDefeat, "../diaryNotes/OnBossDefeat.txt" }
};