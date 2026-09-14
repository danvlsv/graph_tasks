#ifndef GRAPH_CORE__UTILS__DOT_EXPORTER_HPP_
#define GRAPH_CORE__UTILS__DOT_EXPORTER_HPP_

#include "graph_core/utils/iexporter.hpp"

namespace graph_core::utils
{

class DotExporter : public IExporter
{

  bool Export(std::optional<std::string> filePath) override;

};

}

#endif // GRAPH_CORE__UTILS__DOT_EXPORTER_HPP_