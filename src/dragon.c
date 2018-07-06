#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "dragon.h"



void do_fly( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
      CHAR_DATA *victim;
//      CHAR_DATA *mount;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	 {
	  send_to_char( "Fly to whom?\n\r", ch );
	  return;
	 }

	if ( ch->position == POS_FIGHTING )
	 {
	  send_to_char( "No way! You are fighting.\n\r", ch );
	  return;
	 }

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	 {
	  send_to_char( "Nobody by that name.\n\r", ch );
 	  return;
	 }

	if( victim == ch )
	 {
	  stc( "Not to yourself.\n\r",ch);
	  return;
	 }

	act( "You disappear up into the sky.", ch, NULL, NULL, TO_CHAR );
	act( "$n disappears up into the sky.", ch, NULL, NULL, TO_ROOM );
	char_from_room( ch );
	char_to_room( ch, get_room_index(victim->in_room->vnum) );
	act( "You swoop down and land infront of $N", ch, NULL, victim, TO_CHAR );
	act( "$n swoop down and land infront of $N.", ch, NULL, victim, TO_ROOM );
	do_look( ch, "auto" );
	WAIT_STATE(ch,4);
	return;
}


void do_drackform( CHAR_DATA *ch, char *argument)
{
	 char buf [MAX_STRING_LENGTH];
//	 char buf2 [MAX_STRING_LENGTH];
	 char AGE_STRING [MAX_STRING_LENGTH];

	 if (IS_NPC(ch)) return;
	 if (ch->race < 30 || ch->race > 39)
	 {
		send_to_char( "You must be a dragon?\n\r",ch);
		return;
	 }
	 if (IS_POLYAFF(ch, POLY_DRAGON))
	 {
		  REMOVE_BIT(ch->polyaff, POLY_DRAGON);
		  REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
		  act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
		  act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM );
                  ch->pcdata->powers[10] = ch->pcdata->powers[DRAGON_COLOR];
		  ch->damroll -= 1000;
		  ch->hitroll -= 1000;
		  ch->armor[0] += 400;
                  ch->armor[2] += 400;
                  ch->armor[3] += 400;
                  ch->armor[1] += 400;
                  free_string( ch->morph );
		  ch->morph = str_dup( "" );
		  return;
	 }
              if (ch->pcdata->powers[dragonage] <= 1  ) sprintf(AGE_STRING,"Hatchling");
         else if (ch->pcdata->powers[dragonage] == 2  ) sprintf(AGE_STRING,"Very Young");
         else if (ch->pcdata->powers[dragonage] == 3  ) sprintf(AGE_STRING,"Young");
         else if (ch->pcdata->powers[dragonage] == 4  ) sprintf(AGE_STRING,"Juvenile");
         else if (ch->pcdata->powers[dragonage] == 5  ) sprintf(AGE_STRING,"Young Adult");
         else if (ch->pcdata->powers[dragonage] == 6  ) sprintf(AGE_STRING,"Adult");
         else if (ch->pcdata->powers[dragonage] == 7  ) sprintf(AGE_STRING,"Mature Adult");
         else if (ch->pcdata->powers[dragonage] == 8  ) sprintf(AGE_STRING,"Old");
         else if (ch->pcdata->powers[dragonage] == 9  ) sprintf(AGE_STRING,"Very Old");
         else if (ch->pcdata->powers[dragonage] == 10 ) sprintf(AGE_STRING,"Elder");
         else if (ch->pcdata->powers[dragonage] == 11 ) sprintf(AGE_STRING,"Venerable");
         else if (ch->pcdata->powers[dragonage] == 12 ) sprintf(AGE_STRING,"Ancient");
         else if (ch->pcdata->powers[dragonage] == 13 ) sprintf(AGE_STRING,"Immortal");
         else return;


               if (ch->race == 30)
		{
			sprintf(buf, "%s the %s Black Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
		}
		if (ch->race == 31)
		{
			sprintf(buf, "%s the %s Blue Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
		}
                if (ch->race == 32)
		{
			sprintf(buf, "%s the %s Green Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
		}
                if (ch->race == 33)
		{
			sprintf(buf, "%s the %s Red Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
		}
                if (ch->race == 34)
		{		
			sprintf(buf, "%s the %s White Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
		}
                if (ch->race == 35)
                {
			sprintf(buf, "%s the %s Brass Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
                }
                if (ch->race == 36)
                {
			sprintf(buf, "%s the %s Gold Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
                }
                if (ch->race == 37)
                {
			sprintf(buf, "%s the %s Silver Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
                }
                if (ch->race == 38)
                {
			sprintf(buf, "%s the %s Bronze Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
                }
                if (ch->race == 39)
                {
			sprintf(buf, "%s the %s Copper Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
		}
	 act( "You transform into a huge dragon.", ch, NULL, NULL, TO_CHAR );
	 act( "$n's body grows and distorts into a huge dragon.", ch, NULL, NULL, TO_ROOM );
	 SET_BIT(ch->polyaff, POLY_DRAGON);
	 SET_BIT(ch->affected_by, AFF_POLYMORPH);

	 ch->damroll += 1000;
	 ch->hitroll += 1000;
         ch->armor[1] -= 400;
         ch->armor[2] -= 400;
         ch->armor[3] -= 400;
         ch->armor[0] -= 400;
	 return;
}

