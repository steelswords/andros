#ifndef _ANDROS_CIRCULAR_BUFFER_HPP
#define _ANDROS_CIRCULAR_BUFFER_HPP

#include "stddef.h" //For size_t
#include "mem/memory.hpp"

#define CIRCULAR_BUFFER_DEFAULT_SIZE 512

template <class T>
class CircularBuffer
{
public:
  bool add(T element); //Returns false if full
  T get();
  size_t sizeUsed();
  bool isFull(); // Returns true if there is no more room in the buffer
  bool isEmpty(); // Returns true if there are no elements in the buffer
  CircularBuffer<T>(size_t bufferSize);
  CircularBuffer<T>();
  T m_defaultValue;
//private:
  T* m_head;
  T* m_tail;
  T* m_beginning {0};
  T* m_end {0};
  size_t m_sizeUsed;
  size_t m_bufferSize;
  
};

template <class T>
CircularBuffer<T>::CircularBuffer(size_t bufferSize)
  : m_bufferSize(bufferSize),
    m_sizeUsed(0),
    m_defaultValue((T)0)
{
  m_head = (T*) malloc(bufferSize * sizeof(T));
  m_tail = nullptr;
  m_beginning = m_head;
  m_end  = m_head + bufferSize;
}

template <class T>
CircularBuffer<T>::CircularBuffer()
  : m_bufferSize(CIRCULAR_BUFFER_DEFAULT_SIZE),
    m_sizeUsed(0),
    m_defaultValue((T)0)
{
  size_t bufferSize = CIRCULAR_BUFFER_DEFAULT_SIZE;
  m_head = (T*) malloc(bufferSize * sizeof(T));
  m_tail = nullptr;
  m_beginning = m_head;
  m_end  = m_head + bufferSize;
}


template <class T>
bool CircularBuffer<T>::add(T element)
{
    
  // If head = end, and tail != beginning
  if ((m_head == m_end) && (m_tail != m_beginning))
  {
    // Make head = beginning.
    *m_head = element;
    m_sizeUsed++;
    m_head = m_beginning;
  }
  // Else if head != end and head +1 != tail,
  else if ((m_head != m_end) && (m_head +1 != m_tail))
  {
    // make head = head + 1
    *m_head = element;
    m_sizeUsed++;
    m_head = m_head + 1;
    //For the beginning, if tail = nullptr
    if (m_tail == nullptr)
    {
      m_tail = m_beginning;
    }
  }
  else // The buffer is full
  {
    return false;
  }
  return true;
}


template <class T>
T CircularBuffer<T>::get()
{
  T returnValue;
  // If tail = nullptr, then throw an error.
  if (m_tail == nullptr)
  {
    return m_defaultValue;
  }
  // Else,
  else
  {
    // If tail = end and head != beginning, tail = beginning
    if ((m_tail == m_end) && (m_head != m_beginning))
    {
      returnValue = *m_tail;
      m_sizeUsed--;
      m_tail = m_beginning;
    }
    // Else if tail != end and tail != head
    else if ((m_tail != m_end) && (m_tail != m_head))
    {
      returnValue = *m_tail;
      m_sizeUsed--;
      m_tail++;
    }
    else
    {
      returnValue = m_defaultValue;
    }
  }
  return returnValue;
}

template <class T>
bool CircularBuffer<T>::isFull()
{
  return m_sizeUsed == m_bufferSize;
}

template <class T>
bool CircularBuffer<T>::isEmpty()
{
  return m_sizeUsed == 0;
}

template <class T>
size_t CircularBuffer<T>::sizeUsed()
{
  return m_sizeUsed;
}

#endif
