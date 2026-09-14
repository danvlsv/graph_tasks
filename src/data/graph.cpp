#include "graph_core/data/graph.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>
#include <ranges>
#include <print>

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
      v[row][column] = value;
    }
  }

  return Graph(std::move(v), verticesCount);
}

void RecursiveDFS(
  std::vector<std::vector<int>> & adj,
  std::vector<bool> & visited,
  size_t target,
  std::vector<int> & res)
{
  visited[target] = true;
  res.emplace_back(target);
  for (size_t i : adj[target]) {
    if (false == visited[i]) {
      RecursiveDFS(adj, visited, i, res);
    }
  }
}

std::vector<int> Graph::RunDFS(size_t vertexIndex)
{
  std::vector<bool> visited(m_vertexCount, false);
  visited[vertexIndex] = true;
  std::vector<int> res;
  RecursiveDFS(m_adjacencyMatrix, visited, vertexIndex, res);
  return res;
}

bool Graph::IsValidNextEdge(size_t start, size_t target)
{
  if (1 == GetVertexDegree(start)) {
    return true;
  }

  int initialCount = RunDFS(start).size();

  m_adjacencyMatrix[start][target] = m_adjacencyMatrix[target][start] = 0;

  int edgeRemovedCount = RunDFS(start).size();
  m_adjacencyMatrix[start][target] = m_adjacencyMatrix[target][start] = 1;

  return edgeRemovedCount < initialCount ? false : true;
}

bool Graph::RunFleuryAlgorithm(const Graph & graph)
{
  auto tempGraph = graph;

  size_t startVertex = 0;
  const auto oddVertices = std::views::iota(std::size_t{0}, tempGraph.m_vertexCount)
    | std::views::filter([&tempGraph](std::size_t index) {
        return tempGraph.GetVertexDegree(index) % 2 != 0;
      })
    | std::ranges::to<std::vector<std::size_t>>();

  if (2 == oddVertices.size()) {
    startVertex = oddVertices[0];
  } else if (!(0 == oddVertices.size())) {
    std::cerr << "No Eulerian Path or Circuit exists.\n";
    return false;
  }

  std::vector<size_t> path = {startVertex};
  size_t target = startVertex;

  while (tempGraph.GetVertexDegree(target) > 0) {
    for (size_t v = 0; v < tempGraph.m_vertexCount; v++) {
      if (1 == tempGraph.m_adjacencyMatrix[target][v]) {
        if (tempGraph.IsValidNextEdge(target, v)) {
          tempGraph.m_adjacencyMatrix[target][v] =
            tempGraph.m_adjacencyMatrix[v][target] = 0;
          path.emplace_back(v);
          target = v;
          break;
        }
      }
    }
  }


  auto formattedPath = path
    | std::views::transform([](size_t n) { return std::to_string(n); })
    | std::views::join_with(std::string_view(" -> "))
    | std::ranges::to<std::string>();

  std::cout << fmt::format("Successfully completed Fleury algorithm: {}\n", formattedPath);

  return true;

}

} // namespace graph_core::data
