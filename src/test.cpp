#include <cassert>
#include <string>
#include <vector>
#include <unistd.h>
#include <utility>

#include "io/buf/func.h"
#include "io/buf/reader.h"
#include "io/buf/writer.h"
#include "io/file/func.h"
#include "io/file/reader.h"
#include "io/file/writer.h"
#include "io/str/func.h"
#include "io/str/reader.h"
#include "io/str/writer.h"
#include "util/crc32.h"

using std::make_pair;
using std::pair;
using namespace strata::io::base;
using namespace strata::io::buf;
using namespace strata::io::file;
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

void DumpVector(const vector<string>& items) {
    printf("[");
    if (!items.empty()) {
        printf("%zu:'%s'", items[0].size(), items[0].c_str());
    }
    for (size_t i = 1; i < items.size(); ++i) {
        printf(", %zu:'%s'", items[i].size(), items[i].c_str());
    }
    printf("]\n");
}

struct Buffer {
    char* data{nullptr};
    const char* data_begin{nullptr};
    const char* data_end{nullptr};

    Buffer();
    ~Buffer();

    void ToString(string* s) const;

    bool EqualsString(const string& s) const;
};

Buffer::Buffer() {
    data = new char[2048];
    assert(data);
    data_begin = data;
    data_end = &data[2048];
}

Buffer::~Buffer() {
    if (data) {
        delete [] data_begin;
    }
}

void Buffer::ToString(string* s) const {
    size_t data_size = static_cast<size_t>(data - data_begin);
    *s = string(data_begin, data_size);
}

bool Buffer::EqualsString(const string& s) const {
    string t;
    ToString(&t);
    return s == t;
}

void TestBufferIO(const StrataWriteFlags& flags, const vector<string>& items) {
    string data_str;
    {
        Buffer buffer;
        for (auto& item : items) {
            assert(WriteOneToStrataBuffer(flags, item, &buffer.data,
                                          buffer.data_end));
        }
        buffer.ToString(&data_str);
    }

    {
        Buffer buffer;
        assert(WriteToStrataBuffer(flags, items, &buffer.data,
                                   buffer.data_end));
        assert(buffer.EqualsString(data_str));
    }

    {
        Buffer buffer;
        StrataBufferWriter writer(&buffer.data, buffer.data_end);
        for (auto& item : items) {
            assert(writer.WriteOne(flags, item));
        }
        assert(buffer.EqualsString(data_str));
    }

    {
        Buffer buffer;
        StrataBufferWriter writer(&buffer.data, buffer.data_end);
        assert(writer.Write(flags, items) == items.size());
        assert(buffer.EqualsString(data_str));
    }

    printf("[buffer io] snappy = %s, crc32 = %s -> %zu bytes.\n",
           Bool2Str(flags.snappy), Bool2Str(flags.crc32), data_str.size());

    {
        const char* data = &data_str[0];
        const char* data_end = &data_str[data_str.size()];
        string item;
        vector<string> read_items;
        while (ReadOneFromStrataBuffer(&data, data_end, &item)) {
            read_items.emplace_back(item);
        }
        assert(items == read_items);
    }

    {
        const char* data = &data_str[0];
        const char* data_end = &data_str[data_str.size()];
        string item;
        vector<string> read_items;
        assert(ReadFromStrataBuffer(&data, data_end, &read_items) ==
               items.size());
        assert(items == read_items);
    }

    {
        const char* data = &data_str[0];
        const char* data_end = &data_str[data_str.size()];
        StrataBufferReader reader(&data, data_end);
        string item;
        vector<string> read_items;
        while (reader.ReadOne(&item)) {
            read_items.emplace_back(item);
        }
        assert(items == read_items);
    }

    {
        const char* data = &data_str[0];
        const char* data_end = &data_str[data_str.size()];
        StrataBufferReader reader(&data, data_end);
        vector<string> read_items;
        assert(reader.Read(&read_items) == items.size());
        assert(items == read_items);
    }
}

struct File {
    const char* filename{nullptr};
    FILE* file{nullptr};

    File(const char* filename);
    ~File();

    void ToString(string* s) const;

    bool EqualsString(const string& s) const;
};

