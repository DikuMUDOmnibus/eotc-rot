/***************************************************************************

                      _____  __      __  __      __
                     /  _  \/  \    /  \/  \    /  \
                    /  /_\  \   \/\/   /\   \/\/   /
                   /    |    \        /  \        /
                   \____|__  /\__/\  /    \__/\  /
                           \/      \/          \/

    As the Wheel Weaves based on ROM 2.4. Original code by Dalsor.
    See changes.log for a list of changes from the original ROM code.
    Credits for code created by other authors have been left
 	intact at the head of each function.

    Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,
    Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.

    Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael
    Chastain, Michael Quan, and Mitchell Tse.

    In order to use any part of this Merc Diku Mud, you must comply with
    both the original Diku license in 'license.doc' as well the Merc
    license in 'license.txt'.  In particular, you may not remove either of
    these copyright notices.

    Much time and thought has gone into this software and you are
    benefitting.  We hope that you share your changes too.  What goes
    around, comes around.

	ROM 2.4 is copyright 1993-1998 Russ Taylor
	ROM has been brought to you by the ROM consortium
	    Russ Taylor (rtaylor@hypercube.org)
	    Gabrielle Taylor (gtaylor@hypercube.org)
	    Brian Moore (zump@rom.org)
	By using this code, you have agreed to follow the terms of the
	ROM license, in the file Rom24/doc/rom.license

 ***************************************************************************/


struct smith_items_type
{
	char *name;
	int   quantity;
	int   weight;
	int   difficulty;
	int   beats;
	int   wear_flags;
	int   item_type;
	int   base_v0;
	int   base_v1;
	int   base_v2;
	int   base_pierce;
	int   base_bash;
	int   base_slash;
	int   base_exotic;
	char *dam_noun;
};
struct smith_ores_type
{
	char *name;
	int   cost_stone;
	int   armor_mod;
	int   workability;
	char *color;
};
const   struct  smith_ores_type smith_ores_table [] =
{
	{   "bronze",    10,   10, 25, "reddish-gold"  }, /* 1 */
	{   "iron",      25,   15, 22, "bright-gray"   }, /* 2 */
	{   "steel",     50,   25, 20, "silvery-gray"  }, /* 3 */
	{   "silver",    120,  8,  18,  "bright"        }, /* 4 */
	{   "gold",      225,  5,  16,  "gleaming"      }, /* 5 */
	{   "gemstone",  500,  25, 11,  "sparkling"     }, /* 6 */
	{   NULL,        0   }
};

