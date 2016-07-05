/*
	机械师核心
	封装相关通用方法
*/

#include "Define.h"
#include "ObjectGuid.h"
#pragma execution_character_set("UTF-8")

class Item;
class Player;
class WorldSession;
class  MachinistCore
{
private:
	MachinistCore() { };
	~MachinistCore() { };
	MachinistCore(const MachinistCore&);
	MachinistCore& operator=(const MachinistCore&);

public:
	static MachinistCore* instance();

	std::string GetItemLink(uint32 entry, WorldSession* session) const;
	std::string GetItemLink(Item* item, WorldSession* session) const;
	std::string ConvertNumberToString(uint64 numberX) const;
	std::string GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y) const;
};

#define sMachinistCore MachinistCore::instance()
