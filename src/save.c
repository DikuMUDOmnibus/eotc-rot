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
 *  ROM 2.4 is copyright 1993-1998 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@hypercube.org)                                *
 *      Gabrielle Taylor (gtaylor@hypercube.org)                           *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Rom24/doc/rom.license                         *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "clan.h"

#if !defined(macintosh)
extern int _filbuf args ((FILE *));
#endif


/* int rename(const char *oldfname, const char *newfname); viene en stdio.h */

char *print_flags (int flag)
{
    int count, pos = 0;
    static char buf[52];


    for (count = 0; count < 32; count++)
    {
        if (IS_SET (flag, 1 << count))
        {
            if (count < 26)
                buf[pos] = 'A' + count;
            else
                buf[pos] = 'a' + (count - 26);
            pos++;
        }
    }

    if (pos == 0)
    {
        buf[pos] = '0';
        pos++;
    }

    buf[pos] = '\0';

    return buf;
}


/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST    100
static OBJ_DATA *rgObjNest[MAX_NEST];



/*
 * Local functions.
 */
void fwrite_char args ((CHAR_DATA * ch, FILE * fp));
void fwrite_obj args ((CHAR_DATA * ch, OBJ_DATA * obj, FILE * fp, int iNest));
void fwrite_pet args ((CHAR_DATA * pet, FILE * fp));
void fread_char args ((CHAR_DATA * ch, FILE * fp));
void fread_pet args ((CHAR_DATA * ch, FILE * fp));
void fread_obj args ((CHAR_DATA * ch, FILE * fp));



/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_obj (CHAR_DATA * ch)
{
    char strsave[MAX_INPUT_LENGTH];
    FILE *fp;

    if (IS_NPC (ch))
        return;


    if (ch->pcdata->playerid < 12)
      ch->pcdata->playerid = get_next_playerid();

	/*
	 * Fix by Edwin. JR -- 10/15/00
	 *
	 * Don't save if the character is invalidated.
	 * This might happen during the auto-logoff of players.
	 * (or other places not yet found out)
	 */
	if ( !IS_VALID(ch)) {
    	bug("save_char_obj: Trying to save an invalidated character.\n",0);
    	return;
	}

    if (ch->desc != NULL && ch->desc->original != NULL)
        ch = ch->desc->original;

#if defined(unix)
    /* create god log */
    if (IS_IMMORTAL (ch) || ch->level >= LEVEL_IMMORTAL)
    {
        fclose (fpReserve);
        sprintf (strsave, "%s%s", GOD_DIR, capitalize (ch->name));
        if ((fp = fopen (strsave, "w")) == NULL)
        {
            bug ("Save_char_obj: fopen", 0);
            perror (strsave);
        }

        fprintf (fp, "Lev %2d Trust %2d  %s%s\n",
                 ch->level, get_trust (ch), ch->name, ch->pcdata->title);
        fclose (fp);
        fpReserve = fopen (NULL_FILE, "r");
    }
#endif

    fclose (fpReserve);
    sprintf (strsave, "%s%s", PLAYER_DIR, capitalize (ch->name));
    if ((fp = fopen (TEMP_FILE, "w")) == NULL)
    {
        bug ("Save_char_obj: fopen", 0);
        perror (strsave);
    }
    else
    {
        fwrite_char (ch, fp);
        if (ch->carrying != NULL)
            fwrite_obj (ch, ch->carrying, fp, 0);
        /* save the pets */
        if (ch->pet != NULL && ch->pet->in_room == ch->in_room)
            fwrite_pet (ch->pet, fp);
        fprintf (fp, "#END\n");
    }
    fclose (fp);
    rename (TEMP_FILE, strsave);
    fpReserve = fopen (NULL_FILE, "r");
    return;
}


/*
 * Write the char.
 */
