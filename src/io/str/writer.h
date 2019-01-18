#pragma once

#include "io/base/reader.h"
#include "io/base/writer.h"

using strata::io::base::StrataWriter;

namespace strata {
namespace io {
namespace str {

class StrataStringWriter : public StrataWriter {
  public:
    StrataStringWriter(string* data);

  private:
    bool WriteRaw(const char* bytes, size_t size) override;

    string* data_{nullptr};
};

}  // namespace str
}  // namespace io
}  // namespace strata
