
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
#include "interp.h"
#include "magic.h"
#include "recycle.h"

void spell_cocoon (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED (victim, AFF_COCOON))
    {
        if (victim == ch)
            send_to_char ("You already have a Cocoon.\n\r", ch);
        else
            act ("$N already has a Cocoon.", ch, NULL, victim, TO_CHAR);
        return;
    }

    af.where = TO_AFFECTS;
    af.type = sn;
    af.level = level;
    af.duration = level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = AFF_COCOON;
    affect_to_char (victim, &af);
    act ("$n's body is surrounded by a thick, opaque epidermis.", victim, NULL, NULL, TO_ROOM);
    send_to_char ("Your body is surrounded by a thick, opaque epidermis.\n\r", victim);
    return;
}


/* energy spells */
void spell_etheral_fist( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 12 );
    dam *= (ch->spellpower * 4);
    if ( saves_spell( level, victim, DAM_ENERGY ) )
	dam /= 2;
    act("A fist of black, otherworldly ether rams into $N, leaving $M looking stunned!"
		,ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}

void spell_spectral_furor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    dam *= (ch->spellpower * 4);
    if ( saves_spell( level, victim, DAM_ENERGY ) )
	dam /= 2;
    act("The fabric of the cosmos strains in fury about $N!",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}

void spell_disruption( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    dam *= (ch->spellpower * 4);
    if ( saves_spell( level, victim, DAM_ENERGY ) )
	dam /= 2;
    act("A weird energy encompasses $N, causing you to question $S continued existence.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}


void spell_sonic_resonance( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_ENERGY ) )
	dam /= 2;
    act("A cylinder of kinetic energy enshrouds $N causing $S to resonate.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    WAIT_STATE( victim, skill_table[sn].beats );
    return;
}
/* mental */
void spell_mind_wrack( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_MENTAL ) )
	dam /= 2;
    act("$n stares intently at $N, causing $N to seem very lethargic.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_MENTAL,TRUE);
    return;
}

