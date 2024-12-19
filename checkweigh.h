
#ifndef __CHECKWEIGH_INCLUDED
#define __CHECKWEIGH_INCLUDED

#define _ACCEPT 1
#define _OVER 2
#define _UNDER 3

void store_ID(void);
void store_over_under(void);
void recall_over_under(int id);
void scroll_ID(void);
void display_target(void);
void init_checkweigh(void);
void calculate_targets(void);
void edit_target_value(void);
void transaction_memory_init(void);
void transaction_smachine(void);
void check_weigh(void);
void update_stats(void);
void edit_stat_cnt(void);
#endif

