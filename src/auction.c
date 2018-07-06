#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"


#define AUCTION_LENGTH	5


char	*flag_string		args ( ( const struct flag_type *flag_table,
					long bits ) );
BUFFER	*show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
					bool fShort, bool fShowNothing ) );


AUCTION_DATA *auction_free;

AUCTION_DATA *new_auction( void )
{
    AUCTION_DATA *auction;

    if ( auction_free == NULL )
	auction		= alloc_perm( sizeof(*auction) );
    else
    {
	auction		= auction_free;
	auction_free	= auction_free->next;
    }

    auction->next		= NULL;
    auction->item		= NULL;
    auction->owner		= NULL;
    auction->high_bidder	= NULL;
    auction->status		= 0;
    auction->slot		= 0;
    auction->bid_type		= VALUE_GOLD;
    auction->bid_amount		= 0;

    return auction;
}

void free_auction( AUCTION_DATA *auction )
{
    if ( auction == auction_list )
	auction_list = auction_list->next;
    else
    {
	AUCTION_DATA *auc;

	for ( auc = auction_list; auc != NULL; auc = auc->next )
	{
	    if ( auc->next == auction )
	    {
		auc->next = auction->next;
		break;
	    }
	}
    }

    auction->next	= auction_free;
    auction_free	= auction;

    if ( auction_list == NULL )
	auction_ticket = 0;
}

BUFFER * display_stats( OBJ_DATA *obj, CHAR_DATA *ch, bool contents )
{
    BUFFER *output = new_buf( );
    AFFECT_DATA *paf;
    char buf[MAX_STRING_LENGTH], final[MAX_STRING_LENGTH];
    bool place = FALSE;
    sh_int pos;

    sprintf( buf, "( {w%s {B)", obj->short_descr );
    str_replace( buf, "{x", "{w" );
    str_replace( buf, "{0", "{w" );

    while ( strlen_color( buf ) < 77 )
    {
	if ( place )
	    strcat( buf, "=" );
	else
	{
	    sprintf( final, "=%s", buf );
	    strcpy( buf, final );
	}
	place = !place;
    }

    sprintf( final, "{B %s\n\r", buf );
    add_buf( output, final );

    if ( IS_IMMORTAL( ch ) )
    {
	OBJ_DATA *in_obj;
/*
	if ( IS_TRUSTED( ch, MAX_LEVEL ) )
	{
	    if ( obj->loader != NULL )
	    {
		sprintf( buf, "Loaded String: {w%s", obj->loader );
		sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
		add_buf( output, final );
	    }

	    if ( obj->disarmed_from != NULL )
	    {
		sprintf( buf, "Disarmed From: {w%s",
		    obj->disarmed_from->name );
		sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
		add_buf( output, final );
	    }

	    if ( obj->multi_data != NULL )
	    {
		OBJ_MULTI_DATA *obj_mult;

		for ( obj_mult = obj->multi_data; obj_mult != NULL; obj_mult = obj_mult->next )
		{
		    sprintf( buf, "Dropped by: {w%s{y@{w%s{y: {w%d",
			obj_mult->dropper, obj_mult->socket, obj_mult->drop_timer );
		    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
		    add_buf( output, final );
		}
	    }
	}
*/
	sprintf( buf, "Vnum: {w%-5d       {yTimer: {w%-5d      {yResets: {w%-5d",
	    obj->pIndexData->vnum, obj->timer, obj->pIndexData->reset_num );
	sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	add_buf( output, final );

	sprintf( buf, "{yValue[{w0{y]: {w%-7d {yValue[{w1{y]: {w%-7d {yValue[{w2{y]: {w%-7d",
	    obj->value[0], obj->value[1], obj->value[2] );
	sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	add_buf( output, final );

	sprintf( buf, "{yValue[{w3{y]: {w%-7d {yValue[{w4{y]: {w%-7d {yWeight: {w%d{y/{w%d{y/{w%d {y(10th pounds)",
	    obj->value[3], obj->value[4],
	    obj->weight, get_obj_weight( obj ),get_true_weight(obj) );
	sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	add_buf( output, final );

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	sprintf( buf, "In Room: {w%-5d    {yIn Object: {w%s",
	    in_obj->in_room != NULL ? in_obj->in_room->vnum :
	    in_obj->carried_by == NULL ? 0 :
	    in_obj->carried_by->in_room != NULL ?
	    in_obj->carried_by->in_room->vnum : 0,
	    obj->in_obj == NULL ? "(none)" : obj->in_obj->short_descr );
	sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	add_buf( output, final );

	sprintf( buf, "Wear Loc: {w%-3d     {yCarried by: {w%s",
	    obj->wear_loc,
	    in_obj->carried_by == NULL ? "(none)" :
	    PERS( in_obj->carried_by, ch ) );
	sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	add_buf( output, final );

	if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
	{
	    EXTRA_DESCR_DATA *ed;

	    sprintf( buf, "Extra Description Keywords: '{w" );

	    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	    {
		strcat( buf, ed->keyword );
		if ( ed->next != NULL )
		    strcat( buf, " " );
	    }

	    for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	    {
		strcat( buf, ed->keyword );
		if ( ed->next != NULL )
		    strcat( buf, " " );
	    }

	    strcat( buf, "{y'" );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );
	}

	add_buf( output, " =============================================================================\n\r" );

    }
