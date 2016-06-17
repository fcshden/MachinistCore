#ifndef MACHINIST_VIP_CORE_H
#define MACHINIST_VIP_CORE_H

struct VipElements
{
	uint32 vip;
	uint32 jf;
};

struct ItemVIP
{
	uint32 item_id;
	uint32 vip;
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
	static uint32 GetVIPMAX();
	static float GetVIPOFFSET();
	static uint32 GetTALENTBONUS();
	static float GetVIPLootMoneyBonus();
	static uint32 GetVIP(uint32 acct_id);
	static uint32 GetItemVIP(uint32 item_id);
	static uint32 GetPlayerJf(uint32 acct_id);


	// Setterz
	static void SetVIP(uint32 acct_id, uint32 pvip);
	static void SetHearthStone(uint32 guid, uint32 map_id, float x, float y, float z, float o);
	static void SetItemVIP(uint32 item_id, uint32 item_vip);
	static void SetPlayerJf(uint32 acct_id, uint32 jf);
	static bool VipHearthstone(Player* player);
	static bool VipSetHearthstone(Player* player);

private:
	// tools
	void RemoveItem(uint32 id, Player* player);
};
#endif