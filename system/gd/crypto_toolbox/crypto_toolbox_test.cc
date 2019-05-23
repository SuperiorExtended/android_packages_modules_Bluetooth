/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>

#include "crypto_toolbox/aes.h"
#include "crypto_toolbox/crypto_toolbox.h"

#include <vector>

namespace bluetooth {
namespace crypto_toolbox {

// BT Spec 5.0 | Vol 3, Part H D.1
TEST(CryptoToolboxTest, bt_spec_test_d_1_test) {
  uint8_t k[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

  uint8_t m[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t aes_cmac_k_m[] = {0x7d, 0xf7, 0x6b, 0x0c, 0x1a, 0xb8, 0x99, 0xb3,
                            0x3e, 0x42, 0xf0, 0x47, 0xb9, 0x1b, 0x54, 0x6f};

  uint8_t output[16];
  aes_context ctx;
  aes_set_key(k, sizeof(k), &ctx);
  aes_encrypt(m, output, &ctx); /* outputs in byte 48 to byte 63 */

  EXPECT_TRUE(memcmp(output, aes_cmac_k_m, OCTET16_LEN));

  // useful for debugging
  // LOG(INFO) << "k " << base::HexEncode(k, OCTET16_LEN);
  // LOG(INFO) << "m " << base::HexEncode(m, sizeof(m));
  // LOG(INFO) << "output " << base::HexEncode(output, OCTET16_LEN);
}

// BT Spec 5.0 | Vol 3, Part H D.1.1
TEST(CryptoToolboxTest, bt_spec_example_d_1_1_test) {
  Octet16 k{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

  Octet16 aes_cmac_k_m{0xbb, 0x1d, 0x69, 0x29, 0xe9, 0x59, 0x37, 0x28, 0x7f, 0xa3, 0x7d, 0x12, 0x9b, 0x75, 0x67, 0x46};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(k), std::end(k));
  std::reverse(std::begin(aes_cmac_k_m), std::end(aes_cmac_k_m));

  Octet16 output = aes_cmac(k, nullptr /* empty message */, 0);

  EXPECT_EQ(output, aes_cmac_k_m);

  // useful for debugging
  // LOG(INFO) << "k " << base::HexEncode(k.data(), k.size());
  // LOG(INFO) << "aes_cmac(k,nullptr) "
  //           << base::HexEncode(output.data(), output.size());
}

// BT Spec 5.0 | Vol 3, Part H D.1.2
TEST(CryptoToolboxTest, bt_spec_example_d_1_2_test) {
  Octet16 k{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

  Octet16 m = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};

  Octet16 aes_cmac_k_m{0x07, 0x0a, 0x16, 0xb4, 0x6b, 0x4d, 0x41, 0x44, 0xf7, 0x9b, 0xdd, 0x9d, 0xd0, 0x4a, 0x28, 0x7c};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(k), std::end(k));
  std::reverse(std::begin(m), std::end(m));
  std::reverse(std::begin(aes_cmac_k_m), std::end(aes_cmac_k_m));

  Octet16 output = aes_cmac(k, m);

  EXPECT_EQ(output, aes_cmac_k_m);

  // useful for debugging
  // LOG(INFO) << "k " << base::HexEncode(k.data(), k.size());
  // LOG(INFO) << "m " << base::HexEncode(m, sizeof(m));
  // LOG(INFO) << "aes_cmac(k,m) "
  //           << base::HexEncode(output.data(), output.size());
}

// BT Spec 5.0 | Vol 3, Part H D.1.3
TEST(CryptoToolboxTest, bt_spec_example_d_1_3_test) {
  Octet16 k{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

  uint8_t m[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93,
                 0x17, 0x2a, 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac,
                 0x45, 0xaf, 0x8e, 0x51, 0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11};

  Octet16 aes_cmac_k_m{0xdf, 0xa6, 0x67, 0x47, 0xde, 0x9a, 0xe6, 0x30, 0x30, 0xca, 0x32, 0x61, 0x14, 0x97, 0xc8, 0x27};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(k), std::end(k));
  std::reverse(std::begin(m), std::end(m));
  std::reverse(std::begin(aes_cmac_k_m), std::end(aes_cmac_k_m));

  Octet16 output = aes_cmac(k, m, sizeof(m));
  EXPECT_EQ(output, aes_cmac_k_m);
}

// BT Spec 5.0 | Vol 3, Part H D.1.4
TEST(CryptoToolboxTest, bt_spec_example_d_1_4_test) {
  Octet16 k{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

  uint8_t m[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                 0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                 0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};

  Octet16 aes_cmac_k_m{0x51, 0xf0, 0xbe, 0xbf, 0x7e, 0x3b, 0x9d, 0x92, 0xfc, 0x49, 0x74, 0x17, 0x79, 0x36, 0x3c, 0xfe};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(k), std::end(k));
  std::reverse(std::begin(m), std::end(m));
  std::reverse(std::begin(aes_cmac_k_m), std::end(aes_cmac_k_m));

  Octet16 output = aes_cmac(k, m, sizeof(m));

  EXPECT_EQ(output, aes_cmac_k_m);
}

// BT Spec 5.0 | Vol 3, Part H D.2
TEST(CryptoToolboxTest, bt_spec_example_d_2_test) {
  std::vector<uint8_t> u{0x20, 0xb0, 0x03, 0xd2, 0xf2, 0x97, 0xbe, 0x2c, 0x5e, 0x2c, 0x83,
                         0xa7, 0xe9, 0xf9, 0xa5, 0xb9, 0xef, 0xf4, 0x91, 0x11, 0xac, 0xf4,
                         0xfd, 0xdb, 0xcc, 0x03, 0x01, 0x48, 0x0e, 0x35, 0x9d, 0xe6};
  std::vector<uint8_t> v{0x55, 0x18, 0x8b, 0x3d, 0x32, 0xf6, 0xbb, 0x9a, 0x90, 0x0a, 0xfc,
                         0xfb, 0xee, 0xd4, 0xe7, 0x2a, 0x59, 0xcb, 0x9a, 0xc2, 0xf1, 0x9d,
                         0x7c, 0xfb, 0x6b, 0x4f, 0xdd, 0x49, 0xf4, 0x7f, 0xc5, 0xfd};
  Octet16 x{0xd5, 0xcb, 0x84, 0x54, 0xd1, 0x77, 0x73, 0x3e, 0xff, 0xff, 0xb2, 0xec, 0x71, 0x2b, 0xae, 0xab};
  uint8_t z = 0x00;

  Octet16 aes_cmac_k_m{0xf2, 0xc9, 0x16, 0xf1, 0x07, 0xa9, 0xbd, 0x1c, 0xf1, 0xed, 0xa1, 0xbe, 0xa9, 0x74, 0x87, 0x2d};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(u), std::end(u));
  std::reverse(std::begin(v), std::end(v));
  std::reverse(std::begin(x), std::end(x));
  std::reverse(std::begin(aes_cmac_k_m), std::end(aes_cmac_k_m));

  Octet16 output = f4(u.data(), v.data(), x, z);

  EXPECT_EQ(output, aes_cmac_k_m);
}

// BT Spec 5.0 | Vol 3, Part H D.3
TEST(CryptoToolboxTest, bt_spec_example_d_3_test) {
  std::array<uint8_t, 32> dhkey_w{0xec, 0x02, 0x34, 0xa3, 0x57, 0xc8, 0xad, 0x05, 0x34, 0x10, 0x10,
                                  0xa6, 0x0a, 0x39, 0x7d, 0x9b, 0x99, 0x79, 0x6b, 0x13, 0xb4, 0xf8,
                                  0x66, 0xf1, 0x86, 0x8d, 0x34, 0xf3, 0x73, 0xbf, 0xa6, 0x98};
  Octet16 n1{0xd5, 0xcb, 0x84, 0x54, 0xd1, 0x77, 0x73, 0x3e, 0xff, 0xff, 0xb2, 0xec, 0x71, 0x2b, 0xae, 0xab};
  Octet16 n2{0xa6, 0xe8, 0xe7, 0xcc, 0x25, 0xa7, 0x5f, 0x6e, 0x21, 0x65, 0x83, 0xf7, 0xff, 0x3d, 0xc4, 0xcf};
  std::array<uint8_t, 7> a1{0x00, 0x56, 0x12, 0x37, 0x37, 0xbf, 0xce};
  std::array<uint8_t, 7> a2{0x00, 0xa7, 0x13, 0x70, 0x2d, 0xcf, 0xc1};

  Octet16 expected_ltk{0x69, 0x86, 0x79, 0x11, 0x69, 0xd7, 0xcd, 0x23, 0x98, 0x05, 0x22, 0xb5, 0x94, 0x75, 0x0a, 0x38};
  Octet16 expected_mac_key{0x29, 0x65, 0xf1, 0x76, 0xa1, 0x08, 0x4a, 0x02,
                           0xfd, 0x3f, 0x6a, 0x20, 0xce, 0x63, 0x6e, 0x20};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(dhkey_w), std::end(dhkey_w));
  std::reverse(std::begin(n1), std::end(n1));
  std::reverse(std::begin(n2), std::end(n2));
  std::reverse(std::begin(a1), std::end(a1));
  std::reverse(std::begin(a2), std::end(a2));
  std::reverse(std::begin(expected_ltk), std::end(expected_ltk));
  std::reverse(std::begin(expected_mac_key), std::end(expected_mac_key));

