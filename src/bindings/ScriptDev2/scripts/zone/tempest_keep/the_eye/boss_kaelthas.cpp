/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_Kaelthas
SD%Complete: 60
SDComment: SQL, phase 2, phase 3, Mind Control, taunt immunity
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "def_the_eye.h"
#include "WorldPacket.h"

//kael'thas Speech
#define SAY_INTRO                           -1550016
#define SAY_INTRO_CAPERNIAN                 -1550017
#define SAY_INTRO_TELONICUS                 -1550018
#define SAY_INTRO_THALADRED                 -1550019
#define SAY_INTRO_SANGUINAR                 -1550020
#define SAY_PHASE2_WEAPON                   -1550021
#define SAY_PHASE3_ADVANCE                  -1550022
#define SAY_PHASE4_INTRO2                   -1550023
#define SAY_PHASE5_NUTS                     -1550024
#define SAY_SLAY1                           -1550025
#define SAY_SLAY2                           -1550026
#define SAY_SLAY3                           -1550027
#define SAY_MINDCONTROL1                    -1550028
#define SAY_MINDCONTROL2                    -1550029
#define SAY_GRAVITYLAPSE1                   -1550030
#define SAY_GRAVITYLAPSE2                   -1550031
#define SAY_SUMMON_PHOENIX1                 -1550032
#define SAY_SUMMON_PHOENIX2                 -1550033
#define SAY_DEATH                           -1550034

//Thaladred the Darkener speech
#define SAY_THALADRED_AGGRO                 -1550035
#define SAY_THALADRED_DEATH                 -1550036
#define EMOTE_THALADRED_GAZE                -1550037

//Lord Sanguinar speech
#define SAY_SANGUINAR_AGGRO                 -1550038
#define SAY_SANGUINAR_DEATH                 -1550039

//Grand Astromancer Capernian speech
#define SAY_CAPERNIAN_AGGRO                 -1550040
#define SAY_CAPERNIAN_DEATH                 -1550041

//Master Engineer Telonicus speech
#define SAY_TELONICUS_AGGRO                 -1550042
#define SAY_TELONICUS_DEATH                 -1550043

//Phase 2 spells (Not used)
#define SPELL_SUMMON_WEAPONS                36976
#define SPELL_SUMMON_WEAPONA                36958
#define SPELL_SUMMON_WEAPONB                36959
#define SPELL_SUMMON_WEAPONC                36960
#define SPELL_SUMMON_WEAPOND                36961
#define SPELL_SUMMON_WEAPONE                36962
#define SPELL_SUMMON_WEAPONF                36963
#define SPELL_SUMMON_WEAPONG                36964
#define SPELL_RES_VISUAL                    24171
#define SPELL_WEAPON_SPAWN                  41236

//Phase 4 spells
#define SPELL_FIREBALL                      22088           //wrong but works with CastCustomSpell
#define SPELL_PYROBLAST                     36819
#define SPELL_FLAME_STRIKE                  36735
#define SPELL_FLAME_STRIKE_VIS              36730
#define SPELL_FLAME_STRIKE_DMG              36731
#define SPELL_ARCANE_DISRUPTION             36834
#define SPELL_SHOCK_BARRIER                 36815
#define SPELL_PHOENIX_ANIMATION             36723
#define SPELL_MIND_CONTROL                  32830

//Phase 5 spells
#define SPELL_EXPLODE                       36092
#define SPELL_FULLPOWER                     36187
#define SPELL_KNOCKBACK                     11027
#define SPELL_GRAVITY_LAPSE                 34480
#define SPELL_GRAVITY_LAPSE_AURA            39432
#define SPELL_NETHER_BEAM                   35873

//Thaladred the Darkener spells
#define SPELL_PSYCHIC_BLOW                  10689
#define SPELL_SILENCE                       30225
//Lord Sanguinar spells
#define SPELL_BELLOWING_ROAR                40636
//Grand Astromancer Capernian spells
#define CAPERNIAN_DISTANCE                  20              //she casts away from the target
#define SPELL_CAPERNIAN_FIREBALL            36971
#define SPELL_CONFLAGRATION                 37018
#define SPELL_ARCANE_EXPLOSION              36970
//Master Engineer Telonicus spells
#define SPELL_BOMB                          37036
#define SPELL_REMOTE_TOY                    37027
//Nether Vapor spell
#define SPELL_NETHER_VAPOR                  35859
//Phoenix spell
#define SPELL_BURN                          36720
#define SPELL_EMBER_BLAST                   34341
#define SPELL_REBIRTH                       41587

//Creature IDs
#define PHOENIX                             21362
#define PHOENIX_EGG                         21364

//Phoenix egg and phoenix model
#define PHOENIX_MODEL                       19682
#define PHOENIX_EGG_MODEL                   20245

