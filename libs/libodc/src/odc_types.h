#ifndef ODC_TYPES_H
#define ODC_TYPES_H
/********************************************************************
 * The data of ODC, the constraints file of OpenFPGA (docs/spec_odc.md): the
 * words and the commands of a file, the objects that the commands name, and
 * what the checks need to know of a design (its netlist) and of a device
 * (its grid, its I/O pins).
 *
 * Nothing here depends on VPR: OpenFPGA fills the device from the data of
 * VPR, and the library reads the netlist itself.
 *******************************************************************/
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

/* begin namespace odc */
namespace odc {

/* The version of the format that this reader knows (R-3) */
constexpr int ODC_VERSION = 1;

/* An error or a warning, with the file and the line of the command */
struct Diagnostic {
  enum Level { ERROR, WARNING };
  Level level = ERROR;
  std::string file;
  size_t line = 0;
  std::string message;

  /* "file:line: error: message" */
  std::string text() const;
};

/* A file that cannot be read: the message has the file and the line */
class OdcError : public std::runtime_error {
 public:
  explicit OdcError(const std::string& message) : std::runtime_error(message) {}
};

/* An object query, [get_ports ...] (R-10) */
struct Query {
  std::string command; /* get_ports, all_inputs, ... */
  std::string kind;    /* ports, cells, nets, pins, clocks, pblocks, sites */
  bool regexp = false;
  bool hierarchical = false;
  bool nocase = false;
  std::string filter; /* the expression of -filter, or empty */
  std::vector<std::string> patterns;
  std::string text; /* as written, without the brackets */
};

/* A word of a command (R-2) */
struct Word {
  enum Kind { BARE, QUOTED, BRACED, QUERY };
  Kind kind = BARE;
  /* The value of a bare or quoted word; the text inside the braces or the
   * brackets */
  std::string text;
  /* The words of a braced word */
  std::vector<Word> items;
  Query query; /* of a QUERY */

  /* The word as it is written */
  std::string source() const;
};

/* A location of the VPR grid (R-6): a tile, a subtile of it, or a rectangle */
struct Location {
  int x_low = 0;
  int y_low = 0;
  int x_high = 0;
  int y_high = 0;
  int subtile = -1; /* -1 when none is given */
  int layer = 0;
  std::string text;

  bool is_tile() const { return x_low == x_high && y_low == y_high; }
};

/* What a command is about, which says what it changes and who takes it */
enum class Kind { HEADER, INCLUDE, TIMING, SYNTHESIS, FLOORPLAN, ROUTING, PIN };
/* The first input or stage of the flow whose result a command changes */
enum class Stage { CIRCUIT, DEVICE };
/* The tool that a command is for */
enum class Consumer { NONE, YOSYS, VPR_SDC, VPR_CONSTRAINTS, PIN_CONSTRAINTS };

std::string to_string(const Kind& kind);
std::string to_string(const Stage& stage);
std::string to_string(const Consumer& consumer);

/* One command of a file, checked against the table of commands (R-60) */
struct Command {
  std::string file;
  size_t line = 0;
  std::string name;
  std::vector<Word> words; /* the name included */
  Kind kind = Kind::HEADER;
  Stage stage = Stage::DEVICE;
  Consumer consumer = Consumer::NONE;
  /* Read from the package's odc_defaults (R-46): a command of the user's
   * file on the same object wins */
  bool from_defaults = false;

  /* set_property: the property and its value; set_synth_option: the option
   * (flatten, keep_hierarchy, top, define) and its value */
  std::string property;
  std::string value;
  /* LOC, resize_pblock */
  Location location;
  /* create_pblock, resize_pblock, add_cells_to_pblock */
  std::string pblock;
  /* set_global_signal */
  std::string route_model;
  std::string network;
  /* The index in words of each word that names objects: a query, or names */
  std::vector<size_t> objects;

  /* The command, in one line, as write_odc writes it */
  std::string text() const;
};

/* The constraints of a file, with the files it includes */
struct Constraints {
  std::vector<Command> commands;
  /* The errors and warnings of the reading. With an error, the constraints
   * cannot be used */
  std::vector<Diagnostic> diagnostics;
  /* The files read, the main one first */
  std::vector<std::string> files;

  bool has(const Kind& kind) const;
  bool has_errors() const;
};

/* An object that a query can match: its name and its properties */
struct Object {
  std::string name;
  std::map<std::string, std::string> properties;
};

/* The names of a netlist (R-10): the top ports, the cells, the nets and the
 * pins of the cells. Read from a BLIF file (read_blif_netlist) */
struct Netlist {
  std::string file;
  std::string top;
  std::vector<Object> ports; /* direction: in, out */
  std::vector<Object> cells; /* type: names, latch, or the model */
  std::vector<Object> nets;
  std::vector<Object> pins; /* cell/port, for the cells of a model */
  /* The nets that clock a latch or a flip-flop of a model with a port
   * named clk/clock (for the coverage of the timing) */
  std::set<std::string> clock_nets;

  const Object* port(const std::string& name) const;
};

/* A tile of the grid, as the checks need it */
struct Tile {
  std::string type; /* the type of the tile, empty where there is none */
  int capacity = 0; /* the subtiles of its type */
  bool io = false;  /* the tile holds the ports of the design */
  int root_x = 0;   /* the location of the tile that a large tile starts at */
  int root_y = 0;
};

/* A device: what the checks need to know of its grid and its architecture */
struct Device {
  int width = 0;
  int height = 0;
  int layers = 1;
  std::vector<Tile> tiles; /* x * height + y, layer 0 */
  /* The models (names, latch, input, output, or the model of a .subckt) that
   * a tile of each type can hold, with how many */
  std::map<std::string, std::map<std::string, int>> type_models;
  /* The names of the clock networks of the architecture */
  std::set<std::string> clock_networks;
  /* The option general.clock_modeling of the flow */
  std::string clock_modeling;

  const Tile* tile(const int x, const int y) const;
};

/* An I/O pin of the device: a row of the I/O map of the device package, or a
 * subtile of an I/O tile of the grid when there is no map */
struct Site {
  size_t line = 0; /* of the I/O map, 0 for a site of the grid */
  std::string side;
  std::string pin_direction; /* input, output, inout */
  std::string pin_type;      /* GPIO, CONTROL, SCAN, UNDEF */
  bool has_location = false;
  int x = 0;
  int y = 0;
  int subtile = 0;
  std::string netlist_name;
  std::string customer_pin_alias;

  /* X1Y0/3, or the netlist name of a pin without a location */
  std::string name() const;
};

} /* end namespace odc */

#endif
