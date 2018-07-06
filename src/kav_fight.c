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


void take_item(CHAR_DATA * ch, OBJ_DATA * obj)
{
        if (obj == NULL)
                return;
        obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        act("You wince in pain and $p falls to the ground.", ch, obj, NULL,
            TO_CHAR);
        act("$n winces in pain and $p falls to the ground.", ch, obj, NULL,
            TO_ROOM);
        return;
}


void make_part(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        int       vnum;

        argument = one_argument(argument, arg);
        vnum = 0;

        if (arg[0] == '\0')
                return;
        if (ch->in_room == NULL)
                return;

        if (!str_cmp(arg, "head"))
                vnum = OBJ_VNUM_SEVERED_HEAD;
        else if (!str_cmp(arg, "arm"))
                vnum = OBJ_VNUM_SLICED_ARM;
        else if (!str_cmp(arg, "leg"))
                vnum = OBJ_VNUM_SLICED_LEG;
        else if (!str_cmp(arg, "heart"))
                vnum = OBJ_VNUM_TORN_HEART;
        else if (!str_cmp(arg, "turd"))
                vnum = OBJ_VNUM_TORN_HEART;
        else if (!str_cmp(arg, "entrails"))
                vnum = OBJ_VNUM_SPILLED_ENTRAILS;
        else if (!str_cmp(arg, "brain"))
                vnum = OBJ_VNUM_QUIVERING_BRAIN;
        else if (!str_cmp(arg, "eyeball"))
                vnum = OBJ_VNUM_SQUIDGY_EYEBALL;
        else if (!str_cmp(arg, "blood"))
                vnum = OBJ_VNUM_SPILT_BLOOD;
        else if (!str_cmp(arg, "face"))
                vnum = OBJ_VNUM_RIPPED_FACE;
        else if (!str_cmp(arg, "windpipe"))
                vnum = OBJ_VNUM_TORN_WINDPIPE;
        else if (!str_cmp(arg, "cracked_head"))
                vnum = OBJ_VNUM_CRACKED_HEAD;
        else if (!str_cmp(arg, "ear"))
                vnum = OBJ_VNUM_SLICED_EAR;
        else if (!str_cmp(arg, "nose"))
                vnum = OBJ_VNUM_SLICED_NOSE;
        else if (!str_cmp(arg, "tooth"))
                vnum = OBJ_VNUM_KNOCKED_TOOTH;
        else if (!str_cmp(arg, "tongue"))
                vnum = OBJ_VNUM_TORN_TONGUE;
        else if (!str_cmp(arg, "hand"))
                vnum = OBJ_VNUM_SEVERED_HAND;
        else if (!str_cmp(arg, "foot"))
                vnum = OBJ_VNUM_SEVERED_FOOT;
        else if (!str_cmp(arg, "thumb"))
                vnum = OBJ_VNUM_SEVERED_THUMB;
        else if (!str_cmp(arg, "index"))
                vnum = OBJ_VNUM_SEVERED_INDEX;
        else if (!str_cmp(arg, "middle"))
                vnum = OBJ_VNUM_SEVERED_MIDDLE;
        else if (!str_cmp(arg, "ring"))
                vnum = OBJ_VNUM_SEVERED_RING;
        else if (!str_cmp(arg, "little"))
                vnum = OBJ_VNUM_SEVERED_LITTLE;
        else if (!str_cmp(arg, "toe"))
                vnum = OBJ_VNUM_SEVERED_TOE;

        if (vnum != 0)
        {
                char      buf[MAX_STRING_LENGTH];
                OBJ_DATA *obj;
                char     *name;

                name = IS_NPC(ch) ? ch->short_descr : ch->name;
                obj = create_object(get_obj_index(vnum), 0);
                obj->timer = number_range(4, 7);
                if (!str_cmp(arg, "head") && IS_NPC(ch))
                        obj->value[1] = ch->pIndexData->vnum;
                else if (!str_cmp(arg, "head") && !IS_NPC(ch))
                {
//                        ch->pcdata->chobj = obj;
//                        obj->chobj = ch;
                        obj->timer = number_range(1, 2);
                        obj->item_type = ITEM_HEAD;
                }
                else if (!str_cmp(arg, "brain") && !IS_NPC(ch)
                         && IS_AFFECTED(ch, AFF_POLYMORPH)
                         && IS_HEAD(ch, LOST_HEAD))
                {
//                        if (ch->pcdata->chobj != NULL)
//                                ch->pcdata->chobj->chobj = NULL;
//                        ch->pcdata->chobj = obj;
//                        obj->chobj = ch;
                        obj->timer = number_range(1, 2);
                        obj->item_type = ITEM_HEAD;
                }
                if (vnum == OBJ_VNUM_SPILT_BLOOD)
                        obj->timer = 2;
                if (!IS_NPC(ch))
                {
                        sprintf(buf, obj->name, name);
                        free_string(obj->name);
                        obj->name = str_dup(buf);
                }
                else
                {
                        sprintf(buf, obj->name, "mob");
                        free_string(obj->name);
                        obj->name = str_dup(buf);
                }
                sprintf(buf, obj->short_descr, name);
                free_string(obj->short_descr);
                obj->short_descr = str_dup(buf);
                sprintf(buf, obj->description, name);
                free_string(obj->description);
                obj->description = str_dup(buf);
                obj_to_room(obj, ch->in_room);
        }
        return;
}

