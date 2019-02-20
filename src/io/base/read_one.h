#pragma once

namespace strata {
namespace io {
namespace base {

// The reason why reading one Strata record exited.
enum class StrataReadOneExit {
    // Read success.
    OK = 0,                 // Read the record successfully.

    // Reached end of data.
    END = 1,                // Reached the end.  No data left to parse.
    TRUNCATED_HEAD = 2,     // Reached end while reading payload size/flags.
    TRUNCATED_CRC32 = 3,    // Reached end while reading CRC32.
    TRUNCATED_PAYLOAD = 4,  // Reached end while reading payload.

    // Data corruption.
    ERR_CRC32 = 5,          // Read the entire record, but checksum failure.
    ERR_SNAPPY = 6          // Read record and checksum, but uncompress failure.
};

// Details about a Strata record that was read (in 8 bytes).
struct StrataReadOneInfo {
  public:
    // Read progress/result.
    bool was_head_valid() const;         // Whether we read the head field.
    StrataReadOneExit exit() const;      // Detailed reason why we exited.
    bool ok() const;                     // Whether we exited with status OK.

    // How the record was encoded.
    //
    // These methods assert was_head_valid().
    uint32_t head_size() const;  // Size of the head field in bytes (2 or 4).
    bool had_crc32() const;      // Whether the record had a CRC32 checksum.
    bool used_snappy() const;    // Whether the record used Snappy compression.

    // Record and payload size as predicted by the header.
    //
    // END:                                 Asserts (no record).
    // TRUNCATED_HEAD:                      Asserts (unknown record size).
    // TRUNCATED_CRC32, TRUNCATED_PAYLOAD:  Record was truncated.
    // ERR_CRC32, ERR_SNAPPY, OK:           Read entire record/payload.
    uint32_t record_size_in_header() const;
    uint32_t payload_size_in_header() const;

    // Record size read.  May be truncated.
    //
    // END:                                                 Asserts (no record).
    // TRUNCATED_HEAD, TRUNCATED_CRC32, TRUNCATED_PAYLOAD:  Record truncated.
    // ERR_CRC32, ERR_SNAPPY, OK:                           Fully read record.
    uint32_t record_size() const;

    // Encoded (compressed if used Snappy) payload size.  May be truncated.
    //
    // END, TRUNCATED_HEAD, TRUNCATED_CRC32:  Asserts (did not reach payload).
    // TRUNCATED_PAYLOAD:                     Payload data was truncated.
    // ERR_CRC32, ERR_SNAPPY, OK:             Fully read encoded payload data.
    uint32_t encoded_payload_size() const;

    // Decoded (uncompressed if used Snappy) payload size.
    //
    // Not OK:  Asserts (didn't decode a payload).
    // OK:      Successfully decoded payload.
    uint32_t decoded_payload_size() const;

    // Initialize the info depending on how ReadOne() returned.
    void InitEnd();
    void InitTruncatedHead(uint32_t truncated_record_size);
    void InitTruncatedCRC32(bool big_head, bool had_crc32, bool used_snappy,
                            uint32_t payload_size_in_header,
                            uint32_t truncated_record_size);
    void InitTruncatedPayload(bool big_head, bool had_crc32, bool used_snappy,
                              uint32_t payload_size_in_header,
                              uint32_t truncated_record_size);
    void InitErrCRC32(bool big_head, bool had_crc32, bool used_snappy,
                      uint32_t encoded_payload_size);
    void InitErrSnappy(bool big_head, bool had_crc32, bool used_snappy,
                       uint32_t encoded_payload_size);
    void InitOK(bool big_head, bool had_crc32, bool used_snappy,
                uint32_t encoded_payload_size, uint32_t decoded_payload_size);

  private:
    // Size of the header (non-payload) fields.
    //
    // Assumes was_head_valid().
    uint32_t header_size() const;

    // The reason why read returned (zero on success).
    StrataReadOneExit exit_ : 3;

    // Flags giving how the message was encoded, extracted from the head field.
    bool big_head_ : 1;     // Size of head: 4 bytes (true) or 2 bytes (false).
    bool had_crc32_ : 1;    // Whether the record had a CRC32 checksum.
    bool used_snappy_ : 1;  // Whether the payload was compressed using Snappy.

    // Record and payload size.
    //
    // Both as advertised by the header (interesting in the case of truncated
    // records) and actual.  Compressed and uncompressed payloads.
    //
    // Due to some trickery, we store these five fields in two variables xa_ and
    // xb_ whose meanings on how read exited.
    //
    // END:
    // * Header's record size:   N/A (asserts).
    // * Header's payload size:  N/A (asserts).
    // * Record size:            N/A (asserts).
    // * Encoded payload size:   N/A (asserts).
    // * Decoded payload size:   N/A (asserts).
    //
    // TRUNCATED_HEAD:
    // * Header's record size:   N/A (asserts).
    // * Header's payload size:  N/A (asserts).
    // * Record size:            Stored in xa_ (record is truncated).
    // * Encoded payload size:   N/A (asserts).
    // * Decoded payload size:   N/A (asserts).
    //
    // TRUNCATED_CRC32:
    // * Header's record size:   Header size + header's payload size.
    // * Header's payload size:  Stored in xa_.
    // * Record size:            Stored in xb_ (record is truncated).
    // * Encoded payload size:   N/A (asserts).
    // * Decoded payload size:   N/A (asserts).
    //
    // TRUNCATED_PAYLOAD:
    // * Header's record size:   Header size + header's payload size.
    // * Header's payload size:  Stored in xa_.
    // * Record size:            Header size + encoded payload size.
    // * Encoded payload size:   Stored in xb_.
    // * Decoded payload size:   N/A (asserts).
    //
    // ERR_CRC32:
    // * Header's record size:   Same as record size.
    // * Header's payload size:  Same as encoded payload size.
    // * Record size:            Header size + encoded payload size.
    // * Encoded payload size:   Stored in xa_.
    // * Decoded payload size:   N/A (asserts).
    //
    // ERR_SNAPPY:
    // * Header's record size:   Same as record size.
    // * Header's payload size:  Same as encoded payload size.
    // * Record size:            Header size + encoded payload size.
    // * Encoded payload size:   Stored in xa_.
    // * Decoded payload size:   N/A (asserts).
    //
    // OK:
    // * Header's record size:   Same as record size.
    // * Header's payload size:  Same as encoded payload size.
    // * Record size:            Header size + encoded payload size.
    // * Encoded payload size:   Stored in xa_.
    // * Decoded payload size:   Stored in xb_.
    uint32_t xa_ : 29;
    uint32_t xb_ : 29;
} __attribute__((packed));

}  // namespace base
}  // namespace io
}  // namespace strata
