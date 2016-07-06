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
#include "MachinistCore\MachinistCore.h"
#include "WorldSession.h"

std::string green = "|cff00cc00";
std::string white = "|cffFFFFFF";
float ver = 2.00f;
uint8 VIP_MAX;
uint8 VIP_TP_BONUS;
uint32 VIP_UPGRADEITEM_ID;
uint8 VIP_CANUSETELEPORTLEVEL;
int VIP_TELEPORT_COST;
float VIP_OFFSET;
float VIP_MONEY_BONUS;

VIP::VIP() { }

VIP::~VIP()
{
}

void AnnounceLoggingToWorld(Player* player, uint32 type)
{
	std::string pName = player->GetName();
	uint32 acct_id = player->GetSession()->GetAccountId();
	uint8 PlayerLogInVip = VIP::GetVIP(acct_id);

	std::string msg = "|CFFFF0000[会员频道]|r[" + green + "VIP>>" + sMachinistCore->ConvertNumberToString(PlayerLogInVip) + "级玩家|r]: ";
	msg = msg + pName + green + " 踏着七彩祥云，";

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
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|   VIP Teleport GPS's : Loaded   |");

			VIP_MAX = sConfigMgr->GetIntDefault("VIP.MAX", 10);
			VIP_OFFSET = sConfigMgr->GetFloatDefault("VIP.OFFSET", 0.02f);
			VIP_TP_BONUS = sConfigMgr->GetIntDefault("VIP.TP_BONUS", 2);
			VIP_UPGRADEITEM_ID = sConfigMgr->GetIntDefault("VIP.UPGRADEITEM", 60007);
			VIP_CANUSETELEPORTLEVEL = sConfigMgr->GetIntDefault("VIP.CAN_USE_TELE", 5);
			VIP_TELEPORT_COST = sConfigMgr->GetIntDefault("VIP.TELE_COST", 2500);
			VIP_MONEY_BONUS = sConfigMgr->GetFloatDefault("VIP.LOOTMONEY_BONUS", 0.02f);

			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|    VIP MAX_VIP : %u", VIP_MAX);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|    VIP OFFSET : %f", VIP_OFFSET);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|     VIP TP BONUS:%u", VIP_TP_BONUS);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|     VIP MONEY BONUS:%f", VIP_MONEY_BONUS);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|     VIP USE TELE:%u", VIP_CANUSETELEPORTLEVEL);
			TC_LOG_INFO("server.loading", "___________________________________");
			TC_LOG_INFO("server.loading", "|  VIP Config  : Loaded           |");
			TC_LOG_INFO("server.loading", "|  MachinistVIP Engine  : Loaded  |");
			TC_LOG_INFO("server.loading", "|_________________________________|");
		};

};

// VIP最大等级
uint8 VIP::GetVIPMAX()
{
	return VIP_MAX;
};

// VIP装备收益倍率
float VIP::GetVIPOFFSET()
{
	return VIP_OFFSET;
};

// VIP天赋加成
uint8 VIP::GetTALENTBONUS()
{
	return VIP_TP_BONUS;
};

// VIP金币加成
float VIP::GetVIPLootMoneyBonus()
{
	return VIP_MONEY_BONUS;
}

// 获取物品vip等级
uint8 VIP::GetItemVIP(uint32 item_id)
{
	/*return ItemVip[item_id].vip;*/
	QueryResult VIPItemQery = WorldDatabase.PQuery("SELECT vip FROM item_template WHERE entry = %u;", item_id);

	if (!VIPItemQery)
	{
		return 0;
	}
	else
	{
		Field * vipItemdb = VIPItemQery->Fetch();
		uint8 itemVip = vipItemdb[0].GetInt8();
		return itemVip;
	}
};

// 设置瞬飞
void VIP::SetHearthStone(uint32 guid, uint32 map_id, float x, float y, float z, float o)
{
	WorldDatabase.PExecute("REPLACE INTO `hearthstone` SET `guid`='%u', `map_id`='%u', `x`='%f', `y`='%f', `z`='%f', `o`='%f';", guid, map_id, x, y, z, o);
}

// 获得玩家VIP
uint8 VIP::GetVIP(uint32 acct_id)
{
	//return Vip[acct_id].vip;
	QueryResult accVipResult = LoginDatabase.PQuery("SELECT vip FROM account WHERE id = %u", acct_id);

	if (!accVipResult)
	{
		return 0;
	}
	else
	{
		Field *accVIpdb = accVipResult->Fetch();
		uint8 accVip = accVIpdb[0].GetInt8();
		return accVip;
	}
};

void VIP::SetVIP(uint32 acct_id, uint8 pvip)
{
	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SET_VIP);
	stmt->setUInt32(0, pvip);
	stmt->setUInt32(1, acct_id);
	LoginDatabase.Execute(stmt);
};

