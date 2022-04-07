/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/hash/hash.h>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <compare>
#include <string>
#include <string_view>
#include <utility>

#include "roq/compat.hpp"
#include "roq/exceptions.hpp"

namespace roq {

// fixed length string buffer
//! A fixed-length string buffer with automatic conversion to/from
//! \ref std::string_view.
/*!
 * This is useful for managing fixed length arrays as if they were string-like.
 * Typically used to avoid heap allocations, e.g. stack allocated structs used
 * for messaging.

 * This is a higher level abstraction than \ref std::array to provide
 * more * string-specific features.
 * The interface is a subset of \ref std::string and \ref std::string_view.
 */
template <std::size_t N>
class ROQ_PACKED String {
 public:
  using value_type = char;

  constexpr String() = default;

  // cppcheck-suppress noExplicitConstructor
  constexpr String(const std::string_view &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr String(const std::string &text) {  // NOLINT (allow implicit)
    copy(text);
  }

  // cppcheck-suppress noExplicitConstructor
  constexpr String(value_type const *text)  // NOLINT (allow implicit)
      : String(std::string_view{text}) {}

  constexpr String &operator=(const std::string_view &text) {
    copy(text);
    return *this;
  }

  constexpr String &operator=(const std::string &text) {
    copy(text);
    return *this;
  }

  // abseil hash (heterogeneous lookup)
  template <typename H>
  friend H AbslHashValue(H hash, const String<N> &rhs) {
    return H::combine(std::move(hash), static_cast<std::string_view>(rhs));
  }

  // note! clang13 does not yet support spaceship operator for std::string_view
  // https://libcxx.llvm.org/Status/Spaceship.html
#if defined(__clang__)
 protected:
  // references:
  //   https://stackoverflow.com/a/4609795
  template <typename T>
  static constexpr int sign_helper(T value) {
    return (T{} < value) - (value < T{});
  }

 public:
  constexpr bool operator==(const std::string_view &rhs) const {
    return static_cast<std::string_view>(*this).compare(rhs) == 0;
  }
  constexpr auto operator<=>(const std::string_view &rhs) const {
    auto lhs = static_cast<std::string_view>(*this);
    auto sign = lhs.compare(rhs);
    const std::array lookup{
        std::strong_ordering::less,
        std::strong_ordering::equal,
        std::strong_ordering::greater,
    };
    return lookup[sign_helper(sign) + 1];
  }
#else
  constexpr bool operator==(const std::string_view &rhs) const { return static_cast<std::string_view>(*this) == rhs; }
  constexpr auto operator<=>(const std::string_view &rhs) const { return static_cast<std::string_view>(*this) <=> rhs; }
#endif

  constexpr auto operator<=>(const String<N> &rhs) const { return operator<=>(static_cast<std::string_view>(rhs)); }

  constexpr auto operator<=>(const std::string &rhs) const { return operator<=>(std::string_view{rhs}); }

  constexpr value_type &operator[](size_t index) { return buffer_[index]; }

  constexpr value_type operator[](size_t index) const { return buffer_[index]; }

  constexpr std::size_t size() const { return N; }

  constexpr std::size_t length() const {
    auto tmp = buffer_[N - 1];
    if (buffer_[N - 2])
      return N - (tmp ? 0 : 1);
    return tmp;
  }

  constexpr bool empty() const { return length() == 0; }

  constexpr value_type const *data() const { return std::data(buffer_); }

  constexpr operator std::string_view() const { return {data(), length()}; }

  constexpr void clear() {
    // note!
    // we prefer to clear the entire buffer (for security reasons)
    // even though it would be enough to only set the first element
    std::fill(std::begin(buffer_), std::end(buffer_), '\0');
  }

  constexpr void push_back(value_type value) {
    using namespace std::literals;
    auto len = length();
    if (N <= len) [[unlikely]]
      throw LengthError("String buffer is full"sv);
    buffer_[len] = value;
    set_length(++len);
  }

 protected:
  constexpr value_type *data() { return std::data(buffer_); }

  constexpr void copy(const std::string_view &text) {
    using namespace std::literals;
    auto len = std::size(text);
    if (N < len) [[unlikely]]
      throw LengthError(R"(can't copy: len(text="{}")={} exceeds size={})"sv, text, len, N);
    std::copy(std::begin(text), std::end(text), std::begin(buffer_));
    set_length(len);
  }

  constexpr void set_length(size_t len) {
    if (len < N) {
      value_type last;
      if (len < (N - 1)) {
        buffer_[N - 2] = '\0';
        last = len;
      } else {
        last = '\0';
      }
      buffer_[N - 1] = last;
    }
  }

 private:
  std::array<value_type, N> buffer_ = {};
};

}  // namespace roq

template <size_t N>
struct fmt::formatter<roq::String<N> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::String<N> &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
