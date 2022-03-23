#ifndef TATUM_ASSERT_H
#define TATUM_ASSERT_H
/*
 * The header defines useful assertion macros for TATUM projects.
 *
 * Three types of assertions are defined:
 *      TATUM_ASSERT_OPT   - low overhead assertions that should always be enabled
 *      TATUM_ASSERT       - medium overhead assertions that are usually be enabled
 *      TATUM_ASSERT_SAFE  - high overhead assertions typically enabled only for debugging
 *      TATUM_ASSERT_DEBUG - very high overhead assertions typically enabled only for extreme debugging
 * Each of the above assertions also have a *_MSG variants (e.g. TATUM_ASSERT_MSG(expr, msg))
 * which takes an additional argument specifying additional message text to be shown.
 * By convention the message should state the condition *being checked* (and not the failure condition),
 * since that the condition failed is obvious from the assertion failure itself.
 *
 * The macro TATUM_ASSERT_LEVEL specifies the level of assertion checking desired:
 *
 *      TATUM_ASSERT_LEVEL == 4: TATUM_ASSERT_OPT, TATUM_ASSERT, TATUM_ASSERT_SAFE, TATUM_ASSERT_DEBUG enabled
 *      TATUM_ASSERT_LEVEL == 3: TATUM_ASSERT_OPT, TATUM_ASSERT, TATUM_ASSERT_SAFE enabled
 *      TATUM_ASSERT_LEVEL == 2: TATUM_ASSERT_OPT, TATUM_ASSERT enabled
 *      TATUM_ASSERT_LEVEL == 1: TATUM_ASSERT_OPT enabled
 *      TATUM_ASSERT_LEVEL == 0: No assertion checking enabled
 * Note that an assertion levels beyond 4 are currently treated the same as level 4
 */

//Set a default assertion level if none is specified
#ifndef TATUM_ASSERT_LEVEL
#    define TATUM_ASSERT_LEVEL 2
#endif

//Enable the assertions based on the specified level
#if TATUM_ASSERT_LEVEL >= 4
#    define TATUM_ASSERT_DEBUG_ENABLED
#endif

#if TATUM_ASSERT_LEVEL >= 3
#    define TATUM_ASSERT_SAFE_ENABLED
#endif

#if TATUM_ASSERT_LEVEL >= 2
#    define TATUM_ASSERT_ENABLED
#endif

#if TATUM_ASSERT_LEVEL >= 1
#    define TATUM_ASSERT_OPT_ENABLED
#endif

//Define the user assertion macros
#ifdef TATUM_ASSERT_DEBUG_ENABLED
#    define TATUM_ASSERT_DEBUG(expr) TATUM_ASSERT_IMPL(expr, nullptr)
#    define TATUM_ASSERT_DEBUG_MSG(expr, msg) TATUM_ASSERT_IMPL(expr, msg)
#else
#    define TATUM_ASSERT_DEBUG(expr) TATUM_ASSERT_IMPL_NOP(expr, nullptr)
#    define TATUM_ASSERT_DEBUG_MSG(expr, msg) TATUM_ASSERT_IMPL_NOP(expr, msg)
#endif

#ifdef TATUM_ASSERT_SAFE_ENABLED
#    define TATUM_ASSERT_SAFE(expr) TATUM_ASSERT_IMPL(expr, nullptr)
#    define TATUM_ASSERT_SAFE_MSG(expr, msg) TATUM_ASSERT_IMPL(expr, msg)
#else
#    define TATUM_ASSERT_SAFE(expr) TATUM_ASSERT_IMPL_NOP(expr, nullptr)
#    define TATUM_ASSERT_SAFE_MSG(expr, msg) TATUM_ASSERT_IMPL_NOP(expr, msg)
#endif

#ifdef TATUM_ASSERT_ENABLED
#    define TATUM_ASSERT(expr) TATUM_ASSERT_IMPL(expr, nullptr)
#    define TATUM_ASSERT_MSG(expr, msg) TATUM_ASSERT_IMPL(expr, msg)
#else
#    define TATUM_ASSERT(expr) TATUM_ASSERT_IMPL_NOP(expr, nullptr)
#    define TATUM_ASSERT_MSG(expr, msg) TATUM_ASSERT_IMPL_NOP(expr, msg)
#endif

#ifdef TATUM_ASSERT_OPT_ENABLED
#    define TATUM_ASSERT_OPT(expr) TATUM_ASSERT_IMPL(expr, nullptr)
#    define TATUM_ASSERT_OPT_MSG(expr, msg) TATUM_ASSERT_IMPL(expr, msg)
#else
#    define TATUM_ASSERT_OPT(expr) TATUM_ASSERT_IMPL_NOP(expr, nullptr)
#    define TATUM_ASSERT_OPT_MSG(expr, msg) TATUM_ASSERT_IMPL_NOP(expr, msg)
#endif

//Define the assertion implementation macro
// We wrap the check in a do {} while() to ensure the function-like
// macro can be always be followed by a ';'
#define TATUM_ASSERT_IMPL(expr, msg)                                                           \
    do {                                                                                     \
        if (!(expr)) {                                                                       \
            tatum::assert::handle_assert(#expr, __FILE__, __LINE__, TATUM_ASSERT_FUNCTION, msg); \
        }                                                                                    \
    } while (false)

//Define the no-op assertion implementation macro
// We wrap the check in a do {} while() to ensure the function-like
// macro can be always be followed by a ';'
//
// Note that to avoid 'unused' variable warnings when assertions are
// disabled, we pass the expr and msg to sizeof(). We use sizeof specifically
// since it accepts expressions, and the C++ standard gaurentees sizeof's arguments
// are never evaluated (ensuring any expensive expressions are not evaluated when
// assertions are disabled). To avoid warnings about the unused result of sizeof()
// we cast it to void.
#define TATUM_ASSERT_IMPL_NOP(expr, msg)   \
    do {                                 \
        static_cast<void>(sizeof(expr)); \
        static_cast<void>(sizeof(msg));  \
    } while (false)

//Figure out what macro to use to get the name of the current function
// We default to __func__ which is defined in C99
//
// g++ > 2.6 define __PRETTY_FUNC__ which includes class/namespace/overload
// information, so we prefer to use it if possible
#define TATUM_ASSERT_FUNCTION __func__
#ifdef __GNUC__
#    ifdef __GNUC_MINOR__
#        if __GNUC__ >= 2 && __GNUC_MINOR__ > 6
#            undef TATUM_ASSERT_FUNCTION
#            define TATUM_ASSERT_FUNCTION __PRETTY_FUNCTION__
#        endif
#    endif
#endif

namespace tatum {
namespace assert {
//Assertion handling routine
//
//Note that we mark the routine with the standard C++11
//attribute 'noreturn' which tells the compiler this
//function will never return. This should ensure the
//compiler won't warn about detected conditions such as
//dead-code or potential null pointer dereferences
//which are gaurded against by assertions.
[[noreturn]] void handle_assert(const char* expr, const char* file, unsigned int line, const char* function, const char* msg);
} // namespace assert
} // namespace tatum

#endif //TATUM_ASSERT_H
