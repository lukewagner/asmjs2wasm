// vim: set ts=2 sw=2 tw=99 et:

#ifndef list_h__
#define list_h__

template <class T>
class List
{
  T* first_;
  T* last_;

public:
  List() : first_(nullptr), last_(nullptr) {}
  bool empty() const { return first_ == last_; }
  T& first() const { return *first_; }
  T& last() const { return *last_; }

  uint32_t compute_length() const
  {
    uint32_t len = 0;
    for (T* p = first_; p; p = p->next)
      len++;
    return len;
  }

  void append(T& n)
  {
    if (!first_) {
      first_ = last_ = &n;
    } else {
      last_->next = &n;
      last_ = &n;
    }
  }

  void append_and_steal(List<T>& list)
  {
    if (!first_)
      first_ = list.first_;
    else
      last_->next = list.first_;
    last_ = list.last_;
    list.first_ = list.last_ = nullptr;
  }
};

template <class T>
class ListElem
{
  friend class List<T>;
  T* next;

public:
  ListElem() : next(nullptr) {}
};

#endif // defined list_h__
