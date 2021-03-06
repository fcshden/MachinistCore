/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// This is where scripts' loading functions should be declared:
void AddSC_Transmogrification();
void AddSC_Machinist_VIP_Core();
void AddSC_CS_ResetRandomItem();
void AddSC_itemUpgrade();
void AddSC_transportStone();
void AddSC_player_Log();
void AddSC_killstreak_system();
void AddSC_unBindItem();
void AddSC_cs_world_chat();
void AddSC_LearnAllSpellsOnLevelChanged();
void AddSC_clearCombat();
void AddSC_adam_War();
void AddSC_changePlayerInfo();
// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddCustomScripts()
{
	AddSC_Transmogrification();
	AddSC_Machinist_VIP_Core();
	AddSC_CS_ResetRandomItem();
	AddSC_itemUpgrade();
	AddSC_transportStone();
	AddSC_player_Log();
	AddSC_killstreak_system();
	AddSC_unBindItem();
	AddSC_cs_world_chat();
	AddSC_LearnAllSpellsOnLevelChanged();
	AddSC_clearCombat();
	AddSC_adam_War();
	AddSC_changePlayerInfo();
}
