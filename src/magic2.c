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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"


extern char *target_name;


void spell_web( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    act("You point your finger at $N and a web flies from your hand!",ch,NULL,victim,TO_CHAR);
    act("$n points $s finger at $N and a web flies from $s hand!",ch,NULL,victim,TO_NOTVICT);
    act("$n points $s finger at you and a web flies from $s hand!",ch,NULL,victim,TO_VICT);

    if ( IS_AFFECTED2(victim, AFF2_WEBBED) )
    {
	send_to_char( "But they are already webbed!\n\r", ch );
	return;
    }

    if ( is_safe(ch, victim)) return;

    if ((saves_spell( level, victim, DAM_OTHER ) && number_range(1,3)!=1) && victim->position >= POS_FIGHTING )
    {
	send_to_char( "You dodge the web!\n\r", victim );
	act("$n dodges the web!",victim,NULL,NULL,TO_ROOM);
	return;
    }
    af.where = TO_AFFECTS2;
    af.type      = sn;
    af.location  = APPLY_AC;
    af.modifier  = 200;
    af.duration  = number_range(50,60);
    af.bitvector = AFF2_WEBBED;
    affect_to_char( victim, &af );


//	SET_BIT (ch->affected_by2, AFF2_WEBBED);

    send_to_char( "You are coated in a sticky web!\n\r", victim );
    act("$n is coated in a sticky web!",victim,NULL,NULL,TO_ROOM);
    return;
}


/* Spellup spell a'la stone skin */
void spell_bark_skin( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already covered in bark.\n\r",ch);
	else
	  act("$N's skin is already bark.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level/3;
    af.modifier  = -30 - level / 5;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "Your skin becomes as tough as bark.\n\r", victim );
    if ( ch != victim )
	act("$N's skin becomes as tough as bark.",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_animal_instinct( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already animalistic.\n\r",ch);
	else
	  act("$N is already animalistic.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level/2;
    af.modifier  = level/25;
    af.location  = APPLY_STR;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level/2;
    af.modifier  = level/20;
    af.location  = APPLY_DAMROLL;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You suddenly look like a wild beast!\n\r", victim );
    if ( ch != victim )
	act("$N suddenly grows fangs and claws!",ch,NULL,victim,TO_CHAR);
    return;
}


/* Quite strong area affect spell */
void spell_powerstorm( int sn, int level, CHAR_DATA *ch, void *vo,int target )

{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    act( "$n makes a firey blaze of magic engulf the room!", ch, NULL, NULL, TO_ROOM );
    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && !is_safe_spell(ch,vch,TRUE))
		    damage( ch,vch,level / 3 * 2 + dice(20, 20), sn, DAM_FIRE,TRUE);
	    continue;
	}
	if ( vch->in_room->area == ch->in_room->area )
	    send_to_char( "A blazing storm of energy rumbles through the area.\n\r", vch );
    }
    return;
}
void spell_power_kill ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

   
    act( "A stream of darkness from your finger surrounds $N.", 
		ch, NULL, victim, TO_CHAR );
    act( "A stream of darkness from $n's finger surrounds $N.", 
		ch, NULL, victim, TO_NOTVICT );
    act( "A stream of darkness from $N's finger surrounds you.", 
		victim, NULL, ch, TO_CHAR);



    if (saves_spell(level,victim,DAM_MENTAL)) 
	{
	 dam = dice( level , 24 ) ;
    dam *= (ch->spellpower * 4);
	 damage(ch, victim , dam , sn, DAM_MENTAL, TRUE);
	 return;
	}

    send_to_char( "You have been KILLED!\n\r", victim );

    act("$N has been killed!\n\r", ch, NULL, victim, TO_CHAR);
    act("$N has been killed!\n\r", ch, NULL, victim, TO_ROOM);

    raw_kill(victim);
    return;
}


