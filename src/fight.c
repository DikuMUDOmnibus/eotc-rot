/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Strfeldt, Tom Madsen, and Katja Nyboe.    *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*    ROM 2.4 is copyright 1993-1998 Russ Taylor                             *
*    ROM has been brought to you by the ROM consortium                      *
*        Russ Taylor (rtaylor@hypercube.org)                                *
*        Gabrielle Taylor (gtaylor@hypercube.org)                           *
*        Brian Moore (zump@rom.org)                                         *
*    By using this code, you have agreed to follow the terms of the         *
*    ROM license, in the file Rom24/doc/rom.license                         *
****************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "disease.h"
#include "recycle.h"
#include "igen.h"

/*
 * Local functions.
 */
void dam_message ( CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt, bool immune );
void check_assist args ((CHAR_DATA * ch, CHAR_DATA * victim));
bool check_dodge args ((CHAR_DATA * ch, CHAR_DATA * victim));
void check_killer args ((CHAR_DATA * ch, CHAR_DATA * victim));
bool check_parry args ((CHAR_DATA * ch, CHAR_DATA * victim));
bool check_shield_block args ((CHAR_DATA * ch, CHAR_DATA * victim));
bool check_phase     args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void death_cry args ((CHAR_DATA * ch));
void group_gain args ((CHAR_DATA * ch, CHAR_DATA * victim));
int xp_compute args ((CHAR_DATA * gch, CHAR_DATA * victim, int total_levels));
bool is_safe args ((CHAR_DATA * ch, CHAR_DATA * victim));
void make_corpse args ((CHAR_DATA * ch));
void one_hit args ((CHAR_DATA * ch, CHAR_DATA * victim, int dt, bool secondary));
void mob_hit args ((CHAR_DATA * ch, CHAR_DATA * victim, int dt));
void raw_kill args ((CHAR_DATA * victim)); // added Killer
void set_fighting args ((CHAR_DATA * ch, CHAR_DATA * victim));
void disarm args ((CHAR_DATA * ch, CHAR_DATA * victim));

/* Xrakisis */
void bite args ((CHAR_DATA *ch, CHAR_DATA *victim));
void claws args ((CHAR_DATA *ch, CHAR_DATA *victim));
bool check_critical args ((CHAR_DATA *ch, CHAR_DATA *victim));
bool check_counter args ((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt));
int  critical_strike args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
bool check_force_shield args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_static_shield args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_flame_shield args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void update_casting args((CHAR_DATA *ch ));
bool can_counter     args( ( CHAR_DATA *ch, bool forreal) );
bool pole_special	(CHAR_DATA * ch, CHAR_DATA * victim);
int	 axe_special	(CHAR_DATA * ch, CHAR_DATA * victim, int dam);

OBJ_DATA * rand_obj args( ( CHAR_DATA *ch, int mob_level ));


/* Stance stuff - Rhaelar */
bool	stance_checked = FALSE;
const	char*	stance_name []  =
{
	"None", "Viper", "Crane", "Crab", "Mongoose", "Bull",
	"Mantis", "Dragon", "Tiger", "Monkey", "Swallow"
};

void skillstance( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf        [MIL];
    char bufskill   [25];
    int stance;

    stance = victim->stance[0];
    
    if (stance < 1 || stance > 10)
    {
	send_to_char("You aren't in a stance!\n\r",ch);
	return;
    }

         if (victim->stance[stance] <= 0  ) sprintf(bufskill,"completely unskilled in");
    else if (victim->stance[stance] <= 12 ) sprintf(bufskill,"an apprentice of");
    else if (victim->stance[stance] <= 26 ) sprintf(bufskill,"a trainee of");
    else if (victim->stance[stance] <= 39 ) sprintf(bufskill,"a student of");
    else if (victim->stance[stance] <= 51 ) sprintf(bufskill,"fairly experienced in");
    else if (victim->stance[stance] <= 64 ) sprintf(bufskill,"well trained in");
    else if (victim->stance[stance] <= 76 ) sprintf(bufskill,"highly skilled in");
    else if (victim->stance[stance] <= 85 ) sprintf(bufskill,"an expert of");
    else if (victim->stance[stance] <= 99 ) sprintf(bufskill,"a master of");
    else if (victim->stance[stance] >= 100) sprintf(bufskill,"a grand master of");
    else
    {

        send_to_char("You aren't in a stance!\n\r",ch);

        return;

    }

    if (ch == victim)
    	sprintf(buf,"You are %s the %s stance [%d%%].",bufskill,

						stance_name[stance],victim->stance[stance]);
    else
    	sprintf(buf,"$N is %s the %s stance [%d%%].",bufskill,

						stance_name[stance] ,victim->stance[stance]);
    act( buf,ch,NULL,victim,TO_CHAR);

    return;

}

int dambonus( CHAR_DATA *ch, CHAR_DATA *victim, int dam, bool realdam)
{
    if (dam < 1) return 0;
    if (ch->stance[0] < 1) return dam;

	if ( IS_STANCE(ch, STANCE_BULL) || IS_STANCE(ch, STANCE_DRAGON)
					|| IS_STANCE(ch, STANCE_TIGER) )

		{
		if (!can_counter(victim, realdam))
			{
 			if (realdam && !(stance_checked)) improve_stance(ch);
			dam += (dam * ch->stance[ch->stance[0]]) / 100;
			}
		}

	else if ( IS_STANCE(ch, STANCE_MONKEY) )
	{
	    int mindam = (int) (dam * 0.25);
	    dam *= (ch->stance[STANCE_MONKEY]+1) / 100;
	    if (dam < mindam) dam = mindam;
	}

	else if (ch->stance[ch->stance[0]] < 100)  dam = (int) (dam * 0.5);
    
    return dam;
}



int damreduce( CHAR_DATA *ch, CHAR_DATA *victim, int dam, bool realdam)
{

	if ( IS_STANCE(victim, STANCE_CRAB) || IS_STANCE(victim, STANCE_DRAGON)
				|| IS_STANCE(victim, STANCE_SWALLOW) )
    {
    	if (!can_counter(ch, realdam))
		{
			if (realdam && !(stance_checked)) improve_stance(victim);
			dam  = dam * 100 / (100 + victim->stance[victim->stance[0]]);
		}
    }
	return dam;
	}

bool can_counter( CHAR_DATA *ch, bool forreal )
{
    if (
				IS_STANCE(ch, STANCE_MONKEY))
		{
 		if (forreal && !(stance_checked)) improve_stance(ch);
		if (number_percent() < ch->stance[ STANCE_MONKEY ])
		return TRUE;
		}
    return FALSE;

}

bool can_bypass( CHAR_DATA *ch, CHAR_DATA *victim )
{
	if (IS_STANCE(ch, STANCE_VIPER) ||
		IS_STANCE(ch, STANCE_MANTIS) ||
		IS_STANCE(ch, STANCE_TIGER))
			if (!can_counter(victim, TRUE))
			{
 				if (!(stance_checked))	improve_stance(ch);
				if (number_percent() < ch->stance[ch->stance[0]])
					return TRUE;
			}
    return FALSE;
}

void improve_stance( CHAR_DATA *ch )
{
//    OBJ_DATA *weapon;
    char buf        [MIL];
    char bufskill   [35];
    int dice1;
    int dice2;
    int dice3;
    int stance;
//	bool immune = FALSE;
//	int dam_type;
    dice1 = number_percent();
    dice2 = number_percent();
    dice3 = number_percent();

    stance = ch->stance[0];

    if (stance < 1 || stance > 10) return;

    if (ch->stance[stance] >= 100)
      {
        ch->stance[stance] = 100;
        return;
      }

	if (ch->fighting == NULL ||
		(IS_NPC(ch->fighting) && ch->fighting->pIndexData == NULL ))
		return;


   if ( !double_stance )
   {
    if ( dice3 > 2) return;
    if ( dice1 < 24 ) return;
    if ( dice2 < 24 ) return;
   }
   else
   {
    if ( dice3 > 4) return;
    if ( dice1 < 12 ) return;
    if ( dice2 < 12 ) return;
   }   
    if ( (dice1 > ch->stance[stance] && dice2 > ch->stance[stance] ) ||

	 ( dice1 == 100 || dice2 == 100 ) )
		ch->stance[stance] += 1;
    else
      return;

    send_to_char("{CYour skill at this fighting stance has improved.{0\n\r",ch);

	stance_checked = TRUE;

         if (ch->stance[stance] == 1  ) sprintf(bufskill,"an apprentice of");
    else if (ch->stance[stance] == 13 ) sprintf(bufskill,"a trainee of");
    else if (ch->stance[stance] == 25 ) sprintf(bufskill,"a student of");
    else if (ch->stance[stance] == 38 ) sprintf(bufskill,"fairly experienced in");
    else if (ch->stance[stance] == 50 ) sprintf(bufskill,"well trained in");
    else if (ch->stance[stance] == 63 ) sprintf(bufskill,"highly skilled in");
    else if (ch->stance[stance] == 75 ) sprintf(bufskill,"an expert of");
    else if (ch->stance[stance] == 84 ) sprintf(bufskill,"a master of");
    else if (ch->stance[stance] == 100) sprintf(bufskill,"a grand master of");
    else return;

    sprintf(buf,"{cYou are now {C%s {cthe {R%s {cstance{x.\n\r",bufskill,
					stance_name[stance]);
    send_to_char(buf,ch);
    return;
}


void do_stance( CHAR_DATA *ch, char *argument )
{
    char            arg      [MIL];
    int             selection;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	if (ch->stance[0] == -1)
	{
	    ch->stance[0] = -1;
	    send_to_char("You are not currently in a stance. For a list, type 'sstat'.\n\r",ch);
	}
	else
	{
	    ch->stance[0] = -1;
	    send_to_char("You relax from your fighting stance.\n\r",ch);
	    act("$n relaxes from $s fighting stance.",ch,NULL,NULL,TO_ROOM);
	}

	return;

    }
    if ( ch->stance[0] > 0)
    {
    	if ( !IS_NPC ( ch ) )
	do_stance ( ch, "" );
	else return;
    }

    if (!str_cmp(arg,"none")    ) {selection = -1;
	send_to_char("You drop into a default fighting stance.\n\r",ch);
	act("$n drops into a default fighting stance.",ch,NULL,NULL,TO_ROOM);}
    else if (!str_cmp(arg,"viper")   ) {selection = STANCE_VIPER;
	send_to_char("You arch your body into the viper fighting stance.\n\r",ch);
	act("$n arches $s body into the viper fighting stance.",ch,NULL,NULL,TO_ROOM);}
    else if (!str_cmp(arg,"crane")   ) {selection = STANCE_CRANE;
	send_to_char("You swing your body into the crane fighting stance.\n\r",ch);
	act("$n swings $s body into the crane fighting stance.",ch,NULL,NULL,TO_ROOM);}
    else if (!str_cmp(arg,"crab")    ) {selection = STANCE_CRAB;
	send_to_char("You squat down into the crab fighting stance.\n\r",ch);
	act("$n squats down into the crab fighting stance. ",ch,NULL,NULL,TO_ROOM);}
    else if (!str_cmp(arg,"mongoose")) {selection = STANCE_MONGOOSE;
	send_to_char("You twist into the mongoose fighting stance.\n\r",ch);
	act("$n twists into the mongoose fighting stance. ",ch,NULL,NULL,TO_ROOM);}
    else if (!str_cmp(arg,"bull")    ) {selection = STANCE_BULL;
	send_to_char("You hunch down into the bull fighting stance.\n\r",ch);
	act("$n hunches down into the bull fighting stance. ",ch,NULL,NULL,TO_ROOM);}
    else
    {
	if (!str_cmp(arg,"mantis") && ch->stance[STANCE_CRANE] >= 100 &&
	    ch->stance[STANCE_VIPER] >= 100)
	{
	    selection = STANCE_MANTIS;
	    send_to_char("You spin your body into the mantis fighting stance.\n\r",ch);
	    act("$n spins $s body into the mantis fighting stance.",ch,NULL,NULL,TO_ROOM);
	}
	else if (!str_cmp(arg,"dragon") && ch->stance[STANCE_BULL] >= 100 &&
	    ch->stance[STANCE_CRAB] >= 100)
	{
	    selection = STANCE_DRAGON;
	    send_to_char("You coil your body into the dragon fighting stance.\n\r",ch);
	    act("$n coils $s body into the dragon fighting stance.",ch,NULL,NULL,TO_ROOM);
	}
	else if (!str_cmp(arg,"tiger") && ch->stance[STANCE_BULL] >= 100 &&
	    ch->stance[STANCE_VIPER] >= 100)
	{
	    selection = STANCE_TIGER;
	    send_to_char("You lunge into the tiger fighting stance.\n\r",ch);
	    act("$n lunges into the tiger fighting stance.",ch,NULL,NULL,TO_ROOM);
	}	
	else if (!str_cmp(arg,"monkey") && ch->stance[STANCE_CRANE] >= 100 &&
	    ch->stance[STANCE_MONGOOSE] >= 100)
	{
	    selection = STANCE_MONKEY;
	    send_to_char("You rotate your body into the monkey fighting stance.\n\r",ch);
	    act("$n rotates $s body into the monkey fighting stance.",ch,NULL,NULL,TO_ROOM);
	}
	else if (!str_cmp(arg,"swallow") && ch->stance[STANCE_CRAB] >= 100 &&
	    ch->stance[STANCE_MONGOOSE] >= 100)
	{
	    selection = STANCE_SWALLOW;
	    send_to_char("You slide into the swallow fighting stance.\n\r",ch);
	    act("$n slides into the swallow fighting stance.",ch,NULL,NULL,TO_ROOM);
	}
	else
	{
	    send_to_char("Syntax is: stance <style>.\n\r",ch);
	    send_to_char("Stance being one of: None, Viper, Crane, Crab, Mongoose, Bull.\n\r",ch);
	    return;
	}
    }
    ch->stance[0] = selection;
    WAIT_STATE(ch,16);
    return;
}



bool check_stance ( CHAR_DATA *ch  )
{
if (ch->stance[0] < 1 || ch->stance[0] > 10)
	return FALSE;

	if ( ch->stance[0] == 9 ) 						
		if ((ch->fighting->stance[0] <= 0) || (ch->fighting->stance[0] == 9))
			return FALSE;
	return TRUE;
} 

void process_shields ( CHAR_DATA * ch, CHAR_DATA * victim )
{
    int dam, dt;
    int count = 0, total = 0;
    bool immune;


    if ( IS_SHIELDED ( victim, SHD_POISON ) )
    {
        if ( ch->fighting != victim )
            return;
        if ( !IS_SHIELDED ( ch, SHD_POISON ) )
        {
            dt = skill_lookup ( "poisonshield" );
            dam = number_range ( 5, 15 );
            total +=
                // damage ( victim, ch, dam, dt, DAM_POISON, TRUE,VERBOSE_SHIELD, 1 );
								damage(ch, victim, dam, dt ,DAM_POISON,TRUE);
            count++;
        }
    }
    if ( IS_SHIELDED ( victim, SHD_ICE ) )
    {
        if ( ch->fighting != victim )
            return;
        if ( !IS_SHIELDED ( ch, SHD_ICE ) )
        {
            dt = skill_lookup ( "iceshield" );
            dam = number_range ( 15, 25 );
            total +=
//                xdamage ( victim, ch, dam, dt, DAM_COLD, TRUE,
                          //VERBOSE_SHIELD, 1 );
                damage(ch,victim, dam, dt ,DAM_COLD,TRUE);

            count++;
        }
    }
    if ( IS_SHIELDED ( victim, SHD_FIRE ) )
    {
        if ( ch->fighting != victim )
            return;
        if ( !IS_SHIELDED ( ch, SHD_FIRE ) )
        {
            dt = skill_lookup ( "fireshield" );
            dam = number_range ( 25, 35 );
            total +=
                //xdamage ( victim, ch, dam, dt, DAM_FIRE, TRUE,
                          //VERBOSE_SHIELD, 1 );
                damage(ch, victim, dam, dt ,DAM_FIRE,TRUE);

            count++;

        }
    }
    if ( IS_SHIELDED ( victim, SHD_SHOCK ) )
    {
        if ( ch->fighting != victim )
            return;
        if ( !IS_SHIELDED ( ch, SHD_SHOCK ) )
        {
            dt = skill_lookup ( "shockshield" );

            dam = number_range ( 35, 45 );
            total +=
                // xdamage ( victim, ch, dam, dt, DAM_POISON, TRUE,   VERBOSE_SHIELD, 1 );
                damage(ch,victim, dam, dt ,DAM_POISON,TRUE);

            count++;
        }
    }
    if ( IS_SHIELDED ( victim, SHD_ACID ) )
    {
        if ( ch->fighting != victim )
            return;
        if ( !IS_SHIELDED ( ch, SHD_ACID ) )
        {
            dt = skill_lookup ( "acidshield" );
            dam = number_range ( 45, 85 );
            total +=
                // xdamage ( victim, ch, dam, dt, DAM_POISON, TRUE,   VERBOSE_SHIELD, 1 );
                damage(ch, victim, dam, dt ,DAM_POISON,TRUE);

            count++;
        }
    }
    if ( IS_SHIELDED ( victim, SHD_BRIAR ) )
    {
        if ( ch->fighting != victim )
            return;
        if ( !IS_SHIELDED ( ch, SHD_BRIAR ) )
        {
            dt = skill_lookup ( "briarshield" );
            dam = number_range ( 10, 25 );
            total +=
                //xdamage ( victim, ch, dam, dt, DAM_POISON, TRUE,
                          //VERBOSE_SHIELD, 1 );
                damage(ch,victim, dam, dt ,DAM_POISON,TRUE);

            count++;
        }
    }

    if ( ch->fighting != victim )
        return;
    if ( count > 1 )
        //dam_message ( victim, ch, total, -1, FALSE, VERBOSE_SHIELD_COMP,  FALSE );
				dam_message (victim, ch, dam, dt, immune);
    else if ( count == 1 )
        // dam_message ( victim, ch, total, -1, FALSE, VERBOSE_SHIELD_COMP, TRUE );
				dam_message (victim, ch, dam, dt, immune);

    return;
}




/* Stance stuff done - Rhaelar */


/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update (void)
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;

    for (ch = char_list; ch != NULL; ch = ch_next)
    {
        ch_next = ch->next;

        if ( ch->action[0] == ACTION_CASTING )
          update_casting(ch);

        if ((victim = ch->fighting) == NULL || ch->in_room == NULL)
            continue;

//        if (IS_AWAKE (ch) && ch->in_room == victim->in_room)
//	{
	if (--ch->init <= 0)
	{
            multi_hit (ch, victim, TYPE_UNDEFINED);
						process_shields ( ch, victim );
	    			ch->init = get_speed(ch);
	}
	else continue;
//	}

        if (IS_AWAKE (ch) && ch->in_room == victim->in_room)
		{
			//Using 'Psyche Up'...
			if (ch->tension > 0)
			{
				AFFECT_DATA * af;

				//Tweak these to your liking.
				sh_int tension_data[5][3] =
				{
					{100,  0,  0},
					{100,  5,  0},
					{ 90, 10,  5},
					{ 80, 15, 10},
					{ 40, 20, 15},
				};

				//Super High Tension has lower chance in even battles.
				sh_int chance;
				chance = tension_data[ch->tension][0];
//				chance = ch->tension;
				if (ch->tension == 4 && ch->fighting->level - ch->level < 5)
					chance -= 10;

				//Tension increased.
				if (number_percent () < chance)
				{
					//Boost hitroll and damroll the easy way.
					af = new_affect ();
					af->where = TO_AFFECTS;
					af->type = gsn_tension;
					af->duration = -1;
					af->bitvector = 0;
					af->level = 0;

					af->location = APPLY_DAMROLL;
					af->modifier = tension_data[ch->tension][1];
				//	af->modifier = ch->tension;
					affect_to_char (ch, af);

					af->location = APPLY_HITROLL;
					af->modifier = tension_data[ch->tension][2];
				//	af->modifier = ch->tension;
					affect_to_char (ch, af);

					++ch->tension;

					//Send the proper message.
					if (ch->tension == 4)
					{
						act ("You reach a state of high tension!",
								ch, NULL, NULL, TO_CHAR);
						act ("$n reaches a state of high tension!",
								ch, NULL, NULL, TO_ROOM);
					}
					else if (ch->tension == 5)
					{
						act ("You reach a state of super high tension!",
								ch, NULL, NULL, TO_CHAR);
						act ("$n reaches a state of super high tension!",
								ch, NULL, NULL, TO_ROOM);
					}
					else
					{
						act ("Your tension increases!", ch, NULL, NULL, TO_CHAR);
						act ("$n's tension increases!", ch, NULL, NULL, TO_ROOM);
					}

					//Automatically blow it at Super High.
					if (ch->tension > 4)
						do_tension (ch, "");
				}
				else
				{
					//Failed Super High in an even match, use it up.
					if (ch->tension == 4 && ch->fighting->level - ch->level < 5)
						do_tension (ch, "");
					//Otherwise, let them keep trying.
					else
					{
						act ("You try to psyche yourself up, but nothing happens.",
								ch, NULL, NULL, TO_CHAR);
						act ("$n tries to increase $s tension, but nothing happens.",
								ch, NULL, NULL, TO_ROOM);
					}
				}
			}
			//Not using 'Psyche Up', act normally.
			else
	            multi_hit (ch, victim, TYPE_UNDEFINED);
		}

        else
            stop_fighting (ch, FALSE);


        //if ((victim = ch->fighting) == NULL)
            //continue;

        /*
         * Fun for the whole family!
         */
        check_assist (ch, victim);

        if (IS_NPC (ch))
        {
            if (HAS_TRIGGER (ch, TRIG_FIGHT))
                mp_percent_trigger (ch, victim, NULL, NULL, TRIG_FIGHT);
            if (HAS_TRIGGER (ch, TRIG_HPCNT))
                mp_hprct_trigger (ch, victim);
        }
    }

    return;
}