//weapon id + position
float KaelthasWeapons[7][5] =
{
    {21270, 794.38, 15, 48.72, 2.9},                        //[Cosmic Infuser]
    {21269, 785.47, 12.12, 48.72, 3.14},                    //[Devastation]
    {21271, 781.25, 4.39, 48.72, 3.14},                     //[Infinity Blade]
    {21273, 777.38, -0.81, 48.72, 3.06},                    //[Phaseshift Bulwark]
    {21274, 781.48, -6.08, 48.72, 3.9},                     //[Staff of Disintegration]
    {21272, 785.42, -13.59, 48.72, 3.4},                    //[Warp Slicer]
    {21268, 793.06, -16.61, 48.72, 3.10}                    //[Netherstrand Longbow]
};

#define GRAVITY_X 795.0f
#define GRAVITY_Y 0.0f
#define GRAVITY_Z 70.0f

#define TIME_PHASE_2_3      120000
#define TIME_PHASE_3_4      120000

#define KAEL_VISIBLE_RANGE  50.0f

//Base AI for Advisors
struct MANGOS_DLL_DECL advisorbase_ai : public ScriptedAI
{
    advisorbase_ai(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;
    bool FakeDeath;
    uint32 DelayRes_Timer;
    uint64 DelayRes_Target;

    void Reset()
    {
        FakeDeath = false;
        DelayRes_Timer = 0;
        DelayRes_Target = 0;

        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        //reset encounter
        if (pInstance && (pInstance->GetData(DATA_KAELTHASEVENT) == 1 || pInstance->GetData(DATA_KAELTHASEVENT) == 3))
        {
            if (Creature *Kaelthas = (Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KAELTHAS)))
                Kaelthas->AI()->EnterEvadeMode();
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || FakeDeath || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void AttackStart(Unit* who)
    {
        if (!who || FakeDeath || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::AttackStart(who);
    }

    void Revive(Unit* Target)
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetHealth(m_creature->GetMaxHealth());
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);

        DoCast(m_creature, SPELL_RES_VISUAL, false);
        DelayRes_Timer = 2000;
    }

    void DamageTaken(Unit* pKiller, uint32 &damage)
    {
        if (damage < m_creature->GetHealth())
            return;

        //Prevent glitch if in fake death
        if (FakeDeath)
        {
            damage = 0;
            return;
        }

        //Don't really die in phase 1 & 3, only die after that
        if (pInstance && pInstance->GetData(DATA_KAELTHASEVENT) != 0)
        {
            //prevent death
            damage = 0;
            FakeDeath = true;

            m_creature->InterruptNonMeleeSpells(false);
            m_creature->SetHealth(0);
            m_creature->StopMoving();
            m_creature->ClearComboPointHolders();
            m_creature->RemoveAllAurasOnDeath();
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
            m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, false);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->ClearAllReactives();
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET,0);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

            if (pInstance->GetData(DATA_KAELTHASEVENT) == 3)
                JustDied(pKiller);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (DelayRes_Timer)
        {
            if (DelayRes_Timer <= diff)
            {
                DelayRes_Timer = 0;
                FakeDeath = false;

                Unit* Target = Unit::GetUnit((*m_creature), DelayRes_Target);
                if (!Target)
                    Target = m_creature->getVictim();

                DoResetThreat();
                AttackStart(Target);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(Target);
                m_creature->AddThreat(Target, 0.0f);
            }else DelayRes_Timer -= diff;
        }
    }

};

//Kael'thas AI
struct MANGOS_DLL_DECL boss_kaelthasAI : public ScriptedAI
{
    boss_kaelthasAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        AdvisorGuid[0] = 0;
        AdvisorGuid[1] = 0;
        AdvisorGuid[2] = 0;
        AdvisorGuid[3] = 0;
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 Fireball_Timer;
    uint32 ArcaneDisruption_Timer;
    uint32 Phoenix_Timer;
    uint32 ShockBarrier_Timer;
    uint32 GravityLapse_Timer;
    uint32 GravityLapse_Phase;
    uint32 NetherBeam_Timer;
    uint32 NetherVapor_Timer;
    uint32 FlameStrike_Timer;
    uint32 MindControl_Timer;
    uint32 Phase;
    uint32 PhaseSubphase;                                   //generic
    uint32 Phase_Timer;                                     //generic timer
    uint32 PyrosCasted;

    bool InGravityLapse;
    bool IsCastingFireball;
    bool ChainPyros;

    uint64 AdvisorGuid[4];

    void Reset()
    {
        Fireball_Timer = 5000+rand()%10000;
        ArcaneDisruption_Timer = 45000;
        MindControl_Timer = 40000;
        Phoenix_Timer = 50000;
        ShockBarrier_Timer = 60000;
        FlameStrike_Timer = 30000;
        GravityLapse_Timer = 20000;
        GravityLapse_Phase = 0;
        NetherBeam_Timer = 8000;
        NetherVapor_Timer = 10000;
        PyrosCasted = 0;
        Phase = 0;
        InGravityLapse = false;
        IsCastingFireball = false;
        ChainPyros = false;

        if (InCombat)
            PrepareAdvisors();

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (pInstance)
            pInstance->SetData(DATA_KAELTHASEVENT, 0);
    }

