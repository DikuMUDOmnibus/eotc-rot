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


void    initialize_spell_types args((void));
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

int creations_today;
long connections_today;
long logins_today;
int max_on;
int max_ever;
long commands_today;
long socials_today;
long mobdeaths_today;
long mobkills_today;
int pkills_today;



void do_mudinfo(CHAR_DATA *ch,char *argument) 
{
    char buf[MAX_STRING_LENGTH];    

    sprintf ( buf, "\n\r          {gOn {GEotC{g, there are {G%d{g areas which contain {G%d{g\n\r", top_area, top_room );
    send_to_char(buf, ch);
    sprintf ( buf, "          rooms total. Need assistance? Don't worry, EotC has a big help\n\r" );
    send_to_char(buf, ch);
    sprintf ( buf, "          archive, containing {G%d{g helpfiles, easily accessable with the\n\r", top_help );
    send_to_char(buf, ch);
    sprintf ( buf, "          'help' command. If you feel like socializing, you can show lots\n\r" );
    send_to_char(buf, ch);
    sprintf ( buf, "          of emotions, {G%d{g to be exact. For a list of socials, type 'socials'.\n\r", MAX_SOCIALS);
    send_to_char(buf, ch);
    sprintf ( buf, "\n\r          {gSince the last boot, a total of {G%d{g new player%s have created.\n\r", creations_today, creations_today == 1 ? "" : "s" );
    send_to_char(buf, ch);
    sprintf ( buf, "\n\r          Also since the last boot, a total of {G%ld{g connections have\n\r", connections_today );
    send_to_char(buf, ch);
    sprintf ( buf, "          been made to EotC, and {G%ld{g complete logins have been done.\n\r", logins_today );
    send_to_char(buf, ch);
    sprintf ( buf, "\n\r          The max people online at once today was {G%d{g, the most\n\r", max_on );
    send_to_char(buf, ch);
    sprintf ( buf, "          online ever at once was {G%d{g.\n\r", max_ever );
    send_to_char(buf, ch);
    sprintf ( buf, "\n\r          People have been active, executing {G%ld{g command%s and {G%ld{g social%s,\n\r", commands_today, commands_today == 1 ? "" : "s", socials_today, socials_today == 1 ? "" : "s" );
    send_to_char(buf, ch);
    sprintf ( buf, "          as well as having died to {G%ld{g mobiles and killed {G%ld{g.\n\r", mobdeaths_today, mobkills_today );
    send_to_char(buf, ch);
    sprintf ( buf, "\n\r          {G%d{g real pkill%s %s occured.{x\n\r", 
	pkills_today, pkills_today == 1 ? "" : "s", pkills_today == 1 ? "has" : "have" );
    send_to_char(buf, ch);
    return;
}


