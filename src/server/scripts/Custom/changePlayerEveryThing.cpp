/*

更改玩家相关信息

作者 : 机械师

*/

#include <map>
#include "ScriptMgr.h"	
#include "DisableMgr.h"
#include "Player.h"
#include "Spell.h"
#include "Player.h"
#include "Define.h"
#include "GossipDef.h"
#pragma execution_character_set("UTF-8")
#include "resetRandomItem.h"
#include "Config.h"

uint32 changePlayerInfoItemID = sConfigMgr->GetIntDefault("changePlayerInfoItemID", 60005);
uint32 changePlayerInfoItemCount = sConfigMgr->GetIntDefault("changePlayerInfoItemCount", 20);
uint32 changePlayerClassItemCount = sConfigMgr->GetIntDefault("changePlayerClassItemCount", 600);
// 数字转字符串的函数
std::string NumberToString(uint64 numberX)
{
	auto number = numberX;
	std::stringstream convert;
	std::string number32_to_string;
	convert << number;
	number32_to_string = convert.str();

	return number32_to_string;
};

// 根据职业返回技能ID值
uint32 getPlayerClassSpellId(uint8 playerClass)
{
	uint32 playerClassSpellId;
	switch (playerClass)
	{
	case CLASS_WARRIOR:
		playerClassSpellId = 200001;
		break;
	case CLASS_PALADIN:
		playerClassSpellId = 200002;
		break;
	case CLASS_HUNTER:
		playerClassSpellId = 200003;
		break;
	case CLASS_ROGUE:
		playerClassSpellId = 200004;
		break;
	case CLASS_PRIEST:
		playerClassSpellId = 200005;
		break;
	case CLASS_DEATH_KNIGHT:
		playerClassSpellId = 200006;
		break;
	case CLASS_SHAMAN:
		playerClassSpellId = 200007;
		break;
	case CLASS_MAGE:
		playerClassSpellId = 200008;
		break;
	case CLASS_WARLOCK:
		playerClassSpellId = 200009;
		break;
	case CLASS_DRUID:
		playerClassSpellId = 200010;
		break;
	}
	return playerClassSpellId;
};

void learnPlayerClassSpell(Player* player, uint8 playerClass)
{
	QueryResult res = WorldDatabase.PQuery("SELECT `SpellID` FROM `change_class_spell` WHERE `id` = '%u'", getPlayerClassSpellId(playerClass));
	if (res)
	{
		do
		{
			Field* pField = res->Fetch();
			uint32 spell_ID = pField[0].GetUInt32();
			if (player->HasSpell(spell_ID))
				continue;
			player->LearnSpell(spell_ID, false);
		} while (res->NextRow());
	}
};
void forgetPlayerClassSpell(Player* player)
{
	QueryResult res = WorldDatabase.PQuery("SELECT `SpellID` FROM `change_class_spell` WHERE `id` = '%u'", getPlayerClassSpellId(player->getClass()));
	if (res)
	{
		do
		{
			Field* pField = res->Fetch();
			uint32 spell_ID = pField[0].GetUInt32();
			if (!player->HasSpell(spell_ID))
				continue;
			player->RemoveSpell(spell_ID, false);
		} while (res->NextRow());
	}
};


void playerClassGossipShow(Player* player, uint8 playerClass)
{
	switch (playerClass)
	{
	case CLASS_WARRIOR:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Sword_27:30:30:-18:0|t |cFF330066 变更为战士职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11, "您确定变更为战士职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	case CLASS_PALADIN:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Hammer_01:30:30:-18:0|t |cFF330066 变更为圣骑职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12, "您确定变更为圣骑士职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	case CLASS_HUNTER:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Weapon_Bow_07:30:30:-18:0|t |cFF330066 变更为猎人职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13, "您确定变更为猎人职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	case CLASS_ROGUE:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_ThrowingKnife_04:30:30:-18:0|t |cFF330066 变更为盗贼职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14
			, "您确定变更为盗贼职业?\n\n  职业变更后需要大退再次上线成效! \n\n 请仔细阅读说明!", 0, false);
		break;
	case CLASS_PRIEST:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Staff_30:30:30:-18:0|t |cFF330066 变更为牧师职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15, "您确定变更为牧师职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	case CLASS_DEATH_KNIGHT:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\Spell_Deathknight_ClassIcon:30:30:-18:0|t |cFF330066 变更为死骑职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16, "您确定变更为死亡骑士职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	case CLASS_SHAMAN:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\Spell_Nature_BloodLust:30:30:-18:0|t |cFF330066 变更为萨满职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17, "您确定变更为萨满祭司职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	case CLASS_MAGE:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Staff_13:30:30:-18:0|t |cFF330066 变更为法师职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18, "您确定变更为法师职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	case CLASS_WARLOCK:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\Spell_Nature_FaerieFire:30:30:-18:0|t |cFF330066 变更为术士职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19, "您确定变更为术士职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	case CLASS_DRUID:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\Ability_Druid_Maul:30:30:-18:0|t |cFF330066 变更为小德职业", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20, "您确定变更为德鲁伊职业?\n\n  职业变更后需要大退再次上线成效! \n\n  请仔细阅读说明!", 0, false);
		break;
	}
};

