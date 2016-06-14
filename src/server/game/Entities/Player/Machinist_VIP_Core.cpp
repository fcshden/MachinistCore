/*
	机械师VIP系统
	完成： 100%
*/﻿

#include "AccountMgr.h"
#include "chat.h"
#include "Config.h"
#include "Group.h"
#include "Machinist_VIP_Core.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "player.h"
#include "RBAC.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include <unordered_map>
#include "World.h"
#pragma execution_character_set("UTF-8")
#include "../../scripts/Custom/resetRandomItem.h"
#include "WorldSession.h"

// color definitions since i hate yellow..
std::string green = "|cff00cc00";
std::string red = "|cffFF0000";
std::string white = "|cffFFFFFF";
std::string blue = "|cff3333FF";
std::string black = "|cff000000";

float ver = 1.00f;

uint32 VIP_MAX;
uint32 VIP_TP_BONUS;
uint32 VIP_UPGRADEITEM_ID;
uint32 VIP_JF_COUNT;
uint32 VIP_CANUSETELEPORTLEVEL;
int VIP_TELEPORT_COST;
float VIP_OFFSET;

std::unordered_map<uint32, VipElements> Vip;
std::unordered_map<uint32, ItemVIP> ItemVip;
std::unordered_map<uint32, VipHearthStoneGPS> HearthStone;


VIP::VIP() { }

VIP::~VIP()
{
}

std::string ConvertNumberToString(uint64 numberX)
{
	auto number = numberX;
	std::stringstream convert;
	std::string number32_to_string;
	convert << number;
	number32_to_string = convert.str();

	return number32_to_string;
};

void AnnounceLoggingToWorld(Player* player, uint32 type)
{
	std::string pName = player->GetName();
	uint32 acct_id = player->GetSession()->GetAccountId();
	uint32 PlayerLogInVip = VIP::GetVIP(acct_id);

	std::string msg = "|CFFFF0000[会员频道]|r[" + green + "VIP>>" + ConvertNumberToString(PlayerLogInVip) + "级玩家|r]: ";
	msg = msg + pName + green + " 踏着七彩祥云，";

	if (type == 0) { msg = msg + "离开游戏服务器|r"; };
	if (type == 1) { msg = msg + "进入游戏服务器|r"; };

	if (PlayerLogInVip > 0)
	{
		sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str());
	}
};

class VIP_Load_Conf : public WorldScript
{
public: VIP_Load_Conf() : WorldScript("VIP_Load_Conf"){ };

		virtual void OnConfigLoad(bool /*reload*/)
		{
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "| Machinist VIP  Engine : Loading |");
			TC_LOG_INFO("server.loading", "|__________By_  机械师  __________|");
			TC_LOG_INFO("server.loading", "|_________________________________|");
			TC_LOG_INFO("server.loading", "|-                               -|");
			TC_LOG_INFO("server.loading", "|____________Ver:%.1f_____________|", ver);

			QueryResult VIPItemQery = WorldDatabase.Query("SELECT entry, vip FROM item_template;");

			if (VIPItemQery)
			{
				do
				{
					Field* fields = VIPItemQery->Fetch();
					uint32 item_id = fields[0].GetUInt32();
					uint32 vip = fields[1].GetUInt32();

					ItemVIP& data1 = ItemVip[item_id];
					// Save the DB values to the MyData object
					data1.item_id = item_id;
					data1.vip = vip;

				} while (VIPItemQery->NextRow());
			}

			QueryResult gpsQery = WorldDatabase.Query("SELECT * FROM hearthstone;");

			if (gpsQery)
			{
				do
				{
					// unpacks the results of `result` into fields and appoint data to variable.
					Field* fields = gpsQery->Fetch();
					uint32 guid = fields[0].GetUInt32();
					uint32 map_id = fields[1].GetUInt32();
					float x = fields[2].GetFloat();
					float y = fields[3].GetFloat();
					float z = fields[4].GetFloat();
					float o = fields[5].GetFloat();

					VipHearthStoneGPS& data3 = HearthStone[guid];
					// Save the DB values to the MyData object
					data3.guid = guid;
					data3.map_id = map_id;
					data3.x = x;
					data3.y = y;
					data3.z = z;
					data3.o = o;

				} while (gpsQery->NextRow());
			}

			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|   VIP Teleport GPS's : Loaded   |");

