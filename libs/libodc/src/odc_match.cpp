/********************************************************************
 * Matching objects (R-11 to R-13): the patterns, globs or regular
 * expressions matched against the whole name, and the -filter expressions.
 *******************************************************************/
#include <algorithm>
#include <cctype>
#include <memory>
#include <regex>

#include "odc.h"

/* begin namespace odc */
namespace odc {

namespace {

bool is_separator(const char c) { return '.' == c || '/' == c; }

char fold(const char c, const bool nocase) {
  return nocase ? static_cast<char>(std::tolower(static_cast<unsigned char>(c)))
                : c;
}

bool glob_at(const std::string& pattern, size_t p, const std::string& name,
             size_t n, const bool hierarchical, const bool nocase) {
  while (p < pattern.size()) {
    const char c = pattern[p];
    if ('*' == c) {
      /* '*' takes as much as it can, back to nothing */
      size_t end = n;
      while (end < name.size() && (hierarchical || !is_separator(name[end]))) {
        ++end;
      }
      for (size_t k = end + 1; k-- > n;) {
        if (glob_at(pattern, p + 1, name, k, hierarchical, nocase)) {
          return true;
        }
      }
      return false;
    }
    if (n >= name.size()) {
      return false;
    }
    if ('?' == c) {
      if (!hierarchical && is_separator(name[n])) {
        return false;
      }
    } else if (fold(c, nocase) != fold(name[n], nocase)) {
      return false;
    }
    ++p;
    ++n;
  }
  return n == name.size();
}

/* The -filter expressions --------------------------------------------- */

struct Token {
  enum Kind { WORD, OP, AND, OR, OPEN, CLOSE, END } kind = END;
  std::string text;
};

std::vector<Token> tokenize(const std::string& text, std::string& error) {
  std::vector<Token> tokens;
  size_t i = 0;
  while (i < text.size()) {
    const char c = text[i];
    if (' ' == c || '\t' == c) {
      ++i;
      continue;
    }
    const std::string two = text.substr(i, 2);
    if ("&&" == two || "||" == two) {
      tokens.push_back({"&&" == two ? Token::AND : Token::OR, two});
      i += 2;
    } else if ("==" == two || "!=" == two || "=~" == two || "!~" == two) {
      tokens.push_back({Token::OP, two});
      i += 2;
    } else if ('(' == c || ')' == c) {
      tokens.push_back({'(' == c ? Token::OPEN : Token::CLOSE, {c}});
      ++i;
    } else if ('"' == c) {
      const size_t end = text.find('"', i + 1);
      if (std::string::npos == end) {
        error = "a quote is not closed in the filter '" + text + "'";
        return {};
      }
      tokens.push_back({Token::WORD, text.substr(i + 1, end - i - 1)});
      i = end + 1;
    } else {
      size_t end = i;
      while (end < text.size() && ' ' != text[end] && '\t' != text[end] &&
             '(' != text[end] && ')' != text[end] &&
             std::string::npos == std::string("=!&|").find(text[end])) {
        ++end;
      }
      if (end == i) {
        error = "'" + text.substr(i) + "' cannot be read in the filter '" +
                text + "'";
        return {};
      }
      tokens.push_back({Token::WORD, text.substr(i, end - i)});
      i = end;
    }
  }
  tokens.push_back({Token::END, ""});
  return tokens;
}

std::string lower(std::string text) {
  for (char& c : text) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }
  return text;
}

/* The values of a direction, as a port and the I/O map write them */
std::string direction(const std::string& value) {
  const std::string text = lower(value);
  if ("input" == text) {
    return "in";
  }
  if ("output" == text) {
    return "out";
  }
  return text;
}

/* A filter, parsed: a tree of comparisons */
struct Node {
  enum Kind { COMPARE, AND, OR } kind = COMPARE;
  std::string property;
  std::string op;
  std::string value;
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;

