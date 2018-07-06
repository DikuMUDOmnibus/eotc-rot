#define CHANGES_FILE    "changes.txt"

void load_changes        (void);
void save_changes        (void);
char *  wordwrap         ( char *oldstring );
#define  NULLSTR( str )  ( str == NULL || str[0] == '\0' )

extern int max_change;



char *wordwrapchange (char *word);

struct change_data
{
    char *         text;
    char *         imm;
    char *         date;
};


 typedef struct change_data CHANGE_DATA;

