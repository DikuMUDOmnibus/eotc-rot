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
 **************************************************************************/

/***************************************************************************
 *   ROM 2.4 is copyright 1993-1998 Russ Taylor                            *
 *   ROM has been brought to you by the ROM consortium                     *
 *       Russ Taylor (rtaylor@hypercube.org)                               *
 *       Gabrielle Taylor (gtaylor@hypercube.org)                          *
 *       Brian Moore (zump@rom.org)                                        *
 *   By using this code, you have agreed to follow the terms of the        *
 *   ROM license, in the file Rom24/doc/rom.license                        *
 **************************************************************************/

/*   QuickMUD - The Lazy Man's ROM - $Id: act_move.c,v 1.2 2000/12/01 10:48:33 ring0 Exp $ */

#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include "merc.h"
#include "interp.h"
#include "clan.h"

//char *const dir_name[] = {
//    "north", "east", "south", "west", "up", "down"
//};
char *const dir_name[] =
{
    "north", "east", "south", "west", "up", "down",
    "northwest", "northeast", "southwest", "southeast"
};

//const sh_int rev_dir[] = {
//    2, 3, 0, 1, 5, 4
//};

char *  const   dir_rev     []      =
{
    "the south", "the west", "the north", "the east", "below", "above",
    "the southeast", "the southwest", "the northeast", "the northwest"
 };

//const sh_int movement_loss[SECT_MAX] = {
//    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
//};

const sh_int rev_dir[] =
{
    2, 3, 0, 1, 5, 4, 6, 7, 8, 9
};

const sh_int movement_loss[SECT_MAX] =
{
     1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
 };



/*
 * Local functions.
 */
int find_door args ((CHAR_DATA * ch, char *arg));
bool has_key args ((CHAR_DATA * ch, int key));

int focus_level args ((long total));
long focus_left args ((long total));

int focus_dam args ((CHAR_DATA *ch));
int focus_str args ((CHAR_DATA *ch));
int focus_ac args ((CHAR_DATA *ch));
int focus_move args ((CHAR_DATA *ch));
int focus_hit args ((CHAR_DATA *ch));
int focus_dex args ((CHAR_DATA *ch));
int focus_hp args ((CHAR_DATA *ch));
int focus_con args ((CHAR_DATA *ch));
int focus_sorc args ((CHAR_DATA *ch));
int focus_sorcbonus args ((CHAR_DATA *ch));
int focus_mana args ((CHAR_DATA *ch));
int focus_int args ((CHAR_DATA *ch));
int focus_save args ((CHAR_DATA *ch));
int focus_wis args ((CHAR_DATA *ch));

void focus_up args ((CHAR_DATA *ch));
void check_focus args ((CHAR_DATA *ch));


int focus_level( long total )
{
        int level=0;
        long i=0;

        while (total>(25000+i))
        {
                level++;
                total -= (25000+i);
                i += (((25000+i)*3)/20);
        }

        return level;
}
long focus_left( long total )
{
        int level=0;
        long i=0;

        while (total>(25000+i))
        {
                level++;
                total -= (25000+i);
                i += (((25000+i)*3)/20);
        }

        return ((25000+i)-total);
}
int focus_dam ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
	return (focus_level(ch->pcdata->focus[COMBAT_POWER])*5/2);
}

int focus_str ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
	return (focus_level(ch->pcdata->focus[COMBAT_POWER])/2);
}

int focus_ac ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
	return (focus_level(ch->pcdata->focus[COMBAT_DEFENSE])*-4);
}

int focus_move ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
//      return (focus_level(ch->pcdata->focus[COMBAT_DEFENSE])*40);
        return 30;
}

int focus_hit ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
	return 0;
	return (focus_level(ch->pcdata->focus[COMBAT_AGILITY])*5/2);
}

int focus_dex ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
	return (focus_level(ch->pcdata->focus[COMBAT_AGILITY])/2);
}

int focus_hp ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
//        return (focus_level(ch->pcdata->focus[COMBAT_TOUGHNESS])*40);
        return 40;
}

int focus_con ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
	return (focus_level(ch->pcdata->focus[COMBAT_TOUGHNESS])/2);
}

int focus_sorc ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
         return 0;
	return (focus_level(ch->pcdata->focus[MAGIC_POWER])*3); //was *2
}

int focus_sorcbonus ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
	 return 0;
	return (focus_level(ch->pcdata->focus[MAGIC_POWER])*7);  //was *5
}

int focus_mana ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
//        return (focus_level(ch->pcdata->focus[MAGIC_ABILITY])*40);
        return 30;
}

int focus_int ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
	return (focus_level(ch->pcdata->focus[MAGIC_ABILITY])/2);
}

int focus_save ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
	return (focus_level(ch->pcdata->focus[MAGIC_DEFENSE]));
}

int focus_wis ( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return 0;
	return (focus_level(ch->pcdata->focus[MAGIC_DEFENSE])/2);
}

void focus_up ( CHAR_DATA *ch )
{
   int curr=0;

   if (IS_NPC(ch))
	return;

   curr = ch->pcdata->focus[CURRENT_FOCUS];

   send_to_char("{YYou have advanced in your focus!{x\n\r",ch);

   if (curr == COMBAT_DEFENSE)
   {
	ch->pcdata->perm_move += focus_move(ch);
	ch->max_move += focus_move(ch);
        ch->move += focus_move(ch);
   }

   if (curr == COMBAT_TOUGHNESS)
   {
        ch->pcdata->perm_hit += focus_hp(ch);
        ch->max_hit += focus_hp(ch);
        ch->hit += focus_hp(ch);
   }

   if (curr == MAGIC_ABILITY)
   {
	ch->pcdata->perm_mana += focus_mana(ch);
	ch->max_mana += focus_mana(ch);
        ch->mana += focus_mana(ch);
   }

   if (curr == MAGIC_DEFENSE)
   {
        ch->saving_throw -= 1;
   }

   ch-> hit	=	ch->max_hit;
   ch-> move	=	ch->max_move;
   ch-> mana	=	ch->max_mana;
   update_pos(ch);

}

/* Focus Bug Fixr - Skyn/Jorm */
void check_focus ( CHAR_DATA *ch )
{
 int ctlevel, malevel, cdlevel;
 int i, hploss=0, maloss=0, moloss=0;

 ctlevel = focus_level(ch->pcdata->focus[COMBAT_TOUGHNESS]);
 malevel = focus_level(ch->pcdata->focus[MAGIC_ABILITY]);
 cdlevel = focus_level(ch->pcdata->focus[COMBAT_DEFENSE]);

 // Reset Hitpoints
    // First Remove
    for( i = ctlevel; i > 0; i--)
    {
        ch->pcdata->perm_hit -= ctlevel*40;
        ch->max_hit -= ctlevel*40;
        ch->hit -= ctlevel*40;
        hploss += ctlevel*40;
    }
    printf_to_char(ch,"Total Hitpoint Loss: %d\n\r",hploss);
    hploss = 0;
    // Then Add
    for( i = 0; i < ctlevel; i++)
    {
     ch->pcdata->perm_hit += focus_hp(ch);
     ch->max_hit += focus_hp(ch);
     ch->hit += focus_hp(ch);
     hploss += focus_hp(ch);
    }
 printf_to_char(ch,"Total Hitpoint Gain: %d\n\r",hploss);
 send_to_char("Combat Toughness Reset. Hitpoints Adjusted.\n\r",ch);

 // Reset Mana
    // First Remove
    for( i = malevel; i > 0; i--)
    {
        ch->pcdata->perm_mana -= malevel*40;
        ch->max_mana -= malevel*40;
        ch->mana -= malevel*40;
        maloss += malevel*40;
    }
    printf_to_char(ch,"Total Mana Loss: %d\n\r",maloss);
    maloss = 0;
    // Then Add
    for( i = 0; i < malevel; i++)
    {
	ch->pcdata->perm_mana += focus_mana(ch);
	ch->max_mana += focus_mana(ch);
        ch->mana += focus_mana(ch);
        maloss += focus_mana(ch);
    }
    printf_to_char(ch,"Total Mana Gain: %d\n\r",maloss);
    send_to_char("Magic Ability. Mana Adjusted.\n\r",ch);

 // Reset Moves
    // First Remove
    for( i = cdlevel; i > 0; i--)
    {
        ch->pcdata->perm_move -= cdlevel*40;
        ch->max_move -= cdlevel*40;
        ch->move -= cdlevel*40;
        moloss += cdlevel*40;
    }
    printf_to_char(ch,"Total Move Loss: %d\n\r",moloss);
    moloss = 0;
    // Then Add
    for( i = 0; i < cdlevel; i++)
    {
	ch->pcdata->perm_move += focus_move(ch);
	ch->max_move += focus_move(ch);
        ch->move += focus_move(ch);
        moloss += focus_move(ch);
    }
    printf_to_char(ch,"Total Move Gain: %d\n\r",moloss);
    send_to_char("Combat Defense Reset. Moves Adjusted.\n\r",ch);
//    SET_BIT(ch->plyr,PLAYER_STAT_FOCUS_FIXED);
    return;
}


