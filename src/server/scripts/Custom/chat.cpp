#include "ScriptPCH.h"
#include "Chat.h"
#include <stdarg.h>
#include "GameObject.h"
#include "PoolMgr.h"
#include "ObjectAccessor.h"
#include "Transport.h"
#include "Language.h"
#include "Config.h"

std::string PLAYER_H_ICON;
std::string PLAYER_H_PREFIX;
std::string PLAYER_H_SUFFIX;
std::string PLAYER_A_ICON;
std::string PLAYER_A_PREFIX;
std::string PLAYER_A_SUFFIX;
std::string NOBLE_ICON;
std::string NOBLE_PREFIX;
std::string NOBLE_SUFFIX;
std::string BUILDER_ICON;
std::string BUILDER_PREFIX;
std::string BUILDER_SUFFIX;
std::string DUNGEONMASTER_ICON;
std::string DUNGEONMASTER_PREFIX;
std::string DUNGEONMASTER_SUFFIX;
std::string LOREMASTER_ICON;
std::string LOREMASTER_PREFIX;
std::string LOREMASTER_SUFFIX;
std::string STAFF_ICON;
std::string STAFF_PREFIX;
std::string STAFF_SUFFIX;
std::string COUNCILMEMBER_ICON;
std::string COUNCILMEMBER_PREFIX;
std::string COUNCILMEMBER_SUFFIX;

std::string GetNameLink(Player* player)
{
	std::string name = player->GetName();
	std::string color;
	switch (player->getClass())
	{
	case CLASS_DEATH_KNIGHT:
		color = "|cffC41F3B";
		break;
	case CLASS_DRUID:
		color = "|cffFF7D0A";
		break;
	case CLASS_HUNTER:
		color = "|cffABD473";
		break;
	case CLASS_MAGE:
		color = "|cff69CCF0";
		break;
	case CLASS_PALADIN:
		color = "|cffF58CBA";
		break;
	case CLASS_PRIEST:
		color = "|cffFFFFFF";
		break;
	case CLASS_ROGUE:
		color = "|cffFFF569";
		break;
	case CLASS_SHAMAN:
		color = "|cff0070DE";
		break;
	case CLASS_WARLOCK:
		color = "|cff9482C9";
		break;
	case CLASS_WARRIOR:
		color = "|cffC79C6E";
		break;
	}
	return "|Hplayer:" + name + "|h|cffFFFFFF[" + color + name + "|cffFFFFFF]|h|r";
}

class chat : public CommandScript
{
public:
	chat() : CommandScript("chat"){}

	ChatCommand * GetCommands() const
	{
		static ChatCommand toggleCommandTable[] =
		{
			{ "chat", SEC_PLAYER, false, &HandleToggleChatCommand, "", NULL },
			{ "appear", SEC_PLAYER, false, &HandleToggleAppearCommand, "", NULL },
			{ "summon", SEC_PLAYER, false, &HandleToggleSummonCommand, "", NULL },
			{ NULL, 0, false, NULL, "", NULL }
		};

		static ChatCommand ChatCommandTable[] =
		{
			{ "chat", SEC_PLAYER, true, &HandleChatCommand, "", NULL },
			{ "toggle", SEC_PLAYER, false, NULL, "", toggleCommandTable },
			{ NULL, 0, false, NULL, "", NULL }
		};

		return ChatCommandTable;
	}

	static bool HandleToggleChatCommand(ChatHandler* handler, const char* args)
	{
		if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
			return false;

		std::string argstr = (char*)args;

		Player* _player = handler->GetSession()->GetPlayer();

		if (!*args)
			argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(TOGGLE_WORLD_CHAT)) ? "off" : "on";

		if (argstr == "on")
		{
			_player->SetCommandStatusOn(TOGGLE_WORLD_CHAT);
			handler->SendSysMessage("World Chat is ON.");
			return true;
		}
		else if (argstr == "off")
		{
			_player->SetCommandStatusOff(TOGGLE_WORLD_CHAT);
			handler->SendSysMessage("World Chat is OFF.");
			return true;
		}

