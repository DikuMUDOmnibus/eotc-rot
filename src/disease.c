/* Xrakisis's Lazy mans Cotn Quicker Quickmud */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "disease.h"


void do_diseaseself(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char      arg1 [MAX_INPUT_LENGTH];
  argument = one_argument( argument, arg1 );


  if (IS_NPC(ch))return;

/*  if (!IS_NPC(ch) && ch->disease != 0)
  {   
    send_to_char("You already have a disease.\n\r",ch);
    return;
  }
*/
if ( arg1[0] == '\0' )
{
stc("Type diseaseself <vampire or werewolf>\n\r",ch);
stc("{G************************************************{x\n\r",ch);
stc("{Y--{CVampire       {Y--{CWerewolf\n\r",ch);
stc("{Y--{CDemon         {Y--{CUndead Knight\n\r",ch);
stc("{G************************************************{x\n\r",ch);
return;
}

if (!str_cmp(arg1,"werewolf"))
{
ch->disease = DISEASE_WEREWOLF;
SET_BIT(ch->vuln_flags, VULN_SILVER);
send_to_char( "You have Become a {GWerewolf{x.\n\r", ch);
sprintf(buf,"%s Has become a Werewolf.", ch->name);
do_info(ch,buf);
return;
}
if (!str_cmp(arg1,"vampire"))
{
ch->disease = DISEASE_VAMPIRE;
SET_BIT(ch->vuln_flags, VULN_HOLY);
send_to_char( "You have Become a {GVampire{x.\n\r", ch);
sprintf(buf,"%s Has become a Vampire.", ch->name);
do_info(ch,buf);
return;
}
if (!str_cmp(arg1,"undead"))
{
ch->disease = DISEASE_UNDEAD;
SET_BIT(ch->vuln_flags, VULN_HOLY);
send_to_char( "You have Become a {GDeath Knight{x.\n\r", ch);
sprintf(buf,"%s Has become a Death Knight.", ch->name);
do_info(ch,buf);
return;
}
if (!str_cmp(arg1,"demon"))
{
ch->disease = DISEASE_DEMON;
SET_BIT(ch->vuln_flags, VULN_HOLY);
send_to_char( "You have Become a {GDemon{x.\n\r", ch);
sprintf(buf,"%s Has become a Demon.", ch->name);
do_info(ch,buf);
return;
}
  else do_diseaseself(ch,"");   
  return;
}

void do_leech(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim = NULL;
    char buf[MAX_STRING_LENGTH];

        if (ch->disease != DISEASE_DEMON)
        {
                stc("Huh?\n\r", ch);
                return;
        }

    if (argument[0] == '\0')
    {
	stc("Who's life do you wish to leech off of?\n\r", ch);
	return;
    }

    if ((victim = get_char_room(ch, argument)) == NULL)
    {
	    stc("They aren't here.\n\r", ch);
	    return;
    }

      if (is_safe(ch, victim)) return;

	WAIT_STATE(ch, 10);

	act("$n stares intently at $N.", ch, NULL, victim, TO_NOTVICT);
	act("You stare intently at $N.", ch, NULL, victim, TO_CHAR);
	act("$n stares intently at you.", ch, NULL, victim, TO_VICT);

      if (victim->hit >= 1000)
      {
	  sh_int power;
	  sh_int dam;
	  power = 20;
	  dam = number_range (5000, 9000);
	  ch->hit += dam/2;
	  if (ch->hit >= ch->max_hit + 500)
		ch->hit = ch->max_hit + 500;
 		damage (ch, victim, dam, 0, DAM_NEGATIVE, FALSE);
	  sprintf(buf,"#1You absorb %d hitpoints.#n\n\r",dam);
	  stc(buf,ch);
	  sprintf(buf,"#1%s absorbed %d of your hitpoints!#n\n\r",ch->name,dam);
	  stc(buf,victim);
	  }
      else stc("Nothing seemed to happen.\n\r",ch);
	return;

}


