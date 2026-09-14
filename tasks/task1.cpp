#include "graph_core/data/graph.hpp"

#include <iostream>
#include <optional>

using Graph = graph_core::data::Graph;

int main (int argc, char* argv[])
{
  if (!(2 == argc))
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  const auto filePath = std::string(argv[1]);
  const auto graph = Graph::FromAdjencyMatrix(filePath);

  if (graph.has_value()) {
    Graph::RunFleuryAlgorithm(graph.value());
  }

  return 0;
}