/*
    if ( obj->pIndexData->history != NULL
    &&   obj->pIndexData->history[0] != '\0' )
    {
	char *rdesc;

	buf[0] = '\0';
	final[0] = '\0';

	for ( rdesc = obj->pIndexData->history; *rdesc; rdesc++ )
	{
	    if ( *rdesc != '{' && *rdesc != '\n' )
	    {
		sprintf( buf, "%c", rdesc[0] );
		strcat( final, buf );
	    }

	    else if ( *rdesc != '\n' )
	    {
		sprintf( buf,"%c%c", rdesc[0], rdesc[1] );
		strcat( final, buf );
		rdesc++;
	    }

	    if ( *rdesc == '\n' && *(rdesc + 1) )
	    {
		sprintf( buf,"| {y%s {B|\n\r",
		    end_string( final, 75 ) );
		add_buf( output, buf );

		buf[0] = '\0';
		final[0] = '\0';
		rdesc++;
	    }
	}
    }
*/
    sprintf( buf, "Keywords: {w%s", obj->name );
    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
    add_buf( output, final );

    sprintf( buf, "Long: {w%s", obj->description );
    str_replace( buf, "{x", "{w" );
    str_replace( buf, "{0", "{w" );
    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
    add_buf( output, final );

    sprintf( buf, "Type: {w%s", flag_string( type_flags, obj->item_type ) );
    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
    add_buf( output, final );

    sprintf( buf, "Flags: {w%s", flag_string( extra_flags, obj->extra_flags ) );
    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
    add_buf( output, final );

    sprintf( buf, "Location: {w%s", flag_string( wear_flags, obj->wear_flags ) );
    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
    add_buf( output, final );
/*
    sprintf( buf, "Size: {w%d {y[{w%s{y]",
       obj->size, flag_string( size_flags, obj->size ) );
    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
    add_buf( output, final );
*/
    sprintf( buf, "Weight: {w%d{y, Value: {w%d{y, Level: {w%d",
        obj->weight, obj->cost, obj->level );
    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
    add_buf( output, final );
