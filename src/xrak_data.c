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



void load_leaderboard()
{
  FILE *fp;

  if ((fp = fopen("../txt/leader.txt", "r")) == NULL)
  {   
    log_string("Error: leader.txt not found!");
    exit(1);
  }
  leader_board.bestpk_name = fread_string(fp);  
  leader_board.bestpk_number = fread_number(fp);
  leader_board.pk_name = fread_string(fp);
  leader_board.pk_number = fread_number(fp);
  leader_board.pd_name = fread_string(fp);
  leader_board.pd_number = fread_number(fp);
  leader_board.mk_name = fread_string(fp);
  leader_board.mk_number = fread_number(fp);
  leader_board.md_name = fread_string(fp);
  leader_board.md_number = fread_number(fp);
  leader_board.tt_name = fread_string(fp);
  leader_board.tt_number = fread_number(fp);
  leader_board.qc_name = fread_string(fp);
  leader_board.qc_number = fread_number(fp);
  fclose(fp);
}

void save_leaderboard()
{
  FILE *fp;

  if ((fp = fopen("../txt/leader.txt","w")) == NULL)
  {
    log_string("Error writing to leader.txt");
    return;
  }
  fprintf(fp, "%s~\n", leader_board.bestpk_name); 
  fprintf(fp, "%d\n", leader_board.bestpk_number);
  fprintf(fp, "%s~\n", leader_board.pk_name);
  fprintf(fp, "%d\n", leader_board.pk_number);
  fprintf(fp, "%s~\n", leader_board.pd_name);     
  fprintf(fp, "%d\n", leader_board.pd_number);
  fprintf(fp, "%s~\n", leader_board.mk_name);
  fprintf(fp, "%d\n", leader_board.mk_number);
  fprintf(fp, "%s~\n", leader_board.md_name);
  fprintf(fp, "%d\n", leader_board.md_number);
  fprintf(fp, "%s~\n", leader_board.tt_name);
  fprintf(fp, "%d\n", leader_board.tt_number);
  fprintf(fp, "%s~\n", leader_board.qc_name);
  fprintf(fp, "%d\n", leader_board.qc_number);
  fclose (fp);
}

void do_leader( CHAR_DATA *ch, char *argument )
{    
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  stc("{Y-{8=========================={YLEADER BOARD{8=========================={Y-{x\n\r\n\r",ch);

  send_to_char("   {yMr. Best PK'er   {C--->    ",ch);
  sprintf(buf, "{G%-13s",leader_board.bestpk_name);
  send_to_char(buf,ch);
  sprintf(buf, " {ywith a {G%d {yPK score{x\n\r",leader_board.bestpk_number);
  send_to_char(buf,ch);

  send_to_char("   {yMr. Deadly       {C--->    ",ch);
  sprintf(buf, "{G%-13s",leader_board.pk_name);
  send_to_char(buf,ch);
  sprintf(buf, " {ywith {G%d {ypkills{x\n\r",leader_board.pk_number);
  send_to_char(buf,ch);

  send_to_char("   {yMr. Pathetic     {C--->    ",ch);
  sprintf(buf, "{G%-13s",leader_board.pd_name);
  send_to_char(buf,ch);
  sprintf(buf, " {ywith {G%d {ypdeaths{x\n\r",leader_board.pd_number);
  send_to_char(buf,ch);
  
  send_to_char("   {yMr. Slayer       {C--->    ",ch);
  sprintf(buf, "{G%-13s",leader_board.mk_name);
  send_to_char(buf,ch);
  sprintf(buf, " {ywith {G%d {ymkills{x\n\r",leader_board.mk_number);
  send_to_char(buf,ch);
  
  send_to_char("   {yMr. Wimpy        {C--->    ",ch);
  sprintf(buf, "{G%-13s",leader_board.md_name);
  send_to_char(buf,ch);
  sprintf(buf, " {ywith {G%d {ymdeaths{x\n\r",leader_board.md_number);
  send_to_char(buf,ch);
  
  send_to_char("   {yMr. No life      {C--->    ",ch);
  sprintf(buf, "{G%-13s",leader_board.tt_name);
  send_to_char(buf,ch);
  sprintf(buf, " {ywith {G%d {yHours played{x\n\r",leader_board.tt_number);
  send_to_char(buf,ch);
  
  send_to_char("   {yMr. Questor      {C--->    ",ch);
  sprintf(buf, "{G%-13s",leader_board.qc_name);
  send_to_char(buf,ch);
  sprintf(buf, " {ywith {G%d {yQuests completed{x\n\r",leader_board.qc_number);
  send_to_char(buf,ch);

  
  stc("Y-{8=========================={YLEADER BOARD{8=========================={Y-{x\n\r\n\r",ch);

  return;
}

void check_leaderboard( CHAR_DATA *ch )
{
  bool changed = FALSE;

  if (IS_NPC(ch)) return;
  if (ch->level > 206) return;
  if (get_ratio(ch) > leader_board.bestpk_number || !str_cmp(leader_board.bestpk_name , ch->name))
  {
    leader_board.bestpk_number = get_ratio(ch);
    free_string(leader_board.bestpk_name);
    leader_board.bestpk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->mdeaths > leader_board.md_number)
  {
    leader_board.md_number = ch->mdeaths;
    free_string(leader_board.md_name);
    leader_board.md_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pcdata->mkills > leader_board.mk_number)
  {
    leader_board.mk_number = ch->pcdata->mkills;
    free_string(leader_board.mk_name);
    leader_board.mk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pkill > leader_board.pk_number)
  {
    leader_board.pk_number = ch->pkill;
    free_string(leader_board.pk_name);
    leader_board.pk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pdeath > leader_board.pd_number)
  {
    leader_board.pd_number = ch->pdeath;
    free_string(leader_board.pd_name);
    leader_board.pd_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->questscomp > leader_board.qc_number)
  {
    leader_board.qc_number = ch->questscomp;
    free_string(leader_board.qc_name);
    leader_board.qc_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (get_age(ch)-17*2  > leader_board.tt_number)
  {
    leader_board.tt_number = (get_age(ch)-17)*2;
    free_string(leader_board.tt_name);
    leader_board.tt_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (changed) save_leaderboard();
  return;
}

void do_leaderclear(CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (ch->level < 7) return;
  free_string(leader_board.bestpk_name);
  leader_board.bestpk_name = str_dup("Noone");
  leader_board.bestpk_number = 0;
  free_string(leader_board.pk_name);
  leader_board.pk_name = str_dup("Noone");
  leader_board.pk_number = 0;
  free_string(leader_board.pd_name);
  leader_board.pd_name = str_dup("Noone");
  leader_board.pd_number = 0;
  free_string(leader_board.md_name);
  leader_board.md_name = str_dup("Noone");
  leader_board.md_number = 0;
  free_string(leader_board.mk_name);
  leader_board.mk_name = str_dup("Noone");
  leader_board.mk_number = 0;
  free_string(leader_board.qc_name);
  leader_board.qc_name = str_dup("Noone");
  leader_board.qc_number = 0;
  free_string(leader_board.tt_name);
  leader_board.tt_name = str_dup("Noone");
  leader_board.tt_number = 0;
  save_leaderboard();
  send_to_char("Leader board cleared.\n\r",ch);
  return;
}