  bool eval(const Object& object) const {
    if (AND == kind) {
      return left->eval(object) && right->eval(object);
    }
    if (OR == kind) {
      return left->eval(object) || right->eval(object);
    }
    const auto found = object.properties.find(property);
    std::string actual =
      object.properties.end() == found ? std::string() : found->second;
    std::string expected = value;
    if ("direction" == property || "pin_direction" == property) {
      actual = direction(actual);
      expected = direction(expected);
    }
    bool equal = false;
    if ("==" == op || "!=" == op) {
      equal = lower(actual) == lower(expected);
    } else {
      equal = glob_match(expected, actual, true, true);
    }
    return ('=' == op[0]) == equal;
  }
};

class FilterParser {
 public:
  FilterParser(const std::vector<Token>& tokens, const std::string& kind)
    : tokens_(tokens), kind_(kind) {}

  std::unique_ptr<Node> parse(std::string& error) {
    std::unique_ptr<Node> node = parse_or();
    if (error_.empty() && Token::END != peek().kind) {
      error_ = "'" + peek().text + "' is not expected";
    }
    error = error_;
    return error_.empty() ? std::move(node) : nullptr;
  }

 private:
  const Token& peek() const { return tokens_[pos_]; }
  const Token& next() {
    return tokens_[pos_ < tokens_.size() - 1 ? pos_++ : pos_];
  }

  std::unique_ptr<Node> parse_or() {
    std::unique_ptr<Node> left = parse_and();
    while (error_.empty() && Token::OR == peek().kind) {
      next();
      auto node = std::make_unique<Node>();
      node->kind = Node::OR;
      node->left = std::move(left);
      node->right = parse_and();
      left = std::move(node);
    }
    return left;
  }

  std::unique_ptr<Node> parse_and() {
    std::unique_ptr<Node> left = parse_atom();
    while (error_.empty() && Token::AND == peek().kind) {
      next();
      auto node = std::make_unique<Node>();
      node->kind = Node::AND;
      node->left = std::move(left);
      node->right = parse_atom();
      left = std::move(node);
    }
    return left;
  }

  std::unique_ptr<Node> parse_atom() {
    if (!error_.empty()) {
      return nullptr;
    }
    if (Token::OPEN == peek().kind) {
      next();
      std::unique_ptr<Node> node = parse_or();
      if (error_.empty() && Token::CLOSE != next().kind) {
        error_ = "a '(' is not closed";
      }
      return node;
    }
    const Token property = next();
    const Token op = next();
    const Token value = next();
    if (Token::WORD != property.kind || Token::OP != op.kind ||
        Token::WORD != value.kind) {
      error_ = "a filter compares a property to a value: property == value";
      return nullptr;
    }
    const std::set<std::string>& known = filter_properties(kind_);
    if (!known.count(property.text)) {
      std::string names;
      for (const std::string& name : known) {
        names += (names.empty() ? "" : ", ") + name;
      }
      error_ = "unknown property '" + property.text + "' of " + kind_ +
               (names.empty() ? std::string(" (none can be filtered)")
                              : " (" + names + ")");
      return nullptr;
    }
    auto node = std::make_unique<Node>();
    node->property = property.text;
    node->op = op.text;
    node->value = value.text;
    return node;
  }

  const std::vector<Token>& tokens_;
  std::string kind_;
  size_t pos_ = 0;
  std::string error_;
};

std::unique_ptr<Node> parse_filter(const std::string& filter,
                                   const std::string& kind,
                                   std::string& error) {
  const std::vector<Token> tokens = tokenize(filter, error);
  if (!error.empty()) {
    return nullptr;
  }
  FilterParser parser(tokens, kind);
  std::unique_ptr<Node> node = parser.parse(error);
  if (!error.empty()) {
    error = "-filter {" + filter + "}: " + error;
  }
  return node;
}

size_t distance(const std::string& a, const std::string& b) {
  std::vector<size_t> row(b.size() + 1);
  for (size_t j = 0; j <= b.size(); ++j) {
    row[j] = j;
  }
  for (size_t i = 1; i <= a.size(); ++i) {
    size_t diagonal = row[0];
    row[0] = i;
    for (size_t j = 1; j <= b.size(); ++j) {
      const size_t above = row[j];
      row[j] = std::min({row[j] + 1, row[j - 1] + 1,
                         diagonal + (a[i - 1] == b[j - 1] ? 0 : 1)});
      diagonal = above;
    }
  }
  return row[b.size()];
}

std::string escape_regex(const std::string& text) {
  static const std::string special = "\\^$.|?*+()[]{}";
  std::string out;
  for (const char c : text) {
    if (std::string::npos != special.find(c)) {
      out += '\\';
    }
    out += c;
  }
  return out;
}

}  // namespace

