#include "graph_core/utils/dot_exporter.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <fmt/format.h>

namespace graph_core::utils
{

bool DotExporter::Export(const data::Graph & graph, std::string_view filePostfix)
{
  std::ofstream file(fmt::format("graph{}.dot", filePostfix), std::ios::out);

  if (!file) {
    std::cerr << "Failed to create file during export process";
    return false;
  }

  const auto & adjacencyMatrix = graph.GetAdjecencyMatrix();
  const auto vertexCount = graph.GetVertexCount();

  try {
    file << "graph G {\n";

    for (size_t i = 0; i < vertexCount; ++i) {
      for (size_t j = i; j < vertexCount; ++j) {
        if (1 == adjacencyMatrix[i][j]) {
          file << std::setw(2) << "" << GetVertexName(i) << " -- " << GetVertexName(j) << "\n";
        }
      }
    }

    file << "}\n";
    file.close();
  } catch (std::exception & ex) {
    std::cerr << fmt::format("Error during .dot export prceoss: {}", ex.what());
    return false;
  }

  return true;
}

}