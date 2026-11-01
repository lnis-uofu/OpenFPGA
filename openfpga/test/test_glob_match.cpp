/********************************************************************
 * Unit test of the shell-style matching of names. On the platforms that have
 * fnmatch(), every case is also checked against it, and so are many patterns
 * made at random
 *******************************************************************/
#include <cstdio>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

#include "glob_match.h"

#ifndef _WIN32
#include <fnmatch.h>
#endif

using openfpga::glob_match;

static int num_failures = 0;

static bool reference(const std::string& pattern, const std::string& text,
                      const bool expected) {
#ifndef _WIN32
  (void)expected;
  return (0 == fnmatch(pattern.c_str(), text.c_str(), 0));
#else
  return expected;
#endif
}

static void check(const std::string& pattern, const std::string& text,
                  const bool expected) {
  const bool got = glob_match(pattern, text);
  if (got != expected) {
    std::fprintf(stderr, "glob_match('%s', '%s') is %d, expected %d\n",
                 pattern.c_str(), text.c_str(), got, expected);
    ++num_failures;
  }
  if (reference(pattern, text, expected) != expected) {
    std::fprintf(stderr,
                 "the case ('%s', '%s') is not what fnmatch() says: %d\n",
                 pattern.c_str(), text.c_str(), !expected);
    ++num_failures;
  }
}

static void test_cases() {
  /* Literals */
  check("abc", "abc", true);
  check("abc", "abd", false);
  check("abc", "ab", false);
  check("ab", "abc", false);
  check("", "", true);
  check("", "a", false);
  check("a", "", false);

  /* ? */
  check("a?c", "abc", true);
  check("a?c", "ac", false);
  check("???", "abc", true);
  check("???", "ab", false);

  /* * */
  check("*", "", true);
  check("*", "anything", true);
  check("a*", "a", true);
  check("a*", "abc", true);
  check("*c", "abc", true);
  check("*c", "abd", false);
  check("a*c", "abbbc", true);
  check("a*c", "abbbd", false);
  check("a*b*c", "aXbYc", true);
  check("a*b*c", "aXcYb", false);
  check("**", "", true);
  check("n1*", "n1", true);
  check("n1*", "n12", true);
  check("n1*", "n2", false);
  check("*a*", "bab", true);
  check("*a*a", "aba", true);
  check("*a*a", "ab", false);
  /* '/' and '.' are not special */
  check("*", "a/b", true);
  check("*", ".hidden", true);
  check("?", "/", true);

  /* Brackets */
  check("[abc]", "b", true);
  check("[abc]", "d", false);
  check("[a-c]", "b", true);
  check("[a-c]", "d", false);
  check("[!a-c]", "d", true);
  check("[!a-c]", "b", false);
  check("[^a-c]", "d", true);
  check("[^a-c]", "b", false);
  check("[]]", "]", true);
  check("[]a]", "a", true);
  check("[!]]", "a", true);
  check("[!]]", "]", false);
  check("[a-]", "-", true);
  check("[a-]", "a", true);
  check("[a-]", "b", false);
  check("[-a]", "-", true);
  check("n[0-9]*", "n12", true);
  check("n[0-9]*", "nx", false);
  check("[a-cx-z]", "y", true);
  check("[a-cx-z]", "m", false);
  check("[[:digit:]]", "5", true);
  check("[[:digit:]]", "a", false);
  check("[[:alpha:]_]", "_", true);
  check("[[:alpha:][:digit:]]", "7", true);
  check("[![:space:]]", "a", true);
  check("[![:space:]]", " ", false);

  /* A bracket that is not closed is the character '[' */
  check("[", "[", true);
  check("[abc", "[abc", true);
  check("[abc", "a", false);
  check("a[", "a[", true);

  /* Escapes */
  check("\\*", "*", true);
  check("\\*", "a", false);
  check("a\\?c", "a?c", true);
  check("a\\?c", "abc", false);
  check("\\[a]", "[a]", true);
  check("[\\]]", "]", true);
  check("[\\a-c]", "b", true);
  check("a\\\\b", "a\\b", true);

  /* Names that look like those of the netlist */
  check("top^clk*", "top^clk_0", true);
  check("*.out", "dff.out", true);
  check("n[0-9][0-9]", "n12", true);
  check("n[0-9][0-9]", "n1", false);
}

/* Patterns made of the pieces of the syntax, against texts of a few letters */
static void test_random_against_fnmatch() {
#ifndef _WIN32
  std::mt19937 generator(12345);
  const std::vector<std::string> pieces = {
    "a",     "b",    "c",   "*",   "?",          "[ab]", "[!a]",
    "[a-c]", "[^b]", "\\*", "\\a", "[",          "]",    "[]a]",
    "[a-]",  "-",    ".",   "/",   "[[:alpha:]]"};
  const std::string alphabet = "abc*[]-./";
  for (int i = 0; i < 20000; ++i) {
    std::string pattern;
    const int length = 1 + static_cast<int>(generator() % 5);
    for (int j = 0; j < length; ++j) {
      pattern += pieces[generator() % pieces.size()];
    }
    std::string text;
    const int text_length = static_cast<int>(generator() % 6);
    for (int j = 0; j < text_length; ++j) {
      text += alphabet[generator() % alphabet.size()];
    }
    const bool expected = (0 == fnmatch(pattern.c_str(), text.c_str(), 0));
    if (glob_match(pattern, text) != expected) {
      std::fprintf(stderr, "glob_match('%s', '%s') is not %d as fnmatch()\n",
                   pattern.c_str(), text.c_str(), expected);
      if (++num_failures > 20) {
        return;
      }
    }
  }
#endif
}

int main() {
  test_cases();
  test_random_against_fnmatch();
  if (0 != num_failures) {
    std::fprintf(stderr, "%d check(s) failed\n", num_failures);
    return 1;
  }
  std::printf("All glob_match tests passed\n");
  return 0;
}
