#ifndef APP_OPTIONS_H
#define APP_OPTIONS_H

#include <map>
#include <string>
#include <vector>

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * The value of an application option, with what is needed to describe
 * it: its type, a help message and, for selections, the values accepted
 *******************************************************************/
struct AppOptionValue {
  enum e_type {
    STRING,
    INT,
    FLOAT,
    BOOLEAN,
    SELECTION,
  };

  e_type type = STRING;
  std::string help_message;
  /* The value of a STRING and of a SELECTION */
  std::string string_value;
  /* The values accepted by a SELECTION, with the enumeration they stand for */
  std::map<std::string, int> selection_values;
  int int_value = 0;
  double float_value = 0.;
  bool bool_value = false;

  static AppOptionValue make_string(const std::string& value,
                                    const std::string& help_message = "");
  static AppOptionValue make_int(const int value,
                                 const std::string& help_message = "");
  static AppOptionValue make_float(const double value,
                                   const std::string& help_message = "");
  static AppOptionValue make_boolean(const bool value,
                                     const std::string& help_message = "");
  static AppOptionValue make_selection(
    const std::string& value,
    const std::map<std::string, int>& selection_values,
    const std::string& help_message = "");

  /* The enumeration of the selected value. 0 if this is not a selection */
  int to_enum() const;
  /* The value as text, as it is accepted by set_from_string() */
  std::string to_string() const;
  /* Name of the type, to be used in messages and reports */
  std::string type_name() const;
  /* The values accepted by a selection, in a list. Empty for other types */
  std::vector<std::string> choices() const;

  /* Set the value from text, which is parsed according to the type:
   * - STRING: any text
   * - INT: a whole number, e.g., 10 or -2
   * - FLOAT: a number, e.g., 0.5 or 1e-3
   * - BOOLEAN: true, false, on, off, yes, no, 1 or 0 (in any case)
   * - SELECTION: one of the values accepted
   * Return false, with the reason in error, if the text is not valid. The
   * value is left untouched in this case */
  bool set_from_string(const std::string& text, std::string& error);
};

/* The default value of all the application options, by name. The names are
 * made of the group and the option, e.g., place.seed */
const std::map<std::string, AppOptionValue>& app_option_defaults();

/********************************************************************
 * The application options of a flow: the parameters of the engines, e.g.,
 * the seed of the placer. They are meant to be set between the stages.
 * Every option has a name (group.option), a type and a default value.
 * The names are the ones listed by app_option_defaults()
 *******************************************************************/
class AppOptions {
 public: /* Constructors */
  /* All the options are at their default value */
  AppOptions();

 public: /* Public accessors */
  bool has(const std::string& name) const;
  /* Return the option, or nullptr if there is none with this name */
  const AppOptionValue* find(const std::string& name) const;
  /* The names of all the options, sorted */
  std::vector<std::string> names() const;
  /* The names of the options of a group, e.g., place, sorted */
  std::vector<std::string> names(const std::string& group) const;
  /* Return the default value of an option. The option must exist */
  const AppOptionValue& default_of(const std::string& name) const;
  /* Return true if an option still has its default value */
  bool is_default(const std::string& name) const;

 public: /* Public mutators */
  /* Set an option from text, see AppOptionValue::set_from_string(). Return
   * false, with the reason in error, if the option does not exist or the
   * text is not valid for its type. Nothing changes in this case */
  bool set(const std::string& name, const std::string& text,
           std::string& error);
  /* Give an option its default value back. Return false if it does not exist
   */
  bool reset(const std::string& name);
  void reset_all();

 private: /* Internal data */
  std::map<std::string, AppOptionValue> values_;
};

} /* End namespace openfpga */

#endif
