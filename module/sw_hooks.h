//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _STEAMWORKS_GAMESERVER_HOOKS_H_
#define _STEAMWORKS_GAMESERVER_HOOKS_H_

#include "interfaces.h"

class SteamToolsGSHooks
{
	public:

		SteamToolsGSHooks();
		~SteamToolsGSHooks();

	public:

		void LogOnAnonymous();
		bool WasRestartRequested();
		bool BSecure();

	public:

		void RegisterForwards();
		void AddHooks();
		void RemoveHooks();

	private:

		int m_ForwardRestartRequested;
		int m_ForwardTokenRequested;

	private:

		int m_RestartHookID;
		int m_LogAnonHookID;
		int m_BSecureHookID;

		bool m_ShowGameServerInfo;
};

#endif // _STEAMWORKS_GAMESERVER_HOOKS_H_
