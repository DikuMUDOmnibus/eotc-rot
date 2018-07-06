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

const char * exitname2 [6] =
{
	"to the north",
	"to the east",
	"to the south",
	"to the west",
	"above you",
	"below you"
};


char *get_room_description(CHAR_DATA *ch, char *descr) 
{
	char message[MAX_STRING_LENGTH];
	char message2[MAX_STRING_LENGTH];
	char message3[MAX_STRING_LENGTH];
	char message4[MAX_STRING_LENGTH];
	char message5[MAX_STRING_LENGTH];
	char msg[MAX_STRING_LENGTH];
//	char temp[MAX_STRING_LENGTH];
	char rdesc[MAX_STRING_LENGTH];
//	char buf[MAX_STRING_LENGTH];
//	ROOM_INDEX_DATA *pRoom;
	int i, letters, space, newspace, line;
//	EXIT_DATA *pexit;
//	int door;

	strcpy(msg, ch->in_room->description);

//	sprintf(msg, "%s\n\r", ch->in_room->description);

	sprintf(message, " ");
	sprintf(message2, " ");
	sprintf(message3, " ");
	sprintf(message4, " ");
	sprintf(message5, " ");

	switch(ch->in_room->sector_type) {
	case 0:
		strcat(message, " The inside air is still and musty. ");
		break;
	case 1:
		strcat(message, " The City street is below your feet. ");
		break;
	case 2:
		strcat(message, " Rocky, steep terrain leads up into these mountain peaks. ");
		break;
	case 3:
		strcat(message, " Water swirls all around. ");
		break;
	case 4:
		strcat(message, " The thin air swirls past, barely making even its presence known. ");
		break;
	case 5:
		strcat(message, " The details of this location are strangely difficult to determine. ");
		break;
	case 6:
		strcat(message, " Molten lava flows through the volcanic caverns. ");
		break;
	case 7:
		strcat(message, " The solid ice shows no trace of life. ");
		break;
	default:
		strcat(message, "");
		break;
	}

	strcat(msg, message);

	/* List appropriate room flags */
//	strcpy(message, "");
	if (IS_SET(ch->in_room->room_flags, ROOM_SAFE))
		strcat(message2, "A magical aura seems to promote a feeling of peace. ");
	if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
		strcat(message2, "A very weak pulling sensation emenates from this place. ");
	if (IS_SET(ch->in_room->room_flags, ROOM_DARK))
		strcat(message2, "It is quite dark in here. ");
	if (IS_SET(ch->in_room->room_flags, ROOM_PRIVATE))
		strcat(message2, "A sign on the wall states, 'This room is private.' ");
	if (IS_SET(ch->in_room->room_flags, ROOM_GODS_ONLY))
		strcat(message2, "This room is Flaged Gods Only ");
	if (IS_SET(ch->in_room->room_flags, ROOM_NEWBIES_ONLY))
		strcat(message2, "A magical shield eminates here allowing only newbies to enter. ");
	if (IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP))
		strcat(message2, "You are in a Pet Shop. "); 
	if (IS_SET(ch->in_room->room_flags, ROOM_INDOORS))
		strcat(message2, "You are inside. "); 

	strcat(msg, message2);

