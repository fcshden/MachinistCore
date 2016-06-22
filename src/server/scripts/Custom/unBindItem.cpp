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
#include "resetRandomItem.h"
#pragma execution_character_set("UTF-8")

class unBindItem : public ItemScript
{
public:

	unBindItem() : ItemScript("unBind_item") {}

	bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) override
	{
		// 打断施法
		if (player->IsNonMeleeSpellCast(false))
			player->InterruptNonMeleeSpells(false);

		// 是否在战斗
		if (player->IsInCombat())
		{
			player->GetSession()->SendNotification("您处于战斗中，无法使用！");
			player->CLOSE_GOSSIP_MENU();
			return false;
		}
		// 清除对话菜单
		player->PlayerTalkClass->ClearMenus();

		// 获取目标物品
		Item* tar = targets.GetItemTarget();

		// 获取物品类型
		uint32 tarClass = tar->GetTemplate()->Class;

		// 保存entry
		uint32 targetItemEntry = tar->GetEntry();

		// 读取配置文件
		uint32 unBindItemEntry = sConfigMgr->GetIntDefault("unBindItemEntry", 60005);
		uint32 unBindItemCount = sConfigMgr->GetIntDefault("unBindItemCount", 2);

		// 获取消耗物品链接
		WorldSession* session = player->GetSession();
		std::string unBindItemLink = sResetRandomItem->GetItemLink(unBindItemEntry, session);

		// 拼提示字符
		char count[256];
		itoa(unBindItemCount, count, 10);
		std::ostringstream ss;
		ss << std::endl;
		ss << std::endl << unBindItemLink << " x " << count << std::endl;
		std::string message_1 = "解除绑定操作需要消耗" + ss.str() + "你确定这么操作么?";

		// 判断物品类型
		if (4 == tarClass || 2 == tarClass)
		{
			player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "", targetItemEntry, 0, message_1, 0, false);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
		}
		else
		{
			player->GetSession()->SendAreaTriggerMessage("|CFFFF0000该物品不是装备解除绑定！");
			ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000该物品不是装备解除绑定！");
		}
		return false;
	}

	bool OnGossipSelect(Player* player, Item* item, uint32 targetItemEntry, uint32 action) override
	{

		// 调试信息
		//player->GetSession()->SendNotification("DEBUG: ONGOSSIPSELECT");
		// 清楚菜单
		player->PlayerTalkClass->ClearMenus();

		// 读取配置文件
		uint32 unBindItemEntry = sConfigMgr->GetIntDefault("unBindItemEntry", 60005);
		uint32 unBindItemCount = sConfigMgr->GetIntDefault("unBindItemCount", 2);

		// 获取消耗物品链接
		WorldSession* session = player->GetSession();
		std::string unBindItemLink = sResetRandomItem->GetItemLink(unBindItemEntry, session);

		char count[256];
		itoa(unBindItemCount, count, 10);
		std::string message = "|CFFFF0000您的" + unBindItemLink + "|CFFFF0000数量不足 " + count + "|CFFFF0000 枚！";

		std::string message_1 = "解除绑定消耗" + unBindItemLink + " x " + count + " 枚！";
		Item* tar = player->GetItemByEntry(targetItemEntry);

		if (player->HasItemCount(unBindItemEntry, unBindItemCount))
		{
			// 解除装备绑定
			tar->SetBinding(false);
			player->DestroyItemCount(unBindItemEntry, unBindItemCount, true);
			player->GetSession()->SendAreaTriggerMessage("|CFFFF0000解除物品绑定成功！");
			ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000解除物品绑定成功！");
			ChatHandler(player->GetSession()).SendSysMessage(message_1.c_str());
		}
		else
		{
			player->GetSession()->SendAreaTriggerMessage("%s", message.c_str());
			ChatHandler(player->GetSession()).SendSysMessage(message.c_str());
		}

		player->CLOSE_GOSSIP_MENU();

		return false;
	}

};

void AddSC_unBindItem()
{
	new unBindItem();
}