		return false;
	}

	static bool HandleToggleHintCommand(ChatHandler* handler, const char* args)
	{
		if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
			return false;

		std::string argstr = (char*)args;

		Player* _player = handler->GetSession()->GetPlayer();

		if (!*args)
			argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(TOGGLE_WORLD_HINT)) ? "off" : "on";

		if (argstr == "on")
		{
			_player->SetCommandStatusOn(TOGGLE_WORLD_HINT);
			handler->SendSysMessage("World Hints are ON.");
			return true;
		}
		else if (argstr == "off")
		{
			_player->SetCommandStatusOff(TOGGLE_WORLD_HINT);
			handler->SendSysMessage("World Hints are OFF.");
			return true;
		}

		return false;
	}

	static bool HandleToggleAppearCommand(ChatHandler* handler, const char* args)
	{
		if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
			return false;

		std::string argstr = (char*)args;

		Player* _player = handler->GetSession()->GetPlayer();

		if (!*args)
			argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(TOGGLE_APPEAR)) ? "off" : "on";

		if (argstr == "on")
		{
			_player->SetCommandStatusOn(TOGGLE_APPEAR);
			handler->SendSysMessage("Appear is ON. Players can appear to you.");
			return true;
		}
		else if (argstr == "off")
		{
			_player->SetCommandStatusOff(TOGGLE_APPEAR);
			handler->SendSysMessage("Appear is OFF. Players can't appear to you.");
			return true;
		}

		return false;
	}

	static bool HandleToggleSummonCommand(ChatHandler* handler, const char* args)
	{
		if (!handler->GetSession() && !handler->GetSession()->GetPlayer())
			return false;

		std::string argstr = (char*)args;

		Player* _player = handler->GetSession()->GetPlayer();

		if (!*args)
			argstr = (handler->GetSession()->GetPlayer()->GetCommandStatus(TOGGLE_SUMMON)) ? "off" : "on";

		if (argstr == "on")
		{
			_player->SetCommandStatusOn(TOGGLE_SUMMON);
			handler->SendSysMessage("Summon is ON. Players can summon you.");
			return true;
		}
		else if (argstr == "off")
		{
			_player->SetCommandStatusOff(TOGGLE_SUMMON);
			handler->SendSysMessage("Summon is OFF. Players can't summon you.");
			return true;
		}

		return false;
	}

	static bool HandleChatCommand(ChatHandler * handler, const char * args)
	{
		if (!handler->GetSession()->GetPlayer()->CanSpeak())
		{
			handler->PSendSysMessage("|cffFF0000You have been barred from the world chat!|r");
			handler->SetSentErrorMessage(true);
			return false;
		}

		std::string temp = args;

		if (!args || temp.find_first_not_of(' ') == std::string::npos)
		{
			handler->PSendSysMessage("|cffFF0000You can not send blank messages!|r");
			handler->SetSentErrorMessage(true);
			return false;
		}

		std::string msg = "";
		Player * player = handler->GetSession()->GetPlayer();

		if (!player->GetCommandStatus(TOGGLE_WORLD_CHAT)){
			handler->PSendSysMessage("|cffFF0000Chat is disabled, use .toggle chat on!|r");
			handler->SetSentErrorMessage(true);
			return false;
		}

		QueryResult worldMute = LoginDatabase.PQuery("SELECT is_muted FROM world_mute WHERE guid='%u'", player->GetSession()->GetAccountId());

		uint32 isMuted;

		if (!worldMute)
		{
			isMuted = 0;
		}

		if (worldMute)
		{
			Field * m_fields = worldMute->Fetch();
			isMuted = m_fields[0].GetUInt32();
		}

		if (isMuted == 1)
		{
			handler->PSendSysMessage("|cffFF0000You have been barred from the world chat!|r");
			handler->SetSentErrorMessage(true);
			return false;
		}

		switch (player->GetSession()->GetSecurity())
		{
		case SEC_PLAYER:
			if (player->GetTeam() == ALLIANCE)
			{
				msg += PLAYER_H_ICON;
				msg += PLAYER_H_PREFIX;
				msg += GetNameLink(player);
				msg += PLAYER_H_SUFFIX;
			}
			else
			{
				msg += PLAYER_A_ICON;
				msg += PLAYER_A_PREFIX;
				msg += GetNameLink(player);
				msg += PLAYER_A_SUFFIX;
			}
			break;
		case SEC_MODERATOR:
			msg += NOBLE_ICON;
			msg += NOBLE_PREFIX;
			msg += GetNameLink(player);
			msg += NOBLE_SUFFIX;
			break;
		case SEC_GAMEMASTER:
			msg += BUILDER_ICON;
			msg += BUILDER_PREFIX;
			msg += GetNameLink(player);
			msg += BUILDER_SUFFIX;
			break;
		case SEC_ADMINISTRATOR:
			msg += DUNGEONMASTER_ICON;
			msg += DUNGEONMASTER_PREFIX;
			msg += GetNameLink(player);
			msg += DUNGEONMASTER_SUFFIX;
			break;
		case SEC_LOREMASTER:
			msg += LOREMASTER_ICON;
			msg += LOREMASTER_PREFIX;
			msg += GetNameLink(player);
			msg += LOREMASTER_SUFFIX;
			break;
		case SEC_STAFF:
			msg += STAFF_ICON;
			msg += STAFF_PREFIX;
			msg += GetNameLink(player);
			msg += STAFF_SUFFIX;
			break;
		case SEC_COUNCILMEMBER:
			msg += COUNCILMEMBER_ICON;
			msg += COUNCILMEMBER_PREFIX;
			msg += GetNameLink(player);
			msg += COUNCILMEMBER_SUFFIX;
			break;
		case SEC_CONSOLE:
			msg += "|cfffa9900[ROOT] ";
			msg += GetNameLink(player);
			msg += " |cFFFFFFF0 : |cFF66FFFF";
			break;

		}

		sWorld->SendWorldChat(LANG_CHAT_COLOR, msg.c_str(), args);

		return true;
	}
};