void fwrite_char (CHAR_DATA * ch, FILE * fp)
{
    AFFECT_DATA *paf;
    int sn, gn, pos, i;

    fprintf (fp, "#%s\n", IS_NPC (ch) ? "MOB" : "PLAYER");

    fprintf (fp, "Name %s~\n", ch->name);
    fprintf(fp, "Lasthost %s~\n", ch->lasthost);
    fprintf (fp, "Id   %ld\n", ch->id);
    fprintf( fp, "PlayerID     %d\n",   ch->pcdata->playerid    );
    fprintf (fp, "Weight %d\n", ch->weight	);
    fprintf (fp, "Height %d\n", ch->height	);
        fprintf(fp, "Pc_build %d\n", ch->pcdata->pc_build);
        fprintf(fp, "Pc_eyes %d\n", ch->pcdata->pc_eyes);
        fprintf(fp, "Pc_hair %d\n", ch->pcdata->pc_hair);
        fprintf(fp, "Pc_looks %d\n", ch->pcdata->pc_looks);
    fprintf (fp, "LogO %ld\n", current_time);
    fprintf (fp, "Vers %d\n", 5);
        fprintf(fp,
                "CPower %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		ch->power[0], ch->power[1], ch->power[2], ch->power[3],
                ch->power[4], ch->power[5], ch->power[6], ch->power[7],
                ch->power[8], ch->power[9], ch->power[10], ch->power[11],
                ch->power[12], ch->power[13], ch->power[14], ch->power[15],
                ch->power[16], ch->power[17], ch->power[18], ch->power[19],
                ch->power[20], ch->power[21], ch->power[22], ch->power[23],
                ch->power[24], ch->power[25], ch->power[26], ch->power[27],
                ch->power[28], ch->power[29], ch->power[30], ch->power[31],
                ch->power[32], ch->power[33], ch->power[34], ch->power[35],
                ch->power[36], ch->power[37], ch->power[38], ch->power[39],
                ch->power[40], ch->power[41], ch->power[42], ch->power[43],
                ch->power[44], ch->power[45], ch->power[46], ch->power[47],
                ch->power[48], ch->power[49], ch->power[50], ch->power[51],
                ch->power[52], ch->power[53]);

    if (ch->short_descr[0] != '\0')
        fprintf (fp, "ShD  %s~\n", ch->short_descr);
    if (ch->long_descr[0] != '\0')
        fprintf (fp, "LnD  %s~\n", ch->long_descr);
    if (ch->description[0] != '\0')
        fprintf (fp, "Desc %s~\n", ch->description);
    if (ch->prompt != NULL || !str_cmp (ch->prompt, 
"{W<{r%h{W/{R%H{whp {m%m{W/{M%M{wm {y%v{W/{Y%V{wmv {g({B%x{wexp{g){W>{x")
        || !str_cmp (ch->prompt, "{c<%hhp %mm %vmv>{x "))
        fprintf (fp, "Prom %s~\n", ch->prompt);
    fprintf (fp, "Race %s~\n", pc_race_table[ch->race].name);
	if (ch->clan)
	{
	fprintf( fp, "Clan %s~\n",	clan_table[ch->clan].name);
	fprintf( fp, "Rank %d\n",	ch->rank);
	}

    if( ch->pcdata->h_vnum )
        fprintf(fp,"HVnum %d\n", ch->pcdata->h_vnum);
    if( ch->pcdata->horesets )
        fprintf(fp,"HOResets %d\n", ch->pcdata->horesets);
    if( ch->pcdata->hmresets )
        fprintf(fp,"HMResets %d\n", ch->pcdata->hmresets);

	if (ch->petition)
	fprintf( fp, "Petit %s~\n",	clan_table[ch->petition].name);
    fprintf (fp, "Sex  %d\n", ch->sex);
    fprintf (fp, "Cla  %d\n", ch->class);
    fprintf (fp, "Cla2 %d\n", ch->class2);
    fprintf (fp, "Cla3 %d\n", ch->class3);
    fprintf (fp, "Cla4 %d\n", ch->class4);
    fprintf(fp, "Hmtown %d\n", ch->hometown);
    fprintf (fp, "God %d\n", ch->god);
    fprintf (fp, "Levl %d\n", ch->level);
    fprintf (fp, "Levl2 %d\n", ch->level2);
    fprintf (fp, "Levl3 %d\n", ch->level3);
    fprintf (fp, "Levl4 %d\n", ch->level4);
		fprintf (fp, "SubLevel %d\n", ch->sublevel);
    fprintf (fp, "Disease %d\n", ch->disease);
    fprintf (fp, "Blood %d\n", ch->blood);
    fprintf (fp, "Mkills %d\n", ch->pcdata->mkills);
    fprintf (fp, "Mdeaths %d\n", ch->pcdata->mdeaths);
    fprintf (fp, "Dragonage %d\n", ch->pcdata->dragonage);
    fprintf (fp, "Morph %s~\n", ch->morph);
    fprintf (fp, "Status %d\n", ch->status);
    fprintf (fp, "Pkill %d\n", ch->pkill);
    fprintf (fp, "Pdeath %d\n", ch->pdeath);
    fprintf (fp, "Polyaff %d\n", ch->polyaff);
                fprintf(fp, "Power        ");
                for (sn = 0; sn < 20; sn++)
                {
                        fprintf(fp, "%d ", ch->pcdata->powers[sn]);
                }
                fprintf(fp, "\n");
    fprintf (fp, "Spellpower %d\n", ch->spellpower);
    fprintf (fp, "Tier %d\n", ch->tier);


        if (ch->pcdata->damreduct > 100 || ch->pcdata->damreduct < 50)
        {
                ch->pcdata->damreduct = 100;
                ch->pcdata->damreductdec = 0;
        }
        if (ch->pcdata->damreductdec != 0)
        {
                if (ch->pcdata->damreductdec > 9
                    || ch->pcdata->damreductdec < 1)
                        ch->pcdata->damreductdec = 0;
        }
        fprintf(fp, "DamRed %d %d\n", ch->pcdata->damreduct,
                ch->pcdata->damreductdec);


    if (ch->questpoints != 0)
        fprintf( fp, "QuestPnts %d\n",  ch->questpoints );
    if (ch->pcdata->questpoints != 0)
        fprintf( fp, "QuestPoints %d\n",  ch->pcdata->questpoints );
    if (ch->nextquest != 0)
        fprintf( fp, "QuestNext %d\n",  ch->nextquest   );

    else if (ch->countdown != 0)
        fprintf( fp, "QuestNext %d\n",  10              );

    fprintf( fp, "Questcomp %d\n", ch->questscomp );

    if (ch->trust != 0)
        fprintf (fp, "Tru  %d\n", ch->trust);
    fprintf (fp, "Sec  %d\n", ch->pcdata->security);    /* OLC */
    fprintf (fp, "Plyd %d\n", ch->played + (int) (current_time - ch->logon));
    fprintf (fp, "Scro %d\n", ch->lines);
    fprintf (fp, "Room %d\n", (ch->in_room == get_room_index (ROOM_VNUM_LIMBO)
                               && ch->was_in_room != NULL)
             ? ch->was_in_room->vnum
             : ch->in_room == NULL ? 3001 : ch->in_room->vnum);

    fprintf (fp, "HMV  %d %d %d %d %d %d\n",
             ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move,
             ch->max_move);
    if ( ch->platinum > 0 )
        fprintf ( fp, "Plat %ld\n", ch->platinum );
    else
        fprintf ( fp, "Plat %d\n", 0 );

    if (ch->gold > 0)
        fprintf (fp, "Gold %ld\n", ch->gold);
    else
        fprintf (fp, "Gold %d\n", 0);
    if (ch->silver > 0)
        fprintf (fp, "Silv %ld\n", ch->silver);
    else
        fprintf (fp, "Silv %d\n", 0);

    if (ch->copper > 0)
        fprintf (fp, "Copper %ld\n", ch->copper);
    else
        fprintf (fp, "Copper %d\n", 0);

    if (ch->pcdata->gold_bank > 0)
      fprintf( fp, "Gold_bank %ld\n",ch->pcdata->gold_bank);
    else
      fprintf( fp, "Gold_bank %d\n", 0);
    if (ch->pcdata->silver_bank > 0)
	fprintf( fp, "Silv_bank %ld\n",ch->pcdata->silver_bank);
    else
	fprintf( fp, "Silv_bank %d\n",0);

    fprintf (fp, "Exp  %d\n", ch->exp);

	fprintf( fp, "Stance %d %d %d %d %d %d %d %d %d %d %d\n",
	ch->stance[0], ch->stance[1], ch->stance[2], ch->stance[3],
	ch->stance[4], ch->stance[5], ch->stance[6], ch->stance[7],
	ch->stance[8], ch->stance[9], ch->stance[10] );


    if (ch->act != 0)
        fprintf (fp, "Act  %s\n", print_flags (ch->act));
    if (ch->affected_by != 0)
        fprintf (fp, "AfBy %s\n", print_flags (ch->affected_by));
    if (ch->affected2_by != 0)
        fprintf (fp, "AfBy2 %s\n", print_flags (ch->affected2_by));
    if (ch->detection != 0)
	fprintf( fp, "Detect %s\n",   print_flags(ch->detection));

    fprintf (fp, "Comm %s\n", print_flags (ch->comm));
    if (ch->wiznet)
        fprintf (fp, "Wizn %s\n", print_flags (ch->wiznet));
    if (ch->invis_level)
        fprintf (fp, "Invi %d\n", ch->invis_level);
    if (ch->incog_level)
        fprintf (fp, "Inco %d\n", ch->incog_level);
    fprintf (fp, "Pos  %d\n",
             ch->position == POS_FIGHTING ? POS_STANDING : ch->position);
    if (ch->practice != 0)
        fprintf (fp, "Prac %d\n", ch->practice);
    if (ch->train != 0)
        fprintf (fp, "Trai %d\n", ch->train);
    if (ch->saving_throw != 0)
        fprintf (fp, "Save  %d\n", ch->saving_throw);
    fprintf (fp, "Alig  %d\n", ch->alignment);
    if (ch->hitroll != 0)
        fprintf (fp, "Hit   %d\n", ch->hitroll);
    if (ch->damroll != 0)
        fprintf (fp, "Dam   %d\n", ch->damroll);
    fprintf (fp, "ACs %d %d %d %d\n",
             ch->armor[0], ch->armor[1], ch->armor[2], ch->armor[3]);
    if (ch->wimpy != 0)
        fprintf (fp, "Wimp  %d\n", ch->wimpy);
    fprintf (fp, "Attr %d %d %d %d %d %d %d %d %d %d \n",
             ch->perm_stat[STAT_STR],
             ch->perm_stat[STAT_INT],
             ch->perm_stat[STAT_WIS],
             ch->perm_stat[STAT_DEX],
	     ch->perm_stat[STAT_CON],
	     ch->perm_stat[STAT_DIS],
	     ch->perm_stat[STAT_AGI],
	     ch->perm_stat[STAT_VIT],
	     ch->perm_stat[STAT_CHA],
	     ch->perm_stat[STAT_LUC]);

    fprintf (fp, "AMod %d %d %d %d %d %d %d %d %d %d\n",
             ch->mod_stat[STAT_STR],
             ch->mod_stat[STAT_INT],
             ch->mod_stat[STAT_WIS],
             ch->mod_stat[STAT_DEX],
	     ch->mod_stat[STAT_CON],
             ch->mod_stat[STAT_DIS],
             ch->mod_stat[STAT_AGI],
             ch->mod_stat[STAT_VIT],
             ch->mod_stat[STAT_CHA],
             ch->mod_stat[STAT_LUC]);
   

if ( ch->pcdata->spouse[0] != '\0' )
    fprintf ( fp, "Spouse %s~\n", ch->pcdata->spouse );
    fprintf ( fp, "Balance %ld\n", ch->pcdata->balance );
    fprintf ( fp, "Shares %ld\n", ch->pcdata->shares );

    if (IS_NPC (ch))
    {
        fprintf (fp, "Vnum %d\n", ch->pIndexData->vnum);
    }
    else
    {
        fprintf (fp, "Pass %s~\n", ch->pcdata->pwd);
        if (ch->pcdata->bamfin[0] != '\0')
            fprintf (fp, "Bin  %s~\n", ch->pcdata->bamfin);
        if (ch->pcdata->bamfout[0] != '\0')
            fprintf (fp, "Bout %s~\n", ch->pcdata->bamfout);
        fprintf (fp, "Titl %s~\n", ch->pcdata->title);
        fprintf (fp, "Pnts %d\n", ch->pcdata->points);
        fprintf (fp, "TSex %d\n", ch->pcdata->true_sex);
        fprintf (fp, "LLev %d\n", ch->pcdata->last_level);
        fprintf (fp, "HMVP %d %d %d\n", ch->pcdata->perm_hit,
                 ch->pcdata->perm_mana, ch->pcdata->perm_move);
        fprintf (fp, "Cnd  %d %d %d %d\n",
                 ch->pcdata->condition[0],
                 ch->pcdata->condition[1],
                 ch->pcdata->condition[2], ch->pcdata->condition[3]);

        /*
         * Write Colour Config Information.
         */
        fprintf (fp, "Coloura     %d%d%d %d%d%d %d%d%d %d%d%d %d%d%d\n",
                 ch->pcdata->text[2],
                 ch->pcdata->text[0],
                 ch->pcdata->text[1],
                 ch->pcdata->auction[2],
                 ch->pcdata->auction[0],
                 ch->pcdata->auction[1],
                 ch->pcdata->gossip[2],
                 ch->pcdata->gossip[0],
                 ch->pcdata->gossip[1],
                 ch->pcdata->music[2],
                 ch->pcdata->music[0],
                 ch->pcdata->music[1],
                 ch->pcdata->question[2],
                 ch->pcdata->question[0], ch->pcdata->question[1]);
        fprintf (fp, "Colourb     %d%d%d %d%d%d %d%d%d %d%d%d %d%d%d\n",
                 ch->pcdata->answer[2],
                 ch->pcdata->answer[0],
                 ch->pcdata->answer[1],
                 ch->pcdata->quote[2],
                 ch->pcdata->quote[0],
                 ch->pcdata->quote[1],
                 ch->pcdata->quote_text[2],
                 ch->pcdata->quote_text[0],
                 ch->pcdata->quote_text[1],
                 ch->pcdata->immtalk_text[2],
                 ch->pcdata->immtalk_text[0],
                 ch->pcdata->immtalk_text[1],
                 ch->pcdata->immtalk_type[2],
                 ch->pcdata->immtalk_type[0], ch->pcdata->immtalk_type[1]);
        fprintf (fp, "Colourc     %d%d%d %d%d%d %d%d%d %d%d%d %d%d%d\n",
                 ch->pcdata->info[2],
                 ch->pcdata->info[0],
                 ch->pcdata->info[1],
                 ch->pcdata->tell[2],
                 ch->pcdata->tell[0],
                 ch->pcdata->tell[1],
                 ch->pcdata->reply[2],
                 ch->pcdata->reply[0],
                 ch->pcdata->reply[1],
                 ch->pcdata->gtell_text[2],
                 ch->pcdata->gtell_text[0],
                 ch->pcdata->gtell_text[1],
                 ch->pcdata->gtell_type[2],
                 ch->pcdata->gtell_type[0], ch->pcdata->gtell_type[1]);
        fprintf (fp, "Colourd     %d%d%d %d%d%d %d%d%d %d%d%d %d%d%d\n",
                 ch->pcdata->room_title[2],
                 ch->pcdata->room_title[0],
                 ch->pcdata->room_title[1],
                 ch->pcdata->room_text[2],
                 ch->pcdata->room_text[0],
                 ch->pcdata->room_text[1],
                 ch->pcdata->room_exits[2],
                 ch->pcdata->room_exits[0],
                 ch->pcdata->room_exits[1],
                 ch->pcdata->room_things[2],
                 ch->pcdata->room_things[0],
                 ch->pcdata->room_things[1],
                 ch->pcdata->prompt[2],
                 ch->pcdata->prompt[0], ch->pcdata->prompt[1]);
        fprintf (fp, "Coloure     %d%d%d %d%d%d %d%d%d %d%d%d %d%d%d\n",
                 ch->pcdata->fight_death[2],
                 ch->pcdata->fight_death[0],
                 ch->pcdata->fight_death[1],
                 ch->pcdata->fight_yhit[2],
                 ch->pcdata->fight_yhit[0],
                 ch->pcdata->fight_yhit[1],
                 ch->pcdata->fight_ohit[2],
                 ch->pcdata->fight_ohit[0],
                 ch->pcdata->fight_ohit[1],
                 ch->pcdata->fight_thit[2],
                 ch->pcdata->fight_thit[0],
                 ch->pcdata->fight_thit[1],
                 ch->pcdata->fight_skill[2],
                 ch->pcdata->fight_skill[0], ch->pcdata->fight_skill[1]);
        fprintf (fp, "Colourf     %d%d%d %d%d%d %d%d%d %d%d%d %d%d%d\n",
                 ch->pcdata->wiznet[2],
                 ch->pcdata->wiznet[0],
                 ch->pcdata->wiznet[1],
                 ch->pcdata->say[2],
                 ch->pcdata->say[0],
                 ch->pcdata->say[1],
                 ch->pcdata->say_text[2],
                 ch->pcdata->say_text[0],
                 ch->pcdata->say_text[1],
                 ch->pcdata->tell_text[2],
                 ch->pcdata->tell_text[0],
                 ch->pcdata->tell_text[1],
                 ch->pcdata->reply_text[2],
                 ch->pcdata->reply_text[0], ch->pcdata->reply_text[1]);
        fprintf (fp, "Colourg     %d%d%d %d%d%d %d%d%d %d%d%d %d%d%d\n",
                 ch->pcdata->auction_text[2],
                 ch->pcdata->auction_text[0],
                 ch->pcdata->auction_text[1],
                 ch->pcdata->gossip_text[2],
                 ch->pcdata->gossip_text[0],
                 ch->pcdata->gossip_text[1],
                 ch->pcdata->music_text[2],
                 ch->pcdata->music_text[0],
                 ch->pcdata->music_text[1],
                 ch->pcdata->question_text[2],
                 ch->pcdata->question_text[0],
                 ch->pcdata->question_text[1],
                 ch->pcdata->answer_text[2],
                 ch->pcdata->answer_text[0], ch->pcdata->answer_text[1]);


	// write alias
        for (pos = 0; pos < MAX_ALIAS; pos++)
        {
            if (ch->pcdata->alias[pos] == NULL
                || ch->pcdata->alias_sub[pos] == NULL)
                break;

            fprintf (fp, "Alias %s %s~\n", ch->pcdata->alias[pos],
                     ch->pcdata->alias_sub[pos]);
        }

			fprintf(fp, "Focus %ld %ld %ld %ld %ld %ld %ld %ld\n",ch->pcdata->focus[COMBAT_POWER],
        ch->pcdata->focus[COMBAT_DEFENSE],ch->pcdata->focus[COMBAT_AGILITY],
        ch->pcdata->focus[COMBAT_TOUGHNESS],ch->pcdata->focus[MAGIC_POWER],
        ch->pcdata->focus[MAGIC_ABILITY],ch->pcdata->focus[MAGIC_DEFENSE],
        ch->pcdata->focus[CURRENT_FOCUS]);

      fprintf(fp, "Mtype %d %d %d %d %d %d\n",ch->magic[MAGIC_WHITE],
      ch->magic[MAGIC_RED],ch->magic[MAGIC_BLUE],ch->magic[MAGIC_GREEN],
      ch->magic[MAGIC_BLACK], ch->magic[MAGIC_ORANGE]);


		/* Save note board status */
		/* Save number of boards in case that number changes */
		fprintf (fp, "Boards       %d ", MAX_BOARD);
		for (i = 0; i < MAX_BOARD; i++)
			fprintf (fp, "%s %ld ", boards[i].short_name, ch->pcdata->last_note[i]);
		fprintf (fp, "\n");

        for (sn = 0; sn < MAX_SKILL; sn++)
        {
            if (skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0)
            {
                fprintf (fp, "Sk %d '%s'\n",
                         ch->pcdata->learned[sn], skill_table[sn].name);
            }
        }

        for (gn = 0; gn < MAX_GROUP; gn++)
        {
            if (group_table[gn].name != NULL && ch->pcdata->group_known[gn])
            {
                fprintf (fp, "Gr '%s'\n", group_table[gn].name);
            }
        }
    }

    for (paf = ch->affected; paf != NULL; paf = paf->next)
    {
        if (paf->type < 0 || paf->type >= MAX_SKILL)
            continue;

        fprintf (fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
                 skill_table[paf->type].name,
                 paf->where,
                 paf->level,
                 paf->duration, paf->modifier, paf->location, paf->bitvector);
    }
    fwrite_rle ( ch->pcdata->explored, fp );

#ifdef IMC
    imc_savechar( ch, fp );
#endif
    fprintf (fp, "End\n\n");
    return;
}

