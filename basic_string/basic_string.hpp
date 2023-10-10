#include <cstring>
#include <memory>
#include <string>

template <typename Char, typename Traits = std::char_traits<Char>,
          typename Allocator = std::allocator<Char>>
class basic_string {
  using size_type = typename std::allocator_traits<Allocator>::size_type;

public:
  basic_string() noexcept {}

  basic_string(const char *str) {
    auto size = std::char_traits<char>::length(str) + 1;
    auto *buffer = std::allocator_traits<Allocator>::allocate(allocator_, size);
    std::strncpy(buffer, str, size);
    size_ = size;
    str_ = buffer;
  }

  ~basic_string() {
    std::allocator_traits<Allocator>::deallocate(allocator_, str_, size_);
  }

private:
  Char *str_{nullptr};
  size_type size_{0};
  Allocator allocator_;
};
