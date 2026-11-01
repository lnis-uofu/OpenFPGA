#ifndef GLOB_MATCH_H
#define GLOB_MATCH_H

#include <string>

/********************************************************************
 * Shell-style matching of a name against a pattern, as fnmatch() of POSIX
 * with no flag, but available on every platform:
 *   *       any sequence of characters, also an empty one
 *   ?       any one character
 *   [abc]   one of the characters, [a-c] one of a range, [!abc] or [^abc]
 *           any other character. A ']' that comes first is one of the
 *           characters. [:alpha:], [:digit:], [:alnum:], [:upper:],
 *           [:lower:], [:space:], [:punct:], [:xdigit:], [:blank:],
 *           [:cntrl:], [:graph:] and [:print:] are classes of characters
 *   \c      the character c, as it is
 * A '[' that is not closed is the character '['.
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/* Return true if the whole text matches the pattern */
bool glob_match(const std::string& pattern, const std::string& text);

} /* End namespace openfpga */

#endif
