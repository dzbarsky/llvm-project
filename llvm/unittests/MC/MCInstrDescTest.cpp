//===- MCInstrDescTest.cpp - MCInstrDesc unit tests -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCInstrDesc.h"
#include "gtest/gtest.h"

using namespace llvm;

namespace {

TEST(MCInstrDescTest, PackedFields) {
  struct OperandCounts {
    unsigned NumOperands;
    unsigned NumDefs;
  };

  constexpr OperandCounts TestCounts[] = {
      {0, 0},   {1, 0},     {1, 1},    {33, 8},    {67, 65},
      {130, 0}, {131, 129}, {255, 31}, {255, 224}, {255, 255}};
  for (OperandCounts Counts : TestCounts) {
    for (unsigned Size : {0U, 2U, 3U, 4U, 63U, 64U, 252U, 316U}) {
      MCInstrDesc Desc(65535, Counts.NumOperands, Counts.NumDefs, Size, 8191,
                       63, 63, 32767, 1023, (1ULL << MCID::Authenticated) | 1,
                       UINT64_MAX);

      EXPECT_EQ(Desc.getOpcode(), 65535U);
      EXPECT_EQ(Desc.getNumOperands(), Counts.NumOperands);
      EXPECT_EQ(Desc.getNumDefs(), Counts.NumDefs);
      EXPECT_EQ(Desc.getSize(), Size);
      EXPECT_EQ(Desc.getSchedClass(), 8191U);
      EXPECT_EQ(Desc.getNumImplicitUses(), 63U);
      EXPECT_EQ(Desc.getNumImplicitDefs(), 63U);
      EXPECT_TRUE(Desc.isPreISelOpcode());
      EXPECT_TRUE(Desc.isAuthenticated());
      EXPECT_EQ(Desc.TSFlags, UINT64_MAX);
    }
  }
}

} // namespace