void spell_power_stun ( int sn, int level, CHAR_DATA *ch, void *vo , int target) 
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;


    if (IS_AFFECTED2(victim,sn ) || saves_spell( level, victim,DAM_OTHER) )
	return;

    af.where     = TO_AFFECTS2;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 90;
    af.location  = APPLY_DEX;
    af.modifier  = - 3;
    af.bitvector = AFF_STUN;
    affect_to_char( victim, &af );
    send_to_char( "You are stunned.\n\r", victim );
    act("$n is stunned.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_investiture( int sn, int level, CHAR_DATA *ch, void *vo,int target)

{

    CHAR_DATA *victim = (CHAR_DATA *) vo;

    int heal;

    heal = ch->move;

    victim->mana = UMIN( victim->mana + heal, victim->max_mana );

    victim->move = 0;

    update_pos( victim );

    send_to_char( "{cThe forces of the earth fill you with energy!{x\n\r", victim );

    act( "$n draws magic from the very earth!", ch, NULL, NULL, TO_ROOM );

}


void spell_channel( int sn, int level, CHAR_DATA *ch, void *vo,int target)

{

    CHAR_DATA *victim = (CHAR_DATA *) vo;

    int heal;

    heal = dice(3, 3) + (level / 3) * 2;
    heal *= (ch->spellpower * 4);

    if (ch == victim)

     {

      send_to_char( "You cannot channel energy into yourself.\n\r", ch );

      return;

     }

    victim->mana = UMIN( victim->mana + heal, victim->max_mana );

    update_pos( victim );

    send_to_char( "A swirling cloud of energy engulfs you!\n\r", victim );

    if ( ch != victim )

	send_to_char( "A swirling cloud of energy slips from your fingertips.\n\r", ch );

    return;

}


void spell_farsight (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    if (IS_AFFECTED (ch, AFF_BLIND))
    {
        send_to_char ("Maybe it would help if you could see?\n\r", ch);
        return;
    }

    do_function (ch, &do_scan, target_name);
}


void spell_portal (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal, *stone;

    if ((victim = get_char_world (ch, target_name)) == NULL
        || victim == ch
        || victim->in_room == NULL
        || !can_see_room (ch, victim->in_room)
        || IS_SET (victim->in_room->room_flags, ROOM_SAFE)
        || IS_SET (victim->in_room->room_flags, ROOM_PRIVATE)
        || IS_SET (victim->in_room->room_flags, ROOM_SOLITARY)
        || IS_SET (victim->in_room->room_flags, ROOM_NO_RECALL)
        || IS_SET (ch->in_room->room_flags, ROOM_NO_RECALL)
        || victim->level >= level + 3 || (!IS_NPC (victim) && victim->level >= LEVEL_HERO)    /* NOT trust */
        || (IS_NPC (victim) && IS_SET (victim->imm_flags, IMM_SUMMON))
        || (IS_NPC (victim) && saves_spell (level, victim, DAM_NONE))
        || (is_clan (victim) && !is_same_clan (ch, victim)))
    {
        send_to_char ("You failed.\n\r", ch);
        return;
    }

    stone = get_eq_char (ch, WEAR_HOLD);
    if (!IS_IMMORTAL (ch)
        && (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
        send_to_char ("You lack the proper component for this spell.\n\r",
                      ch);
        return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
        act ("You draw upon the power of $p.", ch, stone, NULL, TO_CHAR);
        act ("It flares brightly and vanishes!", ch, stone, NULL, TO_CHAR);
        extract_obj (stone);
    }

    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 2 + level / 25;
    portal->value[3] = victim->in_room->vnum;

    obj_to_room (portal, ch->in_room);

    act ("$p rises up from the ground.", ch, portal, NULL, TO_ROOM);
    act ("$p rises up before you.", ch, portal, NULL, TO_CHAR);
}

void spell_nexus (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal, *stone;
    ROOM_INDEX_DATA *to_room, *from_room;

    from_room = ch->in_room;

    if ((victim = get_char_world (ch, target_name)) == NULL
        || victim == ch
        || (to_room = victim->in_room) == NULL
        || !can_see_room (ch, to_room) || !can_see_room (ch, from_room)
        || IS_SET (to_room->room_flags, ROOM_SAFE)
        || IS_SET (from_room->room_flags, ROOM_SAFE)
        || IS_SET (to_room->room_flags, ROOM_PRIVATE)
        || IS_SET (to_room->room_flags, ROOM_SOLITARY)
        || IS_SET (to_room->room_flags, ROOM_NO_RECALL)
        || IS_SET (from_room->room_flags, ROOM_NO_RECALL)
        || victim->level >= level + 3 || (!IS_NPC (victim) && victim->level >= LEVEL_HERO)    /* NOT trust */
        || (IS_NPC (victim) && IS_SET (victim->imm_flags, IMM_SUMMON))
        || (IS_NPC (victim) && saves_spell (level, victim, DAM_NONE))
        || (is_clan (victim) && !is_same_clan (ch, victim)))
    {
        send_to_char ("You failed.\n\r", ch);
        return;
    }

    stone = get_eq_char (ch, WEAR_HOLD);
    if (!IS_IMMORTAL (ch)
        && (stone == NULL || stone->item_type != ITEM_WARP_STONE))
    {
        send_to_char ("You lack the proper component for this spell.\n\r",
                      ch);
        return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
        act ("You draw upon the power of $p.", ch, stone, NULL, TO_CHAR);
        act ("It flares brightly and vanishes!", ch, stone, NULL, TO_CHAR);
        extract_obj (stone);
    }

    /* portal one */
    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 1 + level / 10;
    portal->value[3] = to_room->vnum;

    obj_to_room (portal, from_room);

    act ("$p rises up from the ground.", ch, portal, NULL, TO_ROOM);
    act ("$p rises up before you.", ch, portal, NULL, TO_CHAR);

    /* no second portal if rooms are the same */
    if (to_room == from_room)
        return;

    /* portal two */
    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 1 + level / 10;
    portal->value[3] = from_room->vnum;

    obj_to_room (portal, to_room);

    if (to_room->people != NULL)
    {
        act ("$p rises up from the ground.", to_room->people, portal, NULL,
             TO_ROOM);
        act ("$p rises up from the ground.", to_room->people, portal, NULL,
             TO_CHAR);
    }
}


void spell_forceshield (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED2 (victim, AFF_FORCE_SHIELD))
    {
        if (victim == ch)
            send_to_char ("You are already force-shielded.\n\r", ch);
        else
            act ("$N is already force-shielded.", ch, NULL, victim,
                 TO_CHAR);
        return;
    }

    af.where = TO_AFFECTS2;
    af.type = sn;
    af.level = level;
    af.duration = level / 4;
    af.location = APPLY_AC;
    af.modifier = (ch->spellpower * 1.8);
    af.bitvector = AFF_FORCE_SHIELD;
    affect_to_char (victim, &af);
    act ("A sparkling force-shield encircles $n.", victim, NULL, NULL, TO_ROOM);
    send_to_char ("You are encircled by a sparkling force-shield.\n\r", victim);
    return;
}

void spell_staticshield (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED2 (victim,AFF_STATIC_SHIELD ))
    {
        if (victim == ch)
            send_to_char ("You are surrounded by static charge.\n\r", ch);
        else
            act ("$N is already surrounded by static charge.", ch, NULL, victim,
                 TO_CHAR);
        return;
    }

    af.where = TO_AFFECTS2;
    af.type = sn;
    af.level = level;
    af.duration = level / 3;
    af.location = APPLY_AC;
    af.modifier = (ch->spellpower * 1.8);
    af.bitvector = AFF_STATIC_SHIELD;
    affect_to_char (victim, &af);
    act ("$n is surrounded by a pulse of static charge.", victim, NULL, NULL, TO_ROOM);
    send_to_char ("You are surrounded by a pulse of static charge.\n\r", victim);
    return;
}

