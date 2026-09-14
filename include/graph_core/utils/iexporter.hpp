#ifndef GRAPH_CORE__UTILS__IEXPORTER_HPP_
#define GRAPH_CORE__UTILS__IEXPORTER_HPP_

#include <string>
#include <optional>
#include <string_view>

#include "graph_core/data/graph.hpp"

namespace graph_core::utils
{

class IExporter
{
public:
  IExporter() = delete;

protected:
  inline static std::string GetVertexName(size_t index)
  {
    std::string name = "";

    while (0 <= index ) {
      name += static_cast<char>('A' + (index % 26));
      if (index < 26) {
        break;
      }
      index = (index / 26) - 1;
    }

    std::reverse(name.begin(), name.end());
    return name;
  }

};

}

#endif // GRAPH_CORE__UTILS__IEXPORTER_HPP_