class Script_chat_WorldScript : public WorldScript
{
public:
	Script_chat_WorldScript() : WorldScript("Script_chat_WorldScript") { }

	void OnConfigLoad(bool /*reload*/)
	{
		PLAYER_H_ICON = sConfigMgr->GetStringDefault("PLAYER_H_ICON", "");
		PLAYER_H_PREFIX = sConfigMgr->GetStringDefault("PLAYER_H_PREFIX", "");
		PLAYER_H_SUFFIX = sConfigMgr->GetStringDefault("PLAYER_H_SUFFIX", "");

		PLAYER_A_ICON = sConfigMgr->GetStringDefault("PLAYER_A_ICON", "");
		PLAYER_A_PREFIX = sConfigMgr->GetStringDefault("PLAYER_A_PREFIX", "");
		PLAYER_A_SUFFIX = sConfigMgr->GetStringDefault("PLAYER_A_SUFFIX", "");

		NOBLE_ICON = sConfigMgr->GetStringDefault("COMMUNITYMANAGER_ICON", "");
		NOBLE_PREFIX = sConfigMgr->GetStringDefault("COMMUNITYMANAGER_PREFIX", "");
		NOBLE_SUFFIX = sConfigMgr->GetStringDefault("COMMUNITYMANAGER_SUFFIX", "");

		BUILDER_ICON = sConfigMgr->GetStringDefault("BUILDER_ICON", "");
		BUILDER_PREFIX = sConfigMgr->GetStringDefault("BUILDER_PREFIX", "");
		BUILDER_SUFFIX = sConfigMgr->GetStringDefault("BUILDER_SUFFIX", "");

		DUNGEONMASTER_ICON = sConfigMgr->GetStringDefault("DUNGEONMASTER_ICON", "");
		DUNGEONMASTER_PREFIX = sConfigMgr->GetStringDefault("DUNGEONMASTER_PREFIX", "");
		DUNGEONMASTER_SUFFIX = sConfigMgr->GetStringDefault("DUNGEONMASTER_SUFFIX", "");

		LOREMASTER_ICON = sConfigMgr->GetStringDefault("LOREMASTER_ICON", "");
		LOREMASTER_PREFIX = sConfigMgr->GetStringDefault("LOREMASTER_PREFIX", "");
		LOREMASTER_SUFFIX = sConfigMgr->GetStringDefault("LOREMASTER_SUFFIX", "");

		STAFF_ICON = sConfigMgr->GetStringDefault("COMMUNITYREPRESENTATIVE_ICON", "");
		STAFF_PREFIX = sConfigMgr->GetStringDefault("COMMUNITYREPRESENTATIVE_PREFIX", "");
		STAFF_SUFFIX = sConfigMgr->GetStringDefault("COMMUNITYREPRESENTATIVE_SUFFIX", "");

		COUNCILMEMBER_ICON = sConfigMgr->GetStringDefault("COUNCILMEMBER_ICON", "");
		COUNCILMEMBER_PREFIX = sConfigMgr->GetStringDefault("COUNCILMEMBER_PREFIX", "");
		COUNCILMEMBER_SUFFIX = sConfigMgr->GetStringDefault("COUNCILMEMBER_SUFFIX", "");
	}
};

void AddSC_chat()
{
	new chat();
	new Script_chat_WorldScript;
}