    void PrepareAdvisors()
    {
        for(uint8 i = 0; i < 4; i++)
        {
            if (Creature *pCreature = (Creature*)Unit::GetUnit((*m_creature), AdvisorGuid[i]))
            {
                pCreature->Respawn();
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pCreature->setFaction(m_creature->getFaction());
                pCreature->AI()->EnterEvadeMode();
            }
        }
    }

    void StartEvent()
    {
        if (!pInstance)
            return;

        AdvisorGuid[0] = pInstance->GetData64(DATA_THALADREDTHEDARKENER);
        AdvisorGuid[1] = pInstance->GetData64(DATA_LORDSANGUINAR);
        AdvisorGuid[2] = pInstance->GetData64(DATA_GRANDASTROMANCERCAPERNIAN);
        AdvisorGuid[3] = pInstance->GetData64(DATA_MASTERENGINEERTELONICUS);

        if (!AdvisorGuid[0] || !AdvisorGuid[1] || !AdvisorGuid[2] || !AdvisorGuid[3])
        {
            error_log("SD2: Kael'Thas One or more advisors missing, Skipping Phases 1-3");

            DoScriptText(SAY_PHASE4_INTRO2, m_creature);

            Phase = 4;

            pInstance->SetData(DATA_KAELTHASEVENT, 4);

            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            if (Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                AttackStart(target);

        }
        else
        {
            PrepareAdvisors();

            DoScriptText(SAY_INTRO, m_creature);

            pInstance->SetData(DATA_KAELTHASEVENT, 1);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            PhaseSubphase = 0;
            Phase_Timer = 23000;
            Phase = 1;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && who->isTargetableForAttack() &&
            m_creature->IsHostileTo(who) && who->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                if (!m_creature->getVictim() && Phase >= 4)
                {
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                    AttackStart(who);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    if (pInstance && !pInstance->GetData(DATA_KAELTHASEVENT) && !Phase)
                        StartEvent();

                    who->SetInCombatWith(m_creature);
                    m_creature->AddThreat(who, 0.0f);
                }
            }
        }
    }

    void Aggro(Unit *who)
    {
        if (pInstance && !pInstance->GetData(DATA_KAELTHASEVENT) && !Phase)
            StartEvent();
    }

    void KilledUnit()
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        DoScriptText(SAY_DEATH, m_creature);

        if (pInstance)
            pInstance->SetData(DATA_KAELTHASEVENT, 0);

        for(uint8 i = 0; i < 4; i++)
        {
            if (Creature *pCreature = (Creature*)Unit::GetUnit((*m_creature), AdvisorGuid[i]))
                pCreature->DealDamage(pCreature, pCreature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Phase 1
        switch (Phase)
        {
            case 1:
            {
                Unit *target;
                Creature* Advisor;

                //Subphase switch
                switch(PhaseSubphase)
                {
                    //Subphase 1 - Start
                    case 0:
                        if (Phase_Timer < diff)
                        {
                            DoScriptText(SAY_INTRO_THALADRED, m_creature);

                            //start advisor within 7 seconds
                            Phase_Timer = 7000;
                            PhaseSubphase++;
                        }else Phase_Timer -= diff;
                        break;

                    //Subphase 1 - Unlock advisor
                    case 1:
                        if(Phase_Timer < diff)
                        {
                            Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[0]));

                            if (Advisor)
                            {
                                Advisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                Advisor->setFaction(m_creature->getFaction());

                                target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                                if (target)
                                    Advisor->AI()->AttackStart(target);
                            }

                            PhaseSubphase++;
                        }else Phase_Timer -= diff;
                        break;

                    //Subphase 2 - Start
                    case 2:
                        Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[0]));
                        if (Advisor && (Advisor->getStandState() == UNIT_STAND_STATE_DEAD))
                        {
                            DoScriptText(SAY_INTRO_SANGUINAR, m_creature);

                            //start advisor within 12.5 seconds
                            Phase_Timer = 12500;
                            PhaseSubphase++;
                        }
                        break;

                    //Subphase 2 - Unlock advisor
                    case 3:
                        if (Phase_Timer < diff)
                        {
                            Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[1]));

                            if (Advisor)
                            {
                                Advisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                Advisor->setFaction(m_creature->getFaction());

                                target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                                if (target)
                                    Advisor->AI()->AttackStart(target);
                            }

