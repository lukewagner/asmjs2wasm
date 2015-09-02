// vim: set ts=2 sw=2 tw=99 et:

#ifndef memory_h__
#define memory_h__

#include <memory>
#include <vector>

namespace memory {

class Pool
{
  std::vector<std::unique_ptr<uint8_t[]>> allocs_;
  uint8_t* free_;
  size_t remain_;

public:
  Pool() : free_(nullptr), remain_(0) {}

  void* alloc(size_t bytes)
  {
    if (remain_ < bytes) {
      remain_ = std::max<size_t>(bytes, 64 * 1024);
      allocs_.emplace_back(new uint8_t[remain_]);
      free_ = allocs_.back().get();
    }

    uint8_t* ret = free_;
    free_ += bytes;
    remain_ -= bytes;
    return ret;
  }

};

class Base
{
  void* operator new(size_t bytes) = delete;

public:
  void* operator new(size_t bytes, Pool& mem) { return mem.alloc(bytes); }
};

}  // namespace memory
#endif  // defined memory_h__
