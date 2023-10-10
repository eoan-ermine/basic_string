#include <cstring>
#include <memory>
#include <string>
#include <utility>

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

  template <typename Allocator_>
  basic_string(const basic_string<Char, Traits, Allocator_> &str) {
    if (!str.size())
      return;

    auto *buffer =
        std::allocator_traits<Allocator>::allocate(allocator_, str.size_);
    std::strncpy(buffer, str, str.size_);

    size_ = str.size_;
    str_ = buffer;
  }

  template <typename Allocator_>
  basic_string &operator=(const basic_string<Char, Traits, Allocator_> &str) {
    *this = std::move(basic_string(str));
    return *this;
  }

  template <typename Allocator_>
  basic_string(basic_string<Char, Traits, Allocator_> &&str) {
    str_ = std::move(str.str_);
    size_ = std::move(str.size_);
    allocator_ = std::move(str.allocator_);
  }

  template <typename Allocator_>
  basic_string &operator=(basic_string<Char, Traits, Allocator_> &&str) {
    if (this == std::addressof(str))
      return *this;

    str_ = std::move(str.str_);
    size_ = std::move(str.size_);
    allocator_ = std::move(str.allocator_);

    return *this;
  }

  ~basic_string() {
    std::allocator_traits<Allocator>::deallocate(allocator_, str_, size_);
  }

private:
  Char *str_{nullptr};
  size_type size_{0};
  Allocator allocator_;
};
