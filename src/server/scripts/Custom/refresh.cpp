#include "Chat.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "ArenaTeamMgr.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "Group.h"
#include "InstanceSaveMgr.h"
#include "Language.h"
#include "MovementGenerator.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "SpellAuras.h"
#include "TargetedMovementGenerator.h"
#include "WeatherMgr.h"
#include "Player.h"
#include "Pet.h"
#include "LFG.h"
#include "GroupMgr.h"
#include "MMapFactory.h"
#include "DisableMgr.h"

class refresh : public CommandScript
{
public:
	refresh() : CommandScript("refresh") { }

	ChatCommand * GetCommands() const
	{

		static ChatCommand refreshcommandTable[] =
		{
			{ "refresh", SEC_PLAYER, false, &HandleRefreshCommand, "", NULL },
		};
		return refreshcommandTable;
	}

	// Teleport player to last position
	static bool HandleRefreshCommand(ChatHandler* handler, char const* args)
	{
		Player* target;
		if (!handler->extractPlayerTarget((char*)args, &target))
			return false;

		target->SaveRecallPosition();

		// check online security
		if (handler->HasLowerSecurity(target, ObjectGuid::Empty))
			return false;

		if (target->IsBeingTeleported())
		{
			handler->PSendSysMessage(LANG_IS_TELEPORTED, handler->GetNameLink(target).c_str());
			handler->SetSentErrorMessage(true);
			return false;
		}

		// stop flight if need
		if (target->IsInFlight())
		{
			target->GetMotionMaster()->MovementExpired();
			target->CleanupAfterTaxiFlight();
		}

		target->TeleportTo(target->m_recallMap, target->m_recallX, target->m_recallY, target->m_recallZ, target->m_recallO);
		return true;
	}
};

void AddSC_refresh()
{
	new refresh;
}