//	strcat(msg, " " );
/*
	if (pRoom->exit[0]->description != NULL){
	strcpy(msg, pRoom->exit[0]->description);
	strcat(msg, message);}
	if (pRoom->exit[1]->description != NULL){
	strcpy(msg, pRoom->exit[1]->description);
	strcat(msg, message);}
	if (pRoom->exit[2]->description != NULL){
	strcpy(msg, pRoom->exit[2]->description);
	strcat(msg, message);}
	if (pRoom->exit[3]->description != NULL){
	strcpy(msg, pRoom->exit[3]->description);
	strcat(msg, message);}
	if (pRoom->exit[4]->description != NULL){
	strcpy(msg, pRoom->exit[4]->description);
	strcat(msg, message);}
	if (pRoom->exit[5]->description != NULL){
	strcpy(msg, pRoom->exit[5]->description);
	strcat(msg, message);}
	strcat(msg, " " );
*/

	    if (ch->in_room->blood == 1000)
		strcat(message3, "You notice that the room is completely drenched in blood. ");
	    else if (ch->in_room->blood > 750)
		strcat(message3, "You notice that there is a very large amount of blood around the room. ");
	    else if (ch->in_room->blood > 500)
		strcat(message3, "You notice that there is a large quantity of blood around the room. ");
	    else if (ch->in_room->blood > 250)
		strcat(message3, "You notice a fair amount of blood on the floor. ");
	    else if (ch->in_room->blood > 100)
		strcat(message3, "You notice several blood stains on the floor. ");
	    else if (ch->in_room->blood > 50)
		strcat(message3, "You notice a few blood stains on the floor. ");
	    else if (ch->in_room->blood > 25)
		strcat(message3, "You notice a couple of blood stains on the floor. ");
	    else if (ch->in_room->blood > 0)
		strcat(message3, "You notice a few drops of blood on the floor. ");
	    else strcat(message3, "You notice nothing special in the room. ");

	strcat(msg, message3);

//	strcat(msg, " " );


	if (weather_info.sunlight == SUN_LIGHT) 
      strcat(message4, "The day has begun. ");

	if (weather_info.sunlight == SUN_RISE)
	strcat (message4, "The sun rises in the east. ");
 
	if (weather_info.sunlight == SUN_SET)
	strcat (message4, "The sun slowly disappears in the west. ");

	if (weather_info.sunlight == SUN_DARK)
      strcat (message4, "The night has begun. ");

	strcat(msg, message4);
//	strcat(msg, " " );



	if (weather_info.sky == SKY_CLOUDLESS)
	strcat (message5, "The sky is getting cloudy. ");

	if (weather_info.sky == SKY_CLOUDY)
	{
      if (weather_info.mmhg < 970 || (weather_info.mmhg < 990 && number_bits (2) == 0))
      strcat (message5, "It starts to rain.\n\r");
 
      else if (weather_info.mmhg > 1030 && number_bits (2) == 0)
      strcat (message5, "The clouds disappear.\n\r");
	}
	if (weather_info.sky == SKY_RAINING)
	{
	if (weather_info.mmhg < 970 && number_bits (2) == 0)
      strcat (message5, "Lightning flashes in the sky.\n\r");

      else if (weather_info.mmhg > 1030 || (weather_info.mmhg > 1010 && number_bits (2) == 0))
      strcat (message5, "The rain stopped.\n\r");
	}

	if (weather_info.sky == SKY_LIGHTNING)
	{
	if (weather_info.mmhg > 1010 || (weather_info.mmhg > 990 && number_bits (2) == 0))
      strcat (message5, "The lightning has stopped.\n\r");
	}
	strcat(msg, message5);
	strcat(msg, " " );


	i=0;
	letters=0;

	/* Strip \r and \n */
	for (i=0; i<strlen(msg); i++) {
		if (msg[i] != '\r' && msg[i] != '\n') {
			rdesc[letters]=msg[i];
			letters++;
		} else if (msg[i] == '\r') {
			rdesc[letters]=' ';
			letters++;
		}
		rdesc[letters]='\0';
	}

	i=0;
	letters=0;
	space=0;
	newspace=0;
	line=0;
	strcpy(msg, rdesc);

	/* Add \r\n's back in at their appropriate places */
	for (i=0; i<strlen(msg); i++) {
		if (msg[i]==' ') {
			space=i;
			newspace=letters;
		}

		if (line > 70) {
			i=space;
			letters=newspace;
			rdesc[letters++]='\r';
			rdesc[letters++]='\n';
			line=0;
		} else if (!(msg[i]==' ' && msg[i+1]==' ')) {
			rdesc[letters]=msg[i];
			letters++; /* Index for rdesc; i is the index for buf */
			line++; /* Counts number of characters on this line */
		}
		rdesc[letters+1]='\0';
	}
	if (strlen(rdesc) > 0)
		strcat(rdesc, "\r\n");

//	descr=STRALLOC(rdesc);
	mudstrlcpy( descr, rdesc, MSL );
	return descr;

	return descr;
}



