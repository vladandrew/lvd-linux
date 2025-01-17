#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/etherdevice.h>
#include <net/rtnetlink.h>

extern void *skb_data_pool;
extern void shared_mem_init(void);
extern void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);

int __module_lcd_init(void);
void __module_lcd_exit(void);

#ifndef LCD_ISOLATE
#define get_jiffies()           ({ jiffies; })
#endif

#endif
