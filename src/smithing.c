/***************************************************************************

                      _____  __      __  __      __
                     /  _  \/  \    /  \/  \    /  \
                    /  /_\  \   \/\/   /\   \/\/   /
                   /    |    \        /  \        /
                   \____|__  /\__/\  /    \__/\  /
                           \/      \/          \/

    As the Wheel Weaves based on ROM 2.4. Original code by Dalsor.
    See changes.log for a list of changes from the original ROM code.
    Credits for code created by other authors have been left
 	intact at the head of each function.

    Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,
    Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.

    Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael
    Chastain, Michael Quan, and Mitchell Tse.

    In order to use any part of this Merc Diku Mud, you must comply with
    both the original Diku license in 'license.doc' as well the Merc
    license in 'license.txt'.  In particular, you may not remove either of
    these copyright notices.

    Much time and thought has gone into this software and you are
    benefitting.  We hope that you share your changes too.  What goes
    around, comes around.

	ROM 2.4 is copyright 1993-1998 Russ Taylor
	ROM has been brought to you by the ROM consortium
	    Russ Taylor (rtaylor@hypercube.org)
	    Gabrielle Taylor (gtaylor@hypercube.org)
	    Brian Moore (zump@rom.org)
	By using this code, you have agreed to follow the terms of the
	ROM license, in the file Rom24/doc/rom.license

 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"
#include "recycle.h"
#include "interp.h"
#include "tables.h"
#include "smithing.h"

int clan_lookup args(( const char *name ));

#define SMITH_BRONZE_ORE 1
#define SMITH_IRON_ORE   2
#define SMITH_STEEL_ORE  3
#define SMITH_SILVER_ORE 4
#define SMITH_GOLD_ORE   5
#define SMITH_GEM_STONE  6
#define SMITH_PRODUCT    3398

#define MAX_SMITH_ITEMS  106
#define MAX_ORE_ITEMS   7

     void add_affect ( OBJ_DATA * obj, int apply, long value )
        {
        AFFECT_DATA * paf;

        if ( affect_free == NULL )
        paf = alloc_perm( sizeof( * paf ) );
        else
        {
        paf         = affect_free;
        affect_free = affect_free->next;
        }
        paf->type      = 0;
        paf->duration  = -1;
        paf->location  = apply;
        paf->modifier  = value;
        paf->bitvector = 0;
        paf->next      = obj->affected;
        obj->affected  = paf;
        }

int smith_item_lookup( const char *name )
{
	int item;
	
	for ( item = 0; item < MAX_SMITH_ITEMS; item++ )
    {
	if (LOWER(name[0]) == LOWER(smith_items_table[item].name[0])
	&&  !str_prefix(name,smith_items_table[item].name))
	    return item;
    }
    return 0;
}
void do_smith( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *ore;
	OBJ_DATA *item;
	OBJ_DATA *furnace;
	OBJ_DATA *anvil;
	OBJ_DATA *hammer;
	OBJ_DATA *hammer_next;
	OBJ_DATA *barrel;
	OBJ_DATA *grinder;
	char arg1[MIL];
	char arg2[MIL];
	char arg3[MIL];
	char arg4[MIL];
	char arg5[MIL];
	char name_buf[MSL];
	char short_buf[MSL];
	char long_buf[MSL];
	/*char *extra_buf = str_dup("");*/
	char *adj;
	char buf[MAX_STRING_LENGTH];
	bool infusing = FALSE, itm = FALSE, hasore = FALSE;
	bool has_furnace = FALSE, has_anvil = FALSE, has_hammer = FALSE;
	bool has_barrel = FALSE, has_grinder = FALSE;
	int i = 0, x = 0, skill = 0, difficulty = 0, output = 0, y = 0;
      int hitroll = 0;
      int damroll = 0;

	name_buf[0] = '\0';
	short_buf[0] = '\0';
	long_buf[0] = '\0';
      buf[0] = '\0';
	
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
	argument = one_argument( argument, arg5 );
	
	if ( IS_NPC(ch) || ch->desc == NULL )
	return;
	