void move_char (CHAR_DATA * ch, int door, bool follow)
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;

    if (door < 0 || door > 5)
    {
        bug ("Do_move: bad door %d.", door);
        return;
    }

    /*
     * Exit trigger, if activated, bail out. Only PCs are triggered.
     */
    if (!IS_NPC (ch) && mp_exit_trigger (ch, door))
        return;

    in_room = ch->in_room;
    if ((pexit = in_room->exit[door]) == NULL
        || (to_room = pexit->u1.to_room) == NULL
        || !can_see_room (ch, pexit->u1.to_room))
    {
        send_to_char ("Alas, you cannot go that way.\n\r", ch);
        return;
    }

    if (IS_SET (pexit->exit_info, EX_CLOSED)
        && (!IS_AFFECTED (ch, AFF_PASS_DOOR)
            || IS_SET (pexit->exit_info, EX_NOPASS))
        && !IS_TRUSTED (ch, ANGEL))
    {
        act ("The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR);
        return;
    }

    if (IS_AFFECTED (ch, AFF_CHARM)
        && ch->master != NULL && in_room == ch->master->in_room)
    {
        send_to_char ("What?  And leave your beloved master?\n\r", ch);
        return;
    }

    if (!is_room_owner (ch, to_room) && room_is_private (to_room))
    {
        send_to_char ("That room is private right now.\n\r", ch);
        return;
    }

    if (!IS_NPC (ch))
    {
        int iClass, iGuild;
        int move;

        for (iClass = 0; iClass < MAX_CLASS; iClass++)
        {
            for (iGuild = 0; iGuild < MAX_GUILD; iGuild++)
            {
                if (iClass != ch->class
                    && to_room->vnum == class_table[iClass].guild[iGuild])
                {
                    send_to_char ("You aren't allowed in there.\n\r", ch);
                    return;
                }
            }
        }

        if (in_room->sector_type == SECT_AIR
            || to_room->sector_type == SECT_AIR)
        {
            if (!IS_AFFECTED (ch, AFF_FLYING) && !IS_IMMORTAL (ch))
            {
                send_to_char ("You can't fly.\n\r", ch);
                return;
            }
        }

        if ((in_room->sector_type == SECT_WATER_NOSWIM
             || to_room->sector_type == SECT_WATER_NOSWIM)
            && !IS_AFFECTED (ch, AFF_FLYING))
        {
            OBJ_DATA *obj;
            bool found;

            /*
             * Look for a boat.
             */
            found = FALSE;

            if (IS_IMMORTAL (ch))
                found = TRUE;

            for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
            {
                if (obj->item_type == ITEM_BOAT)
                {
                    found = TRUE;
                    break;
                }
            }
            if (!found)
            {
                send_to_char ("You need a boat to go there.\n\r", ch);
                return;
            }
        }

        move = movement_loss[UMIN (SECT_MAX - 1, in_room->sector_type)]
            + movement_loss[UMIN (SECT_MAX - 1, to_room->sector_type)];

        move /= 2;                /* i.e. the average */


        /* conditional effects */
        if (IS_AFFECTED (ch, AFF_FLYING) || IS_AFFECTED (ch, AFF_HASTE))
            move /= 2;

        if (IS_AFFECTED (ch, AFF_SLOW))
            move *= 2;

        if (ch->move < move)
        {
            send_to_char ("You are too exhausted.\n\r", ch);
            return;
        }

        WAIT_STATE (ch, 1);
        ch->move -= move;
    }

    if (!IS_AFFECTED (ch, AFF_SNEAK) && ch->invis_level < LEVEL_HERO)
        act ("$n leaves $T.", ch, NULL, dir_name[door], TO_ROOM);

    char_from_room (ch);
    char_to_room (ch, to_room);
    if (!IS_AFFECTED (ch, AFF_SNEAK) && ch->invis_level < LEVEL_HERO)
        act ("$n has arrived.", ch, NULL, NULL, TO_ROOM);

    do_function (ch, &do_look, "auto");

    if (in_room == to_room)        /* no circular follows */
        return;

    for (fch = in_room->people; fch != NULL; fch = fch_next)
    {
        fch_next = fch->next_in_room;

        if (fch->master == ch && IS_AFFECTED (fch, AFF_CHARM)
            && fch->position < POS_STANDING)
            do_function (fch, &do_stand, "");

        if (fch->master == ch && fch->position == POS_STANDING
            && can_see_room (fch, to_room))
        {

            if (IS_SET (ch->in_room->room_flags, ROOM_LAW)
                && (IS_NPC (fch) && IS_SET (fch->act, ACT_AGGRESSIVE)))
            {
                act ("You can't bring $N into the city.",
                     ch, NULL, fch, TO_CHAR);
                act ("You aren't allowed in the city.",
                     fch, NULL, NULL, TO_CHAR);
                continue;
            }

            act ("You follow $N.", fch, NULL, ch, TO_CHAR);
            move_char (fch, door, TRUE);
        }
    }

    /* 
     * If someone is following the char, these triggers get activated
     * for the followers before the char, but it's safer this way...
     */
    if (IS_NPC (ch) && HAS_TRIGGER (ch, TRIG_ENTRY))
        mp_percent_trigger (ch, NULL, NULL, NULL, TRIG_ENTRY);
    if (!IS_NPC (ch))
        mp_greet_trigger (ch);

    return;
}



void do_north (CHAR_DATA * ch, char *argument)
{
/*  if (IS_AFFECTED2(ch,AFF2_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    return;
  }
*/
    move_char (ch, DIR_NORTH, FALSE);
    return;
}



void do_east (CHAR_DATA * ch, char *argument)
{
/*  if (IS_AFFECTED2(ch,AFF2_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    return;
  }
*/
    move_char (ch, DIR_EAST, FALSE);
    return;
}



void do_south (CHAR_DATA * ch, char *argument)
{
/*
  if (IS_AFFECTED2(ch,AFF2_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    return;
  }
*/
    move_char (ch, DIR_SOUTH, FALSE);
    return;
}



void do_west (CHAR_DATA * ch, char *argument)
{

/*
 if (IS_AFFECTED2(ch,AFF2_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    return;
  }
*/
    move_char (ch, DIR_WEST, FALSE);
    return;
}



void do_up (CHAR_DATA * ch, char *argument)
{
/*
  if (IS_AFFECTED2(ch,AFF2_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    return;
  }
*/
    move_char (ch, DIR_UP, FALSE);
    return;
}



void do_down (CHAR_DATA * ch, char *argument)
{
/*
  if (IS_AFFECTED2(ch,AFF2_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    return;
  }
*/
    move_char (ch, DIR_DOWN, FALSE);
    return;
}



int find_door (CHAR_DATA * ch, char *arg)
{
    EXIT_DATA *pexit;
    int door;

    if (!str_cmp (arg, "n") || !str_cmp (arg, "north"))
        door = 0;
    else if (!str_cmp (arg, "e") || !str_cmp (arg, "east"))
        door = 1;
    else if (!str_cmp (arg, "s") || !str_cmp (arg, "south"))
        door = 2;
    else if (!str_cmp (arg, "w") || !str_cmp (arg, "west"))
        door = 3;
    else if (!str_cmp (arg, "u") || !str_cmp (arg, "up"))
        door = 4;
    else if (!str_cmp (arg, "d") || !str_cmp (arg, "down"))
        door = 5;
    else
    {
        for (door = 0; door <= 5; door++)
        {
            if ((pexit = ch->in_room->exit[door]) != NULL
                && IS_SET (pexit->exit_info, EX_ISDOOR)
                && pexit->keyword != NULL && is_name (arg, pexit->keyword))
                return door;
        }
        act ("I see no $T here.", ch, NULL, arg, TO_CHAR);
        return -1;
    }

    if ((pexit = ch->in_room->exit[door]) == NULL)
    {
        act ("I see no door $T here.", ch, NULL, arg, TO_CHAR);
        return -1;
    }

    if (!IS_SET (pexit->exit_info, EX_ISDOOR))
    {
        send_to_char ("You can't do that.\n\r", ch);
        return -1;
    }

    return door;
}



