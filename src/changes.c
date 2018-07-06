/*
 * Change.c   (last update 3rd may, 2001)
 *
 * Allows adding and removing of changes while online,
 * also handles displaying the changes to the player.
 *
 * Would be a good thing to add to a players login,
 * but keep the MAX_CHANGE low in that case, so the player
 * doesn't get spammed to much.
 *
 * Code by Brian Graversen aka Jobo
 */

/*
 * Note, the code uses memory recycling, even though it's
 * probably a waste of time, since most muds won't add that
 * many changes in between each copyover. But I figured why
 * not make it right the first time around.
 */

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "changes.h"
#include "merc.h"

bool remove_change   args((int i));
struct change_data *change_table;
int max_change = 0;
int num_changes args((void));
char     *current_date args((void));

/*
 * load_changes simply loads the list of changes,
 * should only be called at boot time. Add this
 * call to db.c's boot function.
 */
void load_changes()
{
  FILE *fp;
  char *name;
  int i;

  if ((fp = fopen("../txt/changes.txt", "r")) == NULL)
  {
    log_string("Non-fatal error: changes.txt not found!");
    return;
  }
  max_change = fread_number(fp);
  change_table = alloc_perm(sizeof(CHANGE_DATA) * (max_change));
  for ( i = 0; i < max_change; i++ )
  {
    change_table[i].imm  = fread_string(fp);
    change_table[i].date = fread_string(fp);
    change_table[i].text = fread_string(fp);
  }

  fclose(fp);
}


char     *current_date()
{
        static char buf[128];
        struct tm *datetime;

        datetime = localtime(&current_time);
        strftime(buf, sizeof(buf), "%m/%d/%Y", datetime);
        return buf;
}

/*
 * save_changes handles the storage of the change_list into
 * a file. It is called each time the list is changed.
 */
void save_changes()
{
  FILE *fp;
  int i;

  if ((fp = fopen("../txt/changes.txt","w")) == NULL)
  {
    log_string("Error writing to changes.txt");
    return;
  }
  fprintf(fp, "%d\n", max_change);
  for ( i = 0; i < max_change; i++ )
  {
    fprintf(fp, "%s~\n", change_table[i].imm);
    fprintf(fp, "%s~\n", change_table[i].date);
    fprintf(fp, "%s~\n", change_table[i].text);
  }
  fclose(fp);
}

/*
 * This is the immortal function, which allows any
 * immortal with access to the function to add new
 * changes to the changes list. The immortals name
 * will also be added to the list, as well as the
 * date the change was added.
 */
void do_addchange( CHAR_DATA *ch, char *argument )
{
  CHANGE_DATA *newchange;
  char *strtime;
  char buf[100];
  int i;

  if (IS_NPC(ch)) return;
  smash_tilde(argument);

  /* we need something to add to the list */
  if (argument[0] == '\0' || strlen(argument) < 5)
  {
    send_to_char("What did you change?\n\r", ch);
    return;
  }
  /* Mainly to avoid that the list looks ugly */
  if (strlen(argument) > 300)
  {
    send_to_char("Keep it on 300 chars please.\n\r", ch);
    return;
  }

  /* Set the current time */
  strtime = ctime(&current_time);
  for (i = 0; i < 6; i++)
  {
    buf[i] = strtime[i + 4];
  }
  buf[6] = '\0';

  max_change++;

  // Would realloc, but the MUD doesn't seem to like that. -Akurei
  newchange = malloc(sizeof(CHANGE_DATA) * max_change);
  for ( i = 0; i < max_change - 1; i++ )
  {
    newchange[i].imm = str_dup(change_table[i].imm);
    newchange[i].date = str_dup(change_table[i].date);
    newchange[i].text = str_dup(change_table[i].text);
  }
  change_table = newchange;

  /* set the strings for the change */
  change_table[max_change - 1].imm = str_dup(ch->name);
  change_table[max_change - 1].date = str_dup(current_date());
  change_table[max_change - 1].text = str_dup(wordwrapchange(argument));

  send_to_char("Change added.\n\r", ch );
  save_changes();
  sprintf(buf, "%s has added a new change.", ch->name);
  do_info(ch,buf);
  return;
}

int num_changes(void)
{
        char     *test;
        int       today;
        int       i;

        i = 0;
        test = current_date();
        today = 0;

        for (i = 0; i < max_change; i++)
                if (!str_cmp(test, change_table[i].date))
                        today++;

        return today;
}


/*
 * The player function.
 * simply lists the last MAX_CHANGE changes
 */
