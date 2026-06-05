#pragma once

#include <string>
#include <vector>

// Define the active backend for the rest of the application if needed
#if defined(OPENFPGA_USE_READLINE)
    #define BACKEND_NAME "GNU Readline"
#elif defined(OPENFPGA_USE_LIBEDIT)
    #define BACKEND_NAME "libedit"
#else
    #define BACKEND_NAME "std::getline"
#endif

/**
 * Initializes the readline configuration, including custom autocompletion words.
 * @param completion_words A list of strings to match for tab-completion.
 */
void initialize_readline(const std::vector<std::string>& completion_words);

/**
 * Prompts the user for input with history and autocompletion support.
 * @param prompt The string to display before the input cursor.
 * @return The user's input string, or an empty string on EOF (Ctrl+D).
 */
std::string get_user_input(const std::string& prompt);
