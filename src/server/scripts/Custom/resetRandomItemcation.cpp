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
#include <sstream>
#include <string>
#include "resetRandomItem.h"
#pragma execution_character_set("UTF-8")

 ResetRandomItem* ResetRandomItem::instance()
{
	// Thread safe in C++11 standard
	static ResetRandomItem instance;
	return &instance;
}
 // 装备物品名字
 const char* ResetRandomItem::GetSlotName(uint8 slot, WorldSession* /*session*/) const
{
	TC_LOG_DEBUG("custom.resetRandomItem", "ResetRandomItem::GetSlotName");
	switch (slot)
	{
	case EQUIPMENT_SLOT_HEAD: return  " 头盔重置";// session->GetTrinityString(LANG_SLOT_NAME_HEAD);
	case EQUIPMENT_SLOT_NECK: return " 项链重置";// session->GetTrinityString(EQUIPMENT_SLOT_NECK);
	case EQUIPMENT_SLOT_SHOULDERS: return  " 肩膀重置";// session->GetTrinityString(LANG_SLOT_NAME_SHOULDERS);
	case EQUIPMENT_SLOT_CHEST: return  " 胸甲重置";// session->GetTrinityString(LANG_SLOT_NAME_CHEST);
	case EQUIPMENT_SLOT_WAIST: return  " 腰带重置";// session->GetTrinityString(LANG_SLOT_NAME_WAIST);
	case EQUIPMENT_SLOT_LEGS: return  " 护腿重置";// session->GetTrinityString(LANG_SLOT_NAME_LEGS);
	case EQUIPMENT_SLOT_FEET: return  " 长靴重置";// session->GetTrinityString(LANG_SLOT_NAME_FEET);
	case EQUIPMENT_SLOT_WRISTS: return  " 手腕重置";// session->GetTrinityString(LANG_SLOT_NAME_WRISTS);
	case EQUIPMENT_SLOT_HANDS: return  " 手套重置";// session->GetTrinityString(LANG_SLOT_NAME_HANDS);
	case EQUIPMENT_SLOT_FINGER1: return  " 戒指重置";// session->GetTrinityString(EQUIPMENT_SLOT_FINGER1);
	case EQUIPMENT_SLOT_TRINKET1: return  " 饰品重置";// session->GetTrinityString(EQUIPMENT_SLOT_TRINKET1);
	case EQUIPMENT_SLOT_BACK: return  " 披风重置";// session->GetTrinityString(EQUIPMENT_SLOT_BACK);
	case EQUIPMENT_SLOT_OFFHAND: return " 副手重置";
	default: return NULL;
	}
}

// 获取佩戴位置图标
std::string ResetRandomItem::GetSlotIcon(uint8 slot, uint32 width, uint32 height, int x, int y) const
{
	TC_LOG_DEBUG("custom.resetRandomItem", "ResetRandomItem::GetSlotIcon");

	std::ostringstream ss;
	ss << "|TInterface/PaperDoll/";
	switch (slot)
	{
	case EQUIPMENT_SLOT_HEAD: ss << "UI-PaperDoll-Slot-Head"; break;
	case EQUIPMENT_SLOT_NECK: ss << "UI-PaperDoll-Slot-Neck"; break;
	case EQUIPMENT_SLOT_SHOULDERS: ss << "UI-PaperDoll-Slot-Shoulder"; break;
	case EQUIPMENT_SLOT_CHEST: ss << "UI-PaperDoll-Slot-Chest"; break;
	case EQUIPMENT_SLOT_WAIST: ss << "UI-PaperDoll-Slot-Waist"; break;
	case EQUIPMENT_SLOT_LEGS: ss << "UI-PaperDoll-Slot-Legs"; break;
	case EQUIPMENT_SLOT_FEET: ss << "UI-PaperDoll-Slot-Feet"; break;
	case EQUIPMENT_SLOT_WRISTS: ss << "UI-PaperDoll-Slot-Wrists"; break;
	case EQUIPMENT_SLOT_HANDS: ss << "UI-PaperDoll-Slot-Hands"; break;
	case EQUIPMENT_SLOT_FINGER1: ss << "UI-PaperDoll-Slot-Finger"; break;
	case EQUIPMENT_SLOT_TRINKET1: ss << "UI-PaperDoll-Slot-Trinket"; break;
	case EQUIPMENT_SLOT_BACK: ss << "UI-PaperDoll-Slot-Chest"; break;
	case EQUIPMENT_SLOT_OFFHAND: ss << "UI-PaperDoll-Slot-SecondaryHand"; break;
	default: ss << "UI-Backpack-EmptySlot";
	}
	ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";
	return ss.str();
}



// 获取物品图标
std::string ResetRandomItem::GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y) const
{
	TC_LOG_DEBUG("custom.resetRandomItem", "ResetRandomItem::GetItemIcon");

	std::ostringstream ss;
	ss << "|TInterface";
	const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
	const ItemDisplayInfoEntry* dispInfo = NULL;
	if (temp)
	{
		dispInfo = sItemDisplayInfoStore.LookupEntry(temp->DisplayInfoID);
		if (dispInfo)
			ss << "/ICONS/" << dispInfo->inventoryIcon;
	}
	if (!dispInfo)
		ss << "/InventoryItems/WoWUnknownItem01";
	ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";
	return ss.str();
}