/* Slist written by Sirius [o0Sirius0o@hotmail.com]
* Add this to act_info.c, this was written for Rom2.4
* Some of the for loops levels for (level = 1 ;level <= 101;level++)
* may have to be changed depending on your Max Mortal Level.
*/
void do_slist(CHAR_DATA *ch,char *argument) {
char arg1[MIL],group[MSL];
int skill,class = 0,cl = 0;
int sn = 0,col = 0,level = 0, sp,gr;
int skCount = 0,spCount = 0, class_name = 0, gn = 0;
bool found = FALSE;
argument = one_argument(argument,arg1);

    if (arg1[0] == '\0') {
        printf_to_char(ch,"Enter the name of the spell or class.\n\r");
        return;
    }
        if (((skill = skill_lookup(arg1)) == -1) && (cl = class_lookup(arg1)) == -1) {
        printf_to_char(ch,"That is not a valid class or skill.\n\r");
        return;
    }

    if (((skill = skill_lookup(arg1)) == -1) && (cl = class_lookup(arg1)) != -1) {
      for (class_name = 0; class_name < MAX_CLASS; class_name++) {
          if (!str_cmp(arg1, class_table[class_name].name))
            break;
      }
      if (class_name == MAX_CLASS) {
        send_to_char( "Please spell out the full class name.\n\r",ch);
        return;
      } else {
          printf_to_char(ch,"{CAviable skill/spells for the class{c: {W%s{x\n\r", class_table[cl].name);
          printf_to_char(ch,"{cSpells{W:{x\n\r");
          for (level = 1 ;level <= 201;level++) {
              for (sn = 0; sn < MAX_SKILL; sn++) {
                  if (skill_table[sn].skill_level[cl] > 100)
                      continue;
                  if (skill_table[sn].skill_level[cl] != level)
                      continue;
                  if (skill_table[sn].spell_fun == spell_null)
                      continue;
                  printf_to_char(ch, "{B[{C%3d{B] {W%-18s{x",
                  level,
                  skill_table[sn].name);
                  spCount++;
                  if (++col % 3 == 0)
                      send_to_char("\n\r",ch);
              }
          }
          send_to_char("\n\r",ch); col = 0;
          printf_to_char(ch, "{CTotal spells found{g: {W%d{x\n\r",spCount);
          printf_to_char(ch, "{cSkills{W:{x\n\r");
          for (level = 1 ;level <= 201;level++) {
              for (sn = 0; sn < MAX_SKILL; sn++) {
                  if (skill_table[sn].skill_level[cl] > 100)
                      continue;
                  if (skill_table[sn].skill_level[cl] != level)
                      continue;
                  if (skill_table[sn].spell_fun != spell_null)
                      continue;
                  printf_to_char(ch, "{B[{C%3d{B] {W%-18s{x",
                  level,
                  skill_table[sn].name);
                  skCount++;
                  if (++col % 3 == 0)
                      send_to_char("\n\r",ch);
              }
          }
          send_to_char("\n\r",ch);
          printf_to_char(ch, "{cBase Group{W: {W%s{x\n\r",class_table[cl].base_group);
          gn = 0; col = 0; level = 1;
          gn = group_lookup(class_table[class].base_group);
          printf_to_char(ch, "{CSkills included with {W%s{x.{x\n\r",class_table[cl].base_group);
              for (sn = 0; sn < MAX_IN_GROUP; sn++) {
                  if (group_table[gn].spells[sn] == NULL)
                      continue;
                  printf_to_char(ch,"{W%-18s{x",group_table[gn].spells[sn]);
                  if (++col % 3 == 0)
                      send_to_char("\n\r",ch);
              }
          send_to_char("\n\r",ch);
          printf_to_char(ch, "{cDefault Group{W: {W%s{x\n\r",class_table[cl].default_group);
          gn = 0; col = 0; level = 1;
          gn = group_lookup(class_table[cl].default_group);
          printf_to_char(ch, "{CSkills included with {W%s{x.{x\n\r",class_table[cl].default_group);
              for (sn = 0; sn < MAX_IN_GROUP; sn++) {
                  if (group_table[gn].spells[sn] == NULL)
                      continue;
                  printf_to_char(ch,"{W%-18s{x",group_table[gn].spells[sn]);
                  if (++col % 3 == 0)
                      send_to_char("\n\r",ch);
              }
          send_to_char("\n\r",ch);
          printf_to_char(ch, "{CTotal skills found{g: {W%d{x\n\r",skCount);
          printf_to_char(ch, "{WTotal skills/spells found: {B%d{x\n\r",(spCount + skCount));
          return;
      }
    }
    if (((skill = skill_lookup(arg1)) != -1) && (cl = class_lookup(arg1)) == -1) {
        for (gr = 1; gr < MAX_GROUP; gr++) {
          if (group_table[gr].name == NULL) {
            continue;
          }
          for (sp = 0; sp < MAX_IN_GROUP; sp++) {
            if (group_table[gr].spells[sp] == NULL) {
              continue;
            }
            if(group_table[gr].spells[sp] == skill_table[skill].name) {
              found = TRUE;
              sprintf(group,"%s",capitalize(group_table[gr].name));
              break;
            }
          }
        }
        printf_to_char(ch,"{CSettings for skill/spell{c: {W%s{x\n\r",skill_table[skill].name);
        if (found)
          printf_to_char(ch,"{CIn the group: {W%s{C.{x\n\r",group);
        else
          printf_to_char(ch,"{CIn the group: {WUnknown{C!{x\n\r");
        send_to_char("{cClass       Level Gain Wait-Time Min-Mana Rating{x\n\r",ch);
        send_to_char("{W----------- ----- ---- --------- -------- ------{x\n\r",ch);
        for (class = 0;class < MAX_CLASS;class++) {
        if (skill_table[skill].skill_level[class] > 200)
            continue;
        printf_to_char(ch,"{G%-11s {g%5d %4d %9d %8d %6d\n\r",
        class_table[class].name,
        skill_table[skill].skill_level[class],
        skill_table[skill].rating[class],
        skill_table[skill].beats,
        skill_table[skill].min_mana,
        skill_table[skill].rating[class]);
        }
        return;
    }
}


