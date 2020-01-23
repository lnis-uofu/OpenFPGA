#ifndef DIRECT_TYPES_H
#define DIRECT_TYPES_H

/********************************************************************
 * Define the types of point to point connection between CLBs 
 * These types are supplementary to the original VPR direct connections
 * Here we extend to the cross-row and cross-column connections
 ********************************************************************/
enum e_point2point_interconnection_type {
  NO_P2P,
  P2P_DIRECT_COLUMN,
  P2P_DIRECT_ROW,
  NUM_POINT2POINT_INTERCONNECT_TYPE
};

enum e_point2point_interconnection_dir {
  POSITIVE_DIR,
  NEGATIVE_DIR,
  NUM_POINT2POINT_INTERCONNECT_DIR
};

#endif