/* for auto assisting */
void check_assist ( CHAR_DATA * ch, CHAR_DATA * victim )
{
    CHAR_DATA *rch, *rch_next;

/*
    for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
    {
        rch_next = rch->next_in_room;

        if ( IS_AWAKE ( rch ) && rch->fighting == NULL &&
             can_see ( rch, victim ) )
        {

            if ( !IS_NPC ( ch ) && IS_NPC ( rch ) &&
                 IS_SET ( rch->off_flags, ASSIST_PLAYERS ) &&
                 rch->level + 6 > victim->level )
            {
                do_emote ( rch, "{Rscreams and attacks!{x" );
                multi_hit ( rch, victim, TYPE_UNDEFINED );
                continue;
            }

            if ( !IS_NPC ( ch ) || IS_AFFECTED ( ch, AFF_CHARM ) )
            {
                if ( ( ( !IS_NPC ( rch ) &&
                         IS_SET ( rch->act, PLR_AUTOASSIST ) ) ||
                       IS_AFFECTED ( rch, AFF_CHARM ) ) &&
                     is_same_group ( ch, rch ) && !is_safe ( rch, victim ) )
                    multi_hit ( rch, victim, TYPE_UNDEFINED );

                continue;
            }

*/

            /* now check the NPC cases */
/*
            if ( IS_NPC ( ch ) && !IS_AFFECTED ( ch, AFF_CHARM ) )

            {
                if ( ( IS_NPC ( rch ) &&
                       IS_SET ( rch->off_flags, ASSIST_ALL ) ) ||
                     ( IS_NPC ( rch ) && rch->group &&
                       rch->group == ch->group ) || ( IS_NPC ( rch ) &&
                                                      rch->race == ch->race &&
                                                      IS_SET ( rch->off_flags,
                                                               ASSIST_RACE ) )
                     || ( IS_NPC ( rch ) &&
                          IS_SET ( rch->off_flags, ASSIST_ALIGN ) &&
                          ( ( IS_GOOD ( rch ) && IS_GOOD ( ch ) ) ||
                            ( IS_EVIL ( rch ) && IS_EVIL ( ch ) ) ||
                            ( IS_NEUTRAL ( rch ) && IS_NEUTRAL ( ch ) ) ) ) ||
                     ( rch->pIndexData == ch->pIndexData &&
                       IS_SET ( rch->off_flags, ASSIST_VNUM ) ) )

                {
                    CHAR_DATA *vch;
                    CHAR_DATA *target;
                    int number;

                    if ( number_bits ( 1 ) == 0 )
                        continue;

                    target = NULL;
                    number = 0;
                    for ( vch = ch->in_room->people; vch; vch = vch->next )
                    {
                        if ( can_see ( rch, vch ) &&
                             is_same_group ( vch, victim ) &&
                             number_range ( 0, number ) == 0 )
                        {
                            target = vch;
                            number++;
                        }
                    }

                    if ( target != NULL )
                    {
                        do_emote ( rch, "{Rscreams and attacks!{x" );
                        multi_hit ( rch, target, TYPE_UNDEFINED );
                    }
                }
            }
        }
    }
*/
}


/*
 * Do one group of attacks.
 */
void multi_hit (CHAR_DATA * ch, CHAR_DATA * victim, int dt)
{
    int chance;

    stance_checked = FALSE; /* ensure stance can improve only once per round */

    /* decrement the wait */
    if (ch->desc == NULL)
        ch->wait = UMAX (0, ch->wait - PULSE_VIOLENCE);

    if (ch->desc == NULL)
        ch->daze = UMAX (0, ch->daze - PULSE_VIOLENCE);



    /* no attacks for stunnies -- just a check */
    if (ch->position < POS_RESTING)
        return;

    if (ch->casting_spell)
	return;

        if (IS_AWAKE (ch) && ch->in_room == victim->in_room)
		{
//            multi_hit (ch, vch, TYPE_UNDEFINED);

			/*
			 * Swords will very rarely get in an extra round.
			 */
			if (get_weapon_sn (ch) == gsn_sword
				&& ch->wait <= 0)
			{
				int chance;
				CHAR_DATA * was_fighting;
				CHAR_DATA * vch;

				chance = get_skill (ch, gsn_sword) / 15;

				/*
				 * Penalty if the opponent can see.
				 */
				if (can_see (victim, ch))
					chance = chance * 3 / 5;

				for (vch = ch->in_room->people; vch != NULL;
						vch = vch->next_in_room)
				{
					if (vch->fighting == ch
						|| is_same_group (vch->fighting, ch))
					{
						if (number_percent () < chance)
						{
							was_fighting = ch->fighting;
							ch->fighting = vch;

							multi_hit (ch, vch, TYPE_UNDEFINED);
							check_improve (ch, gsn_sword, TRUE, 2);

							ch->fighting = was_fighting;
							WAIT_STATE (ch, PULSE_VIOLENCE);
							break;
						}
					}
				}
			}
		}


/*    if (IS_AFFECTED2(ch,AFF_WEAK_STUN) )
        {
 act("$CYou are too stunned to respond $N's attack.$c",
                ch,NULL,victim,TO_CHAR);
 act("$C$n is too stunned to respond your attack.$c",
                ch,NULL,victim,TO_VICT);
 act("$C$n seems to be stunned.$c",
                ch,NULL,victim,TO_NOTVICT );
         REMOVE_BIT(ch->affected_by,AFF_WEAK_STUN);
         return;
        }


    if (IS_AFFECTED2(ch,AFF_STUN) )
        {
 act("$CYou are too stunned to respond $N's attack.$c",
                ch,NULL,victim,TO_CHAR);
 act("$C$n is too stunned to respond your attack.$c",
                ch,NULL,victim,TO_VICT);
 act("$C$n seems to be stunned.$c",
                ch,NULL,victim,TO_NOTVICT);
         affect_strip(ch,gsn_power_stun);
         SET_BIT(ch->affected_by,AFF_WEAK_STUN);
         return;
        }

*/
    if (IS_NPC (ch))
    {
        mob_hit (ch, victim, dt);
        return;
    }


    one_hit (ch, victim, dt, FALSE);

    if (ch->fighting != victim)
        return;

    if (get_eq_char (ch, WEAR_SECONDARY))
    {
        one_hit( ch, victim, dt, TRUE );
        if ( ch->fighting != victim )
            return;
    }


    if (IS_AFFECTED (ch, AFF_HASTE))
        one_hit (ch, victim, dt, FALSE);

    if (ch->fighting != victim || dt == gsn_backstab || dt == gsn_circle
	|| dt == gsn_dual_backstab || dt == gsn_assassinate
	|| dt == gsn_precisionstrike || dt == gsn_dual_circle)
        return;

    chance = get_skill (ch, gsn_second_attack) / 2;

    if (IS_AFFECTED (ch, AFF_SLOW))
        chance /= 2;

    if (number_percent () < chance)
    {
        one_hit (ch, victim, dt, FALSE);
        check_improve (ch, gsn_second_attack, TRUE, 5);
        if (ch->fighting != victim)
            return;
    }

    chance = get_skill(ch,gsn_third_attack)/3;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt, FALSE);
        check_improve(ch,gsn_third_attack,TRUE,6);
        if ( ch->fighting != victim )
            return;
    }


    chance = get_skill(ch,gsn_fourth_attack)/6;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt, FALSE);
        check_improve(ch,gsn_fourth_attack,TRUE,7);
        if ( ch->fighting != victim )
            return;
    }

    chance = get_skill(ch,gsn_fifth_attack)/8;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt, FALSE);
        check_improve(ch,gsn_fifth_attack,TRUE,8);
        if ( ch->fighting != victim )
            return;
    }

    chance = get_skill(ch,gsn_sixth_attack)/8;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt, FALSE);
        check_improve(ch,gsn_sixth_attack,TRUE,8);
        if ( ch->fighting != victim )
            return;
    }

    chance = get_skill(ch,gsn_seventh_attack)/8;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt, FALSE);
        check_improve(ch,gsn_seventh_attack,TRUE,8);
        if ( ch->fighting != victim )
            return;
    }

    chance = get_skill(ch,gsn_secondary_attack) / 8;
    if ( number_percent( ) < chance )
    {
     if (get_eq_char(ch , WEAR_SECONDARY))
        {
         one_hit(ch , victim , dt, FALSE );
         check_improve(ch, gsn_secondary_attack,TRUE,2);
         if (ch->fighting != victim )
                return;
        }
    }

	/*
	 * Extra hit for daggers.
	 */
	if (get_weapon_sn (ch) == gsn_dagger)
	{
		chance = get_skill (ch, gsn_dagger) / 2;

		if (IS_AFFECTED (ch, AFF_SLOW))
			chance /= 1.5;

		if (IS_AFFECTED (ch, AFF_HASTE))
			chance *= 2;

		if (number_percent () < chance)
		{
			one_hit (ch, victim, dt, TRUE);
			check_improve (ch, gsn_dagger, TRUE, 4);

			if (ch->fighting != victim)
				return;
		}
	}

    if (!IS_NPC(ch) && ch->disease != -1) bite(ch,victim);
    if (!IS_NPC(ch) && ch->disease == 1) claws(ch,victim);



    if (IS_AFFECTED (ch, AFF_SLOW))
        chance = 0;;

    if (number_percent () < chance)
    {
        one_hit (ch, victim, dt, FALSE);
        check_improve (ch, gsn_third_attack, TRUE, 6);
        if (ch->fighting != victim)
            return;
    }

    return;
}

/* procedure for all mobile attacks */
void mob_hit (CHAR_DATA * ch, CHAR_DATA * victim, int dt)
// void mob_hit (CHAR_DATA * ch, CHAR_DATA * victim, int dt, bool secondary)
{
    int chance, number;
    CHAR_DATA *vch, *vch_next;

    one_hit (ch, victim, dt, FALSE);

    if (ch->fighting != victim)
        return;

    if (ch->casting_spell)
	return;

    /* Area attack -- BALLS nasty! */

    if (IS_SET (ch->off_flags, OFF_AREA_ATTACK))
    {
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
            vch_next = vch->next;
            if ((vch != victim && vch->fighting == ch))
                one_hit (ch, vch, dt, FALSE);
        }
    }

    if (IS_AFFECTED (ch, AFF_HASTE)
        || (IS_SET (ch->off_flags, OFF_FAST) && !IS_AFFECTED (ch, AFF_SLOW)))
        one_hit (ch, victim, dt, FALSE);

    if (ch->fighting != victim || dt == gsn_backstab || dt == gsn_dual_backstab)
     return;

    chance = get_skill (ch, gsn_second_attack) / 2;

    if (IS_AFFECTED (ch, AFF_SLOW) && !IS_SET (ch->off_flags, OFF_FAST))
        chance /= 2;

    if (number_percent () < chance)
    {
        one_hit (ch, victim, dt, FALSE);
        if (ch->fighting != victim)
            return;
    }

    chance = get_skill (ch, gsn_third_attack) / 4;

    if (IS_AFFECTED (ch, AFF_SLOW) && !IS_SET (ch->off_flags, OFF_FAST))
        chance = 0;

    if (number_percent () < chance)
    {
        one_hit (ch, victim, dt, FALSE);
        if (ch->fighting != victim)
            return;
    }

    /* oh boy!  Fun stuff! */

    if (ch->wait > 0)
        return;

    number = number_range (0, 2);

    if (number == 1 && IS_SET (ch->act, ACT_MAGE))
    {
        /*  { mob_cast_mage(ch,victim); return; } */ ;
    }

    if (number == 2 && IS_SET (ch->act, ACT_CLERIC))
    {
        /* { mob_cast_cleric(ch,victim); return; } */ ;
    }

    /* now for the skills */

    number = number_range (0, 8);

    switch (number)
    {
        case (0):
            if (IS_SET (ch->off_flags, OFF_BASH))
                do_function (ch, &do_bash, "");
            break;

        case (1):
            if (IS_SET (ch->off_flags, OFF_BERSERK)
                && !IS_AFFECTED (ch, AFF_BERSERK))
                do_function (ch, &do_berserk, "");
            break;


        case (2):
            if (IS_SET (ch->off_flags, OFF_DISARM)
                || (get_weapon_sn (ch) != gsn_hand_to_hand
                    && (IS_SET (ch->act, ACT_WARRIOR)
                        || IS_SET (ch->act, ACT_THIEF))))
                do_function (ch, &do_disarm, "");
            break;

        case (3):
            if (IS_SET (ch->off_flags, OFF_KICK))
                do_function (ch, &do_kick, "");
            break;

        case (4):
            if (IS_SET (ch->off_flags, OFF_KICK_DIRT))
                do_function (ch, &do_dirt, "");
            break;

        case (5):
            if (IS_SET (ch->off_flags, OFF_TAIL))
            {
                /* do_function(ch, &do_tail, "") */ ;
            }
            break;

        case (6):
            if (IS_SET (ch->off_flags, OFF_TRIP))
                do_function (ch, &do_trip, "");
            break;

        case (7):
            if (IS_SET (ch->off_flags, OFF_CRUSH))
            {
                /* do_function(ch, &do_crush, "") */ ;
            }
            break;
        case (8):
            if (IS_SET (ch->off_flags, OFF_BACKSTAB))
            {
                do_function (ch, &do_backstab, "");
            }
    }
}


/*
 * Hit one guy once.
 */
void one_hit (CHAR_DATA * ch, CHAR_DATA * victim, int dt, bool secondary)
{
    OBJ_DATA *wield;
    OBJ_DATA *wield2;
    int victim_ac;
    int thac0;
    int thac0_00;
    int thac0_32;
    int dam;
    int diceroll;
    int sn, skill;
    int dam_type;
    bool result;
    bool counter;
    OBJ_DATA *corpse;

    sn = -1;
    counter = FALSE;



    /* just in case */
    if (victim == ch || ch == NULL || victim == NULL)
        return;

    if(ch->casting_spell)
    	return;

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
        return;

    /*
     * Figure out the type of damage message.
     */

    if (!secondary)
        wield = get_eq_char( ch, WEAR_WIELD );
    else
        wield = get_eq_char( ch, WEAR_SECONDARY );


    if (dt == TYPE_UNDEFINED)
    {
        dt = TYPE_HIT;
        if (wield != NULL && wield->item_type == ITEM_WEAPON)
            dt += wield->value[3];
        else
            dt += ch->dam_type;
    }

    if (dt < TYPE_HIT)
        if (wield != NULL)
            dam_type = attack_table[wield->value[3]].damage;
        else
            dam_type = attack_table[ch->dam_type].damage;
    else
        dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
        dam_type = DAM_BASH;

    /* get the weapon skill */
    sn = get_weapon_sn (ch);
    skill = 20 + get_weapon_skill (ch, sn);

    /*
     * Calculate to-hit-armor-class-0 versus armor.
     */
    if (IS_NPC (ch))
    {
        thac0_00 = 20;
        thac0_32 = -4;            /* as good as a thief */
        if (IS_SET (ch->act, ACT_WARRIOR))
            thac0_32 = -10;
        else if (IS_SET (ch->act, ACT_THIEF))
            thac0_32 = -4;
        else if (IS_SET (ch->act, ACT_CLERIC))
            thac0_32 = 2;
        else if (IS_SET (ch->act, ACT_MAGE))
            thac0_32 = 6;
    }
    else
    {
        thac0_00 = class_table[ch->class].thac0_00;
        thac0_32 = class_table[ch->class].thac0_32;
    }
    thac0 = interpolate (ch->level, thac0_00, thac0_32);

    if (thac0 < 0)
        thac0 = thac0 / 2;

    if (thac0 < -5)
        thac0 = -5 + (thac0 + 5) / 2;

    thac0 -= GET_HITROLL (ch) * skill / 100;
    thac0 += 5 * (100 - skill) / 100;

    if (dt == gsn_backstab)
        thac0 -= 10 * (100 - get_skill (ch, gsn_backstab));

    if (dt == gsn_dual_backstab)
        thac0 -= 10 * (100 - get_skill(ch,gsn_dual_backstab));



    switch (dam_type)
    {
        case (DAM_PIERCE):
            victim_ac = GET_AC (victim, AC_PIERCE) / 10;
            break;
        case (DAM_BASH):
            victim_ac = GET_AC (victim, AC_BASH) / 10;
            break;
        case (DAM_SLASH):
            victim_ac = GET_AC (victim, AC_SLASH) / 10;
            break;
        default:
            victim_ac = GET_AC (victim, AC_EXOTIC) / 10;
            break;
    };

    if (victim_ac < -15)
        victim_ac = (victim_ac + 15) / 5 - 15;

    if (!can_see (ch, victim))
        victim_ac -= 4;

    if (victim->position < POS_FIGHTING)
        victim_ac += 4;

    if (victim->position < POS_RESTING)
        victim_ac += 6;

    /*
     * The moment of excitement!
     */
    while ((diceroll = number_bits (5)) >= 20);

    if (diceroll == 0 || (diceroll != 19 && diceroll < thac0 - victim_ac))
    {
        /* Miss. */
        damage (ch, victim, 0, dt, dam_type, TRUE);
        tail_chain ();
        return;
    }

    if (!is_safe(ch,victim))
    {
      improve_stance(ch);
    }

    /*
     * Hit.
     * Calc damage.
     */
    if (IS_NPC (ch) && (!ch->pIndexData->new_format || wield == NULL))
        if (!ch->pIndexData->new_format)
        {
            dam = number_range (ch->level / 2, ch->level * 3 / 2);
            if (wield != NULL)
                dam += dam / 2;
        }
        else
            dam = dice (ch->damage[DICE_NUMBER], ch->damage[DICE_TYPE]);

    else
    {
        if (sn != -1)
            check_improve (ch, sn, TRUE, 5);
        if (wield != NULL)
        {
            if (wield->pIndexData->new_format)
                dam = dice (wield->value[1], wield->value[2]) * skill / 100;
            else
                dam = number_range (wield->value[1] * skill / 100,
                                    wield->value[2] * skill / 100);

            if (get_eq_char (ch, WEAR_SHIELD) == NULL)    /* no shield = more */
                dam = dam * 11 / 10;

            /* sharpness! */
            if (IS_WEAPON_STAT (wield, WEAPON_SHARP))
            {
                int percent;

                if ((percent = number_percent ()) <= (skill / 8))
                    dam = 2 * dam + (dam * 2 * percent / 100);
            }
        }
        else
            dam =
                number_range (1 + 4 * skill / 100,
                              2 * ch->level / 3 * skill / 100);
    }
    /*
     * Bonuses.
     */
    if (get_skill (ch, gsn_enhanced_damage) > 0)
    {
        diceroll = number_percent ();
        if (diceroll <= get_skill (ch, gsn_enhanced_damage))
        {
            check_improve (ch, gsn_enhanced_damage, TRUE, 6);
            dam += 2 * (dam * diceroll / 300);
        }
    }

  	dam = axe_special (ch, victim, dam);

    if (!IS_AWAKE (victim))
        dam *= 2;
    else if (victim->position < POS_FIGHTING)
        dam = dam * 3 / 2;

    if ( dt == gsn_backstab && wield != NULL)
      dam = (ch->level < 50) ? (ch->level/10 + 1) * dam + ch->level
                             : (ch->level/10) * dam + ch->level;


    else if ( dt == gsn_dual_backstab && wield != NULL)
      dam = (ch->level < 56) ? (ch->level/14 + 1) * dam + ch->level
                             : (ch->level/14) * dam + ch->level;
    

    else if (dt == gsn_circle)
      dam = (ch->level/40 + 1) * dam + ch->level;

    else if (dt == gsn_dual_circle)
      dam = (ch->level/40 + 1) * dam + ch->level;


    else if (dt == gsn_precisionstrike)
      dam = (ch->level/40 + 1) * dam + ch->level;

    else if ( dt == gsn_cleave && wield != NULL)
      {
	if (number_percent() < URANGE(4, 5+(ch->level-victim->level),10))
	  {
	    act("Your cleave chops $N $CIN HALF!$c",
		      ch,NULL,victim,TO_CHAR);
	    act("$n's cleave chops you $CIN HALF!$c",
		      ch,NULL,victim,TO_VICT);
	    act("$n's cleave chops $N $CIN HALF!$c",
		      ch,NULL,victim,TO_NOTVICT);
	    send_to_char("You have been KILLED!\n\r",victim);
	    act("$n is DEAD!",victim,NULL,NULL,TO_ROOM);
	    WAIT_STATE( ch, 2 );
	    raw_kill(victim);
	    if ( !IS_NPC(ch) && IS_NPC(victim) )
	      {
		corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 
		
		if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		    corpse && corpse->contains) /* exists and not empty */
		  do_get( ch, "all corpse" );
		
		if (IS_SET(ch->act,PLR_AUTOGOLD) &&
		    corpse && corpse->contains  && /* exists and not empty */
		    !IS_SET(ch->act,PLR_AUTOLOOT))  {
		  do_get(ch, "gold corpse");
		  do_get(ch, "silver corpse");
		  do_get(ch," platinum corpse");
		}

		if ( IS_SET(ch->act, PLR_AUTOSAC) )
		{ if (IS_SET(ch->act,PLR_AUTOLOOT) && corpse
		      && corpse->contains)
		    return;  /* leave if corpse has treasure */
		  else
		    do_sacrifice( ch, "corpse" );
		}
	      }
	    return;
	  }
	else dam = (dam * 2 + ch->level);
      }
