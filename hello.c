#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Andrii Shnaidruk <ashnaidruk56@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

int i = 0;

struct hello_entry {
    struct list_head list;
    ktime_t time;
};

static LIST_HEAD(hello_list);

static unsigned int print_count = 1;
module_param(print_count, uint, S_IRUGO);
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!' (default=1)");

static int __init hello_init(void)
{
    if (print_count == 0) {
        pr_warn("Print count is 0.\n");
    } else if (print_count > 5 && print_count < 10) {
        pr_warn("Print count is between 5 and 10.\n");
    }else if (print_count > 10) {
        pr_err("Print count is greater than 10. Module cannot be loaded.\n");
        return -EINVAL;
    }
    
    
    for (i = 0; i < print_count; i++) {
        struct hello_entry *entry = kmalloc(sizeof(*entry), GFP_KERNEL);

        if (!entry) {
            pr_err("Failed to allocate memory for hello_entry\n");
            return -ENOMEM;
        }

        entry->time = ktime_get();
        list_add(&entry->list, &hello_list);
        pr_emerg("Hello, world!\n");

    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct hello_entry *entry, *temp;

    list_for_each_entry_safe(entry, temp, &hello_list, list) {
 pr_emerg("Time: %lld ns\n", ktime_to_ns(entry->time));
        list_del(&entry->list);
        kfree(entry);
    }
}

module_init(hello_init);
module_exit(hello_exit);
