/****************************************************
 *	Clan system implemented and fixed by Jeremias	*
 *	This source is free of any bug !!!				*
 ****************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/timeb.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "clan.h"

DECLARE_DO_FUN(do_help		);

/* for clans */
/* 
 * CLAN NAME   CLAN NUMBER 
 * Independent  clan = 0
 * Loner        clan = 1
 * Outcast      clan = 2
 * Crusader     clan = 3
 * Lycan        clan = 4
 * Death        clan = 5
 * Coven        clan = 6
 * Syndicate    clan = 7
 * Light        clan = 8
 * Pantheon     clan = 9
 */
// const struct clan_type clan_table[MAX_CLAN] =
struct clan_type clan_table[MAX_CLAN] =
{
    /* independent should be FALSE if is a real clan */
  /* name,    who entry,    deathroom,        recall,       indep't  min level */
  {"",			"{c[           {c]",	"",		ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, TRUE,		0,0},
  {"Loner",		"{c[{w LONER     {c]",	"Gilean",		ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, TRUE,		6,0},
  {"Outcast",	"{c[  {wOUTCAST{c  ]",	"None",		ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, TRUE,		10,0},
  {"Crusader",	"{c[ {YCRUSADER{c  ]",	"Kiri-Jolith",		ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, FALSE,	 20,0},
  {"Lycan",	"{c[  {W LYCAN   {c]",	"None",		ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, FALSE,	 2,0},
  {"Death",		"{c[  {w DEATH   {c]",	"Chemosh",		ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, FALSE,	20,0},
  {"Coven",		"{c[  {R COVEN   {c]",	"",	ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, FALSE,	20,0},
  {"Syndicate",		"{c[ {RSYNDICATE {c]",	"Takhisis",		ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, FALSE,	20,0},
  {"Light",		"{c[  {w LIGHT   {c]",	"Paladine",		ROOM_VNUM_MORGUE,	ROOM_VNUM_TEMPLE, FALSE,	20,0},
  {"Pantheon",		"{c[ {GP{gA{WN{gT{GH{WE{GO{GN  {c]",	"Sargonnas",		ROOM_VNUM_MORGUE, ROOM_VNUM_TEMPLE, 
FALSE,LEVEL_IMMORTAL,0}
};



const struct clan_titles clan_rank_table[MAX_RANK] =
{
	{"      "},
	{"{rJUNIOR{x"},
	{"{RSENIOR{x"},
	{"{yDEPUTY{x"},
	{"{YSECOND{x"},
	{"{WLEADER{x"}
};

const char *lookup_rank(int cs)
{
	switch (cs)
	{
	case LEADER: return "the Leader";
	case SECOND: return "the Second in command";
	case DEPUTY: return "a Deputy";
	case SENIOR: return "a Senior member";
	case JUNIOR: return "a Junior member";
	}
	return "a member";
}

/* returns clan number */
int clan_lookup (const char *name)
{
	int clan;

	for ( clan = 0; clan < MAX_CLAN ; clan++)
	{
	
		if (LOWER(name[0]) == LOWER(clan_table[clan].name[0])
		&& !str_prefix( name,clan_table[clan].name))
		return clan;
	}

	return INDEP; /* none found return independent */
}

