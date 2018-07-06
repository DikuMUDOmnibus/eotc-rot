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
 **************************************************************************/

/***************************************************************************
 *   ROM 2.4 is copyright 1993-1998 Russ Taylor                            *
 *   ROM has been brought to you by the ROM consortium                     *
 *       Russ Taylor (rtaylor@hypercube.org)                               *
 *       Gabrielle Taylor (gtaylor@hypercube.org)                          *
 *       Brian Moore (zump@rom.org)                                        *
 *   By using this code, you have agreed to follow the terms of the        *
 *   ROM license, in the file Rom24/doc/rom.license                        *
 **************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"


const struct pc_eyes pc_eye_table [] = 
{
	{"null"},
	{"hazel"},
	{"red"},
	{"blue"},
	{"green"},
	{"blue-green"},
	{"light green"},
	{"dark green"},
	{"light brown"},
        {"dark brown"},
	{"black as coal"}
};

const struct pc_looks pc_looks_table [] =
{
        {"null"},
        {"has albino skin, as white as snow"},
        {"has pale skin"},
        {"is slightly tanned"},
        {"has tanned skin, with battle scars"},
        {"has a dark complextion"}
};

const struct pc_build pc_build_table [] =
{
	{"null"},
	{"lean and lanky"},
	{"broad with a protruding beer belly"},
	{"stocky"},
	{"althleticly built"}
};

const struct pc_hair pc_hair_table [] =
{
	{"null"},
        {"a bald head"},
        {"short white hair"},
        {"long white hair"},
        {"short blond hair"},
        {"long blond hair"},
        {"short red hair"},
        {"long red hair"},
        {"short brown hair"},
        {"long brown hair"},
        {"short black hair"},
        {"long black hair"}
};

/* item type list */
const struct item_type item_table[] = {
    {ITEM_LIGHT, "light"},
    {ITEM_SCROLL, "scroll"},
    {ITEM_WAND, "wand"},
    {ITEM_STAFF, "staff"},
    {ITEM_WEAPON, "weapon"},
    {ITEM_TREASURE, "treasure"},
    {ITEM_ARMOR, "armor"},
    {ITEM_POTION, "potion"},
    {ITEM_CLOTHING, "clothing"},
    {ITEM_FURNITURE, "furniture"},
    {ITEM_TRASH, "trash"},
    {ITEM_CONTAINER, "container"},
    {ITEM_DRINK_CON, "drink"},
    {ITEM_KEY, "key"},
    {ITEM_FOOD, "food"},
    {ITEM_MONEY, "money"},
    {ITEM_BOAT, "boat"},
    {ITEM_CORPSE_NPC, "npc_corpse"},
    {ITEM_CORPSE_PC, "pc_corpse"},
    {ITEM_FOUNTAIN, "fountain"},
    {ITEM_PILL, "pill"},
    {ITEM_PROTECT, "protect"},
    {ITEM_MAP, "map"},
    {ITEM_PORTAL, "portal"},
    {ITEM_WARP_STONE, "warp_stone"},
    {ITEM_ROOM_KEY, "room_key"},
    {ITEM_GEM, "gem"},
    {ITEM_JEWELRY, "jewelry"},
    {ITEM_SMITH_FURNACE, "furnace"},
    {ITEM_SMITH_ORE, "ore"},
    {ITEM_SMITH_ANVIL, "anvil"},
    {ITEM_SMITH_HAMMER, "hammer"},
    {ITEM_SMITH_BARREL, "barrel"},
    {ITEM_SMITH_GRINDER, "grinder"},
    {0, NULL}
};


/* weapon selection table */
const struct weapon_type weapon_table[] = {
    {"sword", OBJ_VNUM_SCHOOL_SWORD, WEAPON_SWORD, &gsn_sword},
    {"mace", OBJ_VNUM_SCHOOL_MACE, WEAPON_MACE, &gsn_mace},
    {"dagger", OBJ_VNUM_SCHOOL_DAGGER, WEAPON_DAGGER, &gsn_dagger},
    {"axe", OBJ_VNUM_SCHOOL_AXE, WEAPON_AXE, &gsn_axe},
    {"staff", OBJ_VNUM_SCHOOL_STAFF, WEAPON_SPEAR, &gsn_spear},
    {"flail", OBJ_VNUM_SCHOOL_FLAIL, WEAPON_FLAIL, &gsn_flail},
    {"whip", OBJ_VNUM_SCHOOL_WHIP, WEAPON_WHIP, &gsn_whip},
    {"polearm", OBJ_VNUM_SCHOOL_POLEARM, WEAPON_POLEARM, &gsn_polearm},
    {NULL, 0, 0, NULL}
};



/* wiznet table and prototype for future flag setting */
const struct wiznet_type wiznet_table[] = {
    {"on", WIZ_ON, IM},
    {"prefix", WIZ_PREFIX, IM},
    {"ticks", WIZ_TICKS, IM},
    {"logins", WIZ_LOGINS, IM},
    {"sites", WIZ_SITES, L4},
    {"links", WIZ_LINKS, L7},
    {"newbies", WIZ_NEWBIE, IM},
    {"spam", WIZ_SPAM, L5},
    {"deaths", WIZ_DEATHS, IM},
    {"resets", WIZ_RESETS, L4},
    {"mobdeaths", WIZ_MOBDEATHS, L4},
    {"flags", WIZ_FLAGS, L5},
    {"penalties", WIZ_PENALTIES, L5},
    {"saccing", WIZ_SACCING, L5},
    {"levels", WIZ_LEVELS, IM},
    {"load", WIZ_LOAD, L2},
    {"restore", WIZ_RESTORE, L2},
    {"snoops", WIZ_SNOOPS, L2},
    {"switches", WIZ_SWITCHES, L2},
    {"secure", WIZ_SECURE, L1},
    {NULL, 0, 0}
};


const struct god_type god_table[]=
{
	{ "" },
	{ "Paladine" },
	{ "Astinus" },
	{ "Chemosh" },
	{ "Hiddukel" },
	{ "Nuitari" },
	{ "Sargonnas" },
	{ "Zeboim" },
	{ "Branchala" },
	{ "Habbakuk" },
	{ "Kiri-Jolith" },
	{ "Majere" },
	{ "Mishakal" },
	{ "Solinari" },
	{ "Chislev" },
	{ "Gilean" },
	{ "Lunitari" },
	{ "Reorx" },
	{ "Shinare" },
	{ "Sirrion" },
	{ "Zivilyn" },
	{ "Takhisis" },
	{ "" }
};

/* attack table  -- not very organized :( */
const struct attack_type attack_table[MAX_DAMAGE_MESSAGE] = {
    {"none", "hit", -1},        /*  0 */
    {"slice", "slice", DAM_SLASH},
    {"stab", "stab", DAM_PIERCE},
    {"slash", "slash", DAM_SLASH},
    {"whip", "whip", DAM_SLASH},
    {"claw", "claw", DAM_SLASH},    /*  5 */
    {"blast", "blast", DAM_BASH},
    {"pound", "pound", DAM_BASH},
    {"crush", "crush", DAM_BASH},
    {"grep", "grep", DAM_SLASH},
    {"bite", "bite", DAM_PIERCE},    /* 10 */
    {"pierce", "pierce", DAM_PIERCE},
    {"suction", "suction", DAM_BASH},
    {"beating", "beating", DAM_BASH},
    {"digestion", "digestion", DAM_ACID},
    {"charge", "charge", DAM_BASH},    /* 15 */
    {"slap", "slap", DAM_BASH},
    {"punch", "punch", DAM_BASH},
    {"wrath", "wrath", DAM_ENERGY},
    {"magic", "magic", DAM_ENERGY},
    {"divine", "divine power", DAM_HOLY},    /* 20 */
    {"cleave", "cleave", DAM_SLASH},
    {"scratch", "scratch", DAM_PIERCE},
    {"peck", "peck", DAM_PIERCE},
    {"peckb", "peck", DAM_BASH},
    {"chop", "chop", DAM_SLASH},    /* 25 */
    {"sting", "sting", DAM_PIERCE},
    {"smash", "smash", DAM_BASH},
    {"shbite", "shocking bite", DAM_LIGHTNING},
    {"flbite", "flaming bite", DAM_FIRE},
    {"frbite", "freezing bite", DAM_COLD},    /* 30 */
    {"acbite", "acidic bite", DAM_ACID},
    {"chomp", "chomp", DAM_PIERCE},
    {"drain", "life drain", DAM_NEGATIVE},
    {"thrust", "thrust", DAM_PIERCE},
    {"slime", "slime", DAM_ACID},
    {"shock", "shock", DAM_LIGHTNING},
    {"thwack", "thwack", DAM_BASH},
    {"flame", "flame", DAM_FIRE},
    {"chill", "chill", DAM_COLD},
    {NULL, NULL, 0}
};

/* Tier Table */
const struct tier_type tier_table[] = {
/*
	{
	tiernumber, 
	aff_by_bit,
	imm, res, vuln,
	- roundtime, + spellpower,
	}
*/
        {
        "Tier0",
        0,
	0,
        0,
        0,0},

	{
	"Tier1",
	AFF_INFRARED,
	IMM_CHARM,
	RES_DISEASE,
	1,5},

        {
        "Tier2",
        AFF_INFRARED | AFF_PASS_DOOR,
        IMM_CHARM,
        RES_DISEASE | RES_SUMMON,
        1,10},

        {
        "Tier3",
        AFF_INFRARED | AFF_PASS_DOOR | AFF_HASTE,
        IMM_CHARM | IMM_POISON,
        RES_DISEASE | RES_SUMMON | RES_IRON,
        2,15},

        {
        "Tier4",
        AFF_INFRARED | AFF_PASS_DOOR | AFF_HASTE,
        IMM_CHARM | IMM_POISON,
        RES_DISEASE | RES_SUMMON | RES_IRON | RES_BASH,
        2,20},

        {
        "Tier5",
        AFF_INFRARED | AFF_PASS_DOOR | AFF_HASTE | AFF_SANCTUARY,
        IMM_CHARM | IMM_POISON | IMM_DISEASE,
        RES_DISEASE | RES_SUMMON | RES_IRON | RES_BASH | RES_ACID,
        3,25},

        {
        "Tier6",
        AFF_INFRARED | AFF_PASS_DOOR | AFF_HASTE | AFF_SANCTUARY | AFF_REGENERATION,
        IMM_CHARM | IMM_POISON | IMM_DISEASE,
        RES_DISEASE | RES_SUMMON | RES_IRON | RES_BASH | RES_ACID | RES_MAGIC,
        3,30},

        {
        "Tier7",
        AFF_INFRARED | AFF_PASS_DOOR | AFF_HASTE | AFF_SANCTUARY | AFF_REGENERATION,
        IMM_CHARM | IMM_POISON | IMM_DISEASE,
        RES_DISEASE | RES_SUMMON | RES_IRON | RES_BASH | RES_ACID | RES_MAGIC | RES_WEAPON,
        4,35}
};

     