void do_newscore (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int hitroll;
    int damroll;
    int align;
    char cposition[100];
    char calignment[100];
//    int exptolevel;

    if (ch->level <= 9)
    {
        align = 0;
        strcpy(calignment, "NULL");
    }
    else
    {
        align = ch->alignment;
        if (ch->alignment > 900)
            strcpy(calignment, "saintly");
        else if (ch->alignment > 700)
            strcpy(calignment, "very good");
        else if (ch->alignment > 350)
            strcpy(calignment, "good");
        else if (ch->alignment > 100)
            strcpy(calignment, "neutral good");
        else if (ch->alignment > -100)
            strcpy(calignment, "neutral bad");
        else if (ch->alignment > -350)
            strcpy(calignment, "bad");
        else if (ch->alignment > -700)
            strcpy(calignment, "evil");
        else if (ch->alignment > -900)
            strcpy(calignment, "chaotic");
        else
            strcpy(calignment, "satanic");
    }

    switch (ch->position)
    {
        case POS_DEAD:
            strcpy(cposition, "You are DEAD");
            break;
        case POS_MORTAL:
            strcpy(cposition, "You are Mortally Wounded");
            break;
        case POS_INCAP:
            strcpy(cposition, "You are Incapacitated");
            break;
        case POS_STUNNED:
            strcpy(cposition, "You are Stunned");
            break;
        case POS_SLEEPING:
            strcpy(cposition, "You are Sleeping");
            break;
        case POS_RESTING:
            strcpy(cposition, "You are Resting");
            break;
        case POS_SITTING:
            strcpy(cposition, "You are Sitting");
            break;
        case POS_STANDING:
            strcpy(cposition, "You are Standing");
            break;
        case POS_FIGHTING:
            strcpy(cposition, "You are Fighting");
            break;
    }

    if (ch->level >= 15)
    {
        hitroll = GET_HITROLL(ch);
        damroll = GET_DAMROLL(ch);
    }
    else
    {
        hitroll = 0;
        damroll = 0;
    }

    send_to_char ("    {G+{g------------------------------------------------------{G+{x\n\r", ch);

    sprintf (buf, "    {g|{C Scoresheet %41s {g|{x\n\r", ch->name);
    send_to_char(buf, ch);

    send_to_char ("    {G+{g------------------------{G+{g-----------------------------{G+{x\n\r", ch);

    sprintf (buf, "    {g|{c  Race:{C %-15s {g|{c Health:{C %4d {c/{C %-12d {g|{x\r\n",
	          race_table[ch->race].name,
                  ch->hit, ch->max_hit);
    send_to_char(buf, ch);

    sprintf (buf, "    {g|{c Class:{C %-15s {g|{c   Mana:{C %4d {c/{C %-12d {g|{x\n\r",
                  IS_NPC(ch) ? "mobile" : class_table[ch->class].name,
                  ch->mana, ch->max_mana);
    send_to_char(buf, ch);

    sprintf (buf, "    {g|{c   Sex:{C %-15s {g|{c   Move:{C %4d {c/{C %-12d {g|{x\n\r",
                  ch->sex == 0 ? "Sexless" : ch->sex == 1 ? "Male" : "Female",
                  ch->move, ch->max_move);
    send_to_char(buf, ch);

    sprintf(buf, "    {g|{c Level: {C%-11d {G+{g---{G+{g--------{G+{g--------------{G+     {g|{x\n\r",
                 ch->level);
    send_to_char(buf, ch);

    sprintf(buf, "    {g|{c   Age:{C %-11d {g|{c Str:{C %2d{c/{C%2d {g|{c Prce:{C %-6d {g|     |{x\n\r",
                 get_age(ch), get_curr_stat(ch, STAT_STR), ch->perm_stat[STAT_STR],
                 GET_AC(ch, AC_PIERCE));
    send_to_char(buf, ch);

    sprintf(buf, "    {g|{c   Exp:{C %-11d {g|{c Int:{C %2d{c/{C%2d {g| {cBash:{C %-6d {G+{g-----{G+{x\n\r",
                 ch->exp, get_curr_stat(ch, STAT_INT), ch->perm_stat[STAT_INT],
                 GET_AC(ch, AC_BASH));
    send_to_char(buf, ch);

    sprintf(buf, "    {G+{g--------------------{G+ {cWis:{C %2d{c/{C%2d {g|{c Slsh:{C %-6d {g|{x\n\r",
                 get_curr_stat(ch, STAT_WIS), ch->perm_stat[STAT_WIS],
                 GET_AC(ch, AC_SLASH));
    send_to_char(buf, ch);

    sprintf(buf, "    {g|{c Hitroll:{C %-9d {g|{c Dex:{C %2d{c/{C%2d {g|{c Magc:{C %-6d {g|{x\n\r",
                 GET_HITROLL(ch), get_curr_stat(ch, STAT_DEX), ch->perm_stat[STAT_DEX],
                 GET_AC(ch, AC_EXOTIC));
    send_to_char(buf, ch);

    sprintf(buf, "    {g|{c Damroll:{C %-9d {g|{c Con:{C %2d{c/{C%2d {g|              |{x\n\r",
                 GET_DAMROLL(ch), get_curr_stat(ch, STAT_CON), ch->perm_stat[STAT_CON]);
    send_to_char(buf, ch);

    sprintf(buf, "    {g|{c    Gold:{C %-9ld {G+{g------------{G+{g--------------{G+{x\n\r", ch->gold);
    send_to_char(buf, ch);

    sprintf(buf, "    {g|{c  Silver:{C %-9ld {g|{C %-25s {g|{x\n\r",
                 ch->silver, cposition);
    send_to_char(buf, ch);

    send_to_char("    {G+{g--------------------{G+{g--{G+{g------------------------{G+{x\n\r", ch);

    sprintf(buf, "    {g|{c Alignment:{C %-10d {g|{c  Items:{C %4d {c/{C %-7d {g|{x\n\r",
                 align, ch->carry_number, can_carry_n(ch));
    send_to_char(buf, ch);

    sprintf(buf, "    {g|{c You are{C %-13s {g|{c Weight:{C %4d {c/{C %-7d {g|{x\n\r",
                 calignment, get_carry_weight(ch) / 10, can_carry_w(ch) / 10);
    send_to_char(buf, ch);

    send_to_char("    {G+{g-----------------------{G+{g------------------------{G+{x\n\r", ch);

    sprintf(buf, "    {g|{c Wimpy:{C %-39d {g|{x\n\r",
                 ch->wimpy);
    send_to_char(buf, ch);

    send_to_char("    {G+{g------------------------------------------------{G+{x\n\r", ch);

    sprintf(buf, "    {g|{c Bank Gold:{C %-35ld {g|{x\n\r",ch->pcdata->gold_bank);
    send_to_char(buf, ch);
    sprintf(buf, "    {g|{c Bank Silver: {C %-32ld {g|{x\n\r",ch->pcdata->silver_bank);
    send_to_char(buf, ch);
    send_to_char("    {G+{g------------------------------------------------{G+{x\n\r", ch);
    
    if (ch->level < 200){
    sprintf(buf, "    {g|{c Exp to Level: {C %d/%d/%d  {g|{x\n\r",
			8000 - ch->exp, 64000 - ch->exp, 124000 - ch->exp);
    send_to_char(buf, ch);}


        if (ch->level == 200) {
        send_to_char("    {g| {cExp for Veteran: {C100000 {g|\n\r", ch);}
        if (ch->level == 201) {
        send_to_char("    {g| {cExp for Champion: {C200000 {g|\n\r", ch);}
        if (ch->level == 202) {
        send_to_char("    {g| {cExp for Legend: {C300000 {g|\n\r", ch);}
        if (ch->level == 203) {
        send_to_char("    {g| {cExp for Chosen: {C400000 {g|\n\r", ch);}
        if (ch->level == 204) {
        send_to_char("    {g| {cExp for Master: {C500000 {g|\n\r", ch);}
        if (ch->level == 205) {
        send_to_char("    {g| {cExp for Ancient: {C600000 {g|\n\r", ch);}


//    if (ch->level >= 100 && ch->level <= 105){
//    sprintf(buf, "   {g|{c Exp needed for next Level: {C%d{x\n\r", exptolevel - ch->exp);
//    send_to_char(buf, ch);}
 

//	((ch->level + 1) * exp_per_level (ch, ch->pcdata->points) - ch->exp));
//    send_to_char(buf, ch);
    send_to_char("    {G+{g------------------------------------------------{G+{x\n\r", ch);




    send_to_char("\n\r", ch);

    if (IS_SET(ch->comm, COMM_SHOW_AFFECTS))
        do_function(ch, &do_affects, "");

}

