#ifndef _ANDROS_LINKED_LIST_
#define _ANDROS_LINKED_LIST_ 1

template<class T>
LinkedList<T>
{
public:
  LinkedList<T>* next()     { return m_next; }
  LinkedList<T>* previous() { return m_prev; }
  LinkedList<T>* last();
  //TODO: first()
  bool isEnd()      { return (m_next == nullptr);}
  bool isBegining() { return (m_prev == nullptr); }
  size_t getSize();
  void append(T* node);
  void prepend(T* node);
protected:
  T* m_node {nullptr};
  LinkedList<T>* m_next {nullptr};
  LinkedList<T>* m_prev{nullptr};
};

template <class T>
size_t T* LinkedList::getSize()
{
  T* cur = m_node;
  size_t size = 0; 
  while (!cur->isEnd())
  {
    size++;
    cur = cur->next();
  }
}

template <class T>
LinkedList<T>* LinkedList::last()
{
  LinkedList<T>* cur = m_node;
  LinkedList<T>* prev = m_prev;
  if (cur == nullptr)
  {
    return nullptr;
  }
  while (!cur->isEnd())
  {
    prev = cur;
    cur = cur->next();
  }
  return cur;
}

template<class T>
void LinkedList::apend(T* node)
{
  T* lastNode = last();
  lastNode->m_next = node;
  node->m_prev = lastNode;
  node->
}

#endif
