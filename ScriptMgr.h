/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_SCRIPTMGR_H
#define SC_SCRIPTMGR_H

#include "Common.h"
#include "DBCStructure.h"
#include "Database/DatabaseEnv.h"

class Player;
class Creature;
class CreatureAI;
class InstanceData;
class Quest;
class Item;
class GameObject;
class SpellCastTargets;
class Map;
class Unit;
class WorldObject;
class Aura;

#define MAX_SCRIPTS         5000                            //72 bytes each (approx 351kb)
#define VISIBLE_RANGE       (166.0f)                        //MAX visible range (size of grid)
#define DEFAULT_TEXT        "<ScriptDev2 Text Entry Missing!>"

struct Script
{
    Script() :
        pGossipHello(NULL), pGOGossipHello(NULL), pGossipSelect(NULL), pGOGossipSelect(NULL),
        pGossipSelectWithCode(NULL), pGOGossipSelectWithCode(NULL),
        pQuestSelect(NULL), pQuestComplete(NULL), pNPCDialogStatus(NULL), pGODialogStatus(NULL),
        pChooseReward(NULL), pItemHello(NULL), pGOHello(NULL), pAreaTrigger(NULL), pItemQuestAccept(NULL),
        pQuestAccept(NULL), pGOQuestAccept(NULL), pGOChooseReward(NULL), pItemUse(NULL),
        pEffectDummyCreature(NULL), pEffectDummyGameObj(NULL), pEffectDummyItem(NULL), pEffectAuraDummy(NULL),
        GetAI(NULL), GetInstanceData(NULL)
    {}

    std::string Name;

    //Methods to be scripted
    bool (*pGossipHello             )(Player*, Creature*);
    bool (*pGOGossipHello           )(Player*, GameObject*);
    bool (*pQuestAccept             )(Player*, Creature*, const Quest*);
    bool (*pGossipSelect            )(Player*, Creature*, uint32, uint32);
    bool (*pGOGossipSelect          )(Player*, GameObject*, uint32, uint32);
    bool (*pGossipSelectWithCode    )(Player*, Creature*, uint32, uint32, const char*);
    bool (*pGOGossipSelectWithCode  )(Player*, GameObject*, uint32, uint32, const char*);
    bool (*pQuestSelect             )(Player*, Creature*, const Quest*);
    bool (*pQuestComplete           )(Player*, Creature*, const Quest*);
    uint32 (*pNPCDialogStatus       )(Player*, Creature*);
    uint32 (*pGODialogStatus        )(Player*, GameObject*);
    bool (*pChooseReward            )(Player*, Creature*, const Quest*, uint32);
    bool (*pItemHello               )(Player*, Item*, const Quest*);
    bool (*pGOHello                 )(Player*, GameObject*);
    bool (*pAreaTrigger             )(Player*, AreaTriggerEntry*);
    bool (*pItemQuestAccept         )(Player*, Item*, const Quest*);
    bool (*pGOQuestAccept           )(Player*, GameObject*, const Quest*);
    bool (*pGOChooseReward          )(Player*, GameObject*, const Quest*, uint32);
    bool (*pItemUse                 )(Player*, Item*, SpellCastTargets const&);
    bool (*pEffectDummyCreature     )(Unit*, uint32, SpellEffectIndex, Creature*);
    bool (*pEffectDummyGameObj      )(Unit*, uint32, SpellEffectIndex, GameObject*);
    bool (*pEffectDummyItem         )(Unit*, uint32, SpellEffectIndex, Item*);
    bool (*pEffectAuraDummy         )(const Aura*, bool);

    CreatureAI* (*GetAI)(Creature*);
    InstanceData* (*GetInstanceData)(Map*);

    void RegisterSelf(bool bReportError = true);
};

//Generic scripting text function
void DoScriptText(int32 textEntry, WorldObject* pSource, Unit* target = NULL);
char const* GetScriptText(int32 iTextEntry, Player* pPlayer = NULL);

//DB query
QueryResult* strSD2Pquery(char*);

#if COMPILER == COMPILER_GNU
#define FUNC_PTR(name,callconvention,returntype,parameters)    typedef returntype(*name)parameters __attribute__ ((callconvention));
#else
#define FUNC_PTR(name, callconvention, returntype, parameters)    typedef returntype(callconvention *name)parameters;
#endif

#endif