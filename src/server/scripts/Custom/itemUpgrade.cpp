/*
	物品升级
*/
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
#include "Machinist_VIP_Core.h"
#include "MachinistCore\MachinistCore.h"
#pragma execution_character_set("UTF-8")


class itemUpgrade : public ItemScript
{
public:
	itemUpgrade() :ItemScript("item_Upgrade"){}

	uint32 luckyStoneEntry = sConfigMgr->GetIntDefault("luckyStoneEntry", 60003);
	uint32 uncrazyStoneEntry = sConfigMgr->GetIntDefault("uncrazyStoneEntry", 60004);

	bool OnUse(Player* player, Item* item, SpellCastTargets const & target) override
	{
		// 打断施法
		if (player->IsNonMeleeSpellCast(false))
			player->InterruptNonMeleeSpells(false);

		// 获取玩家VIP等级
		uint32 acct_id = player->GetSession()->GetAccountId();
		uint8 Pvip = VIP::GetVIP(acct_id);

		WorldSession* session = player->GetSession();

		player->PlayerTalkClass->ClearMenus();

		if (player->IsInCombat())
		{
			player->GetSession()->SendNotification("您处于战斗中，无法使用！");
			player->CLOSE_GOSSIP_MENU();
			return false;
		}
		uint32 tarEntry = target.GetItemTargetEntry();

		//player->GetSession()->SendNotification("tarentry = %u！", tarEntry);  // 提示信息

		QueryResult Qresult = WorldDatabase.PQuery("SELECT  upid ,amount, id1 ,amount1 ,id2, amount2 ,id3, amount3 ,chance ,crazing FROM item_up WHERE id = %d", tarEntry);

		if (!Qresult)
		{
			session->SendNotification("这个物品无法升级");
			player->CLOSE_GOSSIP_MENU();
			return false;
		}

		Field *fieldsDB = Qresult->Fetch();
		uint32 upid = fieldsDB[0].GetUInt32();
		uint32 amount = fieldsDB[1].GetUInt32();
		uint32 id1 = fieldsDB[2].GetUInt32();
		uint32 amount1 = fieldsDB[3].GetUInt32();
		uint32 id2 = fieldsDB[4].GetUInt32();
		uint32 amount2 = fieldsDB[5].GetUInt32();
		uint32 id3 = fieldsDB[6].GetUInt32();
		uint32 amount3 = fieldsDB[7].GetUInt32();
		uint32 chance = fieldsDB[8].GetUInt32();
		uint32 crazing = fieldsDB[9].GetUInt32();//  为0物品不碎裂， 为1物品碎裂

		std::string upItemLink = sMachinistCore->GetItemLink(tarEntry, session);
		std::string mat1Link = sMachinistCore->GetItemLink(id1, session);
		std::string mat2Link = sMachinistCore->GetItemLink(id2, session);
		std::string mat3Link = sMachinistCore->GetItemLink(id3, session);
		std::string upedItemLink = sMachinistCore->GetItemLink(upid, session);

		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_12.blp:27|t |cFF330066 升级前物品：" + upItemLink, tarEntry, GOSSIP_ACTION_INFO_DEF + 1);
		player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_01.blp:27|t |cFF330066 需要材料①：" + mat1Link + "×" + sMachinistCore->ConvertNumberToString(amount1), tarEntry, GOSSIP_ACTION_INFO_DEF + 2);
		player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_01.blp:27|t |cFF330066 需要材料②：" + mat2Link + "×" + sMachinistCore->ConvertNumberToString(amount2), tarEntry, GOSSIP_ACTION_INFO_DEF + 3);
		player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_01.blp:27|t |cFF330066 需要材料③：" + mat3Link + "×" + sMachinistCore->ConvertNumberToString(amount3), tarEntry, GOSSIP_ACTION_INFO_DEF + 4);
		player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_01.blp:27|t |cFF330066 升级后物品：" + upedItemLink, tarEntry, GOSSIP_ACTION_INFO_DEF + 5);
		player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_01.blp:27|t |cFF330066 本次升级成功概率：" + sMachinistCore->ConvertNumberToString(chance) + "%", tarEntry, GOSSIP_ACTION_INFO_DEF + 6);

		if (player->HasItemCount(luckyStoneEntry))
		{
			if (chance <= 90)
			{
				uint32 luckyChance = chance + 10; // 幸运宝石加10%概率
				player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_h_15.blp:27|t |cFF330066 幸运宝珠加成后概率：" + sMachinistCore->ConvertNumberToString(luckyChance) + "%", tarEntry, GOSSIP_ACTION_INFO_DEF + 7);
			}
			else
			{
				player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_h_15.blp:27|t |cFF330066 幸运宝珠加成后概率：100%", tarEntry, GOSSIP_ACTION_INFO_DEF + 8);
			}
		}

		player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_h_15.blp:27|t |cFF330066 VIP等级强化成功提升概率：" + sMachinistCore->ConvertNumberToString(Pvip) + " %", tarEntry, GOSSIP_ACTION_INFO_DEF + 12);

		if (0 == crazing)
		{
			player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_A.blp:27|t |cFF330066 升级失败后不会碎裂", tarEntry, GOSSIP_ACTION_INFO_DEF + 9);
		}
		else
		{
			if (player->HasItemCount(uncrazyStoneEntry))
			{
				player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_H.blp:27|t |cFF330066 您拥有防爆宝珠，失败后不会碎裂", tarEntry, GOSSIP_ACTION_INFO_DEF + 10);

			}
			else
			{
				player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Achievement_PVP_P_H.blp:27|t |cFF330066 您没有防爆宝珠， 失败后碎裂，请谨慎升级", tarEntry, GOSSIP_ACTION_INFO_DEF + 11);
			}
		}

		player->ADD_GOSSIP_ITEM(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace.blp:35|t |CFFFF1100 确认合成|r", tarEntry, GOSSIP_ACTION_INFO_DEF);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());

		return false;
	}


	bool OnGossipSelect(Player* player, Item* item/*item*/, uint32 sender/*sender*/, uint32 action) override
	{
		WorldSession* session = player->GetSession();
		std::string playerName = player->GetName();
		uint32 playerTeamNum = player->GetTeamId();
		uint32 chanceUp = rand() % 101; // 随机数 0 -- 100

		QueryResult Qresult = WorldDatabase.PQuery("SELECT  upid ,amount, id1 ,amount1 ,id2, amount2 ,id3, amount3 ,chance ,crazing FROM item_up WHERE id = %d", sender);

		Field *fieldsDB = Qresult->Fetch();
		uint32 upid = fieldsDB[0].GetUInt32();
		uint32 amount = fieldsDB[1].GetUInt32();
		uint32 id1 = fieldsDB[2].GetUInt32();
		uint32 amount1 = fieldsDB[3].GetUInt32();
		uint32 id2 = fieldsDB[4].GetUInt32();
		uint32 amount2 = fieldsDB[5].GetUInt32();
		uint32 id3 = fieldsDB[6].GetUInt32();
		uint32 amount3 = fieldsDB[7].GetUInt32();
		uint32 chance = fieldsDB[8].GetUInt32();
		uint32 crazing = fieldsDB[9].GetUInt32();//  为0物品不碎裂， 为1物品碎裂
		std::string upedItemLink = sMachinistCore->GetItemLink(upid, session);

		// 获取玩家VIP等级
		uint32 acct_id = player->GetSession()->GetAccountId();
		uint8 Pvip = VIP::GetVIP(acct_id);
		chance = chance + Pvip;
		if (50 < chance)
		{
			chance = chance - 5;
		}
		switch (action)
		{
		case GOSSIP_ACTION_INFO_DEF:
		{
			if (player->HasItemCount(sender, amount) && player->HasItemCount(id1, amount1)
				&& player->HasItemCount(id2, amount2) && player->HasItemCount(id3, amount3))
			{
				// 玩家有幸运宝石
				if (player->HasItemCount(luckyStoneEntry))
				{
					uint8 luckyChance = chance + 10;
					if (luckyChance >= chanceUp)
					{
						player->DestroyItemCount(sender, amount, true); // 移除升级前物品
						player->DestroyItemCount(id1, amount1, true); // 移除材料1
						player->DestroyItemCount(id2, amount2, true); //       材料2
						player->DestroyItemCount(id3, amount3, true); //       材料3
						player->DestroyItemCount(luckyStoneEntry, 1, true);  // 移除幸运宝石
						player->AddItem(upid, 1); // 添加升级后物品
						player->GetSession()->SendNotification("恭喜您，升级成功！");
						ChatHandler(player->GetSession()).SendSysMessage("恭喜您，升级成功！");
						player->CLOSE_GOSSIP_MENU();
						if (0 == playerTeamNum)
						{
							std::string str1 = "------------恭喜[|cFF00FF33联盟玩家|r] ♂ |cFFff0066" + playerName + " |r------------";
							std::string str2 = "---升级物品" + upedItemLink + "成功!---";
							sWorld->SendServerMessage(SERVER_MSG_STRING, str1.c_str());
							sWorld->SendServerMessage(SERVER_MSG_STRING, str2.c_str());
						}
						else
						{
							std::string str3 = "------------恭喜[|cFFFF0000部落玩家|r] ♀ |cFFff0066" + playerName + " |r------------";
							std::string str4 = "---升级物品" + upedItemLink + "成功!---";
							sWorld->SendServerMessage(SERVER_MSG_STRING, str3.c_str());
							sWorld->SendServerMessage(SERVER_MSG_STRING, str4.c_str());
						}
					}
					else
					{
						if (0 == crazing)
						{
							player->DestroyItemCount(id1, amount1, true); // 移除材料1
							player->DestroyItemCount(id2, amount2, true); //       材料2
							player->DestroyItemCount(id3, amount3, true); //       材料3
							player->DestroyItemCount(luckyStoneEntry, 1, true);
							player->GetSession()->SendNotification("很遗憾，升级失败！");
							ChatHandler(player->GetSession()).SendSysMessage("很遗憾，升级失败！");
							player->CLOSE_GOSSIP_MENU();
						}
						else
						{
							if (player->HasItemCount(uncrazyStoneEntry))
							{
								player->DestroyItemCount(id1, amount1, true); // 移除材料1
								player->DestroyItemCount(id2, amount2, true); //       材料2
								player->DestroyItemCount(id3, amount3, true); //       材料3
								player->DestroyItemCount(luckyStoneEntry, 1, true);
								player->DestroyItemCount(uncrazyStoneEntry, 1, true);
								player->GetSession()->SendNotification("很遗憾，升级失败，防爆宝石生效， 物品不消失！");
								ChatHandler(player->GetSession()).SendSysMessage("很遗憾，升级失败，防爆宝石生效， 物品不消失！");
								player->CLOSE_GOSSIP_MENU();
							}
							else
							{
								player->DestroyItemCount(sender, amount, true); // 移除升级前物品
								player->DestroyItemCount(id1, amount1, true); // 移除材料1
								player->DestroyItemCount(id2, amount2, true); //       材料2
								player->DestroyItemCount(id3, amount3, true); //       材料3
								player->DestroyItemCount(luckyStoneEntry, 1, true);
								player->GetSession()->SendNotification("很遗憾，升级失败， 物品消失！");
								ChatHandler(player->GetSession()).SendSysMessage("很遗憾，升级失败， 物品消失！");
								player->CLOSE_GOSSIP_MENU();
							}
						}
					}
				}
				else // 玩家无幸运宝石
				{
					if (chance >= chanceUp)
					{
						player->DestroyItemCount(sender, amount, true); // 移除升级前物品
						player->DestroyItemCount(id1, amount1, true); // 移除材料1
						player->DestroyItemCount(id2, amount2, true); //       材料2
						player->DestroyItemCount(id3, amount3, true); //       材料3
						player->AddItem(upid, 1); // 添加升级后物品
						player->GetSession()->SendNotification("恭喜您，升级成功！");
						ChatHandler(player->GetSession()).SendSysMessage("恭喜您，升级成功！");
						player->CLOSE_GOSSIP_MENU();
						if (0 == playerTeamNum)
						{
							std::string str1 = "------------恭喜[|cFF00FF33联盟玩家|r] ♂ |cFFff0066" + playerName + " |r------------";
							std::string str2 = "---升级物品" + upedItemLink + "成功!---";
							sWorld->SendServerMessage(SERVER_MSG_STRING, str1.c_str());
							sWorld->SendServerMessage(SERVER_MSG_STRING, str2.c_str());
						}
						else
						{
							std::string str3 = "------------恭喜[|cFFFF0000部落玩家|r] ♀ |cFFff0066" + playerName + " |r------------";
							std::string str4 = "---升级物品" + upedItemLink + "成功!---";
							sWorld->SendServerMessage(SERVER_MSG_STRING, str3.c_str());
							sWorld->SendServerMessage(SERVER_MSG_STRING, str4.c_str());
						}
					}
					else
					{
						if (0 == crazing)
						{
							player->DestroyItemCount(id1, amount1, true); // 移除材料1
							player->DestroyItemCount(id2, amount2, true); //       材料2
							player->DestroyItemCount(id3, amount3, true); //       材料3
							player->GetSession()->SendNotification("很遗憾，升级失败！");
							ChatHandler(player->GetSession()).SendSysMessage("很遗憾，升级失败！");
							player->CLOSE_GOSSIP_MENU();
						}
						else
						{
							if (player->HasItemCount(uncrazyStoneEntry))
							{
								player->DestroyItemCount(id1, amount1, true); // 移除材料1
								player->DestroyItemCount(id2, amount2, true); //       材料2
								player->DestroyItemCount(id3, amount3, true); //       材料3
								player->DestroyItemCount(uncrazyStoneEntry, 1, true);
								player->GetSession()->SendNotification("很遗憾，升级失败，防爆宝石生效， 物品不消失！");
								ChatHandler(player->GetSession()).SendSysMessage("很遗憾，升级失败，防爆宝石生效， 物品不消失！");
								player->CLOSE_GOSSIP_MENU();
							}
							else
							{
								player->DestroyItemCount(sender, amount, true); // 移除升级前物品
								player->DestroyItemCount(id1, amount1, true); // 移除材料1
								player->DestroyItemCount(id2, amount2, true); //       材料2
								player->DestroyItemCount(id3, amount3, true); //       材料3
								player->GetSession()->SendNotification("很遗憾，升级失败， 物品消失！");
								ChatHandler(player->GetSession()).SendSysMessage("很遗憾，升级失败， 物品消失！");
								player->CLOSE_GOSSIP_MENU();
							}
						}
					}
				}
			}
			else
			{
				player->GetSession()->SendNotification("|CFFFF0000材料数量不足！");
				ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000材料数量不足！");
				player->CLOSE_GOSSIP_MENU();
			}
			break;
		}

		default:
		{
			std::string str = "------升级后的物品为:" + upedItemLink + "------";
			ChatHandler(player->GetSession()).SendSysMessage(str.c_str());
			player->CLOSE_GOSSIP_MENU();
		}
		}
		return false;
	}
};
void AddSC_itemUpgrade()
{
	new itemUpgrade();
}