void do_scream(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);

        if (IS_NPC(ch))
                return;

        if (ch->disease != DISEASE_VAMPIRE)
        {
                stc("Huh?\n\r", ch);
                return;
        }


        if (IS_SET(ch->in_room->room_flags, ROOM_SAFE))
        {
                stc("Your screams will have no effect here.\n\r", ch);
                return;
        }

        if (ch->blood < 50)
        {
                stc("You have insufficient blood.\n\r", ch);
                return;
        }

        for (vch = char_list; vch != NULL; vch = vch_next)
        {
                vch_next = vch->next;

                if (vch->in_room == NULL)
                        continue;
                if (ch == vch)
                        continue;
                if (vch->in_room == ch->in_room)
                {
                        if (can_see(ch, vch))
                        {
                                if (number_range(1,2) == 2)
                                {
                                        act("$n lets out an ear-popping scream!", ch, NULL, vch, TO_ROOM);
                                        stc("You fall to the ground, clutching your ears.\n\r", vch);
                                        vch->position = POS_STUNNED;
                                        act("$n falls to the ground, stunned.", vch, NULL, vch, TO_NOTVICT);
                                        return;
                                }
                                else
                                {
                                        act("$n lets out an ear-popping scream!", ch, NULL, vch, TO_ROOM);
                                        return;
                                }
                        }
                        else
                                continue;

                        return;
                }
        }

        return;
}

void do_conceal(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_STRING_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;

        one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        if (ch->disease != DISEASE_VAMPIRE)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }
        if (arg[0] == '\0')
        {
                send_to_char("Syntax: Conceal (item).\n\r", ch);
                return;
        }
        if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
        {
                send_to_char("You dont have that item.\n\r", ch);
                return;
        }
        if (IS_SET(obj->extra_flags, ITEM_INVIS))
        {
                sprintf(buf, "%s fades into existance.", obj->short_descr);
                send_to_char(buf, ch);
                act(buf, ch, NULL, NULL, TO_ROOM);
                REMOVE_BIT(obj->extra_flags, ITEM_INVIS);
                return;
        }
        if (!IS_SET(obj->extra_flags, ITEM_INVIS))
        {
                sprintf(buf, "%s fades out of existance.", obj->short_descr);
                send_to_char(buf, ch);
                act(buf, ch, NULL, NULL, TO_ROOM);
                SET_BIT(obj->extra_flags, ITEM_INVIS);
                return;
        }

        return;
}

void do_fear(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        char      arg[MAX_INPUT_LENGTH];
        char      buf[MAX_STRING_LENGTH];

        argument = one_argument(argument, arg);

        if (IS_NPC(ch))
                return;

        if (ch->disease != DISEASE_VAMPIRE)
        {
                stc("Huh?\n\r", ch);
                return;
        }


        if ((victim = get_char_room(ch, arg)) == NULL)
        {
                stc("They aren't here.\n\r", ch);
                return;
        }

        arg[0] = UPPER(arg[0]);

        if (ch->fighting == NULL)
        {
                stc("You must be fighting to use Fear.\n\r", ch);
                return;
        }

        if (victim->fighting == NULL)
        {
                sprintf(buf, "%s is not fighting anyone.", arg);
                stc(buf, ch);
                return;
        }

        WAIT_STATE(ch, 8);

        if (IS_NPC(victim))
        {
                act("You bare your fangs and growl at $N.", ch, NULL, victim,
                    TO_CHAR);
                act("$n bares $s fangs and growls at you.", ch, NULL, victim,
                    TO_VICT);
                act("$n bares $s fangs and growls at $N, and $N flees in terror.", ch, NULL, victim, TO_NOTVICT);
                do_flee(victim, "");
                return;
        }

        if (!IS_NPC(victim))
        {
                        if (number_range(1, 6) == 2)
                        {
                                act("You bare your fangs and growl at $N, but nothing happens.", ch, NULL, victim, TO_CHAR);
                                act("$n bares $s fangs and growls at you.",
                                    ch, NULL, victim, TO_VICT);
                                act("$n bares $s fangs and growls at $N, but nothing happens.", ch, NULL, victim, TO_NOTVICT);
                                return;
                        }

                act("You bare your fangs and growl at $N.", ch, NULL, victim,
                    TO_CHAR);
                act("$n bares $s fangs and growls at you.", ch, NULL, victim,
                    TO_VICT);
                act("$n bares $s fangs and growls at $N, and $N flees in terror.", ch, NULL, victim, TO_NOTVICT);
                do_flee(victim, "");
		WAIT_STATE(victim, 8);
                return;
        }
	WAIT_STATE(ch, 10);

        return;

}

