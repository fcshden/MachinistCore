/*
重置随机附魔系统
*/
#include "Bag.h"
#include "Common.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "DBCStructure.h"
#include "Define.h"
#include "Field.h"
#include "GameEventMgr.h"
#include "Item.h"
#include "ItemPrototype.h"
#include "Language.h"
#include "Log.h"
#include "ObjectGuid.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "QueryResult.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "Transaction.h"
#include "WorldSession.h"
#include "resetRandomItem.h"
#include "MachinistCore\MachinistCore.h"
#pragma execution_character_set("UTF-8")


class CS_ResetRandomItem : public CreatureScript
{
public:
	CS_ResetRandomItem() : CreatureScript("npc_randomItem") {}

	// 界面展示	
	bool OnGossipHello(Player* player, Creature* creature) override
	{
		WorldSession* session = player->GetSession();
		// 界面展示
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|t 需要重置的装备位置要穿装备哦！", EQUIPMENT_SLOT_END + 1, 0);
		for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
		{
			if (const char* slotName = sResetRandomItem->GetSlotName(slot, session))
			{
				std::string icon = sResetRandomItem->GetSlotIcon(slot, 30, 30, -18, 0);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, icon + std::string(slotName), EQUIPMENT_SLOT_END, slot);
			}
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

		return true;
	};

	// 选择效果
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
	{

		// 调试信息
		//player->GetSession()->SendNotification("DEBUG: ONGOSSIPSELECT %u /  %u", sender, action);

		player->PlayerTalkClass->ClearMenus();
		WorldSession* session = player->GetSession();

		switch (sender)
		{

		case EQUIPMENT_SLOT_END: // Show items you can use
			//player->GetSession()->SendNotification("DEBUG: test1");
			ShowRandomItems(player, creature, action);
			break;

		case EQUIPMENT_SLOT_END + 1: // Show items you can use
			OnGossipHello(player, creature);
			break;

		default:
		{
			if (!sender && !action)
			{
				OnGossipHello(player, creature);
				return true;
			}
			sResetRandomItem->resetItemAction(player, ObjectGuid(HighGuid::Item, 0, action));
		}break;
		}
		return true;
	};

	void ShowRandomItems(Player* player, Creature* creature, uint8 slot) // Only checks bags while can use an item from anywhere in inventory
	{

		WorldSession* session = player->GetSession();
		Item* oldItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

		// 读取配置文件
		int randomItemEntry = sConfigMgr->GetIntDefault("randomItemEntry", 60005);
		int randomItemCount = sConfigMgr->GetIntDefault("randomItemCount", 1);

		// 拼字符串
		std::ostringstream ss;
		ss << std::endl;
		ss << std::endl << sMachinistCore->GetItemLink(randomItemEntry, session) << " x " << randomItemCount << std::endl;
		std::string message_1 = "重置随机附魔需要消耗\n" + ss.str() + "你确定这么操作么?";


		if (oldItem)
		{
			Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, RESETITEM_INVENTORY_SLOT);

			if (!newItem)
			{
				player->GetSession()->SendAreaTriggerMessage("请将被重置的装备放在系统背包第二格!");
				ChatHandler(player->GetSession()).SendSysMessage("请将被重置的装备放在系统背包第二格!");
				OnGossipHello(player, creature);
				return;
			}

			if (!sResetRandomItem->CanResetRandomItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
			{
				player->GetSession()->SendAreaTriggerMessage("系统背包第二格装备类型不匹配!");
				ChatHandler(player->GetSession()).SendSysMessage("系统背包第二格装备类型不匹配!");
				OnGossipHello(player, creature);
				return;
			}
			player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, sMachinistCore->GetItemIcon(newItem->GetEntry(), 30, 30, -18, 0) + sMachinistCore->GetItemLink(newItem, session), GOSSIP_SENDER_MAIN, newItem->GetGUID().GetCounter(), "将要重置的装备\n" + sMachinistCore->GetItemIcon(newItem->GetEntry(), 20, 20, -15, -10) + sMachinistCore->GetItemLink(newItem, session) + message_1, 0, false);

		}
		else
		{
			player->GetSession()->SendAreaTriggerMessage("需要重置的装备位置需要穿戴任意一件装备, 并且被重置的装备必须放在系统背包第二格！");
			ChatHandler(player->GetSession()).SendSysMessage("需要重置的装备位置需要穿戴任意一件装备，并且被重置的装备必须放在系统背包第二格！");
		}

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t 返回", EQUIPMENT_SLOT_END + 1, 0);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	};

};


void AddSC_CS_ResetRandomItem()
{
	new CS_ResetRandomItem();
}
