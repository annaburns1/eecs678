/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.

  Assumptions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements. If comparer(x, y) < 0, then the priority of x is higher than the priority of y and therefore should be placed earlier in the queue.
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
  q->size = 0;
  q->head = NULL;
  q->comparer = comparer;

}


/**
  Insert the specified element into this priority queue. You should use the queue's `comparer` function to determine where to place the item.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{

  _node *new_node = malloc(sizeof(_node*));
  new_node->ptr = ptr;

  if(q->size == 0) {
    new_node->next = NULL;
    q->head = new_node;
    q->size++;
    return 0;
  }
  else {
    _node *temp = q->head;
    _node *temp_before;
    for (size_t i = 0; i < q->size; i++) {
      if(temp == NULL) {
        temp_before->next = new_node;
        new_node->next = NULL;
        q->size++;
        return i;
      }
      if(q->comparer(temp->ptr, ptr) > 0) {
        if(temp == q->head) {
          new_node->next = temp;
          q->head = new_node;
          q->size++;
          return 0;
        }
        else {
          temp_before->next = new_node;
          new_node->next = temp;
          q->size++;
          return i;
        }
      }

      temp_before = temp;
      temp = temp->next;
    }
    // free(temp);
    // free(temp_before);

  }

  free(new_node);
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.

  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
  if(q->size == 0) {
    return NULL;
  }
  else {
      return q->head->ptr;
  }
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
  if(q->size == 0) {
    return NULL;
  }
  else {
      _node *temp = q->head;
      // _node *new_head = q->head->next;
      q->head = temp->next;
      temp->next = NULL;
      void *ptr = temp->ptr;
      q->size--;
      free(temp);

      return ptr;
  }
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.

  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
  if(q->size < index) {
    return NULL;
  }
  else {
      _node *temp = q->head;

      for (size_t i = 0; i < index; i++) {
        temp = temp->next;
      }
      void *ptr = temp->ptr;

      return ptr;
  }
}


/**
  Removes all instances of ptr from the queue.

  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
  int num = 0;
  _node *temp = q->head;
  _node *temp_before;

  for (size_t i = 0; i < q->size; i++) {
    if(temp->ptr == ptr) {
      if(temp == q->head) {
        q->head = temp->next;
        free(temp);
        temp = q->head;
        num++;
      }
      else {
        temp_before->next = temp->next;
        free(temp);
        temp = temp_before->next;
        num++;
      }
    }
    else {
      temp_before = temp;
      temp = temp->next;
    }
  }

  q->size -= num;

	return num;
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.

  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{

  if(q->size < index) {
    return NULL;
  }
  else {
      _node *temp_before = priqueue_at(q, index-1);
      _node *temp = priqueue_at(q, index);
      _node *temp_next = temp->next;

      if(temp_before != NULL) {
        temp_before->next = temp_next;
      }

      temp->next = NULL;
      q->size--;
      free(temp);

      return temp;
  }

}


/**
  Return the number of elements in the queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
	return q->size;
}


/**
  Destroys and frees all the memory associated with q.

  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
  _node *temp = q->head;
  _node *temp_before;
  for (size_t i = 0; i < q->size; i++) {
    temp_before = temp;
    temp = temp->next;
    free(temp_before);
  }
  //free(temp);
}