void do_cleader( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int clan;

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	/* Lists all possible clans */
	if (arg1[0] == '\0'|| arg2[0] == '\0')
	{
	send_to_char( "{REotC{w Clan list:{x\n\r",ch);
		/*
		 * Won't print the independent
		 */
		for(clan = 0; clan < MAX_CLAN; clan++)
		{
		if(!clan_table[clan].independent)
			{
			sprintf( buf, "   {G%s{x\n\r", clan_table[clan].name);
			send_to_char( buf, ch);
			}
		}
	
		send_to_char( "\n\rSyntax: {Gcleader {c<{wchar{c> <{wclan name{c>{x\n\r",ch);
		send_to_char( "If {c<{wclan name{c>{x is {r'{wnone{r'{x clan leadership has been removed.\n\r",ch);
		return;
	}

	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	{
		send_to_char( "No character by that name exists.\n\r", ch );
		return;
	}

	if ( IS_NPC(victim))
	{
	send_to_char("You must be mad.\n\r",ch);
	return;
	}

	if(!str_cmp(arg2,"none"))
	{
	if(victim->rank == LEADER)
		{
		sprintf( buf,"You remove the leadership from %s.\n\r",victim->name);
		send_to_char( buf, ch);
		sprintf( buf, "You aren't the leader of {G%s{x clan anymore!\n\r",clan_table[victim->clan].name);
		send_to_char( buf, victim);
		victim->rank = MEMBER; /* make victim a member of the CLAN */
		return;
		}

	else
		{
		sprintf( buf,"%s isn't the leader of any clan.\n\r",victim->name);
		send_to_char( buf, ch);
		return;
		}
	}

	if((clan = clan_lookup(arg2)) == INDEP)
	{
	send_to_char("No such clan exists.\n\r",ch);
	return;
	}

	if(clan_table[clan].independent)
	{
	send_to_char("{GOUTCAST{x and {GLONER{x aren't clans!\n\r",ch);
	return;
	}

	else  /* is a TRUE clan */
	{
	if(victim->rank == LEADER)
		{
		sprintf( buf, "They already hold the leadership of %s clan.\n\r",clan_table[victim->clan].name);
		send_to_char( buf, ch);
		return;
		}

	if (victim->level < 50)
		{
		sprintf(buf,"%s's level is too low to be leader of %s clan.\n\r"
					"All {GLEADERS{x must be {wlevel {r50{x or higher.\n\r",
		victim->name,clan_table[clan].name);
		send_to_char(buf,ch);
		return;
		}

	else
		{
		sprintf(buf,"%s is now the {GLEADER{x of %s.\n\r",victim->name,clan_table[clan].name);
		send_to_char(buf,ch);
		sprintf(buf,"You are now the {GLEADER{x of %s.\n\r",clan_table[clan].name);
		send_to_char(buf,victim);
		}
	}

	victim->clan = clan; /* make member of the clan */
	victim->rank = LEADER; /* make victim LEADER of the CLAN */
}

void do_clanlist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int clan;

	send_to_char("{c*************************************{x\n\r",ch);
	send_to_char("{c*{w The Clans of {REotC{c                 *{x\n\r",ch);
	send_to_char("{c*************************************{x\n\n\r",ch);
	send_to_char("{c Clan name   Diety   Min level   Treasury{x\n\r",ch);
	send_to_char("{w-------------------------------------{x\n\r",ch);

	for(clan = 0; clan < MAX_CLAN; clan++)
	{
	sprintf( buf, "%-11s %-12s    %-3d     %d\n\r",
	clan == INDEP ? "Independent" : clan_table[clan].name,
	clan_table[clan].god,
	clan_table[clan].min_level, clan_table[clan].gold);
	send_to_char( buf, ch);
	}
	return;
}

void do_clantalk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

	if (!is_clan(ch) || clan_table[ch->clan].independent)
	{
	send_to_char("You aren't in a clan.\n\r",ch);
	return;
	}

	if ( argument[0] == '\0' )
	{
		if (IS_SET(ch->comm,COMM_NOCLAN))
		{
		send_to_char("{RClan{x channel is now {GON{x\n\r",ch);
		REMOVE_BIT(ch->comm,COMM_NOCLAN);
		}

		else
		{
		send_to_char("{RClan{x channel is now {ROFF{x\n\r",ch);
		SET_BIT(ch->comm,COMM_NOCLAN);
		}
	return;
	}

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	return;
	}

	REMOVE_BIT(ch->comm,COMM_NOCLAN);
	sprintf( buf, "{RYou tell the clan '{w%s{R'{x\n\r", argument );
	send_to_char( buf, ch );

	if (ch->rank >= JUNIOR)
	sprintf( buf, "{c[%s{c]{R%s tells the clan '{w%s{R'{x\n\r", clan_rank_table[ch->rank].rank, ch->name, argument);
	
	else
	sprintf( buf, "{R%s tells the clan '{w%s{R'{x\n\r", ch->name, argument);
	
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
		if ( d->connected == CON_PLAYING
			&& d->character != ch
			&& is_same_clan(ch,d->character)
			&& !IS_SET(d->character->comm,COMM_NOCLAN)
			&& !IS_SET(d->character->comm,COMM_QUIET) )
		{
		send_to_char(buf, d->character);
		}
	}
	return;
}

