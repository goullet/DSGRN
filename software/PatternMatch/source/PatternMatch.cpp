/// PatternMatch.cpp
/// Shaun Harker and Bree Cummins
/// 2016-03-19

#include "PatternMatch.h"
#include <boost/functional/hash.hpp>

bool
QueryCycleMatch ( MatchingGraph const& mg ) {
  typedef MatchingGraph::Vertex Vertex;
  uint64_t N = mg . searchgraph() . size ();
  for ( uint64_t sg_vertex = 0; sg_vertex < N; ++ sg_vertex ) {
    Vertex start = {sg_vertex, mg . patterngraph() . root () };
    Vertex end = {sg_vertex, mg . patterngraph() . leaf () };
    if ( not ( mg . query ( start ) && mg . query ( end ) ) ) continue;
    std::unordered_set<Vertex, boost::hash<Vertex>> explored; 
    std::stack<Vertex> dfs_stack;
    dfs_stack . push ( start );
    while ( not dfs_stack . empty () ) {
      Vertex const& vertex = dfs_stack . top ();
      dfs_stack . pop ();
      if ( vertex == end ) return true;
      explored . insert ( vertex );
      for ( Vertex const& next_vertex : mg . adjacencies ( vertex ) ) {
        if ( explored . count ( next_vertex ) == 0 ) { 
          dfs_stack . push ( next_vertex );
        }
      }
    }
  }
  return false;
}

// Debugging routine
std::string
ExplainCycleMatch ( MatchingGraph const& mg ) {
  typedef MatchingGraph::Vertex Vertex;
  uint64_t N = mg . searchgraph() . size ();
  for ( uint64_t sg_vertex = 0; sg_vertex < N; ++ sg_vertex ) {
    Vertex start = {sg_vertex, mg . patterngraph() . root () };
    Vertex end = {sg_vertex, mg . patterngraph() . leaf () };
    if ( not ( mg . query ( start ) && mg . query ( end ) ) ) continue;
    std::unordered_set<Vertex, boost::hash<Vertex>> preordered;
    std::unordered_set<Vertex, boost::hash<Vertex>> postordered; 
    std::stack<Vertex> dfs_stack;
    std::vector<Vertex> history;
    dfs_stack . push ( start );
    while ( not dfs_stack . empty () ) {
      Vertex const& vertex = dfs_stack . top ();
      if ( preordered . count ( vertex ) == 0 ) {
        // preorder step
        preordered . insert ( vertex );
        history . push_back ( vertex );
        //std::cout << "PREORDER (" << vertex.first << ", " << vertex.second << ")\n";
      } else if ( postordered . count ( vertex ) == 0 ) {
        // postorder step
        postordered . insert ( vertex );
        dfs_stack . pop ();
        history . pop_back ();
        //std::cout << "POSTORDER (" << vertex.first << ", " << vertex.second << ")\n";
      } else {
        dfs_stack . pop ();
      }
      if ( vertex == end ) {
        // Found a path
        //std::cout << "FOUND A PATH.\n";
        std::stringstream ss;
        bool first;
        PatternGraph const& pg = mg . patterngraph();
        auto labelstring = [&](uint64_t L) {
          std::string result;
          for ( uint64_t d = 0; d < pg.dimension(); ++ d ){
            if ( L & (1 << d) ) result.push_back('D'); else result.push_back('I');
          }
          return result;
        };
        first = true;
        for ( Vertex const& v : history ) {
          if ( first ) first = false; else ss << " -> ";
          ss << "(" << v.first <<", " << v.second << ";\t [" << mg . searchgraph () . vertexInformation ( v.first ) 
             << "], " << labelstring(pg.label(v.second)) << ")\n";
        }
        ss << "\n";
        ss << "[";
        first = true;
        for ( Vertex const& v : history ) {
          if ( first ) first = false; else ss << ", ";
          ss << mg . searchgraph () . vertexInformation ( v.first );
        }
        ss << "]\n";
        for ( uint64_t i = 0; i < history . size () - 1; ++ i ) {
          uint64_t u = history[i].first;
          uint64_t v = history[i+1].first;
          std::cout << "---\n";
          std::cout << mg . searchgraph () . edgeInformation ( u, v );
          uint64_t pg_u = history[i].second;
          uint64_t pg_v = history[i+1].second;
          if ( pg_u == pg_v ) {
            std::cout << "Intermediate match on pattern graph vertex " << pg_u << " with label " << labelstring(pg . label ( pg_u )) << "\n";
          } else {
            std::cout << "Extremal match, consuming pattern graph vertex " << pg_u << " with label " << labelstring(pg . label ( pg_u )) << " and moving to " << pg_v << " with label " << labelstring(pg . label ( pg_v )) <<"\n";
          }
        }
        return ss . str ();
      }
      for ( Vertex const& next_vertex : mg . adjacencies ( vertex ) ) {
        if ( preordered . count ( next_vertex ) == 0 ) { 
          dfs_stack . push ( next_vertex );
        }
      }
    }
  }
  return "false";
}