/* for stances */
extern char* stance_name[];

// CH_CMD ( do_sstat )
void do_sstat(CHAR_DATA *ch, char *argument)
{
char buf[MSL];
int stance;

send_to_char(
"            {dBasic Stances{x\n\r",ch);
send_to_char(
"{c+--------------------------------------+{x\n\r",ch);
sprintf( buf,
"{rViper{x:     {D%d{x\n\r{rCrane{x:     {D%d{x\n\r{rCrab{x:      {D%d{x\n\r{rMongoose{x:  {D%d{x\n\r{rBull{x:      {D%d{x\n\r",
ch->stance[1],ch->stance[2],ch->stance[3],ch->stance[4],ch->stance[5]);
send_to_char(buf,ch);
send_to_char( "\n\r          {dAdvanced Stances{x\n\r",ch);
send_to_char( "{c+--------------------------------------+{x\n\r",ch);
sprintf( buf,
"{rMantis{x:    {D%d{x\n\r{rDragon{x:    {D%d{x\n\r{rTiger{x:     {D%d{x\n\r{rMonkey{x:    {D%d{x\n\r{rSwallow{x:   {D%d{x\n\r",
ch->stance[6],ch->stance[7],ch->stance[8],ch->stance[9],ch->stance[10]);
send_to_char(buf,ch);
send_to_char( "{c+--------------------------------------+{x\n\r",ch);

stance = ch->stance[0];
if (stance >=1 && stance <= 10)
{
	sprintf(buf,"%s{x\n\r", stance_name[stance]);

send_to_char("{xCurrent stance:{! ",ch);
send_to_char(buf,ch);
}
else
{
send_to_char("{xCurrent stance:{! None{x\n\r",ch);
send_to_char("Stance command to pick a stance\n\r",ch);
}


return;
}





