#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"
#pragma execution_character_set("UTF-8")

class transportStone : public CreatureScript
{
public:
	transportStone() : CreatureScript("Npc_Transport") { }

	bool OnGossipHello(Player* player, Creature* creature) override
	{
		if (player->IsInCombat())
		{
			player->GetSession()->SendNotification("您在战斗中哦, 不可以和我对话!");
			player->CLOSE_GOSSIP_MENU();
			return false;
		}
		std::string message = "\n你确定使用传送功能?\n";
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_ZulDrak_03:30:30:-18:0|t |cFF330066 商业中心", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, message, 0, false);
		player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Achievement_Zone_ArathiHighlands_01:30:30:-18:0|t |cFF330066 联盟主城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
		player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Achievement_Zone_Ashenvale_01:30:30:-18:0|t |cFF330066 部落主城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 41);
		player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Achievement_Zone_Azshara_01:30:30:-18:0|t |cFF330066 中立城市", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 51);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_BoreanTundra_01:30:30:-18:0|t |cFF330066 诺兰德", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5, message, 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 奎尔单纳斯岛", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6, message, 0, false);
		player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Achievement_Zone_BoreanTundra_01:30:30:-18:0|t |cFF330066 艾泽拉斯大陆副本", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
		player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Achievement_Zone_BoreanTundra_02:30:30:-18:0|t |cFF330066 外域和诺兰德副本", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;

	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
	{
		if (!player)
			return false;

		player->PlayerTalkClass->ClearMenus();
		std::string message = "\n你确定使用传送功能?\n";
		switch (action)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
			player->TeleportTo(571, 5813.96f, 449.57f, 658.75f, 4.75f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 5:
			player->TeleportTo(571, 8516.00f, 691.20f, 558.23f, 4.72f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 6:
			player->TeleportTo(530, 12955.50f, -6943.40f, 8.79f, 2.48f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 11:
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 怒焰裂谷", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 死亡矿井", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 影牙城堡", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 血色修道院", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 祖尔法拉克", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 黑暗深渊", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 通灵学院", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 熔火之心", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19, message, 0, false);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|t |cFF330066 上一页", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 99);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;
		case GOSSIP_ACTION_INFO_DEF + 21:
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 破碎大厅", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 紫罗兰监狱", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 俄特加德要塞", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 灵魂熔炉", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 奥妮克希亚巢穴", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 26, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 阿尔卡冯宝库", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 27, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 红玉圣殿", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 28, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 冰冠堡垒", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 29, message, 0, false);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|t |cFF330066 上一页", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 99);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;

		case GOSSIP_ACTION_INFO_DEF + 12: // 怒焰裂谷
			player->TeleportTo(1, 1812.78f, -4417.45f, -18.40f, 5.30f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 13: // 死亡矿井
			player->TeleportTo(0, -11084.10f, 1556.17f, 48.12f, 0.0f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 14: // 影牙城堡
			player->TeleportTo(0, -243.85f, 1517.21f, 76.23f, 0.0f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 15: // 血色修道院
			player->TeleportTo(0, 2881.84f, -816.23f, 160.33f, 0.0f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 16: // 祖尔法拉克
			player->TeleportTo(1, -6825.69f, -2882.77f, 8.91f, 0.0f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 17: // 黑石深渊
			player->TeleportTo(1, 4247.34f, 744.05f, -24.71f, 0.0f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 18: // 	通灵学院
			player->TeleportTo(0, 1229.45f, -2576.66f, 90.43f, 0.0f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 19: // 	熔火之心
			player->TeleportTo(230, 1117.61f, -457.36f, -102.49f, 0.0f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 22: // 	破碎大厅
			player->TeleportTo(530, -311.16f, 3082.10f, -3.71f, 1.50f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 23: // 	紫罗兰监狱
			player->TeleportTo(571, 5679.0f, 485.0f, 652.0f, 4.009f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 24: // 	俄特加德要塞
			player->TeleportTo(571, 1231.41f, -4861.30f, 41.24f, 0.309f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 25: // 	灵魂熔炉
			player->TeleportTo(571, 5665.9f, 2009.4f, 798.0f, 5.39f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 26: // 	奥妮克希亚巢穴
			player->TeleportTo(1, -4707.8f, -3727.5f, 54.6f, 3.77f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 27: // 	阿尔卡冯宝库
			player->TeleportTo(571, 5484.50f, 2840.38f, 419.64f, 6.25f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 28: // 	红玉圣殿
			player->TeleportTo(571, 3588.66f, 216.96f, -120.05f, 5.30f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 29: // 	冰冠堡垒
			player->TeleportTo(571, 5817.96f, 2083.99f, 636.00f, 3.56f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 99:
			OnGossipHello(player, creature);
			break;

		case GOSSIP_ACTION_INFO_DEF + 31:  // 联盟相关主城
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 暴风城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 32, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 铁炉堡", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 33, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 达纳苏斯", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 34, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 埃索达", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 35, message, 0, false);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|t |cFF330066 上一页", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 99);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;

		case GOSSIP_ACTION_INFO_DEF + 32:  // 暴风
			player->TeleportTo(0, -8907.23f, 558.86f, 93.36f, 0.64f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 33:  // 铁炉堡
			player->TeleportTo(0, -4928.42f, -946.67f, 501.58f, 5.42f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 34:  // 达纳苏斯
			player->TeleportTo(1, 9953.30f, 2224.29f, 1330.96f, 1.57f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 35:  // 埃索达
			player->TeleportTo(530, -3946.96f, -11622.70f, -138.63f, 2.12f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 41:  // 部落相关主城
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 奥格瑞玛", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 42, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 雷霆崖", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 43, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 幽暗城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 44, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 银月城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 45, message, 0, false);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|t |cFF330066 上一页", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 99);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;

		case GOSSIP_ACTION_INFO_DEF + 42:  // 奥格瑞玛
			player->TeleportTo(1, 1502.71f, -4415.41f, 21.77f, 0.1f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 43:  // 雷霆崖
			player->TeleportTo(1, -1245.33f, 75.63f, 128.49f, 2.04f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 44:  // 幽暗城
			player->TeleportTo(0, 1620.68f, 240.09f, -45.24f, 0.01f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 45:  // 银月城
			player->TeleportTo(530, 9478.13f, -7284.62f, 14.25f, 0.02f);
			player->CLOSE_GOSSIP_MENU();
			break;


		case GOSSIP_ACTION_INFO_DEF + 51:  // 中立城市
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 加基森", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 52, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 棘齿城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 53, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 藏宝海湾", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 54, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 黑暗之门", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 55, message, 0, false);
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t |cFF330066 沙塔斯城", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 56, message, 0, false);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|t |cFF330066 上一页", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 99);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;

		case GOSSIP_ACTION_INFO_DEF + 52:  // 加基森
			player->TeleportTo(1, -7181.39f, -3756.09f, 8.24f, 5.53f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 53:  // 棘齿城
			player->TeleportTo(1, -968.55f, -3774.43f, 5.28f, 4.17f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 54:  // 藏宝海湾
			player->TeleportTo(0, -14287.91f, 551.53f, 8.87f, 4.27f);
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 55:  // 黑暗之门
			player->TeleportTo(530, -250.00f, 1046.35f, 54.31f, 4.74f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 56:  // 沙塔斯城
			player->TeleportTo(530, -1810.73f, 5325.68f, -12.42f, 2.03f);
			player->CLOSE_GOSSIP_MENU();
			break;
		}
		return true;
	}

};

void AddSC_transportStone()
{
	new transportStone();
}