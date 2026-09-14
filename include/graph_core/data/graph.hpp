#ifndef GRAPH_CORE__DATA__GRAPH_HPP_
#define GRAPH_CORE__DATA__GRAPH_HPP_

#include <vector>
#include <optional>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include <exception>

#include <fmt/format.h>

namespace graph_core::data
{

class Graph
{
public:
  Graph() = delete;

private:
  Graph(std::vector<std::vector<int>> && contents, size_t vertexCount) noexcept
  : m_adjacencyMatrix(std::move(contents)),
    m_vertexCount(vertexCount)
  {};

public:
  inline int GetVertexDegree(size_t vertexIndex) const
  {
    if (vertexIndex >= m_vertexCount) {
      throw std::out_of_range::exception();
    }

    int degree = 0;
    const auto & row = m_adjacencyMatrix[vertexIndex];
    std::for_each(row.begin(), row.end(), [&degree](int elem) {
      degree += elem;
    });

    // std::cout << fmt::format("Vertex {} has degree = {}\n", vertexIndex, degree);
    return degree;
  }

  std::vector<int> RunDFS(size_t vertexIndex = 0);

private:
  bool IsValidNextEdge(size_t start, size_t target);

public:
  static std::optional<Graph> FromAdjencyMatrix(std::optional<std::string> filePath);

  static bool RunFleuryAlgorithm(const Graph & graph);

private:
  std::vector<std::vector<int>> m_adjacencyMatrix;
  size_t m_vertexCount;
};

}

#endif // GRAPH_CORE__DATA__GRAPH_HPP_