/* write a pet */
void fwrite_pet (CHAR_DATA * pet, FILE * fp)
{
    AFFECT_DATA *paf;

    fprintf (fp, "#PET\n");

    fprintf (fp, "Vnum %d\n", pet->pIndexData->vnum);

    fprintf (fp, "Name %s~\n", pet->name);
    fprintf (fp, "LogO %ld\n", current_time);
    if (pet->short_descr != pet->pIndexData->short_descr)
        fprintf (fp, "ShD  %s~\n", pet->short_descr);
    if (pet->long_descr != pet->pIndexData->long_descr)
        fprintf (fp, "LnD  %s~\n", pet->long_descr);
    if (pet->description != pet->pIndexData->description)
        fprintf (fp, "Desc %s~\n", pet->description);
    if (pet->race != pet->pIndexData->race)
        fprintf (fp, "Race %s~\n", race_table[pet->race].name);
    if (pet->clan)
        fprintf (fp, "Clan %s~\n", clan_table[pet->clan].name);
    fprintf (fp, "Sex  %d\n", pet->sex);

	fprintf( fp, "PetStance  %d %d %d %d %d %d %d %d %d %d %d\n",
	pet->stance[0], pet->stance[1], pet->stance[2], pet->stance[3],
	pet->stance[4], pet->stance[5], pet->stance[6], pet->stance[7],
	pet->stance[8], pet->stance[9], pet->stance[10] );

    if (pet->level != pet->pIndexData->level)
        fprintf (fp, "Levl %d\n", pet->level);
    fprintf (fp, "HMV  %d %d %d %d %d %d\n",
             pet->hit, pet->max_hit, pet->mana, pet->max_mana, pet->move,
             pet->max_move);

    if ( pet->platinum > 0 )
        fprintf ( fp, "Plat %ld\n", pet->platinum );
    if (pet->gold > 0)
        fprintf (fp, "Gold %ld\n", pet->gold);
    if (pet->silver > 0)
        fprintf (fp, "Silv %ld\n", pet->silver);
    if (pet->copper > 0)
        fprintf (fp, "Copper %ld\n", pet->copper);
    if (pet->exp > 0)
        fprintf (fp, "Exp  %d\n", pet->exp);
    if (pet->act != pet->pIndexData->act)
        fprintf (fp, "Act  %s\n", print_flags (pet->act));
    if (pet->affected_by != pet->pIndexData->affected_by)
        fprintf (fp, "AfBy %s\n", print_flags (pet->affected_by));
    if (pet->detection != pet->pIndexData->detection)
    	fprintf(fp, "Detect %s\n", print_flags(pet->detection));

    if (pet->comm != 0)
        fprintf (fp, "Comm %s\n", print_flags (pet->comm));
    fprintf (fp, "Pos  %d\n", pet->position =
             POS_FIGHTING ? POS_STANDING : pet->position);
    if (pet->saving_throw != 0)
        fprintf (fp, "Save %d\n", pet->saving_throw);
    if (pet->alignment != pet->pIndexData->alignment)
        fprintf (fp, "Alig %d\n", pet->alignment);
    if (pet->hitroll != pet->pIndexData->hitroll)
        fprintf (fp, "Hit  %d\n", pet->hitroll);
    if (pet->damroll != pet->pIndexData->damage[DICE_BONUS])
        fprintf (fp, "Dam  %d\n", pet->damroll);
    fprintf (fp, "ACs  %d %d %d %d\n",
             pet->armor[0], pet->armor[1], pet->armor[2], pet->armor[3]);
    fprintf (fp, "Attr %d %d %d %d %d %d %d %d %d %d\n",
             pet->perm_stat[STAT_STR], pet->perm_stat[STAT_INT],
             pet->perm_stat[STAT_WIS], pet->perm_stat[STAT_DEX],
             pet->perm_stat[STAT_CON], pet->perm_stat[STAT_DIS],
             pet->perm_stat[STAT_AGI], pet->perm_stat[STAT_VIT],
             pet->perm_stat[STAT_CHA], pet->perm_stat[STAT_LUC]);
    fprintf (fp, "AMod %d %d %d %d %d %d %d %d %d %d\n",
             pet->mod_stat[STAT_STR], pet->mod_stat[STAT_INT],
             pet->mod_stat[STAT_WIS], pet->mod_stat[STAT_DEX],
             pet->mod_stat[STAT_CON], pet->mod_stat[STAT_DIS],
             pet->mod_stat[STAT_AGI], pet->mod_stat[STAT_VIT],
             pet->mod_stat[STAT_CHA], pet->mod_stat[STAT_LUC]);

    for (paf = pet->affected; paf != NULL; paf = paf->next)
    {
        if (paf->type < 0 || paf->type >= MAX_SKILL)
            continue;

        fprintf (fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
                 skill_table[paf->type].name,
                 paf->where, paf->level, paf->duration, paf->modifier,
                 paf->location, paf->bitvector);
    }

    fprintf (fp, "End\n");
    return;
}

/*
 * Write an object and its contents.
 */
void fwrite_obj (CHAR_DATA * ch, OBJ_DATA * obj, FILE * fp, int iNest)
{
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if (obj->next_content != NULL)
        fwrite_obj (ch, obj->next_content, fp, iNest);

    /*
     * Castrate storage characters.
     */
    if ((ch->level < obj->level - 2 && obj->item_type != ITEM_CONTAINER)
        || obj->item_type == ITEM_KEY
        || (obj->item_type == ITEM_MAP && !obj->value[0]))
        return;

    fprintf (fp, "#O\n");
    fprintf (fp, "Vnum %d\n", obj->pIndexData->vnum);
    if (!obj->pIndexData->new_format)
        fprintf (fp, "Oldstyle\n");
    if (obj->enchanted)
        fprintf (fp, "Enchanted\n");
    fprintf (fp, "Nest %d\n", iNest);

    /* these data are only used if they do not match the defaults */

    if (obj->name != obj->pIndexData->name)
        fprintf (fp, "Name %s~\n", obj->name);
    fprintf( fp, "OwnerID      %d\n",   obj->ownerid		     );
    if (obj->short_descr != obj->pIndexData->short_descr)
        fprintf (fp, "ShD  %s~\n", obj->short_descr);
    if (obj->description != obj->pIndexData->description)
        fprintf (fp, "Desc %s~\n", obj->description);
    if (obj->questowner != NULL && strlen(obj->questowner) > 1)
	fprintf( fp, "Questowner   %s~\n",      obj->questowner      );
    if (obj->extra_flags != obj->pIndexData->extra_flags)
        fprintf (fp, "ExtF %d\n", obj->extra_flags);
    if (obj->wear_flags != obj->pIndexData->wear_flags)
        fprintf (fp, "WeaF %d\n", obj->wear_flags);
    if (obj->item_type != obj->pIndexData->item_type)
        fprintf (fp, "Ityp %d\n", obj->item_type);
    if (obj->weight != obj->pIndexData->weight)
        fprintf (fp, "Wt   %d\n", obj->weight);
    if (obj->condition != obj->pIndexData->condition)
        fprintf (fp, "Cond %d\n", obj->condition);
    fprintf( fp, "WeaponCurrentXP %d\n", obj->weapon_currentxp);
    fprintf( fp, "WeaponLevel %d\n", obj->weapon_level);
    fprintf( fp, "WeaponPoints %d\n", obj->weapon_points);

    /* variable data */

    fprintf (fp, "Wear %d\n", obj->wear_loc);
    if (obj->level != obj->pIndexData->level)
        fprintf (fp, "Lev  %d\n", obj->level);
    if (obj->timer != 0)
        fprintf (fp, "Time %d\n", obj->timer);
    fprintf (fp, "Cost %d\n", obj->cost);
    if (obj->value[0] != obj->pIndexData->value[0]
        || obj->value[1] != obj->pIndexData->value[1]
        || obj->value[2] != obj->pIndexData->value[2]
        || obj->value[3] != obj->pIndexData->value[3]
        || obj->value[4] != obj->pIndexData->value[4])
        fprintf (fp, "Val  %d %d %d %d %d\n",
                 obj->value[0], obj->value[1], obj->value[2], obj->value[3],
                 obj->value[4]);

    switch (obj->item_type)
    {
        case ITEM_POTION:
        case ITEM_SCROLL:
        case ITEM_PILL:
            if (obj->value[1] > 0)
            {
                fprintf (fp, "Spell 1 '%s'\n",
                         skill_table[obj->value[1]].name);
            }

            if (obj->value[2] > 0)
            {
                fprintf (fp, "Spell 2 '%s'\n",
                         skill_table[obj->value[2]].name);
            }

            if (obj->value[3] > 0)
            {
                fprintf (fp, "Spell 3 '%s'\n",
                         skill_table[obj->value[3]].name);
            }

            break;

        case ITEM_STAFF:
        case ITEM_WAND:
            if (obj->value[3] > 0)
            {
                fprintf (fp, "Spell 3 '%s'\n",
                         skill_table[obj->value[3]].name);
            }

            break;
    }


    for (paf = obj->affected; paf != NULL; paf = paf->next)
    {
        if (paf->type < 0 || paf->type >= MAX_SKILL)
            continue;
        fprintf (fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
                 skill_table[paf->type].name,
                 paf->where,
                 paf->level,
                 paf->duration, paf->modifier, paf->location, paf->bitvector);
    }

    for (ed = obj->extra_descr; ed != NULL; ed = ed->next)
    {
        fprintf (fp, "ExDe %s~ %s~\n", ed->keyword, ed->description);
    }

    fprintf (fp, "End\n\n");

    if (obj->contains != NULL)
        fwrite_obj (ch, obj->contains, fp, iNest + 1);

    return;
}



