#pragma once

#include "io/base/reader.h"
#include "io/base/writer.h"

using strata::io::base::StrataReader;

namespace strata {
namespace io {
namespace str {

class StrataStringReader : public StrataReader {
  public:
    StrataStringReader(const string* data, size_t* index);

  private:
    bool IsAtEnd() const override;

    bool ReadRaw(size_t size, char* bytes) override;

    const string* data_{nullptr};
    size_t* index_{nullptr};
};

}  // namespace str
}  // namespace io
}  // namespace strata
