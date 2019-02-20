#include <cassert>
#include <string>
#include <vector>
#include <utility>

#include "io/str/func.h"
#include "io/str/writer.h"
#include "util/crc32.h"

using std::make_pair;
using std::pair;
using namespace strata::io::str;
using strata::util::InitCRC32;
using std::string;
using std::vector;

namespace {

const char* BOOL_STRS[] = {
    "false",
    "true"
};

const char* Bool2Str(bool x) {
    return BOOL_STRS[static_cast<size_t>(x)];
}

void Test(const StrataWriteFlags& flags, const vector<string>& items) {
    string data;
    for (auto& item : items) {
        assert(WriteOneToStrataString(flags, item, &data));
    }

    {
        string s;
        assert(WriteToStrataString(flags, items, &s));
        assert(s == data);
    }

    {
        string s;
        StrataStringWriter writer(&s);
        for (auto& item : items) {
            assert(writer.WriteOne(flags, item));
        }
        assert(s == data);
    }

    {
        string s;
        StrataStringWriter writer(&s);
        assert(writer.Write(flags, items));
        assert(s == data);
    }

    printf("snappy = %s, crc32 = %s -> %zu bytes.\n", Bool2Str(flags.snappy),
           Bool2Str(flags.crc32), data.size());
}

}  // namespace

int main() {
    InitCRC32();

    vector<string> items = {
        "james",
        "42",
        "",
        "keep on keepin' on",
        "james",
        string(1024, '\0')
    };

    for (auto snappy : {false, true}) {
        for (auto crc32 : {false, true}) {
            StrataWriteFlags flags;
            flags.snappy = snappy;
            flags.crc32 = crc32;
            Test(flags, items);
        }
    }
}