/*
    place = FALSE;
    sprintf( buf, "Class Restrict: {w" );
    for ( pos = 0; pos < MAX_CLASS && class_table[pos].name != NULL; pos++ )
    {
	if ( !obj->pIndexData->class_can_use[pos] )
	{
	    strcat( buf, class_table[pos].name );
	    strcat( buf, ", " );
	    place = TRUE;
	}
    }

    if ( place )
    {
	buf[strlen( buf ) -2] = '\0';
	sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	add_buf( output, final );
    }
*/
    switch ( obj->item_type )
    {
	case ITEM_SCROLL:
	case ITEM_POTION:
	case ITEM_PILL:
	    sprintf( buf, "Contains level {w%d {yspells of:", obj->value[0] );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );

	    buf[0] = '\0';

	    for ( pos = 1; pos <= 4; pos++ )
	    {
		if ( obj->value[pos] >= 0 && obj->value[pos] < MAX_SKILL )
		{
		    strcat( buf, " {y'{w" );
		    strcat( buf, skill_table[obj->value[pos]].name );
		    strcat( buf, "{y'" );
		}
	    }

	    strcat( buf, "." );
	    sprintf( final, "| %s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );
	    break;

	case ITEM_FURNITURE:
	    sprintf( buf, "Flags: {w%s",
		flag_string( furniture_flags, obj->value[2] ) );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );

	    sprintf( buf, "Maximum People: {w%d", obj->value[0] );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );

	    sprintf( buf, "Maximum Weight: {w%d", obj->value[1] );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );

	    sprintf( buf, "Healing rates {w-- {yHp: {w%d{y, Mana: {w%d",
		obj->value[3], obj->value[4] );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );
	    break;

	case ITEM_WAND:
	case ITEM_STAFF:
	    sprintf( buf, "Charges: {w%d{y, Charge level: {w%d{y, Spell: ",
		obj->value[2], obj->value[0] );

	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
		strcat( buf, "'{w" );
		strcat( buf, skill_table[obj->value[3]].name );
		strcat( buf, "{y'" );
	    }

	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );
	    break;

	case ITEM_DRINK_CON:
	    sprintf( buf, "It holds {w%s{y-colored {w%s{y.",
		liq_table[obj->value[2]].liq_color,
		liq_table[obj->value[2]].liq_name );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );
	    break;

	case ITEM_CONTAINER:

	case ITEM_WEAPON:
	    sprintf( buf, "Weapon type is %s.",
		flag_string( weapon_class, obj->value[0] ) );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );

	    sprintf( buf, "Damage is {w%d {yd {w%d {y(average {w%d{y).",
		obj->value[1], obj->value[2],
		( 1 + obj->value[2] ) * obj->value[1] / 2 );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );

	    sprintf( buf, "Damage message: {w%s",
		attack_table[obj->value[3]].noun );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );

	    if ( obj->value[4] )
	    {
		sprintf( buf, "Flags: {w%s",
		    flag_string( weapon_type2, obj->value[4] ) );
		sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
		add_buf( output, final );
	    }
	    break;

	case ITEM_ARMOR:
	    sprintf( buf, "Armor Values: Pierce: {w%d{y, Bash: {w%d{y, Slash: {w%d{y, Magic: {w%d",
		obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
	    add_buf( output, final );
	    break;
    }

    if ( !obj->enchanted )
    {
	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	{
	    if ( IS_IMMORTAL( ch )
	    ||   ( paf->location != APPLY_NONE && paf->modifier != 0 ) )
	    {
/*		if ( paf->where == TO_DAM_MODS )
		{
		    sprintf( buf, "Modifies damage from {w%s {yby {w%d{y%%",
			paf->location == DAM_ALL ? "all" :
			damage_mod_table[paf->location].name, paf->modifier );
		    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
		    add_buf( output, final );
		} else { */
		    sprintf( buf, "Affects {w%s {yby {w%d{y",
			flag_string( apply_flags, paf->location ), paf->modifier );

		    if ( paf->bitvector )
		    {
			switch( paf->where )
			{
			    case TO_AFFECTS:
				sprintf( final, " with {w%s {yaffect.",
				    flag_string( affect_flags, paf->bitvector ) );
				break;

			    case TO_OBJECT:
				sprintf( final, " with {w%s {yobject flag.",
				    flag_string( extra_flags, paf->bitvector ) );
				break;

			    default:
				sprintf( final, " with unknown bit {w%d{y: %d\n\r",
				    paf->where, paf->bitvector );
				break;
			}
		    }

		    else
			sprintf( final, "." );

		    strcat( buf, final );
		    sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
		    add_buf( output, final );

//		}
	    }
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( IS_IMMORTAL( ch )
	||   ( paf->location != APPLY_NONE && paf->modifier != 0 ) )
	{
/*	    if ( paf->where == TO_DAM_MODS )
	    {
		sprintf( buf, "Modifies damage from {w%s {yby {w%d{y%%",
		    paf->location == DAM_ALL ? "all" :
		    damage_mod_table[paf->location].name, paf->modifier );
		sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
		add_buf( output, final );
	    } else { */
		sprintf( buf, "Affects {w%s {yby {w%d{y",
		    flag_string( apply_flags, paf->location ), paf->modifier );

		if ( paf->bitvector )
		{
		    switch( paf->where )
		    {
			case TO_AFFECTS:
			    sprintf( final, " with {w%s {yaffect",
				flag_string( affect_flags, paf->bitvector ) );
			    break;

			case TO_OBJECT:
			    sprintf( final, " with {w%s {yobject flag",
				flag_string( extra_flags, paf->bitvector ) );
			    break;

			case TO_WEAPON:
			    sprintf( final, " with {w%s {yweapon flag",
				flag_string( weapon_type2, paf->bitvector ) );
			    break;

			default:
			    sprintf( final, " with unknown bit {w%d{y: %d",
				paf->where, paf->bitvector );
			    break;
		    }

		    strcat( buf, final );
		}

		if ( paf->duration > -1 )
		    sprintf( final, " for {w%d {yhours.", paf->duration );
		else
		    sprintf( final, "." );
		strcat( buf, final );

		sprintf( final, "| {y%s {B|\n\r", end_string( buf, 75 ) );
		add_buf( output, final );
//	    }
	}
    }

    sprintf( buf, "( {x%s {B)", obj->short_descr );
    str_replace( buf, "{x", "{w" );
    str_replace( buf, "{0", "{w" );

    place = FALSE;
    while ( strlen_color( buf ) < 77 )
    {
	if ( place )
	    strcat( buf, "=" );
	else
	{
	    sprintf( final, "=%s", buf );
	    strcpy( buf, final );
	}
	place = !place;
    }

    sprintf( final, "{B %s{x\n\r", buf );
    add_buf( output, final );

    if ( contents )
    {
	if ( obj->item_type == ITEM_CONTAINER
	&&   IS_SET( obj->value[1], CONT_CLOSED ) )
	{
	    sprintf( buf, "\n\r{x%s {+appears to be closed.\n\r",
		obj->short_descr );
	    add_buf( output, buf );
	}

	if ( ( obj->item_type == ITEM_CONTAINER
	&&     !IS_SET( obj->value[1], CONT_CLOSED ) )
	||   obj->item_type == ITEM_CORPSE_NPC
	||   obj->item_type == ITEM_CORPSE_PC )
	{
	    BUFFER *stats = show_list_to_char( obj->contains, ch, TRUE, TRUE );

	    sprintf( buf, "\n\r{x%s {+holds{w:{x\n\r", obj->short_descr );
	    add_buf( output, buf );

	    add_buf( output, stats->string );
	    free_buf( stats );
	}
    }

    return output;
}

