#pragma execution_character_set("UTF-8")
#include "player.h"
#include "ScriptPCH.h"
#include "World.h"

class player_Log : public PlayerScript
{
public:

	player_Log() : PlayerScript("player_Log") { }

	const char* getPlayerClass(uint32 playerClass)
	{
		switch (playerClass)
		{
		case CLASS_WARRIOR: return "|CFF9d9d9d战士|r";
		case CLASS_PALADIN: return "|CFFffffff圣骑士|r";
		case CLASS_HUNTER: return "|CFF1eff00猎人|r";
		case CLASS_ROGUE: return "|CFF0070dd盗贼|r";
		case CLASS_PRIEST: return "|CFFa335ee牧师|r";
		case CLASS_DEATH_KNIGHT: return "|CFFff8000死亡骑士|r";
		case CLASS_SHAMAN: return "|CFFe6cc80萨满祭司|r";
		case CLASS_MAGE: return "|CFFe5cc80法师|r";
		case CLASS_WARLOCK: return "|CFFeFFF80术士|r";
		case CLASS_DRUID: return "|CCCe5cc80德鲁伊|r";
		default: return "[未知职业]";
		}
	}

	const char* getPlayerTeam(uint32 playerTeam)
	{
		switch (playerTeam)
		{
		case TEAM_ALLIANCE:return "|cFF0070d0联盟|r";
		case TEAM_HORDE:return "|cFF0070d0部落|r";
		default: return "[未知阵营]";
		}
	}

	std::string getPlayerInfo(Player* player)
	{
		std::ostringstream ss;
		// 获取玩家名字
		std::string name = player->GetName();
		// 获取种族字符代码
		uint8 playerClassNum = player->getClass();
		// 获取玩家种族字符串
		std::string playerClass = getPlayerClass(playerClassNum);
		// 获取玩家阵营字符代码
		uint8 playeTeamNum = player->GetTeamId();
		// 获取玩家阵营字符
		std::string playerTeam = getPlayerTeam(playeTeamNum);
		ss << "【" << playerTeam << "】" << playerClass << " 玩家【|cFF00FF00" << name << "|r】";
		return ss.str();
	}

	void OnLogin(Player* player, bool firstLogin/*firstLogin*/) override
	{
		uint8 playerGender = player->getGender();
		if (firstLogin)
		{
			if (GENDER_MALE == playerGender)
			{
				std::string message = "|cFFFF0000[系统公告]|r小鲜肉" + getPlayerInfo(player) + "|cFFFF0000首次|r来到来到最有爱的游戏模拟器，大家用最热烈的掌声欢迎他!";
				sWorld->SendServerMessage(SERVER_MSG_STRING, message.c_str());
			}
			else
			{
				std::string message = "|cFFFF0000[系统公告]|r萌妹子" + getPlayerInfo(player) + "|cFFFF0000首次|r来到来到最有爱的游戏模拟器，大家用最热烈的掌声欢迎他!";
				sWorld->SendServerMessage(SERVER_MSG_STRING, message.c_str());
			}
		}
		else
		{
			if (GENDER_MALE == playerGender)
			{
				if (SEC_ADMINISTRATOR == player->GetSession()->GetSecurity())
				{
					;
				}
				else
				{
					std::string message = "|cFFFF0000[系统公告]|r小鲜肉" + getPlayerInfo(player) + "再次回到最有爱的游戏模拟器，大家用最热烈的掌声欢迎他!";
					sWorld->SendServerMessage(SERVER_MSG_STRING, message.c_str());
				}
			}
			else
			{
				if (SEC_ADMINISTRATOR == player->GetSession()->GetSecurity())
				{
					;
				}
				else
				{
					std::string message = "|cFFFF0000[系统公告]|r萌妹子" + getPlayerInfo(player) + "再次回到最有爱的游戏模拟器，大家用最热烈的掌声欢迎她!";
					sWorld->SendServerMessage(SERVER_MSG_STRING, message.c_str());
				}
			}
		}
	}
};

void AddSC_player_Log()
{
	new player_Log();
}