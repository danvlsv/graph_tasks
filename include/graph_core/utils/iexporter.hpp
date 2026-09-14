#ifndef GRAPH_CORE__UTILS__IEXPORTER_HPP_
#define GRAPH_CORE__UTILS__IEXPORTER_HPP_

#include <string>
#include <optional>

namespace graph_core::utils
{

class IExporter
{
public:
  virtual bool Export(std::optional<std::string> exportPath) = 0;
};

}

#endif // GRAPH_CORE__UTILS__IEXPORTER_HPP_