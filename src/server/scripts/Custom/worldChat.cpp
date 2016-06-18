#include "ScriptMgr.h"
#include "Player.h"
#include "WorldSession.h"
#include "Chat.h"
#pragma execution_character_set("UTF-8")
#define MSG_COLOR_BLUEVIOLET "|cFF8A2BE2"
const char* CLASS_ICON;
const char* RACE_ICON;

#define FACTION_SPECIFIC 0
#define CHAT_COST 100


std::string GetNameLink(Player* player)
{
	std::string name = player->GetName();
	std::string color;

	switch (player->getRace())
	{
		// Done - Bloodelf
	case RACE_BLOODELF:   // 血精灵
		if (player->getGender() == GENDER_MALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Bloodelf_Male:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Bloodelf_Female:15|t";
		break;
		// Done - Dranei
	case RACE_DRAENEI:   // 德莱尼
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Draenei_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Draenei_Male:15|t";
		break;
	case RACE_DWARF:   // 矮人
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Dwarf_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Dwarf_Male:15|t";
		break;
		// Done - Gnome
	case RACE_GNOME:  // 侏儒
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Gnome_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Gnome_Male:15|t";
		break;
		// Done - Human
	case RACE_HUMAN:  // 人类
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Human_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Human_Male:15|t";
		break;
	case RACE_NIGHTELF:  // 暗夜精灵
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Nightelf_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Nightelf_Male:15|t";
		break;
	case RACE_ORC:  // 兽人
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Orc_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Orc_Male:15|t";
		break;
		// Done - Tauren
	case RACE_TAUREN: // 牛头人
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Tauren_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Tauren_Male:15|t";
		break;
	case RACE_TROLL: // 巨魔
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Troll_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Troll_Male:15|t";
		break;
	case RACE_UNDEAD_PLAYER:  // 不死族
		if (player->getGender() == GENDER_FEMALE)
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Undead_Female:15|t";
		else
			RACE_ICON = "|TInterface/ICONS/Achievement_Character_Undead_Male:15|t";
		break;
	}

	switch (player->getClass())
	{
	case CLASS_DEATH_KNIGHT:  // 死亡骑士
		color = "|cffC41F3B";
		//CLASS_ICON = "|TInterface\\icons\\Spell_Deathknight_ClassIcon:15|t|r";
		break;
	case CLASS_DRUID:   // 德鲁伊
		color = "|cffFF7D0A";
		//CLASS_ICON = "|TInterface\\icons\\Ability_Druid_Maul:15|t|r";
		break;
	case CLASS_HUNTER:  // 猎人
		color = "|cffABD473";
		//CLASS_ICON = "|TInterface\\icons\\INV_Weapon_Bow_07:15|t|r";
		break;
	case CLASS_MAGE:   // 法师
		color = "|cff69CCF0";
		//CLASS_ICON = "|TInterface\\icons\\INV_Staff_13:15|t|r";
		break;
	case CLASS_PALADIN:   // 圣骑士
		color = "|cffF58CBA";
		//CLASS_ICON = "|TInterface\\icons\\INV_Hammer_01:15|t|r";
		break;
	case CLASS_PRIEST:  // 牧师
		color = "|cffFFFFFF";
		//CLASS_ICON = "|TInterface\\icons\\INV_Staff_30:15|t|r";
		break;
	case CLASS_ROGUE:  // 潜行者
		color = "|cffFFF569";
		//CLASS_ICON = "|TInterface\\icons\\INV_ThrowingKnife_04:15|t|r";
		break;
	case CLASS_SHAMAN:  // 萨满
		color = "|cff0070DE";
		//CLASS_ICON = "|TInterface\\icons\\Spell_Nature_BloodLust:15|t|r";
		break;
	case CLASS_WARLOCK: // 战士
		color = "|cff9482C9";
		//CLASS_ICON = "|TInterface\\icons\\Spell_Nature_FaerieFire:15|t|r";
		break;
	case CLASS_WARRIOR: // 术士
		color = "|cffC79C6E";
		//CLASS_ICON = "|TInterface\\icons\\INV_Sword_27.png:15|t|r";
		break;
	}
	return "|Hplayer:" + name + "|h" + RACE_ICON + "|cffFFFFFF【" + color + name + "|cffFFFFFF】|h|r";
}

class cs_world_chat : public CommandScript
{
public:
	cs_world_chat() : CommandScript("cs_world_chat") {}

	std::vector<ChatCommand> GetCommands() const override
	{
		static std::vector<ChatCommand> WorldChatCommandTable =
		{
			{ "chat", rbac::RBAC_PERM_COMMAND_CUSTOM_CHAT, false, &HandleWorldChatCommand, "" },
		};

		return WorldChatCommandTable;
	}

	static bool HandleWorldChatCommand(ChatHandler * handler, const char * args)
	{

		if (!handler->GetSession()->GetPlayer()->CanSpeak())
			return false;
		std::string temp = args;

		if (!args || temp.find_first_not_of(' ') == std::string::npos)
			return false;


		std::string msg = "";
		Player * player = handler->GetSession()->GetPlayer();

		switch (player->GetSession()->GetSecurity())
		{
			// Player
		case SEC_PLAYER:
			if (player->GetTeam() == ALLIANCE)
			{
				msg += "|cff00ff00[世界聊天]";
				msg += "|cfffa9900[联盟玩家]";
				msg += "|cff0000ff|TInterface\\pvpframe\\pvp-currency-alliance:15|t";
				msg += GetNameLink(player);
				msg += ": |cffffffff";
			}

			else //Horde
			{
				msg += "|cff00ff00[世界聊天]";
				msg += "|cfffa9900[部落玩家]";
				msg += "|cffff0000|TInterface\\pvpframe\\pvp-currency-horde:16|t";
				msg += GetNameLink(player);
				msg += ": |cffffffff";
			}
			break;
		case SEC_MODERATOR:
			msg += "|cff00ff00[世界聊天]";
			msg += "|cff00ffff[游戏管理员A1] ";
			msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
			msg += GetNameLink(player);
			msg += ": |cff00ccff";
			break;
			// GM
		case SEC_GAMEMASTER:
			msg += "|cff00ff00[世界聊天]";
			msg += "|cff00ffff[游戏管理员A2] ";
			msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
			msg += GetNameLink(player);
			msg += ": |cff00ccff";
			break;
			// Admin
		case SEC_ADMINISTRATOR:
			msg += "|cff00ff00[世界聊天]";
			msg += "|cfffa9900[游戏管理员A3] ";
			msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
			msg += GetNameLink(player);
			msg += ": |cff00ccff";

			break;

		}

		msg += args;

		if (FACTION_SPECIFIC)
		{
			if (player->HasEnoughMoney(CHAT_COST))
			{
				player->ModifyMoney(-CHAT_COST);
				SessionMap sessions = sWorld->GetAllSessions();
				for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
					if (Player* plr = itr->second->GetPlayer())
						if (plr->GetTeam() == player->GetTeam())
							sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), plr);
			}
			else
			{
				player->GetSession()->SendNotification("|CFFFF0000金币数量不足，暂时不能使用世界聊天给功能!");
				ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000金币数量不足，暂时不能使用世界聊天功能!");
			}
		}
		else
			if (player->HasEnoughMoney(CHAT_COST))
			{
				player->ModifyMoney(-CHAT_COST);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), 0);
			}
			else
			{
				player->GetSession()->SendNotification("|CFFFF0000金币数量不足，暂时不能使用世界聊天给功能!");
				ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000金币数量不足，暂时不能使用世界聊天功能!");
			}
		return true;

	}
};

class Player_WorldChat : public PlayerScript
{
public:
	Player_WorldChat() : PlayerScript("Player_WorldChat") { }

	void OnLogin(Player* player, bool /*firstLogin*/) override
	{
		ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000[系统提示]|r世界聊天频道已经开启, 每次聊天消耗1银币, 聊天命令为：.chat + 聊天内容!");
	}
};

void AddSC_cs_world_chat()
{
	new cs_world_chat;
	new Player_WorldChat();
}