const std::set<std::string>& filter_properties(const std::string& kind) {
  static const std::map<std::string, std::set<std::string>> properties = {
    {"ports", {"direction"}},
    {"pins", {"direction"}},
    {"cells", {"type"}},
    {"sites",
     {"side", "pin_direction", "pin_type", "netlist_name",
      "customer_pin_alias"}}};
  static const std::set<std::string> none;
  const auto found = properties.find(kind);
  return properties.end() == found ? none : found->second;
}

bool check_filter(const std::string& filter, const std::string& kind,
                  std::string& error) {
  return nullptr != parse_filter(filter, kind, error);
}

bool glob_match(const std::string& pattern, const std::string& name,
                const bool hierarchical, const bool nocase) {
  return glob_at(pattern, 0, name, 0, hierarchical, nocase);
}

bool match_query(const Query& query, const std::vector<Object>& objects,
                 std::vector<const Object*>& matched, std::string& error) {
  std::unique_ptr<Node> filter;
  if (!query.filter.empty()) {
    filter = parse_filter(query.filter, query.kind, error);
    if (!filter) {
      return false;
    }
  }
  std::vector<std::regex> regexes;
  if (query.regexp) {
    try {
      for (const std::string& pattern : query.patterns) {
        regexes.emplace_back(
          pattern, query.nocase ? std::regex::ECMAScript | std::regex::icase
                                : std::regex::ECMAScript);
      }
    } catch (const std::regex_error&) {
      error = "a pattern of '" + query.text + "' is not a regular expression";
      return false;
    }
  }
  for (const Object& object : objects) {
    bool hit = false;
    if (query.regexp) {
      for (const std::regex& regex : regexes) {
        if (std::regex_match(object.name, regex)) {
          hit = true;
          break;
        }
      }
    } else {
      for (const std::string& pattern : query.patterns) {
        if (glob_match(pattern, object.name, query.hierarchical,
                       query.nocase)) {
          hit = true;
          break;
        }
      }
    }
    if (hit && (!filter || filter->eval(object))) {
      matched.push_back(&object);
    }
  }
  return true;
}

std::vector<std::string> nearest_names(const std::string& pattern,
                                       const std::vector<Object>& objects,
                                       const size_t count) {
  std::string plain;
  for (const char c : pattern) {
    if ('*' != c && '?' != c) {
      plain += c;
    }
  }
  std::vector<std::pair<size_t, std::string>> ranked;
  for (const Object& object : objects) {
    ranked.emplace_back(distance(plain, object.name), object.name);
  }
  std::sort(ranked.begin(), ranked.end());
  std::vector<std::string> names;
  for (size_t i = 0; i < ranked.size() && names.size() < count; ++i) {
    names.push_back(ranked[i].second);
  }
  return names;
}

std::string exact_regex(const std::string& name) {
  return "^" + escape_regex(name) + "$";
}

std::string glob_regex(const std::string& pattern, const bool hierarchical) {
  std::string out = "^";
  for (const char c : pattern) {
    if ('*' == c) {
      out += hierarchical ? ".*" : "[^./]*";
    } else if ('?' == c) {
      out += hierarchical ? "." : "[^./]";
    } else {
      out += escape_regex(std::string(1, c));
    }
  }
  return out + "$";
}

} /* end namespace odc */