if (dt == gsn_assassinate)
      {
	if (number_percent() <= URANGE(10, 20+(ch->level-victim->level)*2, 50) && !counter)
	  {
	    act("You $C+++ASSASSINATE+++$c $N!",ch,NULL,victim,TO_CHAR);
	    act("$N is DEAD!",ch,NULL,victim,TO_CHAR);
	    act("$n $C+++ASSASSINATES+++$c $N!",ch,NULL,victim,
		      TO_NOTVICT);
	    act("$N is DEAD!",ch,NULL,victim,TO_NOTVICT);
	    act("$n $C+++ASSASSINATES+++$c you!",ch,NULL,victim,
		      TO_VICT);
	    send_to_char("You have been KILLED!\n\r",victim);
	    check_improve(ch,gsn_assassinate,TRUE,1);
	    raw_kill(victim);

	    if ( !IS_NPC(ch) && IS_NPC(victim) )
	      {
		corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 
		
		if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		    corpse && corpse->contains) /* exists and not empty */
		  do_get( ch, "all corpse" );
		
		if (IS_SET(ch->act,PLR_AUTOGOLD) &&
		    corpse && corpse->contains  && /* exists and not empty */
		    !IS_SET(ch->act,PLR_AUTOLOOT))
		  do_get(ch, "gold corpse");
		
		if ( IS_SET(ch->act, PLR_AUTOSAC) )
		{  if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse
		      && corpse->contains)
		    return;  /* leave if corpse has treasure */
		  else
		    do_sacrifice( ch, "corpse" );
		}
	      }
	    return;

	  }
	else 
	  {
	    check_improve(ch,gsn_assassinate,FALSE,1);
	    dam *= 2;
	  }
      }




    dam += GET_DAMROLL (ch) * UMIN (100, skill) / 100;

	/*
	 * Boosted damage for spears, based on dexterity.
	 */
	if (get_weapon_sn (ch) == gsn_spear
		&& number_percent () < get_skill (ch, gsn_spear))
	{
		dam += get_curr_stat (ch, STAT_DEX)
					* get_skill (ch, gsn_spear) / 100;
		check_improve (ch, gsn_spear, TRUE, 6);
	}

    if (dam <= 0)
        dam = 1;

    if ( !check_counter( ch, victim, dam, dt ) )
        result = damage( ch, victim, dam, dt, dam_type, TRUE );

    else return;


	/*
	 * Knockdown and daze effects for mace and flail.
	 */
	if (result && ch->fighting == victim)
	{
		/*
		 * Maces knock the opponent down and lag them for a moment.
		 */
		if (get_weapon_sn (ch) == gsn_mace
			&& number_percent () < get_skill (ch, gsn_mace) / 10)
		{
			WAIT_STATE (victim, PULSE_VIOLENCE*1);
			victim->position = POS_RESTING;

			act ("Your blow knocks $N to the ground.",
					ch, NULL, victim, TO_CHAR);
			act ("$n knocks you to the ground.",
					ch, NULL, victim, TO_VICT);

  		check_improve (ch, gsn_mace, TRUE, 4);
		}

		/*
		 * Flails daze the opponent and screw up their casting.
		 */
		if (get_weapon_sn (ch) == gsn_flail
			&& number_percent () < get_skill (ch, gsn_flail) / 5)
		{
			DAZE_STATE (victim, PULSE_VIOLENCE*4);
			check_improve (ch, gsn_flail, TRUE, 4);
		}
	}


    /* but do we have a funky weapon? */
    if (result && wield != NULL)
    {
        int dam;

        if (ch->fighting == victim && IS_WEAPON_STAT (wield, WEAPON_POISON))
        {
            int level;
            AFFECT_DATA *poison, af;

            if ((poison = affect_find (wield->affected, gsn_poison)) == NULL)
                level = wield->level;
            else
                level = poison->level;

            if (!saves_spell (level / 2, victim, DAM_POISON))
            {
                send_to_char ("You feel poison coursing through your veins.",
                              victim);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
                act ("$n is poisoned by the venom on $p.",
                     victim, wield, NULL, TO_ROOM);
	    }

                af.where = TO_AFFECTS;
                af.type = gsn_poison;
                af.level = level * 3 / 4;
                af.duration = level / 2;
                af.location = APPLY_STR;
                af.modifier = -1;
                af.bitvector = AFF_POISON;
                affect_join (victim, &af);
            }

            /* weaken the poison if it's temporary */
            if (poison != NULL)
            {
                poison->level = UMAX (0, poison->level - 2);
                poison->duration = UMAX (0, poison->duration - 1);

                if (poison->level == 0 || poison->duration == 0)
                    act ("The poison on $p has worn off.", ch, wield, NULL,
                         TO_CHAR);
            }
        }


        if (ch->fighting == victim && IS_WEAPON_STAT (wield, WEAPON_VAMPIRIC))
        {
            dam = number_range (1, wield->level / 5 + 1);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
            act ("$p draws {Gl{gi{Gf{ge{x from $n.", victim, wield, NULL, TO_ROOM);
            act ("You feel $p drawing your {Gl{gi{Gf{ge{x away.",
                 victim, wield, NULL, TO_CHAR);
	    }
            damage (ch, victim, dam, 0, DAM_NEGATIVE, FALSE);
            ch->alignment = UMAX (-1000, ch->alignment - 1);
            ch->hit += dam;
        }

        if (ch->fighting == victim && IS_WEAPON_STAT (wield, WEAPON_FLAMING))
        {
            dam = number_range (1, wield->level / 4 + 1);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
            act ("$n is {Yb{yu{rr{Rn{Ye{Rd{x by $p.", victim, wield, NULL, TO_ROOM);
            act ("$p sears your flesh.", victim, wield, NULL, TO_CHAR);
	    }
            fire_effect ((void *) victim, wield->level / 2, dam, TARGET_CHAR);
            damage (ch, victim, dam, 0, DAM_FIRE, FALSE);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT (wield, WEAPON_FROST))
        {
            dam = number_range (1, wield->level / 6 + 2);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
            act ("$p {Cf{cr{We{ce{Cz{We{Cs{x $n.", victim, wield, NULL, TO_ROOM);
            act ("The cold touch of $p surrounds you with ice.",
                 victim, wield, NULL, TO_CHAR);
	    }
            cold_effect (victim, wield->level / 2, dam, TARGET_CHAR);
            damage (ch, victim, dam, 0, DAM_COLD, FALSE);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT (wield, WEAPON_SHOCKING))
        {
            dam = number_range (1, wield->level / 5 + 2);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
            act ("$n is struck by {cl{Wi{cg{Wh{ct{Wn{ci{Wn{cg{x from $p.", victim, wield, NULL,                 TO_ROOM);
            act ("You are {cs{Wh{co{Wc{ck{We{cd{x by $p.", victim, wield, NULL, TO_CHAR);
	    }
            shock_effect (victim, wield->level / 2, dam, TARGET_CHAR);
            damage (ch, victim, dam, 0, DAM_LIGHTNING, FALSE);
        }
    }




    /*
     * Figure out the type of damage message.
     */
    wield2 = get_eq_char (ch, WEAR_SECONDARY);

    if (dt == TYPE_UNDEFINED)
    {
        dt = TYPE_HIT;
        if (wield2 != NULL && wield2->item_type == ITEM_WEAPON)
            dt += wield2->value[3];
        else
            dt += ch->dam_type;
    }

    if (dt < TYPE_HIT)
        if (wield2 != NULL)
            dam_type = attack_table[wield2->value[3]].damage;
        else
            dam_type = attack_table[ch->dam_type].damage;
    else
        dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
        dam_type = DAM_BASH;

    /* get the weapon skill */
    sn = get_weapon_sn (ch);
    skill = 20 + get_weapon_skill (ch, sn);

    /*
     * Calculate to-hit-armor-class-0 versus armor.
     */
    if (IS_NPC (ch))
    {
        thac0_00 = 20;
        thac0_32 = -4;            /* as good as a thief */
        if (IS_SET (ch->act, ACT_WARRIOR))
            thac0_32 = -10;
        else if (IS_SET (ch->act, ACT_THIEF))
            thac0_32 = -4;
        else if (IS_SET (ch->act, ACT_CLERIC))
            thac0_32 = 2;
        else if (IS_SET (ch->act, ACT_MAGE))
            thac0_32 = 6;
    }
    else
    {
        thac0_00 = class_table[ch->class].thac0_00;
        thac0_32 = class_table[ch->class].thac0_32;
    }
    thac0 = interpolate (ch->level, thac0_00, thac0_32);

    if (thac0 < 0)
        thac0 = thac0 / 2;

    if (thac0 < -5)
        thac0 = -5 + (thac0 + 5) / 2;

    thac0 -= GET_HITROLL (ch) * skill / 100;
    thac0 += 5 * (100 - skill) / 100;

    if (dt == gsn_backstab)
        thac0 -= 10 * (100 - get_skill (ch, gsn_backstab));

    if (dt == gsn_dual_backstab)
        thac0 -= 10 * (100 - get_skill(ch,gsn_dual_backstab));



    switch (dam_type)
    {
        case (DAM_PIERCE):
            victim_ac = GET_AC (victim, AC_PIERCE) / 10;
            break;
        case (DAM_BASH):
            victim_ac = GET_AC (victim, AC_BASH) / 10;
            break;
        case (DAM_SLASH):
            victim_ac = GET_AC (victim, AC_SLASH) / 10;
            break;
        default:
            victim_ac = GET_AC (victim, AC_EXOTIC) / 10;
            break;
    };

    if (victim_ac < -15)
        victim_ac = (victim_ac + 15) / 5 - 15;

    if (!can_see (ch, victim))
        victim_ac -= 4;

    if (victim->position < POS_FIGHTING)
        victim_ac += 4;

    if (victim->position < POS_RESTING)
        victim_ac += 6;

    /*
     * The moment of excitement!
     */
    while ((diceroll = number_bits (5)) >= 20);

    if (diceroll == 0 || (diceroll != 19 && diceroll < thac0 - victim_ac))
    {
        /* Miss. */
        damage (ch, victim, 0, dt, dam_type, TRUE);
        tail_chain ();
        return;
    }

    /*
     * Hit.
     * Calc damage.
     */
    if (IS_NPC (ch) && (!ch->pIndexData->new_format || wield2 == NULL))
        if (!ch->pIndexData->new_format)
        {
            dam = number_range (ch->level / 2, ch->level * 3 / 2);
            if (wield2 != NULL)
                dam += dam / 2;
        }
        else
            dam = dice (ch->damage[DICE_NUMBER], ch->damage[DICE_TYPE]);

    else
    {
        if (sn != -1)
            check_improve (ch, sn, TRUE, 5);
        if (wield2 != NULL)
        {
            if (wield2->pIndexData->new_format)
                dam = dice (wield2->value[1], wield2->value[2]) * skill / 100;
            else
                dam = number_range (wield2->value[1] * skill / 100,
                                    wield2->value[2] * skill / 100);

            if (get_eq_char (ch, WEAR_SHIELD) == NULL)    /* no shield = more */
                dam = dam * 11 / 10;

            /* sharpness! */
            if (IS_WEAPON_STAT (wield2, WEAPON_SHARP))
            {
                int percent;

                if ((percent = number_percent ()) <= (skill / 8))
                    dam = 2 * dam + (dam * 2 * percent / 100);
            }
        }
        else
            dam =
                number_range (1 + 4 * skill / 100,
                              2 * ch->level / 3 * skill / 100);
    }
    /*
     * Bonuses.
     */
    if (get_skill (ch, gsn_enhanced_damage) > 0)
    {
        diceroll = number_percent ();
        if (diceroll <= get_skill (ch, gsn_enhanced_damage))
        {
            check_improve (ch, gsn_enhanced_damage, TRUE, 6);
            dam += 2 * (dam * diceroll / 300);
        }
    }

    if (!IS_AWAKE (victim))
        dam *= 2;
    else if (victim->position < POS_FIGHTING)
        dam = dam * 3 / 2;

    if ( dt == gsn_backstab && wield != NULL)
      dam = (ch->level < 50) ? (ch->level/10 + 1) * dam + ch->level
                             : (ch->level/10) * dam + ch->level;


    else if ( dt == gsn_dual_backstab && wield != NULL)
      dam = (ch->level < 56) ? (ch->level/14 + 1) * dam + ch->level
                             : (ch->level/14) * dam + ch->level;


    else if (dt == gsn_circle)
      dam = (ch->level/40 + 1) * dam + ch->level;

    else if (dt == gsn_dual_circle)
      dam = (ch->level/40 + 1) * dam + ch->level;

    else if (dt == gsn_precisionstrike)
      dam = (ch->level/40 + 1) * dam + ch->level;



    dam += GET_DAMROLL (ch) * UMIN (100, skill) / 100;

    if (dam <= 0)
        dam = 1;

    if ( !check_counter( ch, victim, dam, dt ) )
        result = damage( ch, victim, dam, dt, dam_type, TRUE );

    else return;

    /* but do we have a funky weapon? */
    if (result && wield2 != NULL)
    {
        int dam;

        if (ch->fighting == victim && IS_WEAPON_STAT (wield2, WEAPON_POISON))
        {
            int level;
            AFFECT_DATA *poison, af;

            if ((poison = affect_find (wield2->affected, gsn_poison)) == NULL)
                level = wield2->level;
            else
                level = poison->level;

            if (!saves_spell (level / 2, victim, DAM_POISON))
            {
                send_to_char ("You feel poison coursing through your veins.",
                              victim);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
                act ("$n is poisoned by the venom on $p.",
                     victim, wield2, NULL, TO_ROOM);
	    }
                af.where = TO_AFFECTS;
                af.type = gsn_poison;
                af.level = level * 3 / 4;
                af.duration = level / 2;
                af.location = APPLY_STR;
                af.modifier = -1;
                af.bitvector = AFF_POISON;
                affect_join (victim, &af);
            }

            /* weaken the poison if it's temporary */
            if (poison != NULL)
            {
                poison->level = UMAX (0, poison->level - 2);
                poison->duration = UMAX (0, poison->duration - 1);

                if (poison->level == 0 || poison->duration == 0)
                    act ("The poison on $p has worn off.", ch, wield2, NULL,
                         TO_CHAR);
            }
        }


        if (ch->fighting == victim && IS_WEAPON_STAT (wield2, WEAPON_VAMPIRIC))
        {
            dam = number_range (1, wield2->level / 5 + 1);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
            act ("$p draws {Gl{gi{Gf{ge{x from $n.", victim, wield2, NULL, TO_ROOM);
            act ("You feel $p drawing your {Gl{gi{Gf{ge{x away.",
                 victim, wield2, NULL, TO_CHAR);
	    }
            damage (ch, victim, dam, 0, DAM_NEGATIVE, FALSE);
            ch->alignment = UMAX (-1000, ch->alignment - 1);
            ch->hit += dam;
        }

        if (ch->fighting == victim && IS_WEAPON_STAT (wield2, WEAPON_FLAMING))
        {
            dam = number_range (1, wield2->level / 4 + 1);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
            act ("$n is {Yb{yu{rr{Rn{Ye{Rd{x by $p.", victim, wield2, NULL, TO_ROOM);
            act ("$p sears your flesh.", victim, wield2, NULL, TO_CHAR);
	    }
            fire_effect ((void *) victim, wield2->level / 2, dam, TARGET_CHAR);
            damage (ch, victim, dam, 0, DAM_FIRE, FALSE);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT (wield2, WEAPON_FROST))
        {
            dam = number_range (1, wield2->level / 6 + 2);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
            act ("$p {Cf{cr{We{ce{Cz{We{Cs{x $n.", victim, wield2, NULL, TO_ROOM);
            act ("The cold touch of $p surrounds you with ice.",
                 victim, wield2, NULL, TO_CHAR);
	    }
            cold_effect (victim, wield2->level / 2, dam, TARGET_CHAR);
            damage (ch, victim, dam, 0, DAM_COLD, FALSE);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT (wield2, WEAPON_SHOCKING))
        {
            dam = number_range (1, wield2->level / 5 + 2);
            if (!IS_SET(ch->comm, COMM_BRIEF2))
            {
            act ("$n is struck by {cl{Wi{cg{Wh{ct{Wn{ci{Wn{cg{x from $p.", victim, wield2, NULL,
                 TO_ROOM);
            act ("You are {cs{Wh{co{Wc{ck{We{cd{x by $p.", victim, wield2, NULL, TO_CHAR);
	    }
            shock_effect (victim, wield2->level / 2, dam, TARGET_CHAR);
            damage (ch, victim, dam, 0, DAM_LIGHTNING, FALSE);
        }
    }

    tail_chain ();
    return;
}


/*
 * Inflict damage from a hit.
 */
bool damage (CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt,
             int dam_type, bool show)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    bool immune = FALSE;
//    bool stance;
    int diety;
    int dam2;
    long copper, silver, gold, platinum;

    if (victim->position == POS_DEAD)
        return FALSE;

    /* damage reduction */
    if (dam > 35)
        dam = (dam - 35) / 2 + 35;
    if (dam > 80)
        dam = (dam - 80) / 2 + 80;
        

	if ( has_skill(ch,gsn_vital_hit) )
        {
          if( (get_skill(ch,gsn_vital_hit) / 12) > number_percent( ))
          {
            check_improve(ch,gsn_vital_hit,TRUE, 3);
            dam = (dam * 8) / 5;
          }
        }



    if (victim != ch)
    {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        if (is_safe (ch, victim))
            return FALSE;
        check_killer (ch, victim);

        if (victim->position > POS_STUNNED)
        {
            if (victim->fighting == NULL)
            {
                set_fighting (victim, ch);
                if (IS_NPC (victim) && HAS_TRIGGER (victim, TRIG_KILL))
                    mp_percent_trigger (victim, ch, NULL, NULL, TRIG_KILL);
            }
            if (victim->timer <= 4)
                victim->position = POS_FIGHTING;
        }

        if (victim->position > POS_STUNNED)
        {
            if (ch->fighting == NULL)
                set_fighting (ch, victim);
        }

        /*
         * More charm stuff.
         */
        if (victim->master == ch)
            stop_follower (victim);
    }

    /*
     * Inviso attacks ... not.
     */
    if (IS_AFFECTED (ch, AFF_INVISIBLE))
    {
        affect_strip (ch, gsn_invis);
        affect_strip (ch, gsn_mass_invis);
        REMOVE_BIT (ch->affected_by, AFF_INVISIBLE);
        act ("$n fades into existence.", ch, NULL, NULL, TO_ROOM);
    }

    /*
     * Damage modifiers.
     */

    if ( double_damage )
// && IS_NPC ( victim ) && !IS_NPC ( ch ) )
        dam += dam * 1.4;

    if (dam > 1 && !IS_NPC (victim)
        && victim->pcdata->condition[COND_DRUNK] > 10)
        dam = 9 * dam / 10;

    if (dam > 1 && IS_AFFECTED (victim, AFF_SANCTUARY))
        dam /= 2;

    if (dam > 1 && IS_AFFECTED2 (victim, AFF_SCLOAK))
        dam /= 2;

    if (dam > 1 && IS_AFFECTED (victim, AFF_COCOON))
        dam /= 2;

    if (dam > 1 && ((IS_AFFECTED (victim, AFF_PROTECT_EVIL) && IS_EVIL (ch))
                    || (IS_AFFECTED (victim, AFF_PROTECT_GOOD)
                        && IS_GOOD (ch))))
        dam -= dam / 4;

    immune = FALSE;



                if (!IS_NPC(victim))
                {
                        if (victim->pcdata->damreduct == 100)
                                dam -= 0;
                        else if (victim->pcdata->damreductdec > 0)
                                dam = (dam *
                                       (victim->pcdata->damreduct + 1)) / 100;
                        else
                                dam = (dam * victim->pcdata->damreduct) / 100;
                }



    /*
     * Check for parry, and dodge.
     */
    if(!victim->casting_spell)	// Ensure the victim isn't casting.
    {
    if (dt >= TYPE_HIT && ch != victim)
    {
       if (!can_bypass(ch,victim))  
			{	
        if (check_parry (ch, victim))
            return FALSE;
        if (check_dodge (ch, victim))
            return FALSE;
        if (check_shield_block (ch, victim))
            return FALSE;
        if (check_phase( ch, victim))
            return FALSE;
        if (pole_special (ch, victim))
            return FALSE;
		}
	if(IS_AFFECTED (victim, AFF_FORCE_SHIELD) && check_force_shield(ch, victim))
		return FALSE;
	if(IS_AFFECTED (victim, AFF_STATIC_SHIELD) && check_static_shield(ch, victim))
		return FALSE;

	    if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE)
						&& victim->stance[STANCE_CRANE] >= 100
						&& check_parry( ch, victim ))
				return FALSE;
	    else if ( !IS_NPC(victim)
						&& IS_STANCE(victim, STANCE_MANTIS)
						&& victim->stance[STANCE_MANTIS] >= 100
						&& check_parry( ch, victim ))
				return FALSE;
	    if ( !IS_NPC(victim)
						&& IS_STANCE(victim, STANCE_MONGOOSE)
						&& victim->stance[STANCE_MONGOOSE] >= 100
						&& check_dodge( ch, victim))
				return FALSE;
	    else if ( !IS_NPC(victim)
						&& IS_STANCE(victim, STANCE_SWALLOW)
						&& victim->stance[STANCE_SWALLOW] >= 100
						&& check_dodge( ch, victim ))
				return FALSE;

    }
}


	if(IS_AFFECTED (victim, AFF_FLAME_SHIELD) && dam_type <= 3)
		check_flame_shield(ch, victim);

//    ch->in_room->blood += dam;

    switch (check_immune (victim, dam_type))
    {
        case (IS_IMMUNE):
	if (dam_type != DAM_BASH)
	{
            immune = TRUE;
            dam = 0;
	}
            break;
        case (IS_RESISTANT):
            dam -= dam / 3;
            break;
        case (IS_VULNERABLE):
            dam += dam / 2;
            break;
    }

/*
	if ( ch->stance )
	{
           dam = dambonus( ch, victim, dam, TRUE );
	   dam = damreduce( ch, victim, dam, TRUE );	
	}
*/

    if ( dam >= 10				/* make sure it's a hit worth a critical */
    && dam_type <= 3				/* NOT a backstab */
    && dt >= TYPE_HIT )				/* only non-spell damage */
    {
        if ( check_critical(ch,victim) )
            dam *= 2;
    }

    if ( dt >= TYPE_HIT && ch != victim )
        {
         if ( (dam2 = critical_strike(ch,victim,dam)) != 0 )
         dam = dam2;
        }



    if (show && !IS_SET(ch->comm, COMM_BRIEF3))
        dam_message (ch, victim, dam, dt, immune);

                /*
                 * Brief 5 System - Jobo
                 */
                if (!IS_NPC(ch) && IS_SET(ch->comm, COMM_BRIEF3))
                {
                        ch->amount_damage_dealt += dam;
                        ch->amount_attacks_dealt++;
                }