// 可以重置的装备
bool ResetRandomItem::CanResetRandomItem(Player* player, ItemTemplate const* target, ItemTemplate const* source) const //ItemTemplate const* target,
{
	TC_LOG_DEBUG("custom.resetRandomItem", "ResetRandomItem::CanResetRandomItem");

	if (!target || !source)
		return false;

	//if (source->ItemId == target->ItemId)
	//return false;

	if (source->Class != target->Class)
		return false;

	if (source->InventoryType == INVTYPE_BAG ||
		source->InventoryType == INVTYPE_RELIC ||
		//source->InventoryType == INVTYPE_BODY ||
		source->InventoryType == INVTYPE_AMMO ||
		source->InventoryType == INVTYPE_QUIVER)
		return false;

	if (target->InventoryType == INVTYPE_BAG ||
		target->InventoryType == INVTYPE_RELIC ||
		//target->InventoryType == INVTYPE_BODY ||
		target->InventoryType == INVTYPE_AMMO ||
		target->InventoryType == INVTYPE_QUIVER)
		return false;

	if (source->InventoryType != target->InventoryType)
		return false;


	return true;
}

// 确认重置后， 给玩家洗附魔
void ResetRandomItem::resetItemAction(Player* player, ObjectGuid itemGUID)const
{
	// 读取配置文件
	uint32 randomItemEntry = sConfigMgr->GetIntDefault("randomItemEntry", 60005);
	uint32 randomItemCount = sConfigMgr->GetIntDefault("randomItemCount", 1);

	WorldSession* session = player->GetSession();
	char count[256];
	itoa(randomItemCount, count, 10);
	std::string resetRandomLink = sResetRandomItem->GetItemLink(randomItemEntry, session);
	std::string message_1 = "重置物品随机附魔消耗" + resetRandomLink + " x " + count + " 枚！";
	std::string message = "|CFFFF0000您的" + resetRandomLink + "|CFFFF0000数量不足 " + count + "|CFFFF0000 枚！";
	
	Item* changedItem = NULL;

	if (!itemGUID.IsEmpty())
	{
		changedItem = player->GetItemByGuid(itemGUID);
	}

	uint32 suffix = changedItem->GetTemplate()->RandomSuffix;

	if (!suffix)
	{
		player->GetSession()->SendNotification("物品没有随机附魔无法重置!");
		return;
	}
	else
	{
		QueryResult result = WorldDatabase.PQuery("SELECT ench FROM item_enchantment_template WHERE entry = '%i' ORDER BY RAND() LIMIT 1", suffix);
		if (!result)
		{
			player->GetSession()->SendNotification("物品没有随机附魔无法重置!");
			return;
		}
		else
		{

			if (player->HasItemCount(randomItemEntry, randomItemCount))
			{
				Field* fields = result->Fetch();
				int32 ench = fields[0].GetInt32();
				changedItem->SetItemRandomProperties(-ench);
				player->DestroyItemCount(randomItemEntry, randomItemCount, true);
				player->GetSession()->SendAreaTriggerMessage("|CFFFF0000重置物品随机附魔成功！");
				ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000重置物品随机附魔成功！");
				ChatHandler(player->GetSession()).SendSysMessage(message_1.c_str());
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("%s", message.c_str());
				ChatHandler(player->GetSession()).SendSysMessage(message.c_str());
				player->CLOSE_GOSSIP_MENU();
			}
		}
	}
}

/*  获取获取装备链接 */
std::string ResetRandomItem::GetItemLink(uint32 entry, WorldSession* session) const
{
	TC_LOG_DEBUG("custom.resetRandomItem", "ResetRandomItem::GetItemLink");

	const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
	int loc_idx = session->GetSessionDbLocaleIndex();
	std::string name = temp->Name1;
	if (ItemLocale const* il = sObjectMgr->GetItemLocale(entry))
		ObjectMgr::GetLocaleString(il->Name, loc_idx, name);

	std::ostringstream oss;
	oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
		"|Hitem:" << entry << ":0:0:0:0:0:0:0:0:0|h[" << name << "]|h|r";

	return oss.str();
}


std::string ResetRandomItem::GetItemLink(Item* item, WorldSession* session) const
{
	TC_LOG_DEBUG("custom.transmog", "Transmogrification::GetItemLink");

	int loc_idx = session->GetSessionDbLocaleIndex();
	const ItemTemplate* temp = item->GetTemplate();
	std::string name = temp->Name1;
	if (ItemLocale const* il = sObjectMgr->GetItemLocale(temp->ItemId))
		ObjectMgr::GetLocaleString(il->Name, loc_idx, name);

	if (int32 itemRandPropId = item->GetItemRandomPropertyId())
	{
		char* const* suffix = NULL;
		if (itemRandPropId < 0)
		{
			const ItemRandomSuffixEntry* itemRandEntry = sItemRandomSuffixStore.LookupEntry(-item->GetItemRandomPropertyId());
			if (itemRandEntry)
				suffix = itemRandEntry->nameSuffix;
		}
		else
		{
			const ItemRandomPropertiesEntry* itemRandEntry = sItemRandomPropertiesStore.LookupEntry(item->GetItemRandomPropertyId());
			if (itemRandEntry)
				suffix = itemRandEntry->nameSuffix;
		}
		if (suffix)
		{
			std::string test(suffix[(name != temp->Name1) ? loc_idx : DEFAULT_LOCALE]);
			if (!test.empty())
			{
				name += ' ';
				name += test;
			}
		}
	}

	std::ostringstream oss;
	oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
		"|Hitem:" << temp->ItemId << ":" <<
		item->GetEnchantmentId(PERM_ENCHANTMENT_SLOT) << ":" <<
		item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT) << ":" <<
		item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2) << ":" <<
		item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3) << ":" <<
		item->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT) << ":" <<
		item->GetItemRandomPropertyId() << ":" << item->GetItemSuffixFactor() << ":" <<
		(uint32)item->GetOwner()->getLevel() << "|h[" << name << "]|h|r";

	return oss.str();
}
