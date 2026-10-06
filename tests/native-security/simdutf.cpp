#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include "../../pkg/simdutf/vendor/simdutf.h"
int main() {
  for (auto impl : simdutf::get_available_implementations()) {
    if (!impl->supported_by_runtime_system()) continue;
    for (size_t n = 0; n < 1025; ++n) {
      std::string input(n, 'a');
      for (size_t i = 0; i < n; ++i) input[i] = char(32 + i % 95);
      std::vector<char16_t> u16(n+1, 0xdead);
      std::vector<char32_t> u32(n+1, 0xdead);
      assert(impl->convert_utf8_to_utf16le(input.data(), n, u16.data()) == n);
      assert(impl->convert_utf8_to_utf32(input.data(), n, u32.data()) == n);
      for (size_t i = 0; i < n; ++i) { assert(u16[i] == input[i]); assert(u32[i] == input[i]); }
      assert(u16[n] == 0xdead && u32[n] == 0xdead);
      auto valid = impl->validate_utf32_with_errors(u32.data(), n);
      assert(valid.error == simdutf::error_code::SUCCESS && valid.count == n);
      if (n) {
        u32[n-1] = 0x110000;
        auto invalid = impl->validate_utf32_with_errors(u32.data(), n);
        assert(invalid.error == simdutf::error_code::TOO_LARGE && invalid.count == n-1);
      }
    }
    std::printf("ASCII widening and UTF-32 validation passed: %s\n", impl->name().c_str());
  }
}