/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj (DESCRIPTOR_DATA * d, char *name)
{
    char strsave[MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *ch;
    FILE *fp;
    bool found;
    int stat;
    int sn;

    ch = new_char ();
    ch->pcdata = new_pcdata ();

    d->character = ch;
    ch->desc = d;
    ch->name = str_dup (name);
    ch->lasthost = str_dup("");
    ch->id = get_pc_id ();
//    ch->pcdata->playerid = 0;
    ch->class2 = -1;
    ch->class3 = -1;
    ch->class4 = -1;
    ch->level2 = 1;
    ch->level3 = 1;
    ch->level4 = 1;
		ch->sublevel = 0;
    ch->god = -1;
    ch->disease = 0;
    ch->blood = 0;
    ch->status = 0;
    ch->pkill = 0;
    ch->pdeath = 0;
    ch->pcdata->mkills = 0;
    ch->pcdata->mdeaths = 0;
    ch->pcdata->dragonage = 0;
    ch->morph = str_dup("");
    ch->polyaff = 0;
    ch->spellpower = 0;
    ch->tier = 0;
    ch->pcdata->damreduct = 100;
    ch->pcdata->damreductdec = 0;
		ch->pcdata->balance = 0;
    ch->pcdata->spouse = str_dup ( "" );

    for (sn = 0; sn < 20; sn++)
	ch->pcdata->powers[sn] = 0;
    ch->race = race_lookup ("human");
	ch->pcdata->pc_build = 0;
        ch->pcdata->pc_eyes = 0;
        ch->pcdata->pc_hair = 0;
        ch->pcdata->pc_looks = 0;
    bzero ( ch->pcdata->explored, MAX_EXPLORE );

    ch->act = PLR_NOSUMMON;
    ch->comm = COMM_COMBINE | COMM_PROMPT;
    ch->prompt = str_dup ("<%hhp %mm %vmv> ");
    ch->pcdata->confirm_delete = FALSE;
	ch->pcdata->board = &boards[DEFAULT_BOARD];
    ch->pcdata->pwd = str_dup ("");
    ch->pcdata->bamfin = str_dup ("");
    ch->pcdata->bamfout = str_dup ("");
    ch->hometown			= 0;
    ch->pcdata->title = str_dup ("");
    for (stat = 0; stat < MAX_STATS; stat++)
        ch->perm_stat[stat] = 13;
    ch->pcdata->condition[COND_THIRST] = 48;
    ch->pcdata->condition[COND_FULL] = 48;
    ch->pcdata->condition[COND_HUNGER] = 48;
    ch->pcdata->security = 0;    /* OLC */

    ch->pcdata->text[0] = (NORMAL);
    ch->pcdata->text[1] = (WHITE);
    ch->pcdata->text[2] = 0;
    ch->pcdata->auction[0] = (BRIGHT);
    ch->pcdata->auction[1] = (YELLOW);
    ch->pcdata->auction[2] = 0;
    ch->pcdata->auction_text[0] = (BRIGHT);
    ch->pcdata->auction_text[1] = (WHITE);
    ch->pcdata->auction_text[2] = 0;
    ch->pcdata->gossip[0] = (NORMAL);
    ch->pcdata->gossip[1] = (MAGENTA);
    ch->pcdata->gossip[2] = 0;
    ch->pcdata->gossip_text[0] = (BRIGHT);
    ch->pcdata->gossip_text[1] = (MAGENTA);
    ch->pcdata->gossip_text[2] = 0;
    ch->pcdata->music[0] = (NORMAL);
    ch->pcdata->music[1] = (RED);
    ch->pcdata->music[2] = 0;
    ch->pcdata->music_text[0] = (BRIGHT);
    ch->pcdata->music_text[1] = (RED);
    ch->pcdata->music_text[2] = 0;
    ch->pcdata->question[0] = (BRIGHT);
    ch->pcdata->question[1] = (YELLOW);
    ch->pcdata->question[2] = 0;
    ch->pcdata->question_text[0] = (BRIGHT);
    ch->pcdata->question_text[1] = (WHITE);
    ch->pcdata->question_text[2] = 0;
    ch->pcdata->answer[0] = (BRIGHT);
    ch->pcdata->answer[1] = (YELLOW);
    ch->pcdata->answer[2] = 0;
    ch->pcdata->answer_text[0] = (BRIGHT);
    ch->pcdata->answer_text[1] = (WHITE);
    ch->pcdata->answer_text[2] = 0;
    ch->pcdata->quote[0] = (NORMAL);
    ch->pcdata->quote[1] = (YELLOW);
    ch->pcdata->quote[2] = 0;
    ch->pcdata->quote_text[0] = (NORMAL);
    ch->pcdata->quote_text[1] = (GREEN);
    ch->pcdata->quote_text[2] = 0;
    ch->pcdata->immtalk_text[0] = (NORMAL);
    ch->pcdata->immtalk_text[1] = (CYAN);
    ch->pcdata->immtalk_text[2] = 0;
    ch->pcdata->immtalk_type[0] = (NORMAL);
    ch->pcdata->immtalk_type[1] = (YELLOW);
    ch->pcdata->immtalk_type[2] = 0;
    ch->pcdata->info[0] = (BRIGHT);
    ch->pcdata->info[1] = (YELLOW);
    ch->pcdata->info[2] = 1;
    ch->pcdata->say[0] = (NORMAL);
    ch->pcdata->say[1] = (GREEN);
    ch->pcdata->say[2] = 0;
    ch->pcdata->say_text[0] = (BRIGHT);
    ch->pcdata->say_text[1] = (GREEN);
    ch->pcdata->say_text[2] = 0;
    ch->pcdata->tell[0] = (NORMAL);
    ch->pcdata->tell[1] = (GREEN);
    ch->pcdata->tell[2] = 0;
    ch->pcdata->tell_text[0] = (BRIGHT);
    ch->pcdata->tell_text[1] = (GREEN);
    ch->pcdata->tell_text[2] = 0;
    ch->pcdata->reply[0] = (NORMAL);
    ch->pcdata->reply[1] = (GREEN);
    ch->pcdata->reply[2] = 0;
    ch->pcdata->reply_text[0] = (BRIGHT);
    ch->pcdata->reply_text[1] = (GREEN);
    ch->pcdata->reply_text[2] = 0;
    ch->pcdata->gtell_text[0] = (NORMAL);
    ch->pcdata->gtell_text[1] = (GREEN);
    ch->pcdata->gtell_text[2] = 0;
    ch->pcdata->gtell_type[0] = (NORMAL);
    ch->pcdata->gtell_type[1] = (RED);
    ch->pcdata->gtell_type[2] = 0;
    ch->pcdata->wiznet[0] = (NORMAL);
    ch->pcdata->wiznet[1] = (GREEN);
    ch->pcdata->wiznet[2] = 0;
    ch->pcdata->room_title[0] = (NORMAL);
    ch->pcdata->room_title[1] = (CYAN);
    ch->pcdata->room_title[2] = 0;
    ch->pcdata->room_text[0] = (NORMAL);
    ch->pcdata->room_text[1] = (WHITE);
    ch->pcdata->room_text[2] = 0;
    ch->pcdata->room_exits[0] = (NORMAL);
    ch->pcdata->room_exits[1] = (GREEN);
    ch->pcdata->room_exits[2] = 0;
    ch->pcdata->room_things[0] = (NORMAL);
    ch->pcdata->room_things[1] = (CYAN);
    ch->pcdata->room_things[2] = 0;
    ch->pcdata->prompt[0] = (NORMAL);
    ch->pcdata->prompt[1] = (CYAN);
    ch->pcdata->prompt[2] = 0;
    ch->pcdata->fight_death[0] = (BRIGHT);
    ch->pcdata->fight_death[1] = (RED);
    ch->pcdata->fight_death[2] = 0;
    ch->pcdata->fight_yhit[0] = (NORMAL);
    ch->pcdata->fight_yhit[1] = (GREEN);
    ch->pcdata->fight_yhit[2] = 0;
    ch->pcdata->fight_ohit[0] = (NORMAL);
    ch->pcdata->fight_ohit[1] = (YELLOW);
    ch->pcdata->fight_ohit[2] = 0;
    ch->pcdata->fight_thit[0] = (NORMAL);
    ch->pcdata->fight_thit[1] = (RED);
    ch->pcdata->fight_thit[2] = 0;
    ch->pcdata->fight_skill[0] = (BRIGHT);
    ch->pcdata->fight_skill[1] = (WHITE);
    ch->pcdata->fight_skill[2] = 0;
#ifdef IMC
    imc_initchar( ch );
#endif
    found = FALSE;
    fclose (fpReserve);

#if defined(unix)
    /* decompress if .gz file exists */
    sprintf (strsave, "%s%s%s", PLAYER_DIR, capitalize (name), ".gz");

    sprintf (strsave, "%s%s", PLAYER_DIR, capitalize (name)); 


    if ((fp = fopen (strsave, "r")) != NULL)
    {
        fclose (fp);
        sprintf (buf, "gzip -dfq %s", strsave);
        system (buf);
    }
#endif

    sprintf (strsave, "%s%s", PLAYER_DIR, capitalize (name));
    if ((fp = fopen (strsave, "r")) != NULL)
    {
        int iNest;

        for (iNest = 0; iNest < MAX_NEST; iNest++)
            rgObjNest[iNest] = NULL;

        found = TRUE;
        for (;;)
        {
            char letter;
            char *word;

            letter = fread_letter (fp);
            if (letter == '*')
            {
                fread_to_eol (fp);
                continue;
            }

            if (letter != '#')
            {
                bug ("Load_char_obj: # not found.", 0);
                break;
            }

            word = fread_word (fp);
            if (!str_cmp (word, "PLAYER"))
                fread_char (ch, fp);
            else if (!str_cmp (word, "OBJECT"))
                fread_obj (ch, fp);
            else if (!str_cmp (word, "O"))
                fread_obj (ch, fp);
            else if (!str_cmp (word, "PET"))
                fread_pet (ch, fp);
            else if (!str_cmp (word, "END"))
                break;
            else
            {
                bug ("Load_char_obj: bad section.", 0);
                break;
            }
        }
        fclose (fp);
    }

    fpReserve = fopen (NULL_FILE, "r");


    /* initialize race */
    if (found)
    {
        int i;

        if (ch->race == 0)
            ch->race = race_lookup ("human");

        ch->size = pc_race_table[ch->race].size;
        ch->dam_type = 17;        /*punch */

        for (i = 0; i < 5; i++)
        {
            if (pc_race_table[ch->race].skills[i] == NULL)
                break;
            group_add (ch, pc_race_table[ch->race].skills[i], FALSE);
        }
//	ch->detection   = ch->detection|race_table[ch->race].det;
        ch->affected_by = ch->affected_by | race_table[ch->race].aff;
        ch->imm_flags = ch->imm_flags | race_table[ch->race].imm;
        ch->res_flags = ch->res_flags | race_table[ch->race].res;
        ch->vuln_flags = ch->vuln_flags | race_table[ch->race].vuln;
        ch->form = race_table[ch->race].form;
        ch->parts = race_table[ch->race].parts;
    }


    /* RT initialize skills */

    if (found && ch->version < 2)
    {                            /* need to add the new skills */
        group_add (ch, "rom basics", FALSE);
        group_add (ch, class_table[ch->class].base_group, FALSE);
        group_add (ch, class_table[ch->class].default_group, TRUE);
        group_add (ch, class_table[ch->class2].base_group, FALSE);
        group_add (ch, class_table[ch->class2].default_group, TRUE);
        group_add (ch, class_table[ch->class3].base_group, FALSE);
        group_add (ch, class_table[ch->class3].default_group, TRUE);
        group_add (ch, class_table[ch->class4].base_group, FALSE);
        group_add (ch, class_table[ch->class4].default_group, TRUE);

        ch->pcdata->learned[gsn_recall] = 50;
    }

    /* fix levels */
    if (found && ch->version < 3 && (ch->level > 35 || ch->trust > 35))
    {
        switch (ch->level)
        {
            case (40):
                ch->level = 60;
                break;            /* imp -> imp */
            case (39):
                ch->level = 58;
                break;            /* god -> supreme */
            case (38):
                ch->level = 56;
                break;            /* deity -> god */
            case (37):
                ch->level = 53;
                break;            /* angel -> demigod */
        }

        switch (ch->trust)
        {
            case (40):
                ch->trust = 60;
                break;            /* imp -> imp */
            case (39):
                ch->trust = 58;
                break;            /* god -> supreme */
            case (38):
                ch->trust = 56;
                break;            /* deity -> god */
            case (37):
                ch->trust = 53;
                break;            /* angel -> demigod */
            case (36):
                ch->trust = 51;
                break;            /* hero -> hero */
        }
    }

    /* ream gold */
    if (found && ch->version < 4)
    {
        ch->gold /= 100;
    }
    return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                    \
                if ( !str_cmp( word, literal ) )    \
                {                    \
                    field  = value;            \
                    fMatch = TRUE;            \
                    break;                \
                }

/* provided to free strings */
#if defined(KEYS)
#undef KEYS
#endif

#define KEYS( literal, field, value )                    \
                if ( !str_cmp( word, literal ) )    \
                {                    \
                    free_string(field);            \
                    field  = value;            \
                    fMatch = TRUE;            \
                    break;                \
                }

void fread_char (CHAR_DATA * ch, FILE * fp)
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    int count = 0;
    int lastlogoff = current_time;
    int percent;
    int sn;

    sprintf (buf, "Loading %s.", ch->name);
    log_string (buf);

    for (;;)
    {
        word = feof (fp) ? "End" : fread_word (fp);
        fMatch = FALSE;

        switch (UPPER (word[0]))
        {
            case '*':
                fMatch = TRUE;
                fread_to_eol (fp);
                break;

            case 'A':
                KEY ("Act", ch->act, fread_flag (fp));
                KEY ("AffectedBy", ch->affected_by, fread_flag (fp));
                KEY ("AffectedBy2", ch->affected2_by, fread_flag (fp));
                KEY ("AfBy", ch->affected_by, fread_flag (fp));
                KEY ("AfBy2", ch->affected2_by, fread_flag (fp));
                KEY ("Alignment", ch->alignment, fread_number (fp));
                KEY ("Alig", ch->alignment, fread_number (fp));

                if (!str_cmp (word, "Alia"))
                {
                    if (count >= MAX_ALIAS)
                    {
                        fread_to_eol (fp);
                        fMatch = TRUE;
                        break;
                    }

                    ch->pcdata->alias[count] = str_dup (fread_word (fp));
                    ch->pcdata->alias_sub[count] = str_dup (fread_word (fp));
                    count++;
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "Alias"))
                {
                    if (count >= MAX_ALIAS)
                    {
                        fread_to_eol (fp);
                        fMatch = TRUE;
                        break;
                    }

                    ch->pcdata->alias[count] = str_dup (fread_word (fp));
                    ch->pcdata->alias_sub[count] = fread_string (fp);
                    count++;
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "AC") || !str_cmp (word, "Armor"))
                {
                    fread_to_eol (fp);
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "ACs"))
                {
                    int i;

                    for (i = 0; i < 4; i++)
                        ch->armor[i] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "AffD"))
                {
                    AFFECT_DATA *paf;
                    int sn;

                    paf = new_affect ();

                    sn = skill_lookup (fread_word (fp));
                    if (sn < 0)
                        bug ("Fread_char: unknown skill.", 0);
                    else
                        paf->type = sn;

                    paf->level = fread_number (fp);
                    paf->duration = fread_number (fp);
                    paf->modifier = fread_number (fp);
                    paf->location = fread_number (fp);
                    paf->bitvector = fread_number (fp);
                    paf->next = ch->affected;
                    ch->affected = paf;
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "Affc"))
                {
                    AFFECT_DATA *paf;
                    int sn;

                    paf = new_affect ();

                    sn = skill_lookup (fread_word (fp));
                    if (sn < 0)
                        bug ("Fread_char: unknown skill.", 0);
                    else
                        paf->type = sn;

                    paf->where = fread_number (fp);
                    paf->level = fread_number (fp);
                    paf->duration = fread_number (fp);
                    paf->modifier = fread_number (fp);
                    paf->location = fread_number (fp);
                    paf->bitvector = fread_number (fp);
                    paf->next = ch->affected;
                    ch->affected = paf;
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "AttrMod") || !str_cmp (word, "AMod"))
                {
                    int stat;
                    for (stat = 0; stat < MAX_STATS; stat++)
                        ch->mod_stat[stat] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "AttrPerm") || !str_cmp (word, "Attr"))
                {
                    int stat;

                    for (stat = 0; stat < MAX_STATS; stat++)
                        ch->perm_stat[stat] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }
                break;

            case 'B':
                KEY ("Bamfin", ch->pcdata->bamfin, fread_string (fp));
                KEY ("Bamfout", ch->pcdata->bamfout, fread_string (fp));
                KEY ("Bin", ch->pcdata->bamfin, fread_string (fp));
		KEY ("Blood", ch->blood, fread_number(fp));
                KEY ("Bout", ch->pcdata->bamfout, fread_string (fp));
                KEY ( "Balance", ch->pcdata->balance, fread_number ( fp ) );

				/* Read in board status */
				if (!str_cmp(word, "Boards" ))
				{
					int i,num = fread_number (fp); /* number of boards saved */
					char *boardname;

					for (; num ; num-- ) /* for each of the board saved */
					{
						boardname = fread_word (fp);
						i = board_lookup (boardname); /* find board number */

						if (i == BOARD_NOTFOUND) /* Does board still exist ? */
						{
							sprintf (buf, "fread_char: %s had unknown board name: %s. Skipped.",
							ch->name, boardname);
							log_string (buf);
							fread_number (fp); /* read last_note and skip info */
						}
						else /* Save it */
							ch->pcdata->last_note[i] = fread_number (fp);
					} /* for */

					fMatch = TRUE;
				} /* Boards */
                break;

            case 'C':
                KEY ("Class", ch->class, fread_number (fp));
                KEY ("Cla", ch->class, fread_number (fp));
                KEY ("Cla2", ch->class2, fread_number (fp));
                KEY ("Cla3", ch->class3, fread_number (fp));
                KEY ("Cla4", ch->class4, fread_number (fp));
                KEY ("Clan", ch->clan, clan_lookup (fread_string (fp)));
                KEY ("Comm", ch->comm, fread_flag (fp));

                if (!str_cmp (word, "Condition") || !str_cmp (word, "Cond"))
                {
                    ch->pcdata->condition[0] = fread_number (fp);
                    ch->pcdata->condition[1] = fread_number (fp);
                    ch->pcdata->condition[2] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }
                if (!str_cmp (word, "Cnd"))
                {
                    ch->pcdata->condition[0] = fread_number (fp);
                    ch->pcdata->condition[1] = fread_number (fp);
                    ch->pcdata->condition[2] = fread_number (fp);
                    ch->pcdata->condition[3] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "Coloura"))
                {
                    LOAD_COLOUR (text)
                        LOAD_COLOUR (auction)
                        LOAD_COLOUR (gossip)
                        LOAD_COLOUR (music)
                        LOAD_COLOUR (question) fMatch = TRUE;
                    break;
                }
                if (!str_cmp (word, "Colourb"))
                {
                    LOAD_COLOUR (answer)
                        LOAD_COLOUR (quote)
                        LOAD_COLOUR (quote_text)
                        LOAD_COLOUR (immtalk_text)
                        LOAD_COLOUR (immtalk_type) fMatch = TRUE;
                    break;
                }
                if (!str_cmp (word, "Colourc"))
                {
                    LOAD_COLOUR (info)
                        LOAD_COLOUR (tell)
                        LOAD_COLOUR (reply)
                        LOAD_COLOUR (gtell_text)
                        LOAD_COLOUR (gtell_type) fMatch = TRUE;
                    break;
                }
                if (!str_cmp (word, "Colourd"))
                {
                    LOAD_COLOUR (room_title)
                        LOAD_COLOUR (room_text)
                        LOAD_COLOUR (room_exits)
                        LOAD_COLOUR (room_things)
                        LOAD_COLOUR (prompt) fMatch = TRUE;
                    break;
                }
                if (!str_cmp (word, "Coloure"))
                {
                    LOAD_COLOUR (fight_death)
                        LOAD_COLOUR (fight_yhit)
                        LOAD_COLOUR (fight_ohit)
                        LOAD_COLOUR (fight_thit)
                        LOAD_COLOUR (fight_skill) fMatch = TRUE;
                    break;
                }
                if (!str_cmp (word, "Colourf"))
                {
                    LOAD_COLOUR (wiznet)
                        LOAD_COLOUR (say)
                        LOAD_COLOUR (say_text)
                        LOAD_COLOUR (tell_text)
                        LOAD_COLOUR (reply_text) fMatch = TRUE;
                    break;
                }
                if (!str_cmp (word, "Colourg"))
                {
                    LOAD_COLOUR (auction_text)
                        LOAD_COLOUR (gossip_text)
						LOAD_COLOUR (music_text)
                        LOAD_COLOUR (question_text)
                        LOAD_COLOUR (answer_text) fMatch = TRUE;
                    break;
                }

		if (ch->copper > 0)
                KEY ("Copper", ch->copper, fread_number (fp));

                        if (!str_cmp(word, "CPower"))
                        {
                                ch->power[0] = fread_number(fp);
                                ch->power[1] = fread_number(fp);
                                ch->power[2] = fread_number(fp);
                                ch->power[3] = fread_number(fp);
                                ch->power[4] = fread_number(fp);
                                ch->power[5] = fread_number(fp);
                                ch->power[6] = fread_number(fp);
                                ch->power[7] = fread_number(fp);
                                ch->power[8] = fread_number(fp);
                                ch->power[9] = fread_number(fp);
                                ch->power[10] = fread_number(fp);
                                ch->power[11] = fread_number(fp);
                                ch->power[12] = fread_number(fp);
                                ch->power[13] = fread_number(fp);
                                ch->power[14] = fread_number(fp);
                                ch->power[15] = fread_number(fp);
                                ch->power[16] = fread_number(fp);
                                ch->power[17] = fread_number(fp);
                                ch->power[18] = fread_number(fp);
                                ch->power[19] = fread_number(fp);
                                ch->power[20] = fread_number(fp);
                                ch->power[21] = fread_number(fp);
                                ch->power[22] = fread_number(fp);
                                ch->power[23] = fread_number(fp);
                                ch->power[24] = fread_number(fp);
                                ch->power[25] = fread_number(fp);
                                ch->power[26] = fread_number(fp);
                                ch->power[27] = fread_number(fp);
                                ch->power[28] = fread_number(fp);
                                ch->power[29] = fread_number(fp);
                                ch->power[30] = fread_number(fp);
                                ch->power[31] = fread_number(fp);
                                ch->power[32] = fread_number(fp);
                                ch->power[33] = fread_number(fp);
                                ch->power[34] = fread_number(fp);
                                ch->power[35] = fread_number(fp);
                                ch->power[36] = fread_number(fp);
                                ch->power[37] = fread_number(fp);
                                ch->power[38] = fread_number(fp);
                                ch->power[39] = fread_number(fp);
                                ch->power[40] = fread_number(fp);
                                ch->power[41] = fread_number(fp);
                                ch->power[42] = fread_number(fp);
                                ch->power[43] = fread_number(fp);
                                ch->power[44] = fread_number(fp);
                                ch->power[45] = fread_number(fp);
                                ch->power[46] = fread_number(fp);
                                ch->power[47] = fread_number(fp);
                                ch->power[48] = fread_number(fp);
                                ch->power[49] = fread_number(fp);
                                ch->power[50] = fread_number(fp);
                                ch->power[51] = fread_number(fp);
                                ch->power[52] = fread_number(fp);
                                ch->power[53] = fread_number(fp);
                                fMatch = TRUE;
                                break;
                        }


                break;

            case 'D':
                KEY ("Damroll", ch->damroll, fread_number (fp));
                KEY ("Dam", ch->damroll, fread_number (fp));
                        if (!str_cmp(word, "DamRed"))
                        {
                                ch->pcdata->damreduct = fread_number(fp);
                                ch->pcdata->damreductdec = fread_number(fp);
                                fMatch = TRUE;
                                break;
                        }
                KEY ("Description", ch->description, fread_string (fp));
                KEY ("Desc", ch->description, fread_string (fp));
                KEY ("Detect",	ch->detection,		fread_flag(fp)     );
		KEY ("Disease", ch->disease, fread_number(fp));
		KEY ("Dragonage", ch->pcdata->dragonage, fread_number(fp));
                break;

            case 'E':
                if (!str_cmp (word, "End"))
                {
                    /* adjust hp mana move up  -- here for speed's sake */
                    percent =
                        (current_time - lastlogoff) * 25 / (2 * 60 * 60);

                    percent = UMIN (percent, 100);

                    if (percent > 0 && !IS_AFFECTED (ch, AFF_POISON)
                        && !IS_AFFECTED (ch, AFF_PLAGUE))
                    {
                        ch->hit += (ch->max_hit - ch->hit) * percent / 100;
                        ch->mana += (ch->max_mana - ch->mana) * percent / 100;
                        ch->move += (ch->max_move - ch->move) * percent / 100;
                    }
                    return;
                }
                KEY ("Exp", ch->exp, fread_number (fp));
                break;

        case 'F':
            if (!str_cmp(word,"Focus"))
            {
                ch->pcdata->focus[COMBAT_POWER]         = fread_number(fp);
                ch->pcdata->focus[COMBAT_DEFENSE]       = fread_number(fp);
                ch->pcdata->focus[COMBAT_AGILITY]       = fread_number(fp);
                ch->pcdata->focus[COMBAT_TOUGHNESS]     = fread_number(fp);
                ch->pcdata->focus[MAGIC_POWER]          = fread_number(fp);
                ch->pcdata->focus[MAGIC_ABILITY]        = fread_number(fp);
                ch->pcdata->focus[MAGIC_DEFENSE]        = fread_number(fp);
                ch->pcdata->focus[CURRENT_FOCUS]        = fread_number(fp);

                fMatch = TRUE;
                break;
            }
						break;

            case 'G':
		KEY ("God", ch->god, fread_number (fp));
                KEY ("Gold", ch->gold, fread_number (fp));
		KEY ("Gold_bank", ch->pcdata->gold_bank, fread_number(fp));

                if (!str_cmp (word, "Group") || !str_cmp (word, "Gr"))
                {
                    int gn;
                    char *temp;

                    temp = fread_word (fp);
                    gn = group_lookup (temp);
                    /* gn    = group_lookup( fread_word( fp ) ); */
                    if (gn < 0)
                    {
                        fprintf (stderr, "%s", temp);
                        bug ("Fread_char: unknown group. ", 0);
                    }
                    else
                        gn_add (ch, gn);
                    fMatch = TRUE;
                }
                break;

            case 'H':
		KEY ("Height", ch->height, fread_number (fp));
                KEY ("Hitroll", ch->hitroll, fread_number (fp));
                KEY ("Hit", ch->hitroll, fread_number (fp));
	    KEY( "Hmtown",	ch->hometown,		fread_number( fp ) );
		KEY( "HVnum",ch->pcdata->h_vnum,     fread_number( fp ) );
            	KEY( "HResets",ch->pcdata->horesets,   fread_number( fp ) );
            	KEY( "HOResets",ch->pcdata->horesets,   fread_number( fp ) );
            	KEY( "HMResets",ch->pcdata->hmresets,   fread_number( fp ) );

                if (!str_cmp (word, "HpManaMove") || !str_cmp (word, "HMV"))
                {
                    ch->hit = fread_number (fp);
                    ch->max_hit = fread_number (fp);
                    ch->mana = fread_number (fp);
                    ch->max_mana = fread_number (fp);
                    ch->move = fread_number (fp);
                    ch->max_move = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "HpManaMovePerm")
                    || !str_cmp (word, "HMVP"))
                {
                    ch->pcdata->perm_hit = fread_number (fp);
                    ch->pcdata->perm_mana = fread_number (fp);
                    ch->pcdata->perm_move = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }

                break;

            case 'I':
                KEY ("Id", ch->id, fread_number (fp));
                KEY ("InvisLevel", ch->invis_level, fread_number (fp));
                KEY ("Inco", ch->incog_level, fread_number (fp));
                KEY ("Invi", ch->invis_level, fread_number (fp));
