#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <Arduino.h>

class LinkedList
{
  public:
    LinkedList(int maxSize);

    void push(uint8_t* data);
    uint8_t* pull();

  private:
    int m_maxSize;
    int m_currentSize;

    struct Node
    {
      uint8_t* data;
      struct Node** next;
      struct Node** prev;
    };

    struct Node** m_head;
    struct Node** m_tail;
};

#endif