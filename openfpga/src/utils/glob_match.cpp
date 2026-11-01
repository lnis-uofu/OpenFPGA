#include "glob_match.h"

#include <cctype>
#include <cstring>

/* Begin namespace openfpga */
namespace openfpga {

namespace {

/* Return true if the character is in the class that is named, e.g., "alpha".
 * An unknown name matches nothing */
bool in_class(const std::string& name, const unsigned char ch) {
  if ("alpha" == name) return 0 != std::isalpha(ch);
  if ("digit" == name) return 0 != std::isdigit(ch);
  if ("alnum" == name) return 0 != std::isalnum(ch);
  if ("upper" == name) return 0 != std::isupper(ch);
  if ("lower" == name) return 0 != std::islower(ch);
  if ("space" == name) return 0 != std::isspace(ch);
  if ("punct" == name) return 0 != std::ispunct(ch);
  if ("xdigit" == name) return 0 != std::isxdigit(ch);
  if ("blank" == name) return ' ' == ch || '\t' == ch;
  if ("cntrl" == name) return 0 != std::iscntrl(ch);
  if ("graph" == name) return 0 != std::isgraph(ch);
  if ("print" == name) return 0 != std::isprint(ch);
  return false;
}

/* Match one character against the bracket expression that starts at
 * pattern[start], which is the '['. Return false if the expression is not
 * closed. Otherwise, matched tells if the character is in it, and end is the
 * position after the closing ']' */
bool match_bracket(const std::string& pattern, const size_t start,
                   const unsigned char ch, bool& matched, size_t& end) {
  size_t pos = start + 1;
  bool negate = false;
  if (pos < pattern.size() && ('!' == pattern[pos] || '^' == pattern[pos])) {
    negate = true;
    ++pos;
  }
  bool found = false;
  bool first = true;
  while (pos < pattern.size()) {
    if (']' == pattern[pos] && !first) {
      matched = found != negate;
      end = pos + 1;
      return true;
    }
    first = false;
    if ('[' == pattern[pos] && pos + 1 < pattern.size() &&
        ':' == pattern[pos + 1]) {
      const size_t close = pattern.find(":]", pos + 2);
      if (std::string::npos != close) {
        found = found || in_class(pattern.substr(pos + 2, close - pos - 2), ch);
        pos = close + 2;
        continue;
      }
    }
    unsigned char low = static_cast<unsigned char>(pattern[pos]);
    if ('\\' == low && pos + 1 < pattern.size()) {
      ++pos;
      low = static_cast<unsigned char>(pattern[pos]);
    }
    ++pos;
    unsigned char high = low;
    /* A range, unless the '-' is the last character of the expression */
    if (pos + 1 < pattern.size() && '-' == pattern[pos] &&
        ']' != pattern[pos + 1]) {
      ++pos;
      high = static_cast<unsigned char>(pattern[pos]);
      if ('\\' == high && pos + 1 < pattern.size()) {
        ++pos;
        high = static_cast<unsigned char>(pattern[pos]);
      }
      ++pos;
    }
    found = found || (low <= ch && ch <= high);
  }
  return false;
}

}  // End anonymous namespace

bool glob_match(const std::string& pattern, const std::string& text) {
  size_t pat = 0;
  size_t txt = 0;
  /* Where to go back to when a match fails: after the last '*' */
  size_t star_pat = std::string::npos;
  size_t star_txt = 0;
  while (txt < text.size()) {
    bool advanced = false;
    if (pat < pattern.size()) {
      const char token = pattern[pat];
      const unsigned char ch = static_cast<unsigned char>(text[txt]);
      if ('*' == token) {
        star_pat = ++pat;
        star_txt = txt;
        continue;
      }
      if ('?' == token) {
        ++pat;
        ++txt;
        advanced = true;
      } else if ('[' == token) {
        bool matched = false;
        size_t end = 0;
        if (match_bracket(pattern, pat, ch, matched, end)) {
          if (matched) {
            pat = end;
            ++txt;
            advanced = true;
          }
        } else if ('[' == text[txt]) {
          /* Not closed: it is the character '[' */
          ++pat;
          ++txt;
          advanced = true;
        }
      } else {
        size_t literal = pat;
        if ('\\' == token && pat + 1 < pattern.size()) {
          literal = pat + 1;
        }
        if (pattern[literal] == text[txt]) {
          pat = literal + 1;
          ++txt;
          advanced = true;
        }
      }
    }
    if (advanced) {
      continue;
    }
    /* A mismatch: let the last '*' take one more character */
    if (std::string::npos == star_pat) {
      return false;
    }
    pat = star_pat;
    txt = ++star_txt;
  }
  /* What is left of the pattern must be '*' only */
  while (pat < pattern.size() && '*' == pattern[pat]) {
    ++pat;
  }
  return pat == pattern.size();
}

} /* End namespace openfpga */
