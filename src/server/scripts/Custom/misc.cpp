#include "ScriptPCH.h"
#include "Chat.h"
#include <stdarg.h>
#include "GameObject.h"
#include "PoolMgr.h"
#include "ObjectAccessor.h"
#include "Transport.h"
#include "Language.h"

using namespace std;

class misc : public CommandScript
{
public:
	misc() : CommandScript("anim") { }

	ChatCommand * GetCommands() const
	{

		static ChatCommand misccommandTable[] =
		{
			{ NULL, NULL, false, NULL, "", NULL },
		};
		return misccommandTable;
	}

};

void AddSC_misc()
{
	new misc;
}