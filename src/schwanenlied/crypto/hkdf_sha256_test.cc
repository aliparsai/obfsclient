/*
 * Copyright (c) 2014, Yawning Angel <yawning at schwanenlied dot me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "schwanenlied/crypto/hkdf_sha256.h"
#include "gtest/gtest.h"

namespace schwanenlied {
namespace crypto {

class HkdfSha256Test : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(HkdfSha256Test, RFC5968_Case_1) {
  // Basic test case with SHA-256
  const ::std::array<uint8_t, 22> ikm = { {
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b
  } };
  const ::std::array<uint8_t, 13> salt = { {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x0c
  } };
  const ::std::array<uint8_t, 10> info = { {
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9
  } };
  const ::std::array<uint8_t, 32> prk_expected = { {
    0x07, 0x77, 0x09, 0x36, 0x2c, 0x2e, 0x32, 0xdf, 0x0d, 0xdc,
    0x3f, 0x0d, 0xc4, 0x7b, 0xba, 0x63, 0x90, 0xb6, 0xc7, 0x3b,
    0xb5, 0x0f, 0x9c, 0x31, 0x22, 0xec, 0x84, 0x4a, 0xd7, 0xc2,
    0xb3, 0xe5
  } };
  const ::std::array<uint8_t, 42> okm_expected = { {
    0x3c, 0xb2, 0x5f, 0x25, 0xfa, 0xac, 0xd5, 0x7a, 0x90, 0x43,
    0x4f, 0x64, 0xd0, 0x36, 0x2f, 0x2a, 0x2d, 0x2d, 0x0a, 0x90,
    0xcf, 0x1a, 0x5a, 0x4c, 0x5d, 0xb0, 0x2d, 0x56, 0xec, 0xc4,
    0xc5, 0xbf, 0x34, 0x00, 0x72, 0x08, 0xd5, 0xb8, 0x87, 0x18,
    0x58, 0x65 
  } };

  SecureBuffer prk = HkdfSha256::extract(salt.data(), salt.size(),
                                         SecureBuffer(ikm.data(), ikm.size()));
  ASSERT_TRUE(prk.size() == prk_expected.size());
  ASSERT_TRUE(memequals(prk.data(), prk_expected.data(), prk.size()));

  SecureBuffer okm = HkdfSha256::expand(prk, info.data(), info.size(), 42);
  ASSERT_TRUE(okm.size() == okm_expected.size());
  ASSERT_TRUE(memequals(okm.data(), okm_expected.data(), okm.size()));
}

TEST_F(HkdfSha256Test, RFC5968_Case_2) {
  // Test with SHA-256 and longer inputs/outputs
  ::std::array<uint8_t, 80> ikm;
  ::std::array<uint8_t, 80> salt;
  ::std::array<uint8_t, 80> info;

  for (uint8_t i = 0; i < ikm.size(); i++)
    ikm[i] = i;
  for (uint8_t i = 0; i < salt.size(); i++)
    salt[i] = 0x60 + i;
  for (uint8_t i = 0; i < info.size(); i++)
    info[i] = 0xb0 + i;

  const ::std::array<uint8_t, 32> prk_expected = { {
    0x06, 0xa6, 0xb8, 0x8c, 0x58, 0x53, 0x36, 0x1a, 0x06, 0x10,
    0x4c, 0x9c, 0xeb, 0x35, 0xb4, 0x5c, 0xef, 0x76, 0x00, 0x14,
    0x90, 0x46, 0x71, 0x01, 0x4a, 0x19, 0x3f, 0x40, 0xc1, 0x5f,
    0xc2, 0x44
  } };
  const ::std::array<uint8_t, 82> okm_expected = { {
    0xb1, 0x1e, 0x39, 0x8d, 0xc8, 0x03, 0x27, 0xa1, 0xc8, 0xe7,
    0xf7, 0x8c, 0x59, 0x6a, 0x49, 0x34, 0x4f, 0x01, 0x2e, 0xda,
    0x2d, 0x4e, 0xfa, 0xd8, 0xa0, 0x50, 0xcc, 0x4c, 0x19, 0xaf,
    0xa9, 0x7c, 0x59, 0x04, 0x5a, 0x99, 0xca, 0xc7, 0x82, 0x72,
    0x71, 0xcb, 0x41, 0xc6, 0x5e, 0x59, 0x0e, 0x09, 0xda, 0x32,
    0x75, 0x60, 0x0c, 0x2f, 0x09, 0xb8, 0x36, 0x77, 0x93, 0xa9,
    0xac, 0xa3, 0xdb, 0x71, 0xcc, 0x30, 0xc5, 0x81, 0x79, 0xec,
    0x3e, 0x87, 0xc1, 0x4c, 0x01, 0xd5, 0xc1, 0xf3, 0x43, 0x4f,
    0x1d, 0x87
  } };

  SecureBuffer prk = HkdfSha256::extract(salt.data(), salt.size(),
                                         SecureBuffer(ikm.data(), ikm.size()));
  ASSERT_TRUE(prk.size() == prk_expected.size());
  ASSERT_TRUE(memequals(prk.data(), prk_expected.data(), prk.size()));

  SecureBuffer okm = HkdfSha256::expand(prk, info.data(), info.size(), 82);
  ASSERT_TRUE(okm.size() == okm_expected.size());
  ASSERT_TRUE(memequals(okm.data(), okm_expected.data(), okm.size()));
}

TEST_F(HkdfSha256Test, RFC5968_Case_3) {
  // Test with SHA-256 and zero-length salt/info
  const ::std::array<uint8_t, 22> ikm = { {
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b
  } };
  const ::std::array<uint8_t, 32> prk_expected = { {
    0x19, 0xef, 0x24, 0xa3, 0x2c, 0x71, 0x7b, 0x16, 0x7f, 0x33,
    0xa9, 0x1d, 0x6f, 0x64, 0x8b, 0xdf, 0x96, 0x59, 0x67, 0x76,
    0xaf, 0xdb, 0x63, 0x77, 0xac, 0x43, 0x4c, 0x1c, 0x29, 0x3c,
    0xcb, 0x04
  } };
  const ::std::array<uint8_t, 42> okm_expected = { {
    0x8d, 0xa4, 0xe7, 0x75, 0xa5, 0x63, 0xc1, 0x8f, 0x71, 0x5f,
    0x80, 0x2a, 0x06, 0x3c, 0x5a, 0x31, 0xb8, 0xa1, 0x1f, 0x5c,
    0x5e, 0xe1, 0x87, 0x9e, 0xc3, 0x45, 0x4e, 0x5f, 0x3c, 0x73,
    0x8d, 0x2d, 0x9d, 0x20, 0x13, 0x95, 0xfa, 0xa4, 0xb6, 0x1a,
    0x96, 0xc8
  } };

  SecureBuffer prk = HkdfSha256::extract(nullptr, 0,
                                         SecureBuffer(ikm.data(), ikm.size()));
  ASSERT_TRUE(prk.size() == prk_expected.size());
  ASSERT_TRUE(memequals(prk.data(), prk_expected.data(), prk.size()));

  SecureBuffer okm = HkdfSha256::expand(prk, nullptr, 0, 42);
  ASSERT_TRUE(okm.size() == okm_expected.size());
  ASSERT_TRUE(memequals(okm.data(), okm_expected.data(), okm.size()));
}

} // namespace crypto
} // namespace schwanenlied