  Octet16 mac_key, ltk;
  f5(dhkey_w.data(), n1, n2, a1.data(), a2.data(), &mac_key, &ltk);

  EXPECT_EQ(mac_key, expected_mac_key);
  EXPECT_EQ(ltk, expected_ltk);
}

// BT Spec 5.0 | Vol 3, Part H D.4
TEST(CryptoToolboxTest, bt_spec_example_d_4_test) {
  Octet16 n1{0xd5, 0xcb, 0x84, 0x54, 0xd1, 0x77, 0x73, 0x3e, 0xff, 0xff, 0xb2, 0xec, 0x71, 0x2b, 0xae, 0xab};
  Octet16 n2{0xa6, 0xe8, 0xe7, 0xcc, 0x25, 0xa7, 0x5f, 0x6e, 0x21, 0x65, 0x83, 0xf7, 0xff, 0x3d, 0xc4, 0xcf};
  Octet16 r{0x12, 0xa3, 0x34, 0x3b, 0xb4, 0x53, 0xbb, 0x54, 0x08, 0xda, 0x42, 0xd2, 0x0c, 0x2d, 0x0f, 0xc8};
  std::vector<uint8_t> IOcap{0x01, 0x01, 0x02};
  std::vector<uint8_t> a1{0x00, 0x56, 0x12, 0x37, 0x37, 0xbf, 0xce};
  std::vector<uint8_t> a2{0x00, 0xa7, 0x13, 0x70, 0x2d, 0xcf, 0xc1};

  Octet16 MacKey{0x29, 0x65, 0xf1, 0x76, 0xa1, 0x08, 0x4a, 0x02, 0xfd, 0x3f, 0x6a, 0x20, 0xce, 0x63, 0x6e, 0x20};

  Octet16 expected_aes_cmac{0xe3, 0xc4, 0x73, 0x98, 0x9c, 0xd0, 0xe8, 0xc5,
                            0xd2, 0x6c, 0x0b, 0x09, 0xda, 0x95, 0x8f, 0x61};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(n1), std::end(n1));
  std::reverse(std::begin(n2), std::end(n2));
  std::reverse(std::begin(r), std::end(r));
  std::reverse(std::begin(IOcap), std::end(IOcap));
  std::reverse(std::begin(a1), std::end(a1));
  std::reverse(std::begin(a2), std::end(a2));
  std::reverse(std::begin(MacKey), std::end(MacKey));
  std::reverse(std::begin(expected_aes_cmac), std::end(expected_aes_cmac));