void do_open (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument (argument, arg);

    if (arg[0] == '\0')
    {
        send_to_char ("Open what?\n\r", ch);
        return;
    }

    if ((obj = get_obj_here (ch, arg)) != NULL)
    {
        /* open portal */
        if (obj->item_type == ITEM_PORTAL)
        {
            if (!IS_SET (obj->value[1], EX_ISDOOR))
            {
                send_to_char ("You can't do that.\n\r", ch);
                return;
            }

            if (!IS_SET (obj->value[1], EX_CLOSED))
            {
                send_to_char ("It's already open.\n\r", ch);
                return;
            }

            if (IS_SET (obj->value[1], EX_LOCKED))
            {
                send_to_char ("It's locked.\n\r", ch);
                return;
            }

            REMOVE_BIT (obj->value[1], EX_CLOSED);
            act ("You open $p.", ch, obj, NULL, TO_CHAR);
            act ("$n opens $p.", ch, obj, NULL, TO_ROOM);
            return;
        }

        /* 'open object' */
        if (obj->item_type != ITEM_CONTAINER)
        {
            send_to_char ("That's not a container.\n\r", ch);
            return;
        }
        if (!IS_SET (obj->value[1], CONT_CLOSED))
        {
            send_to_char ("It's already open.\n\r", ch);
            return;
        }
        if (!IS_SET (obj->value[1], CONT_CLOSEABLE))
        {
            send_to_char ("You can't do that.\n\r", ch);
            return;
        }
        if (IS_SET (obj->value[1], CONT_LOCKED))
        {
            send_to_char ("It's locked.\n\r", ch);
            return;
        }

        REMOVE_BIT (obj->value[1], CONT_CLOSED);
        act ("You open $p.", ch, obj, NULL, TO_CHAR);
        act ("$n opens $p.", ch, obj, NULL, TO_ROOM);
        return;
    }

    if ((door = find_door (ch, arg)) >= 0)
    {
        /* 'open door' */
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;

        pexit = ch->in_room->exit[door];
        if (!IS_SET (pexit->exit_info, EX_CLOSED))
        {
            send_to_char ("It's already open.\n\r", ch);
            return;
        }
        if (IS_SET (pexit->exit_info, EX_LOCKED))
        {
            send_to_char ("It's locked.\n\r", ch);
            return;
        }

        REMOVE_BIT (pexit->exit_info, EX_CLOSED);
        act ("$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM);
        send_to_char ("Ok.\n\r", ch);

        /* open the other side */
        if ((to_room = pexit->u1.to_room) != NULL
            && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL
            && pexit_rev->u1.to_room == ch->in_room)
        {
            CHAR_DATA *rch;

            REMOVE_BIT (pexit_rev->exit_info, EX_CLOSED);
            for (rch = to_room->people; rch != NULL; rch = rch->next_in_room)
                act ("The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR);
        }
    }

    return;
}



void do_close (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument (argument, arg);

    if (arg[0] == '\0')
    {
        send_to_char ("Close what?\n\r", ch);
        return;
    }

    if ((obj = get_obj_here (ch, arg)) != NULL)
    {
        /* portal stuff */
        if (obj->item_type == ITEM_PORTAL)
        {

            if (!IS_SET (obj->value[1], EX_ISDOOR)
                || IS_SET (obj->value[1], EX_NOCLOSE))
            {
                send_to_char ("You can't do that.\n\r", ch);
                return;
            }

            if (IS_SET (obj->value[1], EX_CLOSED))
            {
                send_to_char ("It's already closed.\n\r", ch);
                return;
            }

            SET_BIT (obj->value[1], EX_CLOSED);
            act ("You close $p.", ch, obj, NULL, TO_CHAR);
            act ("$n closes $p.", ch, obj, NULL, TO_ROOM);
            return;
        }

        /* 'close object' */
        if (obj->item_type != ITEM_CONTAINER)
        {
            send_to_char ("That's not a container.\n\r", ch);
            return;
        }
        if (IS_SET (obj->value[1], CONT_CLOSED))
        {
            send_to_char ("It's already closed.\n\r", ch);
            return;
        }
        if (!IS_SET (obj->value[1], CONT_CLOSEABLE))
        {
            send_to_char ("You can't do that.\n\r", ch);
            return;
        }

        SET_BIT (obj->value[1], CONT_CLOSED);
        act ("You close $p.", ch, obj, NULL, TO_CHAR);
        act ("$n closes $p.", ch, obj, NULL, TO_ROOM);
        return;
    }

    if ((door = find_door (ch, arg)) >= 0)
    {
        /* 'close door' */
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;

        pexit = ch->in_room->exit[door];
        if (IS_SET (pexit->exit_info, EX_CLOSED))
        {
            send_to_char ("It's already closed.\n\r", ch);
            return;
        }

        SET_BIT (pexit->exit_info, EX_CLOSED);
        act ("$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM);
        send_to_char ("Ok.\n\r", ch);

        /* close the other side */
        if ((to_room = pexit->u1.to_room) != NULL
            && (pexit_rev = to_room->exit[rev_dir[door]]) != 0
            && pexit_rev->u1.to_room == ch->in_room)
        {
            CHAR_DATA *rch;

            SET_BIT (pexit_rev->exit_info, EX_CLOSED);
            for (rch = to_room->people; rch != NULL; rch = rch->next_in_room)
                act ("The $d closes.", rch, NULL, pexit_rev->keyword,
                     TO_CHAR);
        }
    }

    return;
}



bool has_key (CHAR_DATA * ch, int key)
{
    OBJ_DATA *obj;

    for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
    {
        if (obj->pIndexData->vnum == key)
            return TRUE;
    }

    return FALSE;
}



void do_lock (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument (argument, arg);

    if (arg[0] == '\0')
    {
        send_to_char ("Lock what?\n\r", ch);
        return;
    }

    if ((obj = get_obj_here (ch, arg)) != NULL)
    {
        /* portal stuff */
        if (obj->item_type == ITEM_PORTAL)
        {
            if (!IS_SET (obj->value[1], EX_ISDOOR)
                || IS_SET (obj->value[1], EX_NOCLOSE))
            {
                send_to_char ("You can't do that.\n\r", ch);
                return;
            }
            if (!IS_SET (obj->value[1], EX_CLOSED))
            {
                send_to_char ("It's not closed.\n\r", ch);
                return;
            }

            if (obj->value[4] < 0 || IS_SET (obj->value[1], EX_NOLOCK))
            {
                send_to_char ("It can't be locked.\n\r", ch);
                return;
            }

            if (!has_key (ch, obj->value[4]))
            {
                send_to_char ("You lack the key.\n\r", ch);
                return;
            }

            if (IS_SET (obj->value[1], EX_LOCKED))
            {
                send_to_char ("It's already locked.\n\r", ch);
                return;
            }

            SET_BIT (obj->value[1], EX_LOCKED);
            act ("You lock $p.", ch, obj, NULL, TO_CHAR);
            act ("$n locks $p.", ch, obj, NULL, TO_ROOM);
            return;
        }

        /* 'lock object' */
        if (obj->item_type != ITEM_CONTAINER)
        {
            send_to_char ("That's not a container.\n\r", ch);
            return;
        }
        if (!IS_SET (obj->value[1], CONT_CLOSED))
        {
            send_to_char ("It's not closed.\n\r", ch);
            return;
        }
        if (obj->value[2] < 0)
        {
            send_to_char ("It can't be locked.\n\r", ch);
            return;
        }
        if (!has_key (ch, obj->value[2]))
        {
            send_to_char ("You lack the key.\n\r", ch);
            return;
        }
        if (IS_SET (obj->value[1], CONT_LOCKED))
        {
            send_to_char ("It's already locked.\n\r", ch);
            return;
        }

        SET_BIT (obj->value[1], CONT_LOCKED);
        act ("You lock $p.", ch, obj, NULL, TO_CHAR);
        act ("$n locks $p.", ch, obj, NULL, TO_ROOM);
        return;
    }

    if ((door = find_door (ch, arg)) >= 0)
    {
        /* 'lock door' */
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;

        pexit = ch->in_room->exit[door];
        if (!IS_SET (pexit->exit_info, EX_CLOSED))
        {
            send_to_char ("It's not closed.\n\r", ch);
            return;
        }
        if (pexit->key < 0)
        {
            send_to_char ("It can't be locked.\n\r", ch);
            return;
        }
        if (!has_key (ch, pexit->key))
        {
            send_to_char ("You lack the key.\n\r", ch);
            return;
        }
        if (IS_SET (pexit->exit_info, EX_LOCKED))
        {
            send_to_char ("It's already locked.\n\r", ch);
            return;
        }

        SET_BIT (pexit->exit_info, EX_LOCKED);
        send_to_char ("*Click*\n\r", ch);
        act ("$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM);

        /* lock the other side */
        if ((to_room = pexit->u1.to_room) != NULL
            && (pexit_rev = to_room->exit[rev_dir[door]]) != 0
            && pexit_rev->u1.to_room == ch->in_room)
        {
            SET_BIT (pexit_rev->exit_info, EX_LOCKED);
        }
    }

    return;
}



void do_unlock (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument (argument, arg);

    if (arg[0] == '\0')
    {
        send_to_char ("Unlock what?\n\r", ch);
        return;
    }

    if ((obj = get_obj_here (ch, arg)) != NULL)
    {
        /* portal stuff */
        if (obj->item_type == ITEM_PORTAL)
        {
            if (!IS_SET (obj->value[1], EX_ISDOOR))
            {
                send_to_char ("You can't do that.\n\r", ch);
                return;
            }

            if (!IS_SET (obj->value[1], EX_CLOSED))
            {
                send_to_char ("It's not closed.\n\r", ch);
                return;
            }

            if (obj->value[4] < 0)
            {
                send_to_char ("It can't be unlocked.\n\r", ch);
                return;
            }

            if (!has_key (ch, obj->value[4]))
            {
                send_to_char ("You lack the key.\n\r", ch);
                return;
            }

            if (!IS_SET (obj->value[1], EX_LOCKED))
            {
                send_to_char ("It's already unlocked.\n\r", ch);
                return;
            }

            REMOVE_BIT (obj->value[1], EX_LOCKED);
            act ("You unlock $p.", ch, obj, NULL, TO_CHAR);
            act ("$n unlocks $p.", ch, obj, NULL, TO_ROOM);
            return;
        }

        /* 'unlock object' */
        if (obj->item_type != ITEM_CONTAINER)
        {
            send_to_char ("That's not a container.\n\r", ch);
            return;
        }
        if (!IS_SET (obj->value[1], CONT_CLOSED))
        {
            send_to_char ("It's not closed.\n\r", ch);
            return;
        }
        if (obj->value[2] < 0)
        {
            send_to_char ("It can't be unlocked.\n\r", ch);
            return;
        }
        if (!has_key (ch, obj->value[2]))
        {
            send_to_char ("You lack the key.\n\r", ch);
            return;
        }
        if (!IS_SET (obj->value[1], CONT_LOCKED))
        {
            send_to_char ("It's already unlocked.\n\r", ch);
            return;
        }

        REMOVE_BIT (obj->value[1], CONT_LOCKED);
        act ("You unlock $p.", ch, obj, NULL, TO_CHAR);
        act ("$n unlocks $p.", ch, obj, NULL, TO_ROOM);
        return;
    }

    if ((door = find_door (ch, arg)) >= 0)
    {
        /* 'unlock door' */
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;

        pexit = ch->in_room->exit[door];
        if (!IS_SET (pexit->exit_info, EX_CLOSED))
        {
            send_to_char ("It's not closed.\n\r", ch);
            return;
        }
        if (pexit->key < 0)
        {
            send_to_char ("It can't be unlocked.\n\r", ch);
            return;
        }
        if (!has_key (ch, pexit->key))
        {
            send_to_char ("You lack the key.\n\r", ch);
            return;
        }
        if (!IS_SET (pexit->exit_info, EX_LOCKED))
        {
            send_to_char ("It's already unlocked.\n\r", ch);
            return;
        }

        REMOVE_BIT (pexit->exit_info, EX_LOCKED);
        send_to_char ("*Click*\n\r", ch);
        act ("$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM);

        /* unlock the other side */
        if ((to_room = pexit->u1.to_room) != NULL
            && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL
            && pexit_rev->u1.to_room == ch->in_room)
        {
            REMOVE_BIT (pexit_rev->exit_info, EX_LOCKED);
        }
    }

    return;
}



void do_pick (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument (argument, arg);

    if (arg[0] == '\0')
    {
        send_to_char ("Pick what?\n\r", ch);
        return;
    }

    WAIT_STATE (ch, skill_table[gsn_pick_lock].beats);

    /* look for guards */
    for (gch = ch->in_room->people; gch; gch = gch->next_in_room)
    {
        if (IS_NPC (gch) && IS_AWAKE (gch) && ch->level + 5 < gch->level)
        {
            act ("$N is standing too close to the lock.",
                 ch, NULL, gch, TO_CHAR);
            return;
        }
    }

    if (!IS_NPC (ch) && number_percent () > get_skill (ch, gsn_pick_lock))
    {
        send_to_char ("You failed.\n\r", ch);
        check_improve (ch, gsn_pick_lock, FALSE, 2);
        return;
    }

    if ((obj = get_obj_here (ch, arg)) != NULL)
    {
        /* portal stuff */
        if (obj->item_type == ITEM_PORTAL)
        {
            if (!IS_SET (obj->value[1], EX_ISDOOR))
            {
                send_to_char ("You can't do that.\n\r", ch);
                return;
            }

            if (!IS_SET (obj->value[1], EX_CLOSED))
            {
                send_to_char ("It's not closed.\n\r", ch);
                return;
            }

            if (obj->value[4] < 0)
            {
                send_to_char ("It can't be unlocked.\n\r", ch);
                return;
            }

            if (IS_SET (obj->value[1], EX_PICKPROOF))
            {
                send_to_char ("You failed.\n\r", ch);
                return;
            }

            REMOVE_BIT (obj->value[1], EX_LOCKED);
            act ("You pick the lock on $p.", ch, obj, NULL, TO_CHAR);
            act ("$n picks the lock on $p.", ch, obj, NULL, TO_ROOM);
            check_improve (ch, gsn_pick_lock, TRUE, 2);
            return;
        }





        /* 'pick object' */
        if (obj->item_type != ITEM_CONTAINER)
        {
            send_to_char ("That's not a container.\n\r", ch);
            return;
        }
        if (!IS_SET (obj->value[1], CONT_CLOSED))
        {
            send_to_char ("It's not closed.\n\r", ch);
            return;
        }
        if (obj->value[2] < 0)
        {
            send_to_char ("It can't be unlocked.\n\r", ch);
            return;
        }
        if (!IS_SET (obj->value[1], CONT_LOCKED))
        {
            send_to_char ("It's already unlocked.\n\r", ch);
            return;
        }
        if (IS_SET (obj->value[1], CONT_PICKPROOF))
        {
            send_to_char ("You failed.\n\r", ch);
            return;
        }

        REMOVE_BIT (obj->value[1], CONT_LOCKED);
        act ("You pick the lock on $p.", ch, obj, NULL, TO_CHAR);
        act ("$n picks the lock on $p.", ch, obj, NULL, TO_ROOM);
        check_improve (ch, gsn_pick_lock, TRUE, 2);
        return;
    }

    if ((door = find_door (ch, arg)) >= 0)
    {
        /* 'pick door' */
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;

        pexit = ch->in_room->exit[door];
        if (!IS_SET (pexit->exit_info, EX_CLOSED) && !IS_IMMORTAL (ch))
        {
            send_to_char ("It's not closed.\n\r", ch);
            return;
        }
        if (pexit->key < 0 && !IS_IMMORTAL (ch))
        {
            send_to_char ("It can't be picked.\n\r", ch);
            return;
        }
        if (!IS_SET (pexit->exit_info, EX_LOCKED))
        {
            send_to_char ("It's already unlocked.\n\r", ch);
            return;
        }
        if (IS_SET (pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL (ch))
        {
            send_to_char ("You failed.\n\r", ch);
            return;
        }

        REMOVE_BIT (pexit->exit_info, EX_LOCKED);
        send_to_char ("*Click*\n\r", ch);
        act ("$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM);
        check_improve (ch, gsn_pick_lock, TRUE, 2);

        /* pick the other side */
        if ((to_room = pexit->u1.to_room) != NULL
            && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL
            && pexit_rev->u1.to_room == ch->in_room)
        {
            REMOVE_BIT (pexit_rev->exit_info, EX_LOCKED);
        }
    }

    return;
}




void do_stand (CHAR_DATA * ch, char *argument)
{
    OBJ_DATA *obj = NULL;

    if (argument[0] != '\0')
    {
        if (ch->position == POS_FIGHTING)
        {
            send_to_char ("Maybe you should finish fighting first?\n\r", ch);
            return;
        }
        obj = get_obj_list (ch, argument, ch->in_room->contents);
        if (obj == NULL)
        {
            send_to_char ("You don't see that here.\n\r", ch);
            return;
        }
        if (obj->item_type != ITEM_FURNITURE
            || (!IS_SET (obj->value[2], STAND_AT)
                && !IS_SET (obj->value[2], STAND_ON)
                && !IS_SET (obj->value[2], STAND_IN)))
        {
            send_to_char ("You can't seem to find a place to stand.\n\r", ch);
            return;
        }
        if (ch->on != obj && count_users (obj) >= obj->value[0])
        {
            act_new ("There's no room to stand on $p.",
                     ch, obj, NULL, TO_CHAR, POS_DEAD);
            return;
        }
        ch->on = obj;
    }

    switch (ch->position)
    {
        case POS_SLEEPING:
            if (IS_AFFECTED (ch, AFF_SLEEP))
            {
                send_to_char ("You can't wake up!\n\r", ch);
                return;
            }

            if (obj == NULL)
            {
                send_to_char ("You wake and stand up.\n\r", ch);
                act ("$n wakes and stands up.", ch, NULL, NULL, TO_ROOM);
                ch->on = NULL;
            }
            else if (IS_SET (obj->value[2], STAND_AT))
            {
                act_new ("You wake and stand at $p.", ch, obj, NULL, TO_CHAR,
                         POS_DEAD);
                act ("$n wakes and stands at $p.", ch, obj, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], STAND_ON))
            {
                act_new ("You wake and stand on $p.", ch, obj, NULL, TO_CHAR,
                         POS_DEAD);
                act ("$n wakes and stands on $p.", ch, obj, NULL, TO_ROOM);
            }
            else
            {
                act_new ("You wake and stand in $p.", ch, obj, NULL, TO_CHAR,
                         POS_DEAD);
                act ("$n wakes and stands in $p.", ch, obj, NULL, TO_ROOM);
            }
            ch->position = POS_STANDING;
            do_function (ch, &do_look, "auto");
            break;

        case POS_RESTING:
        case POS_SITTING:
            if (obj == NULL)
            {
                send_to_char ("You stand up.\n\r", ch);
                act ("$n stands up.", ch, NULL, NULL, TO_ROOM);
                ch->on = NULL;
            }
            else if (IS_SET (obj->value[2], STAND_AT))
            {
                act ("You stand at $p.", ch, obj, NULL, TO_CHAR);
                act ("$n stands at $p.", ch, obj, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], STAND_ON))
            {
                act ("You stand on $p.", ch, obj, NULL, TO_CHAR);
                act ("$n stands on $p.", ch, obj, NULL, TO_ROOM);
            }
            else
            {
                act ("You stand in $p.", ch, obj, NULL, TO_CHAR);
                act ("$n stands on $p.", ch, obj, NULL, TO_ROOM);
            }
            ch->position = POS_STANDING;
            break;

        case POS_STANDING:
            send_to_char ("You are already standing.\n\r", ch);
            break;

        case POS_FIGHTING:
            send_to_char ("You are already fighting!\n\r", ch);
            break;
    }

    return;
}



void do_rest (CHAR_DATA * ch, char *argument)
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
        send_to_char ("You are already fighting!\n\r", ch);
        return;
    }

    /* okay, now that we know we can rest, find an object to rest on */
    if (argument[0] != '\0')
    {
        obj = get_obj_list (ch, argument, ch->in_room->contents);
        if (obj == NULL)
        {
            send_to_char ("You don't see that here.\n\r", ch);
            return;
        }
    }
    else
        obj = ch->on;

    if (obj != NULL)
    {
        if (obj->item_type != ITEM_FURNITURE
            || (!IS_SET (obj->value[2], REST_ON)
                && !IS_SET (obj->value[2], REST_IN)
                && !IS_SET (obj->value[2], REST_AT)))
        {
            send_to_char ("You can't rest on that.\n\r", ch);
            return;
        }

        if (obj != NULL && ch->on != obj
            && count_users (obj) >= obj->value[0])
        {
            act_new ("There's no more room on $p.", ch, obj, NULL, TO_CHAR,
                     POS_DEAD);
            return;
        }

        ch->on = obj;
    }

    switch (ch->position)
    {
        case POS_SLEEPING:
            if (IS_AFFECTED (ch, AFF_SLEEP))
            {
                send_to_char ("You can't wake up!\n\r", ch);
                return;
            }

            if (obj == NULL)
            {
                send_to_char ("You wake up and start resting.\n\r", ch);
                act ("$n wakes up and starts resting.", ch, NULL, NULL,
                     TO_ROOM);
            }
            else if (IS_SET (obj->value[2], REST_AT))
            {
                act_new ("You wake up and rest at $p.",
                         ch, obj, NULL, TO_CHAR, POS_SLEEPING);
                act ("$n wakes up and rests at $p.", ch, obj, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], REST_ON))
            {
                act_new ("You wake up and rest on $p.",
                         ch, obj, NULL, TO_CHAR, POS_SLEEPING);
                act ("$n wakes up and rests on $p.", ch, obj, NULL, TO_ROOM);
            }
            else
            {
                act_new ("You wake up and rest in $p.",
                         ch, obj, NULL, TO_CHAR, POS_SLEEPING);
                act ("$n wakes up and rests in $p.", ch, obj, NULL, TO_ROOM);
            }
            ch->position = POS_RESTING;
            break;

        case POS_RESTING:
            send_to_char ("You are already resting.\n\r", ch);
            break;

        case POS_STANDING:
            if (obj == NULL)
            {
                send_to_char ("You rest.\n\r", ch);
                act ("$n sits down and rests.", ch, NULL, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], REST_AT))
            {
                act ("You sit down at $p and rest.", ch, obj, NULL, TO_CHAR);
                act ("$n sits down at $p and rests.", ch, obj, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], REST_ON))
            {
                act ("You sit on $p and rest.", ch, obj, NULL, TO_CHAR);
                act ("$n sits on $p and rests.", ch, obj, NULL, TO_ROOM);
            }
            else
            {
                act ("You rest in $p.", ch, obj, NULL, TO_CHAR);
                act ("$n rests in $p.", ch, obj, NULL, TO_ROOM);
            }
            ch->position = POS_RESTING;
            break;

        case POS_SITTING:
            if (obj == NULL)
            {
                send_to_char ("You rest.\n\r", ch);
                act ("$n rests.", ch, NULL, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], REST_AT))
            {
                act ("You rest at $p.", ch, obj, NULL, TO_CHAR);
                act ("$n rests at $p.", ch, obj, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], REST_ON))
            {
                act ("You rest on $p.", ch, obj, NULL, TO_CHAR);
                act ("$n rests on $p.", ch, obj, NULL, TO_ROOM);
            }
            else
            {
                act ("You rest in $p.", ch, obj, NULL, TO_CHAR);
                act ("$n rests in $p.", ch, obj, NULL, TO_ROOM);
            }
            ch->position = POS_RESTING;
            break;
    }


    return;
}


