//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#include "sw_forwards.h"
#include "steamtools.h"

SteamToolsGSForwards::SteamToolsGSForwards()
	:
	m_ForwardFullyLoaded(0),
	m_ForwardShutdown(0),
	m_ForwardConnected(0),
	m_ForwardConnectFailure(0),
	m_ForwardDisconnected(0),
	m_ForwardValidateClient(0),
	m_ForwardInvalidateClient(0),
	m_ForwardGroupStatusResult(0)
{
}

SteamToolsGSForwards::~SteamToolsGSForwards()
{
}

void SteamToolsGSForwards::RegisterForwards()
{
	m_ForwardFullyLoaded       = MF_RegisterForward("Steam_FullyLoaded"               , ET_IGNORE, FP_DONE);
	m_ForwardShutdown          = MF_RegisterForward("Steam_Shutdown"                  , ET_IGNORE, FP_DONE);
	m_ForwardConnected         = MF_RegisterForward("Steam_SteamServersConnected"     , ET_IGNORE, FP_DONE);
	m_ForwardConnectFailure    = MF_RegisterForward("Steam_SteamServersConnectFailure", ET_IGNORE, FP_CELL, FP_DONE);
	m_ForwardDisconnected      = MF_RegisterForward("Steam_SteamServersDisconnected"  , ET_IGNORE, FP_CELL, FP_DONE);
	m_ForwardValidateClient    = MF_RegisterForward("Steam_OnValidateClient"          , ET_IGNORE, FP_CELL, FP_CELL, FP_DONE);
	m_ForwardInvalidateClient  = MF_RegisterForward("Steam_OnInvalidateClient"        , ET_IGNORE, FP_CELL, FP_CELL, FP_CELL, FP_STRING, FP_DONE);
	m_ForwardGroupStatusResult = MF_RegisterForward("Steam_GroupStatusResult"         , ET_IGNORE, FP_CELL, FP_CELL, FP_CELL, FP_CELL, FP_DONE);
}

void SteamToolsGSForwards::OnSteamFullyLoaded()
{
	MF_ExecuteForward(m_ForwardFullyLoaded);
}

void SteamToolsGSForwards::OnSteamShutdown()
{
	MF_ExecuteForward(m_ForwardShutdown);
}

void SteamToolsGSForwards::OnGSClientApprove(GSClientApprove_t* pApprove)
{
	MF_ExecuteForward(m_ForwardValidateClient, pApprove->m_OwnerSteamID.GetAccountID(), pApprove->m_SteamID.GetAccountID());
}

void SteamToolsGSForwards::OnValidateTicket(ValidateAuthTicketResponse_t* pTicket)
{
	MF_ExecuteForward(m_ForwardValidateClient, pTicket->m_OwnerSteamID.GetAccountID(), pTicket->m_SteamID.GetAccountID());
}

void SteamToolsGSForwards::OnGSClientDeny(GSClientDeny_t* pDeny)
{
	MF_ExecuteForward(m_ForwardInvalidateClient, pDeny->m_SteamID.GetAccountID(), pDeny->m_eDenyReason, false, pDeny->m_pchOptionalText);
}

void SteamToolsGSForwards::OnGSClientKick(GSClientKick_t* pKick)
{
	MF_ExecuteForward(m_ForwardInvalidateClient, pKick->m_SteamID.GetAccountID(), pKick->m_eDenyReason, true, "");
}

void SteamToolsGSForwards::OnSteamServersConnected(SteamServersConnected_t* pResponse)
{
	MF_ExecuteForward(m_ForwardConnected);
}

void SteamToolsGSForwards::OnSteamServersConnectFailure(SteamServerConnectFailure_t* pResponse)
{
	MF_ExecuteForward(m_ForwardConnectFailure, static_cast<cell>(pResponse->m_eResult));
}

void SteamToolsGSForwards::OnSteamServersDisconnected(SteamServersDisconnected_t* pResponse)
{
	MF_ExecuteForward(m_ForwardDisconnected, static_cast<cell>(pResponse->m_eResult));
}

void SteamToolsGSForwards::OnGroupStatusResult(GSClientGroupStatus_t* pGroup)
{
	int id;

	for (id = 1; id <= gpGlobals->maxClients; ++id)
	{
		if (MF_IsPlayerAuthorized(id) && !MF_IsPlayerBot(id) && !MF_IsPlayerHLTV(id))
		{
			if (g_SteamTools->RenderedIDToCSteamID(GETPLAYERAUTHID(MF_GetPlayerEdict(id))) == pGroup->m_SteamIDUser)
			{
				break;
			}
		}
	}

	if (id > gpGlobals->maxClients)
	{
		id = -1;
	}

	MF_ExecuteForward(m_ForwardGroupStatusResult, id, pGroup->m_SteamIDGroup.GetAccountID(), pGroup->m_bMember, pGroup->m_bOfficer);
}
