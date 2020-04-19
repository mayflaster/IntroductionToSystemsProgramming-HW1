//
// Created by mayfl on 20/11/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "uniqueOrderedList.h"

typedef struct node{
    Element data;
    struct node* next;
} *Node;

struct uniqueOrderedList_t {
    Node node;
    int size;
    Node node_iterator;
    freeElements destroyElement;
    copyElements copyElement;
    elementGreaterThan elementGreaterFunction;
    elementsEquals elementsEqualsFunction;
};
/**
 * This function called by uniqueOrderedListInsert.
 * this function checks if the element need to be the head of the node.
 * if it does it itserts element to the head of the list and return true.
 * return false if not supposed to be the head of the node.
 */
static int addingElementToTheHeadOfList(UniqueOrderedList List, Node current,Node
new_node);

/**
 * This function called by uniqueOrderedListRemove.
 * it checks if the element that need to be removed, appears in the head of the list.
 * if so remove it and update the size of the list.
 * ** note that the internal iterator is not valid after calling this function.
 * return values:
 * True - in case of successful removal which means element exist in the head of the list
 * False- element does not exist in the head of the list
 */

static int removeTheHeadOfNode(UniqueOrderedList list,Element element);

/**
*Creates a new empty node.
* 	NULL - if allocations failed.
* 	A new node in case of success.
 */
static Node nodeCreate(){
    Node ptr = malloc(sizeof(*ptr));
    if(!ptr){
        return  NULL;
    }
    ptr->next = NULL;
    return ptr;
}


UniqueOrderedList uniqueOrderedListCreate(copyElements copyElement,
                                          freeElements destroyElement,
                                          elementsEquals elementsEqualsFunction, elementGreaterThan elementGreaterFunction){
    if(!copyElement || !destroyElement ||  !elementsEqualsFunction ||
       !elementGreaterFunction){
        return NULL;
    }
    UniqueOrderedList uniqueOrderedList = malloc(sizeof(*uniqueOrderedList));
    if(uniqueOrderedList == NULL){
        return NULL;
    }
    uniqueOrderedList->node = nodeCreate();
    if (!uniqueOrderedList->node){
        uniqueOrderedListDestroy(uniqueOrderedList);
        return NULL;
    }
    uniqueOrderedList->size =0;
    uniqueOrderedList->node_iterator = uniqueOrderedList->node;
    uniqueOrderedList->destroyElement = destroyElement;
    uniqueOrderedList->copyElement = copyElement;
    uniqueOrderedList->elementGreaterFunction = elementGreaterFunction;
    uniqueOrderedList->elementsEqualsFunction = elementsEqualsFunction;
    return uniqueOrderedList;
}




void uniqueOrderedListDestroy(UniqueOrderedList uniqueOrderedList){
    if (!uniqueOrderedList){
        return;
    }
    uniqueOrderedListClear(uniqueOrderedList);
    free(uniqueOrderedList->node);
    free(uniqueOrderedList);

}

UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList list){
    if (!list){
        return  NULL;
    }
    UniqueOrderedList new_list = uniqueOrderedListCreate(list->copyElement,
                                                         list->destroyElement, list->elementsEqualsFunction, list->elementGreaterFunction);
    if (new_list==NULL){
        return NULL;
    }
    Node current = list->node;
    while (current != NULL &&list->size!=0 ){
        if   (uniqueOrderedListInsert(new_list, current->data) ==
              UNIQUE_ORDERED_LIST_SUCCESS){
            current = current->next;
        }
        else {
            uniqueOrderedListDestroy(new_list);
            return NULL;
        }
    }
    return  new_list;
}

int uniqueOrderedListSize(UniqueOrderedList uniqueOrderedList){
    if (!uniqueOrderedList){
        return (-1);
    }
    int size =uniqueOrderedList->size;
    return size;

}

bool uniqueOrderedListContains(UniqueOrderedList uniqueOrderedList, Element
element){
    if (!uniqueOrderedList||!element||!uniqueOrderedList->size){
        return false;
    }
    Node temp_node = uniqueOrderedList->node;
    if (uniqueOrderedList->elementsEqualsFunction(temp_node->data, element)){
        return true;
    }
    while(temp_node->next){
        if (uniqueOrderedList->elementsEqualsFunction
                (temp_node->next->data, element)){
            return true;
        }
        else{
            temp_node = temp_node->next;
        }
    }
    return false;
}