/*	if ( ch->clan == clan_lookup( "aessedai" ) || IS_SET(ch->ajah_flags,AJAH_BLACK) )
	{
		stc( "The Oaths forbid the making of weapons for man to kill man.\n\r", ch );
		stc( "The Oaths are binding.\n\r", ch );
		return;
	}
*/

	if ( arg1[0] == '\0' )
	{
		stc("None of the arguments can be null, and the syntax must be exact.\n\r\n\r", ch );
		stc("Syntax:  smith <material> into <item>\n\r", ch );
		stc("Example: smith steel into dagger\n\r", ch );
		stc("To see what can be smithed type smith list\n\r", ch );
		return;
	}
	if ( !str_cmp( arg1, "list" ) )
	{
		for ( y = 0; y < MAX_SMITH_ITEMS-1; y++ )
		{
//			ptc( ch, "Item: %s.\n\r", smith_items_table[y].name );			
                sprintf(buf, "Item %s\n\r",smith_items_table[y].name );
                send_to_char(buf, ch);

		}
		return;
	}
	if ( arg2[0] == '\0' || arg3[0] == '\0' )
	{
		stc("None of the arguments can be null, and the syntax must be exact.\n\r\n\r", ch );
		stc("Syntax:  smith <material> into <item>\n\r", ch );
		stc("Example: smith steel into dagger\n\r", ch );
                stc("To see what can be smithed type smith list\n\r", ch );
		return;
	}
	if ( str_cmp( arg2, "into" ) )
	{
		stc("You must provide the correct, exact syntax.\n\r\n\r", ch );
		stc("Syntax:  smith <material> into <item>\n\r", ch );
		stc("Example: smith steel into dagger\n\r", ch );
                stc("To see what can be smithed type smith list\n\r", ch );
		return;
	}