void do_sit (CHAR_DATA * ch, char *argument)
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
        send_to_char ("Maybe you should finish this fight first?\n\r", ch);
        return;
    }

    /* okay, now that we know we can sit, find an object to sit on */
    if (argument[0] != '\0')
    {
        obj = get_obj_list (ch, argument, ch->in_room->contents);
        if (obj == NULL)
        {
            send_to_char ("You don't see that here.\n\r", ch);
            return;
        }
    }
    else
        obj = ch->on;

    if (obj != NULL)
    {
        if (obj->item_type != ITEM_FURNITURE
            || (!IS_SET (obj->value[2], SIT_ON)
                && !IS_SET (obj->value[2], SIT_IN)
                && !IS_SET (obj->value[2], SIT_AT)))
        {
            send_to_char ("You can't sit on that.\n\r", ch);
            return;
        }

        if (obj != NULL && ch->on != obj
            && count_users (obj) >= obj->value[0])
        {
            act_new ("There's no more room on $p.", ch, obj, NULL, TO_CHAR,
                     POS_DEAD);
            return;
        }

        ch->on = obj;
    }
    switch (ch->position)
    {
        case POS_SLEEPING:
            if (IS_AFFECTED (ch, AFF_SLEEP))
            {
                send_to_char ("You can't wake up!\n\r", ch);
                return;
            }

            if (obj == NULL)
            {
                send_to_char ("You wake and sit up.\n\r", ch);
                act ("$n wakes and sits up.", ch, NULL, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], SIT_AT))
            {
                act_new ("You wake and sit at $p.", ch, obj, NULL, TO_CHAR,
                         POS_DEAD);
                act ("$n wakes and sits at $p.", ch, obj, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], SIT_ON))
            {
                act_new ("You wake and sit on $p.", ch, obj, NULL, TO_CHAR,
                         POS_DEAD);
                act ("$n wakes and sits at $p.", ch, obj, NULL, TO_ROOM);
            }
            else
            {
                act_new ("You wake and sit in $p.", ch, obj, NULL, TO_CHAR,
                         POS_DEAD);
                act ("$n wakes and sits in $p.", ch, obj, NULL, TO_ROOM);
            }

            ch->position = POS_SITTING;
            break;
        case POS_RESTING:
            if (obj == NULL)
                send_to_char ("You stop resting.\n\r", ch);
            else if (IS_SET (obj->value[2], SIT_AT))
            {
                act ("You sit at $p.", ch, obj, NULL, TO_CHAR);
                act ("$n sits at $p.", ch, obj, NULL, TO_ROOM);
            }

            else if (IS_SET (obj->value[2], SIT_ON))
            {
                act ("You sit on $p.", ch, obj, NULL, TO_CHAR);
                act ("$n sits on $p.", ch, obj, NULL, TO_ROOM);
            }
            ch->position = POS_SITTING;
            break;
        case POS_SITTING:
            send_to_char ("You are already sitting down.\n\r", ch);
            break;
        case POS_STANDING:
            if (obj == NULL)
            {
                send_to_char ("You sit down.\n\r", ch);
                act ("$n sits down on the ground.", ch, NULL, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], SIT_AT))
            {
                act ("You sit down at $p.", ch, obj, NULL, TO_CHAR);
                act ("$n sits down at $p.", ch, obj, NULL, TO_ROOM);
            }
            else if (IS_SET (obj->value[2], SIT_ON))
            {
                act ("You sit on $p.", ch, obj, NULL, TO_CHAR);
                act ("$n sits on $p.", ch, obj, NULL, TO_ROOM);
            }
            else
            {
                act ("You sit down in $p.", ch, obj, NULL, TO_CHAR);
                act ("$n sits down in $p.", ch, obj, NULL, TO_ROOM);
            }
            ch->position = POS_SITTING;
            break;
    }
    return;
}


