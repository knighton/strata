#pragma once

#include <cstdio>

#include "io/base/writer.h"

using strata::io::base::StrataWriter;

namespace strata {
namespace io {
namespace file {

class StrataFileWriter : public StrataWriter {
  public:
    StrataFileWriter(FILE* file);

  private:
    bool WriteRaw(const char* bytes, size_t size) override;

    FILE* file_{nullptr};
};

}  // namespace file
}  // namespace io
}  // namespace strata