  Octet16 aes_cmac = f6(MacKey, n1, n2, r, IOcap.data(), a1.data(), a2.data());

  EXPECT_EQ(aes_cmac, expected_aes_cmac);
}

// BT Spec 5.0 | Vol 3, Part H D.5
TEST(CryptoToolboxTest, bt_spec_example_d_5_test) {
  std::array<uint8_t, 32> u{0x20, 0xb0, 0x03, 0xd2, 0xf2, 0x97, 0xbe, 0x2c, 0x5e, 0x2c, 0x83,
                            0xa7, 0xe9, 0xf9, 0xa5, 0xb9, 0xef, 0xf4, 0x91, 0x11, 0xac, 0xf4,
                            0xfd, 0xdb, 0xcc, 0x03, 0x01, 0x48, 0x0e, 0x35, 0x9d, 0xe6};
  std::array<uint8_t, 32> v{0x55, 0x18, 0x8b, 0x3d, 0x32, 0xf6, 0xbb, 0x9a, 0x90, 0x0a, 0xfc,
                            0xfb, 0xee, 0xd4, 0xe7, 0x2a, 0x59, 0xcb, 0x9a, 0xc2, 0xf1, 0x9d,
                            0x7c, 0xfb, 0x6b, 0x4f, 0xdd, 0x49, 0xf4, 0x7f, 0xc5, 0xfd};

  Octet16 x{0xd5, 0xcb, 0x84, 0x54, 0xd1, 0x77, 0x73, 0x3e, 0xff, 0xff, 0xb2, 0xec, 0x71, 0x2b, 0xae, 0xab};
  Octet16 y{0xa6, 0xe8, 0xe7, 0xcc, 0x25, 0xa7, 0x5f, 0x6e, 0x21, 0x65, 0x83, 0xf7, 0xff, 0x3d, 0xc4, 0xcf};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(u), std::end(u));
  std::reverse(std::begin(v), std::end(v));
  std::reverse(std::begin(x), std::end(x));
  std::reverse(std::begin(y), std::end(y));

  uint32_t val = g2(u.data(), v.data(), x, y);

  /* the returned value is already mod 1000000, so do mod on the test result
   * value too */
  EXPECT_EQ(val, 0x2f9ed5baU % 1000000);
}