void do_promote( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument( argument, arg1 );

	if ( IS_NPC(ch) )
	{
	send_to_char( "NPC's can not promote someone.\n\r",ch);
	return;
	}

	if ((ch->rank != LEADER) && (!IS_IMMORTAL(ch)))
	{
	send_to_char( "You must be a clan Leader to promote someone.\n\r",ch);
	return;
	}

	if ( arg1[0] == '\0' )
	{
	send_to_char( "Syntax: {Gpromote {c<{wchar{c>{x\n\r",ch);
	return;
	}

	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	{
	send_to_char( "They must be playing to be promoted.\n\r", ch );
	return;
	}

	if ( IS_NPC(victim) || (victim->clan <= OUTCAST))
	{
	send_to_char("You must be mad.\n\r",ch);
	return;
	}

	if ((victim->clan != ch->clan ) && (!IS_IMMORTAL(ch)))
	{
	send_to_char("You can not promote a player who is not in your clan.\n\r",ch);
	return;
	}

	if (ch == victim)
	{
	send_to_char("You can not promote yourself.\n\r",ch);
	return;
	}

	if (victim->rank >= SECOND)
	{
	send_to_char("You can not promote this player anymore.\n\r",ch);
	return;
	}

	victim->rank += 1;

	sprintf( buf, "They are now {G%s{x of the clan.\n\r", lookup_rank(victim->rank));
	send_to_char( buf, ch );
	sprintf( buf, "You are now {G%s{x of the clan.\n\r", lookup_rank(victim->rank));
	send_to_char( buf, victim );
	return;
}

void do_demote( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument( argument, arg1 );

	if ( IS_NPC(ch) )
	{
	send_to_char( "NPC's can not demote someone.\n\r",ch);
	return;
	}

	if ((ch->rank != LEADER) && (!IS_IMMORTAL(ch)))
	{
	send_to_char( "You must be a clan Leader to demote someone.\n\r",ch);
	return;
	}

	if ( arg1[0] == '\0' )
	{
	send_to_char( "Syntax: {Gdemote {c<{wchar{c>{x\n\r",ch);
	return;
	}

	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	{
	send_to_char( "They must be playing to be demoted.\n\r", ch );
	return;
	}

	if ( IS_NPC(victim) || (victim->clan <= OUTCAST))
	{
	send_to_char("You must be mad.\n\r",ch);
	return;
	}

	if ((victim->clan != ch->clan ) && (!IS_IMMORTAL(ch)))
	{
	send_to_char("You can not demote a player who is not in your clan.\n\r",ch);
	return;
	}

	if (ch == victim)
	{
	send_to_char("You can not demote yourself.\n\r",ch);
	return;
	}

	if (victim->rank <= MEMBER)
	{
	send_to_char("You can not demote this player anymore.\n\r",ch);
	return;
	}

	victim->rank -= 1;

	sprintf( buf, "They are now {G%s{x of the clan.\n\r", lookup_rank(victim->rank));
	send_to_char( buf, ch );
	sprintf( buf, "You are now {G%s{x of the clan.\n\r", lookup_rank(victim->rank));
	send_to_char( buf, victim );
	return;
}

