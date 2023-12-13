
#include <sys/types.h>

struct msgbuff
{
  long mtype;
  char mtext[64];
};

int init_up_queue() ;
int init_down_queue();