                            PhaseSubphase++;
                        }else Phase_Timer -= diff;
                        break;

                    //Subphase 3 - Start
                    case 4:
                        Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[1]));
                        if (Advisor && (Advisor->getStandState() == UNIT_STAND_STATE_DEAD))
                        {
                            DoScriptText(SAY_INTRO_CAPERNIAN, m_creature);

                            //start advisor within 7 seconds
                            Phase_Timer = 7000;
                            PhaseSubphase++;
                        }
                        break;

                    //Subphase 3 - Unlock advisor
                    case 5:
                        if (Phase_Timer < diff)
                        {
                            Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[2]));

                            if (Advisor)
                            {
                                Advisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                Advisor->setFaction(m_creature->getFaction());

                                target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                                if(target)
                                    Advisor->AI()->AttackStart(target);
                            }

                            PhaseSubphase++;
                        }else Phase_Timer -= diff;
                        break;

                    //Subphase 4 - Start
                    case 6:
                        Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[2]));
                        if (Advisor && (Advisor->getStandState() == UNIT_STAND_STATE_DEAD))
                        {
                            DoScriptText(SAY_INTRO_TELONICUS, m_creature);

                            //start advisor within 8.4 seconds
                            Phase_Timer = 8400;
                            PhaseSubphase++;
                        }
                        break;

                    //Subphase 4 - Unlock advisor
                    case 7:
                        if (Phase_Timer < diff)
                        {
                            Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[3]));

                            if (Advisor)
                            {
                                Advisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                Advisor->setFaction(m_creature->getFaction());

                                target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                                if (target)
                                    Advisor->AI()->AttackStart(target);
                            }

                            Phase_Timer = 3000;
                            PhaseSubphase++;
                        }else Phase_Timer -= diff;
                        break;

                    //End of phase 1
                    case 8:
                        Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[3]));
                        if (Advisor && (Advisor->getStandState() == UNIT_STAND_STATE_DEAD))
                        {
                            Phase = 2;
                            pInstance->SetData(DATA_KAELTHASEVENT, 2);

                            DoScriptText(SAY_PHASE2_WEAPON, m_creature);

                            PhaseSubphase = 0;
                            Phase_Timer = 3500;
                            DoCast(m_creature, SPELL_SUMMON_WEAPONS);
                        }
                        break;
                }
            }break;

            case 2:
            {
                if (PhaseSubphase == 0)
                {
                    if (Phase_Timer < diff)
                    {
                        PhaseSubphase = 1;
                    }else Phase_Timer -= diff;
                }

                //Spawn weapons
                if (PhaseSubphase == 1)
                {
                    Unit* Target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                    Creature* Weapon;
                    for (uint32 i = 0; i < 7; i++)
                    {
                        Weapon = m_creature->SummonCreature(((uint32)KaelthasWeapons[i][0]),KaelthasWeapons[i][1],KaelthasWeapons[i][2],KaelthasWeapons[i][3],0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000);

                        if (!Weapon)
                            error_log("SD2: Kael'thas weapon %i could not be spawned", i);
                        else
                        {
                            Weapon->setFaction(m_creature->getFaction());
                            Weapon->AI()->AttackStart(Target);
                            Weapon->CastSpell(Weapon, SPELL_WEAPON_SPAWN, false);
                        }
                    }

                    PhaseSubphase = 2;
                    Phase_Timer = TIME_PHASE_2_3;
                }

                if (PhaseSubphase == 2)
                {
                    if (Phase_Timer < diff)
                    {
                        DoScriptText(SAY_PHASE3_ADVANCE, m_creature);
                        pInstance->SetData(DATA_KAELTHASEVENT, 3);
                        Phase = 3;
                        PhaseSubphase = 0;
                    }else Phase_Timer -= diff;
                }
            }break;

            case 3:
            {
                if (PhaseSubphase == 0)
                {
                    //Respawn advisors
                    Unit* Target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                    Creature* Advisor;
                    for (uint32 i = 0; i < 4; i++)
                    {
                        Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[i]));
                        if (!Advisor)
                            error_log("SD2: Kael'Thas Advisor %u does not exist. Possibly despawned? Incorrectly Killed?", i);
                        else ((advisorbase_ai*)Advisor->AI())->Revive(Target);
                    }

                    PhaseSubphase = 1;
                    Phase_Timer = TIME_PHASE_3_4;
                }

                if (Phase_Timer < diff)
                {
                    DoScriptText(SAY_PHASE4_INTRO2, m_creature);
                    Phase = 4;

                    pInstance->SetData(DATA_KAELTHASEVENT, 4);

                    // Sometimes people can collect Aggro in Phase 1-3. Reset threat before releasing Kael.
                    DoResetThreat();

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                    if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        AttackStart(target);

                    Phase_Timer = 30000;
                }else Phase_Timer -= diff;
            }
            break;

            case 4:
            case 5:
            case 6:
            {
                //Return since we have no target
                if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
                    return;

                //Fireball_Timer
                if (!InGravityLapse && !ChainPyros && Phase != 5)
                {
                    if (Fireball_Timer < diff)
                    {
                        if (!IsCastingFireball)
                        {
                            if (!m_creature->IsNonMeleeSpellCasted(false))
                            {
                                //interruptable
                                m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
                                int32 dmg = 20000+rand()%5000;
                                m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_FIREBALL, &dmg, 0, 0, false);
                                IsCastingFireball = true;
                                Fireball_Timer = 2500;
                            }
                        }
                        else
                        {
                            //apply resistance
                            m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
                            IsCastingFireball = false;
                            Fireball_Timer = 5000+rand()%10000;
                        }
                    }else Fireball_Timer -= diff;

                    //ArcaneDisruption_Timer
                    if (ArcaneDisruption_Timer < diff)
                    {
                        DoCast(m_creature->getVictim(), SPELL_ARCANE_DISRUPTION, true);
                        ArcaneDisruption_Timer = 60000;
                    }else ArcaneDisruption_Timer -= diff;

                    if (FlameStrike_Timer < diff)
                    {
                        if (Unit* pUnit = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            DoCast(pUnit, SPELL_FLAME_STRIKE);

                        FlameStrike_Timer = 30000;
                    }FlameStrike_Timer -= diff;

                    if (MindControl_Timer < diff)
                    {
                        if (m_creature->getThreatManager().getThreatList().size() >= 2)
                            for (uint32 i = 0; i < 3; i++)
                        {
                            debug_log("SD2: Kael'Thas mind control not supported.");
                            //DoCast(pUnit, SPELL_MIND_CONTROL);
                        }

                        MindControl_Timer = 60000;
                    }MindControl_Timer -= diff;
                }

                //Phoenix_Timer
                if (Phoenix_Timer < diff)
                {
                    DoCast(m_creature, SPELL_PHOENIX_ANIMATION);
                    Creature *Phoenix = DoSpawnCreature(PHOENIX, 0, 0, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);

                    if (Phoenix)
                    {
                        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            Phoenix->AI()->AttackStart(target);
                    }else error_log("SD2: Kael'Thas Phoenix could not be spawned");

                    switch(rand()%2)
                    {
                        case 0: DoScriptText(SAY_SUMMON_PHOENIX1, m_creature); break;
                        case 1: DoScriptText(SAY_SUMMON_PHOENIX2, m_creature); break;
                    }

                    Phoenix_Timer = 60000;
                }else Phoenix_Timer -= diff;

                //Phase 4 specific spells
                if (Phase == 4)
                {
                    if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 50)
                    {
                        pInstance->SetData(DATA_KAELTHASEVENT, 4);
                        Phase = 5;
                        Phase_Timer = 10000;

                        DoScriptText(SAY_PHASE5_NUTS, m_creature);

                        m_creature->StopMoving();
                        m_creature->GetMotionMaster()->Clear();
                        m_creature->GetMotionMaster()->MoveIdle();
                        m_creature->Relocate(GRAVITY_X, GRAVITY_Y, GRAVITY_Z, 0);
                        m_creature->SendMonsterMove(GRAVITY_X, GRAVITY_Y,GRAVITY_Z, 0, 0, 0);

                        m_creature->InterruptNonMeleeSpells(false);
                        DoCast(m_creature, SPELL_FULLPOWER);
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }

                    //ShockBarrier_Timer
                    if (ShockBarrier_Timer < diff)
                    {
                        DoCast(m_creature, SPELL_SHOCK_BARRIER);
                        ChainPyros = true;
                        PyrosCasted = 0;
                        ShockBarrier_Timer = 60000;
                    }else ShockBarrier_Timer -= diff;

                    //Chain Pyros (3 of them max)
                    if (ChainPyros && !m_creature->IsNonMeleeSpellCasted(false))
                    {
                        if (PyrosCasted < 3)
                        {
                            DoCast(m_creature->getVictim(), SPELL_PYROBLAST);
                            PyrosCasted++;

                        }
                        else
                        {
                            ChainPyros = false;
                            Fireball_Timer = 2500;
                            ArcaneDisruption_Timer = 60000;
                        }
                    }
                }

                if (Phase == 5)
                {
                    if (Phase_Timer < diff)
                    {
                        m_creature->InterruptNonMeleeSpells(false);
                        m_creature->RemoveAurasDueToSpell(SPELL_FULLPOWER);

                        DoCast(m_creature, SPELL_EXPLODE);

                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        Phase = 6;
                        AttackStart(m_creature->getVictim());
                    }else Phase_Timer -= diff;
                }

                //Phase 5
                if (Phase == 6)
                {

                    //GravityLapse_Timer
                    if (GravityLapse_Timer < diff)
                    {
                        std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
                        switch(GravityLapse_Phase)
                        {
                            case 0:
                                m_creature->StopMoving();
                                m_creature->GetMotionMaster()->Clear();
                                m_creature->GetMotionMaster()->MoveIdle();
                                m_creature->Relocate(GRAVITY_X, GRAVITY_Y, GRAVITY_Z, 0);
                                m_creature->SendMonsterMove(GRAVITY_X, GRAVITY_Y, GRAVITY_Z, 0, 0, 0);

                                // 1) Kael'thas will portal the whole raid right into his body
                                for (i = m_creature->getThreatManager().getThreatList().begin(); i!= m_creature->getThreatManager().getThreatList().end();++i)
                                {
                                    Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                                    if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                                    {
                                        //Use work around packet to prevent player from being dropped from combat
                                        DoTeleportPlayer(pUnit, GRAVITY_X, GRAVITY_Y, GRAVITY_Z, pUnit->GetOrientation());
                                    }
                                }

                                GravityLapse_Timer = 500;
                                ++GravityLapse_Phase;
                                InGravityLapse = true;
                                ShockBarrier_Timer = 1000;
                                NetherBeam_Timer = 5000;
                                break;

                            case 1:
                                switch(rand()%2)
                                {
                                    case 0: DoScriptText(SAY_GRAVITYLAPSE1, m_creature); break;
                                    case 1: DoScriptText(SAY_GRAVITYLAPSE2, m_creature); break;
                                }

                                // 2) At that point he will put a Gravity Lapse debuff on everyone
                                for (i = m_creature->getThreatManager().getThreatList().begin(); i!= m_creature->getThreatManager().getThreatList().end();i++)
                                {
                                    if (Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid()))
                                    {
                                        m_creature->CastSpell(pUnit, SPELL_KNOCKBACK, true);
                                        //Gravity lapse - needs an exception in Spell system to work

                                        pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE, true, 0, 0, m_creature->GetGUID());
                                        pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE_AURA, true, 0, 0, m_creature->GetGUID());

                                        //Using packet workaround
                                        WorldPacket data(12);
                                        data.SetOpcode(SMSG_MOVE_SET_CAN_FLY);
                                        data.append(pUnit->GetPackGUID());
                                        data << uint32(0);
                                        pUnit->SendMessageToSet(&data, true);
                                    }
                                }
                                GravityLapse_Timer = 10000;
                                GravityLapse_Phase++;
                                break;

                            case 2:
                                //Cast nether vapor aura on self
                                m_creature->InterruptNonMeleeSpells(false);
                                DoCast(m_creature, SPELL_NETHER_VAPOR);

                                GravityLapse_Timer = 20000;
                                GravityLapse_Phase++;
                                break;

                            case 3:
                                //Remove flight
                                for (i = m_creature->getThreatManager().getThreatList().begin(); i!= m_creature->getThreatManager().getThreatList().end();i++)
                                {
                                    if (Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid()))
                                    {
                                        //Using packet workaround
                                        WorldPacket data(12);
                                        data.SetOpcode(SMSG_MOVE_UNSET_CAN_FLY);
                                        data.append(pUnit->GetPackGUID());
                                        data << uint32(0);
                                        pUnit->SendMessageToSet(&data, true);
                                    }
                                }

                                m_creature->RemoveAurasDueToSpell(SPELL_NETHER_VAPOR);
                                InGravityLapse = false;
                                GravityLapse_Timer = 60000;
                                GravityLapse_Phase = 0;
                                AttackStart(m_creature->getVictim());
                                break;
                        }
                    }else GravityLapse_Timer -= diff;

                    if (InGravityLapse)
                    {
                        //ShockBarrier_Timer
                        if (ShockBarrier_Timer < diff)
                        {
                            DoCast(m_creature, SPELL_SHOCK_BARRIER);
                            ShockBarrier_Timer = 20000;
                        }else ShockBarrier_Timer -= diff;

                        //NetherBeam_Timer
                        if (NetherBeam_Timer < diff)
                        {
                            if (Unit* pUnit = SelectUnit(SELECT_TARGET_RANDOM, 0))
                                DoCast(pUnit, SPELL_NETHER_BEAM);

                            NetherBeam_Timer = 4000;
                        }else NetherBeam_Timer -= diff;
                    }
                }

                if (!InGravityLapse)
                    DoMeleeAttackIfReady();
            }
        }
    }
};

