/*

������������Ϣ

���� : ��еʦ

*/

#include <map>
#include "ScriptMgr.h"	
#include "DisableMgr.h"
#include "Player.h"
#include "Spell.h"
#include "Player.h"
#include "Define.h"
#include "GossipDef.h"
#pragma execution_character_set("UTF-8")
#include "resetRandomItem.h"
#include "Config.h"

uint32 changePlayerInfoItemID = sConfigMgr->GetIntDefault("changePlayerInfoItemID", 60005);
uint32 changePlayerInfoItemCount = sConfigMgr->GetIntDefault("changePlayerInfoItemCount", 20);
uint32 changePlayerClassItemCount = sConfigMgr->GetIntDefault("changePlayerClassItemCount", 600);
// ����ת�ַ����ĺ���
std::string NumberToString(uint64 numberX)
{
	auto number = numberX;
	std::stringstream convert;
	std::string number32_to_string;
	convert << number;
	number32_to_string = convert.str();

	return number32_to_string;
};

// ����ְҵ���ؼ���IDֵ
uint32 getPlayerClassSpellId(uint8 playerClass)
{
	uint32 playerClassSpellId;
	switch (playerClass)
	{
	case CLASS_WARRIOR:
		playerClassSpellId = 200001;
		break;
	case CLASS_PALADIN:
		playerClassSpellId = 200002;
		break;
	case CLASS_HUNTER:
		playerClassSpellId = 200003;
		break;
	case CLASS_ROGUE:
		playerClassSpellId = 200004;
		break;
	case CLASS_PRIEST:
		playerClassSpellId = 200005;
		break;
	case CLASS_DEATH_KNIGHT:
		playerClassSpellId = 200006;
		break;
	case CLASS_SHAMAN:
		playerClassSpellId = 200007;
		break;
	case CLASS_MAGE:
		playerClassSpellId = 200008;
		break;
	case CLASS_WARLOCK:
		playerClassSpellId = 200009;
		break;
	case CLASS_DRUID:
		playerClassSpellId = 200010;
		break;
	}
	return playerClassSpellId;
};

void learnPlayerClassSpell(Player* player, uint8 playerClass)
{
	QueryResult res = WorldDatabase.PQuery("SELECT `SpellID` FROM `change_class_spell` WHERE `id` = '%u'", getPlayerClassSpellId(playerClass));
	if (res)
	{
		do
		{
			Field* pField = res->Fetch();
			uint32 spell_ID = pField[0].GetUInt32();
			if (player->HasSpell(spell_ID))
				continue;
			player->LearnSpell(spell_ID, false);
		} while (res->NextRow());
	}
};
void forgetPlayerClassSpell(Player* player)
{
	QueryResult res = WorldDatabase.PQuery("SELECT `SpellID` FROM `change_class_spell` WHERE `id` = '%u'", getPlayerClassSpellId(player->getClass()));
	if (res)
	{
		do
		{
			Field* pField = res->Fetch();
			uint32 spell_ID = pField[0].GetUInt32();
			if (!player->HasSpell(spell_ID))
				continue;
			player->RemoveSpell(spell_ID, false);
		} while (res->NextRow());
	}
};


