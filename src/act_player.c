/* Database interface for a simple player db */

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

#include <mysql/mysql.h>

#include "merc.h"

#define MY_ASC 1
#define MY_DESC 2

#define MY_ID 		0
#define MY_NAME		1
#define MY_ACCOUNT	2
#define MY_RACE		3
#define MY_PLAYED	4
#define MY_CLASS	5
#define MY_MAX		6

#define MY_ALL		999999

MYSQL *mydb;

const char *my_order[] = 
{	"",
	" ASC",
	" DESC"
};

const char *my_player[] = 
{	"player_id",
	"player_name",
	"player_account",
	"player_race",
	"player_played",
	"player_class"
};


typedef struct player_db
	{	MYSQL_RES *players;
	}
PLAYER_DATA;

/* Select the players. Options for order and limit.
 * int sortby, use MY_ID, MY_NAME, etc to order by that field.
 * int order, use MY_ASC, or MY_DESC to signify ascending order, or descending order. 0 for no preference
 * int offset, the number of entries you want to start displaying result (eg, start at the 10th user)
 * int limit, limit the number of results
***	Examples

--*Get All Players, no sorting.
players = get_players_by(-1, 0, 0, MY_ALL);

--*Get all players in alphabetical order (a-z)
players = get_players_by(MY_NAME, MY_ASC, 0, MY_ALL);

--*Get the 10th-19th player, that's played the most. Useful for paging.
players = get_players_by(MY_PLAYED, MY_ASC, 10, 9);

--*Get the first 10 players in class 1.
players = get_players_by(MY_CLASS, MY_ASC, 0, 10);

***
 * To add fields, make sure they're populated with some default value in the mysql database,
 * add them to the variables my_player, and the appropriate MY_ and increase MY_MAX.
 * - Davion
 */
PLAYER_DATA * get_players_by(int sortby, int order, int offset, int limit)
{	char query[MSL];
	int len;
	PLAYER_DATA *p;

	p = calloc(1, sizeof(*p) );
	
	if( sortby == -1 )
	{	len = sprintf(query, "SELECT * FROM players");
		mysql_real_query(mydb, query, len);
		p->players = mysql_store_result(mydb);
		return p;
	}

	sprintf(query, "SELECT * FROM players SORT BY %s%s", my_player[sortby], my_order[order]);

	if(limit > 0 )
	{	char add[MSL];
		sprintf(add, " LIMIT %d", limit);
		strcat(query, add);
		if( offset > 0 )
		{	sprintf(add, ", %d", offset );
			strcat(query, add);
		}
	}
	mysql_real_query(mydb, query, strlen(query) );
	p->players = mysql_store_result(mydb);
	return p;
}

void init_iter(PLAYER_DATA *p, MYSQL_ROW *row)
{	*row = mysql_fetch_row(p->players);
}

bool condition_iter(MYSQL_ROW *row)
{	return *row != NULL;
}

#define FOR_PLAYERS(p, r) for( (init_iter((p),(r)) ; condition_iter( (r) ) ; init_iter( (p), (r) ) )

int my_sort_lookup(const char *str)
{	int i;
	for( i = 0; i != MY_MAX ; ++i)
		if(!strcasecmp(str, my_player[i] ) )
			return i;
	return -1;
}

bool player_exists(const char *name )
{	char query[MSL];
	char safe_name[MSL];
	MYSQL_RES *res;
	int len;
	safe_name[0] ='\0';
	mysql_real_escape_string(mydb, safe_name, name, strlen(name) );
	len = sprintf(query, "SELECT * FROM players WHERE player_name='%s'", safe_name);
	mysql_real_query(mydb, query, len);
	res = mysql_store_result(mydb);
	if(mysql_num_rows(res) > 0 )
		len = TRUE;
	else
		len = FALSE;
	mysql_free_result(res);
	return len;
}

void register_player(CHAR_DATA *ch)
{	char query[MSL];
	int len;

	if(IS_NPC(ch)  || ch->desc == NULL || ch->desc->account == NULL )
		return;

	len = sprintf(query, "INSERT INTO players( player_name, player_account, player_race, player_played, player_class ) VALUES ( '%s', '%s', %d, %d, %d )",
			      ch->name, ch->desc->account->owner, ch->race, ch->played, ch->class );

	mysql_real_query(mydb, query, len);
}

void unregister_player(CHAR_DATA *ch)
{	char query[MSL];
int len;
	if(IS_NPC(ch)  || ch->desc == NULL || ch->desc->account == NULL )
		return;

	len = sprintf(query, "DELETE FROM players WHERE player_name='%s'", ch->name );

	mysql_real_query(mydb, query, len);
}

void update_player(CHAR_DATA *ch)
{	char query[MSL];
	int len;

	if(IS_NPC(ch)  || ch->desc == NULL || ch->desc->account == NULL )
		return;

	if(!player_exists(ch->name) )
		register_player(ch);

	len = sprintf(query, "UPDATE players SET player_played=%d WHERE player_name='%s'", (ch->played + (int) (current_time - ch->logon)) /3600, ch->name );
	mysql_real_query(mydb, query, len);
}
