/// TestWallGraph.cpp
/// Shaun Harker
/// 2015-06-30

#include "common.h"
#include "DSGRN.h"

int main ( int argc, char * argv [] ) {
  try { 
    // Load a network file
    Network network ( "networks/network2.txt" );

    // Construct a parameter graph
    std::cout << "Construct parameter graph.\n";
    ParameterGraph pg ( network );

    // Fetch a random parameter from the parameter graph
    std::cout << "Choosing parameter.\n";
    uint64_t N = pg . size ();
    uint64_t param_index = 43;
    Parameter param = pg . parameter ( param_index );
    std::cout << "Chose " << param << "\n";

    // Construct the domain graph
    std::cout << "Construct wall graph.\n";
    WallGraph wg ( param );

    // Default constructor
    WallGraph wg0;

    // Test <<
    std::cout << wg;
    boost::archive::text_oarchive oa(std::cout);
    oa << wg;

    // Note:
    //   WallGraph::digraph, WallGraph::annotate tested in TestMorseGraph.cpp
  } catch ( std::exception & e ) {
    std::cout << e . what () << "\n";
    return 1;
  }
  return 0;
}
