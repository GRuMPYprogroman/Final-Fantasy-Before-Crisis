#pragma once
#include <map>
#include <string>
#include <fstream>

enum class story_flags_{Initial = 0, Third_rank = 3, Second_rank = 2,First_rank,Boss = 1, number_of_flags = 4};

inline std::map<story_flags_, std::string> storyFiles = {
	{story_flags_::Initial, "../diaryNotes/initial.txt"}
	//{story_flags_::Third_rank, "../diaryNotes/.txt"},
	//{story_flags_::Second_rank, "../diaryNotes/.txt"},
	//{story_flags_::First_rank, "../diaryNotes/.txt"},
	//{story_flags_::Boss, "../diaryNotes/.txt"},
};