// BT Spec 5.0 | Vol 3, Part H D.6
TEST(CryptoToolboxTest, bt_spec_example_d_6_test) {
  Octet16 key{0xec, 0x02, 0x34, 0xa3, 0x57, 0xc8, 0xad, 0x05, 0x34, 0x10, 0x10, 0xa6, 0x0a, 0x39, 0x7d, 0x9b};
  std::array<uint8_t, 4> keyID{0x6c, 0x65, 0x62, 0x72};
  Octet16 expected_aes_cmac{0x2d, 0x9a, 0xe1, 0x02, 0xe7, 0x6d, 0xc9, 0x1c,
                            0xe8, 0xd3, 0xa9, 0xe2, 0x80, 0xb1, 0x63, 0x99};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(key), std::end(key));
  std::reverse(std::begin(keyID), std::end(keyID));
  std::reverse(std::begin(expected_aes_cmac), std::end(expected_aes_cmac));

  Octet16 aes_cmac = h6(key, keyID);
  EXPECT_EQ(aes_cmac, expected_aes_cmac);
}

// BT Spec 5.0 | Vol 3, Part H D.7
TEST(CryptoToolboxTest, bt_spec_example_d_7_test) {
  Octet16 IRK{0xec, 0x02, 0x34, 0xa3, 0x57, 0xc8, 0xad, 0x05, 0x34, 0x10, 0x10, 0xa6, 0x0a, 0x39, 0x7d, 0x9b};
  Octet16 prand{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x81, 0x94};
  Octet16 expected_aes_128{0x15, 0x9d, 0x5f, 0xb7, 0x2e, 0xbe, 0x23, 0x11,
                           0xa4, 0x8c, 0x1b, 0xdc, 0xc4, 0x0d, 0xfb, 0xaa};
  std::array<uint8_t, 3> expected_ah{0x0d, 0xfb, 0xaa};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(IRK), std::end(IRK));
  std::reverse(std::begin(prand), std::end(prand));
  std::reverse(std::begin(expected_aes_128), std::end(expected_aes_128));
  std::reverse(std::begin(expected_ah), std::end(expected_ah));

  Octet16 result = aes_128(IRK, prand.data(), 3);
  EXPECT_EQ(expected_aes_128, result);

  // little/big endian 24 bits
  EXPECT_EQ(result[0], expected_ah[0]);
  EXPECT_EQ(result[1], expected_ah[1]);
  EXPECT_EQ(result[2], expected_ah[2]);
}