//Thaladred the Darkener AI
struct MANGOS_DLL_DECL boss_thaladred_the_darkenerAI : public advisorbase_ai
{
    boss_thaladred_the_darkenerAI(Creature *c) : advisorbase_ai(c) {}

    uint32 Gaze_Timer;
    uint32 Silence_Timer;
    uint32 PsychicBlow_Timer;

    void Reset()
    {
        Gaze_Timer = 100;
        Silence_Timer = 20000;
        PsychicBlow_Timer = 10000;

        advisorbase_ai::Reset();
    }

    void Aggro(Unit *who)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (!who || FakeDeath)
            return;

        DoScriptText(SAY_THALADRED_AGGRO, m_creature);
        m_creature->AddThreat(who, 5000000.0f);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_THALADRED_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        advisorbase_ai::UpdateAI(diff);

        //Faking death, don't do anything
        if (FakeDeath)
            return;

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Gaze_Timer
        if (Gaze_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
            {
                DoResetThreat();
                m_creature->AddThreat(target, 5000000.0f);
                DoScriptText(EMOTE_THALADRED_GAZE, m_creature, target);
                Gaze_Timer = 8500;
            }
        }else Gaze_Timer -= diff;

        //Silence_Timer
        if (Silence_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SILENCE);
            Silence_Timer = 20000;
        }else Silence_Timer -= diff;

        //PsychicBlow_Timer
        if (PsychicBlow_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_PSYCHIC_BLOW);
            PsychicBlow_Timer = 20000+rand()%5000;
        }else PsychicBlow_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Lord Sanguinar AI