const char * cost_string[MAX_AUCTION_PAYMENT] =
{
    "silver",
    "gold",
    "platinum",
    "aquest points",
    "iquest points"
};

sh_int cost_lookup( char *argument )
{
    sh_int stat;

    for ( stat = 0; stat < MAX_AUCTION_PAYMENT; stat++ )
    {
	if ( !str_prefix( argument, cost_string[stat] ) )
	    return stat;
    }

    return -1;
}

bool can_cover_bid( CHAR_DATA *ch, int type, int amount )
{
    switch( type )
    {
	case VALUE_SILVER:
	    if ( ch->silver + ( 100 * ch->gold ) + ( 10000 * ch->platinum ) < amount )
		return FALSE;
	    break;

	case VALUE_GOLD:
	    if ( ch->gold + ( 100 * ch->platinum ) < amount )
		return FALSE;
	    break;

	case VALUE_PLATINUM:
	    if ( ch->platinum < amount )
		return FALSE;
	    break;

	case VALUE_AQUEST_POINT:
	    send_to_char( "Bidding with quest points currently disabled.\n\r", ch );
	    return FALSE;
//	    if ( ch->questpoints < amount )
//		return FALSE;
//	    break;

	case VALUE_IQUEST_POINT:
	    if ( ch->questpoints < amount )
		return FALSE;
	    break;

	default:
	    bug( "can_cover_bid: invalid bid type.", 0 );
	    return FALSE;
    }

    return TRUE;
}


AUCTION_DATA * auction_lookup( int ticket )
{
    AUCTION_DATA *auction;

    for ( auction = auction_list; auction != NULL; auction = auction->next )
    {
	if ( auction->slot == ticket )
	    return auction;
    }

    return NULL;
}

void auction_channel( CHAR_DATA *ch, OBJ_DATA *obj, char *msg )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    sprintf( buf, "{+<{WAUCTION{+> %s{x", msg );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected == CON_PLAYING && victim
	&&   !IS_SET(victim->comm,COMM_NOAUCTION)
	&&   !IS_SET(victim->comm,COMM_QUIET) )
	    act_new( buf, victim, obj, ch, TO_CHAR, POS_DEAD );
    }
}

