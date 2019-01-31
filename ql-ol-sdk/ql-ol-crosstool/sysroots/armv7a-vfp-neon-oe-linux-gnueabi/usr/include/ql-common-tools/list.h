/*
 * list.h
 */

#ifndef __LIST_H__
#define __LIST_H__

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void list_init(struct list_head *head)
{
	head->prev = head;
	head->next = head;
}

static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void *)0;
	entry->prev = (void *)0;
}

static inline int list_empty(struct list_head *head)
{
	return head->next == head;
}

static inline int list_len(struct list_head *head)
{
	struct list_head *list;
	int count = 0;

	for(list = head->next; list != head; list = list->next) {
		count++;
	}
	return count;
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define list_for_each(pos, head) \
	for (pos = (head)->next, prefetch(pos->next); pos != (head); \
	     pos = pos->next, prefetch(pos->next))

#define list_for_each_prev(pos, head)					\
	for (pos = (head)->prev, prefetch(pos->prev); pos != (head);	\
	     pos = pos->prev, prefetch(pos->prev))

#define list_for_each_safe(pos, n, head)			\
        for (pos = (head)->next, n = pos->next; pos != (head);	\
	     pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member)                          \
	for (pos = list_entry((head)->next, typeof(*pos), member);      \
	     &pos->member != (head);                                    \
	     pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		     n = list_entry(pos->member.next, typeof(*pos), member); \
	     &pos->member != (head);					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

#endif /* __LIST_H__ */