void do_moonbeam(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        char      arg1[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg1);

        if (IS_NPC(ch))
                return;

        if (ch->disease != DISEASE_WEREWOLF)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }


        if (ch->mana < 500)
        {
                send_to_char("You do not have enough energy to summon a Moonbeam.\n\r", ch);
                return;
        }

        if (arg1[0] == '\0')
        {
                send_to_char("Who do you wish to strike?\n\r", ch);
                return;
        }

        if ((victim = get_char_room(ch, arg1)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (ch == victim)
                return;
        if (is_safe(ch, victim) == TRUE)
                return;
        if (ch->fighting == NULL)
                set_fighting(ch, victim);
        update_pos(victim);

        act("$N is struck by a huge moonbeam.", ch, NULL, victim, TO_CHAR);
        act("$N is struck by a deadly beam of moonlight from $n.", ch, NULL,
            victim, TO_NOTVICT);
        act("You are struck by a deadly beam of moonlight!", ch, NULL, victim,
            TO_VICT);
        if (IS_GOOD(victim)) victim->hit -= 500;
        if (IS_EVIL(victim)) victim->hit -= 1000;
        if (IS_NEUTRAL(victim)) victim->hit -= 750;
        ch->mana -= 500;
        WAIT_STATE(ch, 12);
        return;

}

void do_roar(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        char      buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
                return;
        if (ch->disease != DISEASE_WEREWOLF)
        {
                send_to_char
                        ("You let out a violent roar\n\r",
                         ch);
                return;
        }
        if ((victim = ch->fighting) == NULL)
        {
                send_to_char("You arent fighting anyone.\n\r", ch);
                return;
        }

        if (number_range(1, 6) == 2)
        {
                sprintf(buf,
                        "%s roars and screams, intimidating the hell out of %s.",
                        ch->name, victim->name);
                act(buf, ch, NULL, NULL, TO_ROOM);
                sprintf(buf,
                        "You roar and scream, intimidating the hell out of %s.\n\r",
                        victim->name);
                send_to_char(buf, ch);
                do_flee(victim, "");
                WAIT_STATE(ch, 18);
                return;
        }
        else
        {
                stc("You roar loudly.\n\r", ch);
                stc("Nothing happens.\n\r", ch);
                act("$n roars loudly.", ch, NULL, NULL, TO_ROOM);
                WAIT_STATE(ch, 12);
                return;
        }
}

void do_soulsuck(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (ch->disease == DISEASE_UNDEAD)
  {
    send_to_char("You dont get this power.\n\r",ch);
    return;
  }
  if (ch->alignment > 0)
  {
    send_to_char("You simply can't get yourself to do this, it's way to evil.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  dam = number_range(5000,6600);

  sprintf(buf,"You suck the soul out of $N and use the energy to heal yourself [%d]",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n sucks at your soul, damn it's nasty, ooo, hurts to [%d]",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"$n looks at $N and grins.");
  act(buf,ch,NULL,victim,TO_NOTVICT);
  ch->hit += dam;
  victim->hit -= dam;
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  if (victim->position > POS_STUNNED)
  {
    if (victim->fighting == NULL) set_fighting(victim, ch);
    if (ch->fighting == NULL) set_fighting(ch, victim);
  }
 	damage (ch, victim, dam, 0, DAM_NEGATIVE, TRUE);
  WAIT_STATE(ch,10);
  return;
}