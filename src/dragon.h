/***************************************************************************
*  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
*  Changes done to the code done by Sage aka Walter Howard, this mud is   *
*  for the public, however if you run this code it means you agree        *
*  to the license.low, license.gw, and license.merc have fun. :)          *
***************************************************************************/
/********************************************************
 *                                                      *
 * Re-done by GryphonHawk for DarkSkies                 *
 * March 2003                                           *
 *                                                      *
 ********************************************************/


#define POLY_DRAGON 1

// Age.
#define D_IMMORTAL	13 // Immortal <color> dragon
#define D_ANCIENT		12 // Ancient <color> dragon
#define D_VENERABLE	11 // Venerable <color> dragon
#define D_ELDER		10 // Elder <color> dragon
#define D_VOLD			9  // Very old <color> dragon
#define D_OLD			8  // Old <color> dragon
#define D_MATURE		7  // Mature adult<color> dragon
#define D_ADULT		6  // Adult <color> dragon
#define D_YADULT		5  // Young adult <color> dragon
#define D_JUVENILE	4  // Juvenile <color> dragon
#define D_YOUNG		3  // Young <color> dragon
#define D_VYOUNG		2  // Very young <color> dragon
#define D_HATCHLING	1  // Hatchling <color> dragon

#define DPOWER_FLAGS    0 /* C_POWERS  1 */
#define dragonage  	   1
#define hatch				2
#define DRAGON_COLOR	   4
//#define DRAGON_FORM     3


// Colors
#define DRED		1   // #R
#define DGREEN		2   // #G
#define DBLUE		3   // #L
#define DBRONZE	4   // #o
#define DSILVER	5   // #C
#define DWHITE		6   // #7
#define DBLACK		7   // #0
#define DGOLD		8   // #y
#define DSHADOW	9   // #0
#define DPURPLE   10  // #p
#define DCHAOS    11  // #RC#0h#Ra#0o#Rs#n
#define DSAPPHIRE 12  // #LS#la#Lp#lp#Lh#li#Lr#le#n
#define DEMERALD  13  // #GE#gm#Ge#gr#Ga#gl#Gd#n
#define DRUBY     14  // #RR#ru#Rb#ry#n
#define DTOPAZ    15  // #yT#oo#yp#oa#yz#n
#define DDIAMOND  16
#define MAX_FORMS	7

//dragon points
// #define DEMON_CURRENT                 8 /* POWER_CURRENT 0*/
// #define DEMON_TOTAL                   9 /* POWER_TOTAL   1*/




/* boggle, both.... */
#define DRA_WINGS                     1
#define DRA_FANGS                     1
#define DRA_LICK                      2
#define DRA_FROST_BREATH              3
#define DRA_FIRE_BREATH               4
#define DRA_LIGHTNING_BREATH          8
#define DRA_ACID_BREATH              16
#define DRA_GAS_BREATH               32
#define DRA_TAIL                     64
#define DRA_TERROR                  128
#define DRA_CONFUSE                 256
#define DRA_MASK                    512
#define DRA_MAGIC                  1024
#define DRA_POLYMORPH              2048
#define DRA_LUCK                   4096
#define DRA_FLY                    8192
#define DRA_SCRY                  16384
#define DRA_PEER                  32768
#define DRA_TRUESIGHT            131072
#define DRA_TOUGHNESS            262144
#define DRA_CLAWS                524288