// BT Spec 5.0 | Vol 3, Part H D.8
TEST(CryptoToolboxTest, bt_spec_example_d_8_test) {
  Octet16 Key{0xec, 0x02, 0x34, 0xa3, 0x57, 0xc8, 0xad, 0x05, 0x34, 0x10, 0x10, 0xa6, 0x0a, 0x39, 0x7d, 0x9b};
  Octet16 SALT{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x74, 0x6D, 0x70, 0x31};
  Octet16 expected_aes_cmac{0xfb, 0x17, 0x35, 0x97, 0xc6, 0xa3, 0xc0, 0xec,
                            0xd2, 0x99, 0x8c, 0x2a, 0x75, 0xa5, 0x70, 0x11};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(Key), std::end(Key));
  std::reverse(std::begin(SALT), std::end(SALT));
  std::reverse(std::begin(expected_aes_cmac), std::end(expected_aes_cmac));

  Octet16 aes_cmac = h7(SALT, Key);
  EXPECT_EQ(expected_aes_cmac, aes_cmac);
}

extern Octet16 smp_calculate_ltk_to_link_key(const Octet16& ltk, bool use_h7);

// BT Spec 5.0 | Vol 3, Part H D.9
TEST(CryptoToolboxTest, bt_spec_example_d_9_test) {
  Octet16 LTK{0x36, 0x8d, 0xf9, 0xbc, 0xe3, 0x26, 0x4b, 0x58, 0xbd, 0x06, 0x6c, 0x33, 0x33, 0x4f, 0xbf, 0x64};
  Octet16 expected_link_key{0x28, 0x7a, 0xd3, 0x79, 0xdc, 0xa4, 0x02, 0x53,
                            0x0a, 0x39, 0xf1, 0xf4, 0x30, 0x47, 0xb8, 0x35};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(LTK), std::end(LTK));
  std::reverse(std::begin(expected_link_key), std::end(expected_link_key));

  Octet16 link_key = ltk_to_link_key(LTK, true);
  EXPECT_EQ(expected_link_key, link_key);
}

// BT Spec 5.0 | Vol 3, Part H D.10
TEST(CryptoToolboxTest, bt_spec_example_d_10_test) {
  Octet16 LTK{0x36, 0x8d, 0xf9, 0xbc, 0xe3, 0x26, 0x4b, 0x58, 0xbd, 0x06, 0x6c, 0x33, 0x33, 0x4f, 0xbf, 0x64};
  Octet16 expected_link_key{0xbc, 0x1c, 0xa4, 0xef, 0x63, 0x3f, 0xc1, 0xbd,
                            0x0d, 0x82, 0x30, 0xaf, 0xee, 0x38, 0x8f, 0xb0};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(LTK), std::end(LTK));
  std::reverse(std::begin(expected_link_key), std::end(expected_link_key));

  Octet16 link_key = ltk_to_link_key(LTK, false);
  EXPECT_EQ(expected_link_key, link_key);
}

// // BT Spec 5.0 | Vol 3, Part H D.11
TEST(CryptoToolboxTest, bt_spec_example_d_11_test) {
  Octet16 link_key{0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
  Octet16 expected_ltk{0xe8, 0x5e, 0x09, 0xeb, 0x5e, 0xcc, 0xb3, 0xe2, 0x69, 0x41, 0x8a, 0x13, 0x32, 0x11, 0xbc, 0x79};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(link_key), std::end(link_key));
  std::reverse(std::begin(expected_ltk), std::end(expected_ltk));

  Octet16 ltk = link_key_to_ltk(link_key, true);
  EXPECT_EQ(expected_ltk, ltk);
}

// BT Spec 5.0 | Vol 3, Part H D.12
TEST(CryptoToolboxTest, bt_spec_example_d_12_test) {
  Octet16 link_key{0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
  Octet16 expected_ltk{0xa8, 0x13, 0xfb, 0x72, 0xf1, 0xa3, 0xdf, 0xa1, 0x8a, 0x2c, 0x9a, 0x43, 0xf1, 0x0d, 0x0a, 0x30};

  // algorithm expect all input to be in little endian format, so reverse
  std::reverse(std::begin(link_key), std::end(link_key));
  std::reverse(std::begin(expected_ltk), std::end(expected_ltk));

  Octet16 ltk = link_key_to_ltk(link_key, false);
  EXPECT_EQ(expected_ltk, ltk);
}

}  // namespace crypto_toolbox
}  // namespace bluetooth