void spell_flameshield (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED2(victim, AFF_FLAME_SHIELD))
    {
        if (victim == ch)
            send_to_char ("You are already protected by fire.\n\r", ch);
        else
            act ("$N is already protected by fire.", ch, NULL, victim,
                 TO_CHAR);
        return;
    }

    af.where = TO_AFFECTS;
    af.type = sn;
    af.level = level;
    af.duration = (level / 10);
    af.location = APPLY_AC;
    af.modifier = (ch->spellpower * 1.8);
    af.bitvector = AFF_FLAME_SHIELD;
    affect_to_char (victim, &af);
    act ("$n is shielded by red walls of flame.", victim, NULL, NULL, TO_ROOM);
    send_to_char ("You are shielded by red walls of flame.\n\r", victim);
    return;
}

void spell_mental_knife( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

  if (ch->level < 40)
        dam = dice(level,8);
  else if (ch->level < 65)
        dam = dice(level,11);
  else dam = dice(level,14);

    dam *= (ch->spellpower * 4);

  if (saves_spell(level,victim, DAM_MENTAL))
              dam /= 2;
  damage(ch,victim,dam,sn,DAM_MENTAL, TRUE);

  if (!is_affected(victim,sn) && !saves_spell(level, victim, DAM_MENTAL))
    {
      af.where              = TO_AFFECTS;
      af.type               = sn;
      af.level              = level;
      af.duration           = level;
      af.location           = APPLY_INT;
      af.modifier           = -7;
      af.bitvector          = 0;
      affect_to_char(victim,&af);

      af.location = APPLY_WIS;
      affect_to_char(victim,&af);
      act("Your mental knife sears $N's mind!",ch,NULL,victim,TO_CHAR);
      act("$n's mental knife sears your mind!",ch,NULL,victim,TO_VICT);
      act("$n's mental knife sears $N's mind!",ch,NULL,victim,TO_NOTVICT);
    }
}