/* race table */
const struct race_type race_table[] = {
/*
    {
    name,        pc_race?,
    act bits,    aff_by bits,    off bits,
    imm,        res,        vuln,
    form,        parts 
    },
*/
    {"unique", FALSE, 0, 0, 0, 0, 0, 0, 0, 0},

	/* 1 */
    {
     "human", TRUE,
     0, 0, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 2 */
    {
     "elf", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_CHARM, VULN_IRON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 3 */
    {
     "dwarf", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_POISON | RES_DISEASE, VULN_DROWNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 4 */
    {
     "giant", TRUE,
     0, 0, 0,
     0, RES_FIRE | RES_COLD | RES_BASH | RES_WEAPON | RES_DISEASE,
		 VULN_MENTAL | VULN_LIGHTNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 5 */
    {
     "draconian", TRUE,
     0, 0, 0,
     0, RES_FIRE | RES_COLD, RES_WEAPON | VULN_MAGIC,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 6 */
    {
     "gnome", TRUE,
     0, 0, 0,
     0, IMM_MENTAL, VULN_WEAPON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 7 */
    {
     "hobbit", TRUE,
     0, 0, 0,
     0, VULN_BASH, VULN_POISON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 8 */
    {
     "kender", TRUE,
     0, 0, 0,
     0, VULN_BASH, VULN_POISON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 9 */
    {
     "troll", TRUE,
     0, 0, 0,
     0, VULN_FIRE, RES_WEAPON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 10 */
    {
     "pixie", TRUE,
     0, 0, 0,
     0, VULN_BASH, VULN_POISON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    { /* 11 */
     "half-elf", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_CHARM, VULN_IRON,
     A|H|M|V,A|B|C|D|E|F|G|H|I|J|K
    },

	/* 12 */
    {
     "half-giant", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_POISON | RES_DISEASE, VULN_DROWNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 13 */
    {
     "half-orc", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_POISON | RES_DISEASE, VULN_DROWNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 14 */
    {
     "duergar", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_POISON | RES_DISEASE, VULN_DROWNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 15 */
    {
     "minotaur", TRUE,
     0, 0, 0,
     0, RES_FIRE | RES_COLD, VULN_MENTAL | VULN_LIGHTNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 16 */
    {
     "centaur", TRUE,
     0, 0, 0,
     0, RES_FIRE | RES_COLD, VULN_MENTAL | VULN_LIGHTNING,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 17 */
    {
     "drow", TRUE,
     0, AFF_INFRARED, 0,
     0, RES_CHARM, VULN_IRON,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

	/* 18 */
    { 
     "storm giant",  TRUE,
     0, AFF_FLYING, 0,
     0, RES_LIGHTNING | RES_COLD | RES_BASH | RES_WEAPON | RES_DISEASE,
	   VULN_MENTAL|VULN_COLD|VULN_FIRE,
     A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 19 */
    "cloud giant",  TRUE,
    0, AFF_FLYING, 0,
    0, RES_FIRE | RES_COLD | RES_BASH | RES_WEAPON | RES_DISEASE,
		VULN_MENTAL,
    A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 20 */
    "fire giant",   TRUE,
    0, 0, 0,
    0, RES_FIRE | RES_BASH | RES_WEAPON | RES_DISEASE,
		VULN_MENTAL|VULN_LIGHTNING|VULN_COLD,
    A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 21 */
    "frost giant",  TRUE,
    0, 0, 0,
    0, RES_COLD | RES_BASH | RES_WEAPON | RES_DISEASE,
		VULN_MENTAL|VULN_LIGHTNING|VULN_FIRE,
    A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 22 */
    "cyclops",  TRUE, 
    0, 0, 0,
    0, 0, VULN_MENTAL|VULN_MAGIC,
    A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 23 */
    "hydra",  TRUE, 
    0, 0, 0,
    0, RES_BASH, VULN_MENTAL|VULN_MAGIC,
    A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 24 */
        "rockseer",     TRUE, 
        0,              AFF_INFRARED|AFF_SNEAK|AFF_PASS_DOOR, 0,
        0,              RES_CHARM,      VULN_WOOD,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {  /* 25 */
      "svirfnebli",     TRUE,
      0,                AFF_INFRARED,   0,
      0,                RES_MAGIC|RES_POISON|RES_DISEASE, VULN_BASH,
      A|H|M|V,          A|B|C|D|E|F|G|H|I|J|K
    },
    { /* 26 */
        "arial",        TRUE,
        0,              AFF_FLYING,             0,
        0,              0,              0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },
    { /* 27 */
        "felar",        TRUE, 	
        0,              AFF_INFRARED,   OFF_TAIL,
        0,              RES_LIGHT|RES_COLD,     VULN_FIRE|VULN_DROWNING,
        A|H|M|V,        A|C|D|E|F|H|J|K|Q|U|V
    },

    { /* 28 */
	"githyanki",	TRUE,  	
	0,		AFF_INFRARED,	0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 29 */
	"satyr",	TRUE, 	
	0,		AFF_INFRARED,	0,
	0,		RES_WOOD|RES_DISEASE,	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 30 */
        "black dragon", TRUE,  
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_ACID,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 31 */
        "blue dragon", 	TRUE, 	
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_LIGHTNING, VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 32 */
        "green dragon", TRUE, 	
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_POISON,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 33 */
        "red dragon", 	TRUE, 	
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 34 */
        "white dragon", TRUE, 	
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_COLD,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 35 */
        "brass dragon", TRUE, 	
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 36 */
        "gold dragon", 	TRUE, 	
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_FIRE|RES_POISON, 
	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 37 */
        "silver dragon",TRUE, 	
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_COLD,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 38 */
        "bronze dragon", TRUE, 	 
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_LIGHTNING, 
	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    { /* 39 */
        "copper dragon",TRUE, 	
	0, 		AFF_SANCTUARY|AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_BASH|RES_CHARM|RES_ACID,	VULN_PIERCE,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    
    {
     "bat", FALSE,
     0, AFF_FLYING | AFF_DARK_VISION, OFF_DODGE | OFF_FAST,
     0, 0, VULN_LIGHT,
     A | G | V, A | C | D | E | F | H | J | K | P},

    {
     "bear", FALSE,
     0, 0, OFF_CRUSH | OFF_DISARM | OFF_BERSERK,
     0, RES_BASH | RES_COLD, 0,
     A | G | V, A | B | C | D | E | F | H | J | K | U | V},

    {
     "cat", FALSE,
     0, AFF_DARK_VISION, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | Q | U | V},

    {
     "centipede", FALSE,
     0, AFF_DARK_VISION, 0,
     0, RES_PIERCE | RES_COLD, VULN_BASH,
     A | B | G | O, A | C | K},

    {
     "dog", FALSE,
     0, 0, OFF_FAST,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | U | V},

    {
     "doll", FALSE,
     0, 0, 0,
     IMM_COLD | IMM_POISON | IMM_HOLY | IMM_NEGATIVE | IMM_MENTAL |
     IMM_DISEASE | IMM_DROWNING, RES_BASH | RES_LIGHT,
     VULN_SLASH | VULN_FIRE | VULN_ACID | VULN_LIGHTNING | VULN_ENERGY,
     E | J | M | cc, A | B | C | G | H | K},

    {"dragon", FALSE,
     0, AFF_INFRARED | AFF_FLYING, 0,
     0, RES_FIRE | RES_BASH | RES_CHARM,
     VULN_PIERCE | VULN_COLD,
     A | H | Z, A | C | D | E | F | G | H | I | J | K | P | Q | U | V | X},

    {
     "fido", FALSE,
     0, 0, OFF_DODGE | ASSIST_RACE,
     0, 0, VULN_MAGIC,
     A | B | G | V, A | C | D | E | F | H | J | K | Q | V},

    {
     "fox", FALSE,
     0, AFF_DARK_VISION, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | Q | V},

    {
     "goblin", FALSE,
     0, AFF_INFRARED, 0,
     0, RES_DISEASE, VULN_MAGIC,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "hobgoblin", FALSE,
     0, AFF_INFRARED, 0,
     0, RES_DISEASE | RES_POISON, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K | Y},

    {
     "kobold", FALSE,
     0, AFF_INFRARED, 0,
     0, RES_POISON, VULN_MAGIC,
     A | B | H | M | V, A | B | C | D | E | F | G | H | I | J | K | Q},

    {
     "lizard", FALSE,
     0, 0, 0,
     0, RES_POISON, VULN_COLD,
     A | G | X | cc, A | C | D | E | F | H | K | Q | V},

    {
     "modron", FALSE,
     0, AFF_INFRARED, ASSIST_RACE | ASSIST_ALIGN,
     IMM_CHARM | IMM_DISEASE | IMM_MENTAL | IMM_HOLY | IMM_NEGATIVE,
     RES_FIRE | RES_COLD | RES_ACID, 0,
     H, A | B | C | G | H | J | K},

    {
     "orc", FALSE,
     0, AFF_INFRARED, 0,
     0, RES_DISEASE, VULN_LIGHT,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "pig", FALSE,
     0, 0, 0,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K},

    {
     "rabbit", FALSE,
     0, 0, OFF_DODGE | OFF_FAST,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K},

    {
     "school monster", FALSE,
     ACT_NOALIGN, 0, 0,
     IMM_CHARM | IMM_SUMMON, 0, VULN_MAGIC,
     A | M | V, A | B | C | D | E | F | H | J | K | Q | U},

    {
     "snake", FALSE,
     0, 0, 0,
     0, RES_POISON, VULN_COLD,
     A | G | X | Y | cc, A | D | E | F | K | L | Q | V | X},

    {
     "song bird", FALSE,
     0, AFF_FLYING, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | W, A | C | D | E | F | H | K | P},

    {
     "troll", FALSE,
     0, AFF_REGENERATION | AFF_INFRARED | AFF_DETECT_HIDDEN,
     OFF_BERSERK,
     0, RES_CHARM | RES_BASH, VULN_FIRE | VULN_ACID,
     A | B | H | M | V, A | B | C | D | E | F | G | H | I | J | K | U | V},

    {
     "water fowl", FALSE,
     0, AFF_SWIM | AFF_FLYING, 0,
     0, RES_DROWNING, 0,
     A | G | W, A | C | D | E | F | H | K | P},

    {
     "wolf", FALSE,
     0, AFF_DARK_VISION, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | V, A | C | D | E | F | J | K | Q | V},

    {
     "wyvern", FALSE,
     0, AFF_FLYING | AFF_DETECT_INVIS | AFF_DETECT_HIDDEN,
     OFF_BASH | OFF_FAST | OFF_DODGE,
     IMM_POISON, 0, VULN_LIGHT,
     A | B | G | Z, A | C | D | E | F | H | J | K | Q | V | X},

    {
     "unique", FALSE,
     0, 0, 0,
     0, 0, 0,
     0, 0},


    {
     NULL, 0, 0, 0, 0, 0, 0}
};

const struct pc_race_type pc_race_table[] = {
    {"null race", "", 0, {100, 100, 100, 100},
    70, 140,    {""}, {13, 13, 13, 13, 13}, {18, 18, 18, 18, 18}, 0},

/*
    {
    "race name",     short name,     points,    { class multipliers },
    "height", "weight", { bonus skills },
    { base stats },        { max stats },        size 
    },
*/
    {
     "human", "Human    ", 0, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     70, 140, {""},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
     "elf", "Elf      ", 20, {100, 125, 100, 120, 120, 120, 100, 120, 125, 100},
     72, 130, {"sneak", "hide"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
     "dwarf", "Dwarf    ", 20, {150, 100, 125, 100, 100, 100, 150, 100, 100, 130},
     40, 170, {"berserk"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
     "giant", "Giant    ", 25, {200, 150, 150, 105, 120, 120, 200, 120, 150, 175},
     120, 250, {"bash", "fast healing"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_LARGE},

    {
     "draconian", "Draconian", 20, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     80, 170, {""},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
     "gnome", "Gnome    ", 10, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     35, 80, {""},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_SMALL},

    {
     "hobbit", "Hobbit   ", 10, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     40, 150, {""},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_SMALL},

    {
     "kender", "Kender   ", 10, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     35, 75, {""},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_SMALL},

    {
     "troll", "Troll    ", 25, {200, 150, 150, 105, 100, 100, 200, 100, 150, 175},
     95, 220, {"bash", "fast healing"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_LARGE},

    {
     "pixie", "Pixie    ", 10, {100, 125, 100, 120, 120, 120, 100, 120, 125, 100},
     6, 1, {"sneak", "hide"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_SMALL},

    {
     "half-elf", "Half-elf ", 20, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     70, 140, {""},
     {13, 13, 13, 13, 13}, {18, 18, 18, 18, 18}, SIZE_MEDIUM},

    {
     "half-giant", "Hlf-Giant", 25, {125, 100, 125, 100, 100, 100, 125, 100, 100, 125},
     80, 220, {"berserk"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_LARGE},

    {
     "half-orc", "Half-Orc ", 20, {150, 100, 125, 100, 100, 100, 150, 100, 100, 125},
     70, 150, {"berserk"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
     "duergar", "Duergar  ", 20, {150, 100, 125, 100, 100, 100, 150, 100, 100, 130},
     40, 160, {"berserk"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
     "minotaur", "Minotaur ", 20, {200, 150, 150, 105, 100, 100, 200, 100, 150, 150},
     85, 230, {"bash", "fast healing"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_LARGE},

    {
     "centaur", "Centaur  ", 10, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     85, 260, {"berserk"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_LARGE},

    {
     "drow", "Dark Elf ", 20, {100, 125, 100, 120, 120, 120, 100, 120, 125, 100},
     80, 120, {"sneak", "hide"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
     "storm giant",  "StormGia ",30,{200, 150, 150, 105, 100, 100, 200, 100, 150, 150},
     168, 600, { "bash"," enhanced damage" , "sword" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_HUGE},
    
    {
     "cloud giant",  "CloudGia ", 30,{200, 150, 150, 105, 100, 100, 200, 100, 150, 150},
     168, 600, { "bash"," enhanced damage" , "sword" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_HUGE},

    {
        "fire giant",   "FireGia  ",30 ,{200, 150, 150, 105, 100, 100, 200, 100, 150, 150},
     168, 600, { "bash"," enhanced damage" , "sword" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_HUGE},

    {
        "frost giant",  "FrostGia ",30,{200, 150, 150, 105, 100, 100, 200, 100, 150, 150},
     168, 600, { "bash"," enhanced damage" , "sword" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_HUGE},
    
    {
        "cyclops",  "Cyclops  ",30,{200, 150, 150, 105, 100, 100, 200, 100, 150, 150},
     168, 600, { "bash"," enhanced damage" , "sword" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_HUGE},

    {
        "hydra",  "Hydra    ",20,{200, 150, 150, 105, 100, 100, 200, 100, 150, 150},
     168, 600, { "bash"," enhanced damage" , "sword" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_HUGE},

    {
        "rockseer","Rockseer ",20,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     70, 150,	{ "wand" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
	"svirfnebli",	"Svirf    ",20,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     50, 120,	{ "lore", "identify" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_SMALL},

    {
	"arial",	"Arial    ",10,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     70, 150,	{ "" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},
   {
	"felar",	"Felar    ",20,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     70, 150,{ "hand to hand" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},
    {
	"githyanki",	"Githy    ",20,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     70, 150,{ "sword" ,"dagger"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

    {
	"satyr","Satyr    ",20,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     70, 150,	{ "camouflage" ,"ambush", "steal" },
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_MEDIUM},

   {
	"black dragon","BlackDrag",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"blue dragon","BlueDrag",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"green dragon","GreenDrag",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"red dragon","RedDragon",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"white dragon","WhiteDrag",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"brass dragon","BrassDrag",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"gold dragon","GoldDrag ",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"silver dragon","SilverDr ",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"bronze dragon","BronzeDr ",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT},

    {
	"copper dragon","CopperDr ",30,{100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
     170, 800,	{ "hand to hand" , "bash"},
     {16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
     {250, 250, 250, 250, 250, 250, 250, 250, 250, 250}, SIZE_GIANT}



};


// struct    class_type
// {
//    char *  name;              /* the full name of the class  */
//    char    who_name    [4];   /* Three-letter name for 'who' */
//    sh_int  attr_prime;        /* Prime attribute             */
//    sh_int  weapon;            /* First weapon                */
//    sh_int  guild[MAX_GUILD];  /* Vnum of guild rooms         */
//    sh_int  skill_adept;       /* Maximum skill level         */
//    sh_int  thac0_00;          /* Thac0 for level  0          */
//    sh_int  thac0_32;          /* Thac0 for level 32          */
//    sh_int  hp_min;            /* Min hp gained on leveling   */
//    sh_int  hp_max;            /* Max hp gained on leveling   */
//    bool    fMana;             /* Class gains mana on level   */
//    char *  base_group;        /* base skills gained          */
//    char *  default_group;     /* default skills gained       */
// };


/*
 * Class table.
 */
const struct class_type class_table[MAX_CLASS] = {
    {
     "sorcerer", "Sor", "Sorcerer", STAT_INT, OBJ_VNUM_SCHOOL_DAGGER,
     {3018, 9618}, 75, 20, 6, 200, 300, TRUE,
     "mage basics", "sorcerer default"},

    {
     "bishop", "Bsh", "Bishop", STAT_WIS, OBJ_VNUM_SCHOOL_MACE,
     {3003, 9619}, 75, 20, 2, 300, 400, TRUE,
     "cleric basics", "bishop default"},

    {
     "ninja", "Nin", "Ninja", STAT_AGI, OBJ_VNUM_SCHOOL_DAGGER,
     {3028, 9639}, 75, 20, -4, 300, 400, TRUE,
     "thief basics", "ninja default"},

    {
     "hoplite", "Hop", "Hoplite", STAT_VIT, OBJ_VNUM_SCHOOL_SWORD,
     {3022, 9633}, 75, 20, -10, 400, 500, TRUE,
     "warrior basics", "hoplite default"},

    {
     "templar", "Tmp", "Templar", STAT_DIS, OBJ_VNUM_SCHOOL_SWORD,
     {3022, 9633}, 80, 20, -10, 400, 500, TRUE,
     "warrior basics", "templar default"},

    {
     "avenger", "Avn", "Avenger", STAT_STR, OBJ_VNUM_SCHOOL_SWORD,
     {3022, 9633}, 80, 20, -10, 400, 500, TRUE,
     "warrior basics", "avenger default"},

    {
     "lich", "Lch", "Lich", STAT_INT, OBJ_VNUM_SCHOOL_DAGGER,
     {3018, 9618}, 75, 20, 6, 200, 300, TRUE,
     "mage basics", "lich default"},

    {
     "shaman", "Sha", "Shaman", STAT_WIS, OBJ_VNUM_SCHOOL_MACE,
     {3022, 9633}, 75, 20, -10, 400, 500, TRUE,
     "warrior basics", "shaman default"},

    {
     "druid", "Dru", "Druid", STAT_WIS, OBJ_VNUM_SCHOOL_MACE,
     {3022, 9633}, 75, 20, -10, 300, 400, TRUE,
     "cleric basics", "druid default"},

    {
     "assassin", "Asn", "Assassin", STAT_DEX, OBJ_VNUM_SCHOOL_DAGGER,
     {3028, 9639}, 75, 20, -4, 300, 400, TRUE,
     "thief basics", "assassin default"},




//    char *  name;              /* the full name of the class  */
//    char    who_name    [4];   /* Three-letter name for 'who' */
//    char *  finger_name;       /* For do_finger Class output */
//    sh_int  attr_prime;        /* Prime attribute             */
//    sh_int  weapon;            /* First weapon                */
//    sh_int  guild[MAX_GUILD];  /* Vnum of guild rooms         */
//    sh_int  skill_adept;       /* Maximum skill level         */
//    sh_int  thac0_00;          /* Thac0 for level  0          */
//    sh_int  thac0_32;          /* Thac0 for level 32          */
//    sh_int  hp_min;            /* Min hp gained on leveling   */
//    sh_int  hp_max;            /* Max hp gained on leveling   */
//    bool    fMana;             /* Class gains mana on level   */
//    char *  base_group;        /* base skills gained          */
//    char *  default_group;     /* default skills gained       */

};



/*
 * Titles.
 */
char *const title_table[MAX_CLASS][MAX_LEVEL + 1][2] = {
    {
     {"Man", "Woman"},

     {"Apprentice of Magic", "Apprentice of Magic"},
     {"Spell Student", "Spell Student"},
     {"Scholar of Magic", "Scholar of Magic"},
     {"Delver in Spells", "Delveress in Spells"},
     {"Medium of Magic", "Medium of Magic"},

     {"Scribe of Magic", "Scribess of Magic"},
     {"Seer", "Seeress"},
     {"Sage", "Sage"},
     {"Illusionist", "Illusionist"},
     {"Abjurer", "Abjuress"},

     {"Invoker", "Invoker"},
     {"Enchanter", "Enchantress"},
     {"Conjurer", "Conjuress"},
     {"Magician", "Witch"},
     {"Creator", "Creator"},

     {"Savant", "Savant"},
     {"Magus", "Craftess"},
     {"Wizard", "Wizard"},
     {"Warlock", "War Witch"},
     {"Sorcerer", "Sorceress"},

     {"Elder Sorcerer", "Elder Sorceress"},
     {"Grand Sorcerer", "Grand Sorceress"},
     {"Great Sorcerer", "Great Sorceress"},
     {"Golem Maker", "Golem Maker"},
     {"Greater Golem Maker", "Greater Golem Maker"},

     {"Maker of Stones", "Maker of Stones",},
     {"Maker of Potions", "Maker of Potions",},
     {"Maker of Scrolls", "Maker of Scrolls",},
     {"Maker of Wands", "Maker of Wands",},
     {"Maker of Staves", "Maker of Staves",},

     {"Demon Summoner", "Demon Summoner"},
     {"Greater Demon Summoner", "Greater Demon Summoner"},
     {"Dragon Charmer", "Dragon Charmer"},
     {"Greater Dragon Charmer", "Greater Dragon Charmer"},
     {"Master of all Magic", "Master of all Magic"},

     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},

     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},

     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},

     {"Mage Hero", "Mage Heroine"},
     {"Avatar of Magic", "Avatar of Magic"},
     {"Angel of Magic", "Angel of Magic"},
     {"Demigod of Magic", "Demigoddess of Magic"},
     {"Immortal of Magic", "Immortal of Magic"},
     {"God of Magic", "Goddess of Magic"},
     {"Deity of Magic", "Deity of Magic"},
     {"Supremity of Magic", "Supremity of Magic"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },

    {
     {"Man", "Woman"},

     {"Believer", "Believer"},
     {"Attendant", "Attendant"},
     {"Acolyte", "Acolyte"},
     {"Novice", "Novice"},
     {"Missionary", "Missionary"},

     {"Adept", "Adept"},
     {"Deacon", "Deaconess"},
     {"Vicar", "Vicaress"},
     {"Priest", "Priestess"},
     {"Minister", "Lady Minister"},

     {"Canon", "Canon"},
     {"Levite", "Levitess"},
     {"Curate", "Curess"},
     {"Monk", "Nun"},
     {"Healer", "Healess"},

     {"Chaplain", "Chaplain"},
     {"Expositor", "Expositress"},
     {"Bishop", "Bishop"},
     {"Arch Bishop", "Arch Lady of the Church"},
     {"Patriarch", "Matriarch"},

     {"Elder Patriarch", "Elder Matriarch"},
     {"Grand Patriarch", "Grand Matriarch"},
     {"Great Patriarch", "Great Matriarch"},
     {"Demon Killer", "Demon Killer"},
     {"Greater Demon Killer", "Greater Demon Killer"},

     {"Cardinal of the Sea", "Cardinal of the Sea"},
     {"Cardinal of the Earth", "Cardinal of the Earth"},
     {"Cardinal of the Air", "Cardinal of the Air"},
     {"Cardinal of the Ether", "Cardinal of the Ether"},
     {"Cardinal of the Heavens", "Cardinal of the Heavens"},

     {"Avatar of an Immortal", "Avatar of an Immortal"},
     {"Avatar of a Deity", "Avatar of a Deity"},
     {"Avatar of a Supremity", "Avatar of a Supremity"},
     {"Avatar of an Implementor", "Avatar of an Implementor"},
     {"Master of all Divinity", "Mistress of all Divinity"},

     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},

     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},

     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},

     {"Holy Hero", "Holy Heroine"},
     {"Holy Avatar", "Holy Avatar"},
     {"Angel", "Angel"},
     {"Demigod", "Demigoddess",},
     {"Immortal", "Immortal"},
     {"God", "Goddess"},
     {"Deity", "Deity"},
     {"Supreme Master", "Supreme Mistress"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },

    {
     {"Man", "Woman"},

     {"Pilferer", "Pilferess"},
     {"Footpad", "Footpad"},
     {"Filcher", "Filcheress"},
     {"Pick-Pocket", "Pick-Pocket"},
     {"Sneak", "Sneak"},

     {"Pincher", "Pincheress"},
     {"Cut-Purse", "Cut-Purse"},
     {"Snatcher", "Snatcheress"},
     {"Sharper", "Sharpress"},
     {"Rogue", "Rogue"},

     {"Robber", "Robber"},
     {"Magsman", "Magswoman"},
     {"Highwayman", "Highwaywoman"},
     {"Burglar", "Burglaress"},
     {"Thief", "Thief"},

     {"Knifer", "Knifer"},
     {"Quick-Blade", "Quick-Blade"},
     {"Killer", "Murderess"},
     {"Brigand", "Brigand"},
     {"Cut-Throat", "Cut-Throat"},

     {"Spy", "Spy"},
     {"Grand Spy", "Grand Spy"},
     {"Master Spy", "Master Spy"},
     {"Assassin", "Assassin"},
     {"Greater Assassin", "Greater Assassin"},

     {"Master of Vision", "Mistress of Vision"},
     {"Master of Hearing", "Mistress of Hearing"},
     {"Master of Smell", "Mistress of Smell"},
     {"Master of Taste", "Mistress of Taste"},
     {"Master of Touch", "Mistress of Touch"},

     {"Crime Lord", "Crime Mistress"},
     {"Infamous Crime Lord", "Infamous Crime Mistress"},
     {"Greater Crime Lord", "Greater Crime Mistress"},
     {"Master Crime Lord", "Master Crime Mistress"},
     {"Godfather", "Godmother"},

     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},

     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},

     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},

     {"Assassin Hero", "Assassin Heroine"},
     {"Avatar of Death", "Avatar of Death",},
     {"Angel of Death", "Angel of Death"},
     {"Demigod of Assassins", "Demigoddess of Assassins"},
     {"Immortal Assasin", "Immortal Assassin"},
     {"God of Assassins", "God of Assassins",},
     {"Deity of Assassins", "Deity of Assassins"},
     {"Supreme Master", "Supreme Mistress"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },

    {
     {"Man", "Woman"},

     {"Swordpupil", "Swordpupil"},
     {"Recruit", "Recruit"},
     {"Sentry", "Sentress"},
     {"Fighter", "Fighter"},
     {"Soldier", "Soldier"},

     {"Warrior", "Warrior"},
     {"Veteran", "Veteran"},
     {"Swordsman", "Swordswoman"},
     {"Fencer", "Fenceress"},
     {"Combatant", "Combatess"},

     {"Hero", "Heroine"},
     {"Myrmidon", "Myrmidon"},
     {"Swashbuckler", "Swashbuckleress"},
     {"Mercenary", "Mercenaress"},
     {"Swordmaster", "Swordmistress"},

     {"Lieutenant", "Lieutenant"},
     {"Champion", "Lady Champion"},
     {"Dragoon", "Lady Dragoon"},
     {"Cavalier", "Lady Cavalier"},
     {"Knight", "Lady Knight"},

     {"Grand Knight", "Grand Knight"},
     {"Master Knight", "Master Knight"},
     {"Paladin", "Paladin"},
     {"Grand Paladin", "Grand Paladin"},
     {"Demon Slayer", "Demon Slayer"},

     {"Greater Demon Slayer", "Greater Demon Slayer"},
     {"Dragon Slayer", "Dragon Slayer"},
     {"Greater Dragon Slayer", "Greater Dragon Slayer"},
     {"Underlord", "Underlord"},
     {"Overlord", "Overlord"},

     {"Baron of Thunder", "Baroness of Thunder"},
     {"Baron of Storms", "Baroness of Storms"},
     {"Baron of Tornadoes", "Baroness of Tornadoes"},
     {"Baron of Hurricanes", "Baroness of Hurricanes"},
     {"Baron of Meteors", "Baroness of Meteors"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Knight Hero", "Knight Heroine"},
     {"Avatar of War", "Avatar of War"},
     {"Angel of War", "Angel of War"},
     {"Demigod of War", "Demigoddess of War"},
     {"Immortal Warlord", "Immortal Warlord"},
     {"God of War", "God of War"},
     {"Deity of War", "Deity of War"},
     {"Supreme Master of War", "Supreme Mistress of War"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },
    {
     {"Man", "Woman"},

     {"Swordpupil", "Swordpupil"},
     {"Recruit", "Recruit"},
     {"Sentry", "Sentress"},
     {"Fighter", "Fighter"},
     {"Soldier", "Soldier"},

     {"Warrior", "Warrior"},
     {"Veteran", "Veteran"},
     {"Swordsman", "Swordswoman"},
     {"Fencer", "Fenceress"},
     {"Combatant", "Combatess"},

     {"Hero", "Heroine"},
     {"Myrmidon", "Myrmidon"},
     {"Swashbuckler", "Swashbuckleress"},
     {"Mercenary", "Mercenaress"},
     {"Swordmaster", "Swordmistress"},

     {"Lieutenant", "Lieutenant"},
     {"Champion", "Lady Champion"},
     {"Dragoon", "Lady Dragoon"},
     {"Cavalier", "Lady Cavalier"},
     {"Knight", "Lady Knight"},

     {"Grand Knight", "Grand Knight"},
     {"Master Knight", "Master Knight"},
     {"Paladin", "Paladin"},
     {"Grand Paladin", "Grand Paladin"},
     {"Demon Slayer", "Demon Slayer"},

     {"Greater Demon Slayer", "Greater Demon Slayer"},
     {"Dragon Slayer", "Dragon Slayer"},
     {"Greater Dragon Slayer", "Greater Dragon Slayer"},
     {"Underlord", "Underlord"},
     {"Overlord", "Overlord"},

     {"Baron of Thunder", "Baroness of Thunder"},
     {"Baron of Storms", "Baroness of Storms"},
     {"Baron of Tornadoes", "Baroness of Tornadoes"},
     {"Baron of Hurricanes", "Baroness of Hurricanes"},
     {"Baron of Meteors", "Baroness of Meteors"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Knight Hero", "Knight Heroine"},
     {"Avatar of War", "Avatar of War"},
     {"Angel of War", "Angel of War"},
     {"Demigod of War", "Demigoddess of War"},
     {"Immortal Warlord", "Immortal Warlord"},
     {"God of War", "God of War"},
     {"Deity of War", "Deity of War"},
     {"Supreme Master of War", "Supreme Mistress of War"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },
    {
     {"Man", "Woman"},

     {"Swordpupil", "Swordpupil"},
     {"Recruit", "Recruit"},
     {"Sentry", "Sentress"},
     {"Fighter", "Fighter"},
     {"Soldier", "Soldier"},

     {"Warrior", "Warrior"},
     {"Veteran", "Veteran"},
     {"Swordsman", "Swordswoman"},
     {"Fencer", "Fenceress"},
     {"Combatant", "Combatess"},

     {"Hero", "Heroine"},
     {"Myrmidon", "Myrmidon"},
     {"Swashbuckler", "Swashbuckleress"},
     {"Mercenary", "Mercenaress"},
     {"Swordmaster", "Swordmistress"},

     {"Lieutenant", "Lieutenant"},
     {"Champion", "Lady Champion"},
     {"Dragoon", "Lady Dragoon"},
     {"Cavalier", "Lady Cavalier"},
     {"Knight", "Lady Knight"},

     {"Grand Knight", "Grand Knight"},
     {"Master Knight", "Master Knight"},
     {"Paladin", "Paladin"},
     {"Grand Paladin", "Grand Paladin"},
     {"Demon Slayer", "Demon Slayer"},

     {"Greater Demon Slayer", "Greater Demon Slayer"},
     {"Dragon Slayer", "Dragon Slayer"},
     {"Greater Dragon Slayer", "Greater Dragon Slayer"},
     {"Underlord", "Underlord"},
     {"Overlord", "Overlord"},

     {"Baron of Thunder", "Baroness of Thunder"},
     {"Baron of Storms", "Baroness of Storms"},
     {"Baron of Tornadoes", "Baroness of Tornadoes"},
     {"Baron of Hurricanes", "Baroness of Hurricanes"},
     {"Baron of Meteors", "Baroness of Meteors"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Knight Hero", "Knight Heroine"},
     {"Avatar of War", "Avatar of War"},
     {"Angel of War", "Angel of War"},
     {"Demigod of War", "Demigoddess of War"},
     {"Immortal Warlord", "Immortal Warlord"},
     {"God of War", "God of War"},
     {"Deity of War", "Deity of War"},
     {"Supreme Master of War", "Supreme Mistress of War"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },

    {
     {"Man", "Woman"},

     {"Apprentice of Magic", "Apprentice of Magic"},
     {"Spell Student", "Spell Student"},
     {"Scholar of Magic", "Scholar of Magic"},
     {"Delver in Spells", "Delveress in Spells"},
     {"Medium of Magic", "Medium of Magic"},

     {"Scribe of Magic", "Scribess of Magic"},
     {"Seer", "Seeress"},
     {"Sage", "Sage"},
     {"Illusionist", "Illusionist"},
     {"Abjurer", "Abjuress"},

     {"Invoker", "Invoker"},
     {"Enchanter", "Enchantress"},
     {"Conjurer", "Conjuress"},
     {"Magician", "Witch"},
     {"Creator", "Creator"},

     {"Savant", "Savant"},
     {"Magus", "Craftess"},
     {"Wizard", "Wizard"},
     {"Warlock", "War Witch"},
     {"Sorcerer", "Sorceress"},

     {"Elder Sorcerer", "Elder Sorceress"},
     {"Grand Sorcerer", "Grand Sorceress"},
     {"Great Sorcerer", "Great Sorceress"},
     {"Golem Maker", "Golem Maker"},
     {"Greater Golem Maker", "Greater Golem Maker"},

     {"Maker of Stones", "Maker of Stones",},
     {"Maker of Potions", "Maker of Potions",},
     {"Maker of Scrolls", "Maker of Scrolls",},
     {"Maker of Wands", "Maker of Wands",},
     {"Maker of Staves", "Maker of Staves",},

     {"Demon Summoner", "Demon Summoner"},
     {"Greater Demon Summoner", "Greater Demon Summoner"},
     {"Dragon Charmer", "Dragon Charmer"},
     {"Greater Dragon Charmer", "Greater Dragon Charmer"},
     {"Master of all Magic", "Master of all Magic"},

     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},

     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},

     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},
     {"Master Mage", "Master Mage"},

     {"Mage Hero", "Mage Heroine"},
     {"Avatar of Magic", "Avatar of Magic"},
     {"Angel of Magic", "Angel of Magic"},
     {"Demigod of Magic", "Demigoddess of Magic"},
     {"Immortal of Magic", "Immortal of Magic"},
     {"God of Magic", "Goddess of Magic"},
     {"Deity of Magic", "Deity of Magic"},
     {"Supremity of Magic", "Supremity of Magic"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },
    {
     {"Man", "Woman"},

     {"Swordpupil", "Swordpupil"},
     {"Recruit", "Recruit"},
     {"Sentry", "Sentress"},
     {"Fighter", "Fighter"},
     {"Soldier", "Soldier"},

     {"Warrior", "Warrior"},
     {"Veteran", "Veteran"},
     {"Swordsman", "Swordswoman"},
     {"Fencer", "Fenceress"},
     {"Combatant", "Combatess"},

     {"Hero", "Heroine"},
     {"Myrmidon", "Myrmidon"},
     {"Swashbuckler", "Swashbuckleress"},
     {"Mercenary", "Mercenaress"},
     {"Swordmaster", "Swordmistress"},

     {"Lieutenant", "Lieutenant"},
     {"Champion", "Lady Champion"},
     {"Dragoon", "Lady Dragoon"},
     {"Cavalier", "Lady Cavalier"},
     {"Knight", "Lady Knight"},

     {"Grand Knight", "Grand Knight"},
     {"Master Knight", "Master Knight"},
     {"Paladin", "Paladin"},
     {"Grand Paladin", "Grand Paladin"},
     {"Demon Slayer", "Demon Slayer"},

     {"Greater Demon Slayer", "Greater Demon Slayer"},
     {"Dragon Slayer", "Dragon Slayer"},
     {"Greater Dragon Slayer", "Greater Dragon Slayer"},
     {"Underlord", "Underlord"},
     {"Overlord", "Overlord"},

     {"Baron of Thunder", "Baroness of Thunder"},
     {"Baron of Storms", "Baroness of Storms"},
     {"Baron of Tornadoes", "Baroness of Tornadoes"},
     {"Baron of Hurricanes", "Baroness of Hurricanes"},
     {"Baron of Meteors", "Baroness of Meteors"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},
     {"Master Warrior", "Master Warrior"},

     {"Knight Hero", "Knight Heroine"},
     {"Avatar of War", "Avatar of War"},
     {"Angel of War", "Angel of War"},
     {"Demigod of War", "Demigoddess of War"},
     {"Immortal Warlord", "Immortal Warlord"},
     {"God of War", "God of War"},
     {"Deity of War", "Deity of War"},
     {"Supreme Master of War", "Supreme Mistress of War"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },

    {
     {"Man", "Woman"},

     {"Believer", "Believer"},
     {"Attendant", "Attendant"},
     {"Acolyte", "Acolyte"},
     {"Novice", "Novice"},
     {"Missionary", "Missionary"},

     {"Adept", "Adept"},
     {"Deacon", "Deaconess"},
     {"Vicar", "Vicaress"},
     {"Priest", "Priestess"},
     {"Minister", "Lady Minister"},

     {"Canon", "Canon"},
     {"Levite", "Levitess"},
     {"Curate", "Curess"},
     {"Monk", "Nun"},
     {"Healer", "Healess"},

     {"Chaplain", "Chaplain"},
     {"Expositor", "Expositress"},
     {"Bishop", "Bishop"},
     {"Arch Bishop", "Arch Lady of the Church"},
     {"Patriarch", "Matriarch"},

     {"Elder Patriarch", "Elder Matriarch"},
     {"Grand Patriarch", "Grand Matriarch"},
     {"Great Patriarch", "Great Matriarch"},
     {"Demon Killer", "Demon Killer"},
     {"Greater Demon Killer", "Greater Demon Killer"},

     {"Cardinal of the Sea", "Cardinal of the Sea"},
     {"Cardinal of the Earth", "Cardinal of the Earth"},
     {"Cardinal of the Air", "Cardinal of the Air"},
     {"Cardinal of the Ether", "Cardinal of the Ether"},
     {"Cardinal of the Heavens", "Cardinal of the Heavens"},

     {"Avatar of an Immortal", "Avatar of an Immortal"},
     {"Avatar of a Deity", "Avatar of a Deity"},
     {"Avatar of a Supremity", "Avatar of a Supremity"},
     {"Avatar of an Implementor", "Avatar of an Implementor"},
     {"Master of all Divinity", "Mistress of all Divinity"},

     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},

     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},

     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},
     {"Master Cleric", "Master Cleric"},

     {"Holy Hero", "Holy Heroine"},
     {"Holy Avatar", "Holy Avatar"},
     {"Angel", "Angel"},
     {"Demigod", "Demigoddess",},
     {"Immortal", "Immortal"},
     {"God", "Goddess"},
     {"Deity", "Deity"},
     {"Supreme Master", "Supreme Mistress"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },

    {
     {"Man", "Woman"},

     {"Pilferer", "Pilferess"},
     {"Footpad", "Footpad"},
     {"Filcher", "Filcheress"},
     {"Pick-Pocket", "Pick-Pocket"},
     {"Sneak", "Sneak"},

     {"Pincher", "Pincheress"},
     {"Cut-Purse", "Cut-Purse"},
     {"Snatcher", "Snatcheress"},
     {"Sharper", "Sharpress"},
     {"Rogue", "Rogue"},

     {"Robber", "Robber"},
     {"Magsman", "Magswoman"},
     {"Highwayman", "Highwaywoman"},
     {"Burglar", "Burglaress"},
     {"Thief", "Thief"},

     {"Knifer", "Knifer"},
     {"Quick-Blade", "Quick-Blade"},
     {"Killer", "Murderess"},
     {"Brigand", "Brigand"},
     {"Cut-Throat", "Cut-Throat"},

     {"Spy", "Spy"},
     {"Grand Spy", "Grand Spy"},
     {"Master Spy", "Master Spy"},
     {"Assassin", "Assassin"},
     {"Greater Assassin", "Greater Assassin"},

     {"Master of Vision", "Mistress of Vision"},
     {"Master of Hearing", "Mistress of Hearing"},
     {"Master of Smell", "Mistress of Smell"},
     {"Master of Taste", "Mistress of Taste"},
     {"Master of Touch", "Mistress of Touch"},

     {"Crime Lord", "Crime Mistress"},
     {"Infamous Crime Lord", "Infamous Crime Mistress"},
     {"Greater Crime Lord", "Greater Crime Mistress"},
     {"Master Crime Lord", "Master Crime Mistress"},
     {"Godfather", "Godmother"},

     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},

     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},

     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},
     {"Master Thief", "Master Thief"},

     {"Assassin Hero", "Assassin Heroine"},
     {"Avatar of Death", "Avatar of Death",},
     {"Angel of Death", "Angel of Death"},
     {"Demigod of Assassins", "Demigoddess of Assassins"},
     {"Immortal Assasin", "Immortal Assassin"},
     {"God of Assassins", "God of Assassins",},
     {"Deity of Assassins", "Deity of Assassins"},
     {"Supreme Master", "Supreme Mistress"},
     {"Creator", "Creator"},
     {"Implementor", "Implementress"}
     },


};



/*
 * Attribute bonus tables.
 */
// Hitroll, Damroll, Weight, Max Weight of Weapon

const struct str_app_type str_app[251] = {
    {-5, -4, 0, 0},                /* 0  */
    {-5, -4, 3, 1},                /* 1  */
    {-3, -2, 3, 2},
    {-3, -1, 10, 3},            /* 3  */
    {-2, -1, 25, 4},
    {-2, -1, 55, 5},            /* 5  */
    {-1, 0, 80, 6},
    {-1, 0, 90, 7},
    {0, 0, 100, 8},
    {0, 0, 111, 9},
    {0, 0, 112, 9},	// 10
    {1, 1, 113, 9},	
    {1, 1, 114, 9},
    {1, 1, 115, 10},            
    {1, 1, 116, 11},
    {1, 1, 118, 11},	// 15
    {1, 1, 120, 11},
    {1, 1, 122, 11},
    {1, 1, 124, 11},
    {1, 1, 126, 11},
    {1, 1, 128, 11},	// 20
    {1, 1, 130, 11},
    {2, 2, 130, 12},
    {2, 2, 130, 13},            
    {2, 2, 140, 14},
    {2, 2, 142, 14},	// 25
    {2, 2, 144, 14},
    {2, 2, 146, 14},
    {2, 2, 148, 14},
    {2, 2, 150, 15},            
    {2, 2, 165, 16},	// 30
    {2, 3, 180, 22},
    {2, 3, 200, 25},   
    {3, 4, 225, 30},
    {3, 5, 250, 35},            
    {3, 6, 300, 40},	// 35
    {3, 6, 350, 45},
    {3, 6, 355, 45},
    {3, 6, 360, 45},
    {3, 6, 365, 45},
    {3, 6, 370, 45},	// 40
    {4, 6, 375, 45},
    {4, 6, 380, 45},
    {4, 6, 385, 45},
    {4, 6, 390, 45},
    {4, 6, 395, 45},	// 45
    {4, 7, 400, 50},
    {4, 8, 450, 55},
    {4, 8, 451, 55},
    {5, 8, 452, 55},
    {5, 8, 453, 55},	// 50
    {5, 8, 454, 55},
    {5, 8, 455, 60},
    {5, 8, 456, 60},
    {5, 8, 457, 60},
    {5, 8, 458, 60},	// 55
    {5, 8, 459, 60},
    {5, 8, 460, 60},
    {6, 10, 461, 60},
    {6, 10, 463, 60},
    {6, 10, 464, 60},	// 60
    {6, 10, 465, 62},
    {6, 10, 466, 62},
    {6, 10, 467, 62},
    {6, 10, 468, 62},
    {6, 10, 469, 62},	// 65
    {6, 10, 470, 62},
    {7, 12, 470, 64},
    {7, 12, 470, 64},
    {7, 12, 470, 64},
    {7, 12, 470, 64},	// 70
    {7, 12, 470, 64},
    {7, 12, 470, 64},
    {7, 12, 470, 64},
    {7, 12, 470, 64},
    {7, 12, 470, 64},	// 75
    {7, 12, 470, 64},
    {7, 12, 470, 64},
    {7, 12, 470, 64},
    {7, 12, 470, 64},
    {7, 12, 470, 64},	// 80
    {8, 14, 475, 66},
    {8, 14, 475, 66},
    {8, 14, 475, 66},
    {8, 14, 475, 66},
    {8, 14, 475, 66},	// 85
    {8, 14, 475, 66},
    {8, 14, 475, 66},
    {9, 14, 475, 66},
    {9, 14, 475, 66},
    {9, 14, 475, 66},	// 90
    {10, 16, 480, 68},
    {10, 16, 480, 68},
    {10, 16, 480, 68},
    {10, 16, 480, 68},
    {10, 16, 480, 68},	// 95
    {10, 16, 480, 68},
    {10, 16, 480, 68},
    {10, 16, 480, 68},
    {10, 16, 480, 68},
    {12, 20, 500, 70},	// 100
    {12, 20, 500, 70},
    {12, 20, 500, 70},
    {12, 20, 500, 70},
    {12, 20, 500, 70},
    {12, 20, 500, 70},	// 105
    {12, 20, 500, 70},
    {12, 20, 500, 70},
    {12, 20, 500, 70},
    {12, 20, 500, 70},
    {12, 20, 500, 70},	// 110
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},	// 115
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72}, // 120
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},	// 125
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},
    {14, 22, 520, 72},	// 130
    {18, 26, 540, 72},
    {18, 26, 540, 72},
    {18, 26, 540, 72},
    {18, 26, 540, 72},
    {18, 26, 540, 72},	// 135
    {18, 26, 540, 72},
    {18, 26, 540, 72},
    {18, 26, 540, 72},
    {18, 26, 540, 72},
    {18, 26, 540, 72},	// 140
    {20, 28, 545, 72},
    {20, 28, 545, 72},
    {20, 28, 545, 72},
    {20, 28, 545, 72},
    {20, 28, 545, 72},	// 145
    {20, 28, 545, 72},
    {20, 28, 545, 72},
    {20, 28, 545, 72},
    {20, 28, 545, 72},
    {20, 28, 545, 72},	// 150
    {22, 30, 550, 74},
    {22, 30, 550, 74},
    {22, 30, 550, 74},
    {22, 30, 550, 74},
    {22, 30, 550, 74},	// 155
    {22, 30, 550, 74},
    {22, 30, 550, 74},
    {22, 30, 550, 74},
    {22, 30, 550, 74},
    {22, 30, 550, 74},	// 160
    {24, 32, 560, 74},
    {24, 32, 560, 74},
    {24, 32, 560, 74},
    {24, 32, 560, 74},
    {24, 32, 560, 74},	// 165
    {24, 32, 560, 74},
    {24, 32, 560, 74},
    {24, 32, 560, 74},
    {24, 32, 560, 74},
    {24, 32, 560, 74},	// 170
    {26, 34, 570, 74},
    {26, 34, 570, 74},
    {26, 34, 570, 74},
    {26, 34, 570, 74},
    {26, 34, 570, 74},	// 175
    {26, 34, 570, 74},
    {26, 34, 570, 74},
    {26, 34, 570, 74},
    {26, 34, 570, 74},
    {26, 34, 570, 74},	// 180
    {28, 36, 575, 76},
    {28, 36, 575, 76},
    {28, 36, 575, 76},
    {28, 36, 575, 76},
    {28, 36, 575, 76},	// 185
    {28, 36, 575, 76},
    {28, 36, 575, 76},
    {28, 36, 575, 76},
    {28, 36, 575, 76},
    {28, 36, 575, 76},	// 190
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},	// 195
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {32, 42, 585, 78},	// 200
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},	// 205
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},
    {30, 38, 580, 76},	// 210
    {32, 40, 590, 78},
    {32, 40, 590, 78},
    {32, 40, 590, 78},
    {32, 40, 590, 78},
    {32, 40, 590, 78},	// 215
    {32, 40, 590, 78},
    {32, 40, 590, 78},
    {32, 40, 590, 78},
    {32, 40, 590, 78},
    {32, 40, 590, 78},	// 220
    {34, 44, 600, 80},
    {34, 44, 600, 80},
    {34, 44, 600, 80},
    {34, 44, 600, 80},
    {34, 44, 600, 80},	// 225
    {34, 44, 600, 80},
    {34, 44, 600, 80},
    {34, 44, 600, 80},
    {34, 44, 600, 80},
    {34, 44, 600, 80},	// 230
    {36, 46, 600, 80},
    {36, 46, 600, 80},
    {36, 46, 600, 80},
    {36, 46, 600, 80},
    {36, 46, 600, 80},	// 235
    {36, 46, 600, 80},
    {36, 46, 600, 80},
    {36, 46, 600, 80},
    {36, 46, 600, 80},
    {36, 46, 600, 80},	// 240
    {40, 50, 600, 80},
    {40, 50, 600, 80},
    {40, 50, 600, 80},
    {40, 50, 600, 80},
    {40, 50, 600, 80},	// 245
    {40, 50, 600, 80},
    {40, 50, 600, 80},
    {40, 50, 600, 80},
    {40, 50, 600, 80},
    {50, 60, 650, 85}	// 250
};
// Hitroll, Damroll, Weight, Max Weight of Weapon
//    {6, 9, 500, 60}    was 25

// const struct int_app_type int_app[26] = {
/*
I removed this struct because it was only refered to in five places
its much easyer to just have those 5 places be equipped with an
equation that is 250 as max and some division. -Xrakisis
ps. the same for con app, wis app and dex app
*/







/*
 * Liquid properties.
 * Used in world.obj.
 */
const struct liq_type liq_table[] = {
/*    name            color    proof, full, thirst, food, ssize */
    {"water", "clear", {0, 1, 10, 0, 16}},
    {"beer", "amber", {12, 1, 8, 1, 12}},
    {"red wine", "burgundy", {30, 1, 8, 1, 5}},
    {"ale", "brown", {15, 1, 8, 1, 12}},
    {"dark ale", "dark", {16, 1, 8, 1, 12}},

    {"whisky", "golden", {120, 1, 5, 0, 2}},
    {"lemonade", "pink", {0, 1, 9, 2, 12}},
    {"firebreather", "boiling", {190, 0, 4, 0, 2}},
    {"local specialty", "clear", {151, 1, 3, 0, 2}},
    {"slime mold juice", "green", {0, 2, -8, 1, 2}},

    {"milk", "white", {0, 2, 9, 3, 12}},
    {"tea", "tan", {0, 1, 8, 0, 6}},
    {"coffee", "black", {0, 1, 8, 0, 6}},
    {"blood", "red", {0, 2, -1, 2, 6}},
    {"salt water", "clear", {0, 1, -2, 0, 1}},

    {"coke", "brown", {0, 2, 9, 2, 12}},
    {"root beer", "brown", {0, 2, 9, 2, 12}},
    {"elvish wine", "green", {35, 2, 8, 1, 5}},
    {"white wine", "golden", {28, 1, 8, 1, 5}},
    {"champagne", "golden", {32, 1, 8, 1, 5}},

    {"mead", "honey-colored", {34, 2, 8, 2, 12}},
    {"rose wine", "pink", {26, 1, 8, 1, 5}},
    {"benedictine wine", "burgundy", {40, 1, 8, 1, 5}},
    {"vodka", "clear", {130, 1, 5, 0, 2}},
    {"cranberry juice", "red", {0, 1, 9, 2, 12}},

    {"orange juice", "orange", {0, 2, 9, 3, 12}},
    {"absinthe", "green", {200, 1, 4, 0, 2}},
    {"brandy", "golden", {80, 1, 5, 0, 4}},
    {"aquavit", "clear", {140, 1, 5, 0, 2}},
    {"schnapps", "clear", {90, 1, 5, 0, 2}},

    {"icewine", "purple", {50, 2, 6, 1, 5}},
    {"amontillado", "burgundy", {35, 2, 8, 1, 5}},
    {"sherry", "red", {38, 2, 7, 1, 5}},
    {"framboise", "red", {50, 1, 7, 1, 5}},
    {"rum", "amber", {151, 1, 4, 0, 2}},

    {"cordial", "clear", {100, 1, 5, 0, 2}},
    {NULL, NULL, {0, 0, 0, 0, 0}}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)    n

const struct skill_type skill_table[MAX_SKILL] = {

/*
 * Magic spells.
 */

//    char *    name;            /* Name of skill        */
//    sh_int    skill_level[MAX_CLASS];    /* Level needed by class    */
//    sh_int    rating[MAX_CLASS];    /* How hard it is to learn    */
//    SPELL_FUN *    spell_fun;        /* Spell pointer (for spells)    */
//    sh_int    target;            /* Legal targets        */
//    sh_int    minimum_position;    /* Position for caster / user    */
//    sh_int *    pgsn;            /* Pointer to associated gsn    */
//    sh_int    slot;            /* Slot for #OBJECT loading    */
//    sh_int    min_mana;        /* Minimum mana used        */
//    sh_int    beats;            /* Waiting time after use    */
//    char *    noun_damage;        /* Damage message        */
//    char *    msg_off;        /* Wear off message        */
//    char *    msg_obj;        /* Wear off message for obects    */

    {
     "reserved", {99, 99, 99, 99}, {99, 99, 99, 99},
     0, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (0), 0, 0,
     "", "", ""},

// slot 75 OPEN
    {
	"acetum primus",	{ 22, 206, 206, 206, 206, 206,23,206,206,206},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_acetum_primus,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(654),	20,	12,
	"acetum primus",	"!acetum primus!"  },

    {
     "achilles", {200, 200, 200, 200, 200, 200, 200, 200, 200, 200 }, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (78), 5, 12,
     "", "You feel weaker.", ""},

    {
        "acid arrow",           { 20, 206, 206, 206, 206, 206,22,206,206,206},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_acid_arrow,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(644),      20,     12,
        "acid arrow", "!Acid Arrow!" },

    {
     "acid blast", {28, 101, 35, 32, 28, 25, 20, 42, 101, 35}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_acid_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (70), 20, 12,
     "acid blast", "!Acid Blast!"},

    {
     "acid breath", {31, 32, 33, 34, 34, 34, 31, 34, 32, 33}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_acid_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (200), 100, 24,
     "blast of acid", "!Acid Breath!", ""},

    {
	"acute vision",  {7, 206, 206, 206, 206, 25, 7, 206, 206, 206},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
        spell_acute_vision,        TAR_CHAR_SELF,          POS_STANDING,
        NULL,                    SLOT(527),        10,       0,
	"",         "Your vision seems duller.", ""},

    {
	"adamantite golem",  {207, 207, 207, 207, 207, 207, 126, 207, 207, 207},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_adamantite_golem,	TAR_IGNORE,	POS_STANDING,
	NULL,		SLOT(665),	500,		72,
	"",	"You gained enough mana to make more golems now.",""},

   {
	"aid", 	{28, 101, 206, 206, 28, 25, 20, 42, 206, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_aid,		TAR_CHAR_DEFENSIVE,	 POS_STANDING,
	NULL,			SLOT(525),	15,	12,
	"",			"You don't feel so aided anymore." },


    {
     "animal instinct", {50, 206, 70, 75, 80, 70, 55, 80, 206, 70}, 
     { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_animal_instinct, TAR_OBJ_CHAR_DEF, POS_STANDING,
     NULL, SLOT (45), 5, 12,
     "", "Your Animal Instinct fades.",
     "$p's animal instinct fades."},
    {
        "animate dead",         { 206, 206, 206, 206, 206, 206, 40, 206, 206, 206 },
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
        spell_animate_dead,     TAR_OBJ_CHAR_OFF,       POS_STANDING,
        NULL,           SLOT(581),      50,     12,
        "",     "You gain energy to animate new deads."},


    {
     "armor", {7, 2, 10, 5, 5, 5, 8, 5, 2, 10 }, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (1), 5, 12,
     "", "You feel less armored.", ""},

    {
    "assassinate", {206, 206, 206, 206, 206, 206, 206, 206, 206, 10},
 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    spell_null, TAR_IGNORE,          POS_STANDING,
    &gsn_assassinate,       SLOT(0),        0,       18,
    "assassination attempt", "!assassinate!", ""},

    {
	"assist",  {206, 43, 206, 206, 206, 206, 206, 206, 206, 206},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
        spell_assist,        TAR_CHAR_DEFENSIVE,          POS_FIGHTING,
        NULL,             SLOT(670),        100,       12,
	"", "You can assist more, now.", ""
    },

    {
	"astral walk",  {50, 200, 200, 200, 200, 120, 52, 200, 200, 200},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_astral_walk,	TAR_IGNORE,	POS_FIGHTING,
	NULL,			SLOT(622),	80,	12,
	"",			"!Astral Walk!",	""
    },

    {
     "axe", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {6, 6, 5, 4, 4, 4, 6, 4, 5, 5},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_axe, SLOT (0), 0, 0,
     "", "!Axe!", ""},

    {
     "backstab", {101, 101, 1, 101, 101, 101, 101, 101, 101, 1}, {0, 0, 5, 0, 0, 0, 0, 0, 0, 5},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_backstab, SLOT (0), 0, 12,
     "backstab", "!Backstab!", ""},

    {
     "bash", {101, 101, 101, 1, 1, 1, 101, 1, 101, 101}, {0, 0, 0, 4, 4, 4, 0, 4, 0, 0},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_bash, SLOT (0), 0, 24,
     "bash", "!Bash!", ""},

    {
     "berserk", {101, 101, 101, 8, 15, 15, 101, 15, 101, 101}, {0, 0, 0, 5, 5, 5, 0, 5, 0, 0},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_berserk, SLOT (0), 0, 24,
     "", "You feel your pulse slow down.", ""},

   {
    "bite", 
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_bite, SLOT (0), 0, 0,
    "Bite", "!Bite!", ""},

    {
     "bless", {207, 7, 207, 207, 207, 207, 207, 207, 207, 207}, 
     { 0,  1,  0,  0, 0, 0, 0, 0, 0, 0},
     spell_bless, TAR_OBJ_CHAR_DEF, POS_STANDING,
     NULL, SLOT (3), 5, 12,
     "", "You feel less righteous.",
     "$p's holy aura fades."},

    {
     "bark skin", {206, 25, 206, 206, 206, 206, 206, 206, 35, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_bark_skin, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (46), 12, 18,
     "", "Your skin feels soft again.", ""},


    {
     "blindness", {12, 8, 17, 15, 20, 10, 10, 20, 8, 15}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_blindness, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_blindness, SLOT (4), 5, 12,
     "", "You can see again.", ""},

    {
     "bomuzite", {206, 206, 10, 206, 206, 206, 206, 206, 206, 206}, {0, 0, 5, 0, 0, 0, 0, 0, 0, 0},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_bomuzite, SLOT (0), 0, 24,
     "bomuzite", "!Bomuzite!", ""},

    {
     "burning hands", {7, 101, 10, 9, 8, 7, 7, 10, 101, 7}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_burning_hands, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (5), 15, 12,
     "burning hands", "!Burning Hands!", ""},

    {
     "call lightning", {26, 18, 31, 22, 20, 20, 25, 20, 16, 30}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_call_lightning, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (6), 15, 12,
     "lightning bolt", "!Call Lightning!", ""},

    {"calm", {48, 16, 50, 20, 17, 23, 45, 20, 16, 45},  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_calm, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (509), 30, 12,
     "", "You have lost your peace of mind.", ""},

    {
     "cancellation", {18, 26, 34, 34, 34, 34, 18, 34, 25, 34},  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cancellation, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (507), 20, 12,
     "" "!cancellation!", ""},

    {
     "cause critical", {101, 13, 101, 19, 16, 15, 101, 17, 12, 101},  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cause_critical, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (63), 20, 12,
     "spell", "!Cause Critical!", ""},

    {
     "cause light", {101, 1, 101, 3, 3, 2, 101, 3, 1, 101},  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cause_light, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (62), 15, 12,
     "spell", "!Cause Light!", ""},

    {
     "cause serious", {101, 7, 101, 10, 8, 8, 101, 9, 7, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cause_serious, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (64), 17, 12,
     "spell", "!Cause Serious!", ""},

    {
	"caustic font", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_caustic_font,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(653),	20,	12,
	"caustic font",	"!caustic font!", ""},


    {
     "chain lightning", {33, 101, 39, 36, 30, 30, 32, 30, 101, 35 }, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_chain_lightning, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (500), 25, 12,
     "lightning", "!Chain Lightning!", ""},

    {
     "change sex", {101, 101, 101, 101, 101, 101, 101, 101, 101, 101},  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_change_sex, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (82), 15, 12,
     "", "Your body feels familiar again.", ""},

    {
     "channel", {30, 20, 45, 45, 45, 40, 33, 45, 34, 46}, 
     { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_channel, TAR_OBJ_CHAR_DEF, POS_STANDING,
     NULL, SLOT (9), 5, 12,
     "", "!Channel!.", ""},


    {
     "charm person", {20, 101, 25, 101, 50, 50, 25, 30, 101, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_charm_person, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_charm_person, SLOT (7), 5, 12,
     "", "You feel more self-confident.", ""},

    {
     "chill touch", {4, 101, 6, 6, 5, 4, 4, 20, 101, 6}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_chill_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (8), 15, 12,
     "chilling touch", "You feel less cold.", ""},

    {
     "circle", {206, 206, 10, 206, 206, 206, 206, 206, 206, 10}, {0, 0, 5, 0, 0, 0, 0, 0, 0, 5},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_circle, SLOT (0), 0, 12,
     "circle", "!Circle!", ""},

   {
    "claws",
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_claws, SLOT (0), 0, 0,
    "Claws", "!Claws!", ""},

    {
     "cleave", {  206, 25, 206, 206, 206, 206, 206, 206 ,25,206},{ 0,  5,  0,  0, 0, 0, 0, 0, 5, 0},
     spell_null,     TAR_CHAR_OFFENSIVE,     POS_STANDING,
     &gsn_cleave,         SLOT(0),        0,       24,
     "cleave",         "!Cleave!", ""},

    {
     "cocoon", {66, 50, 72, 60, 45, 55, 66, 50, 50, 70}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cocoon, TAR_CHAR_DEFENSIVE, POS_STANDING,
     &gsn_cocoon, SLOT (150), 75, 12,
     "", "Your cocoon breaks and falls to the ground..",
     ""},


    {
     "colour spray", {16, 101, 22, 20, 18, 18, 15, 17, 101, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_colour_spray, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (10), 15, 12,
     "colour spray", "!Colour Spray!", ""},

    {
     "continual light", {6, 4, 6, 9, 9, 9, 5, 9, 4, 6}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_continual_light, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (57), 7, 12,
     "", "!Continual Light!", ""},

    {
     "control weather", {15, 19, 28, 22, 22, 22, 15, 22, 19, 28}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_control_weather, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (11), 25, 12,
     "", "!Control Weather!", ""},

    {
     "counter", { 101, 101, 20, 25, 20, 20, 101, 20, 101, 30 },  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_counter, SLOT( 0), 0, 0,
     "counter attack", "!Counter!",   ""
    },

    {
     "create food", {10, 5, 11, 12, 10, 10, 10, 10, 5, 10}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_create_food, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (12), 5, 12,
     "", "!Create Food!", ""},

    {
     "create rose", {16, 11, 10, 24, 24, 24, 15, 20, 11, 10}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_create_rose, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (511), 30, 12,
     "", "!Create Rose!", ""},

    {
     "create spring", {14, 17, 23, 20, 15, 15, 12, 10, 17, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_create_spring, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (80), 20, 12,
     "", "!Create Spring!", ""},

    {
     "create water", {8, 3, 12, 11, 11, 11, 8, 11, 3, 10}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_create_water, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (13), 5, 12,
     "", "!Create Water!", ""},

    {
	"critical strike",	{   50, 45, 40, 38, 40, 40, 50, 40, 43 ,40},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_critical,			SLOT(0),	0,	18,
	"",			"!critical strike!",	""},

    {
     "cure blindness", {206, 6, 206, 206, 8, 206, 206, 8, 6, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (14), 5, 12,
     "", "!Cure Blindness!", ""},

    {
     "cure critical", {206, 13, 206, 19, 10, 20, 206, 7, 13, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cure_critical, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (15), 20, 12,
     "", "!Cure Critical!", ""},

    {
     "cure disease", {206, 13, 206, 14, 14, 14, 206, 14, 14, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cure_disease, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (501), 20, 12,
     "", "!Cure Disease!", ""},

    {
     "cure light", {206, 1, 206, 3, 2, 5, 206, 3, 1, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cure_light, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (16), 10, 12,
     "", "!Cure Light!", ""},

    {
     "cure poison", {206, 14, 206, 16, 12, 20, 206, 15, 14, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cure_poison, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (43), 5, 12,
     "", "!Cure Poison!", ""},

    {
     "cure serious", {206, 7, 206, 10, 8, 13, 206, 10, 7, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_cure_serious, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (61), 15, 12,
     "", "!Cure Serious!", ""},

    {
     "curse", {18, 18, 26, 22, 22, 22, 18, 22, 18, 23}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_curse, TAR_OBJ_CHAR_OFF, POS_FIGHTING,
     &gsn_curse, SLOT (17), 20, 12,
     "curse", "The curse wears off.",
     "$p is no longer impure."},

    {
     "dagger", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {2, 3, 2, 2, 2, 2, 2, 2, 3, 2},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_dagger, SLOT (0), 0, 0,
     "", "!Dagger!", ""},

    {
        "death ray",           { 206, 206, 206, 206, 206, 206, 120,206,206,206},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        spell_death_ray,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(526),      20,     12,
        "death ray", "!Death Ray!" },


    {
     "demonfire", {101, 34, 101, 45, 50, 35, 101, 50, 34, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_demonfire, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (505), 20, 12,
     "torments", "!Demonfire!", ""},

    {
	"desert fist",	{ 65, 60, 206, 206, 206, 206, 70,50,40,206},
		        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	spell_desert_fist, 	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(681),	50,	12,
	"desert fist",	"!desert fist!", ""
    },


    {
     "detect evil", {11, 4, 12, 101, 10, 10, 10, 15, 4, 12}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_detect_evil, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (18), 5, 12,
     "", "The red in your vision disappears.", ""},

    {
     "detect good", {11, 4, 12, 101, 10, 10, 10, 15, 4, 12}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_detect_good, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (513), 5, 12,
     "", "The gold in your vision disappears.", ""},

    {
     "detect hidden", {15, 11, 12, 101, 101, 101, 15, 20, 11, 12}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_detect_hidden, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (44), 5, 12,
     "", "You feel less aware of your surroundings.",
     ""},

    {
     "detect invis", {3, 8, 6, 101, 101, 101, 3, 101, 8, 5}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_detect_invis, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (19), 5, 12,
     "", "You no longer see invisible objects.",
     ""},

    {
     "detect magic", {2, 6, 5, 101, 101, 101, 2, 101, 6, 3}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_detect_magic, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (20), 5, 12,
     "", "The detect magic wears off.", ""},

    {
     "detect poison", {15, 7, 9, 101, 101, 101, 15, 40, 7, 9},  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_detect_poison, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (21), 5, 12,
     "", "!Detect Poison!", ""},

    {
     "dirt kicking", {101, 101, 3, 3, 3, 3, 101, 3, 101, 3}, {0, 0, 4, 4, 4, 4, 0, 4, 0, 4},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_dirt, SLOT (0), 0, 24,
     "kicked dirt", "You rub the dirt out of your eyes.", ""},

    {
     "disarm", {101, 101, 12, 11, 11, 11, 101, 11, 101, 12}, {0, 0, 6, 4, 4, 4, 0, 4, 0, 6},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_disarm, SLOT (0), 0, 24,
     "", "!Disarm!", ""},

    {
     "dispel evil", {101, 15, 101, 21, 10, 20, 101, 20, 15, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_dispel_evil, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (22), 15, 12,
     "dispel evil", "!Dispel Evil!", ""},

    {
     "dispel good", {101, 15, 101, 21, 30, 20, 101, 30, 15, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_dispel_good, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (512), 15, 12,
     "dispel good", "!Dispel Good!", ""},

    {
     "dispel magic", {16, 24, 30, 30, 25, 25, 16, 25, 24, 23}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_dispel_magic, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (59), 15, 12,
     "", "!Dispel Magic!", ""},

    {
	"disruption", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_disruption,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(648),	20,	12,
	"disruption",	"!disruption!", ""},


    {
     "dodge", {20, 22, 1, 13, 13, 13, 20, 13, 22, 1}, {8, 8, 4, 6, 6, 6, 8, 6, 8, 4},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_dodge, SLOT (0), 0, 0,
     "", "!Dodge!", ""},

    {
	"doppelganger",         {  25, 25, 25,25,25,25,25,25 ,25,25},
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_doppelganger,      TAR_CHAR_DEFENSIVE,       POS_STANDING,
	&gsn_doppelganger,       SLOT(529),        75,       12,
	"", "You return to your native form.", ""},


   {
     "dual backstab", {  207, 207, 207, 207, 207, 207, 207, 207 ,207, 15},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 10},
        spell_null,             TAR_CHAR_OFFENSIVE,           POS_STANDING,
        &gsn_dual_backstab,       SLOT(0),        0,       0,
        "second backstab",      "!dual backstab!", ""},

   {
     "dual circle", {  207, 207, 207, 207, 207, 207, 207, 207 ,207, 55},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 10},
        spell_null,             TAR_CHAR_OFFENSIVE,           POS_STANDING,
        &gsn_dual_circle,       SLOT(0),        0,       0,
        "dual circle",      "!dual circle!", ""},


    {
     "earthquake", {101, 10, 101, 14, 12, 12, 101, 13, 10, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_earthquake, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (23), 15, 12,
     "earthquake", "!Earthquake!", ""},

    {
     "enchant armor", {16, 101, 101, 101, 35, 35, 16, 35, 101, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_enchant_armor, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (510), 100, 24,
     "", "!Enchant Armor!", ""},

    {
     "enchant weapon", {17, 101, 101, 101, 35, 35, 16, 35, 101, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_enchant_weapon, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (24), 100, 24,
     "", "!Enchant Weapon!", ""},

    {
     "energy drain", {19, 22, 26, 23, 20, 15, 19, 25, 22, 23}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_energy_drain, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (25), 35, 12,
     "energy drain", "!Energy Drain!", ""},

    {
     "enhanced damage", {45, 30, 25, 1, 1, 1, 45, 1, 30, 25}, {10, 9, 5, 3, 3, 3, 10, 3, 9, 5},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_enhanced_damage, SLOT (0), 0, 0,
     "", "!Enhanced Damage!", ""},

    {
     "envenom", {101, 101, 10, 101, 101, 101, 101, 101, 101, 10}, {0, 0, 4, 0, 0, 0, 0, 0, 0, 4},
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_envenom, SLOT (0), 0, 36,
     "", "!Envenom!", ""},

    {
	"etheral fist", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_etheral_fist,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(645),	20,	12,
	"etheral fist",		"!Etheral Fist!", ""},


    {
     "faerie fire", {6, 3, 5, 8, 8, 8, 6, 8, 3, 5}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_faerie_fire, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (72), 5, 12,
     "faerie fire", "The pink aura around you fades away.",
     ""},

    {
     "faerie fog", {14, 21, 16, 24, 24, 24, 14, 24, 31, 16}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_faerie_fog, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (73), 12, 12,
     "faerie fog", "!Faerie Fog!", ""},

    {
     "farsight", {14, 16, 16, 101, 40, 40, 14, 40, 15, 15}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_farsight, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (521), 36, 20,
     "farsight", "!Farsight!", ""},

    {
     "fast healing", {15, 9, 16, 6, 6, 6, 15, 6, 9, 16}, {8, 5, 6, 4, 4, 4, 8, 4, 5, 6},
     spell_null, TAR_IGNORE, POS_SLEEPING,
     &gsn_fast_healing, SLOT (0), 0, 0,
     "", "!Fast Healing!", ""},

    {
     "fifth attack", {90, 90, 45, 38, 40, 40, 90, 40, 90, 45}, {5, 4, 3, 2, 2, 2, 5, 2, 4, 3},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_fifth_attack, SLOT (0), 0, 0,
     "", "!Fifth Attack!", ""},

    {
     "fireball", {22, 101, 30, 26, 26, 24, 22, 25, 101, 25}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_fireball, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (26), 15, 12,
     "fireball", "!Fireball!", ""},

    {
     "fire breath", {40, 45, 50, 51, 51, 51, 40, 51, 45, 50}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_fire_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (201), 200, 24,
     "blast of flame", "The smoke leaves your eyes.", ""},

    {
     "fireproof", {13, 12, 19, 18, 18, 18, 13, 18, 12, 19}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_fireproof, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (523), 10, 12,
     "", "", "$p's protective aura fades."},

    {
     "fireshield",
		{13, 12, 19, 18, 18, 18, 13, 18, 12, 19}, 
		{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_fireshield, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (411), 75, 12,
     "fireball", "", "Your firey shield gutters out."},


    {
     "flail", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {6, 3, 6, 4, 4, 4, 6, 4, 3, 6},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_flail, SLOT (0), 0, 0,
     "", "!Flail!", ""},

	{
	 "flame shield", {20, 207, 207, 207, 207, 207, 207, 207, 207, 207}, 
	 { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},	 
	 spell_flameshield, TAR_CHAR_SELF, POS_STANDING,
	 NULL, SLOT (550), 40, 12,
	 "flame shield", "The flames surrounding you die away.", ""},

    {
     "flamestrike", {101, 20, 101, 27, 25, 25, 101, 25, 20, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_flamestrike, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (65), 20, 12,
     "flamestrike", "!Flamestrike!", ""},

    {
     "fly", {10, 18, 20, 22, 20, 20, 10, 20, 18, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_fly, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (56), 10, 18,
     "", "You slowly float to the ground.", ""},

    {
     "floating disc", {4, 10, 7, 16, 15, 15, 4, 15, 10, 7}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_floating_disc, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (522), 40, 24,
     "", "!Floating disc!", ""},

	{
	 "force shield", {20, 207, 207, 207, 207, 207, 207, 207, 207, 207}, 
	 { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},	 
	 spell_forceshield, TAR_CHAR_SELF, POS_STANDING,
	 NULL, SLOT (551), 30, 12,
	 "force shield", "The shield about you fades away.", ""},

    {
     "forging", {10, 10, 6, 10, 10, 10, 10, 10, 10, 6}, {6, 6, 4, 8, 8, 8, 6, 8, 6, 4},
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_forging, SLOT (0), 0, 36,
     "", "!Forge!", ""},


    {
     "fourth attack", {80, 80, 30, 20, 20, 20, 80, 20, 75, 30}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_fourth_attack, SLOT (0), 0, 0,
     "", "!Fourth Attack!", ""},

    {
     "frenzy", {101, 24, 101, 26, 25, 25, 101, 25, 24, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_frenzy, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (504), 30, 24,
     "", "Your rage ebbs.", ""},

    {
     "frost breath", {34, 36, 38, 40, 40, 40, 34, 40, 36, 38}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_frost_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (202), 125, 24,
     "blast of frost", "!Frost Breath!", ""},

    {
	"galvanic whip", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_galvanic_whip,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(655),	20,	12,
	"galvanic whip",	"!galvanic whip!", ""},

    {
     "garrote", {207, 207, 207, 207, 207, 207, 207, 207, 207, 45},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_garrote, SLOT (0), 0, 12,
     "{GG{ga{Gr{gr{Go{gt{Ge{x", "!Garrote!", ""},


    {
     "gas breath", {39, 43, 47, 50, 50, 50, 39, 50, 43, 47}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_gas_breath, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (203), 175, 24,
     "blast of gas", "!Gas Breath!", ""},

    {
     "gate", {10, 17, 8, 28, 12, 12, 10, 25, 17, 8}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_gate, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (83), 80, 12,
     "", "!Gate!", ""},

    {
     "giant strength", {11, 101, 22, 20, 20, 20, 11, 20, 101, 22}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_giant_strength, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (39), 20, 12,
     "", "You feel weaker.", ""},

    {
     "godsword", {207, 207, 207, 207, 5, 207, 207, 207, 207, 207},
     { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
     spell_godsword, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (76), 9, 12,
     "", "!Gods Sword!", ""},


    {
        "grip",           { 40, 35, 30, 10, 15, 15, 40, 16, 30, 30},
        { 1, 1, 1,  1, 1, 1, 1, 1, 1, 1},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_grip,            SLOT( 0),        0,     18,
        "",                     "!Grip!",               ""},

    {
     "haggle", {7, 18, 1, 14, 14, 14, 7, 14, 18, 1}, {5, 8, 3, 6, 6, 6, 5, 6, 8, 3},
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_haggle, SLOT (0), 0, 0,
     "", "!Haggle!", ""},

    {
	"hand of undead",	{120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_hand_of_undead,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(647),	20,	24,
	"hand of undead",	"!hand of undead!", ""},

    {
     "hand to hand", {25, 10, 15, 6, 6, 6, 25, 6, 10, 15}, {8, 5, 6, 4, 4, 4, 8, 4, 5, 6},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_hand_to_hand, SLOT (0), 0, 0,
     "", "!Hand to Hand!", ""},

    {
     "harm", {101, 23, 101, 28, 30, 20, 101, 30, 23, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_harm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (27), 35, 12,
     "harm spell", "!Harm!,        " ""},

   {
     "haste", {21, 101, 26, 29, 25, 25, 21, 25, 101, 24}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_haste, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (502), 30, 12,
     "", "You feel yourself slow down.", ""},

    {
     "heal", {101, 21, 33, 30, 10, 40, 101, 20, 21, 35}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (28), 50, 12,
     "", "!Heal!", ""},

    {
     "heat metal", {101, 16, 101, 23, 23, 23, 101, 25, 16, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_heat_metal, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (516), 25, 18,
     "spell", "!Heat Metal!", ""},

    {
     "hide", {101, 101, 1, 12, 12, 12, 101, 12, 101, 1}, {0, 0, 4, 6, 6, 6, 0, 6, 0, 4},
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_hide, SLOT (0), 0, 12,
     "", "!Hide!", ""},

    {
     "holy word", {207, 36, 207, 207, 207, 207, 207, 207, 207, 207},
     { 0,  1,  0,  0, 0, 0, 0, 0, 0, 0},
     spell_holy_word, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (506), 200, 24,
     "divine wrath", "!Holy Word!", ""},

    {
     "iceshield", 
     {207, 36, 207, 207, 207, 207, 207, 207, 207, 207},
     { 0,  1,  0,  0, 0, 0, 0, 0, 0, 0},
     spell_iceshield, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (410), 75, 14,
     "chilling touch", "Your icy shield slowly melts away.", ""},

    {
     "identify", 
     {15, 16, 18, 101, 15, 15, 10, 15, 15, 15 }, 
     { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_identify, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (101), 12, 24,
     "", "!Identify!", ""},

    {
     "infravision", {9, 13, 10, 16, 10, 10, 10, 10, 10, 10}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_infravision, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (77), 5, 18,
     "", "You no longer see in the dark.", ""},

    {
     "improved heal", {206, 25, 206, 206, 206, 206, 206, 206, 206, 206}, 
     { 0,  1,  0,  0, 0, 0, 0, 0, 0, 0},
     spell_improved_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (47), 50, 12,
     "", "!Improved Heal!", ""},


    {
     "invisibility", {5, 101, 9, 101, 35, 35, 5, 35, 101, 7}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_invis, TAR_OBJ_CHAR_DEF, POS_STANDING,
     &gsn_invis, SLOT (29), 5, 12,
     "", "You are no longer invisible.",
     "$p fades into view."},

    {
     "investiture", {35, 25, 48, 50, 45, 43, 32, 50, 34, 47},
     { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_investiture, TAR_OBJ_CHAR_DEF, POS_STANDING,
     NULL, SLOT (37), 5, 12,
     "", "!Investiture!.", ""},

    {
     "jugular slice", {206, 206, 206, 1, 206, 206, 206, 206, 206, 206}, 
     {0, 0, 0, 3, 0, 0, 0, 0, 0, 0},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_jugular_slice, SLOT (0), 0, 24,
     "jugular slice", "!Jugular Slice!", ""},

   {
     "kick", {101, 12, 14, 8, 8, 8, 101, 8, 12, 14}, {0, 4, 6, 3, 3, 3, 0, 3, 4, 6},
     spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_kick, SLOT (0), 0, 12,
     "kick", "!Kick!", ""},

    {
     "knee", 
     {207, 207, 22, 207, 206, 207, 207, 207, 207, 207},
     {0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_knee, SLOT (0), 0, 12,
     "{Rk{rn{Re{re{B-{Ys{yl{Ya{ym{x", "!knee!", ""},

    {
     "knights shield", {207, 207, 207, 207, 5, 207, 207, 207, 207, 207}, 
     { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
     spell_knightshield, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (76), 9, 12,
     "", "!Knights Shield!", ""},

    {
     "know alignment", {12, 9, 20, 101, 20, 20, 12, 20, 9, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_know_alignment, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
     NULL, SLOT (58), 9, 12,
     "", "!Know Alignment!", ""},

    {
     "lightning bolt", {13, 23, 18, 16, 14, 14, 13, 14, 23, 18}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_lightning_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (30), 15, 12,
     "lightning bolt", "!Lightning Bolt!", ""},

   {
     "lightning breath", {37, 40, 43, 46, 46, 46, 37, 46, 40, 43},{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_lightning_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (204), 150, 24,
     "blast of lightning", "!Lightning Breath!", ""},

    {
     "locate object", {9, 15, 11, 101, 12, 12, 9, 12, 15, 10}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_locate_object, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (31), 20, 18,
     "", "!Locate Object!", ""},

    {
     "lore", {10, 10, 6, 20, 20, 20, 10, 20, 10, 6}, {6, 6, 4, 8, 8, 8, 6, 8, 6, 4},
     spell_null, TAR_IGNORE, POS_RESTING,
     &gsn_lore, SLOT (0), 0, 36,
     "", "!Lore!", ""},

    {
     "mace", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {5, 2, 3, 3, 3, 3, 5, 3, 2, 4},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_mace, SLOT (0), 0, 0,
     "", "!Mace!", ""},

    {
     "magic missile", {1, 101, 2, 2, 2, 2, 1, 2, 101, 1}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_magic_missile, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (32), 15, 12,
     "magic missile", "!Magic Missile!", ""},

    {
	"magnetic trust", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_magnetic_trust,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(656),	20,	12,
	"magnetic trust",	"!magnetic trust!", ""},

    {
     "mana shield",
			{1, 101, 2, 2, 2, 2, 1, 2, 101, 1}, 
			{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_mana_shield, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (32), 15, 12,
     "mana shield", "Your {Bm{bana{x shield fades away!", ""},

    {
     "ward",
      {1, 101, 2, 2, 2, 2, 1, 2, 101, 1},
      { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_ward, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (32), 15, 12,
     "ward", "You are no longer warded.", ""},

    {
     "mass healing", {101, 38, 101, 46, 20, 101, 101, 30, 38, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_mass_healing, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (508), 100, 36,
     "", "!Mass Healing!", ""},

    {
     "mass invis", {22, 25, 31, 101, 101, 101, 22, 101, 25, 31}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_mass_invis, TAR_IGNORE, POS_STANDING,
     &gsn_mass_invis, SLOT (69), 20, 24,
     "", "You are no longer invisible.", ""},

    {
     "meditation", {6, 6, 15, 15, 15, 15, 6, 15, 6, 15}, {5, 5, 8, 8, 8, 8, 5, 8, 5, 8},
     spell_null, TAR_IGNORE, POS_SLEEPING,
     &gsn_meditation, SLOT (0), 0, 0,
     "", "Meditation", ""},

    {
     "mental knife", {28, 101, 101, 101, 101, 101, 101, 101, 101, 101},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_mental_knife, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (531), 20, 12,
     "mental knife", "!Mental Knife!"},

    {
	"mind wrack", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_mind_wrack,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(650),	20,	12,
	"mind wrack",	"!mind wrack!", ""},

    {
	"mind wrench", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_mind_wrench,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(651),	20,	12,
	"mind wrench",	"!mind wrench!", ""},


    {
     "nexus", {40, 35, 50, 45, 40, 40, 40, 40, 35, 45},  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_nexus, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (520), 150, 36,
     "", "!Nexus!", ""},

    {
     "objectgate", 
     {207, 207, 207, 207, 207, 207, 22, 207, 207, 207}, 
     {1, 1, 1, 1, 1, 1, 10, 1, 1, 1},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_objgate, SLOT (0), 0, 0,
     "", "!Object Gate!", ""},

    {
     "parry", {22, 20, 13, 1, 1, 1, 22, 1, 20, 13}, {8, 8, 6, 4, 4, 4, 8, 4, 8, 6},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_parry, SLOT (0), 0, 0,
     "", "!Parry!", ""},

    {
     "pass door", {24, 32, 25, 37, 25, 25, 24, 25, 32, 25}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_pass_door, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (74), 20, 12,
     "", "You feel solid again.", ""},

    {
     "peek", {8, 21, 1, 14, 14, 14, 8, 14, 21, 1}, {5, 7, 3, 6, 6, 6, 5, 6, 7, 3},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_peek, SLOT (0), 0, 0,
     "", "!Peek!", ""},

    {
        "phase", {30, 24, 12, 5, 5, 5, 30, 5, 24, 15}, {10, 8, 5, 3, 3, 3, 10, 3, 8, 5},
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_phase,             SLOT( 0),       0,      0,
        "",                     "!Phase!",   ""
    },

    {
     "pick lock", {25, 25, 7, 25, 25, 25, 25, 25, 25, 7}, {8, 8, 4, 8, 8, 8, 8, 8, 4},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_pick_lock, SLOT (0), 0, 12,
     "", "!Pick!", ""},

    {
     "plague", {23, 17, 36, 26, 101, 20, 23, 35, 17, 25 }, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_plague, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_plague, SLOT (503), 20, 12,
     "sickness", "Your sores vanish.", ""},

    {
     "poison", {17, 12, 15, 21, 21, 20, 17, 22, 12, 16}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_poison, TAR_OBJ_CHAR_OFF, POS_FIGHTING,
     &gsn_poison, SLOT (33), 10, 12,
     "poison", "You feel less sick.",
     "The poison on $p dries up."},

    {
     "polearm", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {6, 6, 6, 4, 4, 4, 6, 4, 6, 6},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_polearm, SLOT (0), 0, 0,
     "", "!Polearm!", ""},

    {
     "portal", {35, 30, 45, 40, 30, 30, 35, 40, 25, 40},  { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_portal, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (519), 100, 24,
     "", "!Portal!", ""},

    {
     "powerstorm", {40, 80, 80,70 , 70, 65, 40, 70, 80, 80}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_powerstorm, TAR_IGNORE, POS_FIGHTING,
     NULL, SLOT (205), 175, 24,
     "powerstorm", "!Powerstorm!", ""},

    {
        "power word kill",      {   78, 206, 206, 206,206,206,206,206,206 ,206},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
        spell_power_kill,       TAR_CHAR_OFFENSIVE,     POS_STANDING,
        NULL,           SLOT(604),      200,    0,
        "powerful word", "You gain back your durability.", ""},

    {
        "power word stun", {   63, 206, 206, 206,206,206,206,206,206 ,206},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
        spell_power_stun,       TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_power_stun,                SLOT(625),      200,    24,
        "",     "You can move now.",    ""},

    {
     "precision", {36, 20, 42, 30, 15, 25, 36, 20, 20, 40}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_precision, TAR_CHAR_DEFENSIVE, POS_STANDING,
     &gsn_precision, SLOT (626), 75, 12,
     "", "You lose your deadly precision.",
     ""},


    {
     "precision strike", {206, 206, 206, 5, 206, 206, 206, 206, 206, 206}, 
	{0, 0, 0, 5, 0, 0, 0, 0, 0, 0},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_precisionstrike, SLOT (0), 0, 24,
     "precision strike", "!Precision Strike!", ""},

    {
     "protection evil", {12, 9, 17, 11, 10, 10, 12, 15, 9, 17}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_protection_evil, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (34), 5, 12,
     "", "You feel less protected.", ""},

    {
     "protection good", {12, 9, 17, 11, 10, 10, 12, 10, 9, 17}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_protection_good, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (514), 5, 12,
     "", "You feel less protected.", ""},

    {
	"protection negative", {125, 206, 206, 206, 206, 206, 115, 206, 206, 206},
        {5, 0, 0, 0, 0, 0, 5, 0, 0, 0},
	spell_protection_negative,	TAR_CHAR_SELF,	POS_STANDING,
	NULL,		SLOT(636),	20,	12,
	"",		"You feel less protected from your own attacks.", ""},

    {
	"quantum spike", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_quantum_spike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(657),	20,	12,
	"quantum spike",	"!quantum spike!", ""},


    {
     "rabies", {27, 24, 30, 26, 25, 25, 206, 25, 24, 30}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_rabies, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (524), 30, 24,
     "", "You feel less Rabid.", ""},

    {
     "rampage", {206, 206, 65, 30, 45, 40, 206, 206, 206, 55},
        {0, 0, 5, 4, 5, 5, 0, 0, 0, 4},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_rampage, SLOT (0), 0, 12,
     "{RR{rampag{Re{x", "!Rampage!", ""},


    {
     "ray of truth", {101, 35, 101, 47, 40, 101, 101, 40, 35, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_ray_of_truth, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (518), 20, 12,
     "ray of truth", "!Ray of Truth!", ""},

    {
     "recall", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_recall, SLOT (0), 0, 12,
     "", "!Recall!", ""},

    {
     "recharge", {9, 101, 101, 101, 101, 101, 9, 101, 101, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_recharge, TAR_OBJ_INV, POS_STANDING,
     NULL, SLOT (517), 60, 24,
     "", "!Recharge!", ""},

    {
     "refresh", {8, 5, 12, 9, 7, 7, 6, 7, 5, 12}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_refresh, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (81), 12, 18,
     "refresh", "!Refresh!", ""},

    {
     "relinquish", {206, 206, 10, 206, 206, 206, 206, 206, 206, 206}, {0, 0, 15, 0, 0, 0, 0, 0, 0, 0},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_relinquish, SLOT (0), 0, 24,
     "relinquish", "!Relinquish!", ""},

    {
     "repair", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     spell_repair, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (245), 0, 24,
     "repair", "!Repair!", ""},

    {
     "remove curse", {101, 18, 101, 22, 10, 20, 101, 30, 18, 101}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_remove_curse, TAR_OBJ_CHAR_DEF, POS_STANDING,
     NULL, SLOT (35), 5, 12,
     "", "!Remove Curse!", ""},

    {
     "rescue", {101, 101, 101, 1, 1, 1, 101, 1, 101, 101}, {0, 0, 0, 4, 4, 4, 0, 4, 0, 0},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_rescue, SLOT (0), 0, 12,
     "", "!Rescue!", ""},

    {
     "sacred cloak", {206, 30, 206, 206, 206, 35, 206, 30, 30, 206}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_sacredcloak, TAR_CHAR_DEFENSIVE, POS_STANDING,
     &gsn_sacredcloak, SLOT (530), 75, 12,
     "", "The Sacred Cloak around your body fades.",
     ""},


    {
     "sanctuary", {36, 20, 42, 30, 15, 25, 36, 20, 20, 40}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_sanctuary, TAR_CHAR_DEFENSIVE, POS_STANDING,
     &gsn_sanctuary, SLOT (36), 75, 12,
     "", "The white aura around your body fades.",
     ""},

    {
     "satiate", {15, 9, 16, 6, 6, 6, 15, 6, 9, 16}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     spell_null, TAR_IGNORE, POS_SLEEPING,
     &gsn_satiate, SLOT (0), 0, 0,
     "", "!Satiate!", ""},


    {
	"scourge", {20, 25, 32, 30, 15, 25, 36, 20, 20, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
        spell_scourge,      TAR_IGNORE,       POS_FIGHTING,
        NULL,       SLOT(528),        50,       18,
        "Scourge", "!scourge!", ""},

    {
     "scrolls", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  {2, 3, 5, 8, 8, 8, 2, 8, 3, 5},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_scrolls, SLOT (0), 0, 24,
     "", "!Scrolls!", ""},

    {
     "second attack", {30, 24, 12, 5, 5, 5, 30, 5, 24, 15}, {10, 8, 5, 3, 3, 3, 10, 3, 8, 5},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_second_attack, SLOT (0), 0, 0,
     "", "!Second Attack!", ""},

    {
     "secondary attack", {40, 34, 22, 15, 15, 15, 40, 15, 34, 25}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     spell_null, TAR_CHAR_SELF, POS_STANDING,
     &gsn_secondary_attack,  SLOT(0), 0, 0,
     "", "!secondary attack!", ""},


    {
     "seventh attack", {206, 206, 70, 52, 60, 60, 206, 70, 206, 70}, {7, 6, 5, 4, 4, 4, 7, 4, 6, 5},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_seventh_attack, SLOT (0), 0, 0,
     "", "!Seventh Attack!", ""},

    {
     "shield", {20, 35, 35, 40, 20, 25, 20, 25, 35, 30}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_shield, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (67), 12, 18,
     "", "Your force shield shimmers then fades away.",
     ""},

    {
     "shield block", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {6, 4, 6, 2, 2, 2, 6, 2, 4, 6 },
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_shield_block, SLOT (0), 0, 0,
     "", "!Shield!", ""},

    {
     "shield levitation", {206, 206, 65, 30, 40, 40, 206, 206, 206, 55},
        {0, 0, 5, 4, 5, 5, 0, 0, 0, 4},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_shield_levitation, SLOT (0), 0, 0,
     "", "!Shield Levitation!", ""},

    {
     "shocking grasp", {10, 101, 14, 13, 13, 13, 10, 13, 101, 14}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_shocking_grasp, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (101), 15, 12,
     "shocking grasp", "!Shocking Grasp!", ""},

    {
     "shockshield", 
			{10, 101, 14, 13, 13, 13, 10, 13, 101, 14}, 
			{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_shockshield, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (412), 75, 12,
     "lightning bolt", "Your crackling shield sizzles and fades.", ""},

    {
     "acidshield",
      {10, 101, 14, 13, 13, 13, 10, 13, 101, 14},
      { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_acidshield, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (536), 75, 12,
     "acidic blast", "Your acidic shield fades away.", ""},

			{
     "poisonshield",
      {10, 101, 14, 13, 13, 13, 10, 13, 101, 14},
      { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_acidshield, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (882), 75, 12,
     "poisonous shield", "Your poisonous shield clears up.", ""},

      {
     "briarshield",
      {10, 101, 14, 13, 13, 13, 10, 13, 101, 14},
      { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_briarshield, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (492), 75, 12,
     "{gt{yh{go{yr{gn{yy {xprick", "Your thorny shield fades away.", ""},


   {
     "side kick", {207, 207, 207, 20, 207, 207, 207, 207, 207, 207},
        {0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_sidekick, SLOT (0), 0, 12,
     "kick", "", ""},


    {
     "siphon energy", 
	{207, 207, 207, 207, 207, 207, 52, 207, 207, 207}, 
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_siphon_energy, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (101), 15, 12,
     "{yenergy drawing essence{x", "{mYour link fades away{x", ""},

    {
     "siphon life", 
        {207, 207, 207, 207, 207, 207, 52, 207, 207, 207},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_siphon_life, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (101), 15, 12,
     "{ylife drawing essence{x", "{mYour link fades away{x", ""},
    {
     "sixth attack", {206, 206, 60, 43, 50, 50, 206, 60, 206, 55}, {6, 5, 4, 3, 3, 3, 6, 3, 5, 4},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_sixth_attack, SLOT (0), 0, 0,
     "", "!Sixth Attack!", ""},

    {
     "skeletal hand", {206, 206, 206, 206, 206, 206, 40, 206, 206, 206},
        {0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_shand, SLOT (0), 0, 12,
     "{WS{ckeleta{Wl H{can{Wd{x", "!Skeletal hand!", ""},

    {
     "sleep", {10, 101, 11, 101, 101, 40, 10, 101, 101, 10}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_sleep, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_sleep, SLOT (38), 15, 12,
     "", "You feel less tired.", ""},

    {
     "slow", {23, 30, 29, 32, 32, 32, 23, 32, 30, 25}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_slow, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (515), 30, 12,
     "", "You feel yourself speed up.", ""},

    {
     "sneak", {101, 101, 4, 10, 10, 10, 101, 10, 101, 4}, {0, 0, 4, 6, 6, 6, 0, 6, 0, 4},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_sneak, SLOT (0), 0, 12,
     "", "You no longer feel stealthy.", ""},

    {
	"sonic resonance", {120, 207, 207, 207, 207, 207, 207, 207, 207, 207}, 
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_sonic_resonance,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(649),	20,	12,
	"sonic resonance",	"!sonic resonance!", ""},


    {
     "spear", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {4, 4, 4, 3, 3, 3, 4, 3, 4, 4},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_spear, SLOT (0), 0, 0,
     "", "!Spear!", ""},

    {
	"spectral furor",	{207, 207, 207, 207, 207, 207, 25, 207, 207, 207}, 
	{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_spectral_furor,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(646),	20,	12,
	"spectral furor",	"!spectral furor!", ""},

	{
	 "static shield", {20, 207, 207, 207, 207, 207, 207, 207, 207, 207}, 
	 { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},	 
	 spell_staticshield, TAR_CHAR_SELF, POS_STANDING,
	 NULL, SLOT (552), 35, 12,
	 "static shield", "The static around you grounds out.", ""},

    {
     "staves", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {2, 3, 5, 8, 8, 8, 2, 8, 3, 5},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_staves, SLOT (0), 0, 12,
     "", "!Staves!", ""},

    {
     "steal", {101, 101, 5, 101, 101, 101, 101, 101, 101, 5}, {0, 0, 4, 0, 0, 0, 0, 0, 0, 4},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_steal, SLOT (0), 0, 24,
     "", "!Steal!", ""},

    {
     "stone skin", {25, 40, 40, 45, 30, 30, 25, 40, 40, 30}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_stone_skin, TAR_CHAR_SELF, POS_STANDING,
     NULL, SLOT (66), 12, 18,
     "", "Your skin feels soft again.", ""},

    {
     "summon", {24, 12, 29, 22, 20, 20, 24, 25, 12, 26}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_summon, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (40), 50, 12,
     "", "!Summon!", ""},

    {
	"sulfurus spray", {120, 130, 207, 207, 207, 207, 125, 130, 130, 207}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
	spell_sulfurus_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,		SLOT(652),	20,	12,
	"sulfurus spray",	"!sulfurus spray!", ""},


    {
     "sword", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {5, 6, 3, 2, 2, 2, 5, 2, 6, 3},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_sword, SLOT (0), 0, 0,
     "", "!sword!", ""},

    {
     "tesseract", {  20, 25, 28, 30,25,25,20,30,25,20},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
        spell_tesseract,      TAR_IGNORE,       POS_STANDING,
        &gsn_tesseract,       SLOT(556),        150,       12,
        "", "!tesseract!", ""},

    {
     "teleport", {13, 22, 25, 36, 36, 36, 13, 36, 22, 20}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_teleport, TAR_CHAR_SELF, POS_FIGHTING,
     NULL, SLOT (2), 35, 12,
     "", "!Teleport!", ""},

    {
     "tension", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {5, 6, 3, 2, 2, 2, 5, 2, 6, 3},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_tension, SLOT (0), 0, 0,
     "", "!tension!", ""},

    {
     "third attack", {35, 30, 20, 12, 12, 12, 35, 12, 30, 20}, {10, 8, 5, 4, 4, 4, 10, 4, 10, 10},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_third_attack, SLOT (0), 0, 0,
     "", "!Third Attack!", ""},

    {
     "trip", {101, 101, 1, 15, 15, 15, 101, 15, 101, 1}, {0, 0, 4, 8, 8, 8, 0, 8, 0, 4},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_trip, SLOT (0), 0, 24,
     "trip", "!Trip!", ""},

    {
     "unnerve", {207, 207, 140, 207, 207, 207, 207, 207, 207, 307},
        {0, 0, 5, 0, 0, 0, 0, 0, 0, 0},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_unnerve, SLOT (0), 0, 12,
     "", "!Unnerve!", ""},

    {
     "ventriloquate", {1, 101, 2, 101, 101, 101, 1, 101, 101, 2}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_ventriloquate, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (41), 5, 12,
     "", "!Ventriloquate!", ""},

    {
     "vital hit", 
			{207, 207, 125, 45, 58, 75, 207, 207, 207, 40},
			{2, 3, 5, 8, 8, 8, 2, 8, 3, 5},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_vital_hit, SLOT (0), 0, 12,
     "vital hit", "", ""},

    {
     "wands", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {2, 3, 5, 8, 8, 8, 2, 8, 3, 5},
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_wands, SLOT (0), 0, 12,
     "", "!Wands!", ""},

    {
     "weaken", {11, 14, 16, 17, 16, 16, 11, 16, 14, 16},{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_weaken, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_weaken, SLOT (68), 20, 12,
     "spell", "You feel stronger.", ""},


     {
     "web", {11, 14, 16, 17, 16, 16, 11, 16, 14, 16},{ 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_web, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_web, SLOT (561), 10,   12,
     "", "The web surrounding you breaks away.", ""},

    {
     "whip", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {6, 5, 5, 4, 4, 4, 6, 4, 5, 5},
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_whip, SLOT (0), 0, 0,
     "", "!Whip!", ""},

    {
     "word of recall", {32, 28, 40, 30, 30, 30, 32, 30, 28, 40}, { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
     spell_word_of_recall, TAR_CHAR_SELF, POS_RESTING,
     NULL, SLOT (42), 5, 12,
     "", "!Word of Recall!", ""},

// Bishop Only
    {
        "wrath of god", {207, 110, 207, 207, 207, 207, 207, 207, 207, 207},
        { 1,  1,  1,  1, 1, 1, 1, 1, 1, 1},
        spell_wrath_of_god,    TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,           SLOT(75),      20,     12,
        "wrath of god",        "!wrath of god!", ""},



/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
    {
     "general purpose", {101, 101, 101, 101, 101, 101, 101, 101, 101, 101}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     spell_general_purpose, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (401), 0, 12,
     "general purpose ammo", "!General Purpose Ammo!", ""},

    {
     "high explosive", {101, 101, 101, 101, 101, 101, 101, 101, 101, 101},{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     spell_high_explosive, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     NULL, SLOT (402), 0, 12,
     "high explosive ammo", "!High Explosive Ammo!", ""}

};

const struct group_type group_table[MAX_GROUP] = {

    {
     "rom basics", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {"scrolls", "staves", "wands", "recall"}
     },

    {
     "mage basics", {0, -1, -1, -1, -1, -1, 0, -1, -1, -1},
     {"dagger"}
     },

    {
     "cleric basics", {-1, 0, -1, -1, -1, -1, -1, -1, 0, -1},
     {"mace"}
     },

    {
     "thief basics", {-1, -1, 0, -1, -1, -1, -1, -1, -1, 0},
     {"dagger", "steal"}
     },

    {
     "warrior basics", {-1, -1, -1, 0, 0, 0, -1, 0, -1, -1},
     {"sword", "second attack"}
     },

    {
     "sorcerer default", {40, -1, -1, -1, -1, -1, -1, -1, -1, -1},
     {"lore", "beguiling", "combat", "detection", "enhancement", "illusion",
      "maladictions", "protective", "transportation", "weather", "attack",
	"creation", "draconian"}
     },

    {
     "bishop default", {-1, 40, -1, -1, -1, -1, -1, -1, -1, -1},
     {"flail", "attack", "creation", "curative", "benedictions",
      "healing", "maladictions", "protective", "shield block",
      "weather", "harmful", "wrath of god"}
     },

    {
     "templar default", {-1, -1, -1, -1, 40, -1, -1, -1, -1, -1},
     {"weaponsmaster", "shield block", "bash", "maladictions", "enhanced damage",
      "benedictions", "protective", "parry", "rescue", "third attack", "knights shield",
      "godsword"}
     },

    {
     "avenger default", {-1, -1, -1, -1, -1, 40, -1, -1, -1, -1},
     {"weaponsmaster", "shield block", "bash", "maladictions", "enhanced damage",
      "combat", "transportation", "parry", "rescue", "third attack", "negative"}
     },

    {
     "ninja default", {-1, -1, 40, -1, -1, -1, -1, -1, -1, -1},
     {"mace", "sword", "backstab", "disarm", "dodge", "second attack",
      "trip", "hide", "peek", "pick lock", "sneak"}
     },

    {
     "hoplite default", {-1, -1, -1, 40, -1, -1, -1, -1, -1, -1},
     {"weaponsmaster", "shield block", "bash", "disarm", "enhanced damage",
      "parry", "rescue", "third attack", "fourth attack", "fifth attack",
      "sixth attack", "seventh attack", "secondary attack"}
     },

    {
     "lich default", {-1, -1, -1, -1, -1, -1, 0, -1, -1, -1},
     {"lore", "beguiling", "combat", "detection", "enhancement", "attack",
      "maladictions", "protective", "transportation", "weather", "negative"}
     },

    {
     "shaman default", {-1, -1, -1, -1, -1, -1, -1, 0, -1, -1},
     {"weaponsmaster", "shield block", "detection", "disarm", "enhanced damage",
      "parry", "rescue", "third attack", "protective"}
     },

    {
     "druid default", {-1, -1, -1, -1, -1, -1, -1, -1, 0, -1},
     {"mace", "attack", "creation", "curative", "benedictions",
      "detection", "healing", "protective", "shield block",
      "weather", "harmful"}
     },

    {
     "assassin default", {-1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
     {"mace", "sword", "backstab", "disarm", "dodge", "second attack",
      "trip", "hide", "peek", "pick lock", "sneak", "lore", "illusion",
      "assassinate", "circle"}
     },

    {
     "weaponsmaster", {40, 40, 40, 20, 20, 20, 40, 20, 40, 40},
     {"axe", "dagger", "flail", "mace", "polearm", "spear", "sword", "whip", "repair"}
     },

    {
     "animal", {2, 8, 5, 5, 8, 5, 2, 5, 8, 5},
     {"animal instinct", "rabies", "web"}
     },

    {
     "attack", {-1, 5, -1, 8, 8, 8, -1, 8, 5, 0},
     {"dispel evil", "dispel good", "earthquake",
      "flamestrike", "heat metal", "ray of truth", "power word kill",
      "power word stun"}
     },

    {
     "beguiling", {4, -1, 6, -1, -1, -1, 4, -1, -1, 4},
     {"calm", "charm person", "sleep"}
     },

    {
     "benedictions", {-1, 4, -1, 8, 8, 8, -1, 8, 4, -1},
     {"bless", "calm", "frenzy", "holy word", "remove curse", "investiture"}
     },

    {
     "combat", {6, -1, 10, 9, 9, 9, 6, 9, -1, 10},
     {"acid arrow", "acid blast", "burning hands", "chain lightning", "chill touch",
      "colour spray", "fireball", "lightning bolt", "magic missile",
      "shocking grasp", "powerstorm", "acetum primus", "scourge", "etheral fist"
	"spectral furor", "disruption", "sonic resonance", "sulfurus spray",
	"caustic font", "galvanic whip", "magnetic trust", "quantum spike",
	"hand of undead"}
     },

    {
     "creation", {4, 4, 8, 8, 8, 8, 4, 8, 4, 8},
     {"continual light", "create food", "create spring", "create water",
      "create rose", "floating disc"}
     },

    {
     "curative", {1, 4, 1, 8, 8, 8, 1, 8, 4, 5},
     {"cure blindness", "cure disease", "cure poison"}
     },

    {
     "detection", {4, 3, 6, -1, -1, -1, 4, -1, 3, 6},
     {"detect evil", "detect good", "detect hidden", "detect invis",
      "detect magic", "detect poison", "farsight", "identify",
      "know alignment", "locate object"}
     },

    {
     "draconian", {8, -1, -1, -1, -1, -1, 8, -1, -1, -1},
     {"acid breath", "fire breath", "frost breath", "gas breath",
      "lightning breath"}
     },

    {
     "enchantment", {6, -1, -1, -1, -1, -1, 6, -1, -1, -1},
     {"enchant armor", "enchant weapon", "fireproof", "recharge"}
     },

    {
     "enhancement", {5, -1, 9, 9, 9, 9, 5, 9, -1, 9},
     {"giant strength", "haste", "infravision", "refresh", "channel"}
     },

    {
     "harmful", {-1, 3, -1, 6, 6, 6, -1, 6, 3, -1},
     {"cause critical", "cause light", "cause serious", "harm"}
     },

    {
     "healing", {-1, 3, -1, 6, 6, 6, -1, 6, 3, -1},
     {"cure critical", "cure light", "cure serious", "heal",
      "mass healing", "refresh", "improved heal", "assist"}
     },

    {
     "illusion", {4, -1, 7, -1, -1, -1, 4, -1, -1, 7},
     {"invis", "mass invis", "ventriloquate"}
     },

    {
     "maladictions", {5, 5, 9, 9, 9, 9, 5, 9, 5, 9},
     {"blindness", "change sex", "curse", "energy drain", "plague",
      "poison", "slow", "weaken"}
     },

    {
     "nature", {-1, 2, -1, -1, -1, -1, -1, -1, 2, -1},
     {"bark skin"}
     },

    {
     "negative", {-1, -1, -1, -1, -1, 2, -1, -1, -1, -1},
     {"death ray", "demonfire" }
     },

    {
     "protective", {4, 4, 7, 8, 8, 8, 4, 8, 4, 7},
     {"armor", "cancellation", "dispel magic", "fireproof",
      "protection evil", "protection good", "sanctuary", "shield",
      "stone skin", "flame shield", "force shield", "static shield",
      "sacred cloak", "precision"}
     },

    {
     "shielding", {4, 4, 7, 8, 8, 8, 4, 8, 4, 7},
     {"iceshield", "fireshield", "shockshield", "acidshield", "poisonshield",
      "briarshield",
      "mana shield", "ward"}
     },


    {
     "transportation", {4, 4, 8, 9, 9, 9, 4, 9, 4, 8},
     {"fly", "gate", "nexus", "pass door", "portal", "summon", "teleport",
      "tesseract", "word of recall", "astral walk"}
     },

    {
     "weather", {4, 4, 8, 8, 8, 8, 4, 8, 4, 8},
     {"call lightning", "control weather", "faerie fire", "faerie fog",
      "lightning bolt"}
     },

    {
     "psyche", {4, 4, 8, 8, 8, 8, 4, 8, 4, 8},
     {"mental knife", "mind wrack", "mind wrench"}
     }



};


int god_lookup (const char *name)
{
    int god;

    for (god = 0; god < MAX_GOD; god++)
    {
        if (god_table[god].name == NULL)
            break;
        if (LOWER (name[0]) == LOWER (god_table[god].name[0])
            && !str_prefix (name, god_table[god].name))
            return god;
    }

    return -1;
}