/**
 * This function called by uniqueOrderedListRemove.
 * it checks if the element that need to be removed, appears in the head of the list.
 * if so remove it and update the size of the list.
 * ** note that the internal iterator is not valid after calling this function.
 * return values:
 * True - in case of successful removal which means element exist in the head of the list
 * False- element does not exist in the head of the list
 */

static int removeTheHeadOfNode(UniqueOrderedList list,Element element){
    if (list->elementsEqualsFunction(list->node->data,element)){
        list->destroyElement(list->node->data);
        if(list->size >1) {
            Node obsolete_node = list->node;
            list->node = obsolete_node->next;
            free(obsolete_node);
        }
        list->size--;
        list->node_iterator=list->node;
        return true;
    }
    return false;
}

UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList
                                                uniqueOrderedList, Element element) {
    if (!uniqueOrderedList || !element) {
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    if (!uniqueOrderedListContains(uniqueOrderedList, element)) {
        return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
    }
    if (removeTheHeadOfNode(uniqueOrderedList, element)) {
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    Node current = uniqueOrderedList->node;
    while (current->next) {
        if (!uniqueOrderedList->elementsEqualsFunction(current->next->data, element)) {
            current = current->next;
        } else {
            Node obsolete_node = current->next;
            Node temp_node= obsolete_node->next;
            current->next = temp_node;//temp_node->next->next;
            uniqueOrderedList->destroyElement(obsolete_node->data);
            free(obsolete_node);
            uniqueOrderedList->size--;
            break;
        }

    }
    uniqueOrderedList->node_iterator=uniqueOrderedList->node;
    return UNIQUE_ORDERED_LIST_SUCCESS;
}


Element uniqueOrderedListGetGreatest(UniqueOrderedList uniqueOrderedList){
    if (!uniqueOrderedList || !uniqueOrderedList->size ||
        !uniqueOrderedList->node->data) {
        return NULL;
    }
    Node current = uniqueOrderedList->node;
    while (current->next){
        current = current->next;
        uniqueOrderedList->node_iterator = current;
    }
    return uniqueOrderedList->node_iterator->data;

}

Element uniqueOrderedListGetNext(UniqueOrderedList uniqueOrderedList){
    if (!uniqueOrderedList||!uniqueOrderedList->size||!uniqueOrderedList->node
    ->next ||
        !(uniqueOrderedList->node_iterator->next)){
        return NULL;
    }
    uniqueOrderedList->node_iterator = uniqueOrderedList->node_iterator->next;
    return uniqueOrderedList->node_iterator->data;
}

void uniqueOrderedListClear(UniqueOrderedList uniqueOrderedList){
    if (!uniqueOrderedList ){
        return;
    }
    while (uniqueOrderedList->node->next){
        Element lowest = uniqueOrderedListGetLowest(uniqueOrderedList);
        uniqueOrderedListRemove(uniqueOrderedList, lowest);
    }
    Element lowest = uniqueOrderedListGetLowest(uniqueOrderedList);//*******88
    uniqueOrderedListRemove(uniqueOrderedList, lowest);//************
    uniqueOrderedList->size=0;
}

static int addingElementToTheHeadOfList(UniqueOrderedList List, Node current,Node
new_node)
{
    if (!List->elementGreaterFunction(new_node->data, current->data)) {
        new_node->next = current;
        List->node =new_node;
        List->size ++;
        return true;
    }
    return false;
}

UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList uniqueOrderedList, Element element)
{
    if (!uniqueOrderedList || !element){
        return 	UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    Element element1_copy = uniqueOrderedList->copyElement(element);
    if (uniqueOrderedList->size==0){
        uniqueOrderedList->node->data= element1_copy;
        uniqueOrderedList->size ++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    if (uniqueOrderedListContains(uniqueOrderedList, element1_copy)) {
        uniqueOrderedList->destroyElement(element1_copy);
        return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
    }
    Node current = uniqueOrderedList->node;
    Node new_node = nodeCreate();
    if (new_node == NULL){
        return UNIQUE_ORDERED_LIST_OUT_OF_MEMORY;
    }
    new_node->data = element1_copy;
    if(addingElementToTheHeadOfList(uniqueOrderedList,current,new_node)) {
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    while (current->next &&uniqueOrderedList->elementGreaterFunction
            (element, current->next->data)){
        current = current -> next;
    }
    new_node-> next = current -> next;
    current->next = new_node;
    uniqueOrderedList->size ++;
    return UNIQUE_ORDERED_LIST_SUCCESS;
}

Element uniqueOrderedListGetLowest(UniqueOrderedList list)
{
    if (list == NULL || !list->size)
        return NULL;
    list->node_iterator = list->node;
    return  list->node->data;
}