void do_auction( CHAR_DATA *ch, char * argument )
{
    AUCTION_DATA *auc, *pos_auc;
    OBJ_DATA *obj;
    char arg1[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    sh_int found = 0;

    if ( ch == NULL || IS_NPC( ch ) )
	return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	if ( IS_SET( ch->comm, COMM_NOAUCTION ) )
        REMOVE_BIT (ch->comm, COMM_NOAUCTION);

//	    send_to_char( "Auction channel enabled.\n\r", ch );
//	else

	send_to_char( "Auction [info, list, show, items].\n\r", ch );
//	ch->comm = COMM_AUCTION;
	return;
    }

    if ( IS_SET(ch->comm, COMM_NOAUCTION) || IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char("You can't use the auction command with quiet or noauction set.\n\r",ch);
	return;
    }

    if ( !str_prefix( arg1, "info" ) || !str_prefix( arg1, "show" )
    ||   !str_prefix( arg1, "list" ) || !str_prefix( arg1, "items" ) )
    {
	BUFFER *final, *stats;
	char seller[MAX_INPUT_LENGTH];

	if ( arg2[0] == '\0' )
	{
	    if ( auction_list == NULL )
	    {
		send_to_char("There is currently nothing up for auction.\n\r",ch);
		return;
	    }

	    final = new_buf();

	    add_buf(final,"{C -{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{r( {RAUCTION {r){C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C- \n\r"
			  "{C| {c*************************************************************************** {C|\n\r"
			  "{C| {c* {wNu {c*   {wSeller   {c*            {wItem            {c*{wMn/Cr Bid{c*   {wBidder   {c* {wT {c* {C|\n\r"
			  "{C| {c*************************************************************************** {C|\n\r");

	    for ( auc = auction_list; auc != NULL; auc = auc->next )
	    {
		strcpy( seller, begin_string(PERS(auc->owner,ch),10) );

		sprintf(buf,"{C| {c* {w%2d {c* {w%s {c* {w%s {c*{w%7d {R%c{c* {w%s {c* {w%d {c* {C|\n\r",
		    auc->slot, seller, can_see_obj( ch, auc->item ) ?
		    end_string(auc->item->short_descr,26) :
		    "Something                 ",
		    auc->bid_amount, UPPER( *cost_string[auc->bid_type] ),
		    auc->high_bidder == NULL ? "----------" :
		    begin_string(PERS(auc->high_bidder,ch),10),
		    (AUCTION_LENGTH - auc->status) );
		add_buf( final, buf );
	    }

	    add_buf(final,"{C| {c*************************************************************************** {C|\n\r"
			  "{C -{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{r( {RAUCTION {r){C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{c={C-{x \n\r");

	    page_to_char(final->string,ch);
	    free_buf(final);
	    return;
	}

	if ( !is_number(arg2) )
	{
	    send_to_char("Syntax: auction info <ticket>.\n\r",ch);
	    return;
	}

	if ( ( auc = auction_lookup( atoi( arg2 ) ) ) == NULL )
	{
	    send_to_char("Invalid auction ticket.\n\r",ch);
	    return;
	}

	if ( !auc->item )
	{
	    send_to_char("Error, NULL object.\n\r",ch);
	    return;
	}

	else
	{
	    final = new_buf();

	    if ( !can_see_obj( ch, auc->item )
	    ||   ( !IS_IMMORTAL( ch ) && ch == auc->owner ) )
	    {
		sprintf( buf, "{+The current bid is {Y%d {+%s by %s{+.{x\n\r\n\r",
		    auc->bid_amount, cost_string[auc->bid_type],
		    auc->high_bidder == NULL ? "no one" :
		    PERS(auc->high_bidder,ch) );
		add_buf(final,buf);
	    } else {
		sprintf( buf, "{+The current bid is {Y%d {+%s by %s{+.{x\n\r\n\r",
		    auc->bid_amount, cost_string[auc->bid_type],
		    auc->high_bidder == NULL ? "no one" :
		    PERS(auc->high_bidder,ch) );
		add_buf(final,buf);

		stats = display_stats( auc->item, ch, TRUE );
		add_buf(final,stats->string);
		free_buf(stats);
	    }

	    page_to_char(final->string,ch);
	    free_buf(final);
	}
 	return;
    }

    if ( !str_cmp( arg1, "stop" ) )
    {
	if ( arg2[0] == '\0' || !is_number(arg2) )
	{
	    if ( !str_cmp( arg2, "all" ) )
	    {
		for ( auc = auction_list; auc != NULL; auc = pos_auc )
		{
		    pos_auc = auc->next;

		    sprintf( buf, "stop %d", auc->slot );
		    do_auction( ch, buf );
		}

		return;
	    }

	    send_to_char("Syntax: auction stop <[ticket #] / all>.\n\r",ch);
	    return;
	}

	if ( ( auc = auction_lookup( atoi(arg2) ) ) == NULL )
	{
	    send_to_char("That is not a valid auction ticket number.\n\r",ch);
	    return;
	}

	if ( !IS_IMMORTAL( ch ) && auc->owner != ch )
	{
	    send_to_char( "You may only stop your own auctions.\n\r", ch );
	    return;
	}

	sprintf(buf,"{+Ticket [{Y%d{+], {x$p{+, has been stopped by $N.{x",
	    auc->slot );
	auction_channel( ch, auc->item, buf );

	act_new("You stop the auction of $p.",
	    ch,auc->item,NULL,TO_CHAR, POS_DEAD);
	obj_to_char(auc->item,ch);

        if ( auc->high_bidder != NULL )
        {
	    add_cost( auc->high_bidder, auc->bid_amount, auc->bid_type );
	    send_to_char("Your bid has been returned to you.\n\r",
		auc->high_bidder);
        }

	free_auction( auc );

	return;
    }


    if ( !str_prefix( arg1, "bid" ) )
    {
	int bid;

	if ( arg2[0] == '\0' || !is_number(arg2)
	||   argument[0] == '\0' || !is_number( argument ) )
	{
	    send_to_char("Syntax: auction bid <ticket> <amount>.\n\r",ch);
	    return;
	}

	if ( ( auc = auction_lookup( atoi( arg2 ) ) ) == NULL )
	{
	    send_to_char("Invalid auction ticket.\n\r",ch);
	    return;
	}

        if ( auc->item == NULL )
        {
            send_to_char("Error, NULL Object!\n\r",ch);
            return;
        }

	if ( !can_see_obj( ch, auc->item ) )
	{
	    send_to_char("{+You can't see that item.{x\n\r",ch);
	    return;
	}

	if ( ch == auc->high_bidder )
	{
	    send_to_char("{+You already have the highest bid.{x\n\r",ch);
	    return;
	}

	if ( ch == auc->owner )
	{
	    send_to_char("{+Why bid on something you auctioned?{x\n\r",ch);
	    return;
	}
/*
	if ( !str_cmp( ch->pcdata->socket, auc->owner->pcdata->socket ) )
	{
	    send_to_char("{+You can not bid on your own auctions with your own characters.{x\n\r", ch );
	    return;
	}
*/
	bid = atoi( argument );

	if ( bid < auc->bid_amount || bid <= 0
	||   ( auc->high_bidder != NULL && bid == auc->bid_amount ) )
	{
	    sprintf( buf, "{+You must bid above the current bid of {Y%d {+%s.{x\n\r",
		auc->bid_amount, cost_string[auc->bid_type] );
            send_to_char( buf, ch );
	    return;
	}

	if ( !can_cover_bid( ch, auc->bid_type, bid ) )
	{
	    send_to_char( "{+You can not cover that bid.{x\n\r", ch );
	    return;
	}

	if ( ch->carry_number + get_obj_number(auc->item) > can_carry_n(ch) )
	{
	    send_to_char("You can't carry that many items.\n\r",ch);
	    return;
	}

	if ( get_carry_weight(ch) + get_obj_weight(auc->item) > can_carry_w(ch) )
	{
	    send_to_char("You can't carry that much weight.\n\r",ch);
	    return;
	}

	sprintf(buf, "{+$N {+bids {Y%d {+%s for ticket [{Y%d{+], {x$p{+.{x",
	    bid, cost_string[auc->bid_type], auc->slot );
	auction_channel( ch, auc->item, buf );

	if ( auc->high_bidder != NULL )
	    add_cost( auc->high_bidder, auc->bid_amount, auc->bid_type );

	deduct_cost( ch, bid );

	auc->high_bidder	= ch;
	auc->bid_amount		= bid;
	auc->status	 	= 0;
	return;
    }

    if ( (obj = get_obj_carry( ch, arg1, ch )) == NULL )
    {
	send_to_char("{+You aren't carrying that item.{x\n\r",ch);
	return;
    }

    if ( !can_drop_obj(ch,obj) )
    {
	send_to_char("{+You can't let go of that item.{x\n\r",ch);
	return;
    }

    if ( atoi(arg2) < 0 )
    {
	send_to_char("{+Set the minimum just a little higher.{x\n\r",ch);
	return;
    }

    switch ( obj->item_type )
    {
	default:
	    break;

	case ITEM_CORPSE_PC:
	case ITEM_CORPSE_NPC:
	    send_to_char("You can not auction that!\n\r",ch);
	    return;
    }

    if ( !IS_IMMORTAL( ch ) )
    {
	for ( auc = auction_list; auc != NULL; auc = auc->next )
	{
	    if ( auc->owner == ch )
	    {
		if ( ++found >= 5 )
		{
		    send_to_char( "Max auctions per person reached.\n\r", ch );
		    return;
		}
	    }
	}
    }

    if ( argument[0] != '\0' )
    {
	if ( ( found = cost_lookup( argument ) ) == -1 )
	{
	    send_to_char( "Minbid must be one of: silver, gold, platinum or iquest point.\n\r", ch );
	    return;
	}
    }
    else
	found = VALUE_GOLD;

    auction_ticket++;

    auc = new_auction();

    if ( arg2[0] == '\0' )
	auc->bid_amount = 0;
    else
	auc->bid_amount = atoi( arg2 );

    auc->owner		= ch;
    auc->item		= obj;
    auc->slot		= auction_ticket;
    auc->bid_type	= found;

    sprintf(buf,"{+[{Y%d{+] $N {+is now taking bids on{x $p{+. < {Y%d{+ %s >{x",
	auc->slot, auc->bid_amount, cost_string[auc->bid_type] );
    auction_channel( ch, auc->item, buf );

    obj_from_char( obj );

    if ( auction_list == NULL )
	auction_list = auc;
    else
    {
	for ( pos_auc = auction_list; pos_auc != NULL; pos_auc = pos_auc->next )
	{
	    if ( pos_auc->next == NULL )
	    {
		pos_auc->next = auc;
		break;
	    }
	}
    }
}

