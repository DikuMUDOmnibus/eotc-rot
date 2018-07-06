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
#include "globals.h"


void do_nowaitquesting(CHAR_DATA *ch, char *argument)
{
    char arg[MIL];
    argument = one_argument ( argument, arg );
    if ( ( arg[0] == '\0' ) )
    {
        if ( !no_wait_questing )
        {
            send_to_char ( "Syntax: nowaitquesting <on|off>.\n\r", ch );
            return;
        }
    }

    if ( !str_cmp ( arg, "on" ) )
    {
        if ( no_wait_questing )
        {
            send_to_char ( "No wait questing is already in affect!\n\r", ch );
            return;
        }
        no_wait_questing = TRUE;
        send_to_char ( "No wait questing is now in affect!\n\r", ch );
        do_info ( ch, "has activated no wait questing" );
        return;
    }
    if ( !str_cmp ( arg, "off" ) )
    {
        if ( !no_wait_questing )
        {
            send_to_char ( "No wait questing is already off!.\n\r", ch );
            return;
        }
        no_wait_questing = FALSE;
        send_to_char ( "No wait questing has been turned off!\n\r", ch );
        do_info ( ch, "has turned off no wait questing" );
        return;
    }
}


void do_reimb(CHAR_DATA *ch, char *argument)
{
        char buf[MAX_STRING_LENGTH];
        CHAR_DATA       *vch;
        char            arg[MAX_INPUT_LENGTH];
        char            arg2[MAX_INPUT_LENGTH];
        int v;


        argument = one_argument(argument, arg);




        if (arg[0] == '\0') {
                send_to_char("Reimburse <target> <xp|gold|qp|trains|pracs> <amount>\n\r", ch);
                return;
        }


        if ((vch = get_char_world(ch, arg)) == NULL) {
                send_to_char("They aren't logged on.\n\r", ch);
                return;
        }
        if (IS_NPC(vch)) {
                send_to_char("That is a mob.\n\r", ch);
                return;
        }
        argument = one_argument(argument, arg);
        argument = one_argument(argument, arg2);
        v = atoi(arg2);
        if (arg[0] == '\0' || arg2[0] == '\0'
          || (!is_number(arg2) && v >= 0)) {
                do_reimb(ch, "");
                return;
        }
        if (!str_cmp(arg, "xp")) 
				{
                vch->exp += v;
        }
	else if (!str_cmp(arg, "gold")) 
	{
                vch->gold += v;
        }
        else if (!str_cmp(arg, "platinum"))
        {
		vch->platinum += v;
	}
        else if (!str_cmp(arg, "qp"))
        {
                vch->questpoints += v;
               	vch->pcdata->questpoints += v;
        }
       else if (!str_cmp(arg, "pracs"))
        {
                vch->practice += v;
        }
       else if (!str_cmp(arg, "trains"))
        {
                vch->train += v;
        }


				else 
				{
                send_to_char("Please specify XP, gold, qp, trains or pracs.\n\r", ch);
                return;
        }
        if (vch->mkill < 5) {
                vch->mkill = 5;
                do_autosave(ch, "");
        }
        sprintf(arg2, "%s reimbursed %d %s.\n\r", vch->name, v, arg);
        send_to_char(arg2, ch);
        sprintf(arg2, "%s has reimbursed you %d %s.\n\r", ch->name, v, arg);
        send_to_char(arg2, vch);
}



/* Coded by Hutoshi */
void do_cprime(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];

  if(IS_NPC(ch)) return;

	do_force(ch, "all wake");
	do_force(ch, "all stand");
	do_force(ch, "all call all");
	do_restore(ch, "all");
	do_force(ch, "all save");
	sprintf(buf, "%s is about to rebuild your reality, prepare for a copyover.",ch->name);
	do_info(ch, buf);
	stc("{y[{RWorld prepared for copyover.{y]{x\n\r",ch);
}

/*
 * This simple little function will return a display for num that includes
 * leading zeros.  I used it for score, and figured someone else might want
 * it.  It also colors the zeros and the number itself independently.
 *                                                              -- Midboss
 */
char * meter (long num, int digits, char col1, char col2)
{
	static char buf[16];

	//Initialize the buffer.
	sprintf (buf, "{%c", col1);

	//Repetitive, but it works.
	if (digits >= 10 && num < 100000000)
		strcat (buf, "0");
	if (digits >= 9 && num < 100000000)
		strcat (buf, "0");
	if (digits >= 8 && num < 10000000)
		strcat (buf, "0");
	if (digits >= 7 && num < 1000000)
		strcat (buf, "0");
	if (digits >= 6 && num < 100000)
		strcat (buf, "0");
	if (digits >= 5 && num < 10000)
		strcat (buf, "0");
	if (digits >= 4 && num < 1000)
		strcat (buf, "0");
	if (digits >= 3 && num < 100)
		strcat (buf, "0");
	if (digits >= 2 && num < 10)
		strcat (buf, "0");

	//Finish it off...
	sprintf (strlen(buf) + buf, "{%c%ld", col2, num);

	//Done.
	return buf;
}

