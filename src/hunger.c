#include <stdio.h>
#include <time.h>
#include "merc.h"
// #include "utils.h"

void update_condition(CHAR_DATA * ch)
{
    /*
     * Lookin California 
     */

    if (IS_IMMORTAL(ch))
  	  return;

        if (ch->pcdata->learned[gsn_satiate] > 1)
        {
	check_improve (ch, gsn_satiate, TRUE, 1);	

	if (ch->pcdata->condition[COND_HUNGER] < 40)
	ch->pcdata->condition[COND_HUNGER] = 45;

	if (ch->pcdata->condition[COND_THIRST] < 40)
	ch->pcdata->condition[COND_THIRST] = 45;
	return;
        }


    /*
     * Feelin Minnesota 
     */



    if (ch->pcdata->condition[COND_HUNGER] > 0) 
	    ch->pcdata->condition[COND_HUNGER] -= 1;

    if (ch->pcdata->condition[COND_HUNGER] == 0
	&& ch->max_hit > DAM_HUNGER) 
	{
	      damage(ch, ch, ch->max_hit / 6, gsn_hunger, DAM_HUNGER, FALSE);
	      ((ch->move > 0) ? (ch->move -= (ch->max_move / 10)) : (ch->move = 0));
			
	      ((ch->mana > 0) ? (ch->mana -= (ch->max_mana / 10)) : (ch->mana = 0));

	      send_to_char("The pain in your tummy has become unbearable!\n\r", ch);
	      act("$n doubles over in hunger pains.", ch, NULL, NULL, TO_ROOM);
    } else if (ch->pcdata->condition[COND_HUNGER] == 1) {
        send_to_char("If you do not eat, you will soon die.\n\r", ch);
    } else if (ch->pcdata->condition[COND_HUNGER] <= 3) {
        send_to_char
          ("You feel weak and tired from lack of nourishment.\n\r", ch);
    } else if (ch->pcdata->condition[COND_HUNGER] <= 6) {
        send_to_char("Your body needs nourishment.\n\r", ch);
    } else if (ch->pcdata->condition[COND_HUNGER] <= 9) {
        send_to_char("Your stomach growls.\n\r", ch);
    }


    if (ch->pcdata->condition[COND_THIRST] > 0) 
      ch->pcdata->condition[COND_THIRST] -= 1;

  if (ch->pcdata->condition[COND_THIRST] == 0) {
      send_to_char("You nearly pass out from thirst!\n\r", ch);
      act("Clutching at $s throat, $n looks about for water.", ch, NULL,
        NULL, TO_ROOM);
      damage(ch, ch, ch->max_hit / 6, gsn_thirst, DAM_THIRST, FALSE);
      ((ch->move > 0) ? (ch->move -= (ch->max_move / 10)) : (ch->move = 0));
      ((ch->mana > 0) ? (ch->mana -= (ch->max_mana / 10)) : (ch->mana = 0));
  } else if (ch->pcdata->condition[COND_THIRST] == 1) {
      send_to_char("If you do not find water, you will soon die.\n\r", ch);
  } else if (ch->pcdata->condition[COND_THIRST] <= 3) {
      send_to_char("You feel weak and tired from lack of water.\n\r", ch);
  } else if (ch->pcdata->condition[COND_THIRST] <= 6) {
      send_to_char
        ("Your body's craving for water is beginning to be disruptive.\n\r",
         ch);
  } else if (ch->pcdata->condition[COND_THIRST] <= 9) {
      send_to_char("Your throat is dry.\n\r", ch);
  }

      send_to_char("(practicing satiate will help)\n\r", ch);    

    if (ch->pcdata->condition[COND_DRUNK] > 0)
	    ch->pcdata->condition[COND_DRUNK] -= 1;


    return;
}