void do_changes(CHAR_DATA *ch, char *argument)
{
  char buf[MSL];
  char arg[MSL];
  int i, x, v;

  if (IS_NPC(ch)) return;

  argument = one_argument(argument,arg);
  if ( is_number(arg) ) v = atoi(arg) > max_change/10+1 ? max_change/10+1 : atoi(arg);
  else v = max_change / 10 + 1;

  sprintf(buf, "{C--{c::::{C---{c::{C-------------------------------------------------{c::{C---{c::::{C--{x\n\r"); 
  stc(buf,ch);
  sprintf(buf, "                 {DCode Changes to {CEye of the Cyclops               {x\n\r"); 
  stc(buf,ch);
  sprintf(buf, "{C--{c::::{C---{c::{C-------------------------------------------------{c::{C---{c::::{C--{x\n\r");
  stc(buf,ch);

  if ( is_number(arg) && atoi(arg) > 0)
  for ( i = (v-1)*10; i < (v*10); i++ )
  {
    if ( i >= max_change ) break;
    sprintf(buf, " {C[{c%3d{C] {W%-10s  {D%-12s {c%s{x\n\r",
	i+1, change_table[i].date, change_table[i].imm, change_table[i].text);
    stc(buf,ch);
  }
  else
  for ( i = max_change-10 < 0 ? 0 : max_change-10; i < max_change; i++ )
  {
    if ( i >= max_change ) break;
    sprintf(buf, " {C[{c%3d{C] {W%-10s  {D%-12s {c%s{x\n\r",
	i+1, change_table[i].date, change_table[i].imm, change_table[i].text);
    stc(buf,ch);
  }
  stc("\n\r",ch);
  sprintf(buf, "{C--{c::::{C---{c::{C-------------------------------------------------{c::{C---{c::::{C--{x\n\r");
  stc(buf,ch);
  sprintf(buf, "      {DType 'changes <1-%d>' to view a specific page of changes.\n\r",(max_change/10)+1);
  stc(buf,ch);
  sprintf(buf, "           {DThere are a total of %d changes in the database.\n\r", max_change); 
  stc(buf,ch);
  sprintf(buf, "{C--{c::::{C---{c::{C-------------------------------------------------{c::{C---{c::::{C--{x\n\r");
  stc(buf,ch);
  return;
}

char *wordwrapchange(char *word)
{
  char newword[MAX_STRING_LENGTH];
  char newword2[MAX_STRING_LENGTH];
  int i = 0, x = 0, n = 0, v = 0;

  // i = original word position
  // x = newword position
  // n = spacer
  // v = letter count

  for ( i = 0; i < strlen(word); i++ )
  {
    if ( word[i] == ' ' && v > 40 )
    {
      v = 0;
      newword[x++] = '\n';
      for ( n = 0; n < 33; n++ ) { newword[x++] = ' '; }
    }
    newword[x++] = word[i];
    v++;
  }
  newword[x++] = '\0';
/*
  for ( i = 0; i < strlen(word); i++ )
  {
    if ( word[i] == ' ' && v > 40 ) break;
    newword[x++] = word[i];
    v++;
  }
  if ( strlen(word) > strlen(newword) )
  {
    v = 0;
    newword[x++] = '\n';
    for ( n = 0; n < 28; n++ ) { newword[x++] = ' '; }
    for ( i = i + 1; i < strlen(word); i++ )
    {
      if ( word[i] == ' ' && v > 40 )
      {
        newword[x++] = '\n';
        for ( n = 0; n < 25; n++ ) { newword[x++] = ' '; }
	v = 1;
      }
      else
      {
        newword[x++] = word[i];
        v++;
      }
    }
  }
*/
  strcpy(newword2,newword);
  return str_dup(newword2);
}

/*
 * delchange removes a given change, and adds the
 * change to a free_list so it can be recycled later
 */
void do_delchange(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  bool found = FALSE;
  int i;

  if (IS_NPC(ch)) return;

  one_argument(argument, arg);

  if ((i = atoi(arg)) < 1)
  {
    send_to_char("Which number change did you want to remove ?\n\r", ch);
    return;
  }
  found = remove_change(i);
  if (!found) send_to_char("No such change.\n\r", ch);
  else send_to_char("Change removed.\n\r", ch);
  save_changes();
  return;
}

/*
 * This function handles the actual removing of the change
 */
bool remove_change(int i)
{
  CHANGE_DATA *newchange;
  bool found = FALSE;
  int x, n;

  for ( x = 0; x < max_change; x++ )
  {
    if ( i-1 != x ) continue;
    found = TRUE;
    max_change--;
    newchange = malloc(sizeof(CHANGE_DATA) * max_change);
    for ( n = 0; n < max_change; n++ )
    {
      if ( n < x )
      {
        newchange[n].imm = str_dup(change_table[n].imm);
        newchange[n].date = str_dup(change_table[n].date);
        newchange[n].text = str_dup(change_table[n].text);
      }
      else
      {
        newchange[n].imm = str_dup(change_table[n+1].imm);
        newchange[n].date = str_dup(change_table[n+1].date);
        newchange[n].text = str_dup(change_table[n+1].text);
      }
    }
    change_table = newchange;
    save_changes();
    break;
  }

  return found;
}


