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

    str_ = buffer;
    size_ = size;
  }

  template <typename Allocator_>
  basic_string(const basic_string<Char, Traits, Allocator_> &str) {
    if (!str.size())
      return;
    copy_assign_(str);
  }

  template <typename Allocator_>
  basic_string &operator=(const basic_string<Char, Traits, Allocator_> &str) {
    copy_assign_(str);
    return *this;
  }

  template <typename Allocator_>
  basic_string(basic_string<Char, Traits, Allocator_> &&str) {
    move_assign_(str);
  }

  template <typename Allocator_>
  basic_string &operator=(basic_string<Char, Traits, Allocator_> &&str) {
    if (this == std::addressof(str))
      return *this;

    move_assign_(str);
    return *this;
  }

  template <typename Allocator_>
  bool
  operator==(const basic_string<Char, Traits, Allocator_> &str) const noexcept {
    if (size_ != str.size_)
      return false;
    for (size_type i = 0; i != size_; ++i) {
      if (!Traits::eq(str_[i], str.str_[i]))
        return false;
    }
    return true;
  }

  ~basic_string() {
    std::allocator_traits<Allocator>::deallocate(allocator_, str_, size_);
  }

private:
  template <typename Allocator_>
  void copy_assign_(const basic_string<Char, Traits, Allocator_> &str) {
    auto *buffer =
        std::allocator_traits<Allocator>::allocate(allocator_, str.size_);
    Traits::move(buffer, str, str.size_);

    str_ = buffer;
    size_ = str.size_;
  }

  template <typename Allocator_>
  void move_assign_(const basic_string<Char, Traits, Allocator_> &str) {
    str_ = std::move(str.str_);
    size_ = std::move(str.size_);
    allocator_ = std::move(str.allocator_);
  }

  Char *str_{nullptr};
  size_type size_{0};
  Allocator allocator_;
};