void do_directions (CHAR_DATA * ch, char *argument)
{

send_to_char("{W-={RDirections From MS{W=-{x\n\r",ch);
send_to_char("[All] Ofcol 3w4n2e7n6e4n{x\n\r",ch);
send_to_char("[1-5] Dwarven Day Care 6e3n2e1s{x\n\r",ch);
send_to_char("[1-20] Plains of the North 4e3n2w1n{x\n\r",ch);
send_to_char("[1-35] New Thalos 15e{x\n\r",ch);
send_to_char("[5-10] Haon Dor 5w{x\n\r",ch);
send_to_char("[5-10] Graveyard 3e7s1w1s{x\n\r",ch);
send_to_char("[5-20] Arachnos 13w1s2w1n1u1w1n{x\n\r",ch);
send_to_char("[5-30] Wyvern's Tower 6e4s2e1s2e1d2e{x\n\r",ch);
send_to_char("[5-35] The Shire 5w4n{x\n\r",ch);
send_to_char("[5-50] Olympus 3w4n2e8n2u\n\r",ch);
send_to_char("[5-50] Tower of Sorcery 9e1n{x\n\r",ch);
send_to_char("[20-90] Amber Palace 13w1s2w2n{x\n\r",ch);
send_to_char("[30-51] Valar Cathedral 24e3n2e{x\n\r",ch);
send_to_char("[50-60] Drow Cavern 9e1d{x\n\r",ch);
send_to_char("[55-99] Chessboard 1s1u{x\n\r",ch);
send_to_char("[60-80] Kender Village 10w1n{x\n\r",ch);
send_to_char("[80-110] Goblin Hole 8w2n{x\n\r",ch);
send_to_char("[100-130] Fey-Born Tower 13w1d{x\n\r",ch);
send_to_char("[120-150] Midgaard Prison 3e6n{x\n\r",ch);
send_to_char("[120-175] Hobgoblin Keep 13w7n{x\n\r",ch);
send_to_char("[150-175] Gangland 6s1e1s2e1s2e{x\n\r",ch);
send_to_char("[175-200] Elemental Canyon 6e4s2e1s2e1d1s4u1n{x\n\r",ch);
send_to_char("[175-200] Drakyri Isle 8s1e2s2e1s1w1s1e3s1w7s2e2s2e{x\n\r",ch);
send_to_char("[200-ANC] Daggermoon Keep 9e2n1d2n2e1u{x\n\r",ch);
send_to_char("[200-ANC] Liches Fortress 10w3n1e1n1e2n1e{x\n\r",ch);
send_to_char("[200-ANC] The Temple of Baah 10w3n1e1n1e2n1e2n2e1n\n\r",ch);
send_to_char("[200-ANC] Chaotic Dwellings 10w3n4w2n\n\r",ch);
send_to_char("[200-ANC] Forest Castle 5w4n2w6n\n\r",ch);
return;

}