void do_gift( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if ( ch->exp < 500 ) {send_to_char("It costs 500 exp to make a gift of an item.\n\r",ch); return;}

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Make a gift of which object to whom?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry(ch, arg1, ch) ) == NULL )
	{send_to_char("You are not carrying that item.\n\r",ch); return;}
    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{send_to_char("Nobody here by that name.\n\r",ch); return;}

    if ( IS_NPC(victim) )
    {
	send_to_char("Not on NPC's.\n\r",ch);
	return;
    }
    if ( obj->questowner == NULL || strlen(obj->questowner) < 2 )
    {
	send_to_char("That item has not yet been claimed.\n\r",ch);
	return;
    }
    if (str_cmp(ch->name,obj->questowner))
    {
	send_to_char("But you don't own it!\n\r",ch);
	return;
    }

    ch->exp -= 500;
    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(victim->name);
    obj->ownerid = victim->pcdata->playerid;
    act("You grant ownership of $p to $N.",ch,obj,victim,TO_CHAR);
    act("$n grants ownership of $p to $N.",ch,obj,victim,TO_NOTVICT);
    act("$n grants ownership of $p to you.",ch,obj,victim,TO_VICT);
    return;
}


void do_claim(CHAR_DATA * ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;

  one_argument(argument, arg);

  if (IS_NPC(ch))
  {
    send_to_char("Not while switched.\n\r", ch);
    return;
  }

  if (ch->exp < 500)
  {
    send_to_char("It costs 500 exp to claim ownership of an item.\n\r", ch);
    return;
  }

  if (arg[0] == '\0')
  {
    send_to_char("What object do you wish to claim ownership of?\n\r", ch);
    return;
  }
  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You are not carrying that item.\n\r", ch);
    return;
  }

  if (obj->ownerid != 0)
  {
    if (obj->ownerid == ch->pcdata->playerid)
      send_to_char("But you already own it.\n\r", ch);
    else
      send_to_char("Someone else already own it.\n\r", ch);
    return;
  }
  if (obj->questowner != NULL && strlen(obj->questowner) > 1)
  {
    if (!str_cmp(ch->name, obj->questowner))
    {
      if (obj->ownerid != 0)
        send_to_char("But you already own it!\n\r", ch);
      else
      {
        send_to_char("You {Creclaim{x the object.\n\r", ch);
        obj->ownerid = ch->pcdata->playerid;
      }
    }
    else
      send_to_char("Someone else has already claimed ownership to it.\n\r", ch);
    return;
  }

  ch->exp -= 500;
  if (obj->questowner != NULL)
    free_string(obj->questowner);
  obj->questowner = str_dup(ch->name);
  obj->ownerid = ch->pcdata->playerid;
  act("You are now the owner of $p.", ch, obj, NULL, TO_CHAR);
  act("$n is now the owner of $p.", ch, obj, NULL, TO_ROOM);
  return;
}



CH_CMD ( do_autoreboot )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];

    argument = one_argument ( argument, arg1 );
    argument = one_argument ( argument, arg2 );

    if ( IS_IMMORTAL ( ch ) && !IS_NPC ( ch ) )
    {
        if ( is_reboot_countdown == TRUE )
        {
            send_to_char
                ( "There is already a reboot countdown in progress.\n\rType ABORT to abort.\n\r",
                  ch );
            sprintf ( buf, "Current countdown has %d minute(s) remaining.\n\r",
                      ( reboot_countdown - 1 ) );
            send_to_char ( buf, ch );
            return;
        }
        if ( arg1[0] == '\0' )
        {
            send_to_char
                ( "Reboot in 5 minutes.\n\r( default if no time specified )\n\r\n\rType ABORT to abort.\n\r",
                  ch );
            reboot_countdown = 6;
            is_reboot_countdown = TRUE;
        }
        else
        {
            sprintf ( buf,
                      "\n\rReboot in %d minutes\n\r\n\rType abort to abort.\n\r",
                      atoi ( arg1 ) );
            send_to_char ( buf, ch );
            reboot_countdown = ( atoi ( arg1 ) + 1 );
            is_reboot_countdown = TRUE;
        }
        if ( !str_cmp ( arg2, "silent" ) )
        {
            send_to_char ( "Reboot countdown started silently.\n\r", ch );
        }
        else
        {
            if ( reboot_countdown > 6 )
            {
                sprintf ( buf,
                          "\n\r{R[{Wreboot{R]{C Reboot countdown started, {R%d{C minutes remaining.{x\n\r{R[{WReboot{R] {CPlease see '{chelp reboot{C' for more information.{x\n\r",
                          ( reboot_countdown - 1 ) );
                do_gmessage ( buf );
            }
        }

    }
}

CH_CMD ( do_abort )
{
    if ( IS_IMMORTAL ( ch ) )
    {

        if ( is_reboot_countdown == FALSE )
        {
            send_to_char ( "There is no reboot countdown to abort.\n\r", ch );
            return;
        }
        if ( str_cmp ( argument, "silent" ) )
        {
            do_info ( ch, "has aborted the reboot.\n\r" );
        }
        else
        {
            send_to_char ( "The reboot was aborted silently.\n\r", ch );
        }
        reboot_countdown = -1;
        is_reboot_countdown = FALSE;
    }

}

