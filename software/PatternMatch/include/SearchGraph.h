/// SearchGraph.h
/// Shaun Harker and Bree Cummins
/// 2016-03-19

#ifndef SEARCHGRAPH_H
#define SEARCHGRAPH_H

#include "DSGRN.h"
#include "common.h"

struct SearchGraph_;

class SearchGraph {
public:
  /// SearchGraph
  ///   Default constructor
  SearchGraph ( void );

  /// SearchGraph
  ///   Create search graph from a domain graph and a morse set index
  SearchGraph ( DomainGraph dg, uint64_t morse_set_index );

  /// SearchGraph
  ///   Create search graph from a sequence of labels
  ///   (This is used for checking for a pattern in a time series)
  SearchGraph ( std::vector<uint64_t> const& labels, uint64_t dim );

  /// assign
  ///   Create search graph from a domain graph and a morse set index
  void
  assign ( DomainGraph dg, uint64_t morse_set_index );

  /// assign
  ///   Create search graph from a sequence of labels
  ///   (This is used for checking for a pattern in a time series)
  void
  assign ( std::vector<uint64_t> const& labels, uint64_t dim );

  /// size
  ///   Return the number of vertices in the search graph
  uint64_t
  size ( void ) const;

  /// dimension
  ///   Return the number of variables
  uint64_t
  dimension ( void ) const;

  /// label
  ///   Given a vertex, return the associated label
  ///   The label is a 64 bit word with bits interpreted as follows:
  ///    bit i+D     bit i   interpretation
  ///         0        1    ith variable decreasing  
  ///         1        0    ith variable increasing
  ///         0        0    ith variable can either increase or decrease
  ///   Note the limitation to 32 dimensions. Here D is the number of
  ///   dimensions.
  uint64_t
  label ( uint64_t v ) const;

  /// adjacencies
  ///   Given a vertex v, return vector of vertices which 
  ///   are out-edge adjacencies of input v
  std::vector<uint64_t> const&
  adjacencies ( uint64_t v ) const;

  /// event
  ///   Given a source and target domain give the variable
  ///   associated with the extremal event which may occur
  ///   on that wall. If no event is possible, return -1
  uint64_t 
  event ( uint64_t source, uint64_t target ) const;

  /// graphviz
  ///   Return a graphviz representation of the search graph
  std::string
  graphviz ( void ) const;

  /// vertexInformation
  ///   Return information about a search graph vertex
  ///   This method is intended for debug purposes
  std::string
  vertexInformation ( uint64_t v ) const;

  /// edgeInformation
  ///   Return information about a search graph edge
  ///   This method is intended for debug purposes
  std::string
  edgeInformation ( uint64_t source, uint64_t target ) const;

private:
  std::shared_ptr<SearchGraph_> data_;
  /// _label_event
  ///   Given source and target labels, the direction, the variable
  ///   being regulated, and the dimension, determine which variable 
  ///   could experience an event between source and target. 
  ///   If there is no such event return -1
  uint64_t
  _label_event ( uint64_t source_label, uint64_t target_label, 
                 uint64_t direction, uint64_t regulator, uint64_t dimension ) const;
  /// serialize
  ///   For use with BOOST Serialization library,
  ///   which is used by the cluster-delegator MPI package
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & data_;
  }
};

struct SearchGraph_ {
  Digraph digraph_;
  std::vector<uint64_t> labels_;
  std::vector<std::unordered_map<uint64_t, uint64_t>> event_;
  uint64_t dimension_;
  std::function<std::string(uint64_t)> vertex_information_;
  std::function<std::string(uint64_t,uint64_t)> edge_information_;
  /// serialize
  ///   For use with BOOST Serialization library,
  ///   which is used by the cluster-delegator MPI package
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & digraph_;
    ar & labels_;
    ar & event_;
    ar & dimension_;
  }
};

#endif
