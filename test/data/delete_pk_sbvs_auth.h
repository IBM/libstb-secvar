/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2023 IBM Corp.
 */
unsigned char delete_pk_sbvs_auth[] = {
  0x7a, 0x00, 0x01, 0x15, 0x00, 0x2c, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xec, 0x04, 0x00, 0x00, 0x00, 0x02, 0xf1, 0x0e,
  0x9d, 0xd2, 0xaf, 0x4a, 0xdf, 0x68, 0xee, 0x49, 0x8a, 0xa9, 0x34, 0x7d,
  0x37, 0x56, 0x65, 0xa7, 0x30, 0x82, 0x04, 0xd0, 0x06, 0x09, 0x2a, 0x86,
  0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x02, 0xa0, 0x82, 0x04, 0xc1, 0x30,
  0x82, 0x04, 0xbd, 0x02, 0x01, 0x01, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x09,
  0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x30,
  0x0b, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01,
  0xa0, 0x82, 0x03, 0x2d, 0x30, 0x82, 0x03, 0x29, 0x30, 0x82, 0x02, 0x11,
  0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x14, 0x6b, 0x7b, 0x12, 0xf0, 0xc6,
  0x66, 0x78, 0x78, 0xa1, 0xa2, 0xe8, 0x79, 0x4b, 0x31, 0xa5, 0xa8, 0x9e,
  0x58, 0x7a, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
  0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x24, 0x31, 0x0b, 0x30, 0x09,
  0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x4e, 0x43, 0x31, 0x15, 0x30,
  0x13, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x74, 0x65, 0x73, 0x74,
  0x69, 0x6e, 0x67, 0x20, 0x63, 0x6f, 0x72, 0x70, 0x30, 0x1e, 0x17, 0x0d,
  0x32, 0x30, 0x31, 0x30, 0x31, 0x36, 0x31, 0x39, 0x30, 0x38, 0x30, 0x35,
  0x5a, 0x17, 0x0d, 0x32, 0x30, 0x31, 0x31, 0x31, 0x35, 0x31, 0x39, 0x30,
  0x38, 0x30, 0x35, 0x5a, 0x30, 0x24, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03,
  0x55, 0x04, 0x06, 0x13, 0x02, 0x4e, 0x43, 0x31, 0x15, 0x30, 0x13, 0x06,
  0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x74, 0x65, 0x73, 0x74, 0x69, 0x6e,
  0x67, 0x20, 0x63, 0x6f, 0x72, 0x70, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05,
  0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82,
  0x01, 0x01, 0x00, 0xcf, 0xf3, 0x6e, 0xc9, 0xaf, 0x12, 0x42, 0xdb, 0xa9,
  0x59, 0x6d, 0xef, 0xf6, 0x0e, 0x54, 0xe8, 0x21, 0x57, 0xc4, 0x78, 0x3f,
  0x6d, 0x83, 0x37, 0x1a, 0x29, 0x10, 0xd2, 0xb8, 0xb7, 0x24, 0x49, 0x5f,
  0x5f, 0x32, 0xc9, 0x39, 0xdc, 0x40, 0xa4, 0x5d, 0xea, 0x7b, 0xb7, 0xb3,
  0x42, 0xab, 0x43, 0x47, 0x3d, 0x91, 0x6e, 0xf1, 0x5d, 0x3f, 0xf9, 0x2d,
  0x9e, 0x88, 0xc4, 0x05, 0x73, 0x9b, 0x0a, 0xa8, 0xf7, 0xb2, 0x8e, 0x24,
  0x69, 0x13, 0xf0, 0xbc, 0x5d, 0xde, 0x32, 0x40, 0x42, 0x31, 0x58, 0x9e,
  0x48, 0x76, 0x1f, 0x6b, 0x10, 0x19, 0x1c, 0x4e, 0x45, 0x82, 0x19, 0xc0,
  0xed, 0xfc, 0x5c, 0x7c, 0x19, 0xfd, 0x85, 0x13, 0x2b, 0xf0, 0x38, 0x83,
  0x7b, 0xe5, 0x1d, 0x86, 0x70, 0xed, 0x8f, 0x52, 0x16, 0x60, 0x14, 0xfb,
  0xaf, 0x23, 0x19, 0xa5, 0x45, 0x44, 0x91, 0x54, 0xd0, 0xe8, 0x79, 0x34,
  0xbe, 0x4f, 0xb3, 0x8c, 0x56, 0x8c, 0x7c, 0xbf, 0x6a, 0xa2, 0x44, 0x04,
  0x51, 0xe5, 0x0f, 0x30, 0x04, 0xe9, 0x90, 0xb1, 0x8a, 0xe3, 0x9b, 0x95,
  0xb1, 0xd6, 0xde, 0x9e, 0x98, 0x14, 0x07, 0x63, 0xbd, 0x88, 0x94, 0x54,
  0xb6, 0x03, 0x8d, 0xf5, 0x8e, 0xca, 0x82, 0xc8, 0xb1, 0x78, 0xba, 0xb4,
  0x21, 0x45, 0x6f, 0xd4, 0x0c, 0xda, 0xcb, 0x81, 0x1a, 0x9b, 0xb2, 0x0c,
  0xa9, 0x7c, 0x48, 0x84, 0xc8, 0xbd, 0xce, 0x46, 0x1b, 0x8b, 0x38, 0xe2,
  0x67, 0x50, 0x3c, 0x3e, 0x3e, 0x32, 0xec, 0x0b, 0x39, 0xc7, 0xe0, 0xd5,
  0x00, 0xc5, 0x7a, 0xd3, 0x1c, 0xd1, 0xfb, 0x0a, 0xc7, 0x5b, 0x1d, 0x21,
  0x3d, 0x50, 0x53, 0x87, 0xa1, 0xb4, 0x16, 0x5b, 0x1e, 0x80, 0x80, 0xaa,
  0xe5, 0xf9, 0x69, 0x6c, 0x48, 0xab, 0x45, 0x66, 0x05, 0x04, 0xc7, 0xad,
  0xc1, 0x24, 0x4a, 0xde, 0xad, 0x14, 0x55, 0x02, 0x03, 0x01, 0x00, 0x01,
  0xa3, 0x53, 0x30, 0x51, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04,
  0x16, 0x04, 0x14, 0x69, 0x73, 0x3b, 0xcc, 0xa5, 0x2b, 0x5b, 0xd1, 0xfe,
  0xe9, 0x73, 0xa3, 0xcb, 0x90, 0x84, 0x0a, 0xa3, 0x60, 0x5f, 0xbb, 0x30,
  0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14,
  0x69, 0x73, 0x3b, 0xcc, 0xa5, 0x2b, 0x5b, 0xd1, 0xfe, 0xe9, 0x73, 0xa3,
  0xcb, 0x90, 0x84, 0x0a, 0xa3, 0x60, 0x5f, 0xbb, 0x30, 0x0f, 0x06, 0x03,
  0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01,
  0xff, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01,
  0x01, 0x0b, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x99, 0xdd, 0xbb,
  0x72, 0xf5, 0x8e, 0xdc, 0x7b, 0xe7, 0x9c, 0x6d, 0x47, 0x37, 0x01, 0xc9,
  0xc2, 0xc9, 0x90, 0x01, 0x5d, 0x26, 0x80, 0xce, 0x2b, 0xd3, 0xdd, 0x26,
  0x67, 0x2a, 0x77, 0x41, 0x06, 0x78, 0xed, 0xe7, 0xb0, 0x58, 0x93, 0x44,
  0xe5, 0x79, 0xc0, 0xa6, 0x52, 0x1d, 0x2c, 0x10, 0x16, 0xaa, 0xee, 0x97,
  0x05, 0x80, 0x6d, 0xba, 0x45, 0xd8, 0xd3, 0xf5, 0x98, 0x8d, 0xd1, 0x66,
  0x9a, 0x1c, 0x96, 0xf5, 0x2e, 0xb4, 0x4d, 0xa3, 0x79, 0x5f, 0x81, 0xb9,
  0x5c, 0xcd, 0x45, 0x80, 0xa4, 0x15, 0xb3, 0x3e, 0x05, 0xf9, 0x12, 0x62,
  0x41, 0x7b, 0x97, 0x08, 0x90, 0x13, 0xd3, 0x1a, 0xe7, 0xe5, 0x68, 0x1d,
  0x24, 0xdb, 0x7c, 0x78, 0xd7, 0x3a, 0x4a, 0x5d, 0x7c, 0xb7, 0xf8, 0x5d,
  0xa4, 0xe2, 0x40, 0x86, 0x26, 0xc0, 0x45, 0x00, 0x80, 0x16, 0x88, 0x97,
  0x3e, 0x8c, 0x5c, 0x38, 0x6b, 0xa6, 0x3f, 0x7d, 0x80, 0xb4, 0xec, 0x1f,
  0x89, 0x0a, 0xd3, 0x64, 0x3a, 0x85, 0xab, 0xd7, 0x0b, 0x17, 0x68, 0xc1,
  0x41, 0xfc, 0xe6, 0xa0, 0x2d, 0xc8, 0x0e, 0xfd, 0xf0, 0x28, 0xf3, 0xe0,
  0xb7, 0x98, 0xf3, 0xc9, 0x93, 0x97, 0xeb, 0x7f, 0x81, 0x13, 0x86, 0x95,
  0x17, 0x45, 0x0d, 0x3e, 0x0d, 0x35, 0x5d, 0x2d, 0xa4, 0xa4, 0x04, 0xaa,
  0x22, 0x7b, 0x40, 0x47, 0xc7, 0x31, 0x88, 0x99, 0x03, 0xaf, 0xf7, 0xe6,
  0x14, 0x1b, 0xcf, 0xf7, 0x3b, 0x5d, 0xc6, 0x48, 0x24, 0x42, 0xcf, 0xfe,
  0x10, 0x10, 0xc0, 0x2b, 0x23, 0x28, 0xb8, 0x4a, 0x3a, 0xff, 0x21, 0xd4,
  0xa3, 0x15, 0x51, 0xbc, 0xd4, 0xd2, 0x09, 0x77, 0x77, 0x3e, 0x65, 0xb4,
  0x3d, 0x1e, 0xd6, 0xc0, 0xe9, 0x3b, 0x0e, 0xee, 0xa8, 0x68, 0x46, 0x25,
  0x47, 0x57, 0x08, 0x2e, 0x80, 0x99, 0x9b, 0x49, 0xfb, 0xd9, 0xc5, 0x46,
  0xd7, 0x31, 0x82, 0x01, 0x67, 0x30, 0x82, 0x01, 0x63, 0x02, 0x01, 0x01,
  0x30, 0x3c, 0x30, 0x24, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
  0x06, 0x13, 0x02, 0x4e, 0x43, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55,
  0x04, 0x0a, 0x0c, 0x0c, 0x74, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x67, 0x20,
  0x63, 0x6f, 0x72, 0x70, 0x02, 0x14, 0x6b, 0x7b, 0x12, 0xf0, 0xc6, 0x66,
  0x78, 0x78, 0xa1, 0xa2, 0xe8, 0x79, 0x4b, 0x31, 0xa5, 0xa8, 0x9e, 0x58,
  0x7a, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03,
  0x04, 0x02, 0x01, 0x05, 0x00, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48,
  0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x04, 0x82, 0x01, 0x00,
  0x7c, 0x58, 0x50, 0xd2, 0x9c, 0xea, 0x3d, 0x31, 0x03, 0xf9, 0x9c, 0xe2,
  0x6f, 0x4b, 0x1c, 0x78, 0x70, 0xa5, 0x02, 0x9d, 0x0b, 0x06, 0xf7, 0x74,
  0x49, 0x49, 0x8c, 0x08, 0x37, 0x8e, 0x50, 0x99, 0x61, 0xce, 0x8c, 0xd8,
  0x2c, 0x41, 0xc5, 0x74, 0xef, 0x1f, 0x9a, 0x33, 0xf2, 0xa6, 0x7e, 0xca,
  0x57, 0xb1, 0x15, 0xfb, 0xb4, 0x43, 0xdf, 0x39, 0xbc, 0x9a, 0x9b, 0xd8,
  0xaa, 0xfd, 0x0a, 0xce, 0x15, 0x62, 0xf2, 0xaa, 0x02, 0x8f, 0xe9, 0xf5,
  0x80, 0x38, 0xdd, 0x9f, 0x45, 0x9d, 0x72, 0xc3, 0x3d, 0xba, 0xc6, 0x71,
  0x8d, 0xce, 0x83, 0x41, 0xb5, 0x65, 0xbe, 0x63, 0x69, 0x74, 0x16, 0xa2,
  0xa3, 0xbc, 0xd4, 0xe5, 0x53, 0x08, 0xa8, 0x32, 0x39, 0xbb, 0x0b, 0x2c,
  0xb4, 0x7b, 0xc5, 0x90, 0x2c, 0x60, 0x6c, 0xd9, 0xea, 0xce, 0xe3, 0xed,
  0xca, 0x3f, 0x23, 0x07, 0xce, 0x51, 0x67, 0x90, 0x82, 0xf2, 0x29, 0x79,
  0xcb, 0xf5, 0x14, 0x4d, 0x1e, 0x91, 0x79, 0x22, 0xd1, 0x32, 0x82, 0xfd,
  0xce, 0xc0, 0x25, 0x21, 0x2a, 0x07, 0x0b, 0x00, 0xea, 0xfa, 0x8f, 0xeb,
  0x38, 0x93, 0xe3, 0xae, 0x51, 0x29, 0x8b, 0xa7, 0x0e, 0xd3, 0x2e, 0xcd,
  0x62, 0xf4, 0x55, 0x42, 0x2f, 0x75, 0x90, 0xd3, 0x56, 0x28, 0x76, 0x1f,
  0x1f, 0x2f, 0x26, 0x58, 0x61, 0xd1, 0xb7, 0x87, 0x9d, 0xd9, 0x37, 0x65,
  0x5f, 0x18, 0x29, 0x30, 0x34, 0xce, 0xbd, 0xf0, 0x9a, 0x54, 0x37, 0x57,
  0x55, 0x0a, 0xef, 0x09, 0x4b, 0x14, 0xc9, 0x93, 0xea, 0x89, 0xed, 0x96,
  0x75, 0x29, 0xd0, 0x27, 0x65, 0x91, 0x74, 0xaa, 0x58, 0x41, 0x13, 0x35,
  0xe0, 0x21, 0x0c, 0x2a, 0x27, 0x54, 0x65, 0xd3, 0x2f, 0x17, 0x7c, 0xac,
  0x08, 0x35, 0x2b, 0xf3, 0x43, 0x53, 0xd5, 0x75, 0xc4, 0xf4, 0xee, 0xa3,
  0xf4, 0x35, 0xf8, 0xf6
};
unsigned int delete_pk_sbvs_auth_len = 1276;
