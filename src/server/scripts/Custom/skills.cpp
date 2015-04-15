#include "ScriptPCH.h"

class customskills : public PlayerScript
{
public:

	customskills() : PlayerScript("newskills") { }
	void OnLogin(Player * player, bool)
	{
		player->GetSession()->SendNotification("|cffFFFF00Welcome to Ascendance!");
		player->SetCommandStatusOn(TOGGLE_WORLD_CHAT);
	}
};

void AddSC_customskills()
{
	new customskills;
}