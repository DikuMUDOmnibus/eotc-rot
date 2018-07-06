/***************************************************************************
 * This code may be used freely within any non-commercial MUD, all I ask   *
 * is that these comments remain in tact and that you give me any feedback *
 * or bug reports you come up with.  Credit in a helpfile might be nice,   *
 * too.                             -- Midboss (eclipsing.souls@gmail.com) *
 ***************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include "merc.h"

//Imported from fight.c.
void one_hit args ((CHAR_DATA * ch, CHAR_DATA * victim, int dt, bool secondary));

/*
 * Strips the effects of tension.
 */
void strip_tension (CHAR_DATA * ch)
{
	if (ch->tension == 0)
		return;

	act ("Your tension level returns to normal.", ch, NULL, NULL, TO_CHAR);
	act ("$n's tension level returns to normal.", ch, NULL, NULL, TO_ROOM);
	affect_strip (ch, gsn_tension);
	ch->tension = 0;
}

void do_tension (CHAR_DATA * ch, char * argument)
{
	/*
	 * Make sure they're fighting.
	 */
	if (ch->position != POS_FIGHTING)
	{
		send_to_char ("But you're not fighting!\n\r", ch);
		return;
	}

	/* 
	 * Start psyching up.
	 */
	if (ch->tension < 1)
	{
		act ("You begin to psyche yourself up!", ch, NULL, NULL, TO_CHAR);
		act ("$n begins to psyche $mself up!", ch, NULL, NULL, TO_ROOM);
		ch->tension = 1;
	}
	/*
	 * Unleash the tension!
	 */
	else
	{
		//Grab the victim.
		CHAR_DATA * vch = ch->fighting;

		//They're not fighting anymore?
		if (ch->fighting == NULL)
		{
			send_to_char ("You try to unleash your tension, but can't find"
						  " any poor sap to do it on.\n\r", ch);
			strip_tension (ch);
			return;
		}

		//Unleash ch's Squirrely Wr-- er, Tension.
		act ("You unleash all your tension on $N!", ch, NULL, vch, TO_CHAR);
		act ("$n unleashes all $s tension on $N!", ch, NULL, vch, TO_NOTVICT);
		act ("$n unleashes all $s tension on you!", ch, NULL, vch, TO_VICT);
		tension_hit (ch, vch);
		strip_tension (ch);
	}
	return;
}

/*
 * Group of tension attacks, hacked off of multi_hit.
 * Second and third attack are much easier with tension.
 * Also, tension attacks are unaffected by haste or slow.
 */
void tension_hit (CHAR_DATA * ch, CHAR_DATA * victim)
{
    int chance, mod;

	switch (ch->tension)
	{
		//They have no real tension, cancel.
		default:
			act ("You stop psyching yourself up.", ch, NULL, NULL, TO_CHAR);
			act ("$n stops psyching $mself up.", ch, NULL, NULL, TO_ROOM);
			ch->tension = 0;
			return;
		case 2:
			mod = 50;
		case 3:
			mod = 25;
		case 4:
			mod = 5;
		case 5:
			mod = 0;
	}

	//First hit.
    one_hit (ch, victim, TYPE_UNDEFINED, FALSE);

	//Make sure the enemy is still there.
    if (ch->fighting != victim)
        return;

	//Second attack check.
    chance = UMAX (0, get_skill (ch, gsn_second_attack) / 2 + mod);

    if (number_percent () < chance)
    {
        one_hit (ch, victim, TYPE_UNDEFINED, FALSE);

        if (ch->fighting != victim)
            return;
    }

	//Third attack check.
    chance = UMAX (0, get_skill (ch, gsn_third_attack) / 4 + mod);

    if (number_percent () < chance)
    {
        one_hit (ch, victim, TYPE_UNDEFINED, FALSE);

        if (ch->fighting != victim)
            return;
    }
    return;
}