#ifdef IMC
           if( ( fMatch = imc_loadchar( ch, fp, word ) ) )
                break;
#endif
                break;

            case 'L':
                KEY ("LastLevel", ch->pcdata->last_level, fread_number (fp));
		KEY("Lasthost",	ch->lasthost,	fread_string( fp ) );
                KEY ("LLev", ch->pcdata->last_level, fread_number (fp));
                KEY ("Level", ch->level, fread_number (fp));
                KEY ("Lev", ch->level, fread_number (fp));
                KEY ("Levl", ch->level, fread_number (fp));
                KEY ("Levl2", ch->level2, fread_number (fp));
                KEY ("Levl3", ch->level3, fread_number (fp));
                KEY ("Levl4", ch->level4, fread_number (fp));
                KEY ("LogO", lastlogoff, fread_number (fp));
                KEY ("LongDescr", ch->long_descr, fread_string (fp));
                KEY ("LnD", ch->long_descr, fread_string (fp));
                break;

	    case 'M':
            if (!str_cmp(word,"Mtype"))
            {
             ch->magic[MAGIC_WHITE] = fread_number(fp);
             ch->magic[MAGIC_RED] = fread_number(fp);
             ch->magic[MAGIC_BLUE] = fread_number(fp);
             ch->magic[MAGIC_GREEN] = fread_number(fp);
             ch->magic[MAGIC_BLACK] = fread_number(fp);
             ch->magic[MAGIC_ORANGE] = fread_number(fp);
             fMatch = TRUE;
             break;
            }

		KEY ("Mdeaths", ch->pcdata->mdeaths, fread_number(fp));
		KEY ("Mkills", ch->pcdata->mkills, fread_number(fp));
                KEY ("Morph", ch->morph, fread_string(fp));
		break;

            case 'N':
                KEY ("Name", ch->name, fread_string (fp));
                break;

            case 'P':
                KEY ("Password", ch->pcdata->pwd, fread_string (fp));
                KEY ("Pass", ch->pcdata->pwd, fread_string (fp));
                KEY("Pc_build", ch->pcdata->pc_build, fread_number( fp ) );
                KEY("Pc_eyes", ch->pcdata->pc_eyes, fread_number( fp ) );
                KEY("Pc_hair", ch->pcdata->pc_hair, fread_number( fp ) );
                KEY("Pc_looks", ch->pcdata->pc_looks, fread_number( fp ) );
 		KEY ("Petit",	ch->petition,	clan_lookup(fread_string(fp)));
		KEY ("Pdeath", ch->pdeath, fread_number(fp));
		KEY ("Pkill", ch->pkill, fread_number(fp));
                KEY ("Plat", ch->platinum, fread_number ( fp ) );
                KEY ("Played", ch->played, fread_number (fp));
                KEY ("Plyd", ch->played, fread_number (fp));
                KEY ("PlayerID",    ch->pcdata->playerid,   fread_number( fp ) );
                KEY ("Points", ch->pcdata->points, fread_number (fp));
                KEY ("Pnts", ch->pcdata->points, fread_number (fp));
		KEY ("Polyaff", ch->polyaff, fread_number(fp));
                KEY ("Position", ch->position, fread_number (fp));
                KEY ("Pos", ch->position, fread_number (fp));
                if (!str_cmp(word, "Power"))
                {
                for (sn = 0; sn < 20; sn++)
                ch->pcdata->powers[sn] = fread_number(fp);
                fMatch = TRUE;
                break;
                }
                KEY ("Practice", ch->practice, fread_number (fp));
                KEY ("Prac", ch->practice, fread_number (fp));
                KEY ("Prompt", ch->prompt, fread_string (fp));
                KEY ("Prom", ch->prompt, fread_string (fp));
                break;

        case 'Q':
            KEY( "QuestPnts",   ch->questpoints,        fread_number( fp ) );
            KEY( "QuestPoints", ch->pcdata->questpoints,        fread_number( fp ) );
            KEY( "QuestNext",   ch->nextquest,          fread_number( fp ) );
	    KEY( "Questcomp",   ch->questscomp,		fread_number( fp ) );
            break;

            case 'R':
                KEY ("Race", ch->race, race_lookup (fread_string (fp)));
		KEY( "Rank", ch->rank, fread_number( fp ) );

                if (!str_cmp (word, "Room"))
                {
                    ch->in_room = get_room_index (fread_number (fp));
                    if (ch->in_room == NULL)
                        ch->in_room = get_room_index (ROOM_VNUM_LIMBO);
                    fMatch = TRUE;
                    break;
                }
                if ( !str_cmp ( word, "RoomRLE" ) )
                {
                    fread_rle ( ch->pcdata->explored, fp );
                    fMatch = TRUE;
                    break;
                }


                break;

            case 'S':
                KEY ("SavingThrow", ch->saving_throw, fread_number (fp));
                KEY ("Save", ch->saving_throw, fread_number (fp));
                KEY ("Scro", ch->lines, fread_number (fp));
                KEY ("Sex", ch->sex, fread_number (fp));
                KEY ("ShortDescr", ch->short_descr, fread_string (fp));
                KEY ("ShD", ch->short_descr, fread_string (fp));
                KEY ("Sec", ch->pcdata->security, fread_number (fp));    /* OLC */
                KEY ("Silv", ch->silver, fread_number (fp));
								KEY ("Silv_bank", ch->pcdata->silver_bank,  fread_number(fp));
                KEY ( "Shares", ch->pcdata->shares, fread_number ( fp ) );
                if (!str_cmp (word, "Skill") || !str_cmp (word, "Sk"))
                {
                    int sn;
                    int value;
                    char *temp;

                    value = fread_number (fp);
                    temp = fread_word (fp);
                    sn = skill_lookup (temp);
                    /* sn    = skill_lookup( fread_word( fp ) ); */
                    if (sn < 0)
                    {
                        fprintf (stderr, "%s", temp);
                        bug ("Fread_char: unknown skill. ", 0);
                    }
                    else
                        ch->pcdata->learned[sn] = value;
                    fMatch = TRUE;
                }

                KEY ("Spellpower", ch->spellpower, fread_number (fp));
                KEYS ( "Spouse", ch->pcdata->spouse, fread_string ( fp ) );

		if (!str_cmp( word, "Stance" ) )
            {
                ch->stance[0] = fread_number( fp );
                ch->stance[1] = fread_number( fp );
                ch->stance[2] = fread_number( fp );
                ch->stance[3] = fread_number( fp );
                ch->stance[4] = fread_number( fp );
                ch->stance[5] = fread_number( fp );
                ch->stance[6] = fread_number( fp );
                ch->stance[7] = fread_number( fp );
                ch->stance[8] = fread_number( fp );
                ch->stance[9] = fread_number( fp );
                ch->stance[10] = fread_number( fp );
                fMatch = TRUE;
                break;
            }
		KEY ("Status", ch->status, fread_number ( fp ));
		KEY ("SubLevel", ch->sublevel, fread_number (fp));

                break;

            case 'T':
                KEY ("Tier", ch->tier, fread_number (fp));
                KEY ("TrueSex", ch->pcdata->true_sex, fread_number (fp));
                KEY ("TSex", ch->pcdata->true_sex, fread_number (fp));
                KEY ("Trai", ch->train, fread_number (fp));
                KEY ("Trust", ch->trust, fread_number (fp));
                KEY ("Tru", ch->trust, fread_number (fp));

                if (!str_cmp (word, "Title") || !str_cmp (word, "Titl"))
                {
                    ch->pcdata->title = fread_string (fp);
                    if (ch->pcdata->title[0] != '.'
                        && ch->pcdata->title[0] != ','
                        && ch->pcdata->title[0] != '!'
                        && ch->pcdata->title[0] != '?')
                    {
                        sprintf (buf, " %s", ch->pcdata->title);
                        free_string (ch->pcdata->title);
                        ch->pcdata->title = str_dup (buf);
                    }
                    fMatch = TRUE;
                    break;
                }

                break;

            case 'V':
                KEY ("Version", ch->version, fread_number (fp));
                KEY ("Vers", ch->version, fread_number (fp));
                if (!str_cmp (word, "Vnum"))
                {
                    ch->pIndexData = get_mob_index (fread_number (fp));
                    fMatch = TRUE;
                    break;
                }
                break;

            case 'W':
		KEY ("Weight", ch->weight, fread_number (fp));
                KEY ("Wimpy", ch->wimpy, fread_number (fp));
                KEY ("Wimp", ch->wimpy, fread_number (fp));
                KEY ("Wizn", ch->wiznet, fread_flag (fp));
                break;
        }

        if (!fMatch)
        {
            bug ("Fread_char: no match.", 0);
            bug (word, 0);
            fread_to_eol (fp);
        }
    }
}