void do_sleep (CHAR_DATA * ch, char *argument)
{
    OBJ_DATA *obj = NULL;

    switch (ch->position)
    {
        case POS_SLEEPING:
            send_to_char ("You are already sleeping.\n\r", ch);
            break;

        case POS_RESTING:
        case POS_SITTING:
        case POS_STANDING:
            if (argument[0] == '\0' && ch->on == NULL)
            {
                send_to_char ("You go to sleep.\n\r", ch);
                act ("$n goes to sleep.", ch, NULL, NULL, TO_ROOM);
                ch->position = POS_SLEEPING;
            }
            else
            {                    /* find an object and sleep on it */

                if (argument[0] == '\0')
                    obj = ch->on;
                else
                    obj = get_obj_list (ch, argument, ch->in_room->contents);

                if (obj == NULL)
                {
                    send_to_char ("You don't see that here.\n\r", ch);
                    return;
                }
                if (obj->item_type != ITEM_FURNITURE
                    || (!IS_SET (obj->value[2], SLEEP_ON)
                        && !IS_SET (obj->value[2], SLEEP_IN)
                        && !IS_SET (obj->value[2], SLEEP_AT)))
                {
                    send_to_char ("You can't sleep on that!\n\r", ch);
                    return;
                }

                if (ch->on != obj && count_users (obj) >= obj->value[0])
                {
                    act_new ("There is no room on $p for you.",
                             ch, obj, NULL, TO_CHAR, POS_DEAD);
                    return;
                }

                ch->on = obj;
                if (IS_SET (obj->value[2], SLEEP_AT))
                {
                    act ("You go to sleep at $p.", ch, obj, NULL, TO_CHAR);
                    act ("$n goes to sleep at $p.", ch, obj, NULL, TO_ROOM);
                }
                else if (IS_SET (obj->value[2], SLEEP_ON))
                {
                    act ("You go to sleep on $p.", ch, obj, NULL, TO_CHAR);
                    act ("$n goes to sleep on $p.", ch, obj, NULL, TO_ROOM);
                }
                else
                {
                    act ("You go to sleep in $p.", ch, obj, NULL, TO_CHAR);
                    act ("$n goes to sleep in $p.", ch, obj, NULL, TO_ROOM);
                }
                ch->position = POS_SLEEPING;
            }
            break;

        case POS_FIGHTING:
            send_to_char ("You are already fighting!\n\r", ch);
            break;
    }

    return;
}



