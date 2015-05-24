#include "ScriptPCH.h"
#include "Chat.h"
#include <stdarg.h>
#include "GameObject.h"
#include "PoolMgr.h"
#include "ObjectAccessor.h"
#include "Transport.h"
#include "Language.h"
#include "Config.h"

using namespace std;

class misc : public CommandScript
{
public:
	misc() : CommandScript("misc") { }

	ChatCommand * GetCommands() const
	{

		static ChatCommand misccommandTable[] =
		{
			{ "fly",	SEC_PLAYER,		false, &HandleFlyCommand,	"", NULL },
			{ NULL,		0,				false, NULL,				"", NULL }
		};
		return misccommandTable;
	}

	static bool HandleFlyCommand(ChatHandler* handler, char const* args)
	{
		if (!*args)
			return false;

		Player* target = handler->GetSession()->GetPlayer();
		if (!target)
			return false;

		WorldPacket data;
		if (strncmp(args, "on", 3) == 0){
			target->SetCanFly(true);
		}
		else if (strncmp(args, "off", 4) == 0)
			target->SetCanFly(false);
		else
		{
			handler->SendSysMessage(LANG_USE_BOL);
			return false;
		}
		handler->PSendSysMessage(LANG_COMMAND_FLYMODE_STATUS, handler->GetNameLink(target).c_str(), args);
		return true;
	}
};

void AddSC_misc()
{
	new misc;
}