void do_focus(CHAR_DATA *ch, char *argument)
{
 char buf[MSL];

 /*if(ch->level < LEVEL_HERO)
 {
  send_to_char("You just aren't legendary enough to focus.\n\r",ch);
  return;
 }
*/
 //else
	if (!IS_NPC(ch))
 {
 
send_to_char("\n\r{b-----------======================{x{cFOCUS{x{b======================-----------{x\n\r",ch);
 send_to_char("{b|>{x                   {RLvl  Totl Pnts  Pnts Left  Modifier1  Modifier2 {b<|{x\n\r", ch);
 send_to_char("{b|>{x                   {r---  ---------  ---------  ---------  --------- {b<|{x\n\r", ch);
 sprintf(buf,"{b|>{x {gCombat Power{x     : {r%-2d  %-9ld  %-9ld  Dam: %4d  Str: %4d {b<|{x\n\r",
	focus_level(ch->pcdata->focus[COMBAT_POWER]),
	ch->pcdata->focus[COMBAT_POWER],
	focus_left(ch->pcdata->focus[COMBAT_POWER]),
	focus_dam(ch), focus_str(ch) );
 send_to_char(buf,ch);
 sprintf(buf,"{b|>{x {gCombat Defense{x   : {r%-2d  %-9ld  %-9ld  AC: %5d  Mv: %5d {b<|{x\n\r",
	focus_level(ch->pcdata->focus[COMBAT_DEFENSE]),
	ch->pcdata->focus[COMBAT_DEFENSE],
	focus_left(ch->pcdata->focus[COMBAT_DEFENSE]),
        focus_ac(ch), 
focus_move(ch)*focus_level(ch->pcdata->focus[COMBAT_DEFENSE]) );
 send_to_char(buf,ch);
 sprintf(buf,"{b|>{x {gCombat Agility{x   : {r%-2d  %-9ld  %-9ld  Hit: %4d  Dex: %4d {b<|{x\n\r",
	focus_level(ch->pcdata->focus[COMBAT_AGILITY]),
	ch->pcdata->focus[COMBAT_AGILITY],
	focus_left(ch->pcdata->focus[COMBAT_AGILITY]),
	focus_hit(ch), focus_dex(ch) );
 send_to_char(buf,ch);
 sprintf(buf,"{b|>{x {gCombat Toughness{x : {r%-2d  %-9ld  %-9ld  Hp: %5d  Con: %4d {b<|{x\n\r",
	focus_level(ch->pcdata->focus[COMBAT_TOUGHNESS]),
	ch->pcdata->focus[COMBAT_TOUGHNESS],
	focus_left(ch->pcdata->focus[COMBAT_TOUGHNESS]),
        focus_hp(ch)*focus_level(ch->pcdata->focus[COMBAT_TOUGHNESS]), 
focus_con(ch) );
 send_to_char(buf,ch);
 sprintf(buf,"{b|>{x {gMagic Power{x      : {r%-2d  %-9ld  %-9ld  Sor: %4d  Max: +%3d%%{b<|{x\n\r",
	focus_level(ch->pcdata->focus[MAGIC_POWER]),
	ch->pcdata->focus[MAGIC_POWER],
	focus_left(ch->pcdata->focus[MAGIC_POWER]),
	focus_sorc(ch),focus_sorcbonus(ch) );
 send_to_char(buf,ch);
 sprintf(buf,"{b|>{x {gMagic Ability{x    : {r%-2d  %-9ld  %-9ld  Ma: %5d  Int: %4d {b<|{x\n\r",
	focus_level(ch->pcdata->focus[MAGIC_ABILITY]),
	ch->pcdata->focus[MAGIC_ABILITY],
	focus_left(ch->pcdata->focus[MAGIC_ABILITY]),
        focus_mana(ch)*focus_level(ch->pcdata->focus[MAGIC_ABILITY]), focus_int(ch) );
 send_to_char(buf,ch);
 sprintf(buf,"{b|>{x {gMagic Defense{x    : {r%-2d  %-9ld  %-9ld  Svs: %4d  Wis: %4d {b<|{x\n\r",
	focus_level(ch->pcdata->focus[MAGIC_DEFENSE]),
	ch->pcdata->focus[MAGIC_DEFENSE],
	focus_left(ch->pcdata->focus[MAGIC_DEFENSE]),
	focus_save(ch), focus_wis(ch) );
 send_to_char(buf,ch);
 
send_to_char("{b-----------===================={x{c********{x{b=====================-----------{x\n\r",ch);
 
 if (ch->pcdata->focus[CURRENT_FOCUS] == COMBAT_POWER)
	send_to_char("You current focus is Combat Power.\n\r",ch);
 else if (ch->pcdata->focus[CURRENT_FOCUS] == COMBAT_DEFENSE)
	send_to_char("You current focus is Combat Defense.\n\r",ch);
 else if (ch->pcdata->focus[CURRENT_FOCUS] == COMBAT_AGILITY)
	send_to_char("You current focus is Combat Agility.\n\r",ch);
 else if (ch->pcdata->focus[CURRENT_FOCUS] == COMBAT_TOUGHNESS)
	send_to_char("You current focus is Combat Toughness.\n\r",ch);
 else if (ch->pcdata->focus[CURRENT_FOCUS] == MAGIC_POWER)
	send_to_char("You current focus is Magic Power.\n\r",ch);
 else if (ch->pcdata->focus[CURRENT_FOCUS] == MAGIC_ABILITY)
	send_to_char("You current focus is Magic Ability.\n\r",ch);
 else if (ch->pcdata->focus[CURRENT_FOCUS] == MAGIC_DEFENSE)
	send_to_char("You current focus is Magic Defense.\n\r",ch);

 }
 return;
}
