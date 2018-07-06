#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"
#include "globals.h"
#include "proto.h"

char *  string_pad      args( ( char *string, int nWidth, bool Right_Align) );

/* String_pad by Phate of Unforgotten Wrath and Final Fantasy 1 MUD */
char* string_pad(char *string, int nWidth, bool Right_Align)
{
char buf[MSL];
char buf2[MSL];
int x = 0;
int count = 0;

if(string == NULL)
return "";

for( x = 0 ; ; x++ )
{
if( string[x] == '\0')
{
break;
}
else if( string[x] == '{')
{
x++;
if(string[x] == '{')
{
count++;
}
}
else
{
count++;
}
}
strcpy(buf2,"");

for( x = 0; x <= nWidth - count; x++)
strcat(buf2," ");

if(Right_Align)
{
strcpy(buf,buf2);
strcat(buf,string);
}
else
{
strcpy(buf,string);
strcat(buf,buf2);
}
return str_dup(buf);
}


/*
 * CHANCE function. I use this everywhere in my code, very handy :>
 */
bool chance ( int num )
{
    if ( number_range ( 1, 100 ) <= num )
        return TRUE;
    else
        return FALSE;
}
bool bigchance ( int num )
{
    if ( number_range ( 1, 1500 ) * 2 <= num )
        return TRUE;
    else
        return FALSE;
}

void do_calc(CHAR_DATA *ch, char *argument)
{
  char      buf  [MAX_STRING_LENGTH];
  char	    arg1 [MAX_INPUT_LENGTH];
  char      arg2 [MAX_INPUT_LENGTH];
  char      arg3 [MAX_INPUT_LENGTH];

  double value1=0,value2=0,result;

  argument = one_argument( argument, arg1 );	
  argument = one_argument( argument, arg2 );
  argument = one_argument( argument, arg3 );


    value1 = is_number( arg1 ) ? atoi( arg1 ) : -1;
    value2 = is_number( arg3 ) ? atoi( arg3 ) : -1;

sprintf(buf,"value1: %2.0f value2: %2.0f. \n\r", value1,value2);
send_to_char(buf,ch);


if (!str_cmp(arg2,"x")) // mult  
{
result = value1 * value2;
sprintf(buf,"%2.0f X %2.0f = %2.0f\n\r", value1,value2,result);
send_to_char(buf,ch);
return;
}

else if (!str_cmp(arg2,"/")) // division
{
result = value1 / value2;
sprintf(buf,"%2.0f / %2.0f = %2.0f.\n\r", value1,value2,result);
send_to_char(buf,ch);
return;
}
else if (!str_cmp(arg2,"+")) // division
{
result = value1 + value2;
sprintf(buf,"%2.0f + %2.0f = %2.0f.\n\r", value1,value2,result);
send_to_char(buf,ch);
return;
}
else if (!str_cmp(arg2,"-")) // division
{
result = value1 - value2;
sprintf(buf,"%2.0f - %2.0f = %2.0f.\n\r", value1,value2,result);
send_to_char(buf,ch);
return;
}
else
{
send_to_char("Danger Will Robinson!",ch);
return;
}
return;
}



int get_ratio(CHAR_DATA *ch)
{
  int ratio;

  if (IS_NPC(ch)) return 0;
  if ((ch->pkill + ch->pdeath) == 0) ratio = 0; // to avoid divide by zero.
  else if (ch->pkill > 0)
    ratio = ch->pkill * 100 * ((ch->pkill * ch->pkill) - (ch->pdeath * ch->pdeath))/((ch->pkill + ch->pdeath) * (ch->pkill + ch->pdeath));
  else
    ratio = 100 * ((ch->pkill * ch->pkill) - (ch->pdeath * ch->pdeath))/((ch->pkill + ch->pdeath) * (ch->pkill + ch->pdeath));
  return ratio;
}