//		else dam_message (ch, victim, dam, dt, immune);
  
	        if (!IS_NPC(victim) && IS_SET(victim->comm, COMM_BRIEF3))
                {
                        victim->amount_damage_recieved += dam;
                        victim->amount_attacks_recieved++;
                }

//        dam_message ( ch, victim, dam, dt, immune, verbose, TRUE );

    if (dam == 0)
        return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= dam;
    if (!IS_NPC (victim)
        && victim->level >= LEVEL_IMMORTAL && victim->hit < 1)
        victim->hit = 1;
    update_pos (victim);

    switch (victim->position)
    {
        case POS_MORTAL:
            act ("$n is mortally wounded, and will die soon, if not aided.",
                 victim, NULL, NULL, TO_ROOM);
            send_to_char
                ("You are mortally wounded, and will die soon, if not aided.\n\r",
                 victim);
            break;

        case POS_INCAP:
            act ("$n is incapacitated and will slowly die, if not aided.",
                 victim, NULL, NULL, TO_ROOM);
            send_to_char
                ("You are incapacitated and will slowly die, if not aided.\n\r",
                 victim);
            break;

        case POS_STUNNED:
            act ("$n is stunned, but will probably recover.",
                 victim, NULL, NULL, TO_ROOM);
            send_to_char ("You are stunned, but will probably recover.\n\r",
                          victim);
            break;

        case POS_DEAD:
            act ("{R$n is DEAD!!{x", victim, 0, 0, TO_ROOM);
            send_to_char ("{RYou have been KILLED!!{x\n\r\n\r", victim);
            break;

        default:
            if (dam > victim->max_hit / 4)
                send_to_char ("{RThat really did HURT!{x\n\r", victim);
            if (victim->hit < victim->max_hit / 4)
                send_to_char ("{RYou sure are BLEEDING!{x\n\r", victim);
            break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if (!IS_AWAKE (victim))
        stop_fighting (victim, FALSE);

    /*
     * Payoff for killing things.
     */
    if (victim->position == POS_DEAD)
    {
        group_gain (ch, victim);


  if (!IS_NPC(ch) && IS_NPC(victim))
	{
	diety = ch->god;

	gods[diety].souls++;

  save_gods ();

	}



  if (!IS_NPC(ch) && !IS_NPC(victim))
  {
  ch->status += 1;
  ch->pkill += 1;
  victim->pdeath += 1;

  if (victim->status > 0)
  victim->status -= 1;
  }

 if (IS_NPC(ch) && !IS_NPC(victim))  victim->mdeaths++;



	if (!IS_NPC(ch))
	{
        ch->pcdata->mkills++;
        mobkills_today++;
        sprintf(buf, "{B[{WMob Kills{B:{W%d{B]{x\n\r", ch->pcdata->mkills);
        send_to_char(buf, ch);

	copper = victim->level * 10;
	silver = victim->level * 5;
	gold = victim->level / 5;
	platinum = victim->level / 10;
	
	sprintf(buf,"{B[{WLoot{B: {yCopper %ld, {wSilver %ld, {YGold %ld, {WPlatinum %ld{B]{x\n\r",
	copper, silver, gold, platinum);
	send_to_char(buf, ch);

	ch->copper += copper;
	ch->silver += silver;
	ch->gold += gold;
	ch->platinum += platinum;

	}



        if (!IS_NPC (victim))
        {
            sprintf (log_buf, "%s killed by %s at %d",
                     victim->name,
                     (IS_NPC (ch) ? ch->short_descr : ch->name),
                     ch->in_room->vnum);
            log_string (log_buf);

            /*
             * Dying penalty:
             * 2/3 way back to previous level.
             */
/*            if (victim->exp > exp_per_level (victim, victim->pcdata->points)
                * victim->level)
                gain_exp (victim,
                          (2 *
                           (exp_per_level (victim, victim->pcdata->points) *
                            victim->level - victim->exp) / 3) + 50);
*/
        }

        sprintf (log_buf, "%s got toasted by %s at %s [room %d]",
                 (IS_NPC (victim) ? victim->short_descr : victim->name),
                 (IS_NPC (ch) ? ch->short_descr : ch->name),
                 ch->in_room->name, ch->in_room->vnum);

        if (IS_NPC (victim))
            wiznet (log_buf, NULL, NULL, WIZ_MOBDEATHS, 0, 0);
        else
            wiznet (log_buf, NULL, NULL, WIZ_DEATHS, 0, 0);

        /*
         * Death trigger
         */
        if (IS_NPC (victim) && HAS_TRIGGER (victim, TRIG_DEATH))
        {
            victim->position = POS_STANDING;
            mp_percent_trigger (victim, ch, NULL, NULL, TRIG_DEATH);
        }

        raw_kill (victim);

        /* dump the flags */
        if (ch != victim && !IS_NPC (ch) && !is_same_clan (ch, victim))
        {
            if (IS_SET (victim->act, PLR_KILLER))
                REMOVE_BIT (victim->act, PLR_KILLER);
            else
                REMOVE_BIT (victim->act, PLR_THIEF);
        }

        /* RT new auto commands */

        if (!IS_NPC (ch)
            && (corpse =
                get_obj_list (ch, "corpse", ch->in_room->contents)) != NULL
            && corpse->item_type == ITEM_CORPSE_NPC
            && can_see_obj (ch, corpse))
        {
            OBJ_DATA *coins;

            corpse = get_obj_list (ch, "corpse", ch->in_room->contents);

            if (IS_SET (ch->act, PLR_AUTOLOOT) && corpse && corpse->contains)
            {                    /* exists and not empty */
                do_function (ch, &do_get, "all corpse");
            }

            if (IS_SET (ch->act, PLR_AUTOGOLD) && corpse && corpse->contains &&    /* exists and not empty */
                !IS_SET (ch->act, PLR_AUTOLOOT))
            {
                if ((coins = get_obj_list (ch, "gcash", corpse->contains))
                    != NULL)
                {
                    do_function (ch, &do_get, "all.gcash corpse");
                }
            }

            if (IS_SET (ch->act, PLR_AUTOSAC))
            {
                if (IS_SET (ch->act, PLR_AUTOLOOT) && corpse
                    && corpse->contains)
                {
                    return TRUE;    /* leave if corpse has treasure */
                }
                else
                {
                    do_function (ch, &do_sacrifice, "corpse");
                }
            }
        }

        return TRUE;
    }

    if (victim == ch)
        return TRUE;

    /*
     * Take care of link dead people.
     */
    if (!IS_NPC (victim) && victim->desc == NULL)
    {
        if (number_range (0, victim->wait) == 0)
        {
            do_function (victim, &do_recall, "");
            return TRUE;
        }
    }

    /*
     * Wimp out?
     */
    if (IS_NPC (victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
        if ((IS_SET (victim->act, ACT_WIMPY) && number_bits (2) == 0
             && victim->hit < victim->max_hit / 5)
            || (IS_AFFECTED (victim, AFF_CHARM) && victim->master != NULL
                && victim->master->in_room != victim->in_room))
        {
            do_function (victim, &do_flee, "");
        }
    }

    if (!IS_NPC (victim)
        && victim->hit > 0
        && victim->hit <= victim->wimpy && victim->wait < PULSE_VIOLENCE / 2)
    {
        do_function (victim, &do_flee, "");
    }

    tail_chain ();
    return TRUE;
}

bool is_safe (CHAR_DATA * ch, CHAR_DATA * victim)
{
    if (victim->in_room == NULL || ch->in_room == NULL)
        return TRUE;

    if (victim->fighting == ch || victim == ch)
        return FALSE;

    if (IS_IMMORTAL (ch) && ch->level > LEVEL_IMMORTAL)
        return FALSE;

    /* killing mobiles */
    if (IS_NPC (victim))
    {

        /* safe room? */
        if (IS_SET (victim->in_room->room_flags, ROOM_SAFE))
        {
            send_to_char ("Not in this room.\n\r", ch);
            return TRUE;
        }

        if (victim->pIndexData->pShop != NULL)
        {
            send_to_char ("The shopkeeper wouldn't like that.\n\r", ch);
            return TRUE;
        }

        /* no killing healers, trainers, etc */
        if (IS_SET (victim->act, ACT_TRAIN)
            || IS_SET (victim->act, ACT_PRACTICE)
            || IS_SET (victim->act, ACT_IS_HEALER)
            || IS_SET (victim->act, ACT_IS_CHANGER))
        {
            send_to_char ("I don't think Mota would approve.\n\r", ch);
            return TRUE;
        }

        if (!IS_NPC (ch))
        {
            /* no pets */
            if (IS_SET (victim->act, ACT_PET))
            {
                act ("But $N looks so cute and cuddly...",
                     ch, NULL, victim, TO_CHAR);
                return TRUE;
            }

            /* no charmed creatures unless owner */
            if (IS_AFFECTED (victim, AFF_CHARM) && ch != victim->master)
            {
                send_to_char ("You don't own that monster.\n\r", ch);
                return TRUE;
            }
        }
    }
    /* killing players */
    else
    {
        /* NPC doing the killing */
        if (IS_NPC (ch))
        {
            /* safe room check */
            if (IS_SET (victim->in_room->room_flags, ROOM_SAFE))
            {
                send_to_char ("Not in this room.\n\r", ch);
                return TRUE;
            }

            /* charmed mobs and pets cannot attack players while owned */
            if (IS_AFFECTED (ch, AFF_CHARM) && ch->master != NULL
                && ch->master->fighting != victim)
            {
                send_to_char ("Players are your friends!\n\r", ch);
                return TRUE;
            }
        }
        /* player doing the killing */
        else
        {
            if (!is_clan (ch))
            {
                send_to_char ("Join a clan if you want to kill players.\n\r",
                              ch);
                return TRUE;
            }

            if (IS_SET (victim->act, PLR_KILLER)
                || IS_SET (victim->act, PLR_THIEF))
                return FALSE;

            if (!is_clan (victim))
            {
                send_to_char ("They aren't in a clan, leave them alone.\n\r",
                              ch);
                return TRUE;
            }

            if (ch->level > victim->level + 8)
            {
                send_to_char ("Pick on someone your own size.\n\r", ch);
                return TRUE;
            }
        }
    }
    return FALSE;
}

bool is_safe_nomessage(CHAR_DATA *ch, CHAR_DATA *victim )
{
  if (victim->fighting == ch || ch==victim)
    return FALSE;

 

     if  ((!IS_NPC(ch) &&  !IS_NPC(victim) && victim->level < 5 ) || 
         ( !IS_NPC(ch) &&  !IS_NPC(victim) && ch->level < 5 ))
  return TRUE;


  /* level adjustement */
  if ( ch != victim && !IS_IMMORTAL(ch) && !IS_NPC(ch) && !IS_NPC(victim) &&
  	( ch->level >= (victim->level + UMAX(4,ch->level/10 +2) )
	  ||	 ch->level <= (victim->level - UMAX(4,ch->level/10 +2)) )&&
  	( victim->level >= (ch->level + UMAX(4,victim->level/10 +2) )
		|| victim->level <= (ch->level - UMAX(4,victim->level/10 +2))) )
    return TRUE;

  return FALSE;
}



bool is_safe_spell (CHAR_DATA * ch, CHAR_DATA * victim, bool area)
{
    if (victim->in_room == NULL || ch->in_room == NULL)
        return TRUE;

    if (victim == ch && area)
        return TRUE;

    if (victim->fighting == ch || victim == ch)
        return FALSE;

    if (IS_IMMORTAL (ch) && ch->level > LEVEL_IMMORTAL && !area)
        return FALSE;

    /* killing mobiles */
    if (IS_NPC (victim))
    {
        /* safe room? */
        if (IS_SET (victim->in_room->room_flags, ROOM_SAFE))
            return TRUE;

        if (victim->pIndexData->pShop != NULL)
            return TRUE;

        /* no killing healers, trainers, etc */
        if (IS_SET (victim->act, ACT_TRAIN)
            || IS_SET (victim->act, ACT_PRACTICE)
            || IS_SET (victim->act, ACT_IS_HEALER)
            || IS_SET (victim->act, ACT_IS_CHANGER))
            return TRUE;

        if (!IS_NPC (ch))
        {
            /* no pets */
            if (IS_SET (victim->act, ACT_PET))
                return TRUE;

            /* no charmed creatures unless owner */
            if (IS_AFFECTED (victim, AFF_CHARM)
                && (area || ch != victim->master))
                return TRUE;

            /* legal kill? -- cannot hit mob fighting non-group member */
            if (victim->fighting != NULL
                && !is_same_group (ch, victim->fighting)) return TRUE;
        }
        else
        {
            /* area effect spells do not hit other mobs */
            if (area && !is_same_group (victim, ch->fighting))
                return TRUE;
        }
    }
    /* killing players */
    else
    {
        if (area && IS_IMMORTAL (victim) && victim->level > LEVEL_IMMORTAL)
            return TRUE;

        /* NPC doing the killing */
        if (IS_NPC (ch))
        {
            /* charmed mobs and pets cannot attack players while owned */
            if (IS_AFFECTED (ch, AFF_CHARM) && ch->master != NULL
                && ch->master->fighting != victim)
                return TRUE;

            /* safe room? */
            if (IS_SET (victim->in_room->room_flags, ROOM_SAFE))
                return TRUE;

            /* legal kill? -- mobs only hit players grouped with opponent */
            if (ch->fighting != NULL && !is_same_group (ch->fighting, victim))
                return TRUE;
        }

        /* player doing the killing */
        else
        {
            if (!is_clan (ch))
                return TRUE;

            if (IS_SET (victim->act, PLR_KILLER)
                || IS_SET (victim->act, PLR_THIEF))
                return FALSE;

            if (!is_clan (victim))
                return TRUE;

            if (ch->level > victim->level + 8)
                return TRUE;
        }

    }
    return FALSE;
}


void bite( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int dam;

	if (ch->disease == DISEASE_VAMPIRE)
	{
	dam = number_range(get_curr_stat (ch, STAT_STR) + 50, get_curr_stat (ch, STAT_STR) + 100);
        dam += ch->level * 4;
	damage( ch, victim, dam, gsn_bite,DAM_BASH,TRUE);
	if (ch->blood <= 9950)
		ch->blood += 50;
	}
	
	if (ch->disease == DISEASE_WEREWOLF || ch->disease == DISEASE_UNDEAD
	|| ch->disease == DISEASE_DEMON)
	{
        dam = number_range(get_curr_stat (ch, STAT_STR) + 100, get_curr_stat (ch, STAT_STR) + 200);
	dam += ch->level * 4;
	damage (ch, victim, dam, gsn_bite, DAM_BASH, TRUE);
	}
return;
}


void claws( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int dam;

        if (ch->disease == DISEASE_WEREWOLF || ch->disease == DISEASE_UNDEAD
        || ch->disease == DISEASE_DEMON)        
	{
        dam = number_range(get_curr_stat (ch, STAT_STR) + 100, get_curr_stat (ch, STAT_STR) + 200);
        dam += ch->level * 4;
	damage (ch, victim, dam, gsn_claws, DAM_BASH, TRUE);

        }
return;
}


/*
 * See if an attack justifies a KILLER flag.
 */
void check_killer (CHAR_DATA * ch, CHAR_DATA * victim)
{
    char buf[MAX_STRING_LENGTH];
    /*
     * Follow charm thread to responsible character.
     * Attacking someone's charmed char is hostile!
     */
    while (IS_AFFECTED (victim, AFF_CHARM) && victim->master != NULL)
        victim = victim->master;

    /*
     * NPC's are fair game.
     * So are killers and thieves.
     */
    if (IS_NPC (victim)
        || IS_SET (victim->act, PLR_KILLER)
        || IS_SET (victim->act, PLR_THIEF))
        return;

    /*
     * Charm-o-rama.
     */
    if (IS_SET (ch->affected_by, AFF_CHARM))
    {
        if (ch->master == NULL)
        {
            char buf[MAX_STRING_LENGTH];

            sprintf (buf, "Check_killer: %s bad AFF_CHARM",
                     IS_NPC (ch) ? ch->short_descr : ch->name);
            bug (buf, 0);
            affect_strip (ch, gsn_charm_person);
            REMOVE_BIT (ch->affected_by, AFF_CHARM);
            return;
        }
/*
    send_to_char( "*** You are now a KILLER!! ***\n\r", ch->master );
      SET_BIT(ch->master->act, PLR_KILLER);
*/

        stop_follower (ch);
        return;
    }

    /*
     * NPC's are cool of course (as long as not charmed).
     * Hitting yourself is cool too (bleeding).
     * So is being immortal (Alander's idea).
     * And current killers stay as they are.
     */
    if (IS_NPC (ch)
        || ch == victim || ch->level >= LEVEL_IMMORTAL || !is_clan (ch)
        || IS_SET (ch->act, PLR_KILLER) || ch->fighting == victim)
        return;

    send_to_char ("*** You are now a KILLER!! ***\n\r", ch);
    SET_BIT (ch->act, PLR_KILLER);
    sprintf (buf, "$N is attempting to murder %s", victim->name);
    wiznet (buf, ch, NULL, WIZ_FLAGS, 0, 0);
    save_char_obj (ch);
    return;
}



/*
 * Check for parry.
 */
bool check_parry (CHAR_DATA * ch, CHAR_DATA * victim)
{
    int chance;

    if (!IS_AWAKE (victim))
        return FALSE;

    chance = get_skill (victim, gsn_parry) / 2;

    if (get_eq_char (victim, WEAR_WIELD) == NULL)
    {
        if (IS_NPC (victim))
            chance /= 2;
        else
            return FALSE;
    }

    if (!can_see (ch, victim))
        chance /= 2;

	if (ch->tension != 0)
		chance = chance * 2 / 3;


    if (IS_STANCE(victim, STANCE_CRANE)
        && !can_counter(ch, TRUE)
        && !can_bypass(ch,victim))
		{
        chance += (int) (victim->stance[STANCE_CRANE] * 0.25);
		if (!(stance_checked))	improve_stance(victim);
		}
    else if (IS_STANCE(victim, STANCE_MANTIS)
        && !can_counter(ch, TRUE)
        && !can_bypass(ch,victim))
		{
        chance += (int) (victim->stance[STANCE_MANTIS] * 0.25);
		if (!(stance_checked))	improve_stance(victim);
		}
	else if ((victim->stance[0]>0) && (victim->stance[victim->stance[0]]<100))
		chance /= 2;

	chance *= (2*get_weapon_skill(victim,get_weapon_sn(victim)) +
				 get_weapon_skill(victim,get_weapon_sn(ch)) );

        chance += get_curr_stat(victim,STAT_AGI) / 7;


	chance /= 300;

    if (number_percent () >= chance + victim->level - ch->level)
        return FALSE;
if (!IS_SET(ch->comm, COMM_BRIEF2))
    act ("You {cparry{x $n's attack.", ch, NULL, victim, TO_VICT);
if (!IS_SET(ch->comm, COMM_BRIEF2))
    act ("$N {cparries{x your attack.", ch, NULL, victim, TO_CHAR);
    check_improve (victim, gsn_parry, TRUE, 6);
	/* 
	 * Whip innate ability.
	 */
	if (get_weapon_sn (ch) == gsn_whip)
	{
		chance = get_skill (ch, gsn_whip) / 7;
		
		if (number_percent () < chance)
		{
			disarm (ch, victim);
			check_improve (ch, gsn_whip, TRUE, 3);
		}
	}
	else if (get_weapon_sn (victim) == gsn_whip)
	{
		chance = get_skill (ch, gsn_whip) / 8;

		if (number_percent () < chance)
		{
			disarm (victim, ch);
			check_improve (victim, gsn_whip, TRUE, 3);
		}
	}

    return TRUE;
}

/*
 * Check for shield block.
 */
bool check_shield_block (CHAR_DATA * ch, CHAR_DATA * victim)
{
    bool levitate;
    int chance;

    if (!IS_AWAKE (victim))
        return FALSE;



    levitate = ( get_skill ( victim, gsn_shield_levitation ) != 0 ) &&
	( ( skill_table[gsn_shield_levitation].skill_level[ch->class] <= ch->level ) 
	|| ( skill_table[gsn_shield_levitation].skill_level[ch->class2] <= ch->level2 )
	|| ( skill_table[gsn_shield_levitation].skill_level[ch->class3] <= ch->level3 ))
	&& ( get_eq_char ( victim, WEAR_SHIELD ) != NULL ) &&
        ( ( get_eq_char ( victim, WEAR_SECONDARY ) != NULL ) ||
          ( ( get_eq_char ( victim, WEAR_WIELD ) != NULL ) &&
            ( get_eq_char ( victim, WEAR_SECONDARY ) == NULL ) &&
            IS_WEAPON_STAT ( get_eq_char ( victim, WEAR_WIELD ),
                             WEAPON_TWO_HANDS ) ) );

    chance = get_skill (victim, gsn_shield_block) / 5 + 3;
    if ( levitate )
        chance =
            ( chance +
              ( get_skill ( victim, gsn_shield_levitation ) / 3 + 3 ) ) / 2;

	if (ch->tension != 0)
		chance = chance * 2 / 3;


    if (get_eq_char (victim, WEAR_SHIELD) == NULL)
        return FALSE;

    if (number_percent () >= chance + victim->level - ch->level)
        return FALSE;

		if (!IS_SET(ch->comm, COMM_BRIEF2))
    act ("You block $n's attack with your shield.", ch, NULL, victim,
         TO_VICT);
		if (!IS_SET(ch->comm, COMM_BRIEF2))
    act ("$N blocks your attack with a shield.", ch, NULL, victim, TO_CHAR);
    check_improve (victim, gsn_shield_block, TRUE, 6);
    if ( levitate )
        check_improve ( victim, gsn_shield_levitation, TRUE, 6 );
    return TRUE;
}


/*
 * Check for dodge.
 */
bool check_dodge (CHAR_DATA * ch, CHAR_DATA * victim)
{
    int chance;

    if (!IS_AWAKE (victim))
        return FALSE;

    chance = get_skill (victim, gsn_dodge) / 2;

	if (ch->tension != 0)
		chance = chance * 2 / 3;


    if (IS_STANCE(victim, STANCE_MONGOOSE)
        && !can_counter(ch, TRUE)
        && !can_bypass(ch,victim))
		{
        chance += (int) (victim->stance[STANCE_MONGOOSE] * 0.25);
		if (!(stance_checked))	improve_stance(victim);
		}
    else if (IS_STANCE(victim, STANCE_SWALLOW)
        && !can_counter(ch, TRUE)
        && !can_bypass(ch,victim))
		{
        chance += (int) (victim->stance[STANCE_SWALLOW] * 0.25);
		if (!(stance_checked))	improve_stance(victim);
		}
	else if ((victim->stance[0]>0) && (victim->stance[victim->stance[0]]<100))
		chance /= 2;
	chance += get_curr_stat(victim,STAT_AGI) / 7;

    if (!can_see (victim, ch))
        chance /= 2;

    if (number_percent () >= chance + victim->level - ch->level)
        return FALSE;
		if (!IS_SET(ch->comm, COMM_BRIEF2))
    act ("You {cdodge{x $n's attack.", ch, NULL, victim, TO_VICT);
		if (!IS_SET(ch->comm, COMM_BRIEF2))
    act ("$N {cdodges{x your attack.", ch, NULL, victim, TO_CHAR);
    check_improve (victim, gsn_dodge, TRUE, 6);
    return TRUE;
}



bool check_phase( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;

    if ( !IS_AWAKE(victim) )
	return FALSE;

    chance = get_skill(victim,gsn_phase) / 2;

	if (ch->tension != 0)
		chance = chance * 2 / 3;


    if (!can_see(victim,ch))
	chance /= 2;

    if ( number_percent( ) >= chance + victim->level - ch->level )
        return FALSE;

    act( "Your body {cphases{x to avoid $n's attack.", ch, NULL, victim, TO_VICT    );
    act( "$N's body {cphases{x to avoid your attack.", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_phase,TRUE,6);
    return TRUE;
}


/*
 * Set position of a victim.
 */
void update_pos (CHAR_DATA * victim)
{
    if (victim->hit > 0)
    {
        if (victim->position <= POS_STUNNED)
            victim->position = POS_STANDING;
        return;
    }

    if (IS_NPC (victim) && victim->hit < 1)
    {
        victim->position = POS_DEAD;
        return;
    }

    if (victim->hit <= -11)
    {
        victim->position = POS_DEAD;
        return;
    }

    if (victim->hit <= -6)
        victim->position = POS_MORTAL;
    else if (victim->hit <= -3)
        victim->position = POS_INCAP;
    else
        victim->position = POS_STUNNED;

    return;
}

bool check_critical(CHAR_DATA *ch, CHAR_DATA *victim)
{
    OBJ_DATA *obj = NULL;
    int chance;

    /* check for damage from hand-held weapons */
//    if ( !throw_damage )
//    {
        obj = get_eq_char(ch,WEAR_WIELD);

        if (
             ( ( obj == NULL && get_skill(ch,gsn_hand_to_hand) != 100 ) ||
             ( obj != NULL && get_weapon_skill(ch,get_weapon_sn(ch)) < 90 ) )
           )
                return FALSE;
/*    }
    else
    {
        if ( get_skill(ch,gsn_throw) < 90 )
            return FALSE;
    }
*/

    /* mob even get a 1% chance of landing a Critical Hit */
    if ( IS_NPC( ch ) )
        chance = 1;
    else
    {
        if ( obj == NULL )	/* no weapon? 2% chance for hand to hand */
            chance = 2;
        else			/* increased chance when weapon skill is >= 90% */
            chance = 10 - ( 100 - get_weapon_skill( ch, get_weapon_sn( ch ) ) );

        if ( IS_IMMORTAL( ch ) )	/* Immortals?? 50% chance, just cause you rock */
            chance = 50;
    }

    if ( number_range(0,100) > chance )
        return FALSE;			/* check to see if it was your lucky day */


    /* Now, if it passed all the tests... */
    if ( obj == NULL )		/* you personally strike */
    {
	if (!IS_SET (ch->comm, COMM_BRIEF3))
	{
        act("$n {cCRITICALLY STRIKES{x $N!",ch,NULL,victim,TO_NOTVICT);
        act("You {cCRITICALLY STRIKE{x $N!",ch,NULL,victim,TO_CHAR);
	}
    }
    else			/* your weapon strikes */
	 if (!IS_SET (ch->comm, COMM_BRIEF3))
        act("$p {cCRITICALLY STRIKES{x $n!",victim,obj,NULL,TO_NOTVICT);

    /* let the victim know they just got a beat down */
 if (!IS_NPC(victim) && !IS_SET (victim->comm, COMM_BRIEF3))
    act("{cCRITICAL STRIKE!{x",ch,NULL,victim,TO_VICT);
    return TRUE;
}


bool check_counter( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt)
{
        int chance;
        int dam_type;
        OBJ_DATA *wield;

        if (    ( get_eq_char(victim, WEAR_WIELD) == NULL ) ||
                ( !IS_AWAKE(victim) ) ||
                ( !can_see(victim,ch) ) ||
                ( get_skill(victim,gsn_counter) < 1 )
           )
           return FALSE;

        wield = get_eq_char(victim,WEAR_WIELD);

        chance = get_skill(victim,gsn_counter) / 6;
        chance += ( victim->level - ch->level ) / 2;
//        chance += 2 * (get_curr_stat(victim,STAT_DEX) - get_curr_stat(ch,STAT_DEX));
	chance += get_curr_stat(ch,STAT_DEX) / 15;
        chance += get_weapon_skill(victim,get_weapon_sn(victim)) -
                        get_weapon_skill(ch,get_weapon_sn(ch));
//        chance += (get_curr_stat(victim,STAT_STR) - get_curr_stat(ch,STAT_STR) );
        chance += get_curr_stat(ch,STAT_STR) / 15;


    if ( number_percent( ) >= chance )
        return FALSE;

        dt = gsn_counter;

    if ( dt == TYPE_UNDEFINED )
    {
	dt = TYPE_HIT;
	if ( wield != NULL && wield->item_type == ITEM_WEAPON )
	    dt += wield->value[3];
	else 
            dt += ch->dam_type;
    }

    if (dt < TYPE_HIT)
    	if (wield != NULL)
    	    dam_type = attack_table[wield->value[3]].damage;
    	else
    	    dam_type = attack_table[ch->dam_type].damage;
    else
    	dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
	dam_type = DAM_BASH;

    act( "You reverse $n's attack and counter with your own!", ch, NULL, victim, TO_VICT    );
    act( "$N reverses your attack!", ch, NULL, victim, TO_CHAR    );

    damage(victim,ch,dam/2, gsn_counter , dam_type ,TRUE ); /* DAM MSG NUMBER!! */

    check_improve(victim,gsn_counter,TRUE,6);

    return TRUE;
}

bool check_force_shield( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;

	if(!IS_AFFECTED(victim, AFF_FORCE_SHIELD))
		return FALSE;

    chance = 100 / 15;

    if (victim->level >= ch->level)
	chance += 2;

    if ( number_percent( ) >= chance )
        return FALSE;

    act("Your force-shield blocks $n's attack!", ch, NULL, victim, TO_VICT);
    act("$N's force-shield blocks your attack.", ch, NULL, victim, TO_CHAR);

    return TRUE;
}

/*
 *  Shield Spell Group by Tandon
 *  Static Shield Check
 */

bool check_static_shield( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance, sn;
	AFFECT_DATA *shock;

	if(!IS_AFFECTED(victim, AFF_STATIC_SHIELD))
		return FALSE;

    chance = 10;

    if (victim->level >= ch->level)
	chance += 2;

    if ( number_percent( ) >= chance )
        return FALSE;

    
	sn = skill_lookup( "static shield" );
	shock = affect_find (victim->affected, sn);
	
	if(shock != NULL)
	{
		damage (victim, ch, number_fuzzy(shock->level / 5), sn, DAM_ENERGY, TRUE);
	}

    if (get_eq_char (ch, WEAR_WIELD) == NULL)
		return TRUE;

    act("Your static shield catches $n!", victim, NULL, ch, TO_VICT);
    act("$N's static shield catches you!", victim, NULL, ch, TO_CHAR);
    
	spell_heat_metal (skill_lookup( "heat metal" ),
                     victim->level/2, victim, (void *) ch, TARGET_CHAR);		

    return TRUE;
}

/*
 *  Shield Spell Group by Tandon
 *  Flame Shield Check
 */

bool check_flame_shield( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance, sn;
	AFFECT_DATA *burn;

	if(!IS_AFFECTED(victim, AFF_FLAME_SHIELD))
		return FALSE;

    if (get_eq_char (victim, WEAR_WIELD) != NULL)
		return FALSE;

    chance = 100 / 3;

    if (victim->level >= ch->level)
	chance += 2;

    if ( number_percent( ) >= chance )
        return FALSE;

	sn = skill_lookup( "flame shield" );
	burn = affect_find (victim->affected, sn);
	
	if(burn != NULL)
	{   
		fire_effect (ch, burn->level, number_fuzzy(10), TARGET_CHAR);
		damage (victim, ch, number_fuzzy(burn->level), sn, DAM_FIRE, TRUE);
	}

    return TRUE;
}






/*
 * Start fights.
 */
void set_fighting (CHAR_DATA * ch, CHAR_DATA * victim)
{
    if (ch->fighting != NULL)
    {
        bug ("Set_fighting: already fighting", 0);
        return;
    }

    if (IS_AFFECTED (ch, AFF_SLEEP))
        affect_strip (ch, gsn_sleep);

    ch->fighting = victim;
    ch->position = POS_FIGHTING;

    return;
}



/*
 * Stop fights.
 */
void stop_fighting (CHAR_DATA * ch, bool fBoth)
{
    CHAR_DATA *fch;

    for (fch = char_list; fch != NULL; fch = fch->next)
    {
        if (fch == ch || (fBoth && fch->fighting == ch))
        {
            fch->fighting = NULL;
            fch->position = IS_NPC (fch) ? fch->default_pos : POS_STANDING;
            update_pos (fch);
						strip_tension (fch);

        }
    }

    return;
}

/*
 * Make a corpse out of a character.
 */
void make_corpse ( CHAR_DATA * ch )
{
    char buf[MAX_STRING_LENGTH];
		CHAR_DATA * killer;
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char *name;
    ROOM_INDEX_DATA *morgue;

    morgue = get_room_index ( ROOM_VNUM_MORGUE );

    if ( IS_NPC ( ch ) )
    {
                //for ( obj = ch->carrying; obj != NULL; obj = obj_next )
		for ( obj = ch->carrying; obj; obj = obj->next_content )
                {
                    //obj_next = obj->next_content;
                    obj_from_char ( obj );
                    if ( obj->item_type == ITEM_POTION )
                        obj->timer = number_range ( 500, 1000 );
                    if ( obj->item_type == ITEM_SCROLL )
                        obj->timer = number_range ( 1000, 2500 );
                    if ( IS_SET ( obj->extra_flags, ITEM_ROT_DEATH ) )
                    {
                        obj->timer = number_range ( 5, 10 );
                        REMOVE_BIT ( obj->extra_flags, ITEM_ROT_DEATH );
                    }
                    REMOVE_BIT ( obj->extra_flags, ITEM_VIS_DEATH );

                    if ( IS_SET ( obj->extra_flags, ITEM_INVENTORY ) )
                        extract_obj ( obj );
                    act ( "$p falls to the floor.", ch, obj, NULL, TO_ROOM );
                    obj_to_room ( obj, ch->in_room );
										
            return;
					}
        name = ch->short_descr;
        corpse = create_object ( get_obj_index ( OBJ_VNUM_CORPSE_NPC ), 0 );
        corpse->timer = number_range ( 3, 6 );
        if ( ch->gold > 0 || ch->platinum > 0 || ch->silver > 0 )
        {
            obj_to_obj ( create_money ( ch->platinum, ch->gold, ch->silver ),
                         corpse );
            ch->platinum = 0;
            ch->gold = 0;
            ch->silver = 0;
        }
        corpse->cost = 0;
    }
    else
    {
        name = ch->name;
        corpse = create_object ( get_obj_index ( OBJ_VNUM_CORPSE_PC ), 0 );
        corpse->timer = number_range ( 25, 40 );
        REMOVE_BIT ( ch->act, PLR_CANLOOT );
        if ( !is_clan ( ch ) )
        {
            free_string ( corpse->owner );
            corpse->owner = str_dup ( ch->name );
            corpse->killer = NULL;
        }
        else
        {
            free_string ( corpse->owner );
            corpse->owner = str_dup ( ch->name );
            free_string ( corpse->killer );
            corpse->killer = str_dup ( killer->name );
            if ( ch->platinum > 1 || ch->gold > 1 || ch->silver > 1 )
            {
                obj_to_obj ( create_money
                             ( ch->platinum / 2, ch->gold / 2,
                               ch->silver / 2 ), corpse );
                ch->platinum -= ch->platinum / 2;
                ch->gold -= ch->gold / 2;
                ch->silver -= ch->silver / 2;
            }
        }

        corpse->cost = 0;
    }

    corpse->level = ch->level;

    sprintf ( buf, corpse->short_descr, name );
    free_string ( corpse->short_descr );
    corpse->short_descr = str_dup ( buf );

    sprintf ( buf, corpse->description, name );
    free_string ( corpse->description );
    corpse->description = str_dup ( buf );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        bool floating = FALSE;

        obj_next = obj->next_content;
        if ( obj->wear_loc == WEAR_FLOAT )
            floating = TRUE;
        obj_from_char ( obj );
        if ( obj->item_type == ITEM_POTION )
            obj->timer = number_range ( 500, 1000 );
        if ( obj->item_type == ITEM_SCROLL )
            obj->timer = number_range ( 1000, 2500 );
        if ( IS_SET ( obj->extra_flags, ITEM_ROT_DEATH ) && !floating )
        {
            obj->timer = number_range ( 5, 10 );
            REMOVE_BIT ( obj->extra_flags, ITEM_ROT_DEATH );
        }
        REMOVE_BIT ( obj->extra_flags, ITEM_VIS_DEATH );

        if ( IS_SET ( obj->extra_flags, ITEM_INVENTORY ) )
            extract_obj ( obj );
        else if ( floating )
        {
            if ( IS_OBJ_STAT ( obj, ITEM_ROT_DEATH ) )  /* get rid of it! */
            {
                if ( obj->contains != NULL )
                {
                    OBJ_DATA *in, *in_next;

                    act ( "$p evaporates,scattering its contents.", ch, obj,
                          NULL, TO_ROOM );
                    for ( in = obj->contains; in != NULL; in = in_next )
                    {
                        in_next = in->next_content;
                        obj_from_obj ( in );
                        obj_to_room ( in, ch->in_room );
                    }
                }
                else
                    act ( "$p evaporates.", ch, obj, NULL, TO_ROOM );
                extract_obj ( obj );
            }

            else
            {
                act ( "$p falls to the floor.", ch, obj, NULL, TO_ROOM );
                obj_to_room ( obj, ch->in_room );
            }
        }
        else
            obj_to_obj ( obj, corpse );
    }

    obj_to_room ( corpse, ch->in_room );

    return;
}