void playerClassGossipShow(Player* player, uint8 playerClass)
{
	switch (playerClass)
	{
	case CLASS_WARRIOR:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Sword_27:30:30:-18:0|t |cFF330066 ���Ϊսʿְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11, "��ȷ�����Ϊսʿְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_PALADIN:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Hammer_01:30:30:-18:0|t |cFF330066 ���Ϊʥ��ְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12, "��ȷ�����Ϊʥ��ʿְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_HUNTER:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Weapon_Bow_07:30:30:-18:0|t |cFF330066 ���Ϊ����ְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13, "��ȷ�����Ϊ����ְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_ROGUE:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_ThrowingKnife_04:30:30:-18:0|t |cFF330066 ���Ϊ����ְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14
			, "��ȷ�����Ϊ����ְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_PRIEST:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Staff_30:30:30:-18:0|t |cFF330066 ���Ϊ��ʦְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15, "��ȷ�����Ϊ��ʦְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_DEATH_KNIGHT:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\Spell_Deathknight_ClassIcon:30:30:-18:0|t |cFF330066 ���Ϊ����ְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16, "��ȷ�����Ϊ������ʿְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_SHAMAN:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\Spell_Nature_BloodLust:30:30:-18:0|t |cFF330066 ���Ϊ����ְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17, "��ȷ�����Ϊ������˾ְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_MAGE:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\INV_Staff_13:30:30:-18:0|t |cFF330066 ���Ϊ��ʦְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18, "��ȷ�����Ϊ��ʦְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_WARLOCK:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\Spell_Nature_FaerieFire:30:30:-18:0|t |cFF330066 ���Ϊ��ʿְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19, "��ȷ�����Ϊ��ʿְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	case CLASS_DRUID:
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\icons\\Ability_Druid_Maul:30:30:-18:0|t |cFF330066 ���ΪС��ְҵ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20, "��ȷ�����Ϊ��³��ְҵ?\n\n  ְҵ�������Ҫ�����ٴ����߳�Ч! \n\n  ����ϸ�Ķ�˵��!", 0, false);
		break;
	}
};

class changePlayerInfo : public CreatureScript
{
public:
	changePlayerInfo() : CreatureScript("change_Player_info") { }