const   struct  smith_items_type smith_items_table [] =
{
	{   "knife",          10, 10, 10,  5,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 4, 0, 0, 0, 0, "pierce"      }, /* 1 */
	{   "cock-spur",      5,  5,  10,  4,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 3, 0, 0, 0, 0, "pierce"  }, /* 2 */
	{   "stiletto",       12, 10, 15,  5,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 5, 0, 0, 0, 0, "stab"	      }, /* 3 */
	{   "dagger",         15, 10, 20,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 6, 0, 0, 0, 0, "pierce"	      }, /* 4 */
	{   "keris",          16, 12, 22,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 7, 0, 0, 0, 0, "jab"	      }, /* 5 */
	{   "dirk",           16, 12, 22,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 7, 0, 0, 0, 0, "thrust"	      }, /* 6 */
	{   "poinard",        18, 12, 22,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 8, 0, 0, 0, 0, "thrust"	      }, /* 7 */
	{   "long-knife",     20, 15, 25,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 9, 0, 0, 0, 0, "thrust"	      }, /* 8 */
	{   "shortsword",     22, 15, 25,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_DAGGER,
		1, 10, 0, 0, 0, 0, "slice"	      }, /* 9 */
	{   "langsax",        22, 15, 25,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 11, 0, 0, 0, 0, "slice"	      }, /* 10 */
	{   "leaf-sword",     22, 16, 26,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 12, 0, 0, 0, 0, "slice"	      }, /* 11 */
	{   "gladius",        25, 20, 30,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 13, 0, 0, 0, 0, "stab"	      }, /* 12 */
	{   "cutlass",        26, 22, 30,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 14, 0, 0, 0, 0, "slice"	      }, /* 13 */
	{   "sabre",          26, 22, 30,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 15, 0, 0, 0, 0, "slash"	      }, /* 14 */
	{   "rapier",         26, 22, 32,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 16, 0, 0, 0, 0, "pierce"	      }, /* 15 */
	{   "cinqueda",       26, 23, 32,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 17, 0, 0, 0, 0, "slash"	      }, /* 16 */
	{   "spatha",         26, 24, 33,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 18, 0, 0, 0, 0, "slash"	      }, /* 17 */
	{   "falchion",       27, 25, 35,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 19, 0, 0, 0, 0, "slash"	      }, /* 18 */
	{   "scimitar",       28, 28, 35,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 20, 0, 0, 0, 0, "slash"	      }, /* 19 */
	{   "nimcha",         30, 30, 35,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 21, 0, 0, 0, 0, "slash"	      }, /* 20 */
	{   "hanger",         30, 30, 36,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 22, 0, 0, 0, 0, "slash"	      }, /* 21 */
	{   "kastane",        28, 28, 38,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 23, 0, 0, 0, 0, "slash"	      }, /* 22 */
	{   "yatagan",        28, 28, 38,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 24, 0, 0, 0, 0, "slice"	      }, /* 23 */
	{   "longsword",      32, 32, 40,  8,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 25, 0, 0, 0, 0, "slash"	      }, /* 4 */
	{   "backsword",      30, 30, 42,  8,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 26, 0, 0, 0, 0, "slash"	      }, /* 5 */
	{   "ringsword",      32, 32, 45,  8,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 27, 0, 0, 0, 0, "slice"	      }, /* 6*/
	{   "broadsword",     32, 35, 45,  8,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 28, 0, 0, 0, 0, "slash"	      }, /* 7 */
	{   "flamberge",      38, 38, 48,  8,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 29, 0, 0, 0, 0, "slash"	      }, /* 8 */
	{   "bastard-sword",  45, 50, 55,  9,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 30, 0, 0, 0, 0, "chop"	      }, /* 9 */
	{   "greatsword",     48, 58, 60,  9,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 31, 0, 0, 0, 0, "smash"	      }, /* 30 */
	{   "war-sword",      50, 60, 65,  9,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 32, 0, 0, 0, 0, "cleave"	      }, /* 31 */
	{   "claymore",       55, 65, 70,  9,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SWORD,
		1, 33, 0, 0, 0, 0, "sunder"	     }, /* 2 */
	{   "hand-axe",       26, 23, 32,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_AXE,
		1, 14, 0, 0, 0, 0, "chop"	      }, /* 3 */
	{   "hatchet",        26, 25, 34,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_AXE,
		1, 15, 0, 0, 0, 0, "chop"	     }, /* 4 */
	{   "recade",         28, 23, 35,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_AXE,
		1, 16, 0, 0, 0, 0, "chop"	      }, /* 5 */
	{   "francisca",      28, 28, 34,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_AXE,
		1, 22, 0, 0, 0, 0, "cleave"	      }, /* 6 */
	{   "battle-axe",     32, 32, 45,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_AXE,
		1, 28, 0, 0, 0, 0, "cleave"	      }, /* 7 */
	{   "broad-axe",      35, 35, 50,  7,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_AXE,
		1, 33, 0, 0, 0, 0, "cleave"	      }, /* 8 */
	{   "war-axe",        36, 38, 55,  8,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_AXE,
		1, 40, 0, 0, 0, 0, "sunder"	      }, /* 9 */
	{   "horsemans-mace", 28, 28, 40,  5,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_MACE,
		1, 16, 0, 0, 0, 0, "pound"	      }, /* 40 */
	{   "war-hammer",     30, 30, 30,  5,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_MACE,
		1, 18, 0, 0, 0, 0, "beating"	     }, /* 41 */
	{   "morning-star",   32, 32, 30,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_MACE,
		1, 22, 0, 0, 0, 0, "pound"	      }, /* 2 */
	{   "battle-hammer",  34, 34, 30,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_MACE,
		1, 25, 0, 0, 0, 0, "crunch"	     }, /* 3 */
	{   "footmans-mace",  32, 30, 32,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_MACE,
		1, 28, 0, 0, 0, 0, "smash"	     }, /* 4 */
	{   "angon",          10, 10, 30,  5,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SPEAR,
		1, 20, 0, 0, 0, 0, "pierce"	      }, /* 5 */
	{   "assegai",        10, 10, 33,  5,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SPEAR,
		1, 28, 0, 0, 0, 0, "pierce"	      }, /* 6 */
	{   "spear",          10, 10, 34,  5,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SPEAR,
		1, 35, 0, 0, 0, 0, "pierce"	      }, /* 7 */
	{   "pilum",          10, 10, 36,  5,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SPEAR,
		1, 36, 0, 0, 0, 0, "thrust"	     }, /* 8 */
	{   "javelin",        12, 15, 39,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SPEAR,
		1, 38, 0, 0, 0, 0, "jab"	      }, /* 9 */
	{   "lance",          15, 18, 45,  6,  ITEM_TAKE|ITEM_WIELD, ITEM_WEAPON, WEAPON_SPEAR,
		1, 40, 0, 0, 0, 0, "lunge"	      }, /* 50 */
	{   "scale-tunic",    20, 20, 75,  9,  ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 20, 20, 20, 20, NULL	 }, /* 51 */
	{   "hauberk",        25, 25, 80,  10, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 23, 23, 23, 23, NULL	  }, /* 2 */
	{   "haubergeon",     35, 35, 85,  11, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 26, 26, 26, 26, NULL	  }, /* 3 */
	{   "chain-hauberk",  40, 45, 90,  12, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 30, 30, 30, 30, NULL	  }, /* 4 */
	{   "scale-hauberk",  50, 50, 95,  13, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 35, 35, 35, 35, NULL	  }, /* 5 */
	{   "chain-mail",     55, 55, 96,  14, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 39, 39, 39, 39, NULL	  }, /* 6 */
	{   "scale-mail",     60, 60, 97,  15, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 46, 46, 46, 46, NULL	  }, /* 7 */
	{   "plate-mail",     65, 65, 98,  16, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 53, 53, 53, 53, NULL	  }, /* 8 */
	{   "field-plate",    70, 70, 99,  18, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 61, 61, 61, 61, NULL	  }, /* 9 */
	{   "full-plate",     75, 75, 100, 20, ITEM_TAKE|ITEM_WEAR_BODY, ITEM_ARMOR, 0,
		0, 0, 70, 70, 70, 70, NULL	  }, /* 60 */
	{   "finger-ring",    2,  2,  20,  3,  ITEM_TAKE|ITEM_WEAR_FINGER, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	}, /* 61 */
	{   "finger-band",    2,  2,  25,  3,  ITEM_TAKE|ITEM_WEAR_FINGER, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	}, /* 2 */
	{   "nose-ring",      2,  2,  27,  3,  ITEM_TAKE|ITEM_WEAR_NOSE, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 3 */
	{   "nose-hoop",      2,  2,  29,  3,  ITEM_TAKE|ITEM_WEAR_FINGER, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	}, /* 4 */
	{   "nose-chain",     3,  3,  32,  4,  ITEM_TAKE|ITEM_WEAR_FINGER, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	}, /* 5 */
	{   "nose-pendant",   3,  3,  35,  5,  ITEM_TAKE|ITEM_WEAR_FINGER, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	}, /* 6 */
	{   "scarab",         5,  5,  75,  6,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 7 */
	{   "torque",         8,  8,  80,  9,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 8 */
	{   "brooch",         5,  5,  30,  4,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 9 */
	{   "pendant",        5,  5,  32,  5,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 70 */
	{   "necklace",       5,  5,  30,  6,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 71 */
	{   "medallion",      6,  6,  35,  6,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 2 */
	{   "fibula",         8,  8,  40,  7,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 4 */
	{   "neck-chain",     5,  5,  30,  6,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 5 */
	{   "neck-pin",       2,  2,  27,  5,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 6 */
	{   "amulet",         6,  6,  35,  5,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 7 */
	{   "choker",         6,  6,  65,  7,  ITEM_TAKE|ITEM_WEAR_NECK, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 9 */
	{   "cufflink",       2,  2,  55,  6,  ITEM_TAKE|ITEM_WEAR_WRIST, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 80 */
	{   "wrist-pin",      2,  2,  29,  4,  ITEM_TAKE|ITEM_WEAR_WRIST, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 81 */
	{   "wrist-band",     8,  8,  33,  5,  ITEM_TAKE|ITEM_WEAR_WRIST, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 2 */
	{   "armlet",         5,  5,  37,  5,  ITEM_TAKE|ITEM_WEAR_ARMS, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 3 */
	{   "armband",        8,  8,  35,  5,  ITEM_TAKE|ITEM_WEAR_ARMS, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 4 */
	{   "earring",        2,  2,  75,  5,  ITEM_TAKE|ITEM_WEAR_EAR, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 5 */
	{   "ear-hoop",       3,  3,  60,  5,  ITEM_TAKE|ITEM_WEAR_EAR, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 6 */
	{   "ear-chain",      4,  4,  65,  6,  ITEM_TAKE|ITEM_WEAR_EAR, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 7 */
	{   "ear-pendant",    5,  5,  70,  7,  ITEM_TAKE|ITEM_WEAR_EAR, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 8 */
	{   "buckle",         9,  9,  72,  5,  ITEM_TAKE|ITEM_WEAR_WAIST, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	}, /* 9 */
	{   "girdle",         15, 15, 80,  7,  ITEM_TAKE|ITEM_WEAR_WAIST, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 90 */
	{   "tiara",          12, 12, 90,  12, ITEM_TAKE|ITEM_WEAR_HEAD, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 91 */
	{   "comb",           8,  8,  58,  6,  ITEM_TAKE|ITEM_WEAR_HEAD, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 2 */
	{   "hairpin",        4,  4,  32,  4,  ITEM_TAKE|ITEM_WEAR_HEAD, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	 }, /* 3 */
	{   "diadem",         18, 18, 95,  20, ITEM_TAKE|ITEM_WEAR_HEAD, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 4 */
	{   "spectacles",     5,  5,  90,  9,  ITEM_TAKE|ITEM_WEAR_EYES, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 5 */
	{   "monacle",        2,  2,  92,  8,  ITEM_TAKE|ITEM_WEAR_EYES, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 6 */
	{   "eyestrainers",   5,  5,  95,  10, ITEM_TAKE|ITEM_WEAR_EYES, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 7 */
	{   "eyeferns",       5,  5,  97,  12, ITEM_TAKE|ITEM_WEAR_EYES, ITEM_ARMOR, 0,
		0, 0, 1, 1, 1, 1, NULL	  }, /* 8 */
	{   "mask",           20, 20, 98,  15, ITEM_TAKE|ITEM_WEAR_EYES, ITEM_ARMOR, 0,
		0, 0, 2, 2, 2, 2, NULL	  }, /* 9 */
	{   "leggings",           20, 20, 75,  15, ITEM_TAKE|ITEM_WEAR_LEGS, ITEM_ARMOR, 0,
		0, 0, 25, 25, 25, 25, NULL	  }, /* 100 */
	{   "boots",           20, 20, 75,  15, ITEM_TAKE|ITEM_WEAR_FEET, ITEM_ARMOR, 0,
		0, 0, 20, 20, 20, 20, NULL	  }, /* 101 */
	{   "gloves",           5, 5, 75,  15, ITEM_TAKE|ITEM_WEAR_HANDS, ITEM_ARMOR, 0,
		0, 0, 5, 5, 5, 5, NULL	  }, /* 102 */
	{   "gauntlets",           20, 20, 78,  15, ITEM_TAKE|ITEM_WEAR_HANDS, ITEM_ARMOR, 0,
		0, 0, 18, 18, 18, 18, NULL	  }, /* 103 */
	{   "round-shield",           20, 20, 75,  15, ITEM_TAKE|ITEM_WEAR_SHIELD, ITEM_ARMOR, 0,
		0, 0, 50, 50, 50, 50, NULL	  }, /* 104 */
	{   "full-body-shield",           20, 20, 80,  15, ITEM_TAKE|ITEM_WEAR_SHIELD, ITEM_ARMOR, 0,
		0, 0, 80, 80, 80, 80, NULL	  }, /* 105 */
	{   "chainmail-vest",           20, 20, 75,  15, ITEM_TAKE|ITEM_WEAR_ABOUT, ITEM_ARMOR, 0,
		0, 0, 50, 50, 50, 50, NULL	  }, /* 106 */
//	{   "razor",          5,  5,  10,  4,  ITEM_TAKE|ITEM_HOLD, ITEM_RAZOR, 0,
//		0, 0, 0, 0, 0, 0, NULL      }, /* 100 */
	{   NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL  }  /* 101 */
};
extern const struct smith_items_type smith_items_table[];
extern const struct smith_ores_type smith_ores_table[];

