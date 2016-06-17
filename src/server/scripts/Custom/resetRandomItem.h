/*
重置随机附魔系统
*/
#include "Define.h"
#include "ObjectGuid.h"
#define RESETITEM_INVENTORY_SLOT 24

class Item;
class Player;
class WorldSession;
struct ItemTemplate;

class  ResetRandomItem
{
private:
	ResetRandomItem() { };
	~ResetRandomItem() { };
	ResetRandomItem(const ResetRandomItem&);
	ResetRandomItem& operator=(const ResetRandomItem&);

public:
	static ResetRandomItem* instance();

	const char * GetSlotName(uint8 slot, WorldSession* session) const;
	std::string GetItemLink(uint32 entry, WorldSession* session) const;
	std::string GetItemLink(Item* item, WorldSession* session) const;
	std::string GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y) const;
	std::string GetSlotIcon(uint8 slot, uint32 width, uint32 height, int x, int y) const;
	bool ResetRandomItem::CanResetRandomItem(Player* player, ItemTemplate const* target, ItemTemplate const* source) const;
	void ResetRandomItem::resetItemAction(Player* player, ObjectGuid itemGUID)const;


};

#define sResetRandomItem ResetRandomItem::instance()
