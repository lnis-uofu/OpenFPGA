#include "openfpga_readline.h"

#include <cstdlib>
#include <iostream>

// 1. Conditional Includes based on the backend flag
#if defined(OPENFPGA_USE_READLINE)
#include <readline/history.h>
#include <readline/readline.h>
#elif defined(OPENFPGA_USE_LIBEDIT)
#include <editline/readline.h>
#endif

// Global/File-static variable to hold vocabulary for completion
static std::vector<std::string> vocabulary;

#if defined(OPENFPGA_USE_READLINE) || defined(OPENFPGA_USE_LIBEDIT)
/**
 * Generator function for readline's completion engine.
 * It is called repeatedly by readline to find matches.
 */
static char* command_generator(const char* text, int state) {
  static size_t list_index, len;

  // If this is a new word to complete, reset our scan index
  if (!state) {
    list_index = 0;
    len = std::string(text).length();
  }

  // Search through the vocabulary for a prefix match
  while (list_index < vocabulary.size()) {
    const std::string& command = vocabulary[list_index];
    list_index++;

    if (command.compare(0, len, text) == 0) {
      // Readline expects a C-string allocated via malloc
      char* match = static_cast<char*>(malloc(command.length() + 1));
      if (match) {
        std::string(command).copy(match, command.length());
        match[command.length()] = '\0';
        return match;
      }
    }
  }

  return nullptr;  // No more matches
}

/**
 * Custom completion function called by readline/libedit.
 */
static char** my_attempted_completion(const char* text, int /*start*/,
                                      int /*end*/) {
  // Avoid default filename completion if our generator fails
  rl_attempted_completion_over = 1;

  // Return the list of matches using our generator
  return rl_completion_matches(text, command_generator);
}
#endif

void initialize_readline(const std::vector<std::string>& completion_words) {
  vocabulary = completion_words;

#if defined(OPENFPGA_USE_READLINE) || defined(OPENFPGA_USE_LIBEDIT)
  // Register our custom completion function with the readline library
  rl_attempted_completion_function = my_attempted_completion;
  // Explicitly bind the TAB key to the completion command.
  // GNU Readline does this by default, but libedit often requires this
  // to initialize its internal keymaps correctly.
  rl_bind_key('\t', rl_complete);
  // FORCE IMMEDIATE DISPLAY OF CANDIDATES
  // By default, readline beeps on the first Tab if there are multiple choices.
  // Setting this to 0 forces it to list choices immediately on the first Tab.
  rl_completion_query_items = 0;
#endif
}

std::string get_user_input(const std::string& prompt) {
#if defined(OPENFPGA_USE_READLINE) || defined(OPENFPGA_USE_LIBEDIT)
  char* buffer = readline(prompt.c_str());

  if (!buffer) {
    return "";  // User pressed Ctrl+D (EOF)
  }

  std::string result(buffer);

  // Only add to history if it's not empty and not just whitespace
  if (!result.empty() &&
      result.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
    add_history(buffer);
  }

  free(buffer);  // Clean up the C-string allocated by readline
  return result;
#else
  // Fallback implementation using standard C++ streams
  std::cout << prompt << std::flush;
  std::string line;
  if (!std::getline(std::cin, line)) {
    return "";  // EOF reached
  }
  return line;
#endif
}
