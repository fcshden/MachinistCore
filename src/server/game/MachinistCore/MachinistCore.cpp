/*
   机械师核心
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
#include "MachinistCore.h"
#pragma execution_character_set("UTF-8")

MachinistCore* MachinistCore::instance()
{
	// Thread safe in C++11 standard
	static MachinistCore instance;
	return &instance;
}
// 通过entry获取装备链接
std::string MachinistCore::GetItemLink(uint32 entry, WorldSession* session) const
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

// 通过Item获取装备链接
std::string MachinistCore::GetItemLink(Item* item, WorldSession* session) const
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

// int转string函数
std::string MachinistCore::ConvertNumberToString(uint64 numberX) const
{
	auto number = numberX;
	std::stringstream convert;
	std::string number32_to_string;
	convert << number;
	number32_to_string = convert.str();

	return number32_to_string;
};

// 获取物品图标
std::string MachinistCore::GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y) const
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
};
