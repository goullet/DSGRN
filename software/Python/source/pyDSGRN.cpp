
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "DSGRN.h"

#include <vector>

// Network wrapping incidentals

// IntList
typedef std::vector<uint64_t> IntList;
std::string
printIntList ( IntList const* self ) {
  std::stringstream ss;
  bool first = true;
  ss << "[";
  for ( auto const& item : *self ) {
    if ( first ) first = false; else ss << ", ";
    ss << item;
  }
  ss << "]";
  return ss.str();
}

// IntListList
typedef std::vector<std::vector<uint64_t>> IntListList;
std::string
printIntListList ( IntListList const* self ) {
  std::stringstream ss;
  bool first = true;
  ss << "[";
  for ( auto const& item : *self ) {
    if ( first ) first = false; else ss << ", ";
    ss << printIntList(&item);
  }
  ss << "]";
  return ss.str();
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(network_overloads, graphviz, 0, 1)

// LogicParameter wrapping incidentals

//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(LogicParameter_overloads, operator(), 0, 2)

typedef std::vector<LogicParameter> LogicParameterList;
std::string
printLogicParameterList( LogicParameterList const* self ) {
  std::stringstream ss;
  bool first = true;
  ss << "[";
  for ( auto const& item : *self ) {
    if ( first ) first = false; else ss << ", ";
    ss << item.stringify();
  }
  ss << "]";
  return ss.str();
}

// OrderParameter wrapping incidentals
typedef std::vector<OrderParameter> OrderParameterList;
std::string
printOrderParameterList( OrderParameterList const* self ) {
  std::stringstream ss;
  bool first = true;
  ss << "[";
  for ( auto const& item : *self ) {
    if ( first ) first = false; else ss << ", ";
    ss << item.stringify();
  }
  ss << "]";
  return ss.str();
}

// Domain wrapping incidentals
std::string
printDomain ( Domain const* domain ) {
  std::stringstream ss;
  ss << *domain;
  return ss . str ();
}

// Poset wrapping incidentals
typedef std::set<uint64_t> IntSet;

// MorseDecomposition wrapping incidentals
std::string
printMorseDecomposition ( MorseDecomposition const* md ) {
  std::stringstream ss;
  ss << *md;
  return ss . str ();
}

BOOST_PYTHON_MODULE(libpyDSGRN)
{
    using namespace boost::python;

    // This will enable user-defined docstrings and python signatures,
      // while disabling the C++ signatures
      docstring_options local_docstring_options(true, true, false);

    // Network

    class_<IntList>("IntList")
      .def(vector_indexing_suite<IntList>() )
      .def("__str__", &printIntList)
      .def("__repr__", &printIntList)
    ;

    class_<IntListList>("IntListList")
      .def(vector_indexing_suite<IntListList>() )
      .def("__str__", &printIntListList)
      .def("__repr__", &printIntListList)
    ;

    class_<Network>("Network", "Network Class", init<std::string>())
      .def(init<>())
      .def( "load",
            &Network::load,
            "Load a network from a file given by filename."
            , args("self","filename")
          )
      .def( "assign",
            &Network::assign
          )
      .def( "size",
            &Network::size,
            "Return the number of nodes in the network",
            args("self")
          )
      .def( "index",
            &Network::index,
            "Return the node index of a given node variable name",
            args("self","node name")
          )
      .def( "name",
            &Network::name,
            "Return the name of the node of a given node index",
            return_value_policy<copy_const_reference>(),
            args("self", "node index")
          )
      .def( "inputs",
            &Network::inputs,
            "Return the list of inputs of a given node",
            return_value_policy<copy_const_reference>(),
            args("self", "node index")
          )
      .def( "outputs",
            &Network::inputs,
            "Return the list of outputs of a given node",
            return_value_policy<copy_const_reference>(),
            args("self", "node index")
          )
      .def( "logic",
            &Network::logic,
            "Return the logic of a given node",
            return_value_policy<copy_const_reference>(),
            args("self", "node index")
          )
      .def( "essential",
            &Network::essential,
            "Return whether or not to use only essential parameters for a given node",
            args("self", "node index")
          )
      .def( "interaction",
            &Network::interaction,
            "Return the interaction type of an edge (False for depression, True for activation)",
            args("self", "source index", "target index")
          )
      .def( "order",
            &Network::order,
            "Return the out-edge order number of an edge, \ni.e. so outputs(source)[order(source,target)] == target",
            args("self", "source index", "target index")
          )
      .def( "domains",
            &Network::domains,
            "Return a list consisting of the number of domains across (i.e. number of out-edges + 1) \nfor each dimension (i.e. network node)",
            args("self")
          )
      .def( "specification",
            &Network::specification,
            "Return the specification string of the network",
            args("self")
          )
      .def( "graphviz",
            &Network::graphviz,
            network_overloads( "Return a graphviz string (dot language) of the network", args("self") ) )
      .def( "__str__", &Network::specification)
    ;

    // LogicParameter

    class_<LogicParameterList>("LogicParameterList")
      .def(vector_indexing_suite<LogicParameterList>() )
      .def("__str__", &printLogicParameterList)
      .def("__repr__", &printLogicParameterList)
    ; // Note: to get this to work I needed to make LogicParameter == comparable in C++ code

    class_<LogicParameter>("LogicParameter", init<uint64_t, uint64_t, std::string>())
      .def(init<>())
      .def("assign", &LogicParameter::assign)
      .def("__call__",static_cast<bool (LogicParameter::*)( std::vector<bool> const&, uint64_t ) const>
         (&LogicParameter::operator()) ) // How is python user going to get hands on vector<bool> ? TODO
      .def("__call__",static_cast<bool (LogicParameter::*)( uint64_t ) const>
         (&LogicParameter::operator()) )
      .def("bin", &LogicParameter::bin)
      .def("stringify", &LogicParameter::stringify)
      .def("parse", &LogicParameter::parse)
      .def("hex", &LogicParameter::hex, return_value_policy<copy_const_reference>())
      .def("adjacencies", &LogicParameter::adjacencies)
      .def("__str__", &LogicParameter::stringify)
    ;

    // OrderParameter

    class_<OrderParameterList>("OrderParameterList")
      .def(vector_indexing_suite<OrderParameterList>() )
      .def("__str__", &printOrderParameterList)
      .def("__repr__", &printOrderParameterList)
    ; // Note: to get this to work I needed to make OrderParameter == comparable in C++ code

    class_<OrderParameter>("OrderParameter", init<>())
      .def(init<uint64_t,uint64_t>())
      .def(init<std::vector<uint64_t>const&>()) // TODO
      .def("assign", static_cast<void (OrderParameter::*)( uint64_t, uint64_t )>(&OrderParameter::assign) )
      .def("assign", static_cast<void (OrderParameter::*)( std::vector<uint64_t> const&)>(&OrderParameter::assign) )
      .def("__call__", &OrderParameter::operator() )
      .def("inverse", &OrderParameter::inverse )
      .def("permutation", &OrderParameter::permutation, return_value_policy<copy_const_reference>())
      .def("index", &OrderParameter::index)
      .def("size", &OrderParameter::size)
      .def("stringify", &OrderParameter::stringify)
      .def("parse", &OrderParameter::parse)
      .def("adjacencies", &OrderParameter::adjacencies)
      .def("__str__", &OrderParameter::stringify)
    ;

    // Parameter

    class_<Parameter>("Parameter", init<>())
      .def(init<std::vector<LogicParameter> const&, std::vector<OrderParameter> const&, Network const& >())
      .def(init<Network const&>())
      .def("assign", static_cast<void (Parameter::*)(std::vector<LogicParameter> const&, std::vector<OrderParameter> const&, Network const& network)>(&Parameter::assign) )
      .def("assign", static_cast<void (Parameter::*)(Network const& )>(&Parameter::assign) )
      .def("attracting", &Parameter::attracting)
      .def("asborbing", &Parameter::absorbing)
      .def("regulator", &Parameter::regulator)
      .def("labelling", &Parameter::labelling)
      .def("network", &Parameter::network)
      .def("stringify", &Parameter::stringify)
      .def("parse", &Parameter::parse)
      .def("inequalities", &Parameter::parse)
      .def("logic", &Parameter::logic, return_value_policy<copy_const_reference>())
      .def("order", &Parameter::order, return_value_policy<copy_const_reference>())
      .def("__str__", &Parameter::stringify)
    ;

    // ParameterGraph

    class_<ParameterGraph>("ParameterGraph", init<>())
      .def(init<Network const&>())
      .def("assign", &ParameterGraph::assign)
      .def("size", &ParameterGraph::size)
      .def("parameter", &ParameterGraph::parameter)
      .def("index", &ParameterGraph::index)
      .def("adjacencies", &ParameterGraph::adjacencies)
      .def("network", &ParameterGraph::network)
      .def("fixedordersize", &ParameterGraph::fixedordersize)
      .def("reorderings", &ParameterGraph::reorderings)
    ;

    // Digraph

    class_<Digraph>("Digraph", init<>())
      .def(init<std::vector<std::vector<uint64_t>>&>())
      .def( "assign",
            &Digraph::assign,
            "Construct a digraph from an adjacency list",
            args("self", "adjancency list")
          )
      .def( "adjacencies",
            static_cast<std::vector<uint64_t> const& (Digraph::*)(uint64_t)const>(&Digraph::adjacencies), return_value_policy<copy_const_reference>()
          )
      .def( "size",
            &Digraph::size,
            "Return the number of vertives",
            args("self")
          )
      .def( "resize",
            &Digraph::resize,
            "Resize the digraph to have n vertices.\nCreate empty adjacency list if needed.\nn should be greater the current size",
            args("self", "n: new size")
          )
      .def( "add_vertex",
            &Digraph::add_vertex,
            "Add a vertex and return the index of the newly added vertex",
            args("self")
          )
      .def( "add_edge",
            &Digraph::add_edge,
            "Add an edge between source and target.\nWarning: Does not check if the edge already exists",
            args("self", "source index", "target index")
          )
      .def( "finalize",
            &Digraph::finalize,
            "Sort the adjacency list in ascending order",
            args("self")
          )
      .def( "transpose",
            &Digraph::transpose,
            "Return the transpose graph.",
            args("self")
          )
      .def( "transitive_reduction",
            &Digraph::transitive_reduction,
            "Return the transitive reduction\nNote: requires the graph be a transitively closed DAG \nor else a transitively closed DAG with extra self-edges",
            args("self")
          )
      .def( "transitive_closure",
            &Digraph::transitive_closure,
            "Compute the transitive closure.\nNote: self-edges are not in the result even if they are in the input",
            args("self")
          )
      .def( "permute",
            &Digraph::permute,
            "Reorder the digraph according to the provided permutation.\nThe convention on the permutation is that vertex v in the input \ncorresponds to vertex permutation[v] in the output",
            args("self","permutation vector")
          )
      .def( "stringify",
            &Digraph::stringify,
            "Return a JSON description of the digraph.",
            args("self")
          )
      .def( "parse",
            &Digraph::parse,
            "Initialize a digraph from a JSON description.\nFormat: an array of arrays of integers.\nThe inner arrays are adjacency lists.\nVertices are numbered 0...N-1\nThe outer array has length N",
            args("self", "JSON string" )
          )
      .def( "graphviz",
            &Digraph::graphviz,
            "Return a Graphviz representation (dot language) of the digraph",
            args("self")
          )
      .def("__str__", &Digraph::graphviz)
    ;

    // Poset

    class_<IntSet>("IntSet")  // return type, argument type of "Poset::maximal"
      .def(init<>() )
    ;

    class_<Poset>("Poset", init<>())
      .def(init<std::vector<std::vector<uint64_t>>&>())
      .def(init<Digraph const&>())
      .def("assign", static_cast<void(Poset::*)(std::vector<std::vector<uint64_t>> &)>(&Poset::assign))
      .def("assign", static_cast<void(Poset::*)(Digraph const&)>(&Poset::assign))
      .def("size", &Poset::size)
      .def("parents", &Poset::parents, return_value_policy<copy_const_reference>())
      .def("children", &Poset::children, return_value_policy<copy_const_reference>())
      .def("ancestors", &Poset::ancestors, return_value_policy<copy_const_reference>())
      .def("descendants", &Poset::descendants, return_value_policy<copy_const_reference>())
      .def("maximal", &Poset::maximal)
      .def("compare", &Poset::compare)
      .def("permute", &Poset::permute)
      .def("stringify", &Poset::stringify)
      .def("parse", &Poset::parse)
      .def("graphviz", &Poset::graphviz)
      .def("__str__", &Poset::stringify)
    ;

    // Domain
    // TODO: document interface differences between python/C++ this wrapper entails
    class_<Domain>("Domain", init<>())
      .def(init<std::vector<uint64_t> const&>())
      .def("assign", &Domain::assign)
      .def("__getitem__", &Domain::operator[])
      .def("__len__", &Domain::size)
      .def("size", &Domain::size)
      .def("next", static_cast<Domain& (Domain::*)(void)>(&Domain::operator++), return_value_policy<reference_existing_object>() )
      .def("index", &Domain::index)
      .def("setIndex", &Domain::setIndex)
      .def("left", &Domain::left)
      .def("right", &Domain::right)
      .def("isMin", &Domain::isMin)
      .def("isMax", &Domain::isMax)
      .def("isValid", &Domain::isValid)
      .def("__str__", &printDomain)
    ;

    // DomainGraph
    class_<DomainGraph>("DomainGraph", init<>())
      .def(init<Parameter const>())
      .def("digraph", &DomainGraph::digraph)
      .def("dimension", &DomainGraph::dimension)
      .def("coordinates", &DomainGraph::coordinates)
      .def("label", &DomainGraph::label)
      .def("direction", &DomainGraph::direction)
      .def("regulator", &DomainGraph::regulator)
      .def("annotate", &DomainGraph::annotate)
      .def("graphviz", &DomainGraph::graphviz)
    ;

    // MorseDecomposition
    class_<MorseDecomposition>("MorseDecomposition", init<>())
      .def(init<Digraph const&>())
      .def(init<Digraph const&, Components const&>())
      .def("assign", static_cast<void(MorseDecomposition::*)(Digraph const&)>(&MorseDecomposition::assign))
      .def("assign", static_cast<void(MorseDecomposition::*)(Digraph const&, Components const&)>(&MorseDecomposition::assign))
      .def("poset", &MorseDecomposition::poset)
      .def("components", &MorseDecomposition::components)
      .def("recurrent", &MorseDecomposition::recurrent)
      .def("graphviz", &MorseDecomposition::graphviz)
      .def("__str__", &printMorseDecomposition)
    ;

    // MorseGraph
    class_<MorseGraph>("MorseGraph", init<>())
      .def("assign", &MorseGraph::assign<DomainGraph>)
      .def("poset", &MorseGraph::poset)
      //.def("annotation", &MorseGraph::annotation)
      .def("SHA256", &MorseGraph::SHA256)
      .def("stringify", &MorseGraph::stringify)
      .def("parse", &MorseGraph::parse)
      .def("graphviz", &MorseGraph::graphviz)
      .def("__str__", &MorseGraph::stringify)
    ;

}
