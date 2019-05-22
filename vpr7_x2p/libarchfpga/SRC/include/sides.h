#ifndef SIDES_H
#define SIDES_H

/* Define a class for the sides of a physical block in FPGA architecture
 * Basically, each block has four sides :
 * TOP, RIGHT, BOTTOM, LEFT  
 * This class aims to provide a easy proctol for manipulating a side 
 */
#include <cstddef>

/* Orientations. */
enum e_side {
  TOP = 0, 
  RIGHT = 1, 
  BOTTOM = 2, 
  LEFT = 3,
  NUM_SIDES
};

class Side {
  public: /* Constructor */
    Side(enum e_side side);
    Side();
    Side(size_t side);
  public: /* Accessors */
    enum e_side get_side() const;
    enum e_side get_opposite() const;
    enum e_side get_rotate_clockwise() const;
    enum e_side get_rotate_counterclockwise() const;
    bool validate() const;
    size_t to_size_t() const;
  public: /* Mutators */
    void set_side(size_t side);
    void set_side(enum e_side side);
    void set_opposite();
    void rotate_clockwise();
    void rotate_counterclockwise();
  private: /* internal data */
    enum e_side side_;  
};

#endif
