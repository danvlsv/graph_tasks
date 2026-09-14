#ifndef GRAPH_CORE__UTILS__DOT_EXPORTER_HPP_
#define GRAPH_CORE__UTILS__DOT_EXPORTER_HPP_

#include "graph_core/utils/iexporter.hpp"

namespace graph_core::utils
{

class DotExporter : public IExporter
{
public:
  static bool Export(const data::Graph & graph, std::string_view filePostfix = "");

};

}

#endif // GRAPH_CORE__UTILS__DOT_EXPORTER_HPP_