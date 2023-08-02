#include "LinkedList.h"

LinkedList::LinkedList(int maxSize)
{
  m_maxSize = maxSize;
}

void LinkedList::push(uint8_t data) 
{
  if (m_currentSize >= m_maxSize) {
    // Overflow
  }

  struct Node* newNode;
  (*m_head)->prev = &newNode;

  newNode->data = data;
  newNode->next = m_head;

  m_head = &newNode;

  m_currentSize++;
}

uint8_t LinkedList::pull() 
{
  if (m_currentSize <= 0) {
    // Underflow
  }

  struct Node* newTail;
  newTail->data = (*(*m_tail)->prev)->data;
  newTail->prev = (*(*m_tail)->prev)->prev;

  uint8_t data = (*m_tail)->data;
  delete m_tail;
  m_tail = &newTail;

  m_currentSize--;

  return (*m_tail)->data;
}