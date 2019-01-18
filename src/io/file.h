#pragma once

#include <cstdio>

#include "base/reader.h"
#include "base/writer.h"

using strata::base::Reader;
using strata::base::Writer;

namespace strata {
namespace io {

class FileReader : public Reader {
  public:
    FileReader(FILE* file);

  private:
    bool ReadRaw(size_t size, char* bytes) override;

    bool IsAtEnd() const override;

    FILE* file_{nullptr};
};

class FileWriter : public Writer {
  public:
    FileWriter(FILE* file);

  private:
    bool WriteRaw(const char* bytes, size_t size) override;

    FILE* file_{nullptr};
};

}  // namespace io
}  // namespace strata