void spell_mind_wrench( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_MENTAL ) )
	dam /= 2;
    act("$n stares intently at $N, causing $N to seem very hyperactive.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_MENTAL,TRUE);
    return;
}
/* acid */
void spell_sulfurus_spray( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
    act("A stinking spray of sulfurous liquid rains down on $N." ,
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}

void spell_caustic_font( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
    act("A fountain of caustic liquid forms below $N.  The smell of $S degenerating tissues is revolting! ",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}


/*  Electrical  */

void spell_galvanic_whip( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
	dam /= 2;
    act("$n conjures a whip of ionized particles, which lashes ferociously at $N.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}


void spell_magnetic_trust( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
	dam /= 2;
    act("An unseen energy moves nearby, causing your hair to stand on end!",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}

void spell_quantum_spike( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
	dam /= 2;
    act("$N seems to dissolve into tiny unconnected particles, then is painfully reassembled.",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}

/* negative */
void spell_hand_of_undead( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( saves_spell( level, victim,DAM_NEGATIVE) )
    {
	send_to_char("You feel a momentary chill.\n\r",victim);
	return;
    }

    if ( (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
	{
	 send_to_char("Your victim is unaffected by hand of undead.\n\r",ch);
	 return;
	}
    if ( victim->level <= 2 )
    {
	dam		 = ch->hit + 1;
    }
    else
    {
     dam = dice( level, 10 );
    dam *= (ch->spellpower * 4);

     victim->mana	/= 2;
     victim->move	/= 2;
     ch->hit		+= dam / 2;
    }

    send_to_char("You feel your life slipping away!\n\r",victim);
    act("$N is grasped by an incomprehensible hand of undead!",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_NEGATIVE,TRUE);
    return;
}

void spell_precision( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED ( victim, AFF_PRECISION ) )
	{
	if ( victim == ch )
	    send_to_char("You are already affected.\n\r", ch);
	else
	    act("$N is already affected.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where	 = TO_AFFECTS;
    af.type	 = sn;
    af.level	 = level;
    af.duration	 = level*4;
    af.location	 = APPLY_HITROLL;
    af.modifier	 = victim->hitroll;
    af.bitvector = AFF_PRECISION;
    affect_to_char( victim, &af );
    send_to_char("You feel more lethal!\n\r", victim);
    if ( ch != victim )
	act("$N looks more lethal.",ch,NULL,victim,TO_CHAR);
    return;
}


/* holy */
void spell_wrath_of_god( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = number_range(2500,4500);
    dam *= (ch->spellpower * 4);


    send_to_char("You feel your life slipping away!\n\r",victim);
    act("$N is grasped by an incomprehensible hand of undead!",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_HOLY,TRUE);
    return;
}

// Templar
void spell_knightshield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    OBJ_DATA *item;
    char buf     [MAX_STRING_LENGTH];
    int hitroll = 0;
    int damroll = 0;
    int armor   = 0;
            
    item = create_object( get_obj_index( OBJ_VNUM_KNIGHTSHIELD ), 1 );
   
     
    free_string(item->name);
    sprintf(buf,"Templar Shield of %s",god_table[ch->god].name);
    item->name = str_dup(buf);
    free_string(item->short_descr);
    sprintf(buf,"Templar Shield of %s",god_table[ch->god].name);
    item->short_descr = str_dup(buf);
    free_string(item->description);
    sprintf(buf,"Templar Shield of %s",god_table[ch->god].name);
    item->description = str_dup(buf);

//    SET_BIT( item->item_type, ITEM_ARMOR );
    item->item_type = ITEM_ARMOR;
    SET_BIT( item->wear_flags, ITEM_TAKE );
    SET_BIT( item->wear_flags, ITEM_WEAR_SHIELD );
    SET_BIT( item->extra_flags, ITEM_MAGIC);
    SET_BIT( item->extra_flags, ITEM_GLOW);       
    item->value[0] = 200;  // pierce
    item->value[1] = 200;  // bash 
    item->value[2] = 200;  // slash
    item->value[3] = 200;  // magic
    item->level = ch->level;
    item->condition = 100;
    item->weight = 10;
    if (ch->level <= 50) hitroll = 50; 
    else hitroll = ch->level;
    if (ch->level <= 50) damroll = 50; 
    else damroll = ch->level;
    armor = number_range(-200,-300);
    add_affect( item, APPLY_HITROLL, hitroll );
    add_affect( item, APPLY_DAMROLL, damroll );

    obj_to_char(item,ch);
    act("$p fades into existance in your hand.", ch, item, NULL, TO_CHAR);
    act("$p fades into existance in $n's hand.", ch, item, NULL, TO_ROOM);
    return;
}

// Templar
void spell_godsword( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    OBJ_DATA *item;
    char buf     [MAX_STRING_LENGTH];
    int hitroll = 0;
    int damroll = 0;
    int armor   = 0;
            
    item = create_object( get_obj_index( OBJ_VNUM_KNIGHTSWORD ), 1 );
   
     
    free_string(item->name);
    sprintf(buf,"Templar Broad-Sword of %s",god_table[ch->god].name);
    item->name = str_dup(buf);
    free_string(item->short_descr);
    sprintf(buf,"Templar Broad-Sword of %s",god_table[ch->god].name);
    item->short_descr = str_dup(buf);
    free_string(item->description);
    sprintf(buf,"Templar Broad-Sword of %s",god_table[ch->god].name);
    item->description = str_dup(buf);

    item->item_type = ITEM_WEAPON;
    SET_BIT( item->wear_flags, ITEM_TAKE );
    SET_BIT( item->wear_flags, ITEM_WIELD );
    SET_BIT( item->extra_flags, ITEM_MAGIC);
    SET_BIT( item->extra_flags, ITEM_GLOW);       
    item->value[1] = 20;  // bash 
    item->value[2] = 20;  // slash
    item->value[3] = WEAPON_SWORD;
    SET_BIT(item->value[4], WEAPON_FLAMING);
    item->level = ch->level;
    item->condition = 100;
    item->weight = 10;
    if (ch->level <= 50) hitroll = 50; 
    else hitroll = ch->level;
    if (ch->level <= 50) damroll = 50; 
    else damroll = ch->level;
    armor = number_range(-200,-300);
    add_affect( item, APPLY_HITROLL, hitroll );
    add_affect( item, APPLY_DAMROLL, damroll );

    obj_to_char(item,ch);
    act("$p fades into existance in your hand.", ch, item, NULL, TO_CHAR);
    act("$p fades into existance in $n's hand.", ch, item, NULL, TO_ROOM);
    return;
}


void spell_repair( int sn, int level, CHAR_DATA *ch, void *vo , int target )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found = FALSE;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->condition < 100 && can_see_obj( ch, obj ) )
	{
	    found = TRUE;
	    obj->condition = 100;
	    act("$p magically repairs itself.",ch,obj,NULL,TO_CHAR);
	    act("$p magically repairs itself.",ch,obj,NULL,TO_ROOM);
	}
    }
    if ( !found )
    {
	send_to_char( "None of your equipment needs repairing.\n\r", ch );
	return;
    }
    return;
}

/* Siphon Life spell for warlocks/voodans by Dusk */

void spell_siphon_life( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
   
    int dam, mod, pbonus, fbonus, dbonus;    
 
//    if ( ( ch->fighting == NULL )
//    && ( !IS_NPC( ch ) )

	/*Focus Level version is hella bugged! -Fesdor
    pbonus = ( !IS_NPC( ch ) && !IS_NPC( victim ) ? focus_level(ch->pcdata->focus[MAGIC_POWER]) - 
focus_level(victim->pcdata->focus[MAGIC_POWER]) : 0 );
    fbonus = ( !IS_NPC( ch ) ? ch->pcdata->focus[MAGIC_ABILITY] : 0 );
    dbonus = ( !IS_NPC( victim ) ? victim->pcdata->focus[MAGIC_DEFENSE] : 0 );

    fbonus = ( ( fbonus - dbonus < 0 ) ? 0 : fbonus );
    
    pbonus =  ( pbonus < 0 ? -1 : pbonus / 2 );
    pbonus =  ( pbonus > 5 ? 5 : pbonus );   

    mod = ( ( ch->level + get_curr_stat( ch , STAT_INT ) + fbonus ) - ( victim->saving_throw * -1 + get_curr_stat( 
victim , STAT_WIS ) ) );
    */
    pbonus = 0;
    fbonus = 0;
    dbonus = 0;
	mod = number_range(get_curr_stat(ch,STAT_WIS)*4,get_curr_stat(ch,STAT_WIS)*5)-(ch->saving_throw - 
victim->saving_throw);
    dam = ch->level + get_curr_stat( ch, STAT_INT ) * 8 - victim->saving_throw * -1;
    dam *= (ch->spellpower * 4);

//    dam = ( check_sorcery( ch, sn ) ? sorcery_dam2(dam*1.15) : dam );
    
    if ( victim == ch )
    {
       send_to_char("{wYou can't siphon your own life!{x\n\r",ch);
       return;
    }   
 
    if ( is_affected( victim , gsn_siphon ) || is_affected( ch , gsn_siphon)  )
    {
       send_to_char("{mYou draw on your siphoned link to gain more life!{x\n\r",ch);
       send_to_char("{RYou feel your lifeforce fading fast as it is being drawn on!{x\n\r",victim);
       
       victim->hit -= dam;
       ch->hit += dam;      
    }
    else
    {

       af.where            = TO_AFFECTS;
       af.type             = gsn_siphon;
       af.level            = ch->level;
       af.duration         = number_range(1,4);
       af.location         = APPLY_REGEN;
       af.modifier         = mod;
       af.bitvector        = 0; 
    
       affect_to_char(ch, &af);
    
       af.modifier         = mod * -1;
    
       affect_to_char(victim, &af);
    
       send_to_char("{RYou feel your life being siphoned away!{x\n\r",victim);
       send_to_char("{mYou feel your life being restored by your siphon!{x\n\r",ch);
    }
        
    damage( ch, victim, dam, sn,DAM_HARM,TRUE);
    return;
}

/* End of Siphon Life Spell by Dusk */
/* Siphon Energy Spell by Dusk */

void spell_siphon_energy( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
   
    int dam, mod, pbonus, fbonus, dbonus;    
 
//    if ( ( ch->fighting == NULL )
//    && ( !IS_NPC( ch ) )

	/*Same problem as siphon life -Fesdor
    pbonus = ( !IS_NPC( ch ) && !IS_NPC( victim ) ? focus_level(ch->pcdata->focus[MAGIC_POWER]) - 
focus_level(victim->pcdata->focus[MAGIC_POWER]) : 0 );
    fbonus = ( !IS_NPC( ch ) ? ch->pcdata->focus[MAGIC_ABILITY] : 0 );
    dbonus = ( !IS_NPC( victim ) ? victim->pcdata->focus[MAGIC_DEFENSE] : 0 );

    fbonus = ( ( fbonus - dbonus < 0 ) ? 0 : fbonus );
    
    pbonus =  ( pbonus < 0 ? -1 : pbonus / 2 );
    pbonus =  ( pbonus > 5 ? 5 : pbonus );
    */   

    pbonus = 0;
    fbonus = 0;
    dbonus = 0;
	mod = number_range(get_curr_stat(ch,STAT_WIS)*4,get_curr_stat(ch,STAT_WIS)*5)-(ch->saving_throw - 
victim->saving_throw);
	mod /= 2;
	/*	Line above replaces this:
	mod = ( ( ch->level + get_curr_stat( ch , STAT_INT ) + fbonus )	- ( victim->saving_throw * -1 + get_curr_stat( 
victim , STAT_WIS ) ) );
	*/
	dam = ch->level + get_curr_stat( ch, STAT_INT ) * 8 - victim->saving_throw * -1;
    dam *= (ch->spellpower * 4);

//    dam = ( check_sorcery( ch, sn ) ? sorcery_dam2(dam*1.15) : dam );
    
    if ( victim == ch )
    {
       send_to_char("{wYou can't siphon your own energy!{x\n\r",ch);
       return;
    }   
 
    if ( is_affected( victim , gsn_siphon ) || is_affected( ch , gsn_siphon)  )
    {
       send_to_char("{mYou draw on your siphoned link to gain more energy!{x\n\r",ch);
       send_to_char("{RYou feel your energy supply fading fast as it is being drawn on!{x\n\r",victim);
       
       victim->mana -= dam;
       if ( victim-> mana < 0 )
          victim->mana = 0;
       else
          ch->mana += dam;      
    }
    else
    {

       af.where            = TO_AFFECTS;
       af.type             = gsn_siphon;
       af.level            = ch->level;
       af.duration         = number_range(1,4);
       af.location         = APPLY_MANA_REGEN;
       af.modifier         = mod;
       af.bitvector        = 0; 
    
       affect_to_char(ch, &af);
    
       af.modifier         = mod * -1;
    
       affect_to_char(victim, &af);
    
       send_to_char("{RYou feel your energy being siphoned away!{x\n\r",victim);
       send_to_char("{mYou feel your energy being restored by your siphon!{x\n\r",ch);
    }
        
    damage( ch, victim, dam, sn,DAM_HARM,TRUE);
    return;
}