void do_exile( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument( argument, arg );

	if ( IS_NPC(ch) )
	{
	send_to_char( "NPC's can not demote someone.\n\r",ch);
	return;
	}

	if(ch->rank != SECOND && ch->rank != LEADER && !IS_IMMORTAL(ch))
	{
	send_to_char( "You must be a clan Leader or Auxiliary to exile someone.\n\r",ch);
	return;
	}

	if ( arg[0] == '\0' )
	{
	send_to_char( "Syntax: {Gexile {c<{wchar{c>{x\n\r",ch);
	return;
	}

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	send_to_char( "They aren't playing.\n\r", ch );
	return;
	}

	if ( IS_NPC(victim) || (victim->clan <= OUTCAST))
	{
	send_to_char("You must be mad.\n\r",ch);
	return;
	}

	if ( (victim->clan != ch->clan ) && (!IS_IMMORTAL(ch) ) )
	{
	send_to_char("You can not exile a player who is not in your clan.\n\r",ch);
	return;
	}

	if (ch == victim)
	{
	send_to_char("You can not exile yourself.\n\r",ch);
	return;
	}

	/* leader outcasts victim */
	send_to_char("They are now {GOUTCAST{x from the clan.\n\r",ch);
	sprintf( buf, "You have been {GOUTCAST{x from %s clan!\n\r", clan_table[victim->clan].name);
	send_to_char( buf, victim);
	send_to_char( "Type {r'{Ghelp outcast{r'{x for more information.\n\r",victim);
	victim->clan = OUTCAST; /* OUTCAST GROUP */
	victim->rank = MEMBER;
	return;
}

void do_loner( CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
	return;

	if ( ch->level < 6 )
	{
	send_to_char("You are still a {GNEWBIE{x, wait until {wlevel {r6{x.\n\r",ch);
	return;
	}

	if (ch->clan == OUTCAST)	/* IS_OUTCAST */
	{
	send_to_char("You are an {GOUTCAST{x!  You can't join a clan."
				"\n\rType {r'{Ghelp outcast{r'{x for more information.\n\r", ch);
	return;
	}

	if (ch->clan == LONER)
	{
	send_to_char("You are already a {GLONER{x.\n\r", ch);
	return;
	}

	if (is_clan(ch))
	{
	send_to_char("You are already in a Clan.\n\r",ch);
	return;
	}

	if (ch->pcdata->confirm_loner)
	{
		if (argument[0] != '\0')
		{
		send_to_char("{GLONER{x status removed.\n\r",ch);
		ch->pcdata->confirm_loner = FALSE;
		return;
		}
	
		else
		{
		send_to_char("{*{wYou are now a brave {GLONER{x!!\n\r",ch);
		ch->clan = LONER;
		ch->rank = MEMBER;
		ch->pcdata->confirm_loner = FALSE;
		return;
		}
	}

	if (argument[0] != '\0')
	{
	send_to_char("Just type {GLONER{x. No argument.\n\r",ch);
	return;
	}
	
	do_help( ch, "pkill" );
	send_to_char("\n\r", ch);
	send_to_char("Type {GLONER{x again to confirm this command.\n\r", ch);
	send_to_char("{RWARNING:{x this command is irreversible.\n\r", ch);
	send_to_char("Typing {GLONER{x with an argument will undo delete status.\n\r", ch);
	ch->pcdata->confirm_loner = TRUE;
}