File::File(const char* the_filename) {
    filename = the_filename;
    file = fopen(filename, "w");
    assert(file);
}

File::~File() {
    fclose(file);
    unlink(filename);
}

bool FileToString(const char* filename, string* data) {
    FILE* file = fopen(filename,  "r");
    if (!file) {
        return false;
    }
    fseek(file, 0, SEEK_END);
    size_t size = static_cast<size_t>(ftell(file));
    fseek(file, 0, SEEK_SET);
    data->resize(size);
    fread(&(*data)[0], sizeof(char), size, file);
    fclose(file);
    return true;
}

bool StringToFile(const string& data, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        return false;
    }
    if (fwrite(&data[0], sizeof(char), data.size(), file) != data.size()) {
        fclose(file);
        return false;
    }
    fclose(file);
    return true;
}

void File::ToString(string* s) const {
    assert(FileToString(filename, s));
}

bool File::EqualsString(const string& s) const {
    string t;
    ToString(&t);
    return s == t;
}

void TestFileIO(const StrataWriteFlags& flags, const vector<string>& items) {
    const char* filename = "remove_me_used_for_testing.bin";
    string data_str;
    {
        File file(filename);
        for (auto& item : items) {
            assert(WriteOneToStrataFile(flags, item, file.file));
        }
        file.ToString(&data_str);
    }

    {
        File file(filename);
        assert(WriteToStrataFile(flags, items, file.file) == items.size());
        assert(file.EqualsString(data_str));
    }

    {
        File file(filename);
        StrataFileWriter writer(file.file);
        for (auto& item : items) {
            assert(writer.WriteOne(flags, item));
        }
        assert(file.EqualsString(data_str));
    }

    {
        File file(filename);
        StrataFileWriter writer(file.file);
        assert(writer.Write(flags, items) == items.size());
        assert(file.EqualsString(data_str));
    }

    printf("[file io] snappy = %s, crc32 = %s -> %zu bytes.\n",
           Bool2Str(flags.snappy), Bool2Str(flags.crc32), data_str.size());

    assert(StringToFile(data_str, filename));

    {
        FILE* file = fopen(filename, "r");
        string item;
        vector<string> read_items;
        while (ReadOneFromStrataFile(file, &item)) {
            read_items.emplace_back(item);
        }
        assert(items == read_items);
        fclose(file);
    }

    {
        FILE* file = fopen(filename, "r");
        vector<string> read_items;
        assert(ReadFromStrataFile(file, &read_items) == items.size());
        assert(items == read_items);
        fclose(file);
    }

    {
        FILE* file = fopen(filename, "r");
        StrataFileReader reader(file);
        string item;
        vector<string> read_items;
        while (reader.ReadOne(&item)) {
            read_items.emplace_back(item);
        }
        assert(items == read_items);
    }
    {
        FILE* file = fopen(filename, "r");
        StrataFileReader reader(file);
        vector<string> read_items;
        assert(reader.Read(&read_items) == items.size());
        assert(items == read_items);
    }

    unlink(filename);
}

void TestStringIO(const StrataWriteFlags& flags, const vector<string>& items) {
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

    printf("[string io] snappy = %s, crc32 = %s -> %zu bytes.\n",
           Bool2Str(flags.snappy), Bool2Str(flags.crc32), data.size());

    {
        size_t index = 0;
        string item;
        vector<string> read_items;
        while (ReadOneFromStrataString(data, &index, &item)) {
            read_items.emplace_back(item);
        }
        assert(items == read_items);
    }

    {
        size_t index = 0;
        vector<string> read_items;
        assert(ReadFromStrataString(data, &index, &read_items) == items.size());
        assert(items == read_items);
    }

    {
        size_t index = 0;
        string item;
        vector<string> read_items;
        StrataStringReader reader(&data, &index);
        while (reader.ReadOne(&item)) {
            read_items.emplace_back(item);
        }
        assert(items == read_items);
    }

    {
        size_t index = 0;
        vector<string> read_items;
        StrataStringReader reader(&data, &index);
        assert(reader.Read(&read_items) == items.size());
        assert(items == read_items);
    }
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
            TestBufferIO(flags, items);
            TestFileIO(flags, items);
            TestStringIO(flags, items);
        }
    }
}
