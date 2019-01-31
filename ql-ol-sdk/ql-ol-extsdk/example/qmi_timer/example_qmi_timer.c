#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ql_oe.h>
#include <ql_timer.h>
#include <ql_error.h>


int timer_expire_cb(ql_timer_id timer_id, void *params)
{
   printf("time_id = %d expired\n",timer_id);
}
int main(int argc, char **argv)
{
   char scan_string[100];
   int  timer_id;

   while(1)
   {
      printf("\n please input test timer_id(1-10)\n");
      printf("timer_id = ");
      if(fgets(scan_string, sizeof(scan_string),stdin) == NULL)
         continue;
      timer_id = atoi(scan_string);
      if(timer_id >= QL_TIMER_ID_1 && timer_id <= QL_TIMER_ID_10)
      {
         QL_Timer_Register(timer_id, timer_expire_cb, NULL);
	 QL_Timer_Start(timer_id, 2000,1);
      }
   }
}