void do_becomedragon(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char      arg1 [MAX_INPUT_LENGTH];
//  int race;
  
//  race = ch->race;
 
  argument = one_argument( argument, arg1 );
 
	if (IS_NPC(ch)) return;

/*	if (ch->race != 5)
	{
	send_to_char("You must be a Draconian to become a Dragon.\n\r",ch);
	return;
	}
*/
	if (ch->pcdata->mkills < 1000)
	{
	send_to_char("You must have at least 1000 mob kills to become a Dragon.\n\r",ch);
	return;
	}
	

if ( arg1[0] == '\0' )
{
stc("Type becomedragon <color> to become that dragon type.\n\r",ch);
stc("{G*****************{x\n\r",ch);
stc("{Y--{CBlack Dragon\n\r",ch);
stc("{Y--{CBlue Dragon\n\r",ch);
stc("{Y--{CRed Dragon\n\r",ch);
stc("{Y--{CGreen Dragon\n\r",ch);
stc("{Y--{CWhite Dragon\n\r",ch);
stc("{Y--{CBrass Dragon\n\r",ch);
stc("{Y--{CGold Dragon\n\r",ch);
stc("{Y--{CSilver Dragon\n\r",ch);
stc("{Y--{CBronze Dragon\n\r",ch);
stc("{Y--{CCopper Dragon\n\r",ch);
stc("{G*****************{n\n\r",ch);
  return;
}
if (!str_cmp(arg1,"black"))
{
ch->race = 30;
send_to_char("You have become a Black Dragon.\n\r", ch);
sprintf(buf,"%s has become a Black Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"blue"))
{
ch->race = 31;
send_to_char("You have become a Blue Dragon.\n\r", ch);
sprintf(buf,"%s has become a Blue Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"red"))
{
ch->race = 33;
send_to_char("You have become a Red Dragon.\n\r", ch);
sprintf(buf,"%s has become a Red Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"green"))
{
ch->race = 32;
send_to_char("You have become a Green Dragon.\n\r", ch);
sprintf(buf,"%s has become a Green Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"white"))
{
ch->race = 34;
send_to_char("You have become a White Dragon.\n\r", ch);
sprintf(buf,"%s has become a White Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"brass"))
{
ch->race = 35;
send_to_char("You have become a Brass Dragon.\n\r", ch);
sprintf(buf,"%s has become a Brass Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"gold"))
{
ch->race = 36;
send_to_char("You have become a Gold Dragon.\n\r", ch);
sprintf(buf,"%s has become a Gold Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"silver"))
{
ch->race = 37;
send_to_char("You have become a Silver Dragon.\n\r", ch);
sprintf(buf,"%s has become a Silver Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"bronze"))
{
ch->race = 38;
send_to_char("You have become a Bronze Dragon.\n\r", ch);
sprintf(buf,"%s has become a Bronze Dragon.", ch->name);
do_info(ch,buf);
}
else if (!str_cmp(arg1,"copper"))
{
ch->race = 39;
send_to_char("You have become a Copper Dragon.\n\r", ch);
sprintf(buf,"%s has become a Copper Dragon.", ch->name);
do_info(ch,buf);
}

ch->affected_by = ch->affected_by | race_table[ch->race].aff;
ch->imm_flags = ch->imm_flags | race_table[ch->race].imm;
ch->res_flags = ch->res_flags | race_table[ch->race].res;
ch->vuln_flags = ch->vuln_flags | race_table[ch->race].vuln;
ch->form = race_table[ch->race].form;
ch->parts = race_table[ch->race].parts;
ch->weight = pc_race_table[ch->race].weight;
ch->height = pc_race_table[ch->race].height;

return;
}

void do_level(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char arg1 [MAX_INPUT_LENGTH];
  int add_hp = 0;
  int add_mana = 0;
  int add_move = 0;
  int add_prac = 5;
//  int exptolevel;
  argument = one_argument( argument, arg1 );

	if (IS_NPC(ch)) return;
	
	if (!str_cmp(arg1,"sublevel"))
  {
  if (ch->level < 200) return;
	if (ch->sublevel >= 2000) return;

	int subexp;
	subexp = (ch->sublevel * 500);

  if (ch->exp < subexp )
  {
  send_to_char ("Not enough Exp to Level your first subclass\n\r  ", ch);
        sprintf (buf, "You need %d more. {x\n\r", subexp - ch->exp);
        send_to_char (buf, ch);
  return;
  }

		ch->exp -= subexp;

    add_hp = number_range (100,300);
    add_mana = number_range (100,300);
    add_move = number_range (100,300);


    ch->max_hit += add_hp;
    ch->max_mana += add_mana;
    ch->max_move += add_move;
    ch->practice += add_prac;
    if (double_pracs = TRUE);
    ch->practice += add_prac;
    ch->train += 2;

    ch->pcdata->perm_hit += add_hp;
    ch->pcdata->perm_mana += add_mana;
    ch->pcdata->perm_move += add_move;

        ch->sublevel++;

				sprintf(buf, "Your sublevel is now %d\n\r",ch->sublevel);
        send_to_char (buf, ch);

        sprintf (buf,
                 "You gain %d hit point%s, %d mana, %d move, %d practice%s and 2 trains .\n\r",
                 add_hp, add_hp == 1 ? "" : "s", add_mana, add_move,
                 add_prac, add_prac == 1 ? "" : "s");
        send_to_char (buf, ch);

    		ch->hit = ch->max_hit;
        ch->mana = ch->max_mana;
        ch->move = ch->max_move;
        send_to_char( "{WYour body and soul have been healed, you have been given the {Rpower{W to continue{x\n\r",ch);
        save_char_obj (ch);
				return;
}


	if (!str_cmp(arg1,"classone"))
	{
	if (ch->level < 200)
	{
	if (ch->exp < 16000 ) // 8000
	{
	send_to_char ("Not enough Exp to Level your first class\n\r  ", ch);
        sprintf (buf, "You need %d more. {x\n\r", 8000 - ch->exp);
        send_to_char (buf, ch);
	return;
	}
	}
	if (ch->level >= 206)
	{	
        send_to_char ("You are already an Ancient at level 206\n\r  ", ch);
        return;
        }
	if (ch->level == 205 && ch->exp < 600000)
	{
        send_to_char ("Not enough Exp to Level your first class to Ancient\n\r  ", ch);
        sprintf (buf, "You need %d more. {x\n\r", 600000 - ch->exp);
        send_to_char (buf, ch);
        return;
        }
        if (ch->level == 204 && ch->exp < 500000)
        {
        send_to_char ("Not enough Exp to Level your first class to Master\n\r  ", ch);
        sprintf (buf, "You need %d more. {x\n\r", 500000 - ch->exp);
        send_to_char (buf, ch);
        return;
        }
        if (ch->level == 203 && ch->exp < 400000)
        {
        send_to_char ("Not enough Exp to Level your first class to Chosen\n\r  ", ch);
        sprintf (buf, "You need %d more. {x\n\r", 400000 - ch->exp);
        send_to_char (buf, ch);
        return;
        }
        if (ch->level == 202 && ch->exp < 300000)
        {
        send_to_char ("Not enough Exp to Level your first class to Legend\n\r  ", ch);
        sprintf (buf, "You need %d more. {x\n\r", 300000 - ch->exp);
        send_to_char (buf, ch);
        return;
        }
        if (ch->level == 201 && ch->exp < 200000)
        {
        send_to_char ("Not enough Exp to Level your first class to Champion\n\r  ", ch);
        sprintf (buf, "You need %d more. {x\n\r", 200000 - ch->exp);
        send_to_char (buf, ch);
        return;
        }
        if (ch->level == 200 && ch->exp < 100000)
        {
        send_to_char ("Not enough Exp to Level your first class to Veteran\n\r  ", ch);
        sprintf (buf, "You need %d more. {x\n\r", 100000 - ch->exp);
        send_to_char (buf, ch);
        return;
        }





	
	
    add_hp += get_curr_stat (ch, STAT_CON) / 2
	+ number_range (class_table[ch->class].hp_min,
          class_table[ch->class].hp_max);
    add_mana += get_curr_stat(ch, STAT_WIS) / 2;
    add_move += get_curr_stat(ch, STAT_VIT) / 2;

    add_prac += get_curr_stat(ch, STAT_WIS) / 10;
    add_prac += get_curr_stat(ch, STAT_DIS) / 10;
//	add_prac += 10;


    add_hp = UMAX (2, add_hp);
    add_mana = UMAX (2, add_mana);
    add_move = UMAX (6, add_move);

    ch->max_hit += add_hp;
    ch->max_mana += add_mana;
    ch->max_move += add_move;
    ch->practice += add_prac;
    ch->train += 2;

    ch->pcdata->perm_hit += add_hp;
    ch->pcdata->perm_mana += add_mana;
    ch->pcdata->perm_move += add_move;

        sprintf (buf,
                 "You gain %d hit point%s, %d mana, %d move, and %d practice%s.\n\r",
                 add_hp, add_hp == 1 ? "" : "s", add_mana, add_move,
                 add_prac, add_prac == 1 ? "" : "s");
        send_to_char (buf, ch);

	  ch->hit = ch->max_hit;
        ch->mana = ch->max_mana;
        ch->move = ch->max_move;
        update_pos ( ch );
        send_to_char( "{WYour body and soul have been healed, you have been given the {Rpower{W to continue{x\n\r",ch);
        save_char_obj (ch);
	
	if (ch->level <= 199) ch->exp -= 16000;
	if (ch->level == 200) ch->exp -= 100000;
	if (ch->level == 201) ch->exp -= 200000;
	if (ch->level == 202) ch->exp -= 300000;
	if (ch->level == 203) ch->exp -= 400000;
	if (ch->level == 204) ch->exp -= 500000;
	if (ch->level == 205) ch->exp -= 600000;

	ch->level++;
//	ch->exp -= exptolevel; // exptolevel; 

        sprintf (buf, "Your %s class is now level %d. {x\n\r", class_table[ch->class].name, ch->level);
        send_to_char(buf, ch);

//	sprintf(buf, "%s has advanced their first class to level %d.",
//	ch->name, ch->level);

	return;
	}
	if (!str_cmp(arg1,"classtwo"))
	{
	if (ch->exp < 124000) // 64K
	{
	send_to_char ("Not enough Exp to Level your second class\n\r  ", ch);
	return;
	}
        if (ch->class2 == -1 )
        {
        send_to_char ("You do not have a second class\n\r  ", ch);
        return;
        }

        if (ch->level2 >= ch->level)
        {
        send_to_char ("Your second class may not be higher than your first\n\r  ", ch);
        return;
        }
        if (ch->level2 >= 206)
        {
        send_to_char ("Your second class may only be level 206\n\r  ", ch);
        return;
        }


    add_hp += get_curr_stat (ch, STAT_CON) / 2 +
        number_range (class_table[ch->class2].hp_min,
                      class_table[ch->class2].hp_max);
    add_mana += get_curr_stat(ch, STAT_WIS) / 2;
    add_move += get_curr_stat(ch, STAT_VIT) / 2;

    add_prac += get_curr_stat(ch, STAT_WIS) / 10;
    add_prac += get_curr_stat(ch, STAT_DIS) / 10;
//        add_prac += 10;



    add_hp = UMAX (2, add_hp);
    add_mana = UMAX (2, add_mana);
    add_move = UMAX (6, add_move);

    ch->max_hit += add_hp;
    ch->max_mana += add_mana;
    ch->max_move += add_move;
    ch->practice += add_prac;
    ch->train += 2;

    ch->pcdata->perm_hit += add_hp;
    ch->pcdata->perm_mana += add_mana;
    ch->pcdata->perm_move += add_move;



        sprintf (buf,
                 "You gain %d hit point%s, %d mana, %d move, and %d practice%s.\n\r",
                 add_hp, add_hp == 1 ? "" : "s", add_mana, add_move,
                 add_prac, add_prac == 1 ? "" : "s");
        send_to_char (buf, ch);

	  ch->hit = ch->max_hit;
        ch->mana = ch->max_mana;
        ch->move = ch->max_move;
        update_pos ( ch );
        send_to_char( "{WYour body and soul have been healed, you have been given the {Rpower{W to continue{x\n\r",ch);
        save_char_obj (ch);

	ch->level2++;
	ch->exp -= 124000;
        sprintf (buf, "Your %s class is now level %d. {x\n\r", class_table[ch->class2].name, ch->level2);
        send_to_char(buf, ch);

//        sprintf(buf, "%s has advanced their second class to level %d.",
//        ch->name, ch->level2);
//        do_info(ch,buf);

	return;
	}
	if (!str_cmp(arg1,"classthree"))
	{
	if (ch->exp < 248000)
	{
	send_to_char ("Not enough Exp to Level your third class\n\r  ", ch);
	return;
	}
        if (ch->level3 >= ch->level)
        {
        send_to_char ("Your third class may not be higher than your first\n\r  ", ch);
        return;
        }
        if (ch->level3 >= ch->level2)
        {
        send_to_char ("Your third class may not be higher than your second\n\r  ", ch);
        return;
        }
        if (ch->class3 == -1 )
        {
        send_to_char ("You do not have a third class\n\r  ", ch);
        return;
        }

        if (ch->level3 >= 206)
        {
        send_to_char ("Your third class may only be level 206\n\r  ", ch);
        return;
        }



    add_hp += get_curr_stat (ch, STAT_CON) / 2 +
        number_range (class_table[ch->class3].hp_min,
                      class_table[ch->class3].hp_max);
    add_mana += get_curr_stat(ch, STAT_WIS) / 2;
    add_move += get_curr_stat(ch, STAT_VIT) / 2;

    add_prac += get_curr_stat(ch, STAT_DIS) / 10;
    add_prac += get_curr_stat(ch, STAT_WIS) / 10;
//	add_prac += 10;


//      add_hp = add_hp * 30;
      add_mana = add_mana * 45;
      add_move = add_move * 45;


    add_hp = UMAX (2, add_hp);
    add_mana = UMAX (2, add_mana);
    add_move = UMAX (6, add_move);

    ch->max_hit += add_hp;
    ch->max_mana += add_mana;
    ch->max_move += add_move;
    ch->practice += add_prac;
    ch->train += 2;

    ch->pcdata->perm_hit += add_hp;
    ch->pcdata->perm_mana += add_mana;
    ch->pcdata->perm_move += add_move;



      sprintf (buf,
      "You gain %d hit point%s, %d mana, %d move, and %d practice%s.\n\r",
                 add_hp, add_hp == 1 ? "" : "s", add_mana, add_move,
                 add_prac, add_prac == 1 ? "" : "s");
        send_to_char (buf, ch);

	  ch->hit = ch->max_hit;
        ch->mana = ch->max_mana;
        ch->move = ch->max_move;
        update_pos ( ch );
        send_to_char( "{WYour body and soul have been healed, you have been given the {Rpower{W to continue{x\n\r",ch);
        save_char_obj (ch);


	ch->level3++;

        sprintf (buf, "Your %s class is now level %d. {x\n\r", class_table[ch->class3].name, ch->level3);
        send_to_char(buf, ch);
	ch->exp -= 248000;

//        sprintf(buf, "%s has advanced their third class to level %d.",
//        ch->name, ch->level3);
//	do_info(ch,buf);

	return;
	}
	if (!str_cmp(arg1,"classfour"))
	{
	if (ch->exp < 496000)
	{
	send_to_char ("Not enough Exp to Level your fourth class\n\r  ", ch);
	return;
	}
        if (ch->level4 >= ch->level)
        {
        send_to_char ("Your fourth class may not be higher than your first\n\r  ", ch);
        return;
        }
        if (ch->level4 >= ch->level2)
        {
        send_to_char ("Your fourth class may not be higher than your second\n\r  ", ch);
        return;
        }
        if (ch->level4 >= ch->level3)
        {
        send_to_char ("Your fourth class may not be higher than your third\n\r  ", ch);
        return;
        }
        if (ch->class4 == -1 )
        {
        send_to_char ("You do not have a fourth class\n\r  ", ch);
        return;
        }

        if (ch->level4 >= 206)
        {
        send_to_char ("Your fourth class may only be level 206\n\r  ", ch);
        return;
        }



    add_hp += get_curr_stat (ch, STAT_CON) / 2 +
        number_range (class_table[ch->class4].hp_min,
                      class_table[ch->class4].hp_max);

    add_mana += get_curr_stat(ch, STAT_WIS) / 2;
    add_move += get_curr_stat(ch, STAT_VIT) / 2;

    add_prac += get_curr_stat(ch, STAT_DIS) / 10;
    add_prac += get_curr_stat(ch, STAT_WIS) / 10;
//	add_prac += 10;


//      add_hp = add_hp * 30;
//      add_mana = add_mana * 45;
//      add_move = add_move * 45;


    add_hp = UMAX (2, add_hp);
    add_mana = UMAX (2, add_mana);
    add_move = UMAX (6, add_move);

    ch->max_hit += add_hp;
    ch->max_mana += add_mana;
    ch->max_move += add_move;
    ch->practice += add_prac;
    ch->train += 2;

    ch->pcdata->perm_hit += add_hp;
    ch->pcdata->perm_mana += add_mana;
    ch->pcdata->perm_move += add_move;



      sprintf (buf,
      "You gain %d hit point%s, %d mana, %d move, and %d practice%s.\n\r",
                 add_hp, add_hp == 1 ? "" : "s", add_mana, add_move,
                 add_prac, add_prac == 1 ? "" : "s");
        send_to_char (buf, ch);

	  ch->hit = ch->max_hit;
        ch->mana = ch->max_mana;
        ch->move = ch->max_move;
        update_pos ( ch );
        send_to_char( "{WYour body and soul have been healed, you have been given the {Rpower{W to continue{x\n\r",ch);
        save_char_obj (ch);


	ch->level4++;

        sprintf (buf, "Your %s class is now level %d. {x\n\r", class_table[ch->class4].name, ch->level4);
        send_to_char(buf, ch);
	ch->exp -= 496000;

//        sprintf(buf, "%s has advanced their fourth class to level %d.",
//        ch->name, ch->level4);
//        do_info(ch,buf);

	return;
	}


	

	sprintf (buf, " [Class One: %-15s] [Level %d] {x\n\r", class_table[ch->class].name, ch->level);
	send_to_char(buf, ch);
	
	if (ch->class2 != -1) 
	{
	sprintf (buf, " [Class Two: %-15s] [Level %d] {x\n\r", class_table[ch->class2].name, ch->level2);
	send_to_char(buf, ch);
	}
	if (ch->class3 != -1) 
	{
	sprintf (buf, " [Class Three: %-15s] [Level %d] {x\n\r", class_table[ch->class3].name, ch->level3);
	send_to_char(buf, ch);
	}
	if (ch->class4 != -1) 
	{
	sprintf (buf, " [Class Four: %-15s] [Level %d] {x\n\r", class_table[ch->class4].name, ch->level4);
	send_to_char(buf, ch);
	}


        sprintf (buf, " [Sublevel %d] {x\n\r", ch->sublevel);
        send_to_char(buf, ch);

send_to_char (" Level with arg classone, classtwo, classthree, classfour or sublevel.\n\r  ", ch);


return;
}
void do_multiclass(CHAR_DATA *ch, char *argument)
{
  char arg1 [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg1 );

  if (IS_NPC(ch)) return;

  if (ch->class != -1 && ch->class2 != -1 && ch->class3 != -1 && ch->class4 != -1 )
  {   
    send_to_char("You already have 4 classes.\n\r",ch);
    return;
  }

            send_to_char ("{G   Select a Class{x\n\r  ", ch);
            send_to_char ("{G   Sorcerer{x\n\r  ", ch);
            send_to_char ("{G   Bishop{x\n\r  ", ch);
            send_to_char ("{G   Ninja{x\n\r  ", ch);
            send_to_char ("{G   Hoplite{x\n\r  ", ch);
            send_to_char ("{G   Templar{x\n\r  ", ch);
            send_to_char ("{G   Avenger{x\n\r  ", ch);
            send_to_char ("{G   Lich{x\n\r  ", ch);
            send_to_char ("{G   Shaman{x\n\r  ", ch);
            send_to_char ("{G   Druid{x\n\r  ", ch);
            send_to_char ("{G   Assassin{x\n\r  ", ch);
	

if (!str_cmp(arg1,"sorcerer"))
{
	if (ch->class == 0 || ch->class2 == 0 || ch->class3 == 0)
	{
	send_to_char("You are already a Sorcerer.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 0;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become a Sorcerer.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 0;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become a Sorcerer.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 0;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become a Sorcerer.\n\r", ch);
	return;
	}
return;
}

else if (!str_cmp(arg1,"bishop"))
{
	if (ch->class == 1 || ch->class2 == 1 || ch->class3 == 1)
	{
	send_to_char("You are already a Bishop.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 1;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become a Bishop.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 1;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become a Bishop.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 1;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become a Bishop.\n\r", ch);
	return;
	}
return;
}

else if (!str_cmp(arg1,"ninja"))
{
	if (ch->class == 2 || ch->class2 == 2 || ch->class3 == 2)
	{
	send_to_char("You are already a Ninja.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 2;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become a Ninja.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 2;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become a Ninja.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 2;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become a Ninja.\n\r", ch);
	return;
	}
return;
}

else if (!str_cmp(arg1,"hoplite"))
{
	if (ch->class == 3 || ch->class2 == 3 || ch->class3 == 3)
	{
	send_to_char("You are already a Hoplite.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 3;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become a Hoplite.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 3;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become a Hoplite.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 3;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become a Hoplite.\n\r", ch);
	return;
	}
return;
}

else if (!str_cmp(arg1,"templar"))
{
	if (ch->class == 4 || ch->class2 == 4 || ch->class3 == 4)
	{
	send_to_char("You are already a Templar.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 4;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become a Templar.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 4;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become a Templar.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 4;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become a Templar.\n\r", ch);
	return;
	}
return;
}

else if (!str_cmp(arg1,"avenger"))
{
	if (ch->class == 5 || ch->class2 == 5 || ch->class3 == 5)
	{
	send_to_char("You are already an Avenger.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 5;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become an Avenger.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 5;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become an Avenger.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 5;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become an Avenger.\n\r", ch);
	return;
	}
return;
}


else if (!str_cmp(arg1,"lich"))
{
	if (ch->class == 6 || ch->class2 == 6 || ch->class3 == 6)
	{
	send_to_char("You are already a Lich.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 6;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become a Lich.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 6;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become a Lich.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 6;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become a Lich.\n\r", ch);
	return;
	}
return;
}

else if (!str_cmp(arg1,"shaman"))
{
	if (ch->class == 7 || ch->class2 == 7 || ch->class3 == 7)
	{
	send_to_char("You are already a Shaman.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 7;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become a Shaman.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 7;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become a Shaman\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 7;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become a Shaman\n\r", ch);
	return;
	}

return;
}


else if (!str_cmp(arg1,"druid"))
{
	if (ch->class == 8 || ch->class2 == 8 || ch->class3 == 8)
	{
	send_to_char("You are already a Druid.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 8;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become a Druid.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 8;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become a Druid.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 8;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become a Druid.\n\r", ch);
	return;
	}
return;
}

else if (!str_cmp(arg1,"assassin"))
{
	if (ch->class == 9 || ch->class2 == 9 || ch->class3 == 9)
	{
	send_to_char("You are already an Assassin.\n\r", ch);
	return;
	}	
	if (ch->class2 == -1) 
	{
	ch->class2 = 9;
      group_add (ch, class_table[ch->class2].default_group, TRUE);
	send_to_char( "You have become an Assassin.\n\r", ch);
	return;
	}
	if (ch->class3 == -1) 
	{
	ch->class3 = 9;
      group_add (ch, class_table[ch->class3].default_group, TRUE);
	send_to_char( "You have become an Assassin.\n\r", ch);
	return;
	}
	if (ch->class4 == -1) 
	{
	ch->class4 = 9;
      group_add (ch, class_table[ch->class4].default_group, TRUE);
	send_to_char( "You have become an Assassin.\n\r", ch);
	return;
	}
return;
}

return;
}

void do_perm( CHAR_DATA *ch, char *argument )
{
  char item[MAX_INPUT_LENGTH], affect[MAX_INPUT_LENGTH], mod[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  AFFECT_DATA *aff;
 
  argument = one_argument( argument, item );
  argument = one_argument( argument, affect );
  argument = one_argument( argument, mod );
 
  if (item[0] == '\0' || affect[0] == '\0')
  {
    send_to_char( "Perm <item> <affect> [modifier]\n\r", ch );
    return;
  }
 
  if( ( obj = get_obj_carry( ch, item, ch )) == NULL )
  {
    send_to_char( "You don't have that item.\n\r", ch );
    return;
  }
 
  aff = malloc( sizeof( *aff ) );
  aff->level = ch->level;
  aff->duration = -1;
  aff->bitvector = 0;
  aff->type = aff->location; //  = get_item_apply_val (affect);
  if (mod [0] != '\0')
     aff->modifier = atoi (mod);
  else
     aff->modifier = ch-> level;
 
  affect_to_obj( obj, aff );
 
  return;
}


void do_spellup( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    argument = one_argument ( argument, arg );
    if ( IS_NPC ( ch ) )
        return;
    if ( arg[0] == '\0' )
    {
        send_to_char ( "Spellup whom?\n\r", ch );
        return;
    }


    if ( !str_cmp ( arg, "all" ) && ( ch->level >= HERO ) )
    {
        for ( d = descriptor_list; d != NULL; d = d->next )
        {
            if ( d->connected == CON_PLAYING && d->character != ch &&
                 d->character->in_room != NULL &&
                 can_see ( ch, d->character ) )
            {
                char buf[MAX_STRING_LENGTH];
                sprintf ( buf, "%s %s", d->character->name, arg );
                do_spellup ( ch, buf );
            }
        }
        return;
    }
    if ( str_cmp ( "room", arg ) )
    {
        if ( ( vch = get_char_world ( ch, arg ) ) == NULL )
        {
            send_to_char ( "They aren't here.\n\r", ch );
            return;
        }
    }
    else
        vch = ch;
    if ( !str_cmp ( "room", arg ) )
        for ( vch = ch->in_room->people; vch; vch = vch->next_in_room )
        {
            if ( vch == ch )
                continue;
            if ( IS_NPC ( vch ) )
                continue;

            send_to_char ( "{8[{R********{8SPELLUP{R********{R]{x\n\r", vch );

            spell_shield ( skill_lookup ( "shield" ), ch->level, ch, vch,
                           TARGET_CHAR );
            spell_armor ( skill_lookup ( "armor" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_sanctuary ( skill_lookup ( "sanctuary" ), ch->level, ch,
                              vch, TARGET_CHAR );
            spell_fly ( skill_lookup ( "fly" ), ch->level, ch, vch,
                        TARGET_CHAR );
            spell_frenzy ( skill_lookup ( "frenzy" ), ch->level, ch, vch,
                           TARGET_CHAR );
            spell_giant_strength ( skill_lookup ( "giant strength" ),
                                   ch->level, ch, vch, TARGET_CHAR );
            spell_bless ( skill_lookup ( "bless" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_haste ( skill_lookup ( "haste" ), ch->level, ch, vch,
                          TARGET_CHAR );
	    spell_detect_evil ( skill_lookup ( "detect evil" ), ch->level, ch, vch,
			  TARGET_CHAR );
            spell_detect_good ( skill_lookup ( "detect good" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_detect_hidden ( skill_lookup ( "detect hidden" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_detect_invis ( skill_lookup ( "detect invis" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_detect_magic ( skill_lookup ( "detect magic" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_detect_poison ( skill_lookup ( "detect poison" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_stone_skin ( skill_lookup ( "stone skin" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_bark_skin ( skill_lookup ( "bark skin" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_rabies ( skill_lookup ( "rabies" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_forceshield ( skill_lookup ( "force shield" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_staticshield ( skill_lookup ( "static shield" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_flameshield ( skill_lookup ( "flame shield" ), ch->level, ch, vch,
                          TARGET_CHAR );
            spell_aid ( skill_lookup ( "aid" ), ch->level, ch, vch,
                          TARGET_CHAR );
	spell_pass_door (skill_lookup("pass door"), ch->level, ch, vch, TARGET_CHAR);
	spell_animal_instinct (skill_lookup("animal instinct"), ch->level, ch, vch, TARGET_CHAR);
        spell_precision (skill_lookup("precision"), ch->level, ch, vch, TARGET_CHAR);
        spell_cocoon (skill_lookup("cocoon"), ch->level, ch, vch, TARGET_CHAR);
        spell_achilles (skill_lookup("achilles"), ch->level, ch, vch, TARGET_CHAR);

        spell_iceshield (skill_lookup("iceshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_fireshield (skill_lookup("fireshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_shockshield (skill_lookup("shockshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_acidshield (skill_lookup("acidshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_poisonshield (skill_lookup("poisonshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_briarshield (skill_lookup("briarshield"), ch->level, ch, vch, TARGET_CHAR);


    if ( IS_AFFECTED(vch, AFF2_WEBBED) )
    {

	REMOVE_BIT (vch->affected_by, AFF2_WEBBED);
    }

            send_to_char ( "{8[{R***********************{8]{x\n\r", vch );






        }
    else
    {
            send_to_char ( "{8[{R********{8SPELLUP{R********{8]{x\n\r", vch );

        spell_shield ( skill_lookup ( "shield" ), ch->level, ch, vch,
                       TARGET_CHAR );
        spell_armor ( skill_lookup ( "armor" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_sanctuary ( skill_lookup ( "sanctuary" ), ch->level, ch, vch,
                          TARGET_CHAR );
        spell_fly ( skill_lookup ( "fly" ), ch->level, ch, vch, TARGET_CHAR );
        spell_frenzy ( skill_lookup ( "frenzy" ), ch->level, ch, vch,
                       TARGET_CHAR );
        spell_giant_strength ( skill_lookup ( "giant strength" ), ch->level,
                               ch, vch, TARGET_CHAR );
        spell_bless ( skill_lookup ( "bless" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_haste ( skill_lookup ( "haste" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_detect_evil ( skill_lookup ( "detect evil" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_detect_good ( skill_lookup ( "detect good" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_detect_hidden ( skill_lookup ( "detect hidden" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_detect_invis ( skill_lookup ( "detect invis" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_detect_magic ( skill_lookup ( "detect magic" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_detect_poison ( skill_lookup ( "detect poison" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_stone_skin ( skill_lookup ( "stone skin" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_bark_skin ( skill_lookup ( "bark skin" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_rabies ( skill_lookup ( "rabies" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_forceshield ( skill_lookup ( "force shield" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_staticshield ( skill_lookup ( "static shield" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_flameshield ( skill_lookup ( "flame shield" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_aid ( skill_lookup ( "aid" ), ch->level, ch, vch,
                      TARGET_CHAR );
        spell_animal_instinct (skill_lookup("animal instinct"), ch->level, ch, vch, TARGET_CHAR);
        spell_precision (skill_lookup("precision"), ch->level, ch, vch, TARGET_CHAR);
        spell_cocoon (skill_lookup("cocoon"), ch->level, ch, vch, TARGET_CHAR);
        spell_achilles (skill_lookup("achilles"), ch->level, ch, vch, TARGET_CHAR);
        spell_pass_door (skill_lookup("pass door"), ch->level, ch, vch, TARGET_CHAR);
        spell_iceshield (skill_lookup("iceshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_fireshield (skill_lookup("fireshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_shockshield (skill_lookup("shockshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_acidshield (skill_lookup("acidshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_poisonshield (skill_lookup("poisonshield"), ch->level, ch, vch, TARGET_CHAR);
        spell_briarshield (skill_lookup("briarshield"), ch->level, ch, vch, TARGET_CHAR);

            send_to_char ( "{8[{R***********************{8]{x\n\r", vch );


    }
    return;
}


void dump_to_scr( char *text )
{
int a;

  a = strlen( text );
  write(1, text, a);
  return;
}

void do_precisionstrike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( !IS_NPC(ch)
    && ch->level < skill_table[gsn_precisionstrike].skill_level[ch->class]
    && ch->level < skill_table[gsn_precisionstrike].skill_level[ch->class2]
    && ch->level < skill_table[gsn_precisionstrike].skill_level[ch->class3] )
    {
	send_to_char("You don't know how to precision strike.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim)) return;

    WAIT_STATE( ch, skill_table[gsn_precisionstrike].beats );

    if ( IS_NPC(ch) || number_percent( ) < get_skill(ch,gsn_precisionstrike) )
    {
	one_hit(ch, victim, gsn_precisionstrike,FALSE);
        one_hit(ch, victim, gsn_precisionstrike,FALSE);
        one_hit(ch, victim, gsn_precisionstrike,FALSE);
	check_improve(ch,gsn_precisionstrike,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_precisionstrike,TYPE_UNDEFINED, TRUE);
	check_improve(ch,gsn_precisionstrike,FALSE,1);
    }

    return;
}

void do_cleave( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;


    one_argument( argument, arg );

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_cleave].skill_level[ch->class] )
      {
	send_to_char("You don't know how to cleave.\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Cleave whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you sneak up on yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
	send_to_char( "You need to wield a weapon to cleave.\n\r", ch );
	return;
    }


    if ( victim->fighting != NULL )
    {
	send_to_char( "You can't cleave a fighting person.\n\r", ch );
	return;
    }

    if ( (victim->hit < (0.9 * victim->max_hit)) &&
	 (IS_AWAKE(victim)) )
    {
	act( "$N is hurt and suspicious ... you can't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_cleave].beats );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < get_skill(ch,gsn_cleave) )
      {
	check_improve(ch,gsn_cleave,TRUE,1);
	multi_hit(ch,victim,gsn_cleave);
      }
    else
      {
	check_improve(ch,gsn_cleave,FALSE,1);
	damage( ch, victim, 0, gsn_cleave,DAM_NONE, TRUE );
      }
    return;
}

void do_bomuzite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
        
    if (IS_NPC(ch)) return;
	
    if ( !IS_NPC(ch)
    && ch->level < skill_table[gsn_bomuzite].skill_level[ch->class]
    && ch->level < skill_table[gsn_bomuzite].skill_level[ch->class2]
    && ch->level < skill_table[gsn_bomuzite].skill_level[ch->class3] )
      {
        send_to_char("You don't know how to bomuzite.\n\r",ch);
        return;
      }



  if (ch->in_room != NULL)
  {
    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    {
      send_to_char( "You cannot use this power in a safe room.\n\r", ch );
      return;
    }
  }

	
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Bomb who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Not on yourself!\n\r", ch );
	return;
    }
	
    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "Not while fighting!\n\r", ch );
	return;
    }
    

	  
    if ( victim->in_room == ch->in_room )
	{
	act("You toss your bomb onto the floor and put $N to sleep.",ch,NULL,victim,TO_CHAR);
	act("$n tosses a bomb onto the floor.  You feel sleepy.",ch,NULL,victim,TO_VICT);
	victim->position = POS_SLEEPING;
	WAIT_STATE(ch, 1);
	return;
	}
	
	return;
}

void do_circle( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( !IS_NPC(ch)
    && ch->level < skill_table[gsn_circle].skill_level[ch->class]
    && ch->level2 < skill_table[gsn_circle].skill_level[ch->class2]
    && ch->level3 < skill_table[gsn_circle].skill_level[ch->class3]
    && ch->level4 < skill_table[gsn_circle].skill_level[ch->class4]
 )

    {
	send_to_char("You don't know how to circle.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

/*    if ((get_eq_char(ch,WEAR_WIELD) == NULL) ||
	attack_table[get_eq_char(ch,WEAR_WIELD)->value[3]].damage 
	   != DAM_PIERCE)
      {
       send_to_char("You must wield a piercing weapon to circle stab.\n\r",ch);
       return;
     }
*/
    if (is_safe(ch,victim)) return;

    WAIT_STATE( ch, skill_table[gsn_circle].beats );

/*    for (person = ch->in_room->people;person != NULL;
	 person = person->next_in_room)
      {
	if (person->fighting == ch)
	  {
	    send_to_char("You can't circle while defending yourself.\n\r",ch);
	    return;
	  }
      }
*/

    if ( IS_NPC(ch) || number_percent( ) < get_skill(ch,gsn_circle) )
    {
	one_hit(ch, victim, gsn_circle,TRUE);
        one_hit(ch, victim, gsn_circle,TRUE);
        one_hit(ch, victim, gsn_circle,TRUE);
	check_improve(ch,gsn_circle,TRUE,1);

    if ( IS_NPC(ch) || number_percent( ) < get_skill(ch,gsn_dual_circle) )
    {
        one_hit(ch, victim, gsn_dual_circle,TRUE);
        one_hit(ch, victim, gsn_dual_circle,TRUE);
        one_hit(ch, victim, gsn_dual_circle,TRUE);
        check_improve(ch,gsn_dual_circle,TRUE,1);
    }
    else
    {
        damage( ch, victim, 0, gsn_dual_circle,TYPE_UNDEFINED, TRUE);
        check_improve(ch,gsn_dual_circle,FALSE,1);
    }
}

    else
    {
	damage( ch, victim, 0, gsn_circle,TYPE_UNDEFINED, TRUE);
	check_improve(ch,gsn_circle,FALSE,1);
    }

    return;
}
void do_toggle( CHAR_DATA *ch, char *argument )
{
  char arg[MIL], arg2[MIL];

  argument = one_argument( argument, arg );
  argument = one_argument(argument, arg2);

    if (!strcmp(argument, "exp"))
  {
            send_to_char("Syntax: toggle <exp> <on|off>.\n\r",ch);
    		return;
  }      
  if (!str_cmp(arg, "exp"))
  {
      if (!str_cmp(arg2, "on"))  
	{
	if (global_exp)
    {
            send_to_char("Double exp is already in affect!\n\r",ch);
            return;
        }
        global_exp = TRUE;
	do_info( ch, "{gDouble Experience is {wON{x!!!!" );

      return;
    }
    if (!str_cmp(arg2, "off"))
    {
        if (!global_exp)
    {
            send_to_char("The global exp flag isn't on!.\n\r",ch);
      return;
    }
        global_exp = FALSE; 
    do_info( ch, "{gDouble exp is now {wOFF{x" );

        return;
    }}
  if (!str_cmp(arg, "stance"))
  {
      if (!str_cmp(arg2, "on"))
        {
        if (double_stance)
    {
            send_to_char("Double Stance is already in affect!\n\r",ch);
            return;
        }
        double_stance = TRUE;
        do_info( ch, "{gDouble Stance is {wON{x!!!!" );

      return;
    }
    if (!str_cmp(arg2, "off"))
    {
        if (!double_stance)
    {
            send_to_char("The global stance flag isn't on!.\n\r",ch);
      return;
    }
        double_stance = FALSE;
    do_info( ch, "{gDouble stance is now {wOFF{x" );

        return;
    }}  

    if ( !str_cmp ( arg, "christmas" ) )
    {
        if ( its_christmas )
        {
           its_christmas = FALSE;
	   send_to_char ( "Christmas mode OFF\n\r", ch );
	   return;
	}
	else
	{
	   its_christmas = TRUE;
	   send_to_char ( "Christmas mode ON! Merry christmas!\n\r", ch );
/*
	global_exp = TRUE;
	double_stance = TRUE;
	double_improve = TRUE;
	double_quest = TRUE;
	double_pracs = TRUE;
	double_damage = TRUE;
	global_quest = TRUE;
*/

	   return;
	}
    }
    if ( !str_cmp ( arg, "special_day" ) )
    {
        if ( special_day )
        {
           special_day = FALSE;
	   send_to_char ( "Ok, no longer a special day :(\n\r", ch );
	   return;
	}
	else
	{
	   special_day = TRUE;
/*
        global_exp = TRUE;
        double_stance = TRUE;
        double_improve = TRUE;
        double_quest = TRUE;
        double_pracs = TRUE;
        double_damage = TRUE;
        global_quest = TRUE;
*/

	   send_to_char ( "Woo! Happy special day!\n\r", ch );
	   return;
	}
    }
	if ( !str_cmp ( arg, "stuffchanged" ) )
    {
        if ( stuff_was_changed )
        {
           stuff_was_changed = FALSE;
	   send_to_char ( "Nothing was changed, no autoreboot in the morning.\n\r", ch );
	   return;
	}
	else
	{
	   stuff_was_changed = TRUE;
	   send_to_char ( "Woo! Changes! Will reboot in the morning, sir!\n\r", ch );
	   return;
	}
    }



  if (!str_cmp(arg, "improve"))
  {
      if (!str_cmp(arg2, "on"))
        {
        if (double_improve)
    {
            send_to_char("Double Improve is already in affect!\n\r",ch);
            return;
        }
        double_improve = TRUE;
        do_info( ch, "{gDouble Improve is {wON{x!!!!" );

      return;
    }
    if (!str_cmp(arg2, "off"))
    {
        if (!double_improve)
    {
            send_to_char("The global improve flag isn't on!.\n\r",ch);
      return;
    }
        double_improve = FALSE;
    do_info( ch, "{gDouble improve is now {wOFF{x" );

        return;
    }}


return;
}

void load_coreinfo()
{
        FILE     *fp;

        if ((fp = fopen("../txt/coreinfo.txt", "r")) == NULL)
        {
                log_string("Error: coreinfo.txt not found!");
                exit(1);
        }

        top_playerid = fread_number(fp);

        fclose(fp);
}

void save_coreinfo()
{
  FILE *fp;
  int i;

  for (i = 0; i < 2; i++)
  {
    if (i == 0)
    {
      if ((fp = fopen("../txt/coreinfo.bck","w")) == NULL)
      {
        log_string("Error writing to coreinfo.bck");
        return;
      }
    }
    else
    {
      if ((fp = fopen("../txt/coreinfo.txt","w")) == NULL)
      {
        log_string("Error writing to coreinfo.txt");
        return;
      }
    }

    fprintf(fp, "%d\n", top_playerid);

    fclose(fp);
  }
}



void load_gods()
{

  int i;   
  FILE *fp;

  if ((fp = fopen("../txt/gods.txt", "r")) == NULL)
  {
    log_string("Error: gods.txt not found!");
    exit(1);
  }
  gods[0].souls = 0;

  for (i = 0; i < 22; i++)
  {
    gods[i].souls = fread_number(fp);
  }
  fclose(fp);

}
void save_gods()
{

  FILE *fp;
  int i;

  if ((fp = fopen("../txt/gods.txt","w")) == NULL)
  {
    log_string("Error writing to gods.txt");
    return;
  }
  for (i=0;i<22;i++)
  {
    fprintf(fp, "%d\n", gods[i].souls); 
  }
  fclose (fp);

}
void do_gods(CHAR_DATA *ch, char *argument)
{
int i;

char diety[MAX_STRING_LENGTH];
char arg1[MAX_INPUT_LENGTH];
char arg2[MAX_INPUT_LENGTH];
char arg3[MAX_INPUT_LENGTH];
char buf[MAX_STRING_LENGTH];
//unsigned long int value = 0;

argument = one_argument( argument, arg1 );
argument = one_argument( argument, arg2 );
argument = one_argument( argument, arg3 );

save_gods();


  if (arg1[0] == '\0')
  {
send_to_char("Diety                    : Souls  \n\r",ch);
send_to_char("\n\r",ch);
for (i=0;i<22;i++)
{
if (i==0) sprintf(diety,"Athiest     ");
if (i==1) sprintf(diety,"Paladine    ");
if (i==2) sprintf(diety,"Astinus     ");
if (i==3) sprintf(diety,"Chemosh     ");
if (i==4) sprintf(diety,"Hiddukel    ");
if (i==5) sprintf(diety,"Nuitari     ");
if (i==6) sprintf(diety,"Sargonnas   ");
if (i==7) sprintf(diety,"Zeboim      ");
if (i==8) sprintf(diety,"Branchala   ");
if (i==9) sprintf(diety,"Habbakuk    ");
if (i==10) sprintf(diety,"Kiri-Jolith ");
if (i==11) sprintf(diety,"Majere     ");
if (i==12) sprintf(diety,"Mishakal   ");
if (i==13) sprintf(diety,"Solinari   ");
if (i==14) sprintf(diety,"Chislev    ");
if (i==15) sprintf(diety,"Gilean     ");
if (i==16) sprintf(diety,"Lunitari   ");
if (i==17) sprintf(diety,"Reorx      ");
if (i==18) sprintf(diety,"Shinare    ");
if (i==19) sprintf(diety,"Sirrion    ");
if (i==20) sprintf(diety,"Zivilyn    ");
if (i==21) sprintf(diety,"Takhisis   ");


sprintf(buf,"%-18s : %-12d -> \n\r",diety,gods[i].souls); 
send_to_char(buf,ch);
}}
return;
}


/* Modified by Samson 5-15-99 */
void show_race_line( CHAR_DATA * ch, CHAR_DATA * victim )
{
   char buf[MAX_STRING_LENGTH];
   int feet, inches;

   if( !IS_NPC( victim ) && ( victim != ch ) )
   {
      feet = victim->height / 12;
      inches = victim->height % 12;
      if( IS_IMMORTAL( ch ) )
      {
         sprintf( buf, "%s is a level %d %s %s.\n\r", victim->name, victim->level, 
                 pc_race_table[victim->race].name, class_table[victim->class].name);
	stc(buf, ch);
      }
  if (!IS_NPC(victim))
  {

        char const *eyes;
        char const *build;
        char const *looks;
        char const *hair;


	eyes = pc_eye_table[victim->pcdata->pc_eyes].pc_eyes;
        build = pc_build_table[victim->pcdata->pc_build].pc_build;
        looks = pc_looks_table[victim->pcdata->pc_looks].pc_looks;
        hair = pc_hair_table[victim->pcdata->pc_hair].pc_hair;

   sprintf(buf, "%s has %s eyes, is %s, \n\r", victim->name, eyes, build);
   send_to_char(buf, ch);
   if (victim->sex == SEX_FEMALE) send_to_char("She",ch);
   if (victim->sex == SEX_MALE) send_to_char("He",ch);
   sprintf(buf, " %s and %s. \n\r", looks, hair);
   send_to_char(buf, ch);

  }

      sprintf(buf, "%s is %d'%d\" and weighs %d pounds.\n\r", PERS( victim, ch ), feet, inches, victim->weight );
      stc(buf, ch);
      return;
   }
   if( !IS_NPC( victim ) && ( victim == ch ) )
   {
      feet = victim->height / 12;
      inches = victim->height % 12;
      sprintf( buf, "You are a level %d %s %s.\n\r", victim->level, 
	pc_race_table[victim->race].name, class_table[victim->class].name);
      stc(buf, ch);

  if (!IS_NPC(victim))
  {

        char const *eyes;
        char const *build;
        char const *looks;
        char const *hair;


	eyes = pc_eye_table[victim->pcdata->pc_eyes].pc_eyes;
        build = pc_build_table[victim->pcdata->pc_build].pc_build;
        looks = pc_looks_table[victim->pcdata->pc_looks].pc_looks;
        hair = pc_hair_table[victim->pcdata->pc_hair].pc_hair;

   sprintf(buf, "%s has %s eyes, is %s, \n\r", victim->name, eyes, build);
   send_to_char(buf, ch);
   if (victim->sex == SEX_FEMALE) send_to_char("She",ch);
   if (victim->sex == SEX_MALE) send_to_char("He",ch);
   sprintf(buf, " %s and %s. \n\r", looks, hair);
   send_to_char(buf, ch);

  }

      sprintf( buf, "You are %d'%d\" and weigh %d pounds.\n\r", feet, inches, victim->weight );
      stc(buf, ch);
      return;
   }
}

void leave_info(char *str)
{
  DESCRIPTOR_DATA *d;

  if (str[0] == '\0') return;

  for (d = descriptor_list; d != NULL; d = d->next) 
  {  
    if (d->connected == CON_PLAYING && d->character != NULL)                                              
    {    
      send_to_char("{G+{g-{G+ {cLeaves {G+{g-{G+{x ", d->character); 
      send_to_char(str, d->character); 
      send_to_char("\n\r", d->character); 
    }    
  }  
  return;
}

void enter_info(char *str)
{
  DESCRIPTOR_DATA *d;

  if (str[0] == '\0') return;

  for (d = descriptor_list; d != NULL; d = d->next) 
  {  
    if (d->connected == CON_PLAYING && d->character != NULL)                                              
    {    
      send_to_char("{w=-= {bEnters {w=-={x ", d->character); 
      send_to_char(str, d->character); 
      send_to_char("\n\r", d->character); 
    }    
  }  
  return;
}


void do_consecrate(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int livingweap_needed = 0;
	int arti_needed = 0;
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
//        int wear    = 0;
//        int armor   = 0;

//	int value;

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	strcpy(arg3,argument);

	if (arg1[0] == '\0')
	{
	send_to_char("Which item do you wish to consecrate?\n\r", ch);
	return;
	}
	if ( (obj = get_obj_carry(ch, arg1, ch)) == NULL)
	{
	send_to_char("You are not carrying that item.\n\r", ch);
	return;
	}
	if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
	{
	send_to_char("You may only Consecrate weapons and armor.\n\r",ch);
	return;
	}
/*	if (!IS_OBJ_STAT (obj, ITEM_RELIC) && !IS_OBJ_STAT (obj, ITEM_ARTIFACT))
        {
	send_to_char("You may only Consecrate Relics and Artifacts.\n\r",ch);
	return;
	}
*/
	if (obj->item_type == ITEM_ARMOR && arg2[0] == '\0')
	{
        send_to_char("{b[{r**********{b[{wConsecrate Menu{b]{r**********{b]{x.\n\r",ch);
        sprintf( buf, "Artifact Name: %s \n\r", obj->short_descr );
        send_to_char( buf, ch );
        sprintf( buf, "Artifact Level: %d, Artifact Points: %d \n\r", obj->weapon_level, obj->weapon_points);
        send_to_char( buf, ch );
        arti_needed = 20000;
        sprintf( buf, "Artifact Experiance: %d, EXP til next Level: %d \n\r", obj->weapon_currentxp,
        (arti_needed - obj->weapon_currentxp));
        send_to_char( buf, ch );
        stc( "10 Artifact Points are redeamable for:\n\r", ch );
        stc( "Hitroll: 10\n\r", ch );
        stc( "Damroll: 10\n\r", ch );
        stc( "HP: 1000\n\r", ch );
        stc( "Mana: 1000\n\r", ch );
        stc( "Move: 1000\n\r", ch );
        send_to_char("{b[{r**********{b[{wConsecrate Menu{b]{r**********{b]{x.\n\r",ch);
	}


        if (obj->item_type == ITEM_ARMOR)
	{
        hitroll = 10;
        damroll = 10;
        hp      = 1000;
        mana    = 1000;
        move    = 1000;

	if (obj->weapon_points == 10)
	{
	if (!str_cmp(arg2,"hitroll"))
	{
	add_affect( obj, APPLY_HITROLL, hitroll );
	stc( "Hitroll + 10\n\r", ch );
	obj->weapon_points = 0;
	return;
	}
        if (!str_cmp(arg2,"damroll"))
        {
        add_affect( obj, APPLY_DAMROLL, damroll );
        stc( "Damroll + 10\n\r", ch );
        obj->weapon_points = 0;
	return;
        }
        if (!str_cmp(arg2,"hp"))
        {
        add_affect( obj, APPLY_HIT, hp );
        stc( "HP + 1000\n\r", ch );
        obj->weapon_points = 0;
	return;
        }
        if (!str_cmp(arg2,"mana"))
        {
        add_affect( obj, APPLY_MANA, mana );
        stc( "Mana + 1000\n\r", ch );
        obj->weapon_points = 0;
	return;
        }
        if (!str_cmp(arg2,"move"))
	{
        add_affect( obj, APPLY_MOVE, move );
        stc( "Move + 1000\n\r", ch );
        obj->weapon_points = 0;
	return;
        }
	}
return;
}






        if (obj->item_type == ITEM_WEAPON && arg2[0] == '\0')
	{

	send_to_char("{b[{r**********{b[{wConsecrate Menu{b]{r**********{b]{x.\n\r",ch);
	sprintf( buf, "Weapon Name: %s \n\r", obj->short_descr );
	send_to_char( buf, ch );
	sprintf( buf, "Weapon Attack Type: %s \n\r", attack_table[obj->value[3]].name );
	send_to_char( buf, ch );
        send_to_char ("Weapon type is ", ch);
            switch (obj->value[0])
            {
                case (WEAPON_EXOTIC):
                    send_to_char ("exotic.\n\r", ch);
                    break;
                case (WEAPON_SWORD):
                    send_to_char ("sword.\n\r", ch);
                    break;
                case (WEAPON_DAGGER):
                    send_to_char ("dagger.\n\r", ch);
                    break;
                case (WEAPON_SPEAR):
                    send_to_char ("spear/staff.\n\r", ch);
                    break;
                case (WEAPON_MACE):
                    send_to_char ("mace/club.\n\r", ch);
                    break;
                case (WEAPON_AXE):
                    send_to_char ("axe.\n\r", ch);
                    break;
                case (WEAPON_FLAIL):
                    send_to_char ("flail.\n\r", ch);
                    break;
                case (WEAPON_WHIP):
                    send_to_char ("whip.\n\r", ch);
                    break;
                case (WEAPON_POLEARM):
                    send_to_char ("polearm.\n\r", ch);
                    break;
                default:
                    send_to_char ("unknown.\n\r", ch);
                    break;
            }

	sprintf( buf, "Weapon Damage: %d d %d \n\r", obj->value[1], obj->value[2]);
	send_to_char( buf, ch );
	sprintf( buf, "Weapon Level: %d, Weapon Points: %d \n\r", obj->weapon_level, obj->weapon_points);
	send_to_char( buf, ch );

	livingweap_needed = 1500;
	sprintf( buf, "Weapon Experiance: %d, EXP til next Level: %d \n\r", obj->weapon_currentxp, 
	(livingweap_needed - obj->weapon_currentxp));
	send_to_char( buf, ch );
	if (IS_WEAPON_STAT (obj, WEAPON_POISON))
	send_to_char("This Weapon is Poisoned.\n\r",ch);
	if (IS_WEAPON_STAT (obj, WEAPON_VAMPIRIC))
	send_to_char("This Weapon is Vampiric.\n\r",ch);
	if (IS_WEAPON_STAT (obj, WEAPON_FLAMING))
	send_to_char("This Weapon is Enchanted with Fire.\n\r",ch);
	if (IS_WEAPON_STAT (obj, WEAPON_FROST))
	send_to_char("This Weapon is Enchanted with Frost.\n\r",ch);
	if (IS_WEAPON_STAT (obj, WEAPON_SHOCKING))
	send_to_char("This Weapon is Enchanted with Electricity.\n\r",ch);
	if (IS_WEAPON_STAT (obj, WEAPON_SHARP))
	send_to_char("This Weapon is Sharp.\n\r",ch);
	if (IS_WEAPON_STAT (obj, WEAPON_TWO_HANDS))
	send_to_char("This Weapon is Two-Handed\n\r",ch);
	send_to_char("{b[{r**********{b[{wConsecrate Menu{b]{r**********{b]{x.\n\r",ch);
	send_to_char("Upgrade Costs:\n\r",ch);
	if (!IS_WEAPON_STAT (obj, WEAPON_POISON))
	send_to_char("Poison(5)",ch);
	if (!IS_WEAPON_STAT (obj, WEAPON_VAMPIRIC))
	send_to_char(" Vampiric(5)",ch);
	if (!IS_WEAPON_STAT (obj, WEAPON_FLAMING))
	send_to_char(" Flaming(5)",ch);
	if (!IS_WEAPON_STAT (obj, WEAPON_FROST))
	send_to_char(" Frost(5)",ch);
	if (!IS_WEAPON_STAT (obj, WEAPON_SHOCKING))
	send_to_char(" Shocking(5)",ch);
	if (!IS_WEAPON_STAT (obj, WEAPON_SHARP))
	send_to_char(" Sharp(5)",ch);
	send_to_char("\n\rNumber of Die +1 (5)",ch);
	send_to_char("\n\rDamage ammount of Die +1 (1)",ch);
	send_to_char("\n\rName - Short",ch);
	send_to_char("\n\rWeapon Type (weapon)",ch);
	send_to_char("\n\r{b[{r**********{b[{wConsecrate Menu{b]{r**********{b]{x.\n\r",ch);
	}
    if (!str_cmp(arg2,"name"))
    {
        free_string(obj->name);
        obj->name = str_dup(arg3);
    } 
    if (!str_cmp(arg2,"short"))
    {
        free_string(obj->short_descr);
        obj->short_descr=str_dup(arg3);
    }

	if (!str_cmp(arg2,"weapon"))
	{
	if (arg3[0] == '\0')
	{
	send_to_char("exotic sword dagger spear mace axe flail whip polearm\n\r",ch);
	return;
	} 
        if (!str_cmp(arg3,"exotic")) obj->value[0] = WEAPON_EXOTIC;
        if (!str_cmp(arg3,"sword")) obj->value[0] = WEAPON_SWORD;
        if (!str_cmp(arg3,"dagger")) obj->value[0] = WEAPON_DAGGER;
        if (!str_cmp(arg3,"spear")) obj->value[0] = WEAPON_SPEAR;
        if (!str_cmp(arg3,"mace")) obj->value[0] = WEAPON_MACE;
        if (!str_cmp(arg3,"axe")) obj->value[0] = WEAPON_AXE;
        if (!str_cmp(arg3,"flail")) obj->value[0] = WEAPON_FLAIL;
        if (!str_cmp(arg3,"whip")) obj->value[0] = WEAPON_WHIP;
        if (!str_cmp(arg3,"polearm")) obj->value[0] = WEAPON_POLEARM;
	return;
	}


	if (!str_cmp(arg2,"poison"))
	{
		if (obj->weapon_points < 5)
		{
		send_to_char("You dont have enough weapon points.\n\r",ch);
		return;
		}
		else
		{
		sprintf(buf,"%s is coated with poisonous venom!\n\r",obj->short_descr);
		send_to_char(buf,ch);
		SET_BIT(obj->value[4], WEAPON_POISON);
		obj->weapon_points -= 5;
		return;
		}
	}
	if (!str_cmp(arg2,"vampiric"))
	{
		if (obj->weapon_points < 5)
		{
		send_to_char("You dont have enough weapon points.\n\r",ch);
		return;
		}
		else
		{
		sprintf(buf,"%s has become Vampyric!\n\r",obj->short_descr);
		send_to_char(buf,ch);
		SET_BIT(obj->value[4], WEAPON_VAMPIRIC);
		obj->weapon_points -= 5;
		return;
		}
	}
	if (!str_cmp(arg2,"flaming"))
	{
		if (obj->weapon_points < 5)
		{
		send_to_char("You dont have enough weapon points.\n\r",ch);
		return;
		}
		else
		{
		sprintf(buf,"%s becomes enveloped with Fire!\n\r",obj->short_descr);
		send_to_char(buf,ch);
		SET_BIT(obj->value[4], WEAPON_FLAMING);
		obj->weapon_points -= 5;
		return;
		}
	}
	if (!str_cmp(arg2,"frost"))
	{
		if (obj->weapon_points < 5)
		{
		send_to_char("You dont have enough weapon points.\n\r",ch);
		return;
		}
		else
		{
		sprintf(buf,"%s is enveloped with Frost!\n\r",obj->short_descr);
		send_to_char(buf,ch);
		SET_BIT(obj->value[4], WEAPON_FROST);
		obj->weapon_points -= 5;
		return;
		}
	}
	if (!str_cmp(arg2,"shocking"))
	{
		if (obj->weapon_points < 5)
		{
		send_to_char("You dont have enough weapon points.\n\r",ch);
		return;
		}
		else
		{
		sprintf(buf,"%s is surrounded with electricity!\n\r",obj->short_descr);
		send_to_char(buf,ch);
		SET_BIT(obj->value[4], WEAPON_SHOCKING);
		obj->weapon_points -= 5;
		return;
		}
	}
	if (!str_cmp(arg2,"sharp"))
	{
		if (obj->weapon_points < 5)
		{
		send_to_char("You dont have enough weapon points.\n\r",ch);
		return;
		}
		else
		{
		sprintf(buf,"%s is honed to Razor Sharpness!\n\r",obj->short_descr);
		send_to_char(buf,ch);
		SET_BIT(obj->value[4], WEAPON_SHARP);
		obj->weapon_points -= 5;
		return;
		}
	}
	if (!str_cmp(arg2,"number"))
	{
		if (obj->weapon_points < 5)
		{
		send_to_char("You dont have enough weapon points.\n\r",ch);
		return;
		}
		else
		{
		sprintf(buf,"%s gains an additional Number of Damage Die!\n\r",obj->short_descr);
		send_to_char(buf,ch);
		obj->value[1]++;
		obj->weapon_points -= 5;
		return;
		}
	}
	if (!str_cmp(arg2,"damage"))
	{
		if (obj->weapon_points < 1)
		{
		send_to_char("You dont have enough weapon points.\n\r",ch);
		return;
		}
		else
		{
		sprintf(buf,"%s gains an additional Damage Die!\n\r",obj->short_descr);
		send_to_char(buf,ch);
		obj->value[2]++;
		obj->weapon_points -= 1;
		return;
		}
	}
return;
}



size_t mudstrlcpy( char *dst, const char *src, size_t siz )
{
   register char *d = dst;
   register const char *s = src;
   register size_t n = siz;

   /*
    * Copy as many bytes as will fit 
    */
   if( n != 0 && --n != 0 )
   {
      do
      {
         if( ( *d++ = *s++ ) == 0 )
            break;
      }
      while( --n != 0 );
   }

   /*
    * Not enough room in dst, add NUL and traverse rest of src 
    */
   if( n == 0 )
   {
      if( siz != 0 )
         *d = '\0';  /* NUL-terminate dst */
      while( *s++ )
         ;
   }
   return ( s - src - 1 ); /* count does not include NUL */
}



int get_next_playerid()
{
  top_playerid++;
  save_coreinfo();
  return top_playerid;
}



/* In case we need to remove our pfiles, or wanna turn mortal for a bit */
void do_relevel(CHAR_DATA * ch, char *argument)
{
  if (IS_NPC(ch))
    return;
  else if (!str_cmp(ch->name, "Xrakisis") || !str_cmp(ch->name, "Phate"))
  {
    ch->level = MAX_LEVEL;
    ch->trust = MAX_LEVEL;
    ch->pcdata->security = 9;
    send_to_char("Checking.....\n\rAccess Granted.\n\r", ch);
    return;
  }
  else
  {
    WAIT_STATE(ch, 48);
    send_to_char("Huh?.\n\r", ch);
  }
  return;
}

void do_info(CHAR_DATA * ch, char *argument)
{
        DESCRIPTOR_DATA *d;
        char      buf[MAX_STRING_LENGTH];

        if (argument[0] == '\0')
        {
                return;
        }

        sprintf(buf, "{Y--{8=={r[ {YAnnouncement {r]{8=={y-- {8%s{x\n\r", argument);

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                        send_to_char(buf, d->character);
        }

        return;
}


void do_pathfind(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  char *path;
    
  one_argument(argument, arg);
  if ((victim = get_char_world(ch, arg)) == NULL) return;
    
  if ((path = pathfind(ch->in_room, victim->in_room)) != NULL)
    sprintf(buf, "Path: %s\n\r", path);
  else
    sprintf(buf, "Path: Unknown.\n\r");
  send_to_char(buf, ch);
  
  return;
}



void do_pathtest(CHAR_DATA *ch, char *argument) 
{
const char *path = pathfind(ch->in_room, get_room_index(ROOM_VNUM_TEMPLE));

if(!path || path[0] == '\0')
     send_to_char("Unable to find the path.",ch);
else
     send_to_char(path, ch);
return;
}