bool
QueryPathMatch ( MatchingGraph const& mg ) {
  typedef MatchingGraph::Vertex Vertex;
  uint64_t N = mg . searchgraph() . size ();
  for ( uint64_t sg_vertex = 0; sg_vertex < N; ++ sg_vertex ) {
    Vertex start = {sg_vertex, mg . patterngraph() . root () };
    uint64_t end = mg . patterngraph() . leaf ();
    if ( not mg . query ( start ) ) continue;
    std::unordered_set<Vertex, boost::hash<Vertex>> explored; 
    std::stack<Vertex> dfs_stack;
    dfs_stack . push ( start );
    while ( not dfs_stack . empty () ) {
      Vertex vertex = dfs_stack . top ();
      dfs_stack . pop ();
      if ( mg.position(vertex) == end ) return true;
      explored . insert ( vertex );
      for ( Vertex const& next_vertex : mg . adjacencies ( vertex ) ) {
        if ( explored . count ( next_vertex ) == 0 ) { 
          dfs_stack . push ( next_vertex );
        }
      }
    }
  }
  return false;
}

/*

uint64_t
CountCycleMatches ( MatchingGraph const& mg ) {
  uint64_t N = mg . searchgraph() . size ();
  uint64_t result = 0;
  // Loop through starting sg_vertexs
  for ( uint64_t sg_vertex = 0; sg_vertex < N; ++ sg_vertex ) {
    // Count the number of paths from (start_sg_vertex, root) to (start_sg_vertex, leaf)
    uint64_t start = mg . vertex ( sg_vertex, mg . patterngraph . root () );
    uint64_t end = mg . vertex ( sg_vertex, mg . patterngraph . leaf () );
    // TODO MAKE SURE start/end exist
    // We use a data structure which will store the number of paths from a vertex to "end"
    std::unordered_map<uint64_t, uint64_t> memoization_table; 
    memoization_table[end] = 1;
    // The algorithm takes the form of a depth-first-search traversal
    std::stack<uint64_t> dfs_stack;
    dfs_stack . push ( start );
    while ( not dfs_stack . empty () ) {
      uint64_t vertex = dfs_stack . top ();
      dfs_stack . pop ();
      if ( memoization_table . count ( vertex ) == 0 ) {
        // FIRST VISIT to "vertex" (Preorder step)
        memoization_table [ vertex ] = -1;
        // Push the vertex onto dfs_stack again to arrange for second visit
        dfs_stack . push ( vertex );
        result_stack . push ( 0 ); // push to result_stack on preorder step
        for ( uint64_t next_vertex : mg . adjacencies ( vertex ) ) {
          dfs_stack . push ( next_vertex );
        }
      } else { 
        // SECOND VISIT OR LATER to "vertex"
        if ( memoization_table [ vertex ] == -1 ) {
          // SECOND VISIT (Postorder step)
          memoization_table [ vertex ] = result_stack . top ();
          result_stack . pop (); // pop from result_stack on postorder step
        }
        result_stack . top () += memoization_table [ vertex ];
      }
    }
    result += memoization_table[start];
  }
  return result;
}

uint64_t
CountPathMatches ( MatchingGraph const& mg ) {
  uint64_t N = mg . searchgraph() . size ();
  // Loop through starting sg_vertexs
  for ( uint64_t sg_vertex = 0; sg_vertex < N; ++ sg_vertex ) {
    // Count the number of paths from (start_sg_vertex, root) to (start_sg_vertex, leaf)
    uint64_t start = mg . vertex ( sg_vertex, mg . patterngraph . root () );
    uint64_t end = mg . patterngraph() . leaf ();
    // TODO MAKE SURE start/end exist
    // We use a data structure which will store the number of paths from a vertex to "end"
    std::unordered_map<uint64_t, uint64_t> memoization_table; 
    memoization_table[end] = 1;
    // The algorithm takes the form of a depth-first-search traversal
    std::stack<uint64_t> dfs_stack;
    dfs_stack . push ( start );
    while ( not dfs_stack . empty () ) {
      uint64_t vertex = dfs_stack . top ();
      dfs_stack . pop ();
      if ( memoization_table . count ( vertex ) == 0 ) {
        // FIRST VISIT to "vertex" (Preorder step)
        memoization_table [ vertex ] = -1;
        // Push the vertex onto dfs_stack again to arrange for second visit
        dfs_stack . push ( vertex );
        result_stack . push ( 0 ); // push to result_stack on preorder step
        for ( uint64_t next_vertex : mg . adjacencies ( vertex ) ) {
          dfs_stack . push ( next_vertex );
        }
      } else { 
        // SECOND VISIT OR LATER to "vertex"
        if ( memoization_table [ vertex ] == -1 ) {
          // SECOND VISIT (Postorder step)
          memoization_table [ vertex ] = result_stack . top ();
          result_stack . pop (); // pop from result_stack on postorder step
        }
        result_stack . top () += memoization_table [ vertex ];
      }
    }
  }
}
*/