void do_petition_list(int clan, CHAR_DATA *ch)
{
	DESCRIPTOR_DATA *d;
	bool flag = FALSE;
	char buf[MAX_STRING_LENGTH];

	for (d = descriptor_list; d; d = d->next)
	{
	CHAR_DATA *victim;
	victim = d->original ? d->original : d->character;
	
	if (d->connected == CON_PLAYING)
	{
		if (victim->petition == clan)
		{
			if (!flag)
			{
			flag = TRUE;
			send_to_char("The following characters have petitioned your clan:\n\n\r", ch);
			}

		sprintf(buf,"{c[{w%3d %5s %s{c][{w%-10s%-16s{c] ({w%s{c){x\n\r",
		victim->level,
		victim->race < MAX_PC_RACE ? pc_race_table[victim->race].who_name : "     ",
		class_table[victim->class].who_name,
		victim->name,
		IS_NPC(victim) ? "" : victim->pcdata->title,
		clan_table[victim->clan].name);
		send_to_char(buf,ch);
		}
	}
	}

	if (!flag)
	send_to_char("No-one has petitioned your clan.\n\r", ch);
}

void do_petition(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	short status;
	int clan;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	status = ch->clan ? ch->rank : 0;

	if (arg1[0] == 0)
	{
		if (status >= SECOND)
		{
		do_petition_list(ch->clan, ch);
		return;
		}

		if (!ch->petition)
		{
		send_to_char("Syntax: {Gpetition {c<{wclan name{c>{x.\n\r", ch);
		return;
		}

		if (ch->petition)
		{
		ch->petition = 0;
		send_to_char("You withdraw your petition.\n\r", ch);
		return;
		}
	}

	if (arg2[0] == 0 && status < SECOND)
	{
		if((clan = clan_lookup(arg1)) == INDEP)
		{
		send_to_char("There is no clan by that name.\n\r",ch);
		return;
		}

		if(clan_table[clan].independent)
		{
		send_to_char("{GOUTCAST{x and {GLONER{x aren't clans!\n\r",ch);
		return;
		}

		if (clan == ch->clan)
		{
		sprintf( buf,"You are already a member of {G%s{x clan.\n\r", clan_table[clan].name);
		send_to_char(buf, ch);
		return;
		}

		if (ch->clan == OUTCAST)	/* IS_OUTCAST */
		{
		sprintf(buf,"You are an {GOUTCAST{x!  You can't join a clan."
					"\n\rType {r'{Ghelp outcast{r'{x for more information.\n\r");
		send_to_char( buf, ch);
		return;
		}

		if (ch->clan == INDEP )
		{
		send_to_char("You must be at least a {GLONER{x to join a clan.\n\r", ch);
		return;
		}

		if (ch->level < clan_table[clan].min_level)
		{
		sprintf(buf,"Your level is too low to join {G%s{x.\n\r"
					"The requirement was {wlevel {r%d{x.\n\r",
		clan_table[clan].name,
		clan_table[clan].min_level);
		send_to_char(buf, ch);
		return;
		}

		else
		{
		ch->petition = clan;
		sprintf( buf,"You have petitioned {G%s{x clan for membership.\n\r",
		clan_table[clan].name);
		send_to_char(buf, ch);
		return;
		}
	}

	if (status >= SECOND)
	{
		if (!str_prefix(arg1, "accept"))
		{

			if ((victim = get_char_world(ch, arg2)) == NULL)
			{
			send_to_char("They are not playing.\n\r", ch);
			return;
			}

			if (victim->petition != ch->clan)
			{
			send_to_char("They have not petitioned your clan.\n\r", ch);
			return;
			}

			victim->clan = ch->clan;
			victim->rank = MEMBER;
			victim->petition = 0;
			send_to_char("You have accepted them into your clan.\n\r", ch);
			send_to_char("Your clan application was successful.\n\r", victim);
			sprintf(buf,"You are now a proud member of clan {G%s{x.\n\r",
			clan_table[victim->clan].name);
			send_to_char(buf,victim);
			return;
		}

		else if (!str_prefix(arg1, "reject"))
		{

			if ((victim = get_char_world(ch, arg2)) == NULL)
			{
			send_to_char("They are not playing.\n\r", ch);
			return;
			}

			if (victim->petition != ch->clan)
			{
			send_to_char("They have not petitioned your clan.\n\r", ch);
			return;
			}

			victim->petition = 0;
			send_to_char("You have rejected there application.\n\r", ch);
			send_to_char("Your clan application has been rejected.\n\r", victim);
			return;
		}

	send_to_char( "Syntax: {Gpetition accept {c<{wplayer{c>{x\n\r"
				  "        {Gpetition reject {c<{wplayer{c>{x\n\r", ch);
	return;
	}

send_to_char("Syntax: {Gpetition {c<{wclan name{c>{x.\n\r", ch);
return;
}


