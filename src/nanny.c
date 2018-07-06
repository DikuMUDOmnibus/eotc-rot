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
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
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

/****************************************************************************
 *   This file is just the stock nanny() function ripped from comm.c. It    *
 *   seems to be a popular task for new mud coders, so what the heck?       *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>                /* OLC -- for close read write etc */
#include <stdarg.h>                /* printf_to_char */

#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"

#if    defined(macintosh) || defined(MSDOS)
extern const char echo_off_str[];
extern const char echo_on_str[];
extern const char go_ahead_str[];
#endif

#if    defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
extern const char echo_off_str[];
extern const char echo_on_str[];
extern const char go_ahead_str[];
#endif

/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos args ((void));
bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
bool write_to_descriptor args ((int desc, char *txt, int length));
#endif

#if defined(unix)
void game_loop_unix args ((int control));
int init_socket args ((int port));
void init_descriptor args ((int control));
bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
bool write_to_descriptor args ((int desc, char *txt, int length));
#endif

/*
 *  * Other local functions (OS-independent).
 *   */
bool check_parse_name args ((char *name));
bool check_reconnect args ((DESCRIPTOR_DATA * d, char *name, bool fConn));
bool check_playing args ((DESCRIPTOR_DATA * d, char *name));

/*
 * Global variables.
 */
extern DESCRIPTOR_DATA *descriptor_list;    /* All open descriptors     */
extern DESCRIPTOR_DATA *d_next;        /* Next descriptor in loop  */
extern FILE *fpReserve;                /* Reserved file handle     */
extern bool god;                        /* All new chars are gods!  */
extern bool merc_down;                    /* Shutdown         */
extern bool wizlock;                    /* Game is wizlocked        */
extern bool newlock;                    /* Game is newlocked        */
extern char str_boot_time[MAX_INPUT_LENGTH];
extern time_t current_time;            /* time of this pulse */