struct MANGOS_DLL_DECL boss_lord_sanguinarAI : public advisorbase_ai
{
    boss_lord_sanguinarAI(Creature *c) : advisorbase_ai(c){}

    uint32 Fear_Timer;

    void Reset()
    {
        Fear_Timer = 20000;
        advisorbase_ai::Reset();
    }

    void Aggro(Unit *who)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (!who || FakeDeath)
            return;

        DoScriptText(SAY_SANGUINAR_AGGRO, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_SANGUINAR_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        advisorbase_ai::UpdateAI(diff);

        //Faking death, don't do anything
        if (FakeDeath)
            return;

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Fear_Timer
        if (Fear_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_BELLOWING_ROAR);
            Fear_Timer = 25000+rand()%10000;                //approximately every 30 seconds
        }else Fear_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Grand Astromancer Capernian AI
struct MANGOS_DLL_DECL boss_grand_astromancer_capernianAI : public advisorbase_ai
{
    boss_grand_astromancer_capernianAI(Creature *c) : advisorbase_ai(c){}

    uint32 Fireball_Timer;
    uint32 Conflagration_Timer;
    uint32 ArcaneExplosion_Timer;
    uint32 Yell_Timer;
    bool Yell;

    void Reset()
    {
        Fireball_Timer = 2000;
        Conflagration_Timer = 20000;
        ArcaneExplosion_Timer = 5000;
        Yell_Timer = 2000;
        Yell = false;

        advisorbase_ai::Reset();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_CAPERNIAN_DEATH, m_creature);
    }