void do_cdeposit(CHAR_DATA * ch, char *argument)
{
        //char arg1[MAX_STRING_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        int       amt;
	int	  clan;

        if (IS_NPC(ch))
                return;

        if (ch->clan == 0)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        amt = atoi(argument);
        if (amt < 1)
        {
                send_to_char("Syntax: kdeposit <amt>\n\r", ch);
                return;
        }

        if (amt > ch->gold)
        {
                send_to_char ("My my my, you have philanthropy down to an artform?\n\rYou give away more then you own!\n\r", ch);
                return;
        }

        sprintf(buf, "You deposit %d gold into the clan treasury.\n\r", amt);
        send_to_char(buf, ch);
        sprintf(buf, "%s has deposited %d gold into the clan treasury.", ch->name, amt);
        do_info(ch, buf);
        make_note("Clan", "Clan Clerk", clan_table[ch->clan].name, "Treasury Grows",
                  7, buf);

        ch->gold -= amt;
        clan_table[ch->clan].gold += amt;
// clan_table[clan].name)
// clan_table[clan].min_level)
        save_char_obj(ch);
        save_clan();
}


void load_clan()
{
  FILE *fp;
  int i;

  if ((fp = fopen("../txt/clan.txt", "r")) == NULL)
  {
    log_string("Error: clan.txt not found!");
    exit(1);
  }
//  leader_board.bestpk_name = fread_string(fp);
//  leader_board.bestpk_number = fread_number(fp);

  clan_table[0].gold = 0;


  for (i=0;i<11;i++)
  {
  clan_table[i].gold = fread_number(fp);
  }

  fclose(fp);
}


void save_clan()
{
  FILE *fp;
  int i;

  if ((fp = fopen("../txt/clan.txt","w")) == NULL)
  {
    log_string("Error writing to clan.txt");
    return;
  }
//  fprintf(fp, "%s~\n", leader_board.bestpk_name);
//  fprintf(fp, "%d\n", leader_board.bestpk_number);
  for (i=0;i<11;i++)
  {
   fprintf(fp, "%d\n", clan_table[i].gold);
  }
   fclose (fp);
}


void do_cwithdraw(CHAR_DATA * ch, char *argument)
{
        //char arg1[MAX_STRING_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        int       amt;
        int       clan;

        if (IS_NPC(ch))
                return;

        if (ch->clan == 0)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        amt = atoi(argument);
        if (amt < 1)
        {
                send_to_char("Syntax: cwithdraw <amt>\n\r", ch);
                return;
        }

        if ((ch->rank != LEADER) && (ch->rank != SECOND))
        {
        send_to_char( "You must be a clan Leader or Second in command to withdraw.\n\r",ch);
        return;
        }


        if (amt > clan_table[ch->clan].gold)
        {
                send_to_char ("There isnt that much in the treasury!\n\r", ch);
                return;
        }

        sprintf(buf, "You withdraw %d gold from the clan treasury.\n\r", amt);
        send_to_char(buf, ch);
        sprintf(buf, "%s has withdrawn %d gold from the clan treasury.", ch->name, amt);
        do_info(ch, buf);
        make_note("Clan", "Clan Clerk", clan_table[ch->clan].name, "Treasury Grows", 7, buf);

        ch->gold += amt;
        clan_table[ch->clan].gold -= amt;
        save_char_obj(ch);
        save_clan();

return;
}

