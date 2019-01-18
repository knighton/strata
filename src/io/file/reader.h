#pragma once

#include <cstdio>

#include "io/base/reader.h"
#include "io/base/writer.h"

using strata::io::base::StrataReader;

namespace strata {
namespace io {
namespace file {

class StrataFileReader : public StrataReader {
  public:
    StrataFileReader(FILE* file);

  private:
    bool ReadRaw(size_t size, char* bytes) override;

    bool IsAtEnd() const override;

    FILE* file_{nullptr};
};

}  // namespace file
}  // namespace io
}  // namespace strata
