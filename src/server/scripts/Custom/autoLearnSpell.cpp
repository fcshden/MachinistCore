#include "ScriptMgr.h"
#include "DisableMgr.h"
#include "Player.h"
#include "Spell.h"
#include "SpellMgr.h"
#include <map>
#pragma execution_character_set("UTF-8")

uint32 getStarterTrainerIDForPlayer(Player* player)
{
	uint32 secondaryTrainerID;

	switch (player->getClass())
	{
	case CLASS_WARRIOR:
		secondaryTrainerID = 200001;
		break;
	case CLASS_PALADIN:
		secondaryTrainerID = 200003;
		break;
	case CLASS_HUNTER:
		secondaryTrainerID = 200013;
		break;
	case CLASS_ROGUE:
		secondaryTrainerID = 200015;
		break;
	case CLASS_PRIEST:
		secondaryTrainerID = 200011;
		break;
	case CLASS_SHAMAN:
		secondaryTrainerID = 200017;
		break;
	case CLASS_MAGE:
		secondaryTrainerID = 200007;
		break;
	case CLASS_WARLOCK:
		secondaryTrainerID = 200009;
		break;
	case CLASS_DRUID:
		secondaryTrainerID = 200005;
		break;
	}

	return secondaryTrainerID;
}

uint32 getTrainerIDForPlayer(Player* player)
{
	uint32 trainerID;

	switch (player->getClass())
	{
	case CLASS_WARRIOR:
		trainerID = 200002;
		break;
	case CLASS_PALADIN:
		trainerID = 200004;
		break;
	case CLASS_HUNTER:
		trainerID = 200014;
		break;
	case CLASS_ROGUE:
		trainerID = 200016;
		break;
	case CLASS_PRIEST:
		trainerID = 200012;
		break;
	case CLASS_SHAMAN:
		trainerID = 200018;
		break;
	case CLASS_MAGE:
		trainerID = 200008;
		break;
	case CLASS_WARLOCK:
		trainerID = 200010;
		break;
	case CLASS_DRUID:
		trainerID = 200006;
		break;
	case CLASS_DEATH_KNIGHT:
		trainerID = 200019;
		break;
	}

	return trainerID;
}

class LearnAllSpellsOnLevelChanged : public PlayerScript
{
public:
	LearnAllSpellsOnLevelChanged() : PlayerScript("LearnAllSpellsOnLevelChanged") {}

	void OnLevelChanged(Player* player, uint8 /*oldLevel*/) override
	{
		ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000[系统提示]|r自动学习职业技能系统启动:");
		LearnSpellsDependingOnLevel(player);
		player->SetHealth(player->GetMaxHealth());
	}

	void LearnSpellsDependingOnLevel(Player* player)
	{
		uint8 playerLevel = player->getLevel();
		QueryResult res = WorldDatabase.PQuery("SELECT `SpellID` FROM `npc_trainer` WHERE `id` = '%u' AND `ReqLevel` <= '%u'", getStarterTrainerIDForPlayer(player), playerLevel);
		if (res)
		{
			do
			{
				Field* pField = res->Fetch();
				uint32 spell_ID = pField[0].GetUInt32();

				SpellInfo const* sI = sSpellMgr->GetSpellInfo(spell_ID);
				SpellInfo const* prevSpell = sI->GetPrevRankSpell();

				if (prevSpell && !player->HasSpell(prevSpell->Id))
					continue;

				player->LearnSpell(spell_ID, false);

			} while (res->NextRow());
		}

		QueryResult res2 = WorldDatabase.PQuery("SELECT `SpellID` FROM `npc_trainer` WHERE `id` = '%u' AND `ReqLevel` <= '%u'", getTrainerIDForPlayer(player), playerLevel);
		if (res2)
		{
			do
			{
				Field* pField = res2->Fetch();
				uint32 spell_ID = pField[0].GetUInt32();


				SpellInfo const* sI = sSpellMgr->GetSpellInfo(spell_ID);
				SpellInfo const* prevSpell = sI->GetPrevRankSpell();

				if (prevSpell && !player->HasSpell(prevSpell->Id))
					continue;

				player->LearnSpell(spell_ID, false);

			} while (res2->NextRow());
		}
	}

};

void AddSC_LearnAllSpellsOnLevelChanged()
{
	new LearnAllSpellsOnLevelChanged();
}