			VIP_MAX = sConfigMgr->GetIntDefault("VIP.MAX", 10);
			VIP_OFFSET = sConfigMgr->GetFloatDefault("VIP.OFFSET", 0.02f);
			VIP_STONE_ID = sConfigMgr->GetIntDefault("VIP.STONE", 60008);
			VIP_TP_BONUS = sConfigMgr->GetIntDefault("VIP.TP_BONUS", 2);
			VIP_CANUSETELEPORTLEVEL = sConfigMgr->GetIntDefault("VIP.CAN_USE_TELE", 5);
			VIP_TELEPORT_COST = sConfigMgr->GetIntDefault("VIP.TELE_COST", 2500);

			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|    VIP MAX_VIP : %u", VIP_MAX);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|    VIP OFFSET : %f", VIP_OFFSET);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|     VIP TP BONUS:%u", VIP_TP_BONUS);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|     VIP USE TELE:%u", VIP_CANUSETELEPORTLEVEL);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|  VIP Config  : Loaded           |");
			TC_LOG_INFO("server.loading", "|  MachinistVIP Engine  : Loaded  |");
			TC_LOG_INFO("server.loading", "|_________________________________|");
		};

};

// VIP最大等级
uint32 VIP::GetVIPMAX()
{
	return VIP_MAX;
};

// VIP收益倍率
float VIP::GetVIPOFFSET()
{
	return VIP_OFFSET;
};

// VIP天赋加成
uint32 VIP::GetTALENTBONUS()
{
	return VIP_TP_BONUS;
};

// 设置瞬飞
void VIP::SetHearthStone(uint32 guid, uint32 map_id, float x, float y, float z, float o)
{
	WorldDatabase.PExecute("REPLACE INTO `hearthstone` SET `guid`='%u', `map_id`='%u', `x`='%f', `y`='%f', `z`='%f', `o`='%f';", guid, map_id, x, y, z, o);

	VipHearthStoneGPS& data = HearthStone[guid];
	// Save the DB values to the MyData object
	data.guid = guid;
	data.map_id = map_id;
	data.x = x;
	data.y = y;
	data.z = z;
	data.o = o;
}

// 获得玩家VIP
uint32 VIP::GetVIP(uint32 acct_id)
{
	return Vip[acct_id].vip;
};

void VIP::SetVIP(uint32 acct_id, uint32 pvip)
{
	Vip[acct_id].vip = pvip;

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SET_VIP);
	stmt->setUInt32(0, pvip);
	stmt->setUInt32(1, acct_id);
	LoginDatabase.Execute(stmt);
};

// 获得玩家积分
uint32 VIP::GetPlayerJf(uint32 acct_id)
{
	return Vip[acct_id].jf;
};

void VIP::SetPlayerJf(uint32 acct_id, uint32 jf)
{
	Vip[acct_id].jf = jf;

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SET_JF);
	stmt->setUInt32(0, jf);
	stmt->setUInt32(1, acct_id);
	LoginDatabase.Execute(stmt);

};



uint32 VIP::GetItemVIP(uint32 item_id)
{
	return ItemVip[item_id].vip;
};

void VIP::SetItemVIP(uint32 item_id, uint32 item_vip)
{
	if (item_vip < 1) { item_vip = 1; };
	if (item_vip > VIP_MAX) { item_vip = VIP_MAX; };

	PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SET_ITEM_VIP);
	stmt->setUInt8(0, item_vip);
	stmt->setUInt32(1, item_id);
	WorldDatabase.Execute(stmt);

	ItemVip[item_id].vip = item_vip;
};


class Machinist_VIP_Account_Engine : public AccountScript
{
public: Machinist_VIP_Account_Engine() : AccountScript("Machinist_VIP_Account_Engine"){ };