	bool OnGossipHello(Player* player, Creature* creature) override
	{
		if (player->IsInCombat())
		{
			player->GetSession()->SendNotification("����ս����Ŷ, �����Ժ��ҶԻ�!");
			player->CLOSE_GOSSIP_MENU();
			return false;
		}
		WorldSession* session = player->GetSession();
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 �����ɫ���� " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerInfoItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "��ȷ����Ҫ�������?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 �����ɫ��� " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerInfoItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2, "��ȷ����Ҫ������?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 �����ɫ��Ӫ " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerInfoItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3, "��ȷ����Ҫ�����Ӫ?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 �����ɫ���� " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerInfoItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "��ȷ����Ҫ�������?", 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface\\ICONS\\Spell_Holy_HopeAndGrace:30:30:-18:0|t |cFF330066 �����ɫְҵ " + sResetRandomItem->GetItemLink(changePlayerInfoItemID, session) + " x " + NumberToString(changePlayerClassItemCount), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5 ,"��ȷ����Ҫ�������?\n\n ֻ�ܱ�����������ѡ���ְҵ! \n\n �����ѵ�����װ�� \n\n���Ķ�˵�������ѡ��! ", 0, false);
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
	{
		if (!player)
			return false;

		uint8 maxLevel = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);

		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
			if (player->HasItemCount(changePlayerInfoItemID, changePlayerInfoItemCount))
			{
				player->SetAtLoginFlag(AT_LOGIN_RENAME);
				player->DestroyItemCount(changePlayerInfoItemID, changePlayerInfoItemCount, true);
				player->GetSession()->SendNotification("����ɹ�!��ҷ��ص���ɫѡ����漴���޸Ľ�ɫ����!");
				ChatHandler(player->GetSession()).SendSysMessage("����ɹ�!��ҷ��ص���ɫѡ����漴���޸Ľ�ɫ����!");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendNotification("������Ʒ�������㲻��ʹ�øù���!");
				ChatHandler(player->GetSession()).SendSysMessage("������Ʒ�������㲻��ʹ�øù���!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case GOSSIP_ACTION_INFO_DEF + 2:
			if (player->HasItemCount(changePlayerInfoItemID, changePlayerInfoItemCount))
			{
				player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
				player->DestroyItemCount(changePlayerInfoItemID, changePlayerInfoItemCount, true);
				player->GetSession()->SendNotification("����ɹ�!��ҷ��ص���ɫѡ����漴���޸Ľ�ɫ���!");
				ChatHandler(player->GetSession()).SendSysMessage("����ɹ�!��ҷ��ص���ɫѡ����漴���޸Ľ�ɫ���!");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendNotification("������Ʒ�������㲻��ʹ�øù���!");
				ChatHandler(player->GetSession()).SendSysMessage("������Ʒ�������㲻��ʹ�øù���!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case GOSSIP_ACTION_INFO_DEF + 3:
			if (player->HasItemCount(changePlayerInfoItemID, changePlayerInfoItemCount))
			{
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
				player->DestroyItemCount(changePlayerInfoItemID, changePlayerInfoItemCount, true);
				player->GetSession()->SendNotification("����ɹ�!��ҷ��ص���ɫѡ����漴���޸Ľ�ɫ��Ӫ!");
				ChatHandler(player->GetSession()).SendSysMessage("����ɹ�!��ҷ��ص���ɫѡ����漴���޸Ľ�ɫ��Ӫ!");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendNotification("������Ʒ�������㲻��ʹ�øù���!");
				ChatHandler(player->GetSession()).SendSysMessage("������Ʒ�������㲻��ʹ�øù���!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case GOSSIP_ACTION_INFO_DEF + 4:
			if (player->HasItemCount(changePlayerInfoItemID, changePlayerClassItemCount))
			{
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
				player->DestroyItemCount(changePlayerInfoItemID, changePlayerInfoItemCount, true);
				player->GetSession()->SendNotification("����ɹ�!��ҷ��ص���ɫѡ����漴���޸Ľ�ɫ����!");
				ChatHandler(player->GetSession()).SendSysMessage("����ɹ�!��ҷ��ص���ɫѡ����漴���޸Ľ�ɫ����!");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendNotification("������Ʒ�������㲻��ʹ�øù���!");
				ChatHandler(player->GetSession()).SendSysMessage("������Ʒ�������㲻��ʹ�øù���!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case GOSSIP_ACTION_INFO_DEF + 5:

			// Ҫ������ѵ�����װ��
			for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
			{
				Item* playerEquip = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
					if (playerEquip)
					{
						player->GetSession()->SendNotification("���ְҵ��Ҫ�ѵ�����װ��, ��ȷ������װ���Ѿ�����!");
						ChatHandler(player->GetSession()).SendSysMessage("���ְҵ��Ҫ�ѵ�����װ��, ��ȷ������װ���Ѿ�����!");
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
			}

			if (player->HasItemCount(changePlayerInfoItemID, changePlayerInfoItemCount) && maxLevel == player->getLevel())
			{
				// �������츳
				player->ResetTalents();
				// ��������ְҵ����
				forgetPlayerClassSpell(player);

				if (player->getRace() == RACE_HUMAN) // ���� zs qs dz ms sq fs ss
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_PALADIN)
						playerClassGossipShow(player, CLASS_PALADIN);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				
				}
				else if (player->getRace() == RACE_ORC) // ���� zs lr dz sq sm ss
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_SHAMAN)
						playerClassGossipShow(player, CLASS_SHAMAN);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_DWARF) // ���� zs qs lr zd ms sq 
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_PALADIN)
						playerClassGossipShow(player, CLASS_PALADIN);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_NIGHTELF) // ��ҹ���� zs lr zd ms sq xd
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_DRUID)
						playerClassGossipShow(player, CLASS_DRUID);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_UNDEAD_PLAYER) // ������ zs dz ms sq fs ss
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_TAUREN) // ţͷ�� zs lr sq sm dx
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_SHAMAN)
						playerClassGossipShow(player, CLASS_SHAMAN);
					if (player->getClass() != CLASS_DRUID)
						playerClassGossipShow(player, CLASS_DRUID);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_GNOME) // ٪�� zs dz sq fs ss 
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());

				}
				else if (player->getRace() == RACE_TROLL) // ��ħ za lr zd ms sq sm fs
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_SHAMAN)
						playerClassGossipShow(player, CLASS_SHAMAN);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_BLOODELF) // Ѫ���� qs lr zd ms sq fs ss
				{
					if (player->getClass() != CLASS_PALADIN)
						playerClassGossipShow(player, CLASS_PALADIN);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_ROGUE)
						playerClassGossipShow(player, CLASS_ROGUE);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					if (player->getClass() != CLASS_WARLOCK)
						playerClassGossipShow(player, CLASS_WARLOCK);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}
				else if (player->getRace() == RACE_DRAENEI) // ������ zs qs lr ms sq sm fs
				{
					if (player->getClass() != CLASS_WARRIOR)
						playerClassGossipShow(player, CLASS_WARRIOR);
					if (player->getClass() != CLASS_PALADIN)
						playerClassGossipShow(player, CLASS_PALADIN);
					if (player->getClass() != CLASS_HUNTER)
						playerClassGossipShow(player, CLASS_HUNTER);
					if (player->getClass() != CLASS_PRIEST)
						playerClassGossipShow(player, CLASS_PRIEST);
					if (player->getClass() != CLASS_DEATH_KNIGHT)
						playerClassGossipShow(player, CLASS_DEATH_KNIGHT);
					if (player->getClass() != CLASS_SHAMAN)
						playerClassGossipShow(player, CLASS_SHAMAN);
					if (player->getClass() != CLASS_MAGE)
						playerClassGossipShow(player, CLASS_MAGE);
					player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				}

			}
			else
			{
				player->GetSession()->SendNotification("������Ʒ����������ߵȼ�����80��, ����ʹ�øù���!");
				ChatHandler(player->GetSession()).SendSysMessage("������Ʒ����������ߵȼ�����80��, ����ʹ�øù���!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;
		case GOSSIP_ACTION_INFO_DEF + 11: // ���սʿ
			CharacterDatabase.PExecute("UPDATE characters SET class = 1 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_WARRIOR);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч");
			player->CLOSE_GOSSIP_MENU();				
			break;

		case GOSSIP_ACTION_INFO_DEF + 12: // ���ʥ��ʿ
			CharacterDatabase.PExecute("UPDATE characters SET class = 2 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_PALADIN);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 13: // �������
			CharacterDatabase.PExecute("UPDATE characters SET class = 3 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_HUNTER);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 14: // �������
			CharacterDatabase.PExecute("UPDATE characters SET class = 4 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_ROGUE);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 15: // �����ʦ
			CharacterDatabase.PExecute("UPDATE characters SET class = 5 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_PRIEST);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 16: // ���������ʿ
			CharacterDatabase.PExecute("UPDATE characters SET class = 6 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_DEATH_KNIGHT);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 17: // �������
			CharacterDatabase.PExecute("UPDATE characters SET class = 7 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_SHAMAN);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 18: // �����ʦ
			CharacterDatabase.PExecute("UPDATE characters SET class = 8 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_MAGE);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 19: // �����ʿ
			CharacterDatabase.PExecute("UPDATE characters SET class = 9 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_WARLOCK);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF + 20: // �����³��
			CharacterDatabase.PExecute("UPDATE characters SET class = 11 WHERE guid = %u;", player->GetGUID().GetCounter());
			CharacterDatabase.PExecute("DELETE FROM character_aura WHERE guid = %u;", player->GetGUID().GetCounter());
			//learnPlayerClassSpell(player, CLASS_DRUID);
			player->DestroyItemCount(changePlayerInfoItemID, changePlayerClassItemCount, true);
			player->GetSession()->SendNotification("���ְҵ���, ����˺��ٴ�������Ч!");
			ChatHandler(player->GetSession()).SendSysMessage("���ְҵ���, ����˺��ٴ�������Ч!");
			player->CLOSE_GOSSIP_MENU();
			break;
		}

		return true;
	}

};



void AddSC_changePlayerInfo()
{
	new changePlayerInfo();
}

