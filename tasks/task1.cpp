#include "graph_core/data/graph.hpp"
#include "graph_core/utils/dot_exporter.hpp"

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
  const auto graphOpt = Graph::FromAdjencyMatrix(filePath);

  if (!graphOpt.has_value()) {
    std::cerr << "Invalid graph";
    return 1;
  }

  const auto & graph = graphOpt.value();
  Graph::RunFleuryAlgorithm(graph);

  graph_core::utils::DotExporter::Export(graph);

  return 0;
}