void do_wake (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument (argument, arg);
    if (arg[0] == '\0')
    {
        do_function (ch, &do_stand, "");
        return;
    }

    if (!IS_AWAKE (ch))
    {
        send_to_char ("You are asleep yourself!\n\r", ch);
        return;
    }

    if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

    if (IS_AWAKE (victim))
    {
        act ("$N is already awake.", ch, NULL, victim, TO_CHAR);
        return;
    }

    if (IS_AFFECTED (victim, AFF_SLEEP))
    {
        act ("You can't wake $M!", ch, NULL, victim, TO_CHAR);
        return;
    }

    act_new ("$n wakes you.", ch, NULL, victim, TO_VICT, POS_SLEEPING);
    do_function (ch, &do_stand, "");
    return;
}



void do_sneak (CHAR_DATA * ch, char *argument)
{
    AFFECT_DATA af;

    send_to_char ("You attempt to move silently.\n\r", ch);
    affect_strip (ch, gsn_sneak);

    if (IS_AFFECTED (ch, AFF_SNEAK))
        return;

    if (number_percent () < get_skill (ch, gsn_sneak))
    {
        check_improve (ch, gsn_sneak, TRUE, 3);
        af.where = TO_AFFECTS;
        af.type = gsn_sneak;
        af.level = ch->level;
        af.duration = ch->level;
        af.location = APPLY_NONE;
        af.modifier = 0;
        af.bitvector = AFF_SNEAK;
        affect_to_char (ch, &af);
    }
    else
        check_improve (ch, gsn_sneak, FALSE, 3);

    return;
}



void do_hide (CHAR_DATA * ch, char *argument)
{
    send_to_char ("You attempt to hide.\n\r", ch);

    if (IS_AFFECTED (ch, AFF_HIDE))
        REMOVE_BIT (ch->affected_by, AFF_HIDE);

    if (number_percent () < get_skill (ch, gsn_hide))
    {
        SET_BIT (ch->affected_by, AFF_HIDE);
        check_improve (ch, gsn_hide, TRUE, 3);
    }
    else
        check_improve (ch, gsn_hide, FALSE, 3);

    return;
}



/*
 * Contributed by Alander.
 */
void do_visible (CHAR_DATA * ch, char *argument)
{
    affect_strip (ch, gsn_invis);
    affect_strip (ch, gsn_mass_invis);
    affect_strip (ch, gsn_sneak);
    REMOVE_BIT (ch->affected_by, AFF_HIDE);
    REMOVE_BIT (ch->affected_by, AFF_INVISIBLE);
    REMOVE_BIT (ch->affected_by, AFF_SNEAK);
    send_to_char ("Ok.\n\r", ch);
    return;
}



void do_recall (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;

    if (IS_NPC (ch) && !IS_SET (ch->act, ACT_PET))
    {
        send_to_char ("Only players can recall.\n\r", ch);
        return;
    }

    act ("$n prays for transportation!", ch, 0, 0, TO_ROOM);

//    if ((location = get_room_index (ROOM_VNUM_TEMPLE)) == NULL)
//    {
//        send_to_char ("You are completely lost.\n\r", ch);
//        return;
//    }
	if(!is_clan(ch))
	{
		// if((location = get_room_index( ROOM_VNUM_TEMPLE ) ) == NULL )
    if ( ( location = get_room_index( hometown_table[ch->hometown].recall )) == 
NULL )
		{
		send_to_char( "You are completely lost.\n\r", ch );
		return;
		}
	}

	else /* in a clan */
	{
		if((location = get_room_index(clan_table[ch->clan].recall)) == NULL)
		{
		send_to_char( "Your clan recall room doesn't exist.\n\r", ch );
		return;
		}
	}

    if (ch->in_room == location)
        return;

    if (IS_SET (ch->in_room->room_flags, ROOM_NO_RECALL)
        || IS_AFFECTED (ch, AFF_CURSE))
    {
        send_to_char ("Mota has forsaken you.\n\r", ch);
        return;
    }

    if ((victim = ch->fighting) != NULL)
    {
        int lose, skill;

        skill = get_skill (ch, gsn_recall);

        if (number_percent () < 80 * skill / 100)
        {
            check_improve (ch, gsn_recall, FALSE, 6);
            WAIT_STATE (ch, 4);
            sprintf (buf, "You failed!.\n\r");
            send_to_char (buf, ch);
            return;
        }

        lose = (ch->desc != NULL) ? 25 : 50;
//        gain_exp (ch, 0 - lose);
        check_improve (ch, gsn_recall, TRUE, 4);
        sprintf (buf, "You recall from combat!  You lose %d exps.\n\r", lose);
        send_to_char (buf, ch);
        stop_fighting (ch, TRUE);

    }

    ch->move /= 2;
    act ("$n disappears.", ch, NULL, NULL, TO_ROOM);
    char_from_room (ch);
    char_to_room (ch, location);
    act ("$n appears in the room.", ch, NULL, NULL, TO_ROOM);
    do_function (ch, &do_look, "auto");

    if (ch->pet != NULL)
        do_function (ch->pet, &do_recall, "");

    return;
}