void spell_acetum_primus( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    dam *= (ch->spellpower * 4);

    if ( saves_spell( level, victim, DAM_ACID ) 
)
	dam /= 2;
    act("A cloak of primal acid enshrouds $N, sparks form as it consumes all it touches. ",
		ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}


void spell_acute_vision( int sn, int level, CHAR_DATA *ch, void *vo, int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ACUTE_VISION) )
    {
        if (victim == ch)
          send_to_char("Your vision is already acute. \n\r",ch);
        else
          act("$N already sees acutely.",ch,NULL,victim,TO_CHAR);
        return;
    }
    af.where		= TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (ch->spellpower * 2);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ACUTE_VISION;
    affect_to_char( victim, &af );
    send_to_char( "Your vision sharpens.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}


void spell_adamantite_golem( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *golem;
  AFFECT_DATA af;
  int i = 0;

  if (is_affected(ch,sn))
    {
      send_to_char("You lack the power to create another golem right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to create an Adamantite golem.\n\r",ch);
  act("$n attempts to create an Adamantite golem.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  ( gch->pIndexData->vnum == MOB_VNUM_ADAMANTITE_GOLEM ) )
	{
	    send_to_char("More golems are more than you can control!\n\r",ch);
	    return;
	}
    }

  golem = create_mobile( get_mob_index(MOB_VNUM_ADAMANTITE_GOLEM) );


  for (i = 0; i < MAX_STATS; i ++)
       golem->perm_stat[i] = UMIN(25,15 + ch->level/10);
            
  golem->perm_stat[STAT_STR] += 3;
  golem->perm_stat[STAT_INT] -= 1;
  golem->perm_stat[STAT_CON] += 2;

  golem->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: UMIN( (10 * ch->pcdata->perm_hit) + 4000, 30000);
	golem->max_hit += (ch->spellpower * 10);
  golem->hit = golem->max_hit;
  golem->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  golem->mana = golem->max_mana;
  golem->level = ch->level;
  for (i=0; i < 3; i++)
    golem->armor[i] = interpolate(golem->level,100,-100);
  golem->armor[3] = interpolate(golem->level,100,0);
  golem->gold = 0;
  golem->timer = 0;
  golem->damage[DICE_NUMBER] = 13;   
  golem->damage[DICE_TYPE] = 9;
  golem->damage[DICE_BONUS] = ch->level / 2 + 10;

  char_to_room(golem,ch->in_room);
  send_to_char("You created an Adamantite golem!\n\r",ch);
  act("$n creates an Adamantite golem!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(golem->affected_by, AFF_CHARM);
  golem->master = golem->leader = ch;

}


void spell_scourge( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *tmp_vict;
  CHAR_DATA *tmp_next;
  char buf[MAX_STRING_LENGTH];
  int dam;



  if (ch->level < 40)
	dam = dice(level,6);
  else if (ch->level < 65)
	dam = dice(level,9);
  else dam = dice(level,12);

    dam *= (ch->spellpower * 4);

  level += 120;

  for (tmp_vict = ch->in_room->people;tmp_vict != NULL;
       tmp_vict = tmp_next)
    {
      tmp_next = tmp_vict->next_in_room;

      if ( !is_safe_spell(ch,tmp_vict,TRUE))
	{ 
	  if (!IS_NPC(ch) && tmp_vict != ch && 
	      ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
	      (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
	    {
	      if (!can_see(tmp_vict, ch))
		do_yell(tmp_vict, "Help someone is attacking me!");
	      else 
		{
		  sprintf(buf,"Die, %s, you sorcerous dog!",
		    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
		     ch->doppel->name : ch->name);
		  do_yell(tmp_vict,buf);
		}
	    }
	    
	  if (!is_affected(tmp_vict,sn)) {
	    

	    if (number_percent() < level) 
	      spell_poison(gsn_poison, level, ch, tmp_vict, TARGET_CHAR);

	    if (number_percent() < level)
	      spell_blindness(gsn_blindness,level,ch,tmp_vict, TARGET_CHAR);

	    if (number_percent() < level)
	      spell_weaken(gsn_weaken, level, ch, tmp_vict, TARGET_CHAR);

            if (saves_spell(level,tmp_vict, DAM_FIRE))
	      dam /= 2;
	    damage( ch, tmp_vict, dam, sn, DAM_FIRE, TRUE );
	  }

	}
    }
}


void spell_doppelganger( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( (ch == victim) || 
      (is_affected(ch, sn) && (ch->doppel == victim)) )
    {
      act("You already look like $M.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (IS_NPC(victim))
    {
     act("$N is too different from yourself to mimic.",ch,NULL,victim,TO_CHAR);
     return;
   }

  if (IS_IMMORTAL(victim))
    {
      send_to_char("Yeah, sure. And I'm the Pope.\n\r",ch);
      return;
    }

  if (saves_spell(level,victim, DAM_CHARM))
   {
    send_to_char("You failed.\n\r",ch);
    return;
   }

  act("You change form to look like $N.",ch,NULL,victim,TO_CHAR);
  act("$n changes form to look like YOU!",ch,NULL,victim,TO_VICT);
  act("$n changes form to look like $N!",ch,NULL,victim,TO_NOTVICT);

  af.where 		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = (2 * level)/3;
  af.location           = APPLY_NONE;
  af.modifier           = 0;
  af.bitvector          = 0;

  affect_to_char(ch,&af);  
  ch->doppel = victim;

}


void spell_assist( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
      {
	send_to_char("This power is used too recently.\n\r",ch);
	return;
      }

    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 50;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    // victim->hit += 100 + level * 5;
		victim->hit += (ch->spellpower * 10);
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    act("$n looks better.", victim, NULL, NULL, TO_ROOM);
    if (ch != victim) send_to_char("Ok.\n\r",ch);
    return;
} 


void spell_desert_fist( int sn, int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( (ch->in_room->sector_type != SECT_HILLS)
	&& (ch->in_room->sector_type != SECT_MOUNTAIN)
	&& (ch->in_room->sector_type != SECT_DESERT) )
	{
	 send_to_char("You don't find any sand here to create a fist.\n\r",ch);
	 ch->wait = 0;
	 return;
	}

    act("An existing parcel of sand rises up and forms a fist and pummels $n.",
		victim,NULL,NULL,TO_ROOM);
    act("An existing parcel of sand rises up and forms a fist and pummels you.",
		victim,NULL,NULL,TO_CHAR);
    dam = dice( level , 14 );
    dam *= (ch->spellpower * 4);
    damage(ch,victim,dam,sn,DAM_OTHER,TRUE);
    sand_effect(victim,level,dam,TARGET_CHAR);
}


/* travel via astral plains */
void spell_astral_walk( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    bool gate_pet;
    char buf[512];


    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   victim->level >= level + 3
/*    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  * NOT trust */
    ||   saves_spell(level,victim,DAM_OTHER)
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
    if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
	gate_pet = TRUE;
    else
	gate_pet = FALSE;


    act("$n disappears in a flash of light!",ch,NULL,NULL,TO_ROOM);
    sprintf(buf,"You travel via astral planes and go to %s.\n\r",victim->name);
    send_to_char(buf,ch);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("$n appears in a flash of light!",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

    if (gate_pet)
    {
	act("$n disappears in a flash of light!",ch->pet,NULL,NULL,TO_ROOM);
	send_to_char(buf,ch->pet);
	char_from_room(ch->pet);
	char_to_room(ch->pet,victim->in_room);
	act("$n appears in a flash of light!",ch->pet,NULL,NULL,TO_ROOM);
	do_look(ch->pet,"auto");
    }
}

void spell_protection_negative ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
  AFFECT_DATA af;

    if (!is_affected(ch, sn))
    {
      send_to_char("You are now immune to negative attacks.\n\r", ch);

      af.where = TO_IMMUNE;
      af.type = sn;
      af.duration = level / 4;
      af.level = ch->level;
      af.bitvector = IMM_NEGATIVE;
      af.location = 0;
      af.modifier = 0;
      affect_to_char(ch, &af);
    }
  else 
      send_to_char("You are already immune to negative attacks.\n\r", ch);
 return;
}

void spell_iceshield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
// MAGIC ( spell_iceshield )
{
    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( IS_SHIELDED ( victim, SHD_ICE ) )
    {
        if ( victim == ch )
            send_to_char
                ( "You are already surrounded by an {Cicy{x shield.\n\r",
                  ch );
        else
            act ( "$N is already surrounded by an {Cicy{x shield.", ch, NULL,
                  victim, TO_CHAR );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char ( "You failed.\n\r", ch );
        return;
    }

/*
    if ( (skill_table[sn].skill_level[victim->class] > LEVEL_ANCIENT)
	&& (victim->level < LEVEL_IMMORTAL) )
    {
	send_to_char("You are surrounded by an {Cicy{x shield.\n\r", victim);
	act("$n is surrounded by an {Cicy{x shield.",victim, NULL,NULL,TO_ROOM);
	send_to_char("Your {Cicy{x shield quickly melts away.\n\r", victim);
	act("$n's {Cicy{x shield quickly melts away.",victim, NULL,NULL,TO_ROOM);
	return;
    }
*/

    af.where = TO_SHIELDS;
    af.type = sn;
    af.level = level;
    af.duration = level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = SHD_ICE;

    affect_to_char ( victim, &af );
    send_to_char ( "You are surrounded by an {Cicy{x shield.\n\r", victim );
    act ( "$n is surrounded by an {Cicy{x shield.", victim, NULL, NULL,
          TO_ROOM );
    return;
}

void spell_fireshield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
//MAGIC ( spell_fireshield )
{

    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( IS_SHIELDED ( victim, SHD_FIRE ) )
    {
        if ( victim == ch )
            send_to_char
                ( "You are already surrounded by a {Rfirey{x shield.\r\n",
                  ch );
        else
            act ( "$N is already surrounded by a {Rfiery{x shield.", ch, NULL,
                  victim, TO_CHAR );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char ( "You failed.\n\r", ch );
        return;
    }

/*
    if ( (skill_table[sn].skill_level[victim->class] > LEVEL_ANCIENT)
	&& (victim->level < LEVEL_IMMORTAL) )
    {
	send_to_char("You are surrounded by a {Rfiery{x shield.\n\r", victim);
	act("$n is surrounded by a {Rfiery{x shield.",victim, NULL,NULL,TO_ROOM);
	send_to_char("Your {Rfirey{x shield gutters out.\n\r", victim);
	act("$n's {Rfirey{x shield gutters out.",victim, NULL,NULL,TO_ROOM);
	return;
    }
*/

    af.where = TO_SHIELDS;
    af.type = sn;
    af.level = level;
    af.duration = level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = SHD_FIRE;

    affect_to_char ( victim, &af );
    send_to_char ( "You are surrounded by a {Rfiery{x shield.\n\r", victim );
    act ( "$n is surrounded by a {Rfiery{x shield.", victim, NULL, NULL,
          TO_ROOM );
    return;

}
void spell_shockshield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
// MAGIC ( spell_shockshield )
{

    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( IS_SHIELDED ( victim, SHD_SHOCK ) )
    {
        if ( victim == ch )
            send_to_char
                ( "You are already surrounded in a {Bcrackling{x shield.\n\r",
                  ch );
        else
            act ( "$N is already surrounded by a {Bcrackling{x shield.", ch,
                  NULL, victim, TO_CHAR );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char ( "You failed.\n\r", ch );
        return;
    }

/*
    if ( (skill_table[sn].skill_level[victim->class] > LEVEL_ANCIENT)
	&& (victim->level < LEVEL_IMMORTAL) )
    {
	send_to_char("You are surrounded by a {Bcrackling{x shield.\n\r", victim);
	act("$n is surrounded by a {Bcrackling{x shield.",victim, NULL,NULL,TO_ROOM);
	send_to_char("Your {Bcrackling{x shield sizzles and fades.\n\r", victim);
	act("$n's {Bcrackling{x shield sizzles and fades.",victim, 
NULL,NULL,TO_ROOM);
	return;
    }
*/

    af.where = TO_SHIELDS;
    af.type = sn;
    af.level = level;
    af.duration = level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = SHD_SHOCK;

    affect_to_char ( victim, &af );
    send_to_char ( "You are surrounded by a {Bcrackling{x field.\n\r",
                   victim );
    act ( "$n is surrounded by a {Bcrackling{x shield.", victim, NULL, NULL,
          TO_ROOM );
    return;
}

void spell_acidshield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
// MAGIC ( spell_acidshield )
{

    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( IS_SHIELDED ( victim, SHD_ACID ) )
    {
        if ( victim == ch )
            send_to_char
                ( "You are already surrounded in an {GA{gcidic{x shield.\n\r",
                  ch );
        else
            act ( "$N is already surrounded by an {GA{gcidic{x shield.", ch,
                  NULL, victim, TO_CHAR );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char ( "You failed.\n\r", ch );
        return;
    }

    af.where = TO_SHIELDS;
    af.type = sn;
    af.level = level;
    af.duration = level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = SHD_ACID;

    affect_to_char ( victim, &af );
    send_to_char ( "You are surrounded by an {GA{gcidic{x field.\n\r",
                   victim );
    act ( "$n is surrounded by an {GA{gcidic{x shield.", victim, NULL, NULL,
          TO_ROOM );
    return;
}

void spell_poisonshield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
// MAGIC ( spell_poisonshield )
{

    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( IS_SHIELDED ( victim, SHD_POISON ) )
    {
        if ( victim == ch )
            send_to_char
                ( "You are already surrounded in a {MP{moisonous{x {CC{cl{Co{cu{Cd{x.\n\r",
                  ch );
        else
            act ( "$N is already surrounded by a {MP{moisonous{x {CC{cl{Co{cu{Cd{x.", ch, NULL, victim, TO_CHAR );
        return;
    }
    if ( IS_NPC ( victim ) )
    {
        send_to_char ( "You failed.\n\r", ch );
        return;
    }

    af.where = TO_SHIELDS;
    af.type = sn;
    af.level = level;
    af.duration = level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = SHD_POISON;

    affect_to_char ( victim, &af );
    send_to_char
        ( "You are surrounded by a {MP{moisonous{x {CC{cl{Co{cu{Cd{x.\n\r",
          victim );
    act ( "$n is surrounded by a {MP{moisonous{x {CC{cl{Co{cu{Cd{x.", victim,
          NULL, NULL, TO_ROOM );
    return;
}

void spell_briarshield( int sn, int level, CHAR_DATA *ch, void *vo, int target)
// MAGIC ( spell_briarshield )
{

    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( IS_SHIELDED ( victim, SHD_BRIAR ) )
    {
        if ( victim == ch )
            send_to_char
                ( "You are already surrounded in a thorny shield.\n\r", ch );
        else
            act ( "$N is already surrounded by a thorny shield.", ch, NULL,
                  victim, TO_CHAR );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char ( "You failed.\n\r", ch );
        return;
    }

/*
    if ( (skill_table[sn].skill_level[victim->class] > LEVEL_ANCIENT)
	&& (victim->level < LEVEL_IMMORTAL) )
    {
	send_to_char("You are surrounded by a {Bcrackling{x shield.\n\r", victim);
	act("$n is surrounded by a {Bcrackling{x shield.",victim, NULL,NULL,TO_ROOM);
	send_to_char("Your {Bcrackling{x shield sizzles and fades.\n\r", victim);
	act("$n's {Bcrackling{x shield sizzles and fades.",victim, 
NULL,NULL,TO_ROOM);
	return;
    }
*/

    af.where = TO_SHIELDS;
    af.type = sn;
    af.level = level;
    af.duration = level;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = SHD_BRIAR;

    affect_to_char ( victim, &af );
    send_to_char ( "You are surrounded by a thorny shield.\n\r", victim );
    act ( "$n is surrounded by a thorny shield.", victim, NULL, NULL,
          TO_ROOM );
    return;
}

/*original writer unkown, out of paradox codebase RoT1.4/1.5 diriv*/
void spell_mana_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target)
// MAGIC ( spell_mana_shield )
{

    CHAR_DATA *victim = ( CHAR_DATA * ) vo;
    AFFECT_DATA af;

    if ( IS_SHIELDED ( victim, SHD_MANA ) )
    {
        if ( victim == ch )
            send_to_char ( "You are already surrounded by a mana shield.\n\r",
                           ch );
        else
            act ( "$N is already surrounded by a mana shield.", ch, NULL,
                  victim, TO_CHAR );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char ( "You failed.\n\r", ch );
        return;
    }

    af.where = TO_SHIELDS;
    af.type = sn;
    af.level = level;
    af.duration = level / 22;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = SHD_MANA;

    affect_to_char ( victim, &af );
    send_to_char ( "You are surrounded by a mana shield.\n\r", victim );
    act ( "$n is surrounded by a mana shield.", victim, NULL, NULL, TO_ROOM );
    return;
}

void spell_ward( int sn, int level, CHAR_DATA *ch, void *vo, int target)
// MAGIC ( spell_ward )
{
    AFFECT_DATA af;

    if ( is_affected ( ch, sn ) )
    {
        send_to_char ( "You are already in a ward", ch );
        return;
    }

    af.where = TO_SHIELDS;
    af.type = sn;
    af.level = level;
    af.duration = level / 5;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = SHD_WARD;
    affect_to_char ( ch, &af );
    act ( "$n is warded.", ch, NULL, NULL, TO_ROOM );
    send_to_char ( "You are warded.\n\r", ch );
    return;
}