/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry ( CHAR_DATA * ch )
{
    ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;
    int vnum;

    vnum = 0;
    msg = "You hear $n's death cry.";
    if ( IS_NPC ( ch ) )
    {
            switch ( number_bits ( 4 ) )
            {
                case 0:
                    msg = "$n hits the ground ... DEAD.";
                    vnum = OBJ_VNUM_BLOOD;
                    break;
                case 1:
                    msg = "$n splatters blood on your armor.";
                    vnum = OBJ_VNUM_BLOOD;
                    break;
                case 2:
                    if ( IS_SET ( ch->parts, PART_GUTS ) )
                    {
                        msg = "$n spills $s guts all over the floor.";
                        vnum = OBJ_VNUM_GUTS;
                    }
                    break;
                case 3:
                    if ( IS_SET ( ch->parts, PART_HEAD ) )
                    {
                        msg = "$n's severed head plops on the ground.";
                        vnum = OBJ_VNUM_SEVERED_HEAD;
                    }
                    break;
                case 4:
                    if ( IS_SET ( ch->parts, PART_HEART ) )
                    {
                        msg = "$n's heart is torn from $s chest.";
                        vnum = OBJ_VNUM_TORN_HEART;
                    }
                    break;
                case 5:
                    if ( IS_SET ( ch->parts, PART_ARMS ) )
                    {
                        msg = "$n's arm is sliced from $s dead body.";
                        vnum = OBJ_VNUM_SLICED_ARM;
                    }
                    break;
                case 6:
                    if ( IS_SET ( ch->parts, PART_LEGS ) )
                    {
                        msg = "$n's leg is sliced from $s dead body.";
                        vnum = OBJ_VNUM_SLICED_LEG;
                    }
                    break;
                case 7:
                    if ( IS_SET ( ch->parts, PART_BRAINS ) )
                    {
                        msg =
                            "$n's head is shattered, and $s brains splash all over you.";
                        vnum = OBJ_VNUM_BRAINS;
                    }
                    break;
                case 8:
                    msg = "$n hits the ground ... DEAD.";
                    vnum = OBJ_VNUM_BLOOD;
                    break;
                case 9:
                    msg = "$n hits the ground ... DEAD.";
                    vnum = OBJ_VNUM_BLOOD;
        }
    }
    else if ( ch->level > 19 )
    {
        switch ( number_bits ( 4 ) )
        {
            case 0:
                msg = "$n hits the ground ... DEAD.";
                vnum = OBJ_VNUM_BLOOD;
                break;
            case 1:
                msg = "$n splatters blood on your armor.";
                vnum = OBJ_VNUM_BLOOD;
                break;
            case 2:
                if ( IS_SET ( ch->parts, PART_GUTS ) )
                {
                    msg = "$n spills $s guts all over the floor.";
                    vnum = OBJ_VNUM_GUTS;
                }
                break;
            case 3:
                if ( IS_SET ( ch->parts, PART_HEAD ) )
                {
                    msg = "$n's severed head plops on the ground.";
                    vnum = OBJ_VNUM_SEVERED_HEAD;
                }
                break;
            case 4:
                if ( IS_SET ( ch->parts, PART_HEART ) )
                {
                    msg = "$n's heart is torn from $s chest.";
                    vnum = OBJ_VNUM_TORN_HEART;
                }
                break;
            case 5:
                if ( IS_SET ( ch->parts, PART_ARMS ) )
                {
                    msg = "$n's arm is sliced from $s dead body.";
                    vnum = OBJ_VNUM_SLICED_ARM;
                }
                break;
            case 6:
                if ( IS_SET ( ch->parts, PART_LEGS ) )
                {
                    msg = "$n's leg is sliced from $s dead body.";
                    vnum = OBJ_VNUM_SLICED_LEG;
                }
                break;
            case 7:
                if ( IS_SET ( ch->parts, PART_BRAINS ) )
                {
                    msg =
                        "$n's head is shattered, and $s brains splash all over you.";
                    vnum = OBJ_VNUM_BRAINS;
                }
                break;
            case 8:
                msg = "$n hits the ground ... DEAD.";
                vnum = OBJ_VNUM_BLOOD;
                break;
            case 9:
                msg = "$n hits the ground ... DEAD.";
                vnum = OBJ_VNUM_BLOOD;
                break;
            case 10:
                if ( IS_SET ( ch->parts, PART_HEAD ) )
                {
                    msg = "$n's severed head plops on the ground.";
                    vnum = OBJ_VNUM_SEVERED_HEAD;
                }
                break;
            case 11:
                if ( IS_SET ( ch->parts, PART_HEART ) )
                {
                    msg = "$n's heart is torn from $s chest.";
                    vnum = OBJ_VNUM_TORN_HEART;
                }
                break;
            case 12:
                if ( IS_SET ( ch->parts, PART_ARMS ) )
                {
                    msg = "$n's arm is sliced from $s dead body.";
                    vnum = OBJ_VNUM_SLICED_ARM;
                }
                break;
            case 13:
                if ( IS_SET ( ch->parts, PART_LEGS ) )
                {
                    msg = "$n's leg is sliced from $s dead body.";
                    vnum = OBJ_VNUM_SLICED_LEG;
                }
                break;
            case 14:
                if ( IS_SET ( ch->parts, PART_BRAINS ) )
                {
                    msg =
                        "$n's head is shattered, and $s brains splash all over you.";
                    vnum = OBJ_VNUM_BRAINS;
                }
        }
    }

    act ( msg, ch, NULL, NULL, TO_ROOM );

    if ( ( vnum == 0 ) )
    {
        switch ( number_bits ( 4 ) )
        {
            case 0:
                vnum = 0;
                break;
            case 1:
                vnum = OBJ_VNUM_BLOOD;
                break;
            case 2:
                vnum = 0;
                break;
            case 3:
                vnum = OBJ_VNUM_BLOOD;
                break;
            case 4:
                vnum = 0;
                break;
            case 5:
                vnum = OBJ_VNUM_BLOOD;
                break;
            case 6:
                vnum = 0;
                break;
            case 7:
                vnum = OBJ_VNUM_BLOOD;
        }
    }

    if ( vnum != 0 )
    {
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;
        char *name;

        name = IS_NPC ( ch ) ? ch->short_descr : ch->name;
        obj = create_object ( get_obj_index ( vnum ), 0 );
        obj->timer = number_range ( 4, 7 );
        if ( !IS_NPC ( ch ) )
        {
            obj->timer = number_range ( 12, 18 );
        }
        if ( vnum == OBJ_VNUM_BLOOD )
        {
            obj->timer = number_range ( 1, 4 );
        }

        sprintf ( buf, obj->short_descr, name );
        free_string ( obj->short_descr );
        obj->short_descr = str_dup ( buf );

        sprintf ( buf, obj->description, name );
        free_string ( obj->description );
        obj->description = str_dup ( buf );

        sprintf ( buf, obj->name, name );
        free_string ( obj->name );
        obj->name = str_dup ( buf );

        if ( obj->item_type == ITEM_FOOD )
        {
            if ( IS_SET ( ch->form, FORM_POISON ) )
                obj->value[3] = 1;
            else if ( !IS_SET ( ch->form, FORM_EDIBLE ) )
                obj->item_type = ITEM_TRASH;
        }

        if ( IS_NPC ( ch ) )
        {
            obj->value[4] = 0;
        }
        else
        {
            obj->value[4] = 1;
        }

        obj_to_room ( obj, ch->in_room );
    }

    if ( IS_NPC ( ch ) )
        msg = "You hear something's death cry.";
    else
        msg = "You hear someone's death cry.";

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
        EXIT_DATA *pexit;

        if ( ( pexit = was_in_room->exit[door] ) != NULL &&
             pexit->u1.to_room != NULL && pexit->u1.to_room != was_in_room )
        {
            ch->in_room = pexit->u1.to_room;
            act ( msg, ch, NULL, NULL, TO_ROOM );
        }
    }
    ch->in_room = was_in_room;

    return;
}