void do_train (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    sh_int stat = -1;
    char *pOutput = NULL;
    int cost;
		char arg1[MAX_INPUT_LENGTH];
		char arg2[MAX_INPUT_LENGTH];
		char arg3[MAX_INPUT_LENGTH];

    if (IS_NPC (ch))
        return;

    /*
     * Check for trainer.
     */
    for (mob = ch->in_room->people; mob; mob = mob->next_in_room)
    {
        if (IS_NPC (mob) && IS_SET (mob->act, ACT_TRAIN))
            break;
    }

    if (mob == NULL)
    {
        send_to_char ("You can't do that here.\n\r", ch);
        return;
    }

    if (argument[0] == '\0')
    {
        sprintf (buf, "You have %d training sessions.\n\r", ch->train);
        send_to_char (buf, ch);
        argument = "foo";
    }
	strcpy(arg3,argument);//Save it so when we chop it up into str1 and str2 we still have it for focus change purposes.
	argument = one_argument(argument,arg1);//Break down the argument for ability to train all.
	strcpy(arg2,argument);

    cost = 1;

    if (!str_cmp (arg1, "str"))
    {
        if (class_table[ch->class].attr_prime == STAT_STR)
            cost = 1;
        stat = STAT_STR;
        pOutput = "strength";
    }

    else if (!str_cmp (arg1, "int"))
    {
        if (class_table[ch->class].attr_prime == STAT_INT)
            cost = 1;
        stat = STAT_INT;
        pOutput = "intelligence";
    }

    else if (!str_cmp (arg1, "wis"))
    {
        if (class_table[ch->class].attr_prime == STAT_WIS)
            cost = 1;
        stat = STAT_WIS;
        pOutput = "wisdom";
    }

    else if (!str_cmp (arg1, "dex"))
    {
        if (class_table[ch->class].attr_prime == STAT_DEX)
            cost = 1;
        stat = STAT_DEX;
        pOutput = "dexterity";
    }

    else if (!str_cmp (arg1, "con"))
    {
        if (class_table[ch->class].attr_prime == STAT_CON)
            cost = 1;
        stat = STAT_CON;
        pOutput = "constitution";
    }
    else if (!str_cmp (arg1, "vit"))
    {
        if (class_table[ch->class].attr_prime == STAT_VIT)
            cost = 1;
        stat = STAT_VIT;
        pOutput = "vitality";
    }
    else if (!str_cmp (arg1, "agi"))
    {
        if (class_table[ch->class].attr_prime == STAT_AGI)
            cost = 1;
        stat = STAT_AGI;
        pOutput = "agility";
    }
    else if (!str_cmp (arg1, "dis"))
    {
        if (class_table[ch->class].attr_prime == STAT_DIS)
            cost = 1;
        stat = STAT_DIS;
        pOutput = "discipline";
    }
    else if (!str_cmp (arg1, "cha"))
    {
        if (class_table[ch->class].attr_prime == STAT_CHA)
            cost = 1;
        stat = STAT_CHA;
        pOutput = "charisma";
    }
    else if (!str_cmp (arg1, "luc"))
    {
        if (class_table[ch->class].attr_prime == STAT_LUC)
            cost = 1;
        stat = STAT_LUC;
        pOutput = "luck";
    }

	else if (!str_cmp (arg3, "combat power"))
	{
		send_to_char("You are brought to a strange apparatus by your guildmaster. The room you enter has a simple looking training dummy in the center, and two glass rods on either side.  He touches each rod, and they begin to glow. In  moments, you feel a tremendous force pressing against you.  You are instructed  to attack the dummy.  Only through great effort can you raise your weapon and  attack it.  In time, you feel stronger with your weapon.\n\r",ch);
		do_say(mob,"I have done what I can, you must learn the rest on your own.");
		ch->pcdata->focus[CURRENT_FOCUS] = COMBAT_POWER;
		return;
	}

	else if (!str_cmp (arg3, "combat defense"))
	{
		send_to_char("Your guildmater brings you to the training yard, he wields a staff and tells you to defend yourself.  His greater speed and agility allows him to score multiple blows.  He yells out commands to you, and you slowly improve.\n\r",ch);
		do_say(mob,"I have done what I can, you must learn the rest on your own.");
		ch->pcdata->focus[CURRENT_FOCUS] = COMBAT_DEFENSE;
		return;
	}

	else if (!str_cmp (arg3, "combat agility"))
	{
		send_to_char("You are led by your guildmaster to the training yard.  She is wearing monk's garb.  When you arrive, she throws you a rod, and she wields a club and a shield.  You start combat, and you find she is a much harder target than you anticipated.  She is able to hit you several times, and deflect all of your blows.  In time, you are able to by pass her defenses, and dodge her attacks.\n\r",ch);
		do_say(mob,"I have done what I can, you must learn the rest on your own.");
		ch->pcdata->focus[CURRENT_FOCUS] = COMBAT_AGILITY;
		return;
	}

	else if (!str_cmp (arg3, "combat toughness"))
	{
		send_to_char("You are brought to the training yard.  In there, a strange contraption. It appears to be a gauntlet of sorts.  It has several swinging balls and logs that pass by the path in the center.  You are dragged to the start, and are told to go through.  As you enter, you soon realize it's hopeless to try to avoid them all.  You take several hits, but you learn how to take them well.  After you make it through and rest, you begin to feel tougher.\n\r",ch);
		do_say(mob,"I have done what I can, you must learn the rest on your own.");
		ch->pcdata->focus[CURRENT_FOCUS] = COMBAT_TOUGHNESS;
		return;
	}

	else if (!str_cmp (arg3, "magic power"))
	{
		send_to_char("A mage draped in white robes come up to you, and she instructs you to follow.  She leads down to an empty room, only occupied by strange, archaic symbols.  She tells you to cast a spell, and it fades as soon as it's cast.\n\r",ch);
		do_say(mob,"This room is protected from magic, cast freely here, and practice your skills.  I will show you what you are doing wrong, and your will improve.");
		send_to_char("After a time, you complete your training.\n\r",ch);
		do_say(mob,"I have done what I can, you must learn the rest on your own.");
		ch->pcdata->focus[CURRENT_FOCUS] = MAGIC_POWER;
		return;
	}

	else if (!str_cmp (arg3, "magic ability"))
	{
		send_to_char("An old man greets you.  He shows you into a peaceful arboretum filled with plants and running water.  You meditate for several hours, and you feel more focused and in control of your mind.\n\r",ch);
		do_say(mob,"I have done what I can, you must learn the rest on your own.");
		ch->pcdata->focus[CURRENT_FOCUS] = MAGIC_ABILITY;
		return;
	}

	else if (!str_cmp (arg3, "magic defense"))
	{
		send_to_char("As you enter the room with your guildmaster, you feel a magical force. As you become more adjusted, you feel it emanating from a large ruby floating in the center of the room.  Your guildmaster utters a word of magic, and the ruby begins to glow brightly.  It soon send out waves of magic.  In time, and only through intense concentration, can you begin to repel the powerful bursts.\n\r",ch);
		do_say(mob,"I have done what I can, you must learn the rest on your own.");
		ch->pcdata->focus[CURRENT_FOCUS] = MAGIC_DEFENSE;
		return;
	}


    else if (!str_cmp (arg1, "hp"))
        cost = 1;

    else if (!str_cmp (arg1, "mana"))
        cost = 1;

    else if (!str_cmp (arg1, "toughness"))
        cost = 1;

    else if (!str_cmp (arg1, "tier"))
	cost = 1;

    else
    {
        strcpy (buf, "You can train:");
        if (ch->perm_stat[STAT_STR] < get_max_train (ch, STAT_STR))
            strcat (buf, " str");
        if (ch->perm_stat[STAT_INT] < get_max_train (ch, STAT_INT))
            strcat (buf, " int");
        if (ch->perm_stat[STAT_WIS] < get_max_train (ch, STAT_WIS))
            strcat (buf, " wis");
        if (ch->perm_stat[STAT_DEX] < get_max_train (ch, STAT_DEX))
            strcat (buf, " dex");
        if (ch->perm_stat[STAT_CON] < get_max_train (ch, STAT_CON))
            strcat (buf, " con");
        if (ch->perm_stat[STAT_VIT] < get_max_train (ch, STAT_VIT))
            strcat (buf, " vit");
        if (ch->perm_stat[STAT_AGI] < get_max_train (ch, STAT_AGI))
            strcat (buf, " agi");
        if (ch->perm_stat[STAT_DIS] < get_max_train (ch, STAT_DIS))
            strcat (buf, " dis");
        if (ch->perm_stat[STAT_CHA] < get_max_train (ch, STAT_CHA))
            strcat (buf, " cha");
        if (ch->perm_stat[STAT_LUC] < get_max_train (ch, STAT_LUC))
            strcat (buf, " luc");

        strcat (buf, " hp mana move");

        if (buf[strlen (buf) - 1] != ':')
        {
            strcat (buf, ".\n\r");
            send_to_char (buf, ch);
        }


        if (ch->tier == 0)
        send_to_char("Tier 1(tier)\n\r",ch);
        if (ch->tier == 1)
        send_to_char("Tier 2(tier)\n\r",ch);
        if (ch->tier == 2)
        send_to_char("Tier 3(tier)\n\r",ch);
        if (ch->tier == 3)
        send_to_char("Tier 4(tier)\n\r",ch);
        if (ch->tier == 4)
        send_to_char("Tier 5(tier)\n\r",ch);
        if (ch->tier == 5)
        send_to_char("Tier 6(tier)\n\r",ch);
        if (ch->tier == 6)
        send_to_char("Tier 7(tier)\n\r",ch);

	if (ch->level >= 200)
	{
          if (ch->pcdata->damreduct > 50 || (ch->pcdata->damreduct == 50 && ch->pcdata->damreductdec > 0))
          {
            sprintf(buf, "\nToughness - 1 million exp to lower your damage taken to %d.%d%%.\n\r",
       ch->pcdata->damreductdec == 0 ? ch->pcdata->damreduct - 1 : ch->pcdata->damreduct,
        ch->pcdata->damreductdec == 0 ? 9 : ch->pcdata->damreductdec - 1);
        stc(buf,ch);
          }
	}

        send_to_char("\nChoose your current Focus\n\r",ch);
        send_to_char("Combat Power, Combat Defense, Combat Ability, Combat Toughness\n\r",ch);
        send_to_char("Magic Power, Magic Ability, Magic Defense\n\r",ch);

/*        if (buf[strlen (buf) - 1] != ':')
        {
            strcat (buf, ".\n\r");
            send_to_char (buf, ch);
        }
        else
        {
            act ("You have nothing left to train, you $T!",
                 ch, NULL,
                 ch->sex == SEX_MALE ? "big stud" :
                 ch->sex == SEX_FEMALE ? "hot babe" : "wild thing", TO_CHAR);
        }
*/
        return;
    }


    if (!str_cmp ("tier", arg1))
    {
    int tierdif = 0;

    if (ch->tier == 0)
    {
	if (ch->train < 100) 
	{
	tierdif = 100 - ch->train;
	sprintf (buf, "You need %d more training sessions.\n\r", tierdif);	
	send_to_char(buf,ch);
        return;
	}
	if (ch->max_hit < 100000)
	{
        tierdif = 100000 - ch->max_hit;
        sprintf (buf, "You need %d more Maximum HP.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
	if (ch->exp < 10000000)
        {
        tierdif = 10000000 - ch->exp;
        sprintf (buf, "You need %d more Experiance.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
	if (ch->status < 1)
        {
        tierdif = 1 - ch->status;
        sprintf (buf, "You need %d more Status.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }

	ch->train -= 100;
	ch->exp -= 100000;
	ch->tier++;
	sprintf (buf, "You are now Tier %d.\n\r", ch->tier);
        send_to_char(buf,ch);
	return;
     }
    else if (ch->tier == 1)
    {
        if (ch->train < 100)
        {
        tierdif = 100 - ch->train;
        sprintf (buf, "You need %d more training sessions.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->max_hit < 200000)
        {
        tierdif = 200000 - ch->max_hit;
        sprintf (buf, "You need %d more Maximum HP.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }

        if (ch->exp < 10000000)
        {
        tierdif = 10000000 - ch->exp;
        sprintf (buf, "You need %d more Experiance.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->status < 2)
        {
        tierdif = 2 - ch->status;
        sprintf (buf, "You need %d more Status.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }

        ch->train -= 100;
        ch->exp -= 100000;
        ch->tier++;
        sprintf (buf, "You are now Tier %d.\n\r", ch->tier);
        send_to_char(buf,ch);
        return;
     }
    else if (ch->tier == 2)
    {
        if (ch->train < 100)
        {
        tierdif = 100 - ch->train;
        sprintf (buf, "You need %d more training sessions.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->max_hit < 300000)
        {
        tierdif = 300000 - ch->max_hit;
        sprintf (buf, "You need %d more Maximum HP.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->exp < 10000000)
        {
        tierdif = 10000000 - ch->exp;
        sprintf (buf, "You need %d more Experiance.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->status < 3)
        {
        tierdif = 3 - ch->status;
        sprintf (buf, "You need %d more Status.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }

        ch->train -= 100;
        ch->exp -= 100000;
        ch->tier++;
        sprintf (buf, "You are now Tier %d.\n\r", ch->tier);
        send_to_char(buf,ch);
        return;
     }
    else if (ch->tier == 3)
    {
        if (ch->train < 100)
        {
        tierdif = 100 - ch->train;
        sprintf (buf, "You need %d more training sessions.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->max_hit < 400000)
        {
        tierdif = 400000 - ch->max_hit;
        sprintf (buf, "You need %d more Maximum HP.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->exp < 10000000)
        {
        tierdif = 10000000 - ch->exp;
        sprintf (buf, "You need %d more Experiance.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->status < 4)
        {
        tierdif = 4 - ch->status;
        sprintf (buf, "You need %d more Status.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }

        ch->train -= 100;
        ch->exp -= 100000;
        ch->tier++;
        sprintf (buf, "You are now Tier %d.\n\r", ch->tier);
        send_to_char(buf,ch);
        return;
     }
    else if (ch->tier == 4)
    {
        if (ch->train < 100)
        {
        tierdif = 100 - ch->train;
        sprintf (buf, "You need %d more training sessions.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->max_hit < 500000)
        {
        tierdif = 500000 - ch->max_hit;
        sprintf (buf, "You need %d more Maximum HP.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->exp < 10000000)
        {
        tierdif = 10000000 - ch->exp;
        sprintf (buf, "You need %d more Experiance.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->status < 5)
        {
        tierdif = 5 - ch->status;
        sprintf (buf, "You need %d more Status.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }

        ch->train -= 100;
        ch->exp -= 100000;
        ch->tier++;
        sprintf (buf, "You are now Tier %d.\n\r", ch->tier);
        send_to_char(buf,ch);
        return;
     }
    else if (ch->tier == 5)
    {
        if (ch->train < 100)
        {
        tierdif = 100 - ch->train;
        sprintf (buf, "You need %d more training sessions.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->max_hit < 550000)
        {
        tierdif = 550000 - ch->max_hit;
        sprintf (buf, "You need %d more Maximum HP.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->exp < 10000000)
        {
        tierdif = 10000000 - ch->exp;
        sprintf (buf, "You need %d more Experiance.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->status < 6)
        {
        tierdif = 6 - ch->status;
        sprintf (buf, "You need %d more Status.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }

        ch->train -= 100;
        ch->exp -= 100000;
        ch->tier++;
        sprintf (buf, "You are now Tier %d.\n\r", ch->tier);
        send_to_char(buf,ch);
        return;
     }
    else if (ch->tier == 6)
    {
        if (ch->train < 100)
        {
        tierdif = 100 - ch->train;
        sprintf (buf, "You need %d more training sessions.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->max_hit < 600000)
        {
        tierdif = 600000 - ch->max_hit;
        sprintf (buf, "You need %d more Maximum HP.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->exp < 10000000)
        {
        tierdif = 10000000 - ch->exp;
        sprintf (buf, "You need %d more Experiance.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }
        if (ch->status < 7)
        {
        tierdif = 7 - ch->status;
        sprintf (buf, "You need %d more Status.\n\r", tierdif);
        send_to_char(buf,ch);
        return;
        }

        ch->train -= 100;
        ch->exp -= 100000;
        ch->tier++;
        sprintf (buf, "You are now Tier %d.\n\r", ch->tier);
        send_to_char(buf,ch);
        return;
     }

	else
	{
        send_to_char("You are already Tier 7",ch);
	return;
	}
return;



}


    if (!str_cmp ("hp", arg1))
    {
        if (cost > ch->train)
        {
            send_to_char ("You don't have enough training sessions.\n\r", ch);
            return;
        }

        ch->train -= cost;
        ch->pcdata->perm_hit += 10;
        ch->max_hit += 10;
        ch->hit += 10;
        act ("Your durability increases!", ch, NULL, NULL, TO_CHAR);
        act ("$n's durability increases!", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (!str_cmp ("mana", arg1))
    {
        if (cost > ch->train)
        {
            send_to_char ("You don't have enough training sessions.\n\r", ch);
            return;
        }

        ch->train -= cost;
        ch->pcdata->perm_mana += 10;
        ch->max_mana += 10;
        ch->mana += 10;
        act ("Your power increases!", ch, NULL, NULL, TO_CHAR);
        act ("$n's power increases!", ch, NULL, NULL, TO_ROOM);
        return;
    }

 if (!str_cmp("toughness", arg1))
{
  if (IS_NPC(ch))
    return;
  if (ch->pcdata->damreduct < 50 || (ch->pcdata->damreduct == 50 && ch->pcdata->damreductdec <= 0))
  {
    send_to_char("You may not become any tougher.\n\r",ch);
    return;
  }
  if (ch->exp < 100000)
  {
    send_to_char("It cost 100,000 exp per .1% of toughness.\n\r",ch);
    return;
  }
  ch->exp -= 100000;
  ch->pcdata->damreductdec -= 1;
  if (ch->pcdata->damreductdec < 0)
  {
    ch->pcdata->damreductdec = 9;
    ch->pcdata->damreduct -= 1;
  }
  send_to_char("You have gained a bit of stamina.\n\r",ch);
  return;
}


    if (ch->perm_stat[stat] >= get_max_train (ch, stat))
    {
        act ("Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR);
        return;
    }

    if (cost > ch->train)
    {
        send_to_char ("You don't have enough training sessions.\n\r", ch);
        return;
    }

    ch->train -= cost;

    ch->perm_stat[stat] += 1;
    act ("Your $T increases!", ch, NULL, pOutput, TO_CHAR);
    act ("$n's $T increases!", ch, NULL, pOutput, TO_ROOM);
    return;
}