/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny (DESCRIPTOR_DATA * d, char *argument)
{
    DESCRIPTOR_DATA *d_old, *d_next;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    char *pwdnew, *option;
    char *p;
    bool fOld;
    int iClass, race, i, weapon, god;
    extern int mud_telnetga, mud_ansicolor;
    char reRoll[MSL];
    int nStr[3], nInt[3], nWis[3], nCon[3], nDex[3];
    int nVit[3], nDis[3], nAgi[3], nCha[3], nLuc[3];
    int j;
    char pickRace[MSL];
    char pickClass[MSL];
    char rollStats[MSL];
    char sAlign[MSL], sHome[MSL], sCustom[MSL], sWeapon[MSL];

    /* Delete leading spaces UNLESS character is writing a note */
	if (d->connected != CON_NOTE_TEXT)
	{
		while ( isspace(*argument) )
			argument++;
	}
    ch = d->character;

    switch (d->connected)
    {

        default:
            bug ("Nanny: bad d->connected %d.", d->connected);
            close_socket (d);
            return;

        case CON_ANSI:
            if (argument[0] == '\0' || UPPER (argument[0]) == 'Y')
            {
                d->ansi = TRUE;
                send_to_desc ("{RAnsi enabled!{x\n\r", d);
                d->connected = CON_GET_NAME;
                {
                    extern char *help_greeting;
                    if (help_greeting[0] == '.')
                        send_to_desc (help_greeting + 1, d);
                    else
                        send_to_desc (help_greeting, d);
                }
                break;
            }

            if (UPPER (argument[0]) == 'N')
            {
                d->ansi = FALSE;
                send_to_desc ("Ansi disabled!\n\r", d);
                d->connected = CON_GET_NAME;
                {
                    extern char *help_greeting;
                    if (help_greeting[0] == '.')
                        send_to_desc (help_greeting + 1, d);
                    else
                        send_to_desc (help_greeting, d);
                }
                break;
            }
            else
            {
                send_to_desc ("Do you want ANSI? (Y/n) ", d);
                return;
            }


        case CON_GET_NAME:
            if (argument[0] == '\0')
            {
                close_socket (d);
                return;
            }

            argument[0] = UPPER (argument[0]);
            if (!check_parse_name (argument))
            {
                send_to_desc ("Illegal name, try another.\n\rName: ", d);
                return;
            }

            fOld = load_char_obj (d, argument);
            ch = d->character;

            if (IS_SET (ch->act, PLR_DENY))
            {
                sprintf (log_buf, "Denying access to %s@%s.", argument,
                         d->host);
                log_string (log_buf);
                send_to_desc ("You are denied access.\n\r", d);
                close_socket (d);
                return;
            }

            if (check_ban (d->host, BAN_PERMIT)
                && !IS_SET (ch->act, PLR_PERMIT))
            {
                send_to_desc ("Your site has been banned from this mud.\n\r",
                              d);
                close_socket (d);
                return;
            }

            if (check_reconnect (d, argument, FALSE))
            {
                fOld = TRUE;
            }
            else
            {
                if (wizlock && !IS_IMMORTAL (ch))
                {
                    send_to_desc ("The game is wizlocked.\n\r", d);
                    close_socket (d);
                    return;
                }
            }

            if (fOld)
            {
                /* Old player */
                send_to_desc ("Password: ", d);
                write_to_buffer (d, echo_off_str, 0);
                d->connected = CON_GET_OLD_PASSWORD;
                return;
            }
            else
            {
                /* New player */
                if (newlock)
                {
                    send_to_desc ("The game is newlocked.\n\r", d);
                    close_socket (d);
                    return;
                }

                if (check_ban (d->host, BAN_NEWBIES))
                {
                    send_to_desc
                        ("New players are not allowed from your site.\n\r",
                         0);
                    close_socket (d);
                    return;
                }

                sprintf (buf, "{BDid I get that right, {W%s {B[{WYes{c/{WNo{B]{x? ", argument);
                send_to_desc (buf, d);
                d->connected = CON_CONFIRM_NEW_NAME;
                return;
            }
            break;

        case CON_GET_OLD_PASSWORD:
#if defined(unix)
            write_to_buffer (d, "\n\r", 2);
#endif

            if (strcmp (crypt (argument, ch->pcdata->pwd), ch->pcdata->pwd))
            {
                send_to_desc ("{RWrong password.{x\n\r", d);
                close_socket (d);
                return;
            }


            write_to_buffer (d, echo_on_str, 0);

            if (check_playing (d, ch->name))
                return;

            if (check_reconnect (d, ch->name, TRUE))
                return;

            sprintf (log_buf, "{B%s{W@{B%s has connected.{x", ch->name, d->host);
            log_string (log_buf);
	    ch->lasthost = str_dup(d->host);
            wiznet (log_buf, NULL, NULL, WIZ_SITES, 0, get_trust (ch));

            if (ch->desc->ansi)
                SET_BIT (ch->act, PLR_COLOUR);
            else
                REMOVE_BIT (ch->act, PLR_COLOUR);

            if (IS_IMMORTAL (ch))
            {
                do_function (ch, &do_help, "imotd");
                d->connected = CON_READ_IMOTD;
            }
            else
            {
                do_function (ch, &do_help, "motd");
                d->connected = CON_READ_MOTD;
            }
            break;

/* RT code for breaking link */

        case CON_BREAK_CONNECT:
            switch (*argument)
            {
                case 'y':
                case 'Y':
                    for (d_old = descriptor_list; d_old != NULL;
                         d_old = d_next)
                    {
                        d_next = d_old->next;
                        if (d_old == d || d_old->character == NULL)
                            continue;

                        if (str_cmp (ch->name, d_old->original ?
                                     d_old->original->name : d_old->
                                     character->name))
                            continue;

                        close_socket (d_old);
                    }
                    if (check_reconnect (d, ch->name, TRUE))
                        return;
                    send_to_desc ("Reconnect attempt failed.\n\rName: ", d);
                    if (d->character != NULL)
                    {
                        free_char (d->character);
                        d->character = NULL;
                    }
                    d->connected = CON_GET_NAME;
                    break;

                case 'n':
                case 'N':
                    send_to_desc ("Name: ", d);
                    if (d->character != NULL)
                    {
                        free_char (d->character);
                        d->character = NULL;
                    }
                    d->connected = CON_GET_NAME;
                    break;

                default:
                    send_to_desc ("Please type Y or N? ", d);
                    break;
            }
            break;

        case CON_CONFIRM_NEW_NAME:
            switch (*argument)
            {
                case 'y':

                case 'Y':
                    sprintf (buf,
                             "{BNew character.{x\n\r{BGive me a password for {W%s{B:{x %s",
                             ch->name, echo_off_str);
                    send_to_desc (buf, d);
                    d->connected = CON_GET_NEW_PASSWORD;
                    if (ch->desc->ansi)
                        SET_BIT (ch->act, PLR_COLOUR);
                    break;

                case 'n':
                case 'N':
                    send_to_desc ("{BOk, what {WIS {Bit, then?{x ", d);
                    free_char (d->character);
                    d->character = NULL;
                    d->connected = CON_GET_NAME;
                    break;

                default:
                    send_to_desc ("Please type Yes or No? ", d);
                    break;
            }
            break;

        case CON_GET_NEW_PASSWORD:
#if defined(unix)
            write_to_buffer (d, "\n\r", 2);
#endif

            if (strlen (argument) < 5)
            {
                send_to_desc
                    ("{RPassword must be at least five characters long.{X\n\r{BPassword:{x ",
                     d);
                return;
            }

            pwdnew = crypt (argument, ch->name);
            for (p = pwdnew; *p != '\0'; p++)
            {
                if (*p == '~')
                {
                    send_to_desc
                        ("{RNew password not acceptable, try again.\n\r{BPassword:{x ",
                         d);
                    return;
                }
            }

            free_string (ch->pcdata->pwd);
            ch->pcdata->pwd = str_dup (pwdnew);
            send_to_desc ("{BPlease retype password:{x ", d);
            d->connected = CON_CONFIRM_NEW_PASSWORD;
            break;

        case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
            write_to_buffer (d, "\n\r", 2);
#endif

            if (strcmp (crypt (argument, ch->pcdata->pwd), ch->pcdata->pwd))
            {
                send_to_desc ("{RPasswords don't match.\n\r{BRetype password:{x ",
                              d);
                d->connected = CON_GET_NEW_PASSWORD;
                return;
            }

            send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
            sprintf(pickRace, "Pick a Race");
            sprintf(pickClass, "Pick a Class");
            sprintf(sAlign, "None");
            sprintf(sHome, "None");
            sprintf(sCustom, "None");
            sprintf(sWeapon, "None");
            sprintf(rollStats, "Stat Roll [{DN/A{x]");
            d->connected = CON_SHOW_MENU;
            break;

        case CON_SHOW_MENU:

                if(str_cmp(pickRace, "Pick a Race") && str_cmp(pickClass, "Pick a Class") && str_cmp(reRoll, "No"))
                    sprintf(rollStats, "Stat Roll [ {RNEED{x]");


                send_to_desc("+---------------------------------------------+\n", d);

                sprintf(buf, "|       Creation Sheet for %-12s       |\n", ch->name);
                send_to_desc(buf, d);

                send_to_desc("+---------------------------------------------+\n", d);

                sprintf(buf, "| R) %-17s | C) %-17s |\n",
                        pickRace, pickClass);
                send_to_desc(buf, d);

                sprintf(buf, "| G) %-17s | S) %-17s |\n",
                        ch->sex == 0 ? "Pick a Gender" : ch->sex == 1 ? "Male" : "Female",
                        string_pad(rollStats,16,FALSE));
                send_to_desc(buf, d);

                sprintf(buf, "| A) %-17s | H) %-17s |\n",
                !str_cmp(sAlign, "None") ? "Choose Alignment" : string_pad(sAlign,16,FALSE),
                !str_cmp(sHome, "None") ? "Pick a Hometown" : string_pad(sHome,16,FALSE) );
                send_to_desc(buf, d);

                sprintf(buf, "| U) %-17s | W) %-17s |\n",
                !str_cmp(sCustom, "None") ? "Customized? [ {RNO{x]" : "Customized? [{GYES{x]",
                !str_cmp(sWeapon, "None") ? "Pick a Weapon" : string_pad(sWeapon,16,FALSE));
                send_to_desc(buf, d);

                send_to_desc("+---------------------------------------------+\n", d);

                send_to_desc("+ D) Complete the Creation Process            |\n", d);

                send_to_desc("+---------------------------------------------+\n", d);

                send_to_desc("Your ALPHABETICAL choice?: ", d);

                d->connected = CON_GET_MENU;
        break;


       case CON_GET_MENU:

                switch(argument[0])
                {
                        case 'a': case 'A':
                                send_to_desc("\n{BYou may be {Wgood{B, {Wneutral{B, or {Wevil{B.{x\n\r",d);
                                send_to_desc("{BWhich alignment \n\r[{WGood{B]\n\r{B[{WNeutral{B]\n\r{B[{WEvil{B]{x\n\rYour Choice: ",d);
                                d->connected = CON_GET_ALIGNMENT;
                        break;

                        case 'r': case 'R':
                                write_to_buffer (d, echo_on_str, 0);
                                send_to_desc ("The following races are available:\n\r  ", d);
/*
                                for (race = 1; race_table[race].name != NULL; race++)
                                {
                                        if (!race_table[race].pc_race)
                                        break;

                                        write_to_buffer (d, race_table[race].name, 0);
                                        write_to_buffer (d, "\n\r", 1);
                                }
*/
            send_to_desc ("{B[ {WHuman        Elf          Dwarf       Giant       ]\n\r  ", d);
            send_to_desc ("{B[ {WDraconian    Gnome        Hobbit      Kender      ] \n\r  ", d);
            send_to_desc ("{B[ {WTroll        Pixie        Half-Elf    Half-Giant  ]\n\r  ", d);
            send_to_desc ("{B[ {WHalf-Orc     Duergar      Minotaur    Centaur     ]\n\r  ", d);
            send_to_desc ("{B[ {WDrow         StormGiant   CloudGiant  FireGiant   ]\n\r  ", d);
            send_to_desc ("{B[ {WFrostGiant   Cyclops      Hydra       Rockseer    ]\n\r  ", d);
            send_to_desc ("{B[ {WSvirfnebli   Arial        Felar       Githyanki   ]\n\r  ", d);
            send_to_desc ("{B[ {WSatyr 						   ]\n\r  ", d);
                                write_to_buffer (d, "\n\r", 0);
                                send_to_desc ("{BWhat is your race [{Whelp for more information{B]{x? ",
                                        d);
                                d->connected = CON_GET_NEW_RACE;
                        break;
                        case 'c': case 'C':
                                send_to_desc("Select a class: {B[{W",d);
                                for (iClass = 0; iClass < MAX_CLASS; iClass++)
                                {
                                        if (iClass > 0)
                                                send_to_desc (" ",d);

                                        send_to_desc (class_table[iClass].name,d);
                                }
                                send_to_desc ("{B]{x:{x ",d);
                                d->connected = CON_GET_NEW_CLASS;
                        break;
                        case 's': case 'S':
                        if(!str_cmp(pickRace, "Pick a Race") || !str_cmp(pickClass, "Pick a Class"))
                        {
                                send_to_desc("You must first choose a RACE and CLASS.\n\n", d);
                                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                                d->connected = CON_SHOW_MENU;
                                break;
                        }
	     ch->perm_stat[STAT_STR] = number_range(8,16);
             ch->perm_stat[STAT_INT] = number_range(8,16);
             ch->perm_stat[STAT_WIS] = number_range(8,16);
             ch->perm_stat[STAT_DEX] = number_range(8,16);
             ch->perm_stat[STAT_CON] = number_range(8,16);
             ch->perm_stat[STAT_VIT] = number_range(8,16);
             ch->perm_stat[STAT_DIS] = number_range(8,16);
             ch->perm_stat[STAT_AGI] = number_range(8,16);
             ch->perm_stat[STAT_CHA] = number_range(8,16);
             ch->perm_stat[STAT_LUC] = number_range(8,16);

                        sprintf(rollStats, "Stat Roll [ {GDONE{x]");
                        send_to_desc("{BYou will now begin to roll your initial stats from the table below.{x\n\n", d);
                                for(j = 0; j < 4; j++)
                                {
                                        nStr[j] = number_range(8, get_curr_stat(ch, STAT_STR));
                                        nInt[j] = number_range(8, get_curr_stat(ch, STAT_INT));
                                        nWis[j] = number_range(8, get_curr_stat(ch, STAT_WIS));
                                        nDex[j] = number_range(8, get_curr_stat(ch, STAT_DEX));
                                        nCon[j] = number_range(8, get_curr_stat(ch, STAT_CON));
                                        nVit[j] = number_range(8, get_curr_stat(ch, STAT_VIT));
                                        nDis[j] = number_range(8, get_curr_stat(ch, STAT_DIS));
                                        nAgi[j] = number_range(8, get_curr_stat(ch, STAT_AGI));
                                        nCha[j] = number_range(8, get_curr_stat(ch, STAT_CHA));
                                        nLuc[j] = number_range(8, get_curr_stat(ch, STAT_LUC));
                                }


                        sprintf(buf, "{BYour maximum stats are: {c[{W%d{c] {c[{W%d{c] {c[{W%d{c] {c[{W%d{c] {c[{W%d{c]{x\n\r",
                             get_curr_stat (ch, STAT_STR),
                             get_curr_stat (ch, STAT_INT),
                             get_curr_stat (ch, STAT_WIS),
                             get_curr_stat (ch, STAT_DEX),
                             get_curr_stat (ch, STAT_CON));
                        send_to_desc(buf, d);
                        sprintf(buf, "{BYour maximum stats are: {c[{W%d{c] {c[{W%d{c] {c[{W%d{c] {c[{W%d{c] {c[{W%d{c]{x\n\r",
                             get_curr_stat (ch, STAT_VIT),
                             get_curr_stat (ch, STAT_DIS),
                             get_curr_stat (ch, STAT_AGI),
                             get_curr_stat (ch, STAT_CHA),
                             get_curr_stat (ch, STAT_LUC));
                        send_to_desc(buf, d);


                        sprintf(buf, "\t%-16s %-3s %-3s %-3s %-3s\n\r", "", "0", "1", "2", "3");
                        send_to_desc(buf, d);

                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Strength", nStr[0], nStr[1], nStr[2], nStr[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Intelligence", nInt[0], nInt[1], nInt[2], nInt[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Wisdom", nWis[0], nWis[1], nWis[2], nWis[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Dexterity", nDex[0], nDex[1], nDex[2], nDex[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Constitution", nCon[0], nCon[1], nCon[2], nCon[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Vitality", nVit[0], nVit[1], nVit[2], nVit[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Discipline", nDis[0], nDis[1], nDis[2], nDis[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Agility", nAgi[0], nAgi[1], nAgi[2], nAgi[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Charisma", nCha[0], nCha[1], nCha[2], nCha[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Luck", nLuc[0], nLuc[1], nLuc[2], nLuc[3] );
                        send_to_desc(buf, d);


                        send_to_desc("\n\r\n\r\t{BPress enter to roll again,\n\r\telse enter number of column {W[{c0{W/{c1{W/{c2{W/{c3{W]{x\n\r",d);

                        send_to_desc("\n{BYour choice?{x: ",d);
                        d->connected = CON_SELECT_NEW_STATS;
                        break;

                case 'g': case 'G':
                        send_to_desc("Please choose a Gender [{BMale{x/{MFemale{x]: ", d);
                        d->connected = CON_GET_NEW_SEX;
                        break;
                break;

                case 'h': case 'H':
                        send_to_desc("{BSelect a Hometown{c:{x\n\r", d);
                        for (i=0;hometown_table[i].name != NULL; ++i)
                        {
                                sprintf(buf,"{B[{W%-15s{B]{x\n\r", hometown_table[i].name );
                                send_to_desc(buf, d);
                        }
                                send_to_desc("\n\r{BWhat's your hometown? {x", d);
                                d->connected = CON_GET_HOMETOWN;
                break;

                case 'u': case 'U':
                        if(!str_cmp(pickClass, "Pick a Class") || !str_cmp(pickRace, "Pick a Race"))
                        {
                                send_to_desc("Make sure you've chosen a RACE and CLASS.\n", d);
                                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                                d->connected = CON_SHOW_MENU;
                                break;
                        }

                            send_to_desc ("{BYou will now customize this character.\n\r", d);
                            send_to_desc
                                ("\t{BCustomization takes time, but allows a wider range of skills and abilities.\n\r",
                                 d);
                            d->connected = CON_DEFAULT_CHOICE;
                break;


                case 'w': case 'W':
                        if(!str_cmp(pickClass, "Pick a Class"))
                        {
                                send_to_desc("Make sure you've chosen a CLASS.\n", d);
                                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                                d->connected = CON_SHOW_MENU;
                                break;
                        }
                strcat (buf, "\n\rYour choice? ");
                write_to_buffer (d, buf, 0);
                        write_to_buffer (d, "\n\r", 2);
                        write_to_buffer (d,
                                         "Please pick a weapon from the following choices:\n\r",
                                         0);
                                send_to_desc("Make sure you've chosen a CLASS.\n", d);
                        buf[0] = '\0';
                for (i = 0; weapon_table[i].name != NULL; i++)
                    if (ch->pcdata->learned[*weapon_table[i].gsn] > 0)
                    {
                        send_to_desc ("{B[{W",d);
                        sprintf (buf,weapon_table[i].name);
                        send_to_desc(buf, d);
                        send_to_desc ("{B]{x",d);
                    }
                        d->connected = CON_PICK_WEAPON;
                break;

                case 'd': case 'D':

                        if(!str_cmp(pickClass, "Pick a Class") ||
!str_cmp(!pickRace, "Pick a Race") ||
!str_cmp(!rollStats, "Stat Roll [{DN/A]{x") ||
!str_cmp(!sAlign, "None") ||
!str_cmp(!sHome, "None") ||
ch->sex == 0 ||
!str_cmp(!sWeapon, "None") ||
!str_cmp(!sWeapon, "None"))
                        {
                                send_to_desc("You aren't done creation yet!\n", d);
                                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                                d->connected = CON_SHOW_MENU;
                                break;
                        }

                        write_to_buffer (d, "\n\r", 2);
                        do_function (ch, &do_help, "motd");
                        d->connected = CON_READ_MOTD;
                break;


                        default:
                                send_to_desc("That is not a valid choice!\n{x\n\n", d);
                                return;
                }

        break;



        case CON_GET_NEW_RACE:
            one_argument (argument, arg);

            if (!strcmp (arg, "help"))
            {
                argument = one_argument (argument, arg);
                if (argument[0] == '\0')
                    do_function (ch, &do_help, "race help");
                else
                    do_function (ch, &do_help, argument);
                send_to_desc
                    ("What is your race (help for more information)? ", d);
                break;
            }

            race = race_lookup (argument);

            if (race == 0 || !race_table[race].pc_race)
            {
                send_to_desc ("{RThat is not a valid race.\n\r", d);
                send_to_desc ("{BThe following races are available:{x\n\r  ", d);

/*
                for (race = 1; race_table[race].name != NULL; race++)
                {
                    if (!race_table[race].pc_race)
                        break;
                    send_to_desc("{c", d);
                    write_to_buffer (d, race_table[race].name, 0);
                    write_to_buffer (d, "\n\r{x", 1);
                }
*/
            send_to_desc ("{B[ {WHuman        Elf          Dwarf       Giant       ]\n\r  ", d);
            send_to_desc ("{B[ {WDraconian    Gnome        Hobbit      Kender      ] \n\r  ", d);
            send_to_desc ("{B[ {WTroll        Pixie        Half-Elf    Half-Giant  ]\n\r  ", d);
            send_to_desc ("{B[ {WHalf-Orc     Duergar      Minotaur    Centaur     ]\n\r  ", d);
            send_to_desc ("{B[ {WDrow         StormGiant   CloudGiant  FireGiant   ]\n\r  ", d);
            send_to_desc ("{B[ {WFrostGiant   Cyclops      Hydra       Rockseer    ]\n\r  ", d);
            send_to_desc ("{B[ {WSvirfnebli   Arial        Felar       Githyanki   ]\n\r  ", d);
            send_to_desc ("{B[ {WSatyr                                             ]\n\r  ", d);

                write_to_buffer (d, "\n\r", 0);
                send_to_desc
                    ("{BWhat is your race? [{Whelp for more information{B]{x ", d);
                break;
            }

            ch->race = race;
            sprintf(pickRace, "%s", capitalize(race_table[race].name));
            /* initialize stats */
            for (i = 0; i < MAX_STATS; i++)
                ch->perm_stat[i] = pc_race_table[race].stats[i];
            ch->imm_flags = ch->imm_flags | race_table[race].imm;
            ch->res_flags = ch->res_flags | race_table[race].res;
            ch->vuln_flags = ch->vuln_flags | race_table[race].vuln;
            ch->form = race_table[race].form;
            ch->parts = race_table[race].parts;

            /* add skills */
            for (i = 0; i < 5; i++)
            {
                if (pc_race_table[race].skills[i] == NULL)
                    break;
                group_add (ch, pc_race_table[race].skills[i], FALSE);
            }
            /* add cost */
            ch->pcdata->points = pc_race_table[race].points;
            ch->size = pc_race_table[race].size;

            send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
            d->connected = CON_SHOW_MENU;
            break;

        case CON_GET_NEW_CLASS:
            iClass = class_lookup (argument);

            if (iClass == -1)
            {
                send_to_desc ("{RThat's not a class.\n\r{BWhat IS your class?{x ",
                              d);
                return;
            }

            ch->class = iClass;
            sprintf(pickClass, "%s", capitalize(class_table[ch->class].name));
            ch->pcdata->cname = ch->name;

            sprintf (log_buf, "{B%s{W@{B%s new player.{x", ch->name, d->host);
            log_string (log_buf);
	    ch->lasthost = str_dup(d->host);
            wiznet ("{YNewbie alert! {C$N {Bsighted.{x", ch, NULL, WIZ_NEWBIE, 0, 0);
            wiznet (log_buf, NULL, NULL, WIZ_SITES, 0, get_trust (ch));

            send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
            d->connected = CON_SHOW_MENU;
        break;



        // Initalize the current stats




        case CON_SELECT_NEW_STATS:


                if(!str_cmp(reRoll, "Yes"))
                {

	     ch->perm_stat[STAT_STR] = number_range(8,16);
             ch->perm_stat[STAT_INT] = number_range(8,16);
             ch->perm_stat[STAT_WIS] = number_range(8,16);
             ch->perm_stat[STAT_DEX] = number_range(8,16);
             ch->perm_stat[STAT_CON] = number_range(8,16);
             ch->perm_stat[STAT_VIT] = number_range(8,16);
             ch->perm_stat[STAT_DIS] = number_range(8,16);
             ch->perm_stat[STAT_AGI] = number_range(8,16);
             ch->perm_stat[STAT_CHA] = number_range(8,16);
             ch->perm_stat[STAT_LUC] = number_range(8,16);
                                for(j = 0; j < 4; j++)
                                {
                                        nStr[j] = number_range(8, get_curr_stat(ch, STAT_STR));
                                        nInt[j] = number_range(8, get_curr_stat(ch, STAT_INT));
                                        nWis[j] = number_range(8, get_curr_stat(ch, STAT_WIS));
                                        nDex[j] = number_range(8, get_curr_stat(ch, STAT_DEX));
                                        nCon[j] = number_range(8, get_curr_stat(ch, STAT_CON));
                                        nVit[j] = number_range(8, get_curr_stat(ch, STAT_VIT));
                                        nDis[j] = number_range(8, get_curr_stat(ch, STAT_DIS));
                                        nAgi[j] = number_range(8, get_curr_stat(ch, STAT_AGI));
                                        nCha[j] = number_range(8, get_curr_stat(ch, STAT_CHA));
                                        nLuc[j] = number_range(8, get_curr_stat(ch, STAT_LUC));
                                }

                        sprintf(buf, "{BYour maximum stats are: {c[{W%d{c] {c[{W%d{c] {c[{W%d{c] {c[{W%d{c] {c[{W%d{c]{x\n\r",
                             get_curr_stat (ch, STAT_STR),
                             get_curr_stat (ch, STAT_INT),
                             get_curr_stat (ch, STAT_WIS),
                             get_curr_stat (ch, STAT_DEX),
                             get_curr_stat (ch, STAT_CON));
                        send_to_desc(buf, d);
                        sprintf(buf, "{BYour maximum stats are: {c[{W%d{c] {c[{W%d{c] {c[{W%d{c] {c[{W%d{c] {c[{W%d{c]{x\n\r",
                             get_curr_stat (ch, STAT_VIT),
                             get_curr_stat (ch, STAT_DIS),
                             get_curr_stat (ch, STAT_AGI),
                             get_curr_stat (ch, STAT_CHA),
                             get_curr_stat (ch, STAT_LUC));
                        send_to_desc(buf, d);

                        sprintf(buf, "\t%-16s %-3s %-3s %-3s %-3s\n\r", "", "0", "1", "2", "3");
                        send_to_desc(buf, d);

                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Strength", nStr[0], nStr[1], nStr[2], nStr[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Intelligence", nInt[0], nInt[1], nInt[2], nInt[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Wisdom", nWis[0], nWis[1], nWis[2], nWis[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Dexterity", nDex[0], nDex[1], nDex[2], nDex[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Constitution", nCon[0], nCon[1], nCon[2], nCon[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Vitality", nVit[0], nVit[1], nVit[2], nVit[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Discipline", nDis[0], nDis[1], nDis[2], nDis[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Agility", nAgi[0], nAgi[1], nAgi[2], nAgi[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Charisma", nCha[0], nCha[1], nCha[2], nCha[3] );
                        send_to_desc(buf, d);
                        sprintf(buf, "\t{B%-15s{W: {c%-3d %-3d %-3d %-3d{x\n\r", "Luck", nLuc[0], nLuc[1], nLuc[2], nLuc[3] );
                        send_to_desc(buf, d);


                        send_to_desc("\n\r\n\r\t{BPress enter to roll again,\n\r\telse enter number of column {W[{c0{W/{c1{W/{c2{W/{c3{W]{x\n\r",d);
                        sprintf(rollStats, "No");
                        d->connected = CON_SELECT_NEW_STATS;
                        break;
                }

        switch(argument[0])
        {
                case '0':
            ch->perm_stat[STAT_STR] = nStr[0];
           ch->perm_stat[STAT_INT] = nInt[0];
             ch->perm_stat[STAT_WIS] = nWis[0];
             ch->perm_stat[STAT_DEX] = nDex[0];
             ch->perm_stat[STAT_CON] = nCon[0];
             ch->perm_stat[STAT_VIT] = nVit[0];
             ch->perm_stat[STAT_DIS] = nDis[0];
             ch->perm_stat[STAT_AGI] = nAgi[0];
             ch->perm_stat[STAT_CHA] = nCha[0];
             ch->perm_stat[STAT_LUC] = nLuc[0];
                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                sprintf(reRoll, "No");
                d->connected = CON_SHOW_MENU;
                break;
                case '1':
            ch->perm_stat[STAT_STR] = nStr[1];
           ch->perm_stat[STAT_INT] = nInt[1];
          ch->perm_stat[STAT_WIS] = nWis[1];
         ch->perm_stat[STAT_DEX] = nDex[1];
        ch->perm_stat[STAT_CON] = nCon[1];
             ch->perm_stat[STAT_VIT] = nVit[1];
             ch->perm_stat[STAT_DIS] = nDis[1];
             ch->perm_stat[STAT_AGI] = nAgi[1];
             ch->perm_stat[STAT_CHA] = nCha[1];
             ch->perm_stat[STAT_LUC] = nLuc[1];
                sprintf(rollStats, "Stat Roll [ {GDONE{x]");
                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                sprintf(reRoll, "No");
                d->connected = CON_SHOW_MENU;
        break;
        case '2':
                ch->perm_stat[STAT_STR] = nStr[2];
                ch->perm_stat[STAT_INT] = nInt[2];
                ch->perm_stat[STAT_WIS] = nWis[2];
                ch->perm_stat[STAT_DEX] = nDex[2];
                ch->perm_stat[STAT_CON] = nCon[2];
             ch->perm_stat[STAT_VIT] = nVit[2];
             ch->perm_stat[STAT_DIS] = nDis[2];
             ch->perm_stat[STAT_AGI] = nAgi[2];
             ch->perm_stat[STAT_CHA] = nCha[2];
             ch->perm_stat[STAT_LUC] = nLuc[2];
                sprintf(rollStats, "Stat Roll [ {GDONE{x]");
                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                sprintf(reRoll, "No");
                d->connected = CON_SHOW_MENU;
                break;
                case '3':
            ch->perm_stat[STAT_STR] = nStr[3];
             ch->perm_stat[STAT_INT] = nInt[3];
             ch->perm_stat[STAT_WIS] = nWis[3];
             ch->perm_stat[STAT_DEX] = nDex[3];
             ch->perm_stat[STAT_CON] = nCon[3];
             ch->perm_stat[STAT_VIT] = nVit[3];
             ch->perm_stat[STAT_DIS] = nDis[3];
             ch->perm_stat[STAT_AGI] = nAgi[3];
             ch->perm_stat[STAT_CHA] = nCha[3];
             ch->perm_stat[STAT_LUC] = nLuc[3];
                sprintf(rollStats, "Stat Roll [ {GDONE{x]");
                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                sprintf(reRoll, "No");
                d->connected = CON_SHOW_MENU;
                break;

        default:
                sprintf(reRoll, "Yes");
                send_to_desc("\n{B[ {WHit Enter to Reroll Stats {B]{x \n\n", d);
                d->connected = CON_SELECT_NEW_STATS;
        break;
        }
        break;


        case CON_GET_NEW_SEX:
            switch (argument[0])
            {
                case 'm':
                case 'M':
                    ch->sex = SEX_MALE;
                    ch->pcdata->true_sex = SEX_MALE;
                    send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                    d->connected = CON_SHOW_MENU;
                    break;
                case 'f':
                case 'F':
                    ch->sex = SEX_FEMALE;
                    ch->pcdata->true_sex = SEX_FEMALE;
                    send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                    d->connected = CON_SHOW_MENU;
                    break;
                default:
                    send_to_desc ("{RThat's not a sex.{x\n\r{BWhat IS your sex?{x ",
                                  d);
                    return;
            }
        break;

    case CON_GET_HOMETOWN:
        if (get_hometown(argument) == -1)
        {
                send_to_desc("\n\r{RThat's not a valid selection.{x\n\r",d);
                send_to_desc("{BValid selections are:{x\n\r",d);
                for (i=0;hometown_table[i].name != NULL; ++i)
                {
                        sprintf(buf,"{B[{W%-15s{B]{x\n\r", hometown_table[i].name );
                        send_to_desc( buf, d);
                }
                send_to_desc("\n\r{BWhat's your hometown?{x ", d);
                return;
        }

        ch->hometown = get_hometown(argument);
        sprintf(sHome, "%s", capitalize(hometown_table[ch->hometown].name));
        send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
        d->connected = CON_SHOW_MENU;
        break;

        case CON_GET_ALIGNMENT:
            switch (argument[0])
            {
                case 'g':
                case 'G':
                    ch->alignment = 750;
                    sprintf(sAlign, "{CGOOD{x");
                    break;
                case 'n':
                case 'N':
                    ch->alignment = 0;
                    sprintf(sAlign, "{cNEUTRAL{x");
                    break;
                case 'e':
                case 'E':
                    sprintf(sAlign, "{REVIL{x");
                    ch->alignment = -750;
                    break;
                default:
                    send_to_desc ("{RThat's not a valid alignment.\n\r", d);
                    send_to_desc("{BWhich alignment \n\r[{WGood{B]\n\r{B[{WNeutral{B]\n\r{B[{WEvil{B]{x\n\rYour Choice: ",d);
                    return;
            }

            write_to_buffer (d, "\n\r", 0);

            group_add (ch, "rom basics", FALSE);
            group_add (ch, class_table[ch->class].base_group, FALSE);
            ch->pcdata->learned[gsn_recall] = 50;
            send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
            d->connected = CON_SHOW_MENU;
            break;

        case CON_DEFAULT_CHOICE:
            write_to_buffer (d, "\n\r", 2);
                    ch->gen_data = new_gen_data ();
                    ch->gen_data->points_chosen = ch->pcdata->points;
                    do_function (ch, &do_help, "group header");
                    list_group_costs (ch);
                    write_to_buffer (d,
                                     "You already have the following skills:\n\r",
                                     0);
                    do_function (ch, &do_skills, "");
                    do_function (ch, &do_help, "menu choice");
                    d->connected = CON_GEN_GROUPS;
            break;

        case CON_PICK_WEAPON:
            write_to_buffer (d, "\n\r", 2);
            weapon = weapon_lookup (argument);
            if (weapon == -1
                || ch->pcdata->learned[*weapon_table[weapon].gsn] <= 0)
            {
                send_to_desc (
                                 "{RThat's not a valid selection. Choices are:{x\n\r",
                                 d);
                buf[0] = '\0';
                for (i = 0; weapon_table[i].name != NULL; i++)
                    if (ch->pcdata->learned[*weapon_table[i].gsn] > 0)
                    {
                        send_to_desc ("{B[{W",d);
                        sprintf (buf,weapon_table[i].name);
                        send_to_desc(buf, d);
                        send_to_desc ("{B]{x",d);
                    }
                strcat (buf, "\n\rYour choice? ");
                write_to_buffer (d, buf, 0);
                return;
            }

            ch->pcdata->learned[*weapon_table[weapon].gsn] = 40;
            sprintf(sWeapon, "%-17s", capitalize(weapon_table[weapon].name));
            send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
            d->connected = CON_SHOW_MENU;
            break;

        case CON_GEN_GROUPS:
            send_to_char ("\n\r", ch);

            if (!str_cmp (argument, "done"))
            {
                if (ch->pcdata->points == pc_race_table[ch->race].points)
                {
                    send_to_char ("You didn't pick anything.\n\r", ch);
                    break;
                }

                if (ch->pcdata->points < 40 + pc_race_table[ch->race].points)
                {
                    sprintf (buf,
                             "You must take at least %d points of skills and groups",
                             40 + pc_race_table[ch->race].points);
                    send_to_char (buf, ch);
                    break;
                }

                sprintf (buf, "Creation points: %d\n\r", ch->pcdata->points);
                send_to_char (buf, ch);
                sprintf (buf, "Experience per level: %d\n\r",
                         exp_per_level (ch, ch->gen_data->points_chosen));
                if (ch->pcdata->points < 40)
                    ch->train = (40 - ch->pcdata->points + 1) / 2;
                free_gen_data (ch->gen_data);
                ch->gen_data = NULL;
                send_to_char (buf, ch);
                send_to_desc("\n{B[ {WHit Enter to Continue {B]{x\n\n", d);
                sprintf(sCustom, "Done");
                d->connected = CON_SHOW_MENU;
                break;
            }

            if (!parse_gen_groups (ch, argument))
                send_to_char
                    ("Choices are: list,learned,premise,add,drop,info,help, and done.\n\r",
                     ch);

            do_function (ch, &do_help, "menu choice");
            break;

        case CON_READ_IMOTD:
            write_to_buffer (d, "\n\r", 2);
            do_function (ch, &do_help, "motd");
            d->connected = CON_READ_MOTD;
            break;

                /* states for new note system, (c)1995-96 erwin@pip.dknet.dk */
                /* ch MUST be PC here; have nwrite check for PC status! */

                case CON_NOTE_TO:
                        handle_con_note_to (d, argument);
                        break;

                case CON_NOTE_SUBJECT:
                        handle_con_note_subject (d, argument);
                        break;

                case CON_NOTE_EXPIRE:
                        handle_con_note_expire (d, argument);
                        break;

                case CON_NOTE_TEXT:
                        handle_con_note_text (d, argument);
                        break;

                case CON_NOTE_FINISH:
                        handle_con_note_finish (d, argument);
                        break;

        case CON_READ_MOTD:
            if (ch->pcdata == NULL || ch->pcdata->pwd[0] == '\0')
            {
                write_to_buffer (d, "Warning! Null password!\n\r", 0);
                write_to_buffer (d,
                                 "Please report old password with bug.\n\r",
                                 0);
                write_to_buffer (d,
                                 "Type 'password null <new password>' to fix.\n\r",
                                 0);
            }

            write_to_buffer (d,
                             "\n\rWelcome to ROM 2.4.  Please don't feed the mobiles!\n\r",
                             0);
            ch->next = char_list;
            char_list = ch;
            d->connected = CON_PLAYING;
            reset_char (ch);

            if (ch->level == 0)
            {
                if(mud_ansicolor)
                        SET_BIT (ch->act, PLR_COLOUR);
                if(mud_telnetga)
                        SET_BIT (ch->comm, COMM_TELNET_GA);

                ch->perm_stat[class_table[ch->class].attr_prime] += 3;

                ch->level = 1;
                ch->exp = exp_per_level (ch, ch->pcdata->points);
                ch->hit = ch->max_hit;
                ch->mana = ch->max_mana;
                ch->move = ch->max_move;
                ch->train = 3;
                ch->practice = 5;
                ch->pcdata->spouse = "None";
                sprintf (buf, "the %s", title_table[ch->class][ch->level]
                         [ch->sex == SEX_FEMALE ? 1 : 0]);
                set_title (ch, buf);
                do_function(ch, &do_autoexit, "");

//                do_function (ch, &do_outfit, "");
//                obj_to_char (create_object (get_obj_index (OBJ_VNUM_MAP), 0),
//                           ch);
/* Removed for Hometowns
                char_to_room (ch, get_room_index (ROOM_VNUM_SCHOOL)); */
                char_to_room( ch, get_room_index( hometown_table[ch->hometown].school ) );
                send_to_char ("\n\r", ch);
                do_function (ch, &do_help, "newbie info");
                send_to_char ("\n\r", ch);

 /*
 * Small additions to make character creation way easier, by Rhaelar 08'2004
 */
        creations_today++;
 
	do_get ( ch, "all bag" );
	do_wear ( ch, "sword" );
	do_second ( ch, "sword" );
	do_wear ( ch, "all" ); 
	do_eat ( ch, "spellup" );
	do_drink ( ch, "jug" );
	do_eat ( ch, "mre" );
	do_put ( ch, "all bag" );
	do_stance(ch,"");

}
            else if (ch->in_room != NULL)
            {
                char_to_room (ch, ch->in_room);
            }
            else if (IS_IMMORTAL (ch))
            {
                char_to_room (ch, get_room_index (ROOM_VNUM_CHAT));
            }
            else
            {
                char_to_room (ch, get_room_index (ROOM_VNUM_TEMPLE));
            }

            act ("$n has entered the game.", ch, NULL, NULL, TO_ROOM);
            do_function (ch, &do_look, "auto");
            sprintf(buf, "%s {Dhas entered Eotc Advanced!{x", ch->name);

            sprintf(buf, "{r%s {whas entered {DEye of the Cyclops Advanced.{x", ch->name);
            enter_info(buf);

            wiznet ("$N has left real life behind.", ch, NULL,
                    WIZ_LOGINS, WIZ_SITES, get_trust (ch));

            if (ch->pet != NULL)
            {
                char_to_room (ch->pet, ch->in_room);
                act ("$n has entered the game.", ch->pet, NULL, NULL,
                     TO_ROOM);
            }

                        send_to_char("\n", ch);
            do_function (ch, &do_board, "");
            break;
}
    }

