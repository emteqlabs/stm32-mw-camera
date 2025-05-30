/**
  ******************************************************************************
  * @file    vd55g1_patch_cut_2.c
  * @author  MDG Application Team
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#define VD55G1_FWPATCH_REVISION_MAJOR_CUT_2 2
#define VD55G1_FWPATCH_REVISION_MINOR_CUT_2 2

static const uint8_t patch_array_cut_2[] = {
0xde, 0x00, 0x02, 0x02, 0x52, 0x00, 0x42, 0x00, 0x56, 0x00, 0x42, 0x00, 0x5c,
0x00, 0x42, 0x00, 0x52, 0x00, 0x42, 0x00, 0x4c, 0x04, 0x04, 0xfa, 0xc6, 0x0f,
0x94, 0xe0, 0x19, 0x0e, 0xc9, 0x65, 0x01, 0xc0, 0x28, 0xde, 0x0a, 0x42, 0x80,
0xe0, 0x20, 0x42, 0xf8, 0xf3, 0x16, 0xde, 0xc5, 0x8a, 0x19, 0x00, 0xb8, 0xe0,
0x10, 0x02, 0x0c, 0xec, 0x1d, 0xe6, 0x14, 0x02, 0x88, 0x80, 0x4e, 0x04, 0x01,
0x00, 0x10, 0x80, 0x25, 0x02, 0x08, 0x9c, 0x86, 0x02, 0x00, 0x80, 0x08, 0x44,
0x00, 0x98, 0x55, 0x81, 0x11, 0x85, 0x45, 0x81, 0x11, 0x89, 0x25, 0x81, 0x11,
0x83, 0x2b, 0x00, 0x24, 0xe0, 0x64, 0xc2, 0x0b, 0x84, 0xa8, 0x5d, 0x00, 0xef,
0x2b, 0x80, 0x01, 0x83, 0x1b, 0x8c, 0xd8, 0x49, 0x60, 0xef, 0x0b, 0xa1, 0x65,
0x82, 0x0b, 0x0e, 0x88, 0xf9, 0x0a, 0x00, 0x00, 0xe8, 0x09, 0x0e, 0x0c, 0x80,
0x00, 0x40, 0x04, 0x9c, 0x01, 0x4e, 0x0c, 0x80, 0x4c, 0x0c, 0x04, 0xf2, 0x93,
0xdd, 0x4c, 0x04, 0x0c, 0xfe, 0x46, 0x4f, 0xfc, 0xe0, 0x6b, 0x80, 0x84, 0x9c,
0x46, 0x41, 0x38, 0xe2, 0x0e, 0x9c, 0x0b, 0x8c, 0xe8, 0x4a, 0x60, 0xef, 0x0b,
0x8c, 0x8e, 0x9c, 0x28, 0x4f, 0x60, 0xef, 0x0b, 0xa1, 0x6a, 0x40, 0x80, 0xe0,
0x85, 0x86, 0x4a, 0x4c, 0x80, 0xe0, 0x25, 0x86, 0x78, 0x57, 0x60, 0xef, 0x96,
0x4d, 0x9c, 0xe1, 0x01, 0x81, 0x06, 0x98, 0x76, 0x0e, 0xe8, 0xe0, 0xb5, 0x81,
0x08, 0x9c, 0x4a, 0x40, 0x88, 0xe0, 0x85, 0x80, 0x08, 0x41, 0x00, 0xe8, 0x35,
0x81, 0x18, 0x41, 0x00, 0xe8, 0x08, 0x98, 0x4a, 0x00, 0xfc, 0xfb, 0x95, 0xfc,
0x38, 0x59, 0x60, 0xef, 0x2c, 0x00, 0x00, 0xe2, 0x4c, 0x0c, 0x0c, 0xf6, 0x93,
0xdd, 0x83, 0xc1, 0x13, 0xc5, 0x93, 0xdd, 0xc3, 0xc1, 0x83, 0xc1, 0x13, 0xc3,
0x93, 0xdd, 0xc3, 0xc1, 0x0d, 0xc3, 0x1a, 0x41, 0x08, 0xe4, 0x0a, 0x40, 0x84,
0xe1, 0x0c, 0x00, 0x00, 0xe2, 0x93, 0xdd, 0x4c, 0x04, 0x04, 0xfa, 0x86, 0x4e,
0xec, 0xe1, 0x08, 0x9e, 0x65, 0x0e, 0x24, 0xf8, 0x0e, 0x02, 0x99, 0x7a, 0x00,
0xc0, 0x00, 0x40, 0xf8, 0xf3, 0x06, 0x9e, 0x0b, 0x8c, 0x28, 0x57, 0x00, 0xef,
0x25, 0x0e, 0x28, 0xf8, 0x02, 0x02, 0xfc, 0xed, 0xf6, 0x49, 0xfd, 0x6f, 0xe0,
0xff, 0x04, 0xca, 0x14, 0x06, 0xc0, 0xe0, 0x0f, 0x88, 0x3f, 0xa0, 0x0b, 0x8c,
0x3e, 0xca, 0x28, 0x56, 0x00, 0xef, 0x86, 0x02, 0x84, 0xfe, 0x0e, 0x05, 0x09,
0x7d, 0x00, 0xc0, 0x05, 0x4e, 0x08, 0xf8, 0x18, 0x7d, 0xfc, 0xef, 0x4a, 0x40,
0x80, 0xe0, 0x09, 0x0e, 0x04, 0xc0, 0x00, 0x40, 0x40, 0xdc, 0x01, 0x4e, 0x04,
0xc0, 0x4c, 0x0c, 0x04, 0xf2, 0x93, 0xdd, 0xc6, 0x40, 0xfc, 0xe0, 0x04, 0x80,
0x1a, 0x80, 0x06, 0x40, 0x0c, 0xe1, 0x24, 0x80, 0x13, 0x44, 0x48, 0xe1, 0x76,
0x02, 0x40, 0xe2, 0x18, 0x84, 0x06, 0x05, 0x04, 0xe0, 0x34, 0xc2, 0x16, 0x44,
0x0a, 0xe0, 0x86, 0x03, 0x84, 0xe0, 0x23, 0x82, 0x29, 0x04, 0xd8, 0xef, 0x0a,
0x44, 0x80, 0xe0, 0x21, 0xbf, 0x06, 0x04, 0x00, 0xc0, 0x21, 0x46, 0x60, 0xe0,
0x15, 0x02, 0x3c, 0xe5, 0x06, 0x04, 0x01, 0x60, 0x00, 0xf0, 0x1f, 0x84, 0x06,
0x04, 0x01, 0x60, 0x00, 0xe0, 0x1b, 0x85, 0x05, 0x00, 0x00, 0x80, 0x19, 0x00,
0xf0, 0x83, 0x10, 0x42, 0x80, 0x93, 0x10, 0x42, 0x58, 0x9c, 0x11, 0x40, 0xf0,
0x83, 0x93, 0xdd, 0x0c, 0x00, 0x80, 0xfa, 0x15, 0x00, 0x3c, 0xe0, 0x21, 0x81,
0x31, 0x85, 0x21, 0x42, 0x60, 0xe0, 0x15, 0x00, 0x44, 0xe0, 0x31, 0x42, 0x40,
0xe1, 0x15, 0x00, 0x34, 0xe0, 0x21, 0x42, 0x20, 0xe0, 0x15, 0x00, 0x34, 0xe0,
0xd6, 0x04, 0x10, 0xe0, 0x23, 0x42, 0x30, 0xe0, 0x15, 0x00, 0x34, 0xe0, 0x86,
0x44, 0x04, 0xe0, 0x23, 0x42, 0x38, 0xe0, 0x05, 0x00, 0x30, 0xe0, 0xc6, 0x02,
0x08, 0xe0, 0x13, 0x40, 0x10, 0xe3, 0x68, 0x72, 0x40, 0xef, 0x06, 0x40, 0x0c,
0xe1, 0x04, 0x80, 0x06, 0x02, 0x94, 0xe0, 0x2b, 0x02, 0xc4, 0xea, 0x3b, 0x00,
0x78, 0xe2, 0x20, 0x44, 0xfd, 0x73, 0x07, 0xc0, 0x30, 0x46, 0x01, 0x70, 0xf8,
0xc0, 0x3f, 0xa4, 0x33, 0x40, 0x78, 0xe2, 0x0a, 0x84, 0x0c, 0x08, 0x80, 0xf2,
0x78, 0x17, 0x40, 0xff, 0xc3, 0xc1, 0x06, 0x40, 0x0c, 0xe1, 0x04, 0x80, 0x1b,
0x00, 0x40, 0xe4, 0x19, 0xc2, 0x13, 0x40, 0x40, 0xe4, 0x1b, 0x00, 0x40, 0xe4,
0x19, 0xc4, 0x13, 0x40, 0x40, 0xe4, 0x93, 0xdd, 0xc6, 0x43, 0xec, 0xe0, 0x46,
0x41, 0xfc, 0xe0, 0x24, 0x84, 0x04, 0x80, 0x31, 0x81, 0x4a, 0x44, 0x80, 0xe0,
0x86, 0x44, 0x0c, 0xe1, 0x09, 0x00, 0x6c, 0xe0, 0xc4, 0x8a, 0x8e, 0x47, 0xfc,
0x9f, 0x01, 0x42, 0x51, 0x78, 0x0c, 0xc0, 0x31, 0x58, 0x90, 0xe0, 0x34, 0x8a,
0x41, 0xbf, 0x06, 0x08, 0x00, 0xc0, 0x41, 0x46, 0xa0, 0xe0, 0x34, 0x8a, 0x51,
0x81, 0xf6, 0x0b, 0x00, 0xc0, 0x51, 0x46, 0xd0, 0xe0, 0x34, 0x8a, 0x01, 0xbf,
0x51, 0x46, 0xe0, 0xe0, 0x44, 0x84, 0x0a, 0x48, 0x84, 0xe0, 0x75, 0x86, 0x54,
0xca, 0x49, 0x88, 0x44, 0x06, 0x88, 0xe1, 0x36, 0x94, 0x4a, 0x46, 0x80, 0xe0,
0x34, 0xca, 0x47, 0xc6, 0x11, 0x8d, 0x41, 0x46, 0xd0, 0xe0, 0x34, 0x88, 0x76,
0x02, 0x00, 0xc0, 0x06, 0x00, 0x00, 0xc0, 0x16, 0x8c, 0x14, 0x88, 0x01, 0x42,
0xc0, 0xe1, 0x01, 0x42, 0xe0, 0xe1, 0x01, 0x42, 0xf0, 0xe1, 0x93, 0xdd, 0x34,
0xca, 0x41, 0x85, 0x46, 0x8c, 0x34, 0xca, 0x06, 0x48, 0x00, 0xe0, 0x41, 0x46,
0xd0, 0xe0, 0x34, 0x88, 0x41, 0x83, 0x46, 0x8c, 0x34, 0x88, 0x01, 0x46, 0xc0,
0xe1, 0x01, 0x46, 0xe0, 0xe1, 0x01, 0x46, 0xf0, 0xe1, 0x09, 0x02, 0x20, 0xe0,
0x14, 0xca, 0x03, 0x42, 0x58, 0xe0, 0x93, 0xdd, 0xc3, 0xc1, 0x4c, 0x04, 0x04,
0xfa, 0x46, 0x4e, 0x08, 0xe1, 0x06, 0x4c, 0x0c, 0xe1, 0x0a, 0x9e, 0x14, 0x98,
0x05, 0x42, 0x44, 0xe0, 0x10, 0x00, 0xe1, 0x65, 0x03, 0xc0, 0x78, 0x41, 0x00,
0xe8, 0x08, 0x9c, 0x0b, 0xa1, 0x04, 0x98, 0x06, 0x02, 0x10, 0x80, 0x13, 0x40,
0xf8, 0x86, 0x65, 0x82, 0x00, 0x00, 0xe1, 0x65, 0x03, 0xc0, 0xa8, 0x40, 0x00,
0xe8, 0x14, 0x98, 0x04, 0x00, 0xa0, 0xfc, 0x03, 0x42, 0x00, 0xe7, 0x4c, 0x0c,
0x04, 0xf2, 0x93, 0xdd, 0x0a, 0x80, 0x93, 0xdd, 0x08, 0xa9, 0x00, 0x00, 0x74,
0xff, 0x40, 0x00, 0x08, 0x05, 0x80, 0xe0, 0xa8, 0xc1, 0x40, 0x00, 0x48, 0x37,
0x9c, 0xe0, 0x40, 0x6c, 0x40, 0x00, 0xc8, 0x26, 0xc8, 0xe0, 0xfc, 0x91, 0x40,
0x00, 0xe8, 0x03, 0xb8, 0xe0, 0x30, 0x16, 0x41, 0x00, 0xa8, 0x1d, 0x74, 0xe0,
0xb0, 0x7e, 0x40, 0x00, 0x68, 0x1d, 0xc0, 0xe0, 0xf8, 0x89, 0x40, 0x00, 0x48,
0x3c, 0xb8, 0xe0, 0x00, 0x90, 0x40, 0x00, 0x88, 0x19, 0xb8, 0xe0, 0x00, 0xb9,
0x00, 0x00, 0x2a, 0x3c, 0x00, 0x00,
};