    void AttackStart(Unit* who)
    {
        if (!who || FakeDeath || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (m_creature->Attack(who, true))
        {
            m_creature->AddThreat(who, 0.0f);
            m_creature->SetInCombatWith(who);
            who->SetInCombatWith(m_creature);

            if (!InCombat)
            {
                InCombat = true;
                Aggro(who);
            }

            DoStartMovement(who, CAPERNIAN_DISTANCE, M_PI/2);
        }
    }

    void Aggro(Unit *who)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (!who || FakeDeath)
            return;
    }

    void UpdateAI(const uint32 diff)
    {
        advisorbase_ai::UpdateAI(diff);

        //Faking Death, don't do anything
        if (FakeDeath)
            return;

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Yell_Timer
        if (!Yell)
        {
            if (Yell_Timer < diff)
            {
                DoScriptText(SAY_CAPERNIAN_AGGRO, m_creature);
                Yell = true;
            }else Yell_Timer -= diff;
        }

        //Fireball_Timer
        if (Fireball_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_CAPERNIAN_FIREBALL);
            Fireball_Timer = 4000;
        }else Fireball_Timer -= diff;

        //Conflagration_Timer
        if (Conflagration_Timer < diff)
        {
            Unit *target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if (target && m_creature->IsWithinDistInMap(target, 30))
                DoCast(target, SPELL_CONFLAGRATION);
            else
                DoCast(m_creature->getVictim(), SPELL_CONFLAGRATION);

            Conflagration_Timer = 10000+rand()%5000;
        }else Conflagration_Timer -= diff;

        //ArcaneExplosion_Timer
        if (ArcaneExplosion_Timer < diff)
        {
            bool InMeleeRange = false;
            Unit *target = NULL;
            std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
            for (std::list<HostilReference*>::iterator i = m_threatlist.begin(); i!= m_threatlist.end();++i)
            {
                Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                                                            //if in melee range
                if (pUnit && pUnit->IsWithinDistInMap(m_creature, 5))
                {
                    InMeleeRange = true;
                    target = pUnit;
                    break;
                }
            }

            if (InMeleeRange)
                DoCast(target, SPELL_ARCANE_EXPLOSION);

            ArcaneExplosion_Timer = 4000+rand()%2000;
        }else ArcaneExplosion_Timer -= diff;

        //Do NOT deal any melee damage.
    }
};

//Master Engineer Telonicus AI
struct MANGOS_DLL_DECL boss_master_engineer_telonicusAI : public advisorbase_ai
{
    boss_master_engineer_telonicusAI(Creature *c) : advisorbase_ai(c){}

    uint32 Bomb_Timer;
    uint32 RemoteToy_Timer;

    void Reset()
    {
        Bomb_Timer = 10000;
        RemoteToy_Timer = 5000;

        advisorbase_ai::Reset();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_TELONICUS_DEATH, m_creature);
    }

    void Aggro(Unit *who)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (!who || FakeDeath)
            return;

        DoScriptText(SAY_TELONICUS_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        advisorbase_ai::UpdateAI(diff);

        //Faking Death, do nothing
        if (FakeDeath)
            return;

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Bomb_Timer
        if (Bomb_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_BOMB);
            Bomb_Timer = 25000;
        }else Bomb_Timer -= diff;

        //RemoteToy_Timer
        if (RemoteToy_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(target, SPELL_REMOTE_TOY);

            RemoteToy_Timer = 10000+rand()%5000;
        }else RemoteToy_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Flame Strike AI
struct MANGOS_DLL_DECL mob_kael_flamestrikeAI : public ScriptedAI
{
    mob_kael_flamestrikeAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Timer;
    bool Casting;
    bool KillSelf;

