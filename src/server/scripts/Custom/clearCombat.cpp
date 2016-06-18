#include "Common.h"
#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "WorldSession.h"
#include "Chat.h"
#pragma execution_character_set("UTF-8")

class clearCombat : public ItemScript
{
public:

	clearCombat() : ItemScript("clear_Combat") {}

	bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) override
	{
		if (player->IsInCombat())
		{
			std::string message_1 = "你确定需要脱离战斗?";
			player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "", GOSSIP_SENDER_MAIN, 0, message_1, 0, false);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
		}
		else
		{
			ChatHandler(player->GetSession()).SendSysMessage("你没有处于战斗之中!");
			player->CLOSE_GOSSIP_MENU();

		}
		return false;
	}

	bool OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action) override
	{

		player->PlayerTalkClass->ClearMenus();

		player->ClearInCombat();
		ChatHandler(player->GetSession()).SendSysMessage("脱离战斗成功!");
		player->CLOSE_GOSSIP_MENU();

		return false;
	}
};
void AddSC_clearCombat()
{
	new clearCombat();
}