/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Arathi Highlands
SD%Complete: 100
SDComment: Quest support: 665,660
SDCategory: Arathi Highlands
EndScriptData */

/* ContentData
npc_professor_phizzlethorpe
npc_kinelory
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

/*######
## npc_professor_phizzlethorpe
######*/

enum
{
    SAY_PROGRESS_1          = -1000264,
    SAY_PROGRESS_2          = -1000265,
    SAY_PROGRESS_3          = -1000266,
    EMOTE_PROGRESS_4        = -1000267,
    SAY_AGGRO               = -1000268,
    SAY_PROGRESS_5          = -1000269,
    SAY_PROGRESS_6          = -1000270,
    SAY_PROGRESS_7          = -1000271,
    EMOTE_PROGRESS_8        = -1000272,
    SAY_PROGRESS_9          = -1000273,

    QUEST_SUNKEN_TREASURE   = 665,
    ENTRY_VENGEFUL_SURGE    = 2776
};

struct MANGOS_DLL_DECL npc_professor_phizzlethorpeAI : public npc_escortAI
{
    npc_professor_phizzlethorpeAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    void Reset() { }

    void WaypointReached(uint32 uiPointId)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch(uiPointId)
        {
            case 4: DoScriptText(SAY_PROGRESS_2, m_creature, pPlayer); break;
            case 5: DoScriptText(SAY_PROGRESS_3, m_creature, pPlayer); break;
            case 8: DoScriptText(EMOTE_PROGRESS_4, m_creature); break;
            case 9:
                m_creature->SummonCreature(ENTRY_VENGEFUL_SURGE, -2056.41f, -2144.01f, 20.59f, 5.70f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 600000);
                m_creature->SummonCreature(ENTRY_VENGEFUL_SURGE, -2050.17f, -2140.02f, 19.54f, 5.17f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 600000);
                break;
            case 10: DoScriptText(SAY_PROGRESS_5, m_creature, pPlayer); break;
            case 11:
                DoScriptText(SAY_PROGRESS_6, m_creature, pPlayer);
                SetRun();
                break;
            case 19: DoScriptText(SAY_PROGRESS_7, m_creature, pPlayer); break;
            case 20:
                DoScriptText(EMOTE_PROGRESS_8, m_creature);
                DoScriptText(SAY_PROGRESS_9, m_creature, pPlayer);
                pPlayer->GroupEventHappens(QUEST_SUNKEN_TREASURE, m_creature);
                break;
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature);
    }
};

bool QuestAccept_npc_professor_phizzlethorpe(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_SUNKEN_TREASURE)
    {
        pCreature->setFaction(FACTION_ESCORT_N_NEUTRAL_PASSIVE);
        DoScriptText(SAY_PROGRESS_1, pCreature, pPlayer);

        if (npc_professor_phizzlethorpeAI* pEscortAI = dynamic_cast<npc_professor_phizzlethorpeAI*>(pCreature->AI()))
            pEscortAI->Start(false, false, pPlayer->GetGUID(), pQuest, true);
    }
    return true;
}

CreatureAI* GetAI_npc_professor_phizzlethorpe(Creature* pCreature)
{
    return new npc_professor_phizzlethorpeAI(pCreature);
}

/*######
## npc_kinelory
######*/

enum
{
	SPELL_HEAL					= 3627,
	SPELL_BEARFORM				= 4948,
	QUEST_HINTS_OF_A_NEW_PLAGUE = 660
};

struct MANGOS_DLL_DECL npc_kineloryAI : public npc_escortAI
{
    npc_kineloryAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

	int32 m_healtimer;
	int32 m_bearformtimer;

	void Reset()
    {
		m_healtimer=3000;
		m_bearformtimer=3000;
	}

    void WaypointReached(uint32 i)
    {	
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;
		switch(i)
		{
			case 19: //last waypoint
				pPlayer->GroupEventHappens(QUEST_HINTS_OF_A_NEW_PLAGUE, m_creature);
				break;
		}
		
	}
	void UpdateEscortAI(const uint32 uiDiff)
	{	
		 
		 m_healtimer -= uiDiff;
		 m_bearformtimer -= uiDiff;
		 if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
         return;  

		if (Player* pPlayer = GetPlayerForEscort())
		{ 
		
			if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 75)&& m_healtimer < 0){
				if( m_healtimer<0)
				{
					DoCast(m_creature, SPELL_HEAL);
					m_healtimer =30000;
					return;
				}
			}
			if (((pPlayer->GetMaxHealth()*100 / pPlayer->GetHealth())<50)&& m_healtimer < 0)
			{
				DoCast(pPlayer, SPELL_HEAL);
				m_healtimer =30000;
				return;
			}
		
			if (m_creature->getVictim() && m_bearformtimer<0)
			{
				DoCast(m_creature, SPELL_BEARFORM);
				m_creature->CastSpell(m_creature,SPELL_BEARFORM,true);
				m_bearformtimer=30000;
				return;
			}
		}
		DoMeleeAttackIfReady();
		return;
	}
};

CreatureAI* GetAI_npc_kinelory(Creature* pCreature)
{
    return new npc_kineloryAI(pCreature);
}

bool QuestAccept_npc_kinelory(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_HINTS_OF_A_NEW_PLAGUE)
    {
        pCreature->setFaction(FACTION_ESCORT_A_NEUTRAL_PASSIVE);
			if (npc_kineloryAI* pEscortAI = dynamic_cast<npc_kineloryAI*>(pCreature->AI()))
			{
				pEscortAI->Start(false, false, pPlayer->GetGUID(), pQuest, true);
			}
    }
	return true;
}


void AddSC_arathi_highlands()
{
    Script * newscript;

    newscript = new Script;
    newscript->Name = "npc_professor_phizzlethorpe";
    newscript->GetAI = &GetAI_npc_professor_phizzlethorpe;
    newscript->pQuestAccept = &QuestAccept_npc_professor_phizzlethorpe;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "npc_kinelory";
    newscript->GetAI = &GetAI_npc_kinelory;
    newscript->pQuestAccept = &QuestAccept_npc_kinelory;
    newscript->RegisterSelf();
}