/* load a pet from the forgotten reaches */
void fread_pet (CHAR_DATA * ch, FILE * fp)
{
    char *word;
    CHAR_DATA *pet;
    bool fMatch;
    int lastlogoff = current_time;
    int percent;
    int vnum = 0;

    /* first entry had BETTER be the vnum or we barf */
    word = feof (fp) ? "END" : fread_word (fp);
    if (!str_cmp (word, "Vnum"))
    {

        vnum = fread_number (fp);
        if (get_mob_index (vnum) == NULL)
        {
            bug ("Fread_pet: bad vnum %d.", vnum);
            pet = create_mobile (get_mob_index (MOB_VNUM_FIDO));
        }
        else
            pet = create_mobile (get_mob_index (vnum));
    }
    else
    {
        bug ("Fread_pet: no vnum in file.", 0);
        pet = create_mobile (get_mob_index (MOB_VNUM_FIDO));
    }

    for (;;)
    {
        word = feof (fp) ? "END" : fread_word (fp);
        fMatch = FALSE;

        switch (UPPER (word[0]))
        {
            case '*':
                fMatch = TRUE;
                fread_to_eol (fp);
                break;

            case 'A':
                KEY ("Act", pet->act, fread_flag (fp));
                KEY ("AfBy", pet->affected_by, fread_flag (fp));
                KEY ("Alig", pet->alignment, fread_number (fp));

                if (!str_cmp (word, "ACs"))
                {
                    int i;

                    for (i = 0; i < 4; i++)
                        pet->armor[i] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "AffD"))
                {
                    AFFECT_DATA *paf;
                    int sn;

                    paf = new_affect ();

                    sn = skill_lookup (fread_word (fp));
                    if (sn < 0)
                        bug ("Fread_char: unknown skill.", 0);
                    else
                        paf->type = sn;

                    paf->level = fread_number (fp);
                    paf->duration = fread_number (fp);
                    paf->modifier = fread_number (fp);
                    paf->location = fread_number (fp);
                    paf->bitvector = fread_number (fp);
                    paf->next = pet->affected;
                    pet->affected = paf;
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "Affc"))
                {
                    AFFECT_DATA *paf;
                    int sn;

                    paf = new_affect ();

                    sn = skill_lookup (fread_word (fp));
                    if (sn < 0)
                        bug ("Fread_char: unknown skill.", 0);
                    else
                        paf->type = sn;

                    paf->where = fread_number (fp);
                    paf->level = fread_number (fp);
                    paf->duration = fread_number (fp);
                    paf->modifier = fread_number (fp);
                    paf->location = fread_number (fp);
                    paf->bitvector = fread_number (fp);
					/* Added here after Chris Litchfield (The Mage's Lair)
					 * pointed out a bug with duplicating affects in saved
					 * pets. -- JR 2002/01/31
					 */
					if (!check_pet_affected(vnum,paf))
					{
						paf->next       = pet->affected;
						pet->affected   = paf;
					} else{
						free_affect(paf);
					}
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "AMod"))
                {
                    int stat;

                    for (stat = 0; stat < MAX_STATS; stat++)
                        pet->mod_stat[stat] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "Attr"))
                {
                    int stat;

                    for (stat = 0; stat < MAX_STATS; stat++)
                        pet->perm_stat[stat] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }
                break;

            case 'C':
                KEY ("Clan", pet->clan, clan_lookup (fread_string (fp)));
                KEY ("Comm", pet->comm, fread_flag (fp));
                break;

            case 'D':
                KEY ("Dam", pet->damroll, fread_number (fp));
                KEY ("Desc", pet->description, fread_string (fp));
                KEY( "Detect",	pet->detection,		fread_flag(fp));
                break;

            case 'E':
                if (!str_cmp (word, "End"))
                {
                    pet->leader = ch;
                    pet->master = ch;
                    ch->pet = pet;
                    /* adjust hp mana move up  -- here for speed's sake */
                    percent =
                        (current_time - lastlogoff) * 25 / (2 * 60 * 60);

                    if (percent > 0 && !IS_AFFECTED (ch, AFF_POISON)
                        && !IS_AFFECTED (ch, AFF_PLAGUE))
                    {
                        percent = UMIN (percent, 100);
                        pet->hit += (pet->max_hit - pet->hit) * percent / 100;
                        pet->mana +=
                            (pet->max_mana - pet->mana) * percent / 100;
                        pet->move +=
                            (pet->max_move - pet->move) * percent / 100;
                    }
                    return;
                }
                KEY ("Exp", pet->exp, fread_number (fp));
                break;

            case 'G':
                KEY ("Gold", pet->gold, fread_number (fp));
                break;

            case 'H':
                KEY ("Hit", pet->hitroll, fread_number (fp));

                if (!str_cmp (word, "HMV"))
                {
                    pet->hit = fread_number (fp);
                    pet->max_hit = fread_number (fp);
                    pet->mana = fread_number (fp);
                    pet->max_mana = fread_number (fp);
                    pet->move = fread_number (fp);
                    pet->max_move = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }
                break;

            case 'L':
                KEY ("Levl", pet->level, fread_number (fp));
                KEY ("LnD", pet->long_descr, fread_string (fp));
                KEY ("LogO", lastlogoff, fread_number (fp));
                break;

            case 'N':
                KEY ("Name", pet->name, fread_string (fp));
                break;

            case 'P':
                KEY ("Plat", pet->platinum, fread_number ( fp ) );
                KEY ("Pos", pet->position, fread_number (fp));		
	    if (!str_cmp( word, "PetStance" ) )
            {
                pet->stance[0] = fread_number( fp );
                pet->stance[1] = fread_number( fp );
                pet->stance[2] = fread_number( fp );
                pet->stance[3] = fread_number( fp );
                pet->stance[4] = fread_number( fp );
                pet->stance[5] = fread_number( fp );
                pet->stance[6] = fread_number( fp );
                pet->stance[7] = fread_number( fp );
                pet->stance[8] = fread_number( fp );
                pet->stance[9] = fread_number( fp );
                pet->stance[10] = fread_number( fp );
                fMatch = TRUE;
                break;
            }
                break;

            case 'R':
                KEY ("Race", pet->race, race_lookup (fread_string (fp)));
                break;

            case 'S':
                KEY ("Save", pet->saving_throw, fread_number (fp));
                KEY ("Sex", pet->sex, fread_number (fp));
                KEY ("ShD", pet->short_descr, fread_string (fp));
                KEY ("Silv", pet->silver, fread_number (fp));
                break;

                if (!fMatch)
                {
                    bug ("Fread_pet: no match.", 0);
                    fread_to_eol (fp);
                }

        }
    }
}

