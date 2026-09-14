#include "graph_core/data/graph.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <fmt/format.h>

namespace graph_core::data
{

std::optional<Graph> Graph::FromAdjencyMatrix(std::optional<std::string> filePath)
{
  if (!filePath.has_value())
  {
    std::cerr << "Invalid file path";
    return std::optional<Graph>{};
  }

  const auto path = filePath.value();

  if (!std::filesystem::exists(path)) {
    std::cerr << fmt::format("File doesn't exist: {}", filePath.value());
    return std::optional<Graph>{};
  }

  std::ifstream file(path, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Failed to open input file";
    return std::optional<Graph>{};
  }

  size_t verticesCount = 0;
  if (!(file >> verticesCount)) {
    std::cerr << "Failed to read vertices count";
    return std::optional<Graph>{};
  }

  if (0 == verticesCount) {
    std::cerr << "Graph must contatin at least one vertex";
    return std::optional<Graph>{};
  }

  std::vector<std::vector<int>> v(verticesCount, std::vector<int>(verticesCount, 0));

  for (size_t row = 0; row < verticesCount; ++row) {
    for (size_t column = 0; column < verticesCount; ++column) {
      int value = 0;
      if (!(file >> value)) {
        std::cerr << fmt::format(
          "Not enough matrix entries"
        );
        return std::optional<Graph>{};
      }
    }
  }

  return Graph(std::move(v));
}

} // namespace graph_core::data