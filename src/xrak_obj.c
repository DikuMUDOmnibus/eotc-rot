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
#include "olc.h"


void do_thirdeye(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    char      buf [MAX_STRING_LENGTH];
	if (IS_NPC(ch)) return;
	
      if (ch->level < 202)
      {
	send_to_char("This is Champion and Higher Only.\n\r", ch);
	return;
	}
	send_to_char("{CYou experience the world through your third eye.{x\n\r", ch);
	for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected != CON_PLAYING) continue;
        if ((victim = d->character) == NULL) continue;
        if (IS_NPC(victim) || victim->in_room == NULL) continue;
        if (victim == ch) continue;
        if (!can_see(ch,victim)) continue;
        if (victim->level > 6) continue;
        sprintf(buf,"{x%-14s - %s\n\r", victim->name, victim->in_room->name);
        send_to_char(buf,ch);
   }
}

void do_call(CHAR_DATA * ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  OBJ_DATA *in_obj;
  CHAR_DATA *gch;
  CHAR_DATA *victim = NULL;
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *objroom;

  one_argument(argument, arg);

  if (arg[0] == '\0')
  {
    send_to_char("What object do you wish to call?\n\r", ch);
    return;
  }
  if (IS_NPC(ch))
    return;

    act("Your eyes flicker with yellow energy.", ch, NULL, NULL, TO_CHAR);
    act("$n's eyes flicker with yellow energy.", ch, NULL, NULL, TO_ROOM);

if (!str_cmp(arg, "all"))
  {
    call_all(ch);
    return;
  }
  if ((obj = get_obj_world(ch, arg)) == NULL)
  {
    send_to_char("Nothing like that in hell, earth, or heaven.\n\r", ch);
    return;
  }
  if (obj->ownerid == 0 || obj->ownerid != ch->pcdata->playerid)
  {
    send_to_char("Nothing happens.\n\r", ch);
    return;
  }
  if (IS_OBJ_STAT(obj, ITEM_NOLOCATE))
  {
    send_to_char("Nothing happens.\n\r", ch);
    return;
  }
  for (in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj);

  if (in_obj->carried_by != NULL)
  {
    if (in_obj->carried_by == ch)
      return;
    if ((gch = in_obj->carried_by) != NULL)
    {
      if (gch->desc && gch->desc->connected != CON_PLAYING)
        return;
    }
  }
  if (obj->carried_by != NULL && obj->carried_by != ch)
  {
    victim = obj->carried_by;
    if (!IS_NPC(victim) && victim->desc != NULL && victim->desc->connected != CON_PLAYING)
      return;
    act("$p suddenly vanishes from your hands!", victim, obj, NULL, TO_CHAR);
    act("$p suddenly vanishes from $n's hands!", victim, obj, NULL, TO_ROOM);
    obj_from_char(obj);
  }
  else if (obj->in_room != NULL)
  {
    chroom = ch->in_room;
    objroom = obj->in_room;
    char_from_room(ch);
    char_to_room(ch, objroom);
    act("$p vanishes from the ground!", ch, obj, NULL, TO_ROOM);
    if (chroom == objroom)
      act("$p vanishes from the ground!", ch, obj, NULL, TO_CHAR);
    char_from_room(ch);
    char_to_room(ch, chroom);
    obj_from_room(obj);
  }
  else if (obj->in_obj != NULL)
    obj_from_obj(obj);
  else
  {
      send_to_char("Nothing happens.\n\r", ch);
    return;
  }
  obj_to_char(obj, ch);
  act("$p materializes in your hands.", ch, obj, NULL, TO_CHAR);
  act("$p materializes in $n's hands.", ch, obj, NULL, TO_ROOM);
  return;
}



void call_all(CHAR_DATA * ch)
{
  CHAR_DATA *gch;
  OBJ_DATA *obj;
  OBJ_DATA *in_obj;
  CHAR_DATA *victim = NULL;
  DESCRIPTOR_DATA *d;
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *objroom;
  bool found = FALSE;

  for (obj = object_list; obj != NULL; obj = obj->next)
  {
    if (obj->ownerid == 0 || obj->ownerid != ch->pcdata->playerid)
      continue;
    if (IS_OBJ_STAT(obj, ITEM_NOLOCATE))
      continue;

    found = TRUE;

    for (in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj);

    if (in_obj->carried_by != NULL)
    {
      if (in_obj->carried_by == ch)
        continue;
      if ((gch = in_obj->carried_by) != NULL)
      {
        if (gch->desc && gch->desc->connected != CON_PLAYING)
          continue;
      }
    }
    if (obj->carried_by != NULL && obj->carried_by != ch)
    {
      if (obj->carried_by == ch || obj->carried_by->desc == NULL || obj->carried_by->desc->connected != CON_PLAYING)
      {
        if (!IS_NPC(obj->carried_by))
          return;
      }
      act("$p suddenly vanishes from your hands!", obj->carried_by, obj, NULL, TO_CHAR);
      act("$p suddenly vanishes from $n's hands!", obj->carried_by, obj, NULL, TO_ROOM);
      obj_from_char(obj);
    }
    else if (obj->in_room != NULL)
    {
      chroom = ch->in_room;
      objroom = obj->in_room;
      char_from_room(ch);
      char_to_room(ch, objroom);
      act("$p vanishes from the ground!", ch, obj, NULL, TO_ROOM);
      if (chroom == objroom)
        act("$p vanishes from the ground!", ch, obj, NULL, TO_CHAR);
      char_from_room(ch);
      char_to_room(ch, chroom);
      obj_from_room(obj);
    }
    else if (obj->in_obj != NULL)
      obj_from_obj(obj);
    else
      continue;
    obj_to_char(obj, ch);

      act("$p materializes in your hands.", ch, obj, NULL, TO_CHAR);
      act("$p materializes in $n's hands.", ch, obj, NULL, TO_ROOM);
  }
  if (!found)
    send_to_char("Nothing happens.\n\r", ch);

  for (d = descriptor_list; d != NULL; d = d->next)
  {
    if (d->connected != CON_PLAYING)
      continue;
    if ((victim = d->character) == NULL)
      continue;
    if (IS_NPC(victim))
      continue;
  }
  return;
}