extern OBJ_DATA *obj_free;

void fread_obj (CHAR_DATA * ch, FILE * fp)
{
    OBJ_DATA *obj;
    char *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;
    bool first;
    bool new_format;            /* to prevent errors */
    bool make_new;                /* update object */

    fVnum = FALSE;
    obj = NULL;
    first = TRUE;                /* used to counter fp offset */
    new_format = FALSE;
    make_new = FALSE;

    word = feof (fp) ? "End" : fread_word (fp);
    if (!str_cmp (word, "Vnum"))
    {
        int vnum;
        first = FALSE;            /* fp will be in right place */

        vnum = fread_number (fp);
        if (get_obj_index (vnum) == NULL)
        {
            bug ("Fread_obj: bad vnum %d.", vnum);
        }
        else
        {
            obj = create_object (get_obj_index (vnum), -1);
            new_format = TRUE;
        }

    }

    if (obj == NULL)
    {                            /* either not found or old style */
        obj = new_obj ();
        obj->name = str_dup ("");
        obj->short_descr = str_dup ("");
        obj->description = str_dup ("");
    }

    fNest = FALSE;
    fVnum = TRUE;
    iNest = 0;

    for (;;)
    {
        if (first)
            first = FALSE;
        else
            word = feof (fp) ? "End" : fread_word (fp);
        fMatch = FALSE;

        switch (UPPER (word[0]))
        {
            case '*':
                fMatch = TRUE;
                fread_to_eol (fp);
                break;

            case 'A':
                if (!str_cmp (word, "AffD"))
                {
                    AFFECT_DATA *paf;
                    int sn;

                    paf = new_affect ();

                    sn = skill_lookup (fread_word (fp));
                    if (sn < 0)
                        bug ("Fread_obj: unknown skill.", 0);
                    else
                        paf->type = sn;

                    paf->level = fread_number (fp);
                    paf->duration = fread_number (fp);
                    paf->modifier = fread_number (fp);
                    paf->location = fread_number (fp);
                    paf->bitvector = fread_number (fp);
                    paf->next = obj->affected;
                    obj->affected = paf;
                    fMatch = TRUE;
                    break;
                }
                if (!str_cmp (word, "Affc"))
                {
                    AFFECT_DATA *paf;
                    int sn;

                    paf = new_affect ();

                    sn = skill_lookup (fread_word (fp));
                    if (sn < 0)
                        bug ("Fread_obj: unknown skill.", 0);
                    else
                        paf->type = sn;

                    paf->where = fread_number (fp);
                    paf->level = fread_number (fp);
                    paf->duration = fread_number (fp);
                    paf->modifier = fread_number (fp);
                    paf->location = fread_number (fp);
                    paf->bitvector = fread_number (fp);
                    paf->next = obj->affected;
                    obj->affected = paf;
                    fMatch = TRUE;
                    break;
                }
                break;

            case 'C':
                KEY ("Cond", obj->condition, fread_number (fp));
                KEY ("Cost", obj->cost, fread_number (fp));
                break;

            case 'D':
                KEY ("Description", obj->description, fread_string (fp));
                KEY ("Desc", obj->description, fread_string (fp));
                break;

            case 'E':

                if (!str_cmp (word, "Enchanted"))
                {
                    obj->enchanted = TRUE;
                    fMatch = TRUE;
                    break;
                }

                KEY ("ExtraFlags", obj->extra_flags, fread_number (fp));
                KEY ("ExtF", obj->extra_flags, fread_number (fp));

                if (!str_cmp (word, "ExtraDescr") || !str_cmp (word, "ExDe"))
                {
                    EXTRA_DESCR_DATA *ed;

                    ed = new_extra_descr ();

                    ed->keyword = fread_string (fp);
                    ed->description = fread_string (fp);
                    ed->next = obj->extra_descr;
                    obj->extra_descr = ed;
                    fMatch = TRUE;
                }

                if (!str_cmp (word, "End"))
                {
                    if (!fNest || (fVnum && obj->pIndexData == NULL))
                    {
                        bug ("Fread_obj: incomplete object.", 0);
                        free_obj (obj);
                        return;
                    }
                    else
                    {
                        if (!fVnum)
                        {
                            free_obj (obj);
                            obj =
                                create_object (get_obj_index (OBJ_VNUM_DUMMY),
                                               0);
                        }

                        if (!new_format)
                        {
                            obj->next = object_list;
                            object_list = obj;
                            obj->pIndexData->count++;
                        }

                        if (!obj->pIndexData->new_format
                            && obj->item_type == ITEM_ARMOR
                            && obj->value[1] == 0)
                        {
                            obj->value[1] = obj->value[0];
                            obj->value[2] = obj->value[0];
                        }
                        if (make_new)
                        {
                            int wear;

                            wear = obj->wear_loc;
                            extract_obj (obj);

                            obj = create_object (obj->pIndexData, 0);
                            obj->wear_loc = wear;
                        }
                        if (iNest == 0 || rgObjNest[iNest] == NULL)
                            obj_to_char (obj, ch);
                        else
                            obj_to_obj (obj, rgObjNest[iNest - 1]);
                        return;
                    }
                }
                break;

            case 'I':
                KEY ("ItemType", obj->item_type, fread_number (fp));
                KEY ("Ityp", obj->item_type, fread_number (fp));
                break;

            case 'L':
                KEY ("Level", obj->level, fread_number (fp));
                KEY ("Lev", obj->level, fread_number (fp));
                break;

            case 'N':
                KEY ("Name", obj->name, fread_string (fp));

                if (!str_cmp (word, "Nest"))
                {
                    iNest = fread_number (fp);
                    if (iNest < 0 || iNest >= MAX_NEST)
                    {
                        bug ("Fread_obj: bad nest %d.", iNest);
                    }
                    else
                    {
                        rgObjNest[iNest] = obj;
                        fNest = TRUE;
                    }
                    fMatch = TRUE;
                }
                break;

            case 'O':
                if (!str_cmp (word, "Oldstyle"))
                {
                    if (obj->pIndexData != NULL
                        && obj->pIndexData->new_format)
                        make_new = TRUE;
                    fMatch = TRUE;
                }
            KEY( "OwnerID",     obj->ownerid,           fread_number( fp ) );
                break;


	case 'Q':
	    KEY( "Questowner",  obj->questowner,        fread_string( fp ) );
	    break;


            case 'S':
                KEY ("ShortDescr", obj->short_descr, fread_string (fp));
                KEY ("ShD", obj->short_descr, fread_string (fp));

                if (!str_cmp (word, "Spell"))
                {
                    int iValue;
                    int sn;

                    iValue = fread_number (fp);
                    sn = skill_lookup (fread_word (fp));
                    if (iValue < 0 || iValue > 3)
                    {
                        bug ("Fread_obj: bad iValue %d.", iValue);
                    }
                    else if (sn < 0)
                    {
                        bug ("Fread_obj: unknown skill.", 0);
                    }
                    else
                    {
                        obj->value[iValue] = sn;
                    }
                    fMatch = TRUE;
                    break;
                }

                break;

            case 'T':
                KEY ("Timer", obj->timer, fread_number (fp));
                KEY ("Time", obj->timer, fread_number (fp));
                break;

            case 'V':
                if (!str_cmp (word, "Values") || !str_cmp (word, "Vals"))
                {
                    obj->value[0] = fread_number (fp);
                    obj->value[1] = fread_number (fp);
                    obj->value[2] = fread_number (fp);
                    obj->value[3] = fread_number (fp);
                    if (obj->item_type == ITEM_WEAPON && obj->value[0] == 0)
                        obj->value[0] = obj->pIndexData->value[0];
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "Val"))
                {
                    obj->value[0] = fread_number (fp);
                    obj->value[1] = fread_number (fp);
                    obj->value[2] = fread_number (fp);
                    obj->value[3] = fread_number (fp);
                    obj->value[4] = fread_number (fp);
                    fMatch = TRUE;
                    break;
                }

                if (!str_cmp (word, "Vnum"))
                {
                    int vnum;

                    vnum = fread_number (fp);
                    if ((obj->pIndexData = get_obj_index (vnum)) == NULL)
                        bug ("Fread_obj: bad vnum %d.", vnum);
                    else
                        fVnum = TRUE;
                    fMatch = TRUE;
                    break;
                }
                break;

            case 'W':
                KEY ("WearFlags", obj->wear_flags, fread_number (fp));
                KEY ("WeaF", obj->wear_flags, fread_number (fp));
                KEY ("WeaponCurrentXP", obj->weapon_currentxp, fread_number (fp));
		KEY ("WeaponLevel", obj->weapon_level, fread_number (fp));
                KEY ("WeaponPoints", obj->weapon_points, fread_number (fp));
                KEY ("WearLoc", obj->wear_loc, fread_number (fp));
                KEY ("Wear", obj->wear_loc, fread_number (fp));
                KEY ("Weight", obj->weight, fread_number (fp));
                KEY ("Wt", obj->weight, fread_number (fp));
                break;

        }

        if (!fMatch)
        {
            bug ("Fread_obj: no match.", 0);
            fread_to_eol (fp);
        }
    }
}

