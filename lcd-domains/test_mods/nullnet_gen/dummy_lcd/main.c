/*
 * main.c - runs when dummy lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <asm/lcd_domains/libvmfunc.h>

#include "../common.h"

#include <lcd_config/post_hook.h>

struct thc_channel *net_async;
struct glue_cspace *nullnet_cspace;
cptr_t nullnet_sync_endpoint;

int dummy_init_module(void);
void dummy_cleanup_module(void);

int handle_rpc_calls(struct fipc_message* msg)
{
	const size_t id = msg->rpc_id;
	if (!try_dispatch(id, msg, get_register_page(smp_processor_id()))) {
		glue_user_panic("Couldn't dispatch on LCD side");
	}

	return 0;
}

static int dummy_lcd_init(void)
{
	int ret = 0;

	printk("LCD enter: current:%p \n", current);
	ret = lcd_enter();
	if (ret)
		goto fail1;

	glue_user_init();

	ret = dummy_init_module();

	return ret;
fail1:
	lcd_exit(ret);
}

int __module_lcd_init(void)
{
	return dummy_lcd_init();
}

static void dummy_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	dummy_cleanup_module();

	lvd_exit(0);
	return;
}

void __module_lcd_exit(void)
{
	dummy_lcd_exit();
}

module_init(__module_lcd_init);
module_exit(dummy_lcd_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");

/* extract data from linker variables */
size_t vmfunc_sboard_page_size = (size_t)&__vmfunc_sboard_page_size;
unsigned long* vmfunc_sboard_load_addr = (unsigned long*) &__vmfunc_sboard_load_addr;

/* extract data from linker variables */
size_t vmfunc_trampoline_page_size = (size_t)&__vmfunc_trampoline_page_size;
unsigned long* vmfunc_trampoline_load_addr = (unsigned long*) &__vmfunc_trampoline_load_addr;

EXPORT_SYMBOL(vmfunc_sboard_load_addr);
EXPORT_SYMBOL(vmfunc_trampoline_load_addr);
