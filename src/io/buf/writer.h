#pragma once

#include "io/base/writer.h"

using strata::io::base::StrataWriter;

namespace strata {
namespace io {
namespace buf {

class StrataBufferWriter : public StrataWriter {
  public:
    StrataBufferWriter(char** data, const char* data_end);

  private:
    bool WriteRaw(const char* bytes, size_t size) override;

    char** data_{nullptr};
    const char* data_end_{nullptr};
};

}  // namespace buf
}  // namespace io
}  // namespace strata