class changePlayerInfo : public CreatureScript
{
public:
	changePlayerInfo() : CreatureScript("change_Player_info") { }

	bool OnGossipHello(Player* player, Creature* creature) override
	{
		if (player->IsInCombat())
		{
			player->GetSession()->SendNotification("您在战斗中哦, 不可以和我对话!");
			player->CLOSE_GOSSIP_MENU();
			return false;
		}
		WorldSession* session = player->GetSession();
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 变更角色姓名 " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerInfoItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "你确定需要变更姓名?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 变更角色外观 " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerInfoItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2, "你确定需要变更外观?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 变更角色阵营 " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerInfoItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3, "你确定需要变更阵营?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 变更角色种族 " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerInfoItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "你确定需要变更种族?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 变更角色职业 " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerClassItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5 ,"你确定需要变更种族?\n\n 只能变更本种族可以选择的职业! \n\n 并且脱掉所有装备 \n\n请阅读说明后谨慎选择! ", 0, false);
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
	{
		if (!player)
			return false;

		uint8 maxLevel = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);

		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
			if (player->HasItemCount(changePlayerInfoItemID, changePlayerInfoItemCount))
			{
				player->SetAtLoginFlag(AT_LOGIN_RENAME);
				player->DestroyItemCount(changePlayerInfoItemID, changePlayerInfoItemCount, true);
				player->GetSession()->SendNotification("变更成功!玩家返回到角色选择界面即可修改角色名称!");
				ChatHandler(player->GetSession()).SendSysMessage("变更成功!玩家返回到角色选择界面即可修改角色名称!");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendNotification("所需物品数量不足不能使用该功能!");
				ChatHandler(player->GetSession()).SendSysMessage("所需物品数量不足不能使用该功能!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case GOSSIP_ACTION_INFO_DEF + 2:
			if (player->HasItemCount(changePlayerInfoItemID, changePlayerInfoItemCount))
			{
				player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
				player->DestroyItemCount(changePlayerInfoItemID, changePlayerInfoItemCount, true);
				player->GetSession()->SendNotification("变更成功!玩家返回到角色选择界面即可修改角色外观!");
				ChatHandler(player->GetSession()).SendSysMessage("变更成功!玩家返回到角色选择界面即可修改角色外观!");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendNotification("所需物品数量不足不能使用该功能!");
				ChatHandler(player->GetSession()).SendSysMessage("所需物品数量不足不能使用该功能!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case GOSSIP_ACTION_INFO_DEF + 3:
			if (player->HasItemCount(changePlayerInfoItemID, changePlayerInfoItemCount))
			{
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
				player->DestroyItemCount(changePlayerInfoItemID, changePlayerInfoItemCount, true);
				player->GetSession()->SendNotification("变更成功!玩家返回到角色选择界面即可修改角色阵营!");
				ChatHandler(player->GetSession()).SendSysMessage("变更成功!玩家返回到角色选择界面即可修改角色阵营!");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendNotification("所需物品数量不足不能使用该功能!");
				ChatHandler(player->GetSession()).SendSysMessage("所需物品数量不足不能使用该功能!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case GOSSIP_ACTION_INFO_DEF + 4:
			if (player->HasItemCount(changePlayerInfoItemID, changePlayerClassItemCount))
			{
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
				player->DestroyItemCount(changePlayerInfoItemID, changePlayerInfoItemCount, true);
				player->GetSession()->SendNotification("变更成功!玩家返回到角色选择界面即可修改角色种族!");
				ChatHandler(player->GetSession()).SendSysMessage("变更成功!玩家返回到角色选择界面即可修改角色种族!");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendNotification("所需物品数量不足不能使用该功能!");
				ChatHandler(player->GetSession()).SendSysMessage("所需物品数量不足不能使用该功能!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case GOSSIP_ACTION_INFO_DEF + 5:

			// 要求玩家脱掉所有装备
			for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
			{
				Item* playerEquip = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
					if (playerEquip)
					{
						player->GetSession()->SendNotification("变更职业需要脱掉所有装备, 请确认您的装备已经脱下!");
						ChatHandler(player->GetSession()).SendSysMessage("变更职业需要脱掉所有装备, 请确认您的装备已经脱下!");
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
			}

			if (player->HasItemCount(changePlayerInfoItemID, changePlayerInfoItemCount) && maxLevel == player->getLevel())
			{
				// 先重置天赋
				player->ResetTalents();
				// 遗忘所有职业技能
				forgetPlayerClassSpell(player);

				if (player->getRace() == RACE_HUMAN) // 人类 zs qs dz ms sq fs ss
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_PALADIN)
						playerClassGossipShow(player, CLASS_PALADIN);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				
				}
				else if (player->getRace() == RACE_ORC) // 兽人 zs lr dz sq sm ss
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_SHAMAN)
						playerClassGossipShow(player, CLASS_SHAMAN);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_DWARF) // 矮人 zs qs lr zd ms sq 
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_PALADIN)
						playerClassGossipShow(player, CLASS_PALADIN);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_NIGHTELF) // 暗夜精灵 zs lr zd ms sq xd
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_DRUID)
						playerClassGossipShow(player, CLASS_DRUID);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_UNDEAD_PLAYER) // 不死族 zs dz ms sq fs ss
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_TAUREN) // 牛头人 zs lr sq sm dx
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_SHAMAN)
						playerClassGossipShow(player, CLASS_SHAMAN);
					if (player->getClass() != CLASS_DRUID)
						playerClassGossipShow(player, CLASS_DRUID);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_GNOME) // 侏儒 zs dz sq fs ss 
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());

				}
				else if (player->getRace() == RACE_TROLL) // 巨魔 za lr zd ms sq sm fs
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_SHAMAN)
						playerClassGossipShow(player, CLASS_SHAMAN);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_BLOODELF) // 血精灵 qs lr zd ms sq fs ss
				{
					if (player->getClass() != CLASS_PALADIN)
						playerClassGossipShow(player, CLASS_PALADIN);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_DRAENEI) // 德莱尼 zs qs lr ms sq sm fs
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_PALADIN)
						playerClassGossipShow(player, CLASS_PALADIN);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_SHAMAN)
						playerClassGossipShow(player, CLASS_SHAMAN);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}

			}
			else
			{
				player->GetSession()->SendNotification("所需物品数量不足或者等级不足80级, 不能使用该功能!");
				ChatHandler(player->GetSession()).SendSysMessage("所需物品数量不足或者等级不足80级, 不能使用该功能!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;
		case GOSSIP_ACTION_INFO_DEF + 11: // 变更战士
			CharacterDatabase.PExecute("UPDATE characters SET class = 1 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_WARRIOR);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效");
			player->CLOSE_GOSSIP_MENU();				
			break;

		case GOSSIP_ACTION_INFO_DEF + 12: // 变更圣骑士
			CharacterDatabase.PExecute("UPDATE characters SET class = 2 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_PALADIN);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 13: // 变更猎人
			CharacterDatabase.PExecute("UPDATE characters SET class = 3 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_HUNTER);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 14: // 变更盗贼
			CharacterDatabase.PExecute("UPDATE characters SET class = 4 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_ROGUE);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 15: // 变更牧师
			CharacterDatabase.PExecute("UPDATE characters SET class = 5 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_PRIEST);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 16: // 变更死亡骑士
			CharacterDatabase.PExecute("UPDATE characters SET class = 6 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_DEATH_KNIGHT);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 17: // 变更萨满
			CharacterDatabase.PExecute("UPDATE characters SET class = 7 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_SHAMAN);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 18: // 变更法师
			CharacterDatabase.PExecute("UPDATE characters SET class = 8 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_MAGE);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 19: // 变更术士
			CharacterDatabase.PExecute("UPDATE characters SET class = 9 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_WARLOCK);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 20: // 变更德鲁伊
			CharacterDatabase.PExecute("UPDATE characters SET class = 11 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_DRUID);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("变更职业完毕, 请大退后再次上线生效!");
			ChatHandler(player->GetSession()).SendSysMessage("变更职业完毕, 请大退后再次上线生效!");
			player->CLOSE_GOSSIP_MENU();
			break;
		}

		return true;
	}

};



void AddSC_changePlayerInfo()
{
	new changePlayerInfo();
}

