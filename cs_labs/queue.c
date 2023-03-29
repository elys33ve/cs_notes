/* 
 * Code for basic C skills diagnostic.
 * Developed by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 * 
 * @author O'Connell Fiona
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"


// create empty queue
// return NULL if fail to allocate space
queue_t *q_new(){
    queue_t *q = (queue_t*) malloc(sizeof(queue_t));

    // q is NULL / failed to allocate space
    if (!q){
    return NULL;
    }
    
    q->head = NULL;
    q->tail = NULL;
    q->length = 0;

    return q;
}


// free all storage used by queue
void q_free(queue_t *q){
    if (q){
        list_ele_t *current = q->head;      // first element in list
        list_ele_t *tempptr;                // temp element pointer

        while (current != NULL){
            free(current->value);       // free str storage
            tempptr = current;
            current = current->next;    // get next element
            free(tempptr);              // free preveious element
        }
        
        free(q);   
    }     
}


/*
  insert element at head of queue
  return true if successful
  return false if q is NULL or could not allocate space
  argument s points to string to be stored
  the function explicitly allocates space and copies the string into it
*/
bool q_insert_head(queue_t *q, char *s){
    // q is NULL
    if (!q){
        return false;
    }

    list_ele_t *newh;       // new head element

    newh = (list_ele_t*) malloc(sizeof(list_ele_t));            // allocate for new element
    if (!newh){               // could not allocate space
        return false;
    }

    newh->value = (char*) malloc((strlen(s)+1)*sizeof(char));   // allocate memory for string
    if (!newh->value){        // could not allocate space
        free(newh);
        return false;
    }


    // copy string
    strcpy(newh->value, s);
    
    // link previous q head
    if (!q->head){              // if this is first element in list
        newh->next = NULL;
        q->head = newh;
        q->tail = newh;
    }
    else{                       // not first element
        newh->next = q->head;
        q->head = newh;
    }

    q->length++;

    return true;
}


/*
  insert element at tail of queue
  return true if successful
  return false if q is NULL or could not allocate space
  argument s points to string to be stored
  the function explicitly allocates space and copies the string into it
*/
bool q_insert_tail(queue_t *q, char *s){
    // q is NULL
    if (!q){
        return false;
    }
    
    list_ele_t *newt;               // new tail element
    list_ele_t *oldt = q->tail;     // old tail element

    newt = (list_ele_t*) malloc(sizeof(list_ele_t));            // allocate for new element
    if (!newt){               // could not allocate space
        return false;
    }

    newt->value = (char*) malloc((strlen(s)+1)*sizeof(char));   // allocate memory for string
    if (!newt->value){        // could not allocate space
        free(newt);
        return false;
    }


    // copy string
    strcpy(newt->value, s);

    // link previous q tail
    if (!q->tail){              // if this is first element in list
        newt->next = NULL;
        q->head = newt;
        q->tail = newt;
    }   
    else{                       // not first element
        newt->next = NULL;
        oldt->next = newt;
        q->tail = newt;
    }

    q->length++;

    return true;
}


/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize){
    // if q is NULL or list empty
    if ((!q) || (!q->head)){     
        return false;
    }

    list_ele_t *removed = q->head;      // temp pointer for head element to remove
    
    // remove element
    if (q->length > 1){             // if more than one item in list
        q->head = q->head->next;
    }
    else{                           // if only one item in list
        q->head = NULL;
        q->tail = NULL;
    }

    // get str and free space
    if (sp){
        if (strlen(removed->value) > bufsize){      // element str longer than bufsize
            //sp = (char*) malloc(bufsize);
            strncpy(sp, removed->value, bufsize-1);
            sp[bufsize-1] = '\0';
        }
        else{                                       // element str shorter than bufsize
            strcpy(sp, removed->value); 
        }
        free(removed->value);           // free allocated space of removed head element
        free(removed);
    }
    else{
        free(removed->value);
        free(removed);
    }

    q->length--;

    return true;
}


/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q){
    // if q is NULL or empty
    if (!q){
        return 0;
    }
    return q->length;
}


/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q){
    // if q is not NULL
    if (q){
        list_ele_t *previous = NULL;
        list_ele_t *current = q->head;
        list_ele_t *next;

        // reverse order
        q->tail = q->head;
        while (current){
            next = current->next;
            current->next = previous;
            previous = current;
            current = next;
        }
        q->head = previous;

    }
}

// python2 driver.py
// make && ./qtest
