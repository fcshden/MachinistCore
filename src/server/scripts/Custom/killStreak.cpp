#include "TicketMgr.h"
#include "Common.h"
#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "WorldSession.h"
#include "Config.h"
#include "Chat.h"
#pragma execution_character_set("UTF-8")



struct SystemInfo
{
	uint32 KillStreak;
	uint32 LastGUIDKill;
	uint8 KillCount;
};

static std::map<uint32, SystemInfo> KillingStreak;

class killstreak_system : public PlayerScript
{
public:
	killstreak_system() : PlayerScript("killstreak_system") { }

	void OnPVPKill(Player* killer, Player* killed) override
	{

		bool PvPSystemEnabled = sConfigMgr->GetBoolDefault("PvPSystemEnabled", true);
		bool OnlyInBattlegrounds = sConfigMgr->GetBoolDefault("OnlyInBattlegrounds", true);
		uint8 HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim = sConfigMgr->GetIntDefault("HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim", 10);
		const uint8 KillerStreak1 = 1;
		const uint8 KillerStreak2 = 2;
		const uint8 KillerStreak3 = 3;
		const uint8 KillerStreak4 = 5;
		const uint8 KillerStreak5 = 10;
		int32 KillStreaks[5] = { KillerStreak1, KillerStreak2, KillerStreak3, KillerStreak4, KillerStreak5 };

		if (PvPSystemEnabled == false)
		{
			return;
		}
		else if (PvPSystemEnabled == true)
		{
			uint32 kGUID = killer->GetGUID();
			uint32 vGUID = killed->GetGUID();
			char msg[500];

			if (OnlyInBattlegrounds == true)
			{
				if (!killer->GetMap()->IsBattleground() || killer->GetMapId() == 30)
				{
					return;
				}
			}

			if (kGUID == vGUID)
			{
				return;
			}

			if (KillingStreak[kGUID].LastGUIDKill == vGUID)
			{
				KillingStreak[kGUID].KillCount++;
				KillingStreak[vGUID].KillCount = 1;
			}
			if (KillingStreak[kGUID].LastGUIDKill != vGUID)
			{
				KillingStreak[kGUID].KillCount = 1;
				KillingStreak[vGUID].KillCount = 1;
			}

			if (KillingStreak[kGUID].KillCount == HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim)
			{
				return;
			}
			KillingStreak[kGUID].KillStreak++;
			KillingStreak[vGUID].KillStreak = 0;
			KillingStreak[kGUID].LastGUIDKill = vGUID;
			KillingStreak[vGUID].LastGUIDKill = 0;

			switch (KillingStreak[kGUID].KillStreak)
			{
			case KillerStreak1:
				sprintf(msg, "|cffFF0000[战场频道]|r:玩家 |cff00FF00%s|r 瞬间带走了玩家 |cff00FF00%s|r |cff00FFFF 漂亮的完成一次单杀!|r", killer->GetName().c_str(), killed->GetName().c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				killer->CastSpell(killer, 24378, true);
				break;

			case KillerStreak2:
				sprintf(msg, "|cffFF0000[战场频道]|r:玩家 |cff00FF00%s|r 瞬间带走了玩家 |cff00FF00%s|r |cff00FFFF 漂亮的完成一次双杀!|r", killer->GetName().c_str(), killed->GetName().c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				killer->CastSpell(killer, 24378, true);
				break;

			case KillerStreak3:
				sprintf(msg, "|cffFF0000[战场频道]|r:玩家 |cff00FF00%s|r 瞬间带走了玩家 |cff00FF00%s|r |cff00FFFF 漂亮的完成一次三杀!|r", killer->GetName().c_str(), killed->GetName().c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				killer->CastSpell(killer, 24378, true);
				break;

			case KillerStreak4:
				sprintf(msg, "|cffFF0000[战场频道]|r: 玩家 |cff00FF00%s|r 瞬间带走了玩家 |cff00FF00%s|r |cff00FFFF 漂亮的完成一次五杀, 这货已经超神了, 赶紧来个人阻止他吧!|r", killer->GetName().c_str(), killed->GetName().c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				killer->CastSpell(killer, 24378, true);
				break;

			case KillerStreak5:
				sprintf(msg, "|cffFF0000[战场频道]|r:玩家 |cff00FF00%s|r 瞬间带走了玩家 |cff00FF00%s|r |cff00FFFF 漂亮的完成一次十杀, 这货已经嗜血成魔, 快来人终结他的杀戮吧!|r", killer->GetName().c_str(), killed->GetName().c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				killer->CastSpell(killer, 24378, true);
				break;

			}
		}
	}

	void OnLogout(Player* player) override
	{
		KillingStreak.erase(player->GetGUID());
	}
};

void AddSC_killstreak_system()
{
	new killstreak_system;
}