		void OnAccountLogin(uint32 accountId) override
		{
			if (accountId > 0)
			{
				PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_LOAD_VIP);
				stmt->setUInt32(0, accountId);
				PreparedQueryResult result = LoginDatabase.Query(stmt);

				if (!result)
				{
					TC_LOG_INFO("server.loading", "XX ERROR Loading a VIP table ID %u XX", accountId);
				};

				if (result)
				{
					// unpacks the results of `result` into fields and appoint data to variable.
					Field* fields = result->Fetch();
					uint32 pvip = fields[0].GetUInt32();
					uint32 pjf = fields[1].GetUInt32();

					VipElements& data = Vip[accountId]; // like Lua table VIP[acctId].vip
					// Save the DB values to the MyData object
					data.vip = pvip;
					data.jf = pjf;
					VIP::SetVIP(accountId, pvip);

					TC_LOG_INFO("server.loading", "ACCOUNT::LOGIN ID:%u VIP:%u", accountId, Vip[accountId].vip);
				}
			}
		}
};

class Machinist_VIP_Player_Engine : public PlayerScript
{
public: Machinist_VIP_Player_Engine() : PlayerScript("Machinist_VIP_Player_Engine"){ };

		void OnLogout(Player* player) override
		{
			AnnounceLoggingToWorld(player, 0);
		};

