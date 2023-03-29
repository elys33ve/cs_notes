#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>



/*
notes for reviewing linked list and pointers in c

using code from https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm as reference
*/

struct node {
    int key;
    int data;
    struct node *next;      // pointer 'next' of type node
};


struct node *head = NULL;       // ptr var for head of linked list
struct node *current = NULL;    // ptr var for current node in list


// display list
void print_list(){
    struct node *ptr = head;    // local ptr var to head ptr

    // start from beginning of list (while not empty)
    printf("\n[ ");
    while (ptr != NULL){        // print until next ptr is NULL
        printf("(%d, %d)", ptr->key, ptr->data);
        ptr = ptr->next;
    }
    printf(" ]");
}



// insert link at first location
void insert_first (int key, int data){
    // create link
    struct node *link = (struct node*) malloc(sizeof(struct node));  // ptr var type node
    // allocate memory of size struct node (w key,data,*next)
    // (data type) malloc(amt of memory to allocate)
    // in this case, size of struct node (struct containing int key, int data, ptr *next)

    link->key = key;    // -> used to access members of struct (similar to .)
    link->data = data;

    // point to old first node
    link->next = head;

    // point first to new first node
    head = link;

    /*  take arg values for key and data,
        create node ptr link and allocate memory,
        assign key and data w values from args,
        assign ptr next w ptr head (old first node)
        assign head ptr w link node just created    */
}



// delete first item
struct node* delete_first(){
    // save reference to first link
    struct node* templink = head;

    // make next to first link first
    head = head->next;

    // return deleted link
    return templink;
}



// is list empty
bool is_empty (){
    return head == NULL;
}



// length
int length (){
    int length = 0;

    for (current = head; current != NULL; current = current->next){
        length++;
    }
    return length;
}



// find a link with given key
struct node* find (int key){
    struct node* current = head;

    if (head == NULL){      // if list empty
        return NULL;
    }
  
    while (current->key != key){

        if (current->next == NULL){     // if current is last node
            return NULL;
        }
        else {
            current = current->next;
        }
    }

    return current;     // if key is found
}



// delete link w given key
struct node* delete (int key){
    struct node* current = head;
    struct node* previous = NULL;

    // if list empty
    if (head == NULL){
        return NULL;
    }

    // find node
    while (current->key != key){
        if (current->next == NULL){
            return NULL;
        }
        else {
            previous = current;
            current = current->next;
        }
    }

    // match found
    if (current == head){
        head == head->next;
    }
    else {
        previous->next = current->next;     // bypass current link
    }

    return current;
}


/*
// sort
void sort (){
    // sort by data size (integer value) largest to smallest

}

// reverse
void reverse (){
    // reverse list order
}
*/


// main
void main (){
    insert_first(1,2);
    insert_first(2,3);
    insert_first(4,2);
    insert_first(2,54);

    printf("og list: \n");
    print_list();
    printf("\n");

    while(!is_empty()){
        struct node* temp = delete_first();
        printf("\ndeleted (%d, %d)\n", temp->key, temp->data);
    }

    printf("\nlist after deleted:\n");
    print_list();

}