bool char_exists( bool backup, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    bool found = FALSE;

    fclose( fpReserve );
   if (backup)
        sprintf( buf, "%sbackup/%s", PLAYER_DIR, capitalize( argument ) );
    else
        sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen( buf, "r" ) ) != NULL )
    {
        found = TRUE;
        fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}

void do_finger( CHAR_DATA *ch, char *argument )
{
    char strsave[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char letter;
    CHAR_DATA *victim;
    FILE *fp;
    char *word;
    bool fMatch;
    bool drop_out = FALSE;

    char *f_name	= capitalize( argument );
    char *f_title	= "";
    int   f_sex		= 0;
    int   f_level       = 0;
    int   f_level2       = 0;
    int   f_level3       = 0;
    int   f_level4       = 0;
    int   f_sublevel    = 0;
    int   f_race	= race_lookup("");
    int	  f_class	= class_lookup("");
    int   f_class2       = class_lookup("");
    int   f_class3       = class_lookup("");
    int   f_class4       = class_lookup("");
    char *f_lasthost	= "";

    if ( IS_NPC(ch) ) return;

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }
        sprintf( strsave, "%s%s", PLAYER_DIR, capitalize(argument) );
        if ( ( fp = fopen( strsave, "r" ) ) == NULL )
        {
            send_to_char( "That player doesn't exist.\n\r", ch );
            return;
        }



//    if ( ( victim = get_char_finger( ch, argument ) ) != NULL )
    victim = strsave;
    if ( ( victim == argument) )
    {
	f_title		= victim->pcdata->title;
	f_sex		= victim->sex;
	f_race		= victim->race;
	f_class		= victim->class;
        f_class2         = victim->class;
        f_class3         = victim->class;
        f_class4         = victim->class;
        f_level         = victim->level;
        f_level2         = victim->level;
        f_level3         = victim->level;
        f_level4         = victim->level;
	f_sublevel      = victim->sublevel;
	f_lasthost	= victim->lasthost;
    }
/*
  else
    {
	if ( !char_exists(PLAYER_DIR,argument) )
	{
	    send_to_char( "That player doesn't exist.\n\r", ch );
	    return;
	}
*/
{
	fflush( fpReserve );
	fclose( fpReserve );
	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize(argument) );
	if ( ( fp = fopen( strsave, "r" ) ) != NULL )
	{
	    if ( ( letter = fread_letter( fp ) ) == EOF)
	    {
		send_to_char("Their playerfile has been corrupted.\n\r",ch);
		fflush( fp );
		fclose( fp );
		fpReserve = fopen( NULL_FILE, "r" );
		return;
	    }
	    fread_to_eol( fp );

	    while ( !drop_out )
	    {
		word   = feof( fp ) ? "End" : fread_word( fp );
		fMatch = FALSE;

		switch ( UPPER(word[0]) )
		{
		default:
		case '*':
		    fMatch = TRUE;
		    fread_to_eol( fp );
		    break;

		case 'C':
			KEY( "Cla",		f_class,	fread_number( fp ) );
                        KEY( "Cla2",             f_class2,        fread_number( fp ) );
                        KEY( "Cla3",             f_class3,        fread_number( fp ) );
                        KEY( "Cla4",             f_class4,        fread_number( fp ) );
			KEY( "Class",	f_class,	fread_number( fp ) );
			break;

		case 'E':
		    if ( !str_cmp( word, "End" ) ) drop_out = TRUE;
		    break;


		case 'L':
			KEY( "Lasthost",	f_lasthost,	fread_string( fp ) );
	                KEY( "Levl",           f_level,        fread_number (fp));
                        KEY( "Levl2",           f_level2,        fread_number (fp));
                        KEY( "Levl3",           f_level3,        fread_number (fp));
                        KEY( "Levl4",           f_level4,        fread_number (fp));
		    break;

		case 'R':
		    KEY ("Race", f_race, race_lookup (fread_string (fp)));
		    break;

		case 'S':
		    KEY( "Sex",		f_sex,			fread_number( fp ) );
                    KEY( "SubLevel",         f_sublevel,                  fread_number( fp ) );

		    break;

		case 'T':
			if (!str_cmp (word, "Title") || !str_cmp (word, "Titl"))
                {
                    f_title = fread_string (fp);
                    if (ch->pcdata->title[0] != '.'
                        && ch->pcdata->title[0] != ','
                        && ch->pcdata->title[0] != '!'
                        && ch->pcdata->title[0] != '?')
                    {
                        sprintf (buf, " %s", f_title);
                        free_string (f_title);
                        f_title = str_dup (buf);
                    }
                    fMatch = TRUE;
                    break;
                }
		    break;
		}
		if ( !fMatch ) fread_to_eol( fp );
	    }
	}
	else
	{
	    bug( "Do_finger: fopen", 0 );
	    perror( strsave );
	}
	fflush( fp );
	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
    }
    
send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    sprintf( buf, "%s%s%s.\n\r", f_name, drop_out ? "": "", f_title );
    send_to_char( buf, ch );
    
send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	sprintf( buf, "Last Connected from: %s\n\r",
	IS_IMMORTAL(ch) ? f_lasthost : "^^^^.^^^^.^^^^.^^^^" );
	send_to_char(buf, ch );
    sprintf( buf, "Sex: %s \n\r", 
	f_sex == SEX_MALE ? "Male" : f_sex == SEX_FEMALE ? "Female" : "Neither");
    send_to_char( buf, ch );

	sprintf( buf, "Class: %s ",
	class_table[f_class].finger_name  );
	send_to_char(buf, ch);
        sprintf( buf, "[%d]\n\r", f_level );
        send_to_char(buf, ch);
	
	if (f_class2 != -1)
	{
        sprintf( buf, "Class 2: %s ",
        class_table[f_class2].finger_name  );
        send_to_char(buf, ch);
        sprintf( buf, "[%d]\n\r", f_level2 );
        send_to_char(buf, ch);
	}

        if (f_class3 != -1)
	{
        sprintf( buf, "Class 3: %s ",
        class_table[f_class3].finger_name  );
        send_to_char(buf, ch);
        sprintf( buf, "[%d]\n\r", f_level3 );
        send_to_char(buf, ch);
	}

        if (f_class4 != -1)
	{
        sprintf( buf, "Class 4: %s ",
        class_table[f_class4].finger_name  );
        send_to_char(buf, ch);
        sprintf( buf, "[%d]\n\r", f_level4 );
        send_to_char(buf, ch);
	}


    sprintf( buf, "Race: %s\n\r",
       pc_race_table[f_race].who_name );
       send_to_char(buf, ch);
    sprintf( buf, "SubLevel: %d\n\r", f_sublevel );
       send_to_char(buf, ch);


    
send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    return;
}