/*
	if ( arg4[0] != '\0' && arg5[0] != '\0' )
	{
		if ( !str_cmp( arg4, "for" ) && !str_cmp( arg5, "infusion") )
		{
			if ( IS_SET( ch->op_flags,OP_CHANNELER ) )
			{
				infusing = TRUE;
			}
			else
			{
				stc( "You are not a channeler.\n\r", ch );
				return;
			}
		}
		else
		{
			stc("You must provide the correct, exact syntax.\n\r\n\r", ch );
			stc("Syntax:  smith <material> into <item> for infusion\n\r", ch );
			stc("Example: smith steel into dagger for infusion\n\r", ch );
	                stc("To see what can be smithed type smith list\n\r", ch );
			return;
		}		
	}
*/
	if ( ( ore = get_obj_carry( ch, arg1, ch ) ) != NULL )
	{
		if ( ore->item_type != ITEM_SMITH_ORE )
		{
			stc( "This is not a proper ore.\n\r", ch );
			return;
		}
		for ( x = 0; x < MAX_ORE_ITEMS-1; x++ )
		{
			/*ptc( ch, "Searching %s.\n\r", smith_ores_table[x].name );*/
			if ( !str_cmp( ore->name, smith_ores_table[x].name ) )
			{
				/*ptc( ch, "Got ore. Working with %s.\n\r", smith_ores_table[x].name );*/
		                sprintf(buf, "Got ore. Working with %s\n\r",smith_ores_table[x].name );
	        	        send_to_char(buf, ch);
				hasore = TRUE;
				break;
			}
		}
		if ( !hasore )
		{
			stc( "This is not a proper ore.\n\r", ch );
			return;
		}
		for ( i = 0; i < MAX_SMITH_ITEMS-1; i++ )
		{
			/* ptc( ch, "Searching %s.\n\r", smith_items_table[i].name ); */
			if ( !str_cmp( arg3, smith_items_table[i].name ) )
			{
				/*ptc( ch, "Got ore and item. Making %s from %s.\n\r",*/
                           sprintf(buf, "Got ore and item. Making %s from %s\n\r",
                           smith_items_table[i].name, smith_ores_table[x].name );
                           send_to_char(buf, ch);

				/*smith_items_table[i].name, smith_ores_table[x].name );*/
				itm = TRUE;
				break;
			}
		}
		if ( itm == FALSE )
		{
			stc( "This is not a valid item type.\n\r", ch );
			return;
		}
		/*if ( infusing == TRUE )
		{
			ptc( ch, "You are making %s from %s for infusion.\n\r", str_dup(arg3), str_dup(arg1) );
		}
		else
		{
			ptc( ch, "You are making %s from %s.\n\r", str_dup(arg3), str_dup(arg1) );
		}*/
		skill = get_skill( ch, gsn_forging );
		
		if ( skill < smith_items_table[i].difficulty )
		{
//			ptc( ch, "You failed because your skill is only %d.\n\r" 
//			         "It should be at least %d.\n\r",
//			         get_skill( ch, gsn_forging ),
//			         smith_items_table[i].difficulty );

		sprintf(buf, "You failed because your skill is only %d\n\r",get_skill( ch, gsn_forging ));
		send_to_char(buf, ch);
	        sprintf(buf, "It should be at least %d\n\r",smith_items_table[i].difficulty );
	        send_to_char(buf, ch);
	        check_improve ( ch, gsn_forging, FALSE, 1 );
			return;
		}

		difficulty = number_range(1,(smith_items_table[i].difficulty/2));
		difficulty += smith_items_table[i].difficulty;
		difficulty -= smith_ores_table[x].workability;
		if ( difficulty <= 0 )
		{
			difficulty = 1;
		}
		output = difficulty - skill;
		
		if ( skill <= 10 )
			adj = str_dup("poorly");
		else if ( skill <= 25 )
			adj = str_dup("simply");
		else if ( skill <= 50 )
			adj = str_dup("properly");
		else if ( skill <= 75 )
			adj = str_dup("well");
		else if ( skill == 90 || output < 100 )
			adj = str_dup("finely");
		else if ( skill == 100 )
			adj = str_dup("masterfully");
		else
			adj = str_dup("plainly");
		
		if ( ore->weight < ( smith_items_table[i].quantity + smith_items_table[i].weight ) / 2 )
		{
			stc( "You do not have adaquete material.\n\r", ch );
			return;
		}
		/*act("$n settles at the smithy.", ch, NULL,NULL,TO_ROOM );
		act("You settle in at the smithy.", ch, NULL,NULL,TO_CHAR );*/
		
		WAIT_STATE( ch, smith_items_table[i].beats );

		sprintf(buf, "Your are making a %d stones weight %s from %s.\n\r" \
		          "There is a difficulty penalty of %d imposed.\n\r" \
		          "The modifier for success is %d.\n\r" \
		          "The final output is %d.\n\r",
					smith_items_table[i].weight,
					smith_items_table[i].name,
					ore->name,
					difficulty,
					skill,
					output );
				send_to_char(buf, ch);
					
		if ( difficulty > skill )
		{
			act("$n was unable to properly perform the work. Fortunately, the material was spared.", ch, NULL,NULL,TO_ROOM );
			act("You were unable to properly perform the work. Fortunately, the material was spared.", ch, NULL,NULL,TO_CHAR );
	                check_improve ( ch, gsn_forging, FALSE, 1 );
			return;
		}

	    for ( furnace = ch->in_room->contents; furnace; furnace = furnace->next_content )
	    {
	    	if ( furnace == NULL )
	    	has_furnace = FALSE;
	    	if ( furnace->item_type == ITEM_SMITH_FURNACE )
	    	has_furnace = TRUE;
	    }
	    if ( !has_furnace )
    	{
    		stc("There is no furnace here for you to heat the ore.\n\r",ch);
    		return;
    	}		
		if ( ore->value[0] == 0 )
		{
			act("$n prepares the ore by placing it in the furnace.", ch,NULL,NULL,TO_ROOM);
			act("You prepare the ore by placing it in the furnace.", ch,NULL,NULL,TO_CHAR);
			if ( infusing )
				WAIT_STATE( ch, smith_items_table[i].beats );
			else
				WAIT_STATE( ch, smith_items_table[i].beats );
			ore->value[0] += 1;
			return;
		}
	    for ( anvil = ch->in_room->contents; anvil; anvil = anvil->next_content )
	    {
	    	if ( anvil == NULL )
	    	has_anvil = FALSE;
	    	if ( anvil->item_type == ITEM_SMITH_ANVIL )
	    	has_anvil = TRUE;
	    }
	    if ( !has_anvil )
    	{
    		stc("There is no anvil here for you to work upon.\n\r",ch);
    		return;
    	}		
		for ( hammer = ch->carrying; hammer != NULL; hammer = hammer_next )
		{
		    hammer_next = hammer->next_content;
		    if ( hammer->item_type == ITEM_SMITH_HAMMER )
		    {
		    	has_hammer = TRUE;
		    	break;
		    }
		}		
	    if ( !has_hammer )
    	{
    		stc("You have no hammer for which to shape this item with.\n\r",ch);
    		return;
    	}		
		if ( ore->value[0] == 1 )
		{
			act("$n places the heated ore on the anvil and begins to shape it with $t.",
				ch,hammer->short_descr,NULL,TO_ROOM);
			act("You place the heated ore on the anvil and begin to shape it with $t.",
				ch,hammer->short_descr,NULL,TO_CHAR);
			if ( infusing )
				WAIT_STATE( ch, smith_items_table[i].beats );
			else
				WAIT_STATE( ch, smith_items_table[i].beats );
			ore->value[0] += 1;
    		return;
		}
	    for ( barrel = ch->in_room->contents; barrel; barrel = barrel->next_content )
	    {
	    	if ( barrel == NULL )
	    	has_barrel = FALSE;
	    	if ( barrel->item_type == ITEM_SMITH_BARREL )
	    	has_barrel = TRUE;
	    }
	    if ( !has_barrel )
    	{
    		stc("There is no barrel of water here for you to quench your work in.\n\r",ch);
    		return;
    	}		
		if ( ore->value[0] == 2 )
		{
			act("$n places the cooling material in a barrel of water to quench it.",
				ch,NULL,NULL,TO_ROOM);
			act("You place the cooling material in a barrel of water to quench it.",
				ch,NULL,NULL,TO_CHAR);
			if ( infusing )
				WAIT_STATE( ch, smith_items_table[i].beats );
			else
				WAIT_STATE( ch, smith_items_table[i].beats );
		                check_improve ( ch, gsn_forging, TRUE, 1 );
			ore->value[0] += 1;
    		return;
		}
	    for ( grinder = ch->in_room->contents; grinder; grinder = grinder->next_content )
	    {
	    	if ( grinder == NULL )
	    	has_grinder = FALSE;
	    	if ( grinder->item_type == ITEM_SMITH_GRINDER )
	    	has_grinder = TRUE;
	    }
	    if ( !has_grinder )
    	{
    		stc("There is no grinder here for you to finish your work on.\n\r",ch);
    		return;
    	}		
		if ( ore->value[0] == 3 )
		{
			act("$n spins the grinding wheel to smooth the rough edges of $s work.",
				ch,NULL,NULL,TO_ROOM);
			act("You spin the grinding wheel to smooth the rough edges of your work.",
				ch,NULL,NULL,TO_CHAR);
			if ( infusing )
				WAIT_STATE( ch, smith_items_table[i].beats );
			else
				WAIT_STATE( ch, smith_items_table[i].beats );
			ore->value[0] += 1;
	                check_improve ( ch, gsn_forging, TRUE, 1 );
		}
		
		check_improve( ch, gsn_forging, TRUE, 9 );
		
		item = create_object( get_obj_index( SMITH_PRODUCT ), 1 );
		sprintf( name_buf, "%s %s", smith_ores_table[x].name, smith_items_table[i].name );
		sprintf( short_buf, "a %s, %s forged from %s %s",
			smith_items_table[i].name, adj, smith_ores_table[x].color, smith_ores_table[x].name );
		sprintf( long_buf, "Here lies a %s, %s forged from %s %s.",
			smith_items_table[i].name, adj, smith_ores_table[x].color, smith_ores_table[x].name );
		/*sprintf( extra_buf, "This item bears the seal of %s, the %s smith.",
			ch->name,
			get_skill( ch, gsn_forging) <= 50 ? "apprentice" :
			get_skill( ch, gsn_forging) <= 75 ? "journeyman" :
			get_skill( ch, gsn_forging) == 100 ? "master" : "reknowned" );*/
		item->level = number_range(1,ch->level);
		/*bugf("Extra_buf is now equal to %s.", extra_buf );*/
		item->name = str_dup( name_buf );
		item->short_descr = str_dup( short_buf );
		item->description = str_dup( long_buf );
		/*item->extra_descr = new_extra_descr();
		item->extra_descr->description = str_dup( extra_buf );
		item->extra_descr->keyword = str_dup(item->name);
		item->extra_descr->next = NULL;*/
		item->item_type = smith_items_table[i].item_type;
		item->wear_flags += smith_items_table[i].wear_flags;
		item->weight = ( smith_items_table[i].weight + smith_items_table[i].quantity ) / 2;
		item->condition = 100;
		item->cost = smith_items_table[i].weight + smith_items_table[i].quantity;
		item->material = str_dup( smith_ores_table[x].name );



		if ( item->item_type == ITEM_WEAPON )		
		{
			stc("Its a weapon!\n\r", ch );
			item->value[0] = smith_items_table[i].base_v0;
			item->value[1] = smith_items_table[i].base_v1;
			item->value[2] = smith_items_table[i].base_v2;
			item->value[2] += (skill/1.5) + (smith_ores_table[x].armor_mod*2.5);
			item->value[3] = attack_lookup( smith_items_table[i].dam_noun );
		      hitroll += get_skill( ch, gsn_forging ) / 3;
        		damroll += get_skill( ch, gsn_forging ) / 3;
/*	if ( str_cmp( arg1, "gemstone" ) ) 
	{ hitroll += 18; damroll += 18;}
	if ( str_cmp( arg1, "gold" ) ) 
	{ hitroll += 15; damroll += 15;}
	if ( str_cmp( arg1, "steel" ) ) 
	{ hitroll += 12; damroll += 12;}
	if ( str_cmp( arg1, "iron" ) ) 
	{ hitroll += 8; damroll += 8;}
	if ( str_cmp( arg1, "bronze" ) ) 
	{ hitroll += 5; damroll += 5; }
 */
      add_affect( item, APPLY_HITROLL, hitroll );
      add_affect( item, APPLY_DAMROLL, damroll );
		}
		if ( item->item_type == ITEM_ARMOR )
		{
			stc("Its an armor!\n\r", ch );
/*
			item->value[0] = smith_items_table[i].base_pierce;
			item->value[0] += smith_ores_table[i].armor_mod;
			item->value[0] += (skill / 2);
			item->value[1] = smith_items_table[i].base_bash;
			item->value[1] += smith_ores_table[i].armor_mod;
			item->value[1] += (skill / 2);
			item->value[2] = smith_items_table[i].base_slash;
			item->value[2] += smith_ores_table[i].armor_mod;
			item->value[2] += (skill / 2);
			item->value[3] = smith_items_table[i].base_exotic;
			item->value[3] += smith_ores_table[i].armor_mod;
			item->value[3] += (skill / 2);
*/
			item->value[0] += 50;
			item->value[1] += 50;
			item->value[2] += 50;
			item->value[3] += 50;


		      hitroll += get_skill( ch, gsn_forging ) / 3;
        		damroll += get_skill( ch, gsn_forging ) / 3;
/*	if ( str_cmp( arg1, "gemstone" ) ) 
	{ hitroll += 18; damroll += 18;}
	if ( str_cmp( arg1, "gold" ) ) 
	{ hitroll += 15; damroll += 15;}
	if ( str_cmp( arg1, "steel" ) ) 
	{ hitroll += 12; damroll += 12;}
	if ( str_cmp( arg1, "iron" ) ) 
	{ hitroll += 8; damroll += 8;}
	if ( str_cmp( arg1, "bronze" ) ) 
	{ hitroll += 5; damroll += 5; }
*/
	            add_affect( item, APPLY_HITROLL, hitroll );
   	            add_affect( item, APPLY_DAMROLL, damroll );
		}		
/*		if ( item->item_type == ITEM_RAZOR )
		{
			stc("Its a razor!\n\r", ch );
			item->value[0] = ch->level / 2;
		}
*/
		extract_obj( ore );
		obj_to_char( item, ch );
                check_improve ( ch, gsn_forging, TRUE, 1 );
	}
	else
	{
		stc("What are you trying to smith?\n\r", ch );
		stc("Syntax:  smith <material> into <item>\n\r", ch );
		stc("Example: smith steel into dagger\n\r", ch );
                stc("To see what can be smithed type smith list\n\r", ch );
		return;
	}
	tail_chain();
}
