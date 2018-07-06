/***************************************************************************
*   Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
*   Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
*                                                                          *
*   Merc Diku Mud improvements copyright (C) 1992, 1993 by Michael         *
*   Chastain, Michael Quan, and Mitchell Tse.                              *
*                                                                          *
*       ROM 2.4 is copyright 1993-1995 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@pacinfo.com)                              *
*           Gabrielle Taylor (gtaylor@pacinfo.com)                         *
*           Brian Moore (rom@rom.efn.org)                                  *
*       ROT 2.0 is copyright 1996-1999 by Russ Walsh                       *
*       DRM 1.0a is copyright 2000-2002 by Joshua Chance Blackwell         *
*        SD 2.0 is copyright 2004-2006 by Patrick Mylund Nielsen           *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "db.h"
#include "olc.h"

void do_autosave (CHAR_DATA * ch, char *argument)
{
    if (IS_NPC (ch))
        return;

        if (ch->pcdata->pc_eyes == 0)
        ch->pcdata->pc_eyes = number_range(1,10);
        if (ch->pcdata->pc_looks == 0)
        ch->pcdata->pc_looks = number_range(1,4);
        if (ch->pcdata->pc_hair == 0)
        ch->pcdata->pc_hair = number_range(1,11);
        if (ch->pcdata->pc_build == 0)
        ch->pcdata->pc_build = number_range(1,4);


    save_char_obj (ch);
    if (!IS_SET(ch->comm, COMM_BRIEF3))
    send_to_char ("{R+{Y+ {CA{cu{Ct{co{CS{ca{Cv{ce {Y+{R+{x\n\r", ch);
    return;
}


void do_gmessage ( char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    sprintf ( buf, "%s", argument );

    for ( d = descriptor_list; d; d = d->next )
    {
        if ( d->connected == CON_PLAYING )
        {
            send_to_char ( buf, d->character );
        }
    }

    return;
}