void raw_kill (CHAR_DATA * victim)
{
		CHAR_DATA * ch;
    int i;

    stop_fighting (victim, TRUE);
    death_cry (victim);
    make_corpse (victim);

    if (IS_NPC (victim))
    {
        victim->pIndexData->killed++;
        kill_table[URANGE (0, victim->level, MAX_LEVEL - 1)].killed++;
        extract_char (victim, TRUE);
        return;
    }

  if (!IS_NPC (victim))
  {
      mobdeaths_today++;
	if (!IS_NPC(ch) && !IS_NPC(victim)) pkills_today++;

   }


    extract_char (victim, FALSE);
    while (victim->affected)
        affect_remove (victim, victim->affected);
    victim->affected_by = race_table[victim->race].aff;
    for (i = 0; i < 4; i++)
        victim->armor[i] = 100;
    victim->position = POS_RESTING;
    victim->hit = UMAX (1, victim->hit);
    victim->mana = UMAX (1, victim->mana);
    victim->move = UMAX (1, victim->move);
    free_cast(victim); // Make them stop casting ;)
    do_call(victim, "all");
    save_char_obj( victim ); 
    return;
}



void group_gain (CHAR_DATA * ch, CHAR_DATA * victim)
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    OBJ_DATA *wield;
    OBJ_DATA *wield2;
    OBJ_DATA *obj;
    AFFECT_DATA *paf;
    int xp;
    int members;
    int group_levels;
    int livingweap_exp = 0;
    int livingweap_needed = 0;
    int arti_exp = 0;
    int arti_needed = 0;
		int gain;
		int orig = 0;
    /*
     * Monsters don't get kill xp's or alignment changes.
     * P-killing doesn't help either.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     */
    if (victim == ch)
        return;

    members = 0;
    group_levels = 0;
    for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
        if (is_same_group (gch, ch))
        {
            members++;
            group_levels += IS_NPC (gch) ? gch->level / 2 : gch->level;
        }
    }
        if ( IS_QUESTOR ( ch ) && IS_NPC ( victim ) )
        {
            if ( ch->pcdata->questmob == victim->pIndexData->vnum && victim->isquestmob > 0 ) 
            {
                send_to_char
                    ( "{RYou have almost completed your {GQ{gU{GE{gS{GT{Y!{x\n\r",
                      ch );
                send_to_char
                    ( "{RReturn to the {Bq{bu{Be{bs{Bt{bm{Ba{bs{Bt{be{Br{R before your time runs out{Y!{x\n\r",
                      ch );
                ch->pcdata->questmob = -1;
            }
	    else if ( ch->pcdata->questmob == victim->pIndexData->vnum && victim->isquestmob == 0 )
	    {
	    	send_to_char ( "{RYou killed the wrong mob. This was not the one you had to kill.{x\n\r", ch );
	    }
        }


    if (members == 0)
    {
        bug ("Group_gain: members.", members);
        members = 1;
        group_levels = ch->level;
    }

    lch = (ch->leader != NULL) ? ch->leader : ch;

    for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;

        if (!is_same_group (gch, ch) || IS_NPC (gch))
            continue;

        xp = xp_compute (gch, victim, group_levels);

	if (!IS_NPC(gch))
	{
	sprintf (buf, "{YYou recieve {R%d {Yexperience points.{x\n\r", xp);
        send_to_char (buf, gch);
	}

	if (global_exp)
        {
            sprintf ( buf, "{YYou recieve {R%d {Ydouble exp!{x\n\r", xp );
            send_to_char ( buf, gch );
            xp = xp * 2;
        }

        if (xp == 0)
        {
        xp += number_range(4500,6500);
        sprintf (buf, "{YSlaughter Bonus! {R%d {Yexperience points.{x\n\r", xp);
        send_to_char (buf, gch);
        }


//	if (!IS_NPC(ch))
//	{
//	ch->pcdata->mkills++;
//        mobkills_today++;
//	sprintf(buf, "{R[{YMob Kills{R:{Y%d{R]{x\n\r", ch->pcdata->mkills);
//	send_to_char(buf, gch);
//	}

        
//	if (ch->class2 == -1 && ch->class3 == -1)
//	gain_exp (gch, xp);
	ch->exp += xp; // to replace gain_exp
	gain = xp;

    // if ( ch->level >= LEVEL_HERO )
		if (!IS_NPC(ch))
    {
        if (gain < 0)
//                return;
        if (ch->pcdata->focus[CURRENT_FOCUS] > MAGIC_DEFENSE ||
          ch->pcdata->focus[CURRENT_FOCUS] < COMBAT_POWER )
                return;
        orig = focus_level(ch->pcdata->focus[ch->pcdata->focus[CURRENT_FOCUS]]);
        ch->pcdata->focus[ch->pcdata->focus[CURRENT_FOCUS]] += gain;
        if (orig < focus_level(ch->pcdata->focus[ch->pcdata->focus[CURRENT_FOCUS]]))
                focus_up(ch);
//        return;
    }

        wield = get_eq_char( ch, WEAR_WIELD );
        wield2 = get_eq_char( ch, WEAR_SECONDARY );

        if (wield != NULL) 
        {
        livingweap_needed = 1500;

//	if (!wield->extra_flags, ITEM_RELIC) return;
	if (IS_OBJ_STAT (wield, ITEM_WEAPON))
	{
              if (wield->weapon_level < 100) 
		{
              livingweap_exp = (xp * .02);
              xp -= (livingweap_exp);

              sprintf(buf, "{gYour Weapon receives {w%d {gexperience points.{x\n\r", livingweap_exp);
		  stc(buf, gch);

              wield->weapon_currentxp += livingweap_exp;

              if (wield->weapon_currentxp >= livingweap_needed )
		  {
              wield->weapon_level++;
              wield->value[2]++;
              send_to_char("{wYour {rWeapon {whas gained a Level{x\n\r", gch);
		  wield->weapon_currentxp -= livingweap_needed;
		  wield->weapon_points++;
		}
              }
         }
}

        if (wield2 != NULL)
        {
        livingweap_needed = 1500;

//	      if (!wield2->extra_flags, ITEM_RELIC) return;
        if (IS_OBJ_STAT (wield2, ITEM_WEAPON))
//    if (wield2->item_type == ITEM_WEAPON)
	{
              if (wield2->weapon_level < 100) 
		{
              livingweap_exp = (xp * .02);
              xp -= (livingweap_exp);

              sprintf(buf, "{gYour Weapon receives {w%d {gexperience points.{x\n\r", livingweap_exp);
		  stc(buf, gch);
         
              wield2->weapon_currentxp += livingweap_exp;
               
              if (wield2->weapon_currentxp >= livingweap_needed )
		  {
              wield2->weapon_level++;
              wield2->value[2]++;
              send_to_char("{wYour {rWeapon {whas gained a Level{x\n\r", gch);
		  wield2->weapon_currentxp -= livingweap_needed;
		  wield2->weapon_points++;		
		}
	}
	             }
		}


//        head = get_eq_char( ch, WEAR_HEAD );

        for (obj = ch->carrying; obj != NULL; obj = obj_next)
        {
            obj_next = obj->next_content;
            if (obj->wear_loc == WEAR_NONE)
                continue;

//        if (obj != NULL)
	if (obj->item_type == ITEM_ARMOR)
	{
	    arti_needed = 20000;

//            if (IS_OBJ_STAT (obj, ITEM_ARTIFACT))
//            {
              if (obj->weapon_level < 10)
                {
              arti_exp = (xp * .2);
              xp -= (arti_exp);

              sprintf(buf, "{gYour Armor receives {w%d {gexperience points.{x\n\r", arti_exp);
                  stc(buf, gch);

              obj->weapon_currentxp += arti_exp;
		}

              if (obj->weapon_currentxp >= arti_needed )
                  {
              obj->weapon_level++;
              obj->value[0] += 10;
              obj->value[1] += 10;
              obj->value[2] += 10;
              obj->value[3] += 10;
              send_to_char("{wYour {rArtifact {whas gained a Level{x\n\r", gch);
                  obj->weapon_currentxp -= arti_needed;
                  obj->weapon_points++;
}	}
}

//}




        for (obj = ch->carrying; obj != NULL; obj = obj_next)
        {
            obj_next = obj->next_content;
            if (obj->wear_loc == WEAR_NONE)
                continue;

            if ((IS_OBJ_STAT (obj, ITEM_ANTI_EVIL) && IS_EVIL (ch))
                || (IS_OBJ_STAT (obj, ITEM_ANTI_GOOD) && IS_GOOD (ch))
                || (IS_OBJ_STAT (obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL (ch)))
            {
                act ("You are zapped by $p.", ch, obj, NULL, TO_CHAR);
                act ("$n is zapped by $p.", ch, obj, NULL, TO_ROOM);
                obj_from_char (obj);
                obj_to_room (obj, ch->in_room);
            }
        }
    }

    return;
}



/*
 * Compute xp for a kill.
 * Also adjust alignment of killer.
 * Edit this function to change xp computations.
 */
int xp_compute (CHAR_DATA * gch, CHAR_DATA * victim, int total_levels)
{
    int xp, base_exp;
		int gain;
//    int align,
int level_range;
//    int change;
//    int time_per_level;
//    char buf[MAX_STRING_LENGTH];

    level_range = victim->level - gch->level;


    /* compute the base exp */
    switch (level_range)
    {
        default:
            base_exp = 0;
            break;
        case -9:
            base_exp = 10;
            break;
        case -8:
            base_exp = 20;
            break;
        case -7:
            base_exp = 50;
            break;
        case -6:
            base_exp = 90;
            break;
        case -5:
            base_exp = 110;
            break;
        case -4:
            base_exp = 220;
            break;
        case -3:
            base_exp = 330;
            break;
        case -2:
            base_exp = 500;
            break;
        case -1:
            base_exp = 660;
            break;
        case 0:
            base_exp = 830;
            break;
        case 1:
            base_exp = 990;
            break;
        case 2:
            base_exp = 1210;
            break;
        case 3:
            base_exp = 1430;
            break;
        case 4:
            base_exp = 1650;
            break;
    }

//    if (level_range > 4)
//        base_exp = 140 + 20 * (level_range - 4);	// was levelrange - 4


/* Lets try something else -xrak */

	if (level_range > 1)
	base_exp += 150 * (level_range );

    //if ( level_range > 4 ) From ROT, SD 
        //base_exp = 160 + 20 * ( level_range - 4 );


  /* calculate exp multiplier */
    if (IS_SET (victim->act, ACT_NOALIGN))
        xp = base_exp;

  /* alignment */  
  else if ((IS_EVIL(gch) && IS_GOOD(victim)) || (IS_EVIL(victim) && IS_GOOD(gch)))
    xp = base_exp * 8/5;
  
  else if ( IS_GOOD(gch) && IS_GOOD(victim) )
    xp = 0;

  else if ( !IS_NEUTRAL(gch) && IS_NEUTRAL(victim) )
    xp = base_exp * 1.1;

  else if ( IS_NEUTRAL(gch) && !IS_NEUTRAL(victim) )
    xp = base_exp * 1.3;

  else xp = base_exp;

    /* Xp Increase for newbies -Xrak*/
	if (gch->level < 20)
	xp = 7 * xp;

/*
	Why is this in here? seems like overkill, -Xrak
	if (gch->level < victim->level)
	{
	xp += level_range * 30;
	}
*/
	/* level_range = victim->level - gch->level; */

/*
	I put in something, shouldnt need this anymore, -Xrak
        if (level_range < 50 && level_range > 0) xp += 1000;
        if (level_range < 40 && level_range > 0) xp += 500;
        if (level_range < 35 && level_range > 0) xp += 500;
	if (level_range < 30 && level_range > 0) xp += 500;
        if (level_range < 25 && level_range > 0) xp += 500;
        if (level_range < 20 && level_range > 0) xp += 500;
        if (level_range < 15 && level_range > 0) xp += 500;
        if (level_range < 10 && level_range > 0) xp += 500;
        if (level_range < 5 && level_range > 0) xp += 500;
*/

    /* more exp at the low levels */
//    if (gch->level < 6)
//        xp = 20 * xp / (gch->level + 4);

    /* less at high */
//    if (gch->level > 35)
//        xp = 30 * xp / (gch->level - 25);

    /* reduce for playing time */

//    {
//        time_per_level = 4 *
//            (gch->played + (int) (current_time - gch->logon)) / 3600
//            / gch->level;
//
//        time_per_level = URANGE (2, time_per_level, 12);
//        if (gch->level < 15)  
//            time_per_level = UMAX (time_per_level, (15 - gch->level));
//        xp = xp * time_per_level / 12;
//    }
  
  /* randomize the rewards */
    xp = number_range (xp * 3 / 4, xp * 5 / 4);

    /* adjust for grouping */
    xp = xp * gch->level / (UMAX (1, total_levels - 1));

//    if (global_exp)
//	xp = 2 * xp;

//    if (IS_AFFECTED(gch, AFF_4XEXP)) xp * 4;

	if (xp > 10000) xp = 10000;


    return xp;
}


/* void dam_message (CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt,
                  bool immune)
{
    char buf1[256], buf2[256], buf3[256];
    const char *vs;
    const char *vp;
    const char *attack;
    char punct;
    int dam_percent = ((100 * dam) / victim->max_hit);

    if (ch == NULL || victim == NULL)
        return;

*/
/*    if (dam == 0)
    {
        vs = "miss";
        vp = "misses";
    }
    else if (dam_percent <= 5)
    {
        vs = "scratch";
        vp = "scratches";
    }
    else if (dam_percent <= 10)
    {
        vs = "graze";
        vp = "grazes";
    }
    else if (dam_percent <= 15)
    {
        vs = "hit";
        vp = "hits";
    }
    else if (dam_percent <= 20)
    {
        vs = "injure";
        vp = "injures";
    }
    else if (dam_percent <= 25)
    {
        vs = "wound";
        vp = "wounds";
    }
    else if (dam_percent <= 30)
    {
        vs = "maul";
        vp = "mauls";
    }
    else if (dam_percent <= 35)
    {
        vs = "decimate";
        vp = "decimates";
    }
    else if (dam_percent <= 40)
    {
        vs = "devastate";
        vp = "devastates";
    }
    else if (dam_percent <= 45)
    {
        vs = "maim";
        vp = "maims";
    }
    else if (dam_percent <= 50)
    {
        vs = "MUTILATE";
        vp = "MUTILATES";
    }
    else if (dam_percent <= 55)
    {
        vs = "DISEMBOWEL";
        vp = "DISEMBOWELS";
    }
    else if (dam_percent <= 60)
    {
        vs = "DISMEMBER";
        vp = "DISMEMBERS";
    }
    else if (dam_percent <= 65)
    {
        vs = "MASSACRE";
        vp = "MASSACRES";
    }
    else if (dam_percent <= 70)
    {
        vs = "MANGLE";
        vp = "MANGLES";
    }
    else if (dam_percent <= 75)
    {
        vs = "*** DEMOLISH ***";
        vp = "*** DEMOLISHES ***";
    }
    else if (dam_percent <= 80)
    {
        vs = "*** DEVASTATE ***";
        vp = "*** DEVASTATES ***";
    }
    else if (dam_percent <= 85)
    {
        vs = "=== OBLITERATE ===";
        vp = "=== OBLITERATES ===";
    }
    else if (dam_percent <= 90)
    {
        vs = ">>> ANNIHILATE <<<";
        vp = ">>> ANNIHILATES <<<";
    }
    else if (dam_percent <= 95)
    {
        vs = "<<< ERADICATE >>>";
        vp = "<<< ERADICATES >>>";
    }
    else
    {
        vs = "do UNSPEAKABLE things to";
        vp = "does UNSPEAKABLE things to";
    }

    punct = (dam_percent <= 45) ? '.' : '!';



	 if ( dam ==   0 ) { vs = "miss";	vp = "misses";		}
    else if ( dam <=   4 ) { vs = "scratch";	vp = "scratches";	}
    else if ( dam <=   8 ) { vs = "graze";	vp = "grazes";		}
    else if ( dam <=  12 ) { vs = "hit";	vp = "hits";		}
    else if ( dam <=  16 ) { vs = "injure";	vp = "injures";		}
    else if ( dam <=  20 ) { vs = "wound";	vp = "wounds";		}
    else if ( dam <=  24 ) { vs = "maul";       vp = "mauls";		}
    else if ( dam <=  28 ) { vs = "decimate";	vp = "decimates";	}
    else if ( dam <=  32 ) { vs = "devastate";	vp = "devastates";	}
    else if ( dam <=  36 ) { vs = "maim";	vp = "maims";		}
    else if ( dam <=  42 ) { vs = "MUTILATE";	vp = "MUTILATES";	}
    else if ( dam <=  52 ) { vs = "DISEMBOWEL";	vp = "DISEMBOWELS";	}
    else if ( dam <=  65 ) { vs = "DISMEMBER";	vp = "DISMEMBERS";	}
    else if ( dam <=  80 ) { vs = "MASSACRE";	vp = "MASSACRES";	}
    else if ( dam <=  100 ) { vs = "MANGLE";	vp = "MANGLES";		}
    else if ( dam <=  130 ) { vs = "*** DEMOLISH ***";
			     vp = "*** DEMOLISHES ***";			}
    else if ( dam <= 175 ) { vs = "*** DEVASTATE ***";
			     vp = "*** DEVASTATES ***";			}
    else if ( dam <= 250)  { vs = "=== OBLITERATE ===";
			     vp = "=== OBLITERATES ===";		}
    else if ( dam <= 325)  { vs = "==== ATOMIZE ====";
			     vp = "==== ATOMIZES ====";	}
    else if ( dam <= 400)  { vs = "<*> <*> ANNIHILATE <*> <*>";
			     vp = "<*> <*> ANNIHILATES <*> <*>";	}
    else if ( dam <= 500)  { vs = "<*>!<*> ERADICATE <*>!<*>";
			     vp = "<*>!<*> ERADICATES <*>!<*>";	}
    else if ( dam <= 650)  { vs = "<*><*><*> ELECTRONIZE <*><*><*>";
			     vp = "<*><*><*> ELECTRONIZES <*><*><*>";	}
    else if ( dam <= 800)  { vs = "(<*>)!(<*>) SKELETONIZE (<*>)!(<*>)";
			     vp = "(<*>)!(<*>) SKELETONIZES (<*>)!(<*>)";}
    else if ( dam <= 1000)  { vs = "(*)!(*)!(*) NUKE (*)!(*)!(*)";
			     vp = "(*)!(*)!(*) NUKES (*)!(*)!(*)";	}
    else if ( dam <= 1250)  { vs = "(*)!<*>!(*) TERMINATE (*)!<*>!(*)";
			     vp = "(*)!<*>!(*) TERMINATES (*)!<*>!(*)";	}
    else if ( dam <= 1500)  { vs = "<*>!(*)!<*>> TEAR UP <<*)!(*)!<*>";
			     vp = "<*>!(*)!<*>> TEARS UP <<*)!(*)!<*>";	}
    else                   { vs = "\007=<*) (*>= ! POWER HIT ! =<*) (*>=\007";
			     vp = "\007=<*) (*>= ! POWER HITS ! =<*) (*>=\007";}

    if (victim->level < 20 )	punct   = (dam <= 24) ? '.' : '!';
    else if (victim->level < 50)  punct = (dam <= 50 ) ? '.' : '!';
    else punct = (dam <= 75) ? '.' : '!';

    if (dt == TYPE_HIT)
    {
        if (ch == victim)
        {
            sprintf (buf1, "{3$n %s $melf%c{x", vp, punct);
            sprintf (buf2, "{2You %s yourself%c{x", vs, punct);
        }
        else
        {
            sprintf (buf1, "{3$n %s $N%c{x", vp, punct);
            sprintf (buf2, "{2You %s $N%c{x", vs, punct);
            sprintf (buf3, "{4$n %s you%c{x", vp, punct);
        }
    }
    else
    {
        if (dt >= 0 && dt < MAX_SKILL)
            attack = skill_table[dt].noun_damage;
        else if (dt >= TYPE_HIT && dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
            attack = attack_table[dt - TYPE_HIT].noun;
        else
        {
            bug ("Dam_message: bad dt %d.", dt);
            dt = TYPE_HIT;
            attack = attack_table[0].name;
        }

        if (immune && dt = )
        {
            if (ch == victim)
            {
                sprintf (buf1, "{3$n is unaffected by $s own %s.{x", attack);
                sprintf (buf2, "{2Luckily, you are immune to that.{x");
            }
            else
            {
                sprintf (buf1, "{3$N is unaffected by $n's %s!{x", attack);
                sprintf (buf2, "{2$N is unaffected by your %s!{x", attack);
                sprintf (buf3, "{4$n's %s is powerless against you.{x",
                         attack);
            }
        }
        else
        {
            if (ch == victim)
            {
                sprintf (buf1, "{3$n's %s %s $m%c{x", attack, vp, punct);
                sprintf (buf2, "{2Your %s %s you%c{x", attack, vp, punct);
            }
            else
            {
                sprintf (buf1, "{3$n's %s %s $N%c{x", attack, vp, punct);
                sprintf (buf2, "{2Your %s %s $N%c{x", attack, vp, punct);
                sprintf (buf3, "{4$n's %s %s you%c{x", attack, vp, punct);
            }
        }
    }

    if (ch == victim)
    {
        act (buf1, ch, NULL, NULL, TO_ROOM);
        act (buf2, ch, NULL, NULL, TO_CHAR);
    }
    else
    {
        act (buf1, ch, NULL, victim, TO_NOTVICT);
        act (buf2, ch, NULL, victim, TO_CHAR);
        act (buf3, ch, NULL, victim, TO_VICT);
    }
    return;
}
*/

