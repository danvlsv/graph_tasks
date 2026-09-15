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
  const std::vector<std::vector<int>> & adj,
  std::vector<bool> & visited,
  size_t target,
  std::vector<int> & res)
{
  visited[target] = true;
  res.emplace_back(static_cast<int>(target));

  for (size_t i = 0; i < adj[target].size(); ++i) {
    if (adj[target][i] == 1 && !visited[i]) {
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

  const int initialCount = RunDFS(start).size();

  m_adjacencyMatrix[start][target] = m_adjacencyMatrix[target][start] = 0;

  const int edgeRemovedCount = RunDFS(start).size();
  m_adjacencyMatrix[start][target] = m_adjacencyMatrix[target][start] = 1;

  return edgeRemovedCount < initialCount ? false : true;
}

std::optional<std::vector<size_t>> Graph::RunFleuryAlgorithm(const Graph & graph)
{
  auto tempGraph = graph;

  try {
    size_t edgeCount = 0;
    for (size_t i = 0; i < tempGraph.m_vertexCount; ++i) {
      for (size_t j = i + 1; j < tempGraph.m_vertexCount; ++j) {
        if (tempGraph.m_adjacencyMatrix[i][j] == 1) {
          ++edgeCount;
        }
      }
    }

    if (edgeCount == 0) {
      std::cerr << "Graph has no edges.\n";
      return std::optional<std::vector<size_t>>{};
    }

    std::vector<size_t> oddVertices;
    for (size_t i = 0; i < tempGraph.m_vertexCount; ++i) {
      if (tempGraph.GetVertexDegree(i) % 2 != 0) {
        oddVertices.push_back(i);
      }
    }

    if (!(oddVertices.empty() || oddVertices.size() == 2)) {
      std::cerr << "No Eulerian Path or Circuit exists.\n";
      return std::optional<std::vector<size_t>>{};
    }

    size_t startVertex = 0;
    if (oddVertices.size() == 2) {
      startVertex = oddVertices[0];
    } else {
      for (size_t i = 0; i < tempGraph.m_vertexCount; ++i) {
        if (tempGraph.GetVertexDegree(i) > 0) {
          startVertex = i;
          break;
        }
      }
    }

    auto reachable = tempGraph.RunDFS(startVertex);
    std::vector<bool> visited(tempGraph.m_vertexCount, false);
    for (auto v : reachable) {
      visited[v] = true;
    }

    for (size_t i = 0; i < tempGraph.m_vertexCount; ++i) {
      if (tempGraph.GetVertexDegree(i) > 0 && !visited[i]) {
        std::cerr << "Graph is not connected; no Euler path/circuit exists.\n";
        return std::optional<std::vector<size_t>>{};
      }
    }

    std::vector<size_t> path = {startVertex};
    size_t current = startVertex;

    while (tempGraph.GetVertexDegree(current) > 0) {
      bool found = false;
      for (size_t v = 0; v < tempGraph.m_vertexCount; ++v) {
        if (tempGraph.m_adjacencyMatrix[current][v] == 1) {
          if (tempGraph.IsValidNextEdge(current, v)) {
            tempGraph.m_adjacencyMatrix[current][v] = 0;
            tempGraph.m_adjacencyMatrix[v][current] = 0;
            path.push_back(v);
            current = v;
            found = true;
            break;
          }
        }
      }
      if (!found) {
        std::cerr << "Fleury algorithm stuck: no valid next edge.\n";
        return std::optional<std::vector<size_t>>{};
      }
    }

    if (path.size() != edgeCount + 1) {
      std::cerr << "Euler path/circuit not complete (disconnected graph?).\n";
      return std::optional<std::vector<size_t>>{};
    }

    auto formattedPath = path
      | std::views::transform([](size_t n) { return std::to_string(n); })
      | std::views::join_with(std::string_view(" -> "))
      | std::ranges::to<std::string>();

    std::cout << fmt::format("Successfully completed Fleury algorithm: {}\n", formattedPath);

    return path;

    } catch (std::exception & ex) {
    std::cerr << fmt::format("Exception during Fleury Algorithm run: {}", ex.what());
    return std::optional<std::vector<size_t>>{};
  }

}

} // namespace graph_core::data