void auction_update( )
{
    AUCTION_DATA *auc, *auc_next;
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    for ( auc = auction_list; auc != NULL; auc = auc_next )
    {
	auc_next = auc->next;

	if ( auc->item == NULL )
	    continue;

	if ( ++auc->status == AUCTION_LENGTH )
	{
	    if ( auc->high_bidder == NULL )
	    {
		auction_channel( auc->owner, auc->item,
		    "{+No bids on{x $p {+- item removed.{x" );
		obj_to_char( auc->item, auc->owner );

		free_auction( auc );
	    } else {
		sprintf(buf,"{x$p {+SOLD to $N {+for {Y%d {+%s.{x",
		    auc->bid_amount, cost_string[auc->bid_type] );
		auction_channel( auc->high_bidder, auc->item, buf );

		add_cost(auc->owner,auc->bid_amount,auc->bid_type);

		sprintf(buf,"You sell $p for %d %s.",
		    auc->bid_amount, cost_string[auc->bid_type] );
		act_new( buf, auc->owner, auc->item, NULL, TO_CHAR, POS_DEAD );

		// Added by Xrakisis
        if (obj->questowner != NULL)
                free_string(obj->questowner);
        obj->questowner = str_dup(victim->name);
        obj->ownerid = victim->pcdata->playerid;
		obj_to_char( auc->item, auc->high_bidder );


		act_new("{x$p {+appears in your hands.{x",
		    auc->high_bidder, auc->item, NULL, TO_CHAR, POS_DEAD );

		free_auction( auc );
	    }
	} else {
	    if ( auc->high_bidder == NULL )
	    {
		if ( auc->status == AUCTION_LENGTH - 1 )
		    auction_channel( auc->owner, auc->item,
			"{+No bids on{x $p {+- item going twice.{x" );

		else if ( auc->status == AUCTION_LENGTH - 2 )
		    auction_channel( auc->owner, auc->item,
			"{+No bids on{x $p {+- item going once.{x" );
	    } else {
		if ( auc->status == AUCTION_LENGTH - 1 )
		{
		    sprintf(buf, "{x$p {+- going twice at {Y%d {+%s to $N{+.{x",
			auc->bid_amount, cost_string[auc->bid_type] );
		    auction_channel( auc->high_bidder, auc->item, buf );
		}
		else if ( auc->status == AUCTION_LENGTH - 2 )
		{
		    sprintf(buf, "{x$p {+- going once at {Y%d {+%s to $N{+.{x",
			auc->bid_amount, cost_string[auc->bid_type] );
		    auction_channel( auc->high_bidder, auc->item, buf );
		}
	    }
	}
    }
}