void dam_message ( CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt, bool immune )
{
    char buf1[256], buf2[256], buf3[256];
    const char *vs;
    const char *vp;
    const char *attack;
    char punct;
    int dam_percent = ((100 * dam) / victim->max_hit);
    bool critical = FALSE;


    if ( ch == NULL || victim == NULL )
        return;


    if (!IS_NPC(ch) && dam > 0 && number_range(1, 7) == 5)
    critical = TRUE;

    if ( dam == 0 )
    {
        vs = "{bmiss{x";
        vp = "{bmisses{x";
    }
    else if ( dam <= 4 )
    {
        vs = "{Wscratch{x";
        vp = "{Wscratches{x";
    }
    else if ( dam <= 8 )
    {
        vs = "{Wgraze{x";
        vp = "{Wgrazes{x";
    }
    else if ( dam <= 12 )
    {
        vs = "{Whit{x";
        vp = "{Whits{x";
    }
    else if ( dam <= 16 )
    {
        vs = "{Winjure{x";
        vp = "{Winjures{x";
    }
    else if ( dam <= 25 )
    {
        vs = "{Wwound{x";
        vp = "{Wwounds{x";
    }
    else if ( dam <= 35 )
    {
        vs = "{Wmaul{x";
        vp = "{Wmauls{x";
    }
    else if ( dam <= 45 )
    {
        vs = "{Wdecimate{x";
        vp = "{Wdecimates{x";
    }
    else if ( dam <= 55 )
    {
        vs = "{Wdevastate{x";
        vp = "{Wdevastates{x";
    }
    else if ( dam <= 65 )
    {
        vs = "{ymaim{x";
        vp = "{ymaims{x";
    }
    else if ( dam <= 75 )
    {
        vs = "{DMUTILATE{x";
        vp = "{DMUTILATES{x";
    }
    else if ( dam <= 85 )
    {
        vs = "{mDISEMBOWEL{x";
        vp = "{mDISEMBOWELS{x";
    }
    else if ( dam <= 95 )
    {
        vs = "{gDISMEMBER{x";
        vp = "{gDISMEMBERS{x";
    }
    else if ( dam <= 105 )
    {
        vs = "{rMASSACRE{x";
        vp = "{rMASSACRES{x";
    }
    else if ( dam <= 115 )
    {
        vs = "{GMANGLE{x";
        vp = "{GMANGLES{x";
    }
    else if ( dam <= 125 )
    {
        vs = "{Y***{MDEMOLISH{Y***{x";
        vp = "{Y***{MDEMOLISHES{Y***{x";
    }
    else if ( dam <= 145 )
    {
        vs = "{Y***{rDEVASTATE{x{Y***{x";
        vp = "{Y***{rDEVASTATES{x{Y***{x";
    }
    else if ( dam <= 200 )
    {
        vs = "{Y==={x{MOBLITERATE{Y==={x{x";
        vp = "{Y==={x{MOBLITERATES{Y==={x{x";
    }
    else if ( dam <= 250 )
    {
        vs = "{c<{R*{c> <{R*{c> {GA{gN{GN{gI{GH{gI{GL{gA{GT{gE {c<{R*{c> <{R*{c>{x";
        vp = "{c<{R*{c> <{R*{c> {GA{gN{GN{gI{GH{gI{GL{gA{GT{gE{GS {c<{R*{c> <{R*{c>{x{x";
	// Annihilates
    }
    else if ( dam <= 350 )
    {
        vs = "{c<{w*{c>{w!{c<{w*{c> {GE{gR{GA{gD{GI{gC{GA{gT{GE{gS {c<{w*{c>{w!{c<{w*{c>{x";
        vp = "{c<{w*{c>{w!{c<{w*{c> {GE{gR{GA{gD{GI{gC{GA{gT{GE{gS {c<{w*{c>{w!{c<{w*{c>{x";
	// Eradicates
    }
    else if ( dam <= 500 )
    {
        vs = "{C={c={C={c= {GA{gT{GO{gM{GI{gZ{GE {C={c={C={c={x";
        vp = "{C={c={C={c= {GA{gT{GO{gM{GI{gZ{GE{gS {C={c={C={c={x";
	// Atomizes
    }
    else if ( dam <= 600 )
    {
        vs = "{C({c<{w*{c>{C){w!{C({c<{w*{c>{C) {GS{gK{GE{gL{GE{gT{GO{gN{GI{gZ{GE {C({c<{w*{c>{C){w!{C({c<{w*{c>{C){x";
        vp = "{C({c<{w*{c>{C){w!{C({c<{w*{c>{C) {GS{gK{GE{gL{GE{gT{GO{gN{GI{gZ{GE{gS {C({c<{w*{c>{C){w!{C({c<{w*{c>{C){x";
	// Skeletonizes
    }
    else if ( dam <= 850 )
    {
        vs = "{c<{C*{c>{w={c<{C*{c> {GE{gV{GI{gC{GE{gR{GA{gT{GE {c<{C*{c>{w={c<{C*{c>{x";
        vp = "{c<{C*{c>{w={c<{C*{c> {GE{gV{GI{gC{GE{gR{GA{gT{GE{gS {c<{C*{c>{w={c<{C*{c>{x";
	// Evicerates
    }
    else if ( dam <= 1050 )
    {
        vs = "{B({R*{B){rD{De{rs{Dt{rr{Do{r{B({R*{B){x";
        vp = "{B({R*{B){rD{De{rs{Dt{rr{Do{ry{Ds{B({R*{B){x";
    }
    else if ( dam <= 1350 )
    {
        vs = "{r^{b^{r^{yT{Re{ya{Rr{ys {RI{yn{Rt{yo{r^{b^{r^{x";
        vp = "{r^{b^{r^{yT{Re{ya{Rr{ys {RI{yn{Rt{yo{r^{b^{r^{x";
    }
    else if ( dam <= 2800 )
    {
        vs = "{B!{r*{B!{RR{Di{Rp{Ds {RO{Dp{{Re{Dn{B!{r*{B!{x";
        vp = "{B!{r*{B!{RR{Di{Rp{Ds {RO{Dp{Re{Dn{B!{r*{B!{x";
    }
    else if ( dam <= 3800 )
    {
        vs = "{R+{x{Y+{R+{x{Y+{BE{cx{Bt{ce{Br{cm{Bi{cn{Ba{ct{Be{cs{Y+{R+{x{Y+{R+{x{x";
        vp = "{R+{x{Y+{R+{X{Y+{BE{cx{Bt{ce{Br{cm{Bi{cn{Ba{ct{Be{cs{Y+{R+{x{Y+{R+{x{x";
    }
    else if ( dam <= 5800 )
    {
        vs = "{W**{R**{Ddo {WG{co{WD{cl{WI{ck{We {Ddamage to{R**{W**{x";
        vp = "{W**{R**{Ddoes {WG{co{WD{cl{WI{ck{We {Ddamage to{R**{W**{x";
    }
    else
    {
        vs = "{R!!{x{Y!!{WT{GO{WT{GA{WL{GL{WY {BF{CU{BC{CK{BS{x {WU{GP{Y!!{R!!{x{x";
        vp = "{R!!{x{Y!!{WT{GO{WT{GA{WL{GL{WY {BF{CU{BC{CK{BS{x {WU{GP{Y!!{R!!{x{x";
    }

    punct = ( dam <= 24 ) ? '.' : '!';

    if ( dt == TYPE_HIT )
    {
        if ( ch == victim )
        {
            sprintf ( buf1, "{m}%d%%}{x $n %s $melf for {G*{W%d{G*{x damage%c{x", dam_percent, vp,
                      dam, punct );
            sprintf ( buf2, "{m}%d%%}{x You %s yourself for {G*{W%d{G*{x damage%c{x", dam_percent, vs,
                      dam, punct );
        }
        else
        {
            sprintf ( buf1, "{m}%d%%}{x $n %s $N for {g>{G>{Y%d{G<{g<{x damage%c{x",dam_percent, vp,
                      dam, punct );
            sprintf ( buf2, "{m}%d%%}{x You %s $N for {g>{G>{B%d{G<{g<{x damage%c{x",dam_percent, vs,
                      dam, punct );
            sprintf ( buf3, "{m}%d%%}{x $n %s you for {w>{W>{R%d{W<{w<{x damage%c{x",dam_percent, vp,
                      dam, punct );
        }
    }
/*    else
    {
	if ( dt >= 0 && dt < MAX_SKILL )
	    attack	= skill_table[dt].noun_damage;
	else if ( dt >= TYPE_HIT
	&& dt < TYPE_HIT + MAX_DAMAGE_MESSAGE) 
	    attack	= attack_table[dt - TYPE_HIT].noun;
	else
	{
	    bug( "Dam_message: bad dt %d.", dt );
	    dt  = TYPE_HIT;
	    attack  = attack_table[0].name;
	}
*/
    else
    {
        if ( dt >= 0 && dt < MAX_SKILL )
            attack = skill_table[dt].noun_damage;
        else if ( dt >= TYPE_HIT && dt < TYPE_HIT + MAX_DAMAGE_MESSAGE )
            attack = attack_table[dt - TYPE_HIT].noun;
        else
        {
            bug ( "Dam_message: bad dt %d.", dt );
            dt = TYPE_HIT;
            attack = attack_table[0].name;
        }
        if ( immune && dt != TYPE_HIT)

        {
            if ( ch == victim )
            {
                sprintf ( buf1, "$n is unaffected by $s own %s.{x", attack );
                sprintf ( buf2, "Luckily, you are immune to that.{x" );
            }
            else
            {
                sprintf ( buf1, "$N is unaffected by $n's %s!{x", attack );
                sprintf ( buf2, "$N is unaffected by your %s!{x", attack );
                sprintf ( buf3, "$n's %s is powerless against you.{x",
                          attack );
            }
        }
        else
        {
            if ( ch == victim )
            {
                sprintf ( buf1,
                          "{m}%d%%}{x $n's %s %s $m for {g>{G>{W%d{G<{g<{x damage%c{x",
                          dam_percent,attack, vp, dam, punct );
                sprintf ( buf2,
                          "{m}%d%%}{x Your %s %s you for {w>{W>{R%d{W<{w<{x damage%c{x",
                          dam_percent,attack, vp, dam, punct );
            }
            else
            {
                sprintf ( buf1,
                          "{m}%d%%}{x $n's %s %s $N for {g>{G>{Y%d{G<{g<{x damage%c{x",
                          dam_percent,attack, vp, dam, punct );
                sprintf ( buf2,
                          "{m}%d%%}{x Your %s %s $N for {g>{G>{W%d{G<{g<{x damage%c{x",
                          dam_percent,attack, vp, dam, punct );
                sprintf ( buf3,
                          "{m}%d%%}{x $n's %s %s you for {w>{W>{R%d{W<{w<{x damage%c{x",
                          dam_percent,attack, vp, dam, punct );
            }
        }
    }

// THIS NEEDS FIXED kav_fight.c
//                if (critical)
//                critical_hit(ch, victim, dt, dam);

    if (ch == victim)
    {
        act (buf1, ch, NULL, NULL, TO_ROOM);
        act (buf2, ch, NULL, NULL, TO_CHAR);
    }
    else
    {
        act (buf1, ch, NULL, victim, TO_NOTVICT);
        act (buf2, ch, NULL, victim, TO_CHAR);
        act (buf3, ch, NULL, victim, TO_VICT);
    }
    return;

}

/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm (CHAR_DATA * ch, CHAR_DATA * victim)
{
    OBJ_DATA *obj;

    if ((obj = get_eq_char (victim, WEAR_WIELD)) == NULL)
        return;

    if (IS_OBJ_STAT (obj, ITEM_NOREMOVE))
    {
        act ("{5$S weapon won't budge!{x", ch, NULL, victim, TO_CHAR);
        act ("{5$n tries to disarm you, but your weapon won't budge!{x",
             ch, NULL, victim, TO_VICT);
        act ("{5$n tries to disarm $N, but fails.{x", ch, NULL, victim,
             TO_NOTVICT);
        return;
    }


    if ( !IS_NPC(victim) && 
	victim->level >= skill_table[gsn_grip].skill_level[victim->class])
    {
     int skill = get_skill(victim,gsn_grip);

//        skill += 
//	 (get_curr_stat(victim,STAT_STR) - get_curr_stat(ch,STAT_STR)) * 5;
	skill++;

	if ( number_percent() < skill )
	{
	 act("$N grips and prevent you to disarm $S!",ch,NULL,victim,TO_CHAR);
	 act("$n tries to disarm you, but you grip and escape!",
	    ch,NULL,victim,TO_VICT);
	 act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
	 check_improve(victim,gsn_grip,TRUE,1);
	 return;
	}
	else 	 check_improve(victim,gsn_grip,FALSE,1);
    }

    act ("{5$n DISARMS you and sends your weapon flying!{x",
         ch, NULL, victim, TO_VICT);
    act ("{5You disarm $N!{x", ch, NULL, victim, TO_CHAR);
    act ("{5$n disarms $N!{x", ch, NULL, victim, TO_NOTVICT);

    obj_from_char (obj);
    if (IS_OBJ_STAT (obj, ITEM_NODROP) || IS_OBJ_STAT (obj, ITEM_INVENTORY))
        obj_to_char (obj, victim);
    else
    {
        obj_to_room (obj, victim->in_room);
        if (IS_NPC (victim) && victim->wait == 0 && can_see_obj (victim, obj))
            get_obj (victim, obj, NULL);
    }

    return;
}

void do_berserk(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  int number_hit = 0;
  int chance;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

	if ((chance = get_skill (ch, gsn_berserk)) == 0
        || (IS_NPC (ch) && !IS_SET (ch->off_flags, OFF_BERSERK))
        || (!IS_NPC (ch)
        && ch->level < skill_table[gsn_berserk].skill_level[ch->class]))
    {
        send_to_char ("You turn red in the face, but nothing happens.\n\r", ch);
        check_improve (ch, gsn_berserk, FALSE, 2);
        return;
    }

  WAIT_STATE( ch, 8);
  if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
  {
    act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
    act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
        check_improve (ch, gsn_berserk, FALSE, 2);

    return;
  }
  act("You go {RB{WE{RR{WS{RE{WR{RK{W!{x",ch,NULL,NULL,TO_CHAR);
  act("$n goes {RB{WE{RR{WS{RE{WR{RK{W!{x",ch,NULL,NULL,TO_ROOM);
	check_improve (ch, gsn_berserk, TRUE, 2);

  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
    vch_next  = vch->next;
    if (number_hit > 4) continue;
    if (vch->in_room == NULL) continue;
    if (!IS_NPC(vch)) continue;
    if ( ch == vch ) continue;
    if ( vch->in_room == ch->in_room )
    {
      if (can_see(ch,vch))
      {
        multi_hit( ch, vch, TYPE_UNDEFINED );
        number_hit++;
      }
    }  
  }    

	if (ch->position == POS_FIGHTING)
		strip_tension (ch);


  return;
}
/*
void do_berserk (CHAR_DATA * ch, char *argument)
{
    int chance, hp_percent;

    if ((chance = get_skill (ch, gsn_berserk)) == 0
        || (IS_NPC (ch) && !IS_SET (ch->off_flags, OFF_BERSERK))
        || (!IS_NPC (ch)
            && ch->level < skill_table[gsn_berserk].skill_level[ch->class]))
    {
        send_to_char ("You turn red in the face, but nothing happens.\n\r",
                      ch);
        return;
    }

    if (IS_AFFECTED (ch, AFF_BERSERK) || is_affected (ch, gsn_berserk)
        || is_affected (ch, skill_lookup ("frenzy")))
    {
        send_to_char ("You get a little madder.\n\r", ch);
        return;
    }

    if (IS_AFFECTED (ch, AFF_CALM))
    {
        send_to_char ("You're feeling to mellow to berserk.\n\r", ch);
        return;
    }

    if (ch->mana < 50)
    {
        send_to_char ("You can't get up enough energy.\n\r", ch);
        return;
    }


    if (ch->position == POS_FIGHTING)
        chance += 10;

    hp_percent = 100 * ch->hit / ch->max_hit;
    chance += 25 - hp_percent / 2;

    if (number_percent () < chance)
    {
        AFFECT_DATA af;

        WAIT_STATE (ch, PULSE_VIOLENCE);
        ch->mana -= 50;
        ch->move /= 2;

        ch->hit += ch->level * 2;
        ch->hit = UMIN (ch->hit, ch->max_hit);

        send_to_char ("Your pulse races as you are consumed by rage!\n\r",
                      ch);
        act ("$n gets a wild look in $s eyes.", ch, NULL, NULL, TO_ROOM);
        check_improve (ch, gsn_berserk, TRUE, 2);

        af.where = TO_AFFECTS;
        af.type = gsn_berserk;
        af.level = ch->level;
        af.duration = number_fuzzy (ch->level / 8);
        af.modifier = UMAX (1, ch->level / 5);
        af.bitvector = AFF_BERSERK;

        af.location = APPLY_HITROLL;
        affect_to_char (ch, &af);

        af.location = APPLY_DAMROLL;
        affect_to_char (ch, &af);

        af.modifier = UMAX (10, 10 * (ch->level / 5));
        af.location = APPLY_AC;
        affect_to_char (ch, &af);
    }

    else
    {
        WAIT_STATE (ch, 3 * PULSE_VIOLENCE);
        ch->mana -= 25;
        ch->move /= 2;

        send_to_char ("Your pulse speeds up, but nothing happens.\n\r", ch);
        check_improve (ch, gsn_berserk, FALSE, 2);
    }
}

*/

void do_bash (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument (argument, arg);

    if ((chance = get_skill (ch, gsn_bash)) == 0
        || (IS_NPC (ch) && !IS_SET (ch->off_flags, OFF_BASH))
        || (!IS_NPC (ch)
            && ch->level < skill_table[gsn_bash].skill_level[ch->class]))
    {
        send_to_char ("Bashing? What's that?\n\r", ch);
        return;
    }

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char ("But you aren't fighting anyone!\n\r", ch);
            return;
        }
    }

    else if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

    if (victim->position < POS_FIGHTING)
    {
        act ("You'll have to let $M get back up first.", ch, NULL, victim,
             TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        send_to_char ("You try to bash your brains out, but fail.\n\r", ch);
        return;
    }

    if (is_safe (ch, victim))
        return;

    if (IS_NPC (victim) &&
        victim->fighting != NULL && !is_same_group (ch, victim->fighting))
    {
        send_to_char ("Kill stealing is not permitted.\n\r", ch);
        return;
    }

    if (IS_AFFECTED (ch, AFF_CHARM) && ch->master == victim)
    {
        act ("But $N is your friend!", ch, NULL, victim, TO_CHAR);
        return;
    }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 250;
    chance -= victim->carry_weight / 200;

    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 15;
    else
        chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat (ch, STAT_STR) / 5;
    chance -= (get_curr_stat (victim, STAT_DEX) * 4) / 3;
    chance -= GET_AC (victim, AC_BASH) / 25;
    /* speed */
    if (IS_SET (ch->off_flags, OFF_FAST) || IS_AFFECTED (ch, AFF_HASTE))
        chance += 10;
    if (IS_SET (victim->off_flags, OFF_FAST)
        || IS_AFFECTED (victim, AFF_HASTE))
        chance -= 30;

    /* level */
    chance += (ch->level - victim->level);

    if (!IS_NPC (victim) && chance < get_skill (victim, gsn_dodge))
    {                            /*
                                   act("{5$n tries to bash you, but you dodge it.{x",ch,NULL,victim,TO_VICT);
                                   act("{5$N dodges your bash, you fall flat on your face.{x",ch,NULL,victim,TO_CHAR);
                                   WAIT_STATE(ch,skill_table[gsn_bash].beats);
                                   return; */
        chance -= 3 * (get_skill (victim, gsn_dodge) - chance);
    }

    /* now the attack */
    if (number_percent () < chance)
    {

        act ("{5$n sends you sprawling with a powerful bash!{x",
             ch, NULL, victim, TO_VICT);
        act ("{5You slam into $N, and send $M flying!{x", ch, NULL, victim,
             TO_CHAR);
        act ("{5$n sends $N sprawling with a powerful bash.{x", ch, NULL,
             victim, TO_NOTVICT);
        check_improve (ch, gsn_bash, TRUE, 1);

        DAZE_STATE (victim, 3 * PULSE_VIOLENCE);
        WAIT_STATE (ch, skill_table[gsn_bash].beats);
        victim->position = POS_RESTING;
        damage (ch, victim, number_range (2, 2 + 2 * ch->size + chance / 20),
                gsn_bash, DAM_BASH, FALSE);

    }
    else
    {
        damage (ch, victim, 0, gsn_bash, DAM_BASH, FALSE);
        act ("{5You fall flat on your face!{x", ch, NULL, victim, TO_CHAR);
        act ("{5$n falls flat on $s face.{x", ch, NULL, victim, TO_NOTVICT);
        act ("{5You evade $n's bash, causing $m to fall flat on $s face.{x",
             ch, NULL, victim, TO_VICT);
        check_improve (ch, gsn_bash, FALSE, 1);
        ch->position = POS_RESTING;
        WAIT_STATE (ch, skill_table[gsn_bash].beats * 3 / 2);
    }
    check_killer (ch, victim);
}

