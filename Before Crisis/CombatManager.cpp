#include "CombatManager.h"
#include <iostream>

/**void CombatManager::StartCombat(Character& player, Monster& enemy)
{
	while (player.GetStats().hp > 0 && enemy.GetHealth() > 0)
	{
		ui.DisplayText("Enemy: " + enemy.GetName() + " HP: " + std::to_string(enemy.GetHealth()));
		ui.DisplayText("1. Attack");
		if (ui.GetChoice(1,1) == 1)
		{
			enemy.TakeDamage(player.GetStats().strength);
			if (enemy.GetHealth() > 0)
			{
				player.TakeDamage(enemy.GetAttack());
			}
		}
	}
	ui.DisplayText(enemy.GetHealth() <= 0 ? "You win!" : "You lose!");
}**/