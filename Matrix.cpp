#include "Matrix.h"
int Matrix<Vertex*>::max_bid = 0;
Matrix<Vertex*> m;
vector<pair<Vertex*, Direction>> symmetric_corner_inners;
vector<pair<Matrix<Vertex*>::iterator, Direction>> prev_bridge_directions;