void do_dirt (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument (argument, arg);

    if ((chance = get_skill (ch, gsn_dirt)) == 0
        || (IS_NPC (ch) && !IS_SET (ch->off_flags, OFF_KICK_DIRT))
        || (!IS_NPC (ch)
            && ch->level < skill_table[gsn_dirt].skill_level[ch->class]))
    {
        send_to_char ("You get your feet dirty.\n\r", ch);
        return;
    }

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char ("But you aren't in combat!\n\r", ch);
            return;
        }
    }

    else if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

    if (IS_AFFECTED (victim, AFF_BLIND))
    {
        act ("$E's already been blinded.", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        send_to_char ("Very funny.\n\r", ch);
        return;
    }

    if (is_safe (ch, victim))
        return;

    if (IS_NPC (victim) &&
        victim->fighting != NULL && !is_same_group (ch, victim->fighting))
    {
        send_to_char ("Kill stealing is not permitted.\n\r", ch);
        return;
    }

    if (IS_AFFECTED (ch, AFF_CHARM) && ch->master == victim)
    {
        act ("But $N is such a good friend!", ch, NULL, victim, TO_CHAR);
        return;
    }

    /* modifiers */

    /* dexterity */
    chance += get_curr_stat (ch, STAT_DEX);
    chance -= 2 * get_curr_stat (victim, STAT_DEX);

    /* speed  */
    if (IS_SET (ch->off_flags, OFF_FAST) || IS_AFFECTED (ch, AFF_HASTE))
        chance += 10;
    if (IS_SET (victim->off_flags, OFF_FAST)
        || IS_AFFECTED (victim, AFF_HASTE))
        chance -= 25;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* sloppy hack to prevent false zeroes */
    if (chance % 5 == 0)
        chance += 1;

    /* terrain */

    switch (ch->in_room->sector_type)
    {
        case (SECT_INSIDE):
            chance -= 20;
            break;
        case (SECT_CITY):
            chance -= 10;
            break;
        case (SECT_FIELD):
            chance += 5;
            break;
        case (SECT_FOREST):
            break;
        case (SECT_HILLS):
            break;
        case (SECT_MOUNTAIN):
            chance -= 10;
            break;
        case (SECT_WATER_SWIM):
            chance = 0;
            break;
        case (SECT_WATER_NOSWIM):
            chance = 0;
            break;
        case (SECT_AIR):
            chance = 0;
            break;
        case (SECT_DESERT):
            chance += 10;
            break;
    }

    if (chance == 0)
    {
        send_to_char ("There isn't any dirt to kick.\n\r", ch);
        return;
    }

    /* now the attack */
    if (number_percent () < chance)
    {
        AFFECT_DATA af;
        act ("{5$n is blinded by the dirt in $s eyes!{x", victim, NULL, NULL,
             TO_ROOM);
        act ("{5$n kicks dirt in your eyes!{x", ch, NULL, victim, TO_VICT);
        damage (ch, victim, number_range (2, 5), gsn_dirt, DAM_NONE, FALSE);
        send_to_char ("{5You can't see a thing!{x\n\r", victim);
        check_improve (ch, gsn_dirt, TRUE, 2);
        WAIT_STATE (ch, skill_table[gsn_dirt].beats);

        af.where = TO_AFFECTS;
        af.type = gsn_dirt;
        af.level = ch->level;
        af.duration = 0;
        af.location = APPLY_HITROLL;
        af.modifier = -4;
        af.bitvector = AFF_BLIND;

        affect_to_char (victim, &af);
    }
    else
    {
        damage (ch, victim, 0, gsn_dirt, DAM_NONE, TRUE);
        check_improve (ch, gsn_dirt, FALSE, 2);
        WAIT_STATE (ch, skill_table[gsn_dirt].beats);
    }
    check_killer (ch, victim);
}

void do_trip (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument (argument, arg);

    if ((chance = get_skill (ch, gsn_trip)) == 0
        || (IS_NPC (ch) && !IS_SET (ch->off_flags, OFF_TRIP))
        || (!IS_NPC (ch)
            && ch->level < skill_table[gsn_trip].skill_level[ch->class]))
    {
        send_to_char ("Tripping?  What's that?\n\r", ch);
        return;
    }


    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char ("But you aren't fighting anyone!\n\r", ch);
            return;
        }
    }

    else if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

    if (is_safe (ch, victim))
        return;

    if (IS_NPC (victim) &&
        victim->fighting != NULL && !is_same_group (ch, victim->fighting))
    {
        send_to_char ("Kill stealing is not permitted.\n\r", ch);
        return;
    }

    if (IS_AFFECTED (victim, AFF_FLYING))
    {
        act ("$S feet aren't on the ground.", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (victim->position < POS_FIGHTING)
    {
        act ("$N is already down.", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        send_to_char ("{5You fall flat on your face!{x\n\r", ch);
        WAIT_STATE (ch, 2 * skill_table[gsn_trip].beats);
        act ("{5$n trips over $s own feet!{x", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (IS_AFFECTED (ch, AFF_CHARM) && ch->master == victim)
    {
        act ("$N is your beloved master.", ch, NULL, victim, TO_CHAR);
        return;
    }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;    /* bigger = harder to trip */

    /* dex */
    chance += get_curr_stat (ch, STAT_DEX);
    chance -= get_curr_stat (victim, STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET (ch->off_flags, OFF_FAST) || IS_AFFECTED (ch, AFF_HASTE))
        chance += 10;
    if (IS_SET (victim->off_flags, OFF_FAST)
        || IS_AFFECTED (victim, AFF_HASTE))
        chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;


    /* now the attack */
    if (number_percent () < chance)
    {
        act ("{5$n trips you and you go down!{x", ch, NULL, victim, TO_VICT);
        act ("{5You trip $N and $N goes down!{x", ch, NULL, victim, TO_CHAR);
        act ("{5$n trips $N, sending $M to the ground.{x", ch, NULL, victim,
             TO_NOTVICT);
        check_improve (ch, gsn_trip, TRUE, 1);

        DAZE_STATE (victim, 2 * PULSE_VIOLENCE);
        WAIT_STATE (ch, skill_table[gsn_trip].beats);
        victim->position = POS_RESTING;
        damage (ch, victim, number_range (2, 2 + 2 * victim->size), gsn_trip,
                DAM_BASH, TRUE);
    }
    else
    {
        damage (ch, victim, 0, gsn_trip, DAM_BASH, TRUE);
        WAIT_STATE (ch, skill_table[gsn_trip].beats * 2 / 3);
        check_improve (ch, gsn_trip, FALSE, 1);
    }
    check_killer (ch, victim);
}



void do_kill (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument (argument, arg);

    if (arg[0] == '\0')
    {
        send_to_char ("Kill whom?\n\r", ch);
        return;
    }

    if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }
/*  Allow player killing
    if ( !IS_NPC(victim) )
    {
        if ( !IS_SET(victim->act, PLR_KILLER)
        &&   !IS_SET(victim->act, PLR_THIEF) )
        {
            send_to_char( "You must MURDER a player.\n\r", ch );
            return;
        }
    }
*/
    if (victim == ch)
    {
        send_to_char ("You hit yourself.  Ouch!\n\r", ch);
        multi_hit (ch, ch, TYPE_UNDEFINED);
        return;
    }

    if (is_safe (ch, victim))
        return;

    if (victim->fighting != NULL && !is_same_group (ch, victim->fighting))
    {
        send_to_char ("Kill stealing is not permitted.\n\r", ch);
        return;
    }

    if (IS_AFFECTED (ch, AFF_CHARM) && ch->master == victim)
    {
        act ("$N is your beloved master.", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (ch->position == POS_FIGHTING)
    {
        send_to_char ("You do the best you can!\n\r", ch);
        return;
    }

    WAIT_STATE (ch, 1 * PULSE_VIOLENCE);
    check_killer (ch, victim);
    multi_hit (ch, victim, TYPE_UNDEFINED);
    return;
}



void do_murde (CHAR_DATA * ch, char *argument)
{
    send_to_char ("If you want to MURDER, spell it out.\n\r", ch);
    return;
}



void do_murder (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument (argument, arg);

    if (arg[0] == '\0')
    {
        send_to_char ("Murder whom?\n\r", ch);
        return;
    }

    if (IS_AFFECTED (ch, AFF_CHARM)
        || (IS_NPC (ch) && IS_SET (ch->act, ACT_PET)))
        return;

    if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

    if (victim == ch)
    {
        send_to_char ("Suicide is a mortal sin.\n\r", ch);
        return;
    }

    if (is_safe (ch, victim))
        return;

    if (IS_NPC (victim) &&
        victim->fighting != NULL && !is_same_group (ch, victim->fighting))
    {
        send_to_char ("Kill stealing is not permitted.\n\r", ch);
        return;
    }

    if (IS_AFFECTED (ch, AFF_CHARM) && ch->master == victim)
    {
        act ("$N is your beloved master.", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (ch->position == POS_FIGHTING)
    {
        send_to_char ("You do the best you can!\n\r", ch);
        return;
    }

    WAIT_STATE (ch, 1 * PULSE_VIOLENCE);
    if (IS_NPC (ch))
        sprintf (buf, "Help! I am being attacked by %s!", ch->short_descr);
    else
        sprintf (buf, "Help!  I am being attacked by %s!", ch->name);
    do_function (victim, &do_yell, buf);
    check_killer (ch, victim);
    multi_hit (ch, victim, TYPE_UNDEFINED);
    return;
}



void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
//    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );


    if (!IS_NPC(ch) 
    && ch->level < skill_table[gsn_backstab].skill_level[ch->class]
    && ch->level2 < skill_table[gsn_backstab].skill_level[ch->class2]
    && ch->level3 < skill_table[gsn_backstab].skill_level[ch->class3]
    && ch->level4 < skill_table[gsn_backstab].skill_level[ch->class4])
      {
	send_to_char("You don't know how to backstab.\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Backstab whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if ( (IS_NPC(ch)) && (!(IS_NPC(victim))) )
	return;
	

    if ( victim == ch )
    {
	send_to_char( "How can you sneak up on yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
	send_to_char( "You need to wield a weapon to backstab.\n\r", ch );
	return;
    }

 /*   if (attack_table[obj->value[3]].damage != DAM_PIERCE)
      {
	send_to_char("You need to wield a piercing weapon to backstab.\n\r",
		     ch);
	return;
      }
*/
    if ( victim->fighting != NULL )
    {
	send_to_char( "You can't backstab a fighting person.\n\r", ch );
	return;
    }


    WAIT_STATE( ch, skill_table[gsn_backstab].beats );

    if ( victim->hit < (0.7 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "$N is hurt and suspicious ... you couldn't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

/*    if ( current_time-victim->last_fight_time<300 && IS_AWAKE(victim) )
    {
	act( "$N is suspicious ... you couldn't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;       
    }
*/

    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < get_skill(ch,gsn_backstab) )
    {
	check_improve(ch,gsn_backstab,TRUE,1);

    if (!IS_NPC(ch) && number_percent( ) < get_skill(ch,gsn_dual_backstab))
	  {
	    check_improve(ch,gsn_dual_backstab,TRUE,1);
	    one_hit(ch,victim,gsn_backstab, FALSE);
      one_hit(ch,victim,gsn_backstab, FALSE);
      one_hit(ch,victim,gsn_backstab, FALSE);
	    one_hit(ch,victim,gsn_dual_backstab, FALSE);
      one_hit(ch,victim,gsn_dual_backstab, FALSE);
      one_hit(ch,victim,gsn_dual_backstab, FALSE);
	  }
	else
	  {
	    check_improve(ch,gsn_dual_backstab,FALSE,1);
	    multi_hit( ch, victim, gsn_backstab );
	  }
    }
    else
    {
	check_improve(ch,gsn_backstab,FALSE,1);
	damage( ch, victim, 0, gsn_backstab,DAM_NONE, TRUE );
    }
    return;
}


void do_flee (CHAR_DATA * ch, char *argument)
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    int attempt;

    if ((victim = ch->fighting) == NULL)
    {
        if (ch->position == POS_FIGHTING)
            ch->position = POS_STANDING;
        send_to_char ("You aren't fighting anyone.\n\r", ch);
        return;
    }

  if (IS_AFFECTED2(ch, AFF2_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    return;
  }



    was_in = ch->in_room;
    for (attempt = 0; attempt < 6; attempt++)
    {
        EXIT_DATA *pexit;
        int door;

        door = number_door ();
        if ((pexit = was_in->exit[door]) == 0
            || pexit->u1.to_room == NULL
            || IS_SET (pexit->exit_info, EX_CLOSED)
            || number_range (0, ch->daze) != 0 || (IS_NPC (ch)
                                                   && IS_SET (pexit->u1.
                                                              to_room->
                                                              room_flags,
                                                              ROOM_NO_MOB)))
            continue;

        move_char (ch, door, FALSE);
        if ((now_in = ch->in_room) == was_in)
            continue;

        ch->in_room = was_in;
        act ("$n has fled!", ch, NULL, NULL, TO_ROOM);
        ch->in_room = now_in;

        if (!IS_NPC (ch))
        {
            send_to_char ("You flee from combat!\n\r", ch);
            if ((ch->class == 2) && (number_percent () < 3 * (ch->level / 2)))
                send_to_char ("You snuck away safely.\n\r", ch);
//            else
//            {
//                send_to_char ("You lost 10 exp.\n\r", ch);
//                gain_exp (ch, -10);
//            }
        }

        stop_fighting (ch, TRUE);
        return;
    }

    send_to_char ("PANIC! You couldn't escape!\n\r", ch);
    return;
}



void do_rescue (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument (argument, arg);
    if (arg[0] == '\0')
    {
        send_to_char ("Rescue whom?\n\r", ch);
        return;
    }

    if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

    if (victim == ch)
    {
        send_to_char ("What about fleeing instead?\n\r", ch);
        return;
    }

    if (!IS_NPC (ch) && IS_NPC (victim))
    {
        send_to_char ("Doesn't need your help!\n\r", ch);
        return;
    }

    if (ch->fighting == victim)
    {
        send_to_char ("Too late.\n\r", ch);
        return;
    }

    if ((fch = victim->fighting) == NULL)
    {
        send_to_char ("That person is not fighting right now.\n\r", ch);
        return;
    }

    if (IS_NPC (fch) && !is_same_group (ch, victim))
    {
        send_to_char ("Kill stealing is not permitted.\n\r", ch);
        return;
    }

    WAIT_STATE (ch, skill_table[gsn_rescue].beats);
    if (number_percent () > get_skill (ch, gsn_rescue))
    {
        send_to_char ("You fail the rescue.\n\r", ch);
        check_improve (ch, gsn_rescue, FALSE, 1);
        return;
    }

    act ("{5You rescue $N!{x", ch, NULL, victim, TO_CHAR);
    act ("{5$n rescues you!{x", ch, NULL, victim, TO_VICT);
    act ("{5$n rescues $N!{x", ch, NULL, victim, TO_NOTVICT);
    check_improve (ch, gsn_rescue, TRUE, 1);

    stop_fighting (fch, FALSE);
    stop_fighting (victim, FALSE);

    check_killer (ch, fch);
    set_fighting (ch, fch);
    set_fighting (fch, ch);
    return;
}



void do_kick (CHAR_DATA * ch, char *argument)
{
    CHAR_DATA *victim;

    if (!IS_NPC (ch)
        && ch->level < skill_table[gsn_kick].skill_level[ch->class])
    {
        send_to_char ("You better leave the martial arts to fighters.\n\r",
                      ch);
        return;
    }

    if (IS_NPC (ch) && !IS_SET (ch->off_flags, OFF_KICK))
        return;

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char ("You aren't fighting anyone.\n\r", ch);
        return;
    }

    WAIT_STATE (ch, skill_table[gsn_kick].beats);
    if (get_skill (ch, gsn_kick) > number_percent ())
    {
        damage (ch, victim, number_range (1, ch->level), gsn_kick, DAM_BASH,
                TRUE);
        check_improve (ch, gsn_kick, TRUE, 1);
    }
    else
    {
        damage (ch, victim, 0, gsn_kick, DAM_BASH, TRUE);
        check_improve (ch, gsn_kick, FALSE, 1);
    }
    check_killer (ch, victim);
    return;
}




void do_disarm (CHAR_DATA * ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance, hth, ch_weapon, vict_weapon, ch_vict_weapon;

    hth = 0;

    if ((chance = get_skill (ch, gsn_disarm)) == 0)
    {
        send_to_char ("You don't know how to disarm opponents.\n\r", ch);
        return;
    }

    if (get_eq_char (ch, WEAR_WIELD) == NULL
        && ((hth = get_skill (ch, gsn_hand_to_hand)) == 0
            || (IS_NPC (ch) && !IS_SET (ch->off_flags, OFF_DISARM))))
    {
        send_to_char ("You must wield a weapon to disarm.\n\r", ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char ("You aren't fighting anyone.\n\r", ch);
        return;
    }

    if ((obj = get_eq_char (victim, WEAR_WIELD)) == NULL)
    {
        send_to_char ("Your opponent is not wielding a weapon.\n\r", ch);
        return;
    }

    /* find weapon skills */
    ch_weapon = get_weapon_skill (ch, get_weapon_sn (ch));
    vict_weapon = get_weapon_skill (victim, get_weapon_sn (victim));
    ch_vict_weapon = get_weapon_skill (ch, get_weapon_sn (victim));

    /* modifiers */

    /* skill */
    if (get_eq_char (ch, WEAR_WIELD) == NULL)
        chance = chance * hth / 150;
    else
        chance = chance * ch_weapon / 100;

    chance += (ch_vict_weapon / 2 - vict_weapon) / 2;

    /* dex vs. strength */
    chance += get_curr_stat (ch, STAT_DEX);
    chance -= 2 * (get_curr_stat (victim, STAT_STR) / 5);

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* and now the attack */
    if (number_percent () < chance)
    {
        WAIT_STATE (ch, skill_table[gsn_disarm].beats);
        disarm (ch, victim);
        check_improve (ch, gsn_disarm, TRUE, 1);
    }
    else
    {
        WAIT_STATE (ch, skill_table[gsn_disarm].beats);
        act ("{5You fail to disarm $N.{x", ch, NULL, victim, TO_CHAR);
        act ("{5$n tries to disarm you, but fails.{x", ch, NULL, victim,
             TO_VICT);
        act ("{5$n tries to disarm $N, but fails.{x", ch, NULL, victim,
             TO_NOTVICT);
        check_improve (ch, gsn_disarm, FALSE, 1);
    }
    check_killer (ch, victim);
    return;
}

void do_surrender (CHAR_DATA * ch, char *argument)
{
    CHAR_DATA *mob;
    if ((mob = ch->fighting) == NULL)
    {
        send_to_char ("But you're not fighting!\n\r", ch);
        return;
    }
    act ("You surrender to $N!", ch, NULL, mob, TO_CHAR);
    act ("$n surrenders to you!", ch, NULL, mob, TO_VICT);
    act ("$n tries to surrender to $N!", ch, NULL, mob, TO_NOTVICT);
    stop_fighting (ch, TRUE);

    if (!IS_NPC (ch) && IS_NPC (mob)
        && (!HAS_TRIGGER (mob, TRIG_SURR)
            || !mp_percent_trigger (mob, ch, NULL, NULL, TRIG_SURR)))
    {
        act ("$N seems to ignore your cowardly act!", ch, NULL, mob, TO_CHAR);
        multi_hit (mob, ch, TYPE_UNDEFINED);
    }
}

void do_sla (CHAR_DATA * ch, char *argument)
{
    send_to_char ("If you want to SLAY, spell it out.\n\r", ch);
    return;
}



void do_slay (CHAR_DATA * ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument (argument, arg);
    if (arg[0] == '\0')
    {
        send_to_char ("Slay whom?\n\r", ch);
        return;
    }

    if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

    if (ch == victim)
    {
        send_to_char ("Suicide is a mortal sin.\n\r", ch);
        return;
    }

    if (!IS_NPC (victim) && victim->level >= get_trust (ch))
    {
        send_to_char ("You failed.\n\r", ch);
        return;
    }

    act ("{1You slay $M in cold blood!{x", ch, NULL, victim, TO_CHAR);
    act ("{1$n slays you in cold blood!{x", ch, NULL, victim, TO_VICT);
    act ("{1$n slays $N in cold blood!{x", ch, NULL, victim, TO_NOTVICT);
    raw_kill (victim);
    return;
}


/*
 * Special defensive skill for polearms.  Due to their extended range,
 * polearms are able to keep your opponent from closing on on you.
 */
bool pole_special (CHAR_DATA * ch, CHAR_DATA * victim)
{
    int chance;

    if (!IS_AWAKE (victim))
        return FALSE;

	if (get_weapon_sn (victim) != gsn_polearm)
		return FALSE;

    chance = get_skill (victim, gsn_polearm) / 3;

    if (!can_see (victim, ch))
        chance /= 2;

	/*
	 * It's harder to keep another polearm at bay.
	 */
    if (get_weapon_sn (ch) != gsn_polearm)
        chance -= chance / 3;

    if (number_percent () >= chance + victim->level - ch->level)
        return FALSE;

    act ("You manage to keep $n out of range.",
			ch, NULL, victim, TO_VICT);
    act ("$N's weapon keeps you out of range.",
			ch, NULL, victim, TO_CHAR);

    check_improve (victim, gsn_polearm, TRUE, 4);
    return TRUE;
}



/*
 * Critical hit damage modifier for axes.
 */
int axe_special (CHAR_DATA * ch, CHAR_DATA * victim, int dam)
{
    int chance;

	/*
	 * Make sure we have an axe.
	 */
	if (get_weapon_sn (ch) != gsn_axe)
		return dam;

	/*
	 * Base is 1/5th our axe skill.  Bonus if victim can't see us, penalty
	 * if we can't see them.
	 */
	chance = get_skill (ch, gsn_axe) / 5;

    if (!can_see (victim, ch))
        chance *= 1.5;

    if (!can_see (ch, victim))
        chance /= 3;

    if (number_percent () >= chance)
        return dam;

	/*
	 * We got a critical hit!
	 */
	dam += dam * (get_skill (ch, gsn_axe) / 2) / 100;
    check_improve (victim, gsn_axe, TRUE, 4);
    return dam;
}
