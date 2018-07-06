#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include <unistd.h>




void do_version( CHAR_DATA *ch, char *argument)
{
  send_to_char("{Y******************************************************{x\n\r",ch);
  send_to_char("{Y* {RThis mud runs Eye of the Cyclops 2                 {Y*\n\r",ch);
  send_to_char("{Y* {RYou can download the last released version at      {Y*\n\r",ch);
  send_to_char("{Y* {Rwww.mudbytes.net                                   {Y*\n\r",ch);
  send_to_char("{Y* {RThis mud was started from Quickmud                 {Y*\n\r",ch);
  send_to_char("{Y*                                                    {Y*\n\r",ch);
  send_to_char("{Y* {RThis Codebase contains partial code from:          {Y*\n\r",ch);
  send_to_char("{Y* {RAnatolia v2.1, ROT v1.4, Death Rising v1.0a,       {Y*\n\r",ch);
  send_to_char("{Y* {RShattered Dreams, Alanthia, Distorted Illusions    {Y*\n\r",ch);
  send_to_char("{Y* {RGodwars, Dystopia, Mindcloud                       {Y*\n\r",ch);
  send_to_char("{Y******************************************************{x\n\r",ch);
  return;
}