// 获得玩家积分
int32 VIP::GetPlayerJf(uint32 acct_id)
{
	//return Vip[acct_id].jf;
	QueryResult accVipResult = LoginDatabase.PQuery("SELECT jf FROM account WHERE id = %u", acct_id);

	if (!accVipResult)
	{
		return 0;
	}
	else
	{
		Field *accJfdb = accVipResult->Fetch();
		int32 accJf = accJfdb[0].GetInt32();
		return accJf;
	}
};

void VIP::SetPlayerJf(uint32 acct_id, int32 jf)
{
	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SET_JF);
	stmt->setUInt32(0, jf);
	stmt->setUInt32(1, acct_id);
	LoginDatabase.Execute(stmt);
};


class Machinist_VIP_Player_Engine : public PlayerScript
{
public: Machinist_VIP_Player_Engine() : PlayerScript("Machinist_VIP_Player_Engine"){ };

		void OnLogin(Player* player, bool firstLogin) override
		{
			
			AnnounceLoggingToWorld(player, 1);

			uint32 guid = player->GetGUID().GetCounter();
			uint32 acct_id = player->GetSession()->GetAccountId();
			uint8 Pvip = VIP::GetVIP(acct_id);
			uint8 Tp_Bonus = VIP::GetTALENTBONUS();
			int32 jf = VIP::GetPlayerJf(acct_id);

			ChatHandler(player->GetSession()).PSendSysMessage("%s***********************************************************************", green.c_str());
			ChatHandler(player->GetSession()).PSendSysMessage("%s*欢迎您! 亲爱的玩家 %s%s\n%s*您当前VIP等级为: %s%u %s级\n%s*目前系统最高VIP等级为: %s%u %s级", green.c_str(), white.c_str(), player->GetName().c_str(), green.c_str(), white.c_str(), Pvip, green.c_str(), green.c_str(), white.c_str(), VIP_MAX, green.c_str());
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受装备属性加成为: %s%u%s", green.c_str(), white.c_str(), uint32(VIP_OFFSET * 100)*Pvip, "%");
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受天赋加成为: %s%u%s 点", green.c_str(), white.c_str(), uint32(Tp_Bonus)*Pvip, green.c_str());
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受金钱掉率加成为: %s%u%s", green.c_str(), white.c_str(), uint32(VIP_MONEY_BONUS * 100)*Pvip, "%");
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受强化成功概率提升: %s%u%s ", green.c_str(), white.c_str(), Pvip, "%");
			ChatHandler(player->GetSession()).PSendSysMessage("%s*您的帐号积分余额为: %s%u%s 点", green.c_str(), white.c_str(), jf, green.c_str());
			ChatHandler(player->GetSession()).PSendSysMessage("%s***********************************************************************", green.c_str());

			QueryResult gpsQery = WorldDatabase.PQuery("SELECT map_id, x, y, z, o FROM hearthstone WHERE guid = %u;", guid);

			if (!gpsQery)
			{
				WorldDatabase.PExecute("INSERT INTO hearthstone (guid, map_id, x, y, z, o) VALUES (%u, %u, %f, %f, %f, %f);", guid, player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
			}
		};

};

bool VIP::VipSetHearthstone(Player* player)
{

	uint32 acctId = player->GetSession()->GetAccountId();
	uint8 Pvip = VIP::GetVIP(acctId);


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

	VIP::SetHearthStone(player->GetGUID(), player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
	ChatHandler(player->GetSession()).PSendSysMessage("%s*****亲爱的玩家 %s%s%s , 您设置的地点已经保存*****", green.c_str(), white.c_str(), player->GetName(), green.c_str());

	return true;
}

bool VIP::VipHearthstone(Player* player)
{
	uint32 acctId = player->GetSession()->GetAccountId();
	uint8 Pvip = VIP::GetVIP(acctId);
	uint32 guid = player->GetGUID().GetCounter();

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


	QueryResult gpsQery = WorldDatabase.PQuery("SELECT map_id, x, y, z, o FROM hearthstone WHERE guid = %u;", guid);

	if (gpsQery)
	{
			Field* fields = gpsQery->Fetch();
			uint32 map_id = fields[0].GetUInt32();
			float x = fields[1].GetFloat();
			float y = fields[2].GetFloat();
			float z = fields[3].GetFloat();
			float o = fields[4].GetFloat();
			player->TeleportTo(map_id, x, y, z, o);
		return true;
	}
	return true;

}



void RemoveItem(uint32 itemEntry, Player* player, uint32 amount)
{
	uint32 guid = player->GetGUID().GetCounter();
	uint32 acct_id = player->GetSession()->GetAccountId();
	uint8 Pvip = VIP::GetVIP(acct_id);
	Pvip = Pvip + 1;
	uint8 Tp_Bonus = VIP::GetTALENTBONUS();
	player->DestroyItemCount(itemEntry, amount, true);

	ChatHandler(player->GetSession()).PSendSysMessage("%s恭喜您, VIP提升一级, 要继续努力哦, 再次上线生效!", green.c_str());
	ChatHandler(player->GetSession()).PSendSysMessage("%s***********************************************************************", green.c_str());
	ChatHandler(player->GetSession()).PSendSysMessage("%s*亲爱的玩家 %s%s\n%s*您当前VIP等级为: %s%u %s级\n%s*目前系统最高VIP等级为: %s%u %s级", green.c_str(), white.c_str(), player->GetName().c_str(), green.c_str(), white.c_str(), Pvip ,green.c_str(), green.c_str(), white.c_str(), VIP_MAX, green.c_str());
	ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受装备属性加成为: %s%u%s", green.c_str(), white.c_str(), uint32(VIP_OFFSET * 100)*Pvip, "%");
	ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受天赋加成为: %s%u%s 点", green.c_str(), white.c_str(), uint32(Tp_Bonus)*Pvip, green.c_str());
	ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受金钱掉率加成为: %s%u%s", green.c_str(), white.c_str(), uint32(VIP_MONEY_BONUS * 100)*Pvip, "%");
	ChatHandler(player->GetSession()).PSendSysMessage("%s*您当前享受强化成功概率提升: %s%u%s ", green.c_str(), white.c_str(), Pvip, "%");
	ChatHandler(player->GetSession()).PSendSysMessage("%s***********************************************************************", green.c_str());

};

class VIP_Stone_Script : public ItemScript
{
public: VIP_Stone_Script() : ItemScript("VIP_Stone_Script"){ };


		bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
		{
			uint32 acct_id = player->GetSession()->GetAccountId();
			uint8 pVip = VIP::GetVIP(acct_id);
			uint8 upgradeItemCount = pVip + 1;
			int32 jf = VIP::GetPlayerJf(acct_id);

			// 获取消耗物品链接
			WorldSession* session = player->GetSession();
			std::string VIPUpgradeLink = sMachinistCore->GetItemLink(VIP_UPGRADEITEM_ID, session);

			// 拼提示字符
			std::string message_1 = "|cff00cc00提升VIP等级需要消耗: \n" + VIPUpgradeLink + " x " + sMachinistCore->ConvertNumberToString(upgradeItemCount) + "\n|cff00cc00你确定这么操作么?";

			std::string message_2 = "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ************ " + sMachinistCore->ConvertNumberToString(pVip) + "级  ************";

			std::string message_3 = "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ********      " + sMachinistCore->ConvertNumberToString(jf) + "     ********";


			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ******尊敬的VIP玩家*******", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ******您当前VIP等级*******", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, message_2, acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******账户积分数量*******", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, message_3, acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", acct_id, GOSSIP_ACTION_INFO_DEF);

			if (VIP_MAX > pVip)
			{
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ********提升VIP等级********", acct_id, GOSSIP_ACTION_INFO_DEF + 1, message_1, 0, false);
			}
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ********设置顺飞地点*******", acct_id, GOSSIP_ACTION_INFO_DEF + 2);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ******顺飞消耗25银币*****", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM_EXTENDED(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ********传送顺飞地点*******", acct_id, GOSSIP_ACTION_INFO_DEF + 3, "使用顺飞功能需要消耗1点积分\n你确定要这么操作么?", 0, false);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ****10积分兑换10金币****", acct_id, GOSSIP_ACTION_INFO_DEF + 4);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 ***100积分兑换100金币***", acct_id, GOSSIP_ACTION_INFO_DEF + 5);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *1000积分兑换1000金币*", acct_id, GOSSIP_ACTION_INFO_DEF + 6);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 *******************************", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/INV_Misc_Book_11:28:28:-18:0|t|cFF330066 **********关闭菜单**********", acct_id, GOSSIP_ACTION_INFO_DEF);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());

			return false;
		}

		bool OnGossipSelect(Player* player, Item* item, uint32 acct_id, uint32 action) override
		{
			player->PlayerTalkClass->ClearMenus();

			uint8 pVip = VIP::GetVIP(acct_id);
			uint8 upgradeItemCount = pVip + 1;
			int32 jf = VIP::GetPlayerJf(acct_id);

			WorldSession* session = player->GetSession();
			std::string VIPUpgradeLink = sMachinistCore->GetItemLink(VIP_UPGRADEITEM_ID, session);
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
					VIP::SetVIP(acct_id, upgradeItemCount);
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
					int32 jfc = jf - 10;
					VIP::SetPlayerJf(acct_id,  jfc);
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
					int32 jfc = jf - 100;
					VIP::SetPlayerJf(acct_id, jfc);
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
					int32 jfc = jf - 1000;
					VIP::SetPlayerJf(acct_id, jfc);
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
	new Machinist_VIP_Player_Engine;
	new VIP_Stone_Script;
}