#ifndef MACHINIST_VIP_CORE_H
#define MACHINIST_VIP_CORE_H

struct VipElements
{
	uint8 vip;
	int32 jf;
};

struct ItemVIP
{
	uint32 item_id;
	uint8 vip;
};


struct VipHearthStoneGPS
{
	uint32 guid;
	uint32 map_id;
	float x;
	float y;
	float z;
	float o;
};

class VIP
{

public:
	VIP();
	~VIP();

	// Getterz
	static uint8 GetVIPMAX();
	static float GetVIPOFFSET();
	static uint8 GetTALENTBONUS();
	static float GetVIPLootMoneyBonus();
	static uint8 GetVIP(uint32 acct_id);
	static int32 GetPlayerJf(uint32 acct_id);
	static uint8 GetItemVIP(uint32 item_id);

	// Setterz
	static void SetVIP(uint32 acct_id, uint8 pvip);
	static void SetHearthStone(uint32 guid, uint32 map_id, float x, float y, float z, float o);
	static void SetPlayerJf(uint32 acct_id, int32 jf);
	static bool VipHearthstone(Player* player);
	static bool VipSetHearthstone(Player* player);

private:
	// tools
	void RemoveItem(uint32 id, Player* player);
};
#endif