    void Reset()
    {
        Timer = 5000;
        Casting = false;
        KillSelf = false;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->setFaction(14);
    }

    void Aggro(Unit *who)
    {
    }

    void MoveInLineOfSight(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Casting)
        {
            DoCast(m_creature, SPELL_FLAME_STRIKE_VIS);
            Casting = true;
        }

        //Timer
        if (Timer < diff)
        {
            if (!KillSelf)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCast(m_creature, SPELL_FLAME_STRIKE_DMG);
            }else m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

            KillSelf = true;
            Timer = 1000;
        }else Timer -= diff;
    }
};

//Phoenix AI
struct MANGOS_DLL_DECL mob_phoenix_tkAI : public ScriptedAI
{
    mob_phoenix_tkAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Cycle_Timer;

    void Reset()
    {
        Cycle_Timer = 2000;
        m_creature->CastSpell(m_creature,SPELL_BURN,true);
    }

    void Aggro(Unit *who) { }

    void JustDied(Unit* killer)
    {
        //is this spell in use anylonger?
        //m_creature->CastSpell(m_creature,SPELL_EMBER_BLAST,true);
        m_creature->SummonCreature(PHOENIX_EGG,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN,16000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (Cycle_Timer < diff)
        {
            //spell Burn should possible do this, but it doesn't, so do this for now.
            uint32 dmg = urand(4500,5500);
            if (m_creature->GetHealth() > dmg)
                m_creature->SetHealth(uint32(m_creature->GetHealth()-dmg));
            Cycle_Timer = 2000;
        }else Cycle_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Phoenix Egg AI
struct MANGOS_DLL_DECL mob_phoenix_egg_tkAI : public ScriptedAI
{
    mob_phoenix_egg_tkAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Rebirth_Timer;

    void Reset()
    {
        Rebirth_Timer = 15000;
    }

    //ignore any
    void MoveInLineOfSight(Unit* who) { return; }

    void AttackStart(Unit* who)
    {
        if (m_creature->Attack(who, false))
        {
            m_creature->SetInCombatWith(who);
            who->SetInCombatWith(m_creature);

            if (!InCombat)
            {
                InCombat = true;
                Aggro(who);
            }

            DoStartNoMovement(who);
        }
    }

    void Aggro(Unit *who) { }

    void JustSummoned(Creature* summoned)
    {
        summoned->AddThreat(m_creature->getVictim(), 0.0f);
        summoned->CastSpell(summoned,SPELL_REBIRTH,false);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Rebirth_Timer)
            return;

        if (Rebirth_Timer <= diff)
        {
            m_creature->SummonCreature(PHOENIX,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_CORPSE_DESPAWN,5000);
            Rebirth_Timer = 0;
        }else Rebirth_Timer -= diff;
    }
};

CreatureAI* GetAI_boss_kaelthas(Creature *_Creature)
{
    return new boss_kaelthasAI (_Creature);
}

CreatureAI* GetAI_boss_thaladred_the_darkener(Creature *_Creature)
{
    return new boss_thaladred_the_darkenerAI (_Creature);
}

CreatureAI* GetAI_boss_lord_sanguinar(Creature *_Creature)
{
    return new boss_lord_sanguinarAI (_Creature);
}

CreatureAI* GetAI_boss_grand_astromancer_capernian(Creature *_Creature)
{
    return new boss_grand_astromancer_capernianAI (_Creature);
}

CreatureAI* GetAI_boss_master_engineer_telonicus(Creature *_Creature)
{
    return new boss_master_engineer_telonicusAI (_Creature);
}

CreatureAI* GetAI_mob_kael_flamestrike(Creature *_Creature)
{
    return new mob_kael_flamestrikeAI (_Creature);
}

CreatureAI* GetAI_mob_phoenix_tk(Creature *_Creature)
{
    return new mob_phoenix_tkAI (_Creature);
}

CreatureAI* GetAI_mob_phoenix_egg_tk(Creature *_Creature)
{
    return new mob_phoenix_egg_tkAI (_Creature);
}

void AddSC_boss_kaelthas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kaelthas";
    newscript->GetAI = &GetAI_boss_kaelthas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_thaladred_the_darkener";
    newscript->GetAI = &GetAI_boss_thaladred_the_darkener;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_lord_sanguinar";
    newscript->GetAI = &GetAI_boss_lord_sanguinar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_grand_astromancer_capernian";
    newscript->GetAI = &GetAI_boss_grand_astromancer_capernian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_master_engineer_telonicus";
    newscript->GetAI = &GetAI_boss_master_engineer_telonicus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name= "mob_kael_flamestrike";
    newscript->GetAI = &GetAI_mob_kael_flamestrike;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_phoenix_tk";
    newscript->GetAI = &GetAI_mob_phoenix_tk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_phoenix_egg_tk";
    newscript->GetAI = &GetAI_mob_phoenix_egg_tk;
    newscript->RegisterSelf();
}