void critical_hit(CHAR_DATA * ch, CHAR_DATA * victim, int dt, int dam)
{
//        const char *attack;
        OBJ_DATA *obj;
        OBJ_DATA *damaged;
        char      buf[MAX_STRING_LENGTH];
        char      buf2[20];
//        int       dtype;
        int       critical = 0;
//        int       wpn = 0;
//        int       wpn2 = 0;
        int       count;
        int       count2;

        if (IS_NPC(ch))
                critical += ((ch->level + 1) / 5);
        if (!IS_NPC(ch))
                critical += 15;
//        if (IS_NPC(victim))
//                critical -= ((victim->level + 1) / 250);
/*	if (!IS_NPC(victim))
        {
                obj = get_eq_char(victim, WEAR_WIELD);
                dtype = TYPE_HIT;
                if (obj != NULL && obj->item_type == ITEM_WEAPON)
                        dtype += obj->value[3];
                obj = get_eq_char(victim, WEAR_HOLD);
                dtype = TYPE_HIT;
                if (obj != NULL && obj->item_type == ITEM_WEAPON)
                        dtype += obj->value[3];

        }
*/
        if (critical < 1)
                critical = 1;
        else if (IS_NPC(ch) && critical > 45)
                critical = 45;

        if (number_percent() > critical)
                return;
        critical = number_range(1, 23);
        if (critical == 1)
        {
                if (IS_HEAD(victim, LOST_EYE_L)
                    && IS_HEAD(victim, LOST_EYE_R))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_EYES)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevents you from loosing an eye.", victim,
                            damaged, NULL, TO_CHAR);
                        act("$p prevents $n from loosing an eye.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;


                if (!IS_HEAD(victim, LOST_EYE_L) && number_percent() < 50)
                        SET_BIT(victim->loc_hp[0], LOST_EYE_L);
                else if (!IS_HEAD(victim, LOST_EYE_R))
                        SET_BIT(victim->loc_hp[0], LOST_EYE_R);
                else if (!IS_HEAD(victim, LOST_EYE_L))
                        SET_BIT(victim->loc_hp[0], LOST_EYE_L);
                else
                        return;
                act("Your skillful blow takes out $N's eye!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow takes out $N's eye!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow takes out your eye!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "eyeball");
                return;
        }
        else if (critical == 2)
        {
                if (IS_HEAD(victim, LOST_EAR_L)
                    && IS_HEAD(victim, LOST_EAR_R))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_HEAD)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevents you from loosing an ear.", victim,
                            damaged, NULL, TO_CHAR);
                        act("$p prevents $n from loosing an ear.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_HEAD(victim, LOST_EAR_L) && number_percent() < 50)
                        SET_BIT(victim->loc_hp[0], LOST_EAR_L);
                else if (!IS_HEAD(victim, LOST_EAR_R))
                        SET_BIT(victim->loc_hp[0], LOST_EAR_R);
                else if (!IS_HEAD(victim, LOST_EAR_L))
                        SET_BIT(victim->loc_hp[0], LOST_EAR_L);
                else
                        return;
                act("Your skillful blow cuts off $N's ear!", ch, NULL, victim,
                    TO_CHAR);
                act("$n's skillful blow cuts off $N's ear!", ch, NULL, victim,
                    TO_NOTVICT);
                act("$n's skillful blow cuts off your ear!", ch, NULL, victim,
                    TO_VICT);
                make_part(victim, "ear");
                return;
        }
        else if (critical == 3)
        {
                if (IS_HEAD(victim, LOST_NOSE))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_EYES)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevents you from loosing your nose.", victim,
                            damaged, NULL, TO_CHAR);
                        act("$p prevents $n from loosing $s nose.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                SET_BIT(victim->loc_hp[0], LOST_NOSE);
                act("Your skillful blow cuts off $N's nose!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's nose!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your nose!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "nose");
                return;
        }
        else if (critical == 4)
        {
                if (IS_HEAD(victim, LOST_NOSE)
                    || IS_HEAD(victim, BROKEN_NOSE))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_EYES)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevents you from breaking your nose.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevents $n from breaking $s nose.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_HEAD(victim, LOST_NOSE)
                    && !IS_HEAD(victim, BROKEN_NOSE))
                        SET_BIT(victim->loc_hp[0], BROKEN_NOSE);
                else
                        return;
                act("Your skillful blow breaks $N's nose!", ch, NULL, victim,
                    TO_CHAR);
                act("$n's skillful blow breaks $N's nose!", ch, NULL, victim,
                    TO_NOTVICT);
                act("$n's skillful blow breaks your nose!", ch, NULL, victim,
                    TO_VICT);
                return;
        }
        else if (critical == 5)
        {
                if (IS_HEAD(victim, BROKEN_JAW))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_EYES)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevents you from breaking your jaw.", victim,
                            damaged, NULL, TO_CHAR);
                        act("$p prevents $n from breaking $s jaw.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p falls broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_HEAD(victim, BROKEN_JAW))
                        SET_BIT(victim->loc_hp[0], BROKEN_JAW);
                else
                        return;
                act("Your skillful blow breaks $N's jaw!", ch, NULL, victim,
                    TO_CHAR);
                act("$n's skillful blow breaks $N's jaw!", ch, NULL, victim,
                    TO_NOTVICT);
                act("$n's skillful blow breaks your jaw!", ch, NULL, victim,
                    TO_VICT);
                return;
        }
        else if (critical == 6)
        {
                if (IS_ARM_L(victim, LOST_ARM))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_ARMS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing your left arm.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing $s left arm.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }

                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;
                if (!IS_ARM_L(victim, LOST_ARM))
                        SET_BIT(victim->loc_hp[2], LOST_ARM);
                else
                        return;
                if (!IS_BLEEDING(victim, BLEEDING_ARM_L))
                        SET_BIT(victim->loc_hp[6], BLEEDING_ARM_L);
                if (IS_BLEEDING(victim, BLEEDING_HAND_L))
                        REMOVE_BIT(victim->loc_hp[6], BLEEDING_HAND_L);
                act("Your skillful blow cuts off $N's left arm!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's left arm!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your left arm!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "arm");
                if (IS_ARM_L(victim, LOST_ARM) && IS_ARM_R(victim, LOST_ARM))
                {
                        if ((obj = get_eq_char(victim, WEAR_ARMS)) != NULL)
                                take_item(victim, obj);
                }
                if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_HANDS)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_WRIST_L)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_FINGER_L)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 7)
        {
                if (IS_ARM_R(victim, LOST_ARM))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_ARMS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing your right arm.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing $s right arm.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }

                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_ARM_R(victim, LOST_ARM))
                        SET_BIT(victim->loc_hp[3], LOST_ARM);
                else
                        return;
                if (!IS_BLEEDING(victim, BLEEDING_ARM_R))
                        SET_BIT(victim->loc_hp[6], BLEEDING_ARM_R);
                if (IS_BLEEDING(victim, BLEEDING_HAND_R))
                        REMOVE_BIT(victim->loc_hp[6], BLEEDING_HAND_R);
                act("Your skillful blow cuts off $N's right arm!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's right arm!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your right arm!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "arm");
                if (IS_ARM_L(victim, LOST_ARM) && IS_ARM_R(victim, LOST_ARM))
                {
                        if ((obj = get_eq_char(victim, WEAR_ARMS)) != NULL)
                                take_item(victim, obj);
                }
                if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_HANDS)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_WRIST_R)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_FINGER_R)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 8)
        {
                if (IS_ARM_L(victim, LOST_ARM)
                    || IS_ARM_L(victim, BROKEN_ARM))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_ARMS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from breaking your left arm.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from breaking $s left arm.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }

                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_ARM_L(victim, BROKEN_ARM)
                    && !IS_ARM_L(victim, LOST_ARM))
                        SET_BIT(victim->loc_hp[2], BROKEN_ARM);
                else
                        return;
                act("Your skillful blow breaks $N's left arm!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow breaks $N's left arm!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow breaks your left arm!", ch, NULL,
                    victim, TO_VICT);
                if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 9)
        {
                if (IS_ARM_R(victim, LOST_ARM)
                    || IS_ARM_R(victim, BROKEN_ARM))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_ARMS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from breaking your right arm.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from breaking $s right arm.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }

                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_ARM_R(victim, BROKEN_ARM)
                    && !IS_ARM_R(victim, LOST_ARM))
                        SET_BIT(victim->loc_hp[3], BROKEN_ARM);
                else
                        return;
                act("Your skillful blow breaks $N's right arm!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow breaks $N's right arm!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow breaks your right arm!", ch, NULL,
                    victim, TO_VICT);
                if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 10)
        {
                if (IS_ARM_L(victim, LOST_HAND) || IS_ARM_L(victim, LOST_ARM))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_HANDS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing your left hand.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing $s left hand.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }

                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_ARM_L(victim, LOST_HAND)
                    && !IS_ARM_L(victim, LOST_ARM))
                        SET_BIT(victim->loc_hp[2], LOST_HAND);
                else
                        return;
                if (IS_BLEEDING(victim, BLEEDING_ARM_L))
                        REMOVE_BIT(victim->loc_hp[6], BLEEDING_ARM_L);
                if (!IS_BLEEDING(victim, BLEEDING_HAND_L))
                        SET_BIT(victim->loc_hp[6], BLEEDING_HAND_L);
                act("Your skillful blow cuts off $N's left hand!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's left hand!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your left hand!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "hand");
                if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_HANDS)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_WRIST_L)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_FINGER_L)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 11)
        {
                if (IS_ARM_R(victim, LOST_HAND) || IS_ARM_R(victim, LOST_ARM))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_HANDS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing your right hand.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing $s right hand.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }

                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_ARM_R(victim, LOST_HAND)
                    && !IS_ARM_R(victim, LOST_ARM))
                        SET_BIT(victim->loc_hp[3], LOST_HAND);
                else
                        return;
                if (IS_BLEEDING(victim, BLEEDING_ARM_R))
                        REMOVE_BIT(victim->loc_hp[6], BLEEDING_ARM_R);
                if (!IS_BLEEDING(victim, BLEEDING_HAND_R))
                        SET_BIT(victim->loc_hp[6], BLEEDING_HAND_R);
                act("Your skillful blow cuts off $N's right hand!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's right hand!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your right hand!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "hand");

                if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_HANDS)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_WRIST_R)) != NULL)
                        take_item(victim, obj);
                if ((obj = get_eq_char(victim, WEAR_FINGER_R)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 12)
        {
                if (IS_ARM_L(victim, LOST_ARM))
                        return;
                if (IS_ARM_L(victim, LOST_HAND))
                        return;
                if (IS_ARM_L(victim, LOST_THUMB)
                    && IS_ARM_L(victim, LOST_FINGER_I)
                    && IS_ARM_L(victim, LOST_FINGER_M)
                    && IS_ARM_L(victim, LOST_FINGER_R)
                    && IS_ARM_L(victim, LOST_FINGER_L))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_HANDS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing some fingers from your left hand.", victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing some fingers from $s left hand.", victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                count = 0;
                count2 = 0;
                if (!IS_ARM_L(victim, LOST_THUMB) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], LOST_THUMB);
                        count2 += 1;
                        make_part(victim, "thumb");
                }
                if (!IS_ARM_L(victim, LOST_FINGER_I) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], LOST_FINGER_I);
                        count += 1;
                        make_part(victim, "index");
                }
                if (!IS_ARM_L(victim, LOST_FINGER_M) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], LOST_FINGER_M);
                        count += 1;
                        make_part(victim, "middle");
                }
                if (!IS_ARM_L(victim, LOST_FINGER_R) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], LOST_FINGER_R);
                        count += 1;
                        make_part(victim, "ring");
                        if ((obj =
                             get_eq_char(victim, WEAR_FINGER_L)) != NULL)
                                take_item(victim, obj);
                }
                if (!IS_ARM_L(victim, LOST_FINGER_L) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], LOST_FINGER_L);
                        count += 1;
                        make_part(victim, "little");
                }
                if (count == 1)
                        sprintf(buf2, "finger");
                else
                        sprintf(buf2, "fingers");
                if (count > 0 && count2 > 0)
                {
                        sprintf(buf,
                                "Your skillful blow cuts off %d %s and the thumb from $N's left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow cuts off %d %s and the thumb from $N's left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow cuts off %d %s and the thumb from your left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                else if (count > 0)
                {
                        sprintf(buf,
                                "Your skillful blow cuts off %d %s from $N's left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow cuts off %d %s from $N's left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow cuts off %d %s from your left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                else if (count2 > 0)
                {
                        sprintf(buf,
                                "Your skillful blow cuts off the thumb from $N's left hand.");
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow cuts off the thumb from $N's left hand.");
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow cuts off the thumb from your left hand.");
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                return;
        }
        else if (critical == 13)
        {
                if (IS_ARM_L(victim, LOST_ARM))
                        return;
                if (IS_ARM_L(victim, LOST_HAND))
                        return;
                if ((IS_ARM_L(victim, LOST_THUMB)
                     || IS_ARM_L(victim, BROKEN_THUMB))
                    && (IS_ARM_L(victim, LOST_FINGER_I)
                        || IS_ARM_L(victim, BROKEN_FINGER_I))
                    && (IS_ARM_L(victim, LOST_FINGER_M)
                        || IS_ARM_L(victim, BROKEN_FINGER_M))
                    && (IS_ARM_L(victim, LOST_FINGER_R)
                        || IS_ARM_L(victim, BROKEN_FINGER_R))
                    && (IS_ARM_L(victim, LOST_FINGER_L)
                        || IS_ARM_L(victim, BROKEN_FINGER_L)))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_HANDS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from breaking some fingers on your left hand.", victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from breaking some fingers on $s left hand.", victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                count = 0;
                count2 = 0;
                if (IS_ARM_L(victim, LOST_ARM))
                        return;
                if (IS_ARM_L(victim, LOST_HAND))
                        return;

                if (!IS_ARM_L(victim, BROKEN_THUMB)
                    && !IS_ARM_L(victim, LOST_THUMB) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], BROKEN_THUMB);
                        count2 += 1;
                }
                if (!IS_ARM_L(victim, BROKEN_FINGER_I)
                    && !IS_ARM_L(victim, LOST_FINGER_I)
                    && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], BROKEN_FINGER_I);
                        count += 1;
                }
                if (!IS_ARM_L(victim, BROKEN_FINGER_M)
                    && !IS_ARM_L(victim, LOST_FINGER_M)
                    && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], BROKEN_FINGER_M);
                        count += 1;
                }
                if (!IS_ARM_L(victim, BROKEN_FINGER_R)
                    && !IS_ARM_L(victim, LOST_FINGER_R)
                    && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], BROKEN_FINGER_R);
                        count += 1;
                }
                if (!IS_ARM_L(victim, BROKEN_FINGER_L)
                    && !IS_ARM_L(victim, LOST_FINGER_L)
                    && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[2], BROKEN_FINGER_L);
                        count += 1;
                }
                if (count == 1)
                        sprintf(buf2, "finger");
                else
                        sprintf(buf2, "fingers");
                if (count > 0 && count2 > 0)
                {
                        sprintf(buf,
                                "Your skillful breaks %d %s and the thumb on $N's left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow breaks %d %s and the thumb on $N's left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow breaks %d %s and the thumb on your left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                else if (count > 0)
                {
                        sprintf(buf,
                                "Your skillful blow breaks %d %s on $N's left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow breaks %d %s on $N's left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow breaks %d %s on your left hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                else if (count2 > 0)
                {
                        sprintf(buf,
                                "Your skillful blow breaks the thumb on $N's left hand.");
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow breaks the thumb on $N's left hand.");
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow breaks the thumb on your left hand.");
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_HOLD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                return;
        }
        else if (critical == 14)
        {
                if (IS_ARM_R(victim, LOST_ARM))
                        return;
                if (IS_ARM_R(victim, LOST_HAND))
                        return;
                if (IS_ARM_R(victim, LOST_THUMB)
                    && IS_ARM_R(victim, LOST_FINGER_I)
                    && IS_ARM_R(victim, LOST_FINGER_M)
                    && IS_ARM_R(victim, LOST_FINGER_R)
                    && IS_ARM_R(victim, LOST_FINGER_L))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_HANDS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing some fingers from your right hand.", victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing some fingers from $s right hand.", victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;


                count = 0;
                count2 = 0;
                if (IS_ARM_R(victim, LOST_ARM))
                        return;
                if (IS_ARM_R(victim, LOST_HAND))
                        return;

                if (!IS_ARM_R(victim, LOST_THUMB) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], LOST_THUMB);
                        count2 += 1;
                        make_part(victim, "thumb");
                }
                if (!IS_ARM_R(victim, LOST_FINGER_I) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], LOST_FINGER_I);
                        count += 1;
                        make_part(victim, "index");
                }
                if (!IS_ARM_R(victim, LOST_FINGER_M) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], LOST_FINGER_M);
                        count += 1;
                        make_part(victim, "middle");
                }
                if (!IS_ARM_R(victim, LOST_FINGER_R) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], LOST_FINGER_R);
                        count += 1;
                        make_part(victim, "ring");
                        if ((obj =
                             get_eq_char(victim, WEAR_FINGER_R)) != NULL)
                                take_item(victim, obj);
                }
                if (!IS_ARM_R(victim, LOST_FINGER_L) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], LOST_FINGER_L);
                        count += 1;
                        make_part(victim, "little");
                }
                if (count == 1)
                        sprintf(buf2, "finger");
                else
                        sprintf(buf2, "fingers");
                if (count > 0 && count2 > 0)
                {
                        sprintf(buf,
                                "Your skillful blow cuts off %d %s and the thumb from $N's right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow cuts off %d %s and the thumb from $N's right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow cuts off %d %s and the thumb from your right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                else if (count > 0)
                {
                        sprintf(buf,
                                "Your skillful blow cuts off %d %s from $N's right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow cuts off %d %s from $N's right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow cuts off %d %s from your right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                else if (count2 > 0)
                {
                        sprintf(buf,
                                "Your skillful blow cuts off the thumb from $N's right hand.");
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow cuts off the thumb from $N's right hand.");
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow cuts off the thumb from your right hand.");
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                return;
        }
        else if (critical == 15)
        {
                if (IS_ARM_R(victim, LOST_ARM))
                        return;
                if (IS_ARM_R(victim, LOST_HAND))
                        return;
                if ((IS_ARM_R(victim, LOST_THUMB)
                     || IS_ARM_R(victim, BROKEN_THUMB))
                    && (IS_ARM_R(victim, LOST_FINGER_I)
                        || IS_ARM_R(victim, BROKEN_FINGER_I))
                    && (IS_ARM_R(victim, LOST_FINGER_M)
                        || IS_ARM_R(victim, BROKEN_FINGER_M))
                    && (IS_ARM_R(victim, LOST_FINGER_R)
                        || IS_ARM_R(victim, BROKEN_FINGER_R))
                    && (IS_ARM_R(victim, LOST_FINGER_L)
                        || IS_ARM_R(victim, BROKEN_FINGER_L)))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_HANDS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from breaking some fingers on your right hand.", victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from breaking some fingers on $s right hand.", victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;


                count = 0;
                count2 = 0;
                if (IS_ARM_R(victim, LOST_ARM))
                        return;
                if (IS_ARM_R(victim, LOST_HAND))
                        return;

                if (!IS_ARM_R(victim, BROKEN_THUMB)
                    && !IS_ARM_R(victim, LOST_THUMB) && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], BROKEN_THUMB);
                        count2 += 1;
                }
                if (!IS_ARM_R(victim, BROKEN_FINGER_I)
                    && !IS_ARM_R(victim, LOST_FINGER_I)
                    && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], BROKEN_FINGER_I);
                        count += 1;
                }
                if (!IS_ARM_R(victim, BROKEN_FINGER_M)
                    && !IS_ARM_R(victim, LOST_FINGER_M)
                    && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], BROKEN_FINGER_M);
                        count += 1;
                }
                if (!IS_ARM_R(victim, BROKEN_FINGER_R)
                    && !IS_ARM_R(victim, LOST_FINGER_R)
                    && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], BROKEN_FINGER_R);
                        count += 1;
                }
                if (!IS_ARM_R(victim, BROKEN_FINGER_L)
                    && !IS_ARM_R(victim, LOST_FINGER_L)
                    && number_percent() < 25)
                {
                        SET_BIT(victim->loc_hp[3], BROKEN_FINGER_L);
                        count += 1;
                }
                if (count == 1)
                        sprintf(buf2, "finger");
                else
                        sprintf(buf2, "fingers");
                if (count > 0 && count2 > 0)
                {
                        sprintf(buf,
                                "Your skillful breaks %d %s and the thumb on $N's right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow breaks %d %s and the thumb on $N's right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow breaks %d %s and the thumb on your right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                else if (count > 0)
                {
                        sprintf(buf,
                                "Your skillful blow breaks %d %s on $N's right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow breaks %d %s on $N's right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow breaks %d %s on your right hand.",
                                count, buf2);
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                else if (count2 > 0)
                {
                        sprintf(buf,
                                "Your skillful blow breaks the thumb on $N's right hand.");
                        act(buf, ch, NULL, victim, TO_CHAR);
                        sprintf(buf,
                                "$n's skillful blow breaks the thumb on $N's right hand.");
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        sprintf(buf,
                                "$n's skillful blow breaks the thumb on your right hand.");
                        act(buf, ch, NULL, victim, TO_VICT);

                        if ((obj = get_eq_char(victim, WEAR_WIELD)) != NULL)
                                take_item(victim, obj);
                        return;
                }
                return;
        }
        else if (critical == 16)
        {
                if (IS_LEG_L(victim, LOST_LEG))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_LEGS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing your left leg.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing $s left leg.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_LEG_L(victim, LOST_LEG))
                        SET_BIT(victim->loc_hp[4], LOST_LEG);
                else
                        return;
                if (!IS_BLEEDING(victim, BLEEDING_LEG_L))
                        SET_BIT(victim->loc_hp[6], BLEEDING_LEG_L);
                if (IS_BLEEDING(victim, BLEEDING_FOOT_L))
                        REMOVE_BIT(victim->loc_hp[6], BLEEDING_FOOT_L);
                act("Your skillful blow cuts off $N's left leg!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's left leg!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your left leg!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "leg");
                if (IS_LEG_L(victim, LOST_LEG) && IS_LEG_R(victim, LOST_LEG))
                {
                        if ((obj = get_eq_char(victim, WEAR_LEGS)) != NULL)
                                take_item(victim, obj);
                }
                if ((obj = get_eq_char(victim, WEAR_FEET)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 17)
        {
                if (IS_LEG_R(victim, LOST_LEG))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_LEGS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing your right leg.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing $s right leg.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_LEG_R(victim, LOST_LEG))
                        SET_BIT(victim->loc_hp[5], LOST_LEG);
                else
                        return;
                if (!IS_BLEEDING(victim, BLEEDING_LEG_R))
                        SET_BIT(victim->loc_hp[6], BLEEDING_LEG_R);
                if (IS_BLEEDING(victim, BLEEDING_FOOT_R))
                        REMOVE_BIT(victim->loc_hp[6], BLEEDING_FOOT_R);
                act("Your skillful blow cuts off $N's right leg!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's right leg!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your right leg!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "leg");

                if (IS_LEG_L(victim, LOST_LEG) && IS_LEG_R(victim, LOST_LEG))
                {
                        if ((obj = get_eq_char(victim, WEAR_LEGS)) != NULL)
                                take_item(victim, obj);
                }
                if ((obj = get_eq_char(victim, WEAR_FEET)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 18)
        {
                if (IS_LEG_L(victim, BROKEN_LEG)
                    || IS_LEG_L(victim, LOST_LEG))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_LEGS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from breaking your left leg.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from breaking $s left leg.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_LEG_L(victim, BROKEN_LEG)
                    && !IS_LEG_L(victim, LOST_LEG))
                        SET_BIT(victim->loc_hp[4], BROKEN_LEG);
                else
                        return;
                act("Your skillful blow breaks $N's left leg!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow breaks $N's left leg!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow breaks your left leg!", ch, NULL,
                    victim, TO_VICT);
                return;
        }
        else if (critical == 19)
        {
                if (IS_LEG_R(victim, BROKEN_LEG)
                    || IS_LEG_R(victim, LOST_LEG))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_LEGS)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from breaking your right leg.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from breaking $s right leg.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_LEG_R(victim, BROKEN_LEG)
                    && !IS_LEG_R(victim, LOST_LEG))
                        SET_BIT(victim->loc_hp[5], BROKEN_LEG);
                else
                        return;
                act("Your skillful blow breaks $N's right leg!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow breaks $N's right leg!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow breaks your right leg!", ch, NULL,
                    victim, TO_VICT);
                return;
        }
        else if (critical == 20)
        {
                if (IS_LEG_L(victim, LOST_LEG) || IS_LEG_L(victim, LOST_FOOT))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_FEET)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing your left foot.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing $s left foot.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_LEG_L(victim, LOST_LEG)
                    && !IS_LEG_L(victim, LOST_FOOT))
                        SET_BIT(victim->loc_hp[4], LOST_FOOT);
                else
                        return;
                if (IS_BLEEDING(victim, BLEEDING_LEG_L))
                        REMOVE_BIT(victim->loc_hp[6], BLEEDING_LEG_L);
                if (!IS_BLEEDING(victim, BLEEDING_FOOT_L))
                        SET_BIT(victim->loc_hp[6], BLEEDING_FOOT_L);
                act("Your skillful blow cuts off $N's left foot!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's left foot!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your left foot!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "foot");
                if ((obj = get_eq_char(victim, WEAR_FEET)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 21)
        {
                if (IS_LEG_R(victim, LOST_LEG) || IS_LEG_R(victim, LOST_FOOT))
                        return;
                if ((damaged = get_eq_char(victim, WEAR_FEET)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevent you from loosing your right foot.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevent $n from loosing $s right foot.",
                            victim, damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (!IS_LEG_R(victim, LOST_LEG) && !IS_LEG_R(victim, LOST_FOOT))
                        SET_BIT(victim->loc_hp[5], LOST_FOOT);
                else
                        return;
                if (IS_BLEEDING(victim, BLEEDING_LEG_R))
                        REMOVE_BIT(victim->loc_hp[6], BLEEDING_LEG_R);
                if (!IS_BLEEDING(victim, BLEEDING_FOOT_R))
                        SET_BIT(victim->loc_hp[6], BLEEDING_FOOT_R);
                act("Your skillful blow cuts off $N's right foot!", ch, NULL,
                    victim, TO_CHAR);
                act("$n's skillful blow cuts off $N's right foot!", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n's skillful blow cuts off your right foot!", ch, NULL,
                    victim, TO_VICT);
                make_part(victim, "foot");
                if ((obj = get_eq_char(victim, WEAR_FEET)) != NULL)
                        take_item(victim, obj);
                return;
        }
        else if (critical == 22)
        {
                int       bodyloc = 0;
                int       broken = number_range(1, 3);

                if (IS_BODY(victim, BROKEN_RIBS_1))
                        bodyloc += 1;
                if (IS_BODY(victim, BROKEN_RIBS_2))
                        bodyloc += 2;
                if (IS_BODY(victim, BROKEN_RIBS_4))
                        bodyloc += 4;
                if (IS_BODY(victim, BROKEN_RIBS_8))
                        bodyloc += 8;
                if (IS_BODY(victim, BROKEN_RIBS_16))
                        bodyloc += 16;
                if (bodyloc >= 24)
                        return;

                if ((damaged = get_eq_char(victim, WEAR_BODY)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevents you from breaking some ribs.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevents $n from breaking some ribs.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;


                if (IS_BODY(victim, BROKEN_RIBS_1))
                        REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_1);
                if (IS_BODY(victim, BROKEN_RIBS_2))
                        REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_2);
                if (IS_BODY(victim, BROKEN_RIBS_4))
                        REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_4);
                if (IS_BODY(victim, BROKEN_RIBS_8))
                        REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_8);
                if (IS_BODY(victim, BROKEN_RIBS_16))
                        REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_16);
                if (bodyloc + broken > 24)
                        broken -= 1;
                if (bodyloc + broken > 24)
                        broken -= 1;
                bodyloc += broken;
                if (bodyloc >= 16)
                {
                        bodyloc -= 16;
                        SET_BIT(victim->loc_hp[1], BROKEN_RIBS_16);
                }
                if (bodyloc >= 8)
                {
                        bodyloc -= 8;
                        SET_BIT(victim->loc_hp[1], BROKEN_RIBS_8);
                }
                if (bodyloc >= 4)
                {
                        bodyloc -= 4;
                        SET_BIT(victim->loc_hp[1], BROKEN_RIBS_4);
                }
                if (bodyloc >= 2)
                {
                        bodyloc -= 2;
                        SET_BIT(victim->loc_hp[1], BROKEN_RIBS_2);
                }
                if (bodyloc >= 1)
                {
                        bodyloc -= 1;
                        SET_BIT(victim->loc_hp[1], BROKEN_RIBS_1);
                }
                sprintf(buf, "Your skillful blow breaks %d of $N's ribs!",
                        broken);
                act(buf, ch, NULL, victim, TO_CHAR);
                sprintf(buf, "$n's skillful blow breaks %d of $N's ribs!",
                        broken);
                act(buf, ch, NULL, victim, TO_NOTVICT);
                sprintf(buf, "$n's skillful blow breaks %d of your ribs!",
                        broken);
                act(buf, ch, NULL, victim, TO_VICT);
                return;
        }
        else if (critical == 23)
        {
                int       bodyloc = 0;
                int       broken = number_range(1, 3);

                if (IS_HEAD(victim, LOST_TOOTH_1))
                        bodyloc += 1;
                if (IS_HEAD(victim, LOST_TOOTH_2))
                        bodyloc += 2;
                if (IS_HEAD(victim, LOST_TOOTH_4))
                        bodyloc += 4;
                if (IS_HEAD(victim, LOST_TOOTH_8))
                        bodyloc += 8;
                if (IS_HEAD(victim, LOST_TOOTH_16))
                        bodyloc += 16;
                if (bodyloc >= 28)
                        return;

                if ((damaged = get_eq_char(victim, WEAR_EYES)) != NULL &&
                    damaged->toughness > 0)
                {
                        act("$p prevents you from loosing some teeth.",
                            victim, damaged, NULL, TO_CHAR);
                        act("$p prevents $n from loosing some teeth.", victim,
                            damaged, NULL, TO_ROOM);
                        if (dam - damaged->toughness < 0)
                                return;
                        if ((dam - damaged->toughness > damaged->resistance))
                                damaged->condition -= damaged->resistance;
                        else
                                damaged->condition -=
                                        (dam - damaged->toughness);
                        if (damaged->condition < 1)
                        {
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_CHAR);
                                act("$p fall broken to the ground.", ch,
                                    damaged, NULL, TO_ROOM);
                                obj_from_char(damaged);
                                extract_obj(damaged);
                        }
                        return;
                }
                /*
                 * Stone skin will prevent any damage done to arms/legs.
                 */
                if (is_affected(victim, skill_lookup("stone skin")))
                        return;

                if (IS_HEAD(victim, LOST_TOOTH_1))
                        REMOVE_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_1);
                if (IS_HEAD(victim, LOST_TOOTH_2))
                        REMOVE_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_2);
                if (IS_HEAD(victim, LOST_TOOTH_4))
                        REMOVE_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_4);
                if (IS_HEAD(victim, LOST_TOOTH_8))
                        REMOVE_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_8);
                if (IS_HEAD(victim, LOST_TOOTH_16))
                        REMOVE_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_16);
                if (bodyloc + broken > 28)
                        broken -= 1;
                if (bodyloc + broken > 28)
                        broken -= 1;
                bodyloc += broken;
                if (bodyloc >= 16)
                {
                        bodyloc -= 16;
                        SET_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_16);
                }
                if (bodyloc >= 8)
                {
                        bodyloc -= 8;
                        SET_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_8);
                }
                if (bodyloc >= 4)
                {
                        bodyloc -= 4;
                        SET_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_4);
                }
                if (bodyloc >= 2)
                {
                        bodyloc -= 2;
                        SET_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_2);
                }
                if (bodyloc >= 1)
                {
                        bodyloc -= 1;
                        SET_BIT(victim->loc_hp[LOC_HEAD], LOST_TOOTH_1);
                }
                sprintf(buf,
                        "Your skillful blow knocks out %d of $N's teeth!",
                        broken);
                act(buf, ch, NULL, victim, TO_CHAR);
                sprintf(buf,
                        "$n's skillful blow knocks out %d of $N's teeth!",
                        broken);
                act(buf, ch, NULL, victim, TO_NOTVICT);
                sprintf(buf,
                        "$n's skillful blow knocks out %d of your teeth!",
                        broken);
                act(buf, ch, NULL, victim, TO_VICT);
                if (broken >= 1)
                        make_part(victim, "tooth");
                if (broken >= 2)
                        make_part(victim, "tooth");
                if (broken >= 3)
                        make_part(victim, "tooth");
                return;
        }
        return;
}