		void OnLogin(Player* player, bool firstLogin) override
		{
			AnnounceLoggingToWorld(player, 1);

			uint32 guid = player->GetGUID();
			uint32 acct_id = player->GetSession()->GetAccountId();
			uint32 Pvip = VIP::GetVIP(acct_id);
			uint32 Tp_Bonus = VIP::GetTALENTBONUS();
			uint32 jf = VIP::GetPlayerJf(acct_id);

			ChatHandler(player->GetSession()).PSendSysMessage("%s***********************************************************************", green.c_str());
			ChatHandler(player->GetSession()).PSendSysMessage("%s*欢迎您! 亲爱的玩家 %s%s\n%s*您当前VIP等级为: %s%u %s级\n%s*目前系统最高VIP等级为: %s%u %s级", green.c_str(), white.c_str(), player->GetName().c_str(), green.c_str(), white.c_str(), Vip[acct_id].vip, green.c_str(), green.c_str(), white.c_str(), VIP_MAX, green.c_str());
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受装备属性加成为: %s%u%s", green.c_str(), white.c_str(), uint32(VIP_OFFSET * 100)*Pvip, "%");
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受天赋加成为: %s%u%s 点", green.c_str(), white.c_str(), uint32(Tp_Bonus)*Pvip, green.c_str());
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受强化成功概率提升: %s%u%s ", green.c_str(), white.c_str(), Pvip, "%");
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您的帐号积分余额为: %s%u%s 点", green.c_str(), white.c_str(), jf, green.c_str());
			ChatHandler(player->GetSession()).PSendSysMessage("%s***********************************************************************", green.c_str());
			if (HearthStone[guid].guid != guid)
			{
				VIP::SetHearthStone(guid, player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
			}
		};
};

bool VIP::VipSetHearthstone(Player* player)
{

	uint32 acctId = player->GetSession()->GetAccountId();
	uint32 Pvip = VIP::GetVIP(acctId);


	if (player->IsInCombat())
	{
		ChatHandler(player->GetSession()).PSendSysMessage(LANG_YOU_IN_COMBAT);
		return false;
	}

	if (VIP_CANUSETELEPORTLEVEL > Pvip)
	{
		ChatHandler(player->GetSession()).PSendSysMessage("%s*****亲爱的玩家, 您的VIP等级小于%s%u%s级, 无法使用顺飞功能*****", green.c_str(), white.c_str(), VIP_CANUSETELEPORTLEVEL, green.c_str());
		return false;
	}

	// stop flight if need
	if (player->IsInFlight())
	{
		player->GetMotionMaster()->MovementExpired();
		player->CleanupAfterTaxiFlight();
	}
	// save only in non-flight case
	else
		player->SaveRecallPosition();

	VIP::SetHearthStone(player->GetGUID(), player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
	ChatHandler(player->GetSession()).PSendSysMessage("%s*****亲爱的玩家 %s%s%s , 您设置的地点已经保存*****", green.c_str(), white.c_str(), player->GetName(), green.c_str());

	return true;
}

bool VIP::VipHearthstone(Player* player)
{
	uint32 acctId = player->GetSession()->GetAccountId();
	uint32 Pvip = VIP::GetVIP(acctId);
	uint32 guid = player->GetGUID();

	if (player->IsInCombat())
	{
		ChatHandler(player->GetSession()).PSendSysMessage(LANG_YOU_IN_COMBAT);
		return false;
	}

	if (VIP_CANUSETELEPORTLEVEL > Pvip)
	{
		ChatHandler(player->GetSession()).PSendSysMessage("%s*****亲爱的玩家, 您的VIP等级小于%s%u%s级, 无法使用顺飞功能*****", green.c_str(), white.c_str(), VIP_CANUSETELEPORTLEVEL, green.c_str());
		return false;
	}

	// stop flight if need
	if (player->IsInFlight())
	{
		player->GetMotionMaster()->MovementExpired();
		player->CleanupAfterTaxiFlight();
	}

	if (HearthStone[guid].guid != guid)
	{
		WorldDatabase.PExecute("REPLACE INTO hearthstone SET `guid`='%u';", guid);

		VIP::SetHearthStone(guid, player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());

		ChatHandler(player->GetSession()).PSendSysMessage("%s*****亲爱的玩家, 你需要设定一个顺飞的地点, 才可以使用该功能哦!*****", green.c_str());
		ChatHandler(player->GetSession()).PSendSysMessage("%s*****使用当前地点为顺飞目的地, 点击顺飞按钮, 即可传送至当前位置*****", green.c_str());
		return false;
	}

	if (HearthStone[guid].guid == guid)
	{
		// save only in non-flight case and a location is stored
		player->SaveRecallPosition();

		player->TeleportTo(HearthStone[guid].map_id, HearthStone[guid].x, HearthStone[guid].y, HearthStone[guid].z, HearthStone[guid].o);
		return true;
	}
	return true;

}



void RemoveItem(uint32 itemEntry, Player* player, uint32 amount)
{
	uint32 guid = player->GetGUID();
	uint32 acct_id = player->GetSession()->GetAccountId();
	uint32 Pvip = VIP::GetVIP(acct_id);
	uint32 Tp_Bonus = VIP::GetTALENTBONUS();
	player->DestroyItemCount(itemEntry, amount, true);

	ChatHandler(player->GetSession()).PSendSysMessage("%s恭喜您, VIP提升一级, 要继续努力哦, 再次上线生效!", green.c_str());
	ChatHandler(player->GetSession()).PSendSysMessage("%s***********************************************************************", green.c_str());
	ChatHandler(player->GetSession()).PSendSysMessage("%s*亲爱的玩家 %s%s\n%s*您当前VIP等级为: %s%u %s级\n%s*目前系统最高VIP等级为: %s%u %s级", green.c_str(), white.c_str(), player->GetName().c_str(), green.c_str(), white.c_str(), Vip[acct_id].vip, green.c_str(), green.c_str(), white.c_str(), VIP_MAX, green.c_str());
	ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受装备属性加成为: %s%u%s", green.c_str(), white.c_str(), uint32(VIP_OFFSET * 100)*Pvip, "%");
	ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受天赋加成为: %s%u%s 点", green.c_str(), white.c_str(), uint32(Tp_Bonus)*Pvip, green.c_str());
	ChatHandler(player->GetSession()).PSendSysMessage("%s***********************************************************************", green.c_str());

};

class VIP_Stone_Script : public ItemScript
{
public: VIP_Stone_Script() : ItemScript("VIP_Stone_Script"){ };


		bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
		{
			uint32 acct_id = player->GetSession()->GetAccountId();
			uint32 pVip = VIP::GetVIP(acct_id);
			uint32 upgradeItemCount = pVip + 1;
			uint32 jf = VIP::GetPlayerJf(acct_id);

			// 获取消耗物品链接
			WorldSession* session = player->GetSession();
			std::string VIPUpgradeLink = sResetRandomItem->GetItemLink(VIP_UPGRADEITEM_ID, session);

			// 拼提示字符
			std::string message_1 = "|cff00cc00提升VIP等级需要消耗: \n" + VIPUpgradeLink + " x " + ConvertNumberToString(upgradeItemCount) + "\n|cff00cc00你确定这么操作么?";

			std::string message_2 = "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ************ " + ConvertNumberToString(pVip) + "级  ************";

			std::string message_3 = "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ********      " + ConvertNumberToString(jf) + "     ********";


			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ******尊敬的VIP玩家*******", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ******您当前VIP等级*******", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, message_2, VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******账户积分数量*******", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, message_3, VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);

			if (VIP_MAX > pVip)
			{
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ********提升VIP等级********", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF + 1, message_1, 0, false);
			}
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ********设置顺飞地点*******", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF + 2);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ******顺飞消耗25银币*****", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM_EXTENDED(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ********传送顺飞地点*******", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF + 3, "使用顺飞功能需要消耗1点积分\n你确定要这么操作么?", 0, false);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ****10积分兑换10金币****", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF + 4);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ***100积分兑换100金币***", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF + 5);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *1000积分兑换1000金币*", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF + 6);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 **********关闭菜单**********", VIP_UPGRADEITEM_ID, GOSSIP_ACTION_INFO_DEF);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());

			return false;
		}

		bool OnGossipSelect(Player* player, Item* item, uint32 VIP_UPGRADEITEM_ID, uint32 action) override
		{
			player->PlayerTalkClass->ClearMenus();

			uint32 acct_id = player->GetSession()->GetAccountId();
			uint32 pVip = VIP::GetVIP(acct_id);
			uint32 upgradeItemCount = pVip + 1;
			uint32 jf = VIP::GetPlayerJf(acct_id);

			WorldSession* session = player->GetSession();
			std::string VIPUpgradeLink = sResetRandomItem->GetItemLink(VIP_UPGRADEITEM_ID, session);
			switch (action)
			{
			case GOSSIP_ACTION_INFO_DEF:
			{
				player->CLOSE_GOSSIP_MENU();

			}break;

			case GOSSIP_ACTION_INFO_DEF + 1:
			{
				if (player->HasItemCount(VIP_UPGRADEITEM_ID, upgradeItemCount))
				{
					VIP::SetVIP(acct_id, pVip + 1);
					RemoveItem(VIP_UPGRADEITEM_ID, player, upgradeItemCount);
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("您没有足够%s, 无法提升VIP等级!", VIPUpgradeLink);
				}
			}break;

			case GOSSIP_ACTION_INFO_DEF + 2:
			{
				VIP::VipSetHearthstone(player);
				player->CLOSE_GOSSIP_MENU();

			}break;

			case GOSSIP_ACTION_INFO_DEF + 3:
			{
				if (player->HasEnoughMoney(VIP_TELEPORT_COST))
				{
					player->ModifyMoney(-VIP_TELEPORT_COST);
					ChatHandler(player->GetSession()).PSendSysMessage("扣除%u铜币, 顺飞至保存地点!", VIP_TELEPORT_COST);
					VIP::VipHearthstone(player);
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("您没有足够的货币使用该功能!");
					player->CLOSE_GOSSIP_MENU();
				}
			}break;

			case GOSSIP_ACTION_INFO_DEF + 4:
			{
				if (jf >= 10)
				{
					VIP::SetPlayerJf(acct_id, jf - 10);
					player->ModifyMoney(100000);
					player->CLOSE_GOSSIP_MENU();
					ChatHandler(player->GetSession()).PSendSysMessage("扣除10点积分, 获得10个金币!");
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("您没有足够的积分使用该功能!");
					player->CLOSE_GOSSIP_MENU();
				}
			}break;

			case GOSSIP_ACTION_INFO_DEF + 5:
			{
				if (jf >= 100)
				{
					VIP::SetPlayerJf(acct_id, jf - 100);
					player->ModifyMoney(1000000);
					player->CLOSE_GOSSIP_MENU();
					ChatHandler(player->GetSession()).PSendSysMessage("扣除100点积分, 获得100个金币!");
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("您没有足够的积分使用该功能!");
					player->CLOSE_GOSSIP_MENU();
				}
			}break;

			case GOSSIP_ACTION_INFO_DEF + 6:
			{
				if (jf >= 1000)
				{
					VIP::SetPlayerJf(acct_id, jf - 1000);
					player->ModifyMoney(10000000);
					player->CLOSE_GOSSIP_MENU();
					ChatHandler(player->GetSession()).PSendSysMessage("扣除1000点积分, 获得1000个金币!");
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("您没有足够的积分使用该功能!");
					player->CLOSE_GOSSIP_MENU();
				}
			}break;

			}

			player->CLOSE_GOSSIP_MENU();
			return false;
		}
};


void AddSC_Machinist_VIP_Core()
{
	new VIP_Load_Conf;
	new Machinist_VIP_Account_Engine;
	new Machinist_VIP_Player_Engine;
	new VIP_Stone_Script;
}