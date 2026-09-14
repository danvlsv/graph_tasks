#include "graph_core/utils/dot_exporter.hpp"

#include <iostream>

namespace graph_core::utils
{

bool DotExporter::Export(std::optional<std::string> filePath)
{
  if (!filePath.has_value())
  {
    std::cerr << "Invalid file path";
    return false;
  }

  return true;
}

}