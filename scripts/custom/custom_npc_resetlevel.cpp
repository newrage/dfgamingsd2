/* Copyright (C) 2009 - 2010 Project Dark-iCE <http://projectdarkice.clanice.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
/* ScriptData
SDName: custom_npc_resetlevel
SD%Complete: 100
SDComment: Level Reseter with rewards.
SDCategory: NPC
SDAuthor: Darkiss @ Hellscream Network <http://www.hellscream-wow.com/>
EndScriptData */

#include "precompiled.h"
#include "../../../../shared/Config/Config.h"
#include "../../darkice_config.h"

bool GossipHello_custom_npc_resetlevel(Player* pPlayer, Creature* pCreature)
{
	Config DARKICEConfig;
	if(!DARKICEConfig.SetSource(_DARKICE_CONFIG))
		error_log("DARKICE: Unable to open configuration file");

    pPlayer->ADD_GOSSIP_ITEM(0, "Do you want to reset your character ?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(0, "You're not interested.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->PlayerTalkClass->SendGossipMenu(907,pCreature->GetGUID());
    return true;
}

bool GossipSelect_custom_npc_resetlevel(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    Config DARKICEConfig;
	if(!DARKICEConfig.SetSource(_DARKICE_CONFIG))
		error_log("DARKICE: Unable to open configuration file");

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
	{
		if (pPlayer->getLevel() == (DARKICEConfig.GetFloatDefault("ResetMaster.ResetMinimumLevel",0)))
		{
        	Config DARKICEConfig;
        	if(!DARKICEConfig.SetSource(_DARKICE_CONFIG))
       		error_log("DARKICE: Unable to open configuration file");

			pCreature->MonsterWhisper("You have been reset!", pPlayer->GetGUID());
			pPlayer->CLOSE_GOSSIP_MENU();
			pPlayer->ModifyMoney(+(DARKICEConfig.GetFloatDefault("ResetMaster.ResetAddMoney",0)));
			pPlayer->SetLevel((DARKICEConfig.GetFloatDefault("ResetMaster.LevelAfterReset",0)));
		}
		else
		{
			pCreature->MonsterWhisper("Your level is too low or too high to reset!", pPlayer->GetGUID());
			pPlayer->CLOSE_GOSSIP_MENU();
		}
	}

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pCreature->MonsterWhisper("Mh ok. See you around!", pPlayer->GetGUID());
        pPlayer->CLOSE_GOSSIP_MENU();
    }
	
    return true;
}

void AddSC_custom_npc_resetlevel()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "custom_npc_resetlevel";
    newscript->pGossipHello     = &GossipHello_custom_npc_resetlevel;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_resetlevel;
    newscript->RegisterSelf();
}