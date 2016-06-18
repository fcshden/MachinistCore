#include "Player.h"
#include "Item.h"
#pragma execution_character_set("UTF-8")

class adam_War : public PlayerScript
{
public:

	adam_War() : PlayerScript("adam_War") { }

	void OnLogin(Player* player, bool /*firstLogin*/)override
	{
		Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, ADAMWAR_SLOT);

		if (item)
		{

			ItemTemplate const* proto = item->GetTemplate();

			std::string itemDes = proto->Description;
			std::string str = "【神器】战魂护符，放在系统背包第一格可以提升人物属性";

			if (itemDes == str)
			{
				ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000[系统提示]|r战魂护符系统加载成功!");
				player->applyAdamWarItemMod(item, ADAMWAR_SLOT, true);
				player->applyAdamWarBonuses(proto, ADAMWAR_SLOT, true, false);
			}
			else
			{
				ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000[系统提示]|r战魂护符系统加载失败, 请将战魂护符放在系统背包第一格, 再次上线后生效!");
			}
		}
		else
		{
			ChatHandler(player->GetSession()).SendSysMessage("|CFFFF0000[系统提示]|r战魂护符系统加载失败, 请将战魂护符放在系统背包第一格, 再次上线后生效!");
		}
	}
};
void AddSC_adam_War()
{
	new adam_War();
}