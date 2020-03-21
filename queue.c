#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q) {
        list_ele_t *tmp = NULL;
        while (q->head) {
            tmp = q->head->next;
            free(q->head->value);
            free(q->head);
            q->head = tmp;
        }
        free(q);
    }
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    newh->value = malloc(strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    memcpy(newh->value, s, strlen(s) + 1);

    newh->next = q->head;
    q->head = newh;

    if (!q->tail) {
        q->tail = newh;
    }

    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }

    newt->value = malloc(strlen(s) + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }

    memcpy(newt->value, s, strlen(s) + 1);
    newt->next = NULL;
    q->size++;
    if (!q->tail) {
        q->head = newt;
        q->tail = newt;
        return true;
    }

    q->tail->next = newt;
    q->tail = newt;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0) {
        return false;
    }

    list_ele_t *victim = q->head;
    q->head = q->head->next;

    if (strlen(victim->value) < bufsize) {
        bufsize = strlen(victim->value);
    } else {
        bufsize--;
    }

    if (sp) {
        memcpy(sp, victim->value, bufsize);
        sp[bufsize] = '\x00';
    }

    free(victim->value);
    free(victim);

    if (--q->size == 0) {
        q->tail = q->head;
    }

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return (!q) ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size < 2) {
        return;
    }
    list_ele_t *prev = NULL;
    list_ele_t *next = NULL;
    list_ele_t *tmp = q->head;
    while (q->head != NULL) {
        next = q->head->next;
        q->head->next = prev;
        prev = q->head;
        q->head = next;
    }
    q->head = prev;
    q->tail = tmp;
}

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t *temp = NULL;
    list_ele_t *q = NULL;

    if (strcasecmp(l1->value, l2->value) < 1) {
        q = l1;
        temp = l1;
        l1 = l1->next;
    } else {
        q = l2;
        temp = l2;
        l2 = l2->next;
    }

    while (l1 && l2) {
        if (strcasecmp(l1->value, l2->value) < 1) {
            temp->next = l1;
            temp = temp->next;
            l1 = l1->next;
        } else {
            temp->next = l2;
            temp = temp->next;
            l2 = l2->next;
        }
    }

    if (l1) {
        temp->next = l1;
    }
    if (l2) {
        temp->next = l2;
    }


    return q;
}

list_ele_t *MergeSortList(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    list_ele_t *mid = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = MergeSortList(head);
    list_ele_t *l2 = MergeSortList(mid);


    return merge(l1, l2);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }

    if (q->head == q->tail) {
        return;
    }

    q->head = MergeSortList(q->head);

    q->tail = q->head;
    while (q->tail && q->tail->next) {
        q->tail = q->tail->next;
    }
}
