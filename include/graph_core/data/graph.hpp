#ifndef GRAPH_CORE__DATA__GRAPH_HPP_
#define GRAPH_CORE__DATA__GRAPH_HPP_

#include <vector>
#include <optional>
#include <string>

namespace graph_core::data
{

class Graph
{
public:
  Graph() = delete;

protected:
  Graph(std::vector<std::vector<int>> && contents) noexcept
  : m_data(std::move(contents))
  {};

public:
  static std::optional<Graph> FromAdjencyMatrix(std::optional<std::string> filePath);

private:
  std::vector<std::vector<int>> m_data;
};

}

#endif // GRAPH_CORE__DATA__GRAPH_HPP_