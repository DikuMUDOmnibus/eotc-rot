
#define INDEP		0
#define LONER		1
#define OUTCAST		2
#define LEGION		3
#define DEMON		9

#define MEMBER		0
#define JUNIOR		1
#define SENIOR		2
#define DEPUTY		3
#define SECOND		4
#define LEADER		5

int clan_lookup		(const char *name);
extern	const	char *	lookup_rank(int);

typedef struct clan_type CLAN_TYPE;

// CLAN_DATA clan_table2[MAX_CLAN + 1];

// LEADER_BOARD leader_board;
CLAN_TYPE clan_type;

struct clan_type
{
	char *	name;			/* name					*/
	char *	who_name;		/* who entry			*/
	char *	god;			/* sacrifice god		*/
	sh_int	deathroom;		/* death-transfer room	*/
	sh_int	recall;			/* recall room			*/
	bool	independent;	/* true for loners		*/
	sh_int	min_level;		/* min level to petition*/
	int     gold;			/* Clan Treasury - Xrak 	*/
};


struct clan_titles
{
	char *rank;
};


extern  struct  clan_type               clan_table[MAX_CLAN];
extern  const   struct  clan_titles             clan_rank_table[MAX_RANK];


