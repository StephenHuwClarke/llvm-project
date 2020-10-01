//===- AArch64LegalizerInfo.cpp ----------------------------------*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// This file implements the targeting of the Machinelegalizer class for
/// AArch64.
/// \todo This should be generated by TableGen.
//===----------------------------------------------------------------------===//

#include "AArch64LegalizerInfo.h"
#include "AArch64Subtarget.h"
#include "llvm/CodeGen/GlobalISel/LegalizerHelper.h"
#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include <initializer_list>

#define DEBUG_TYPE "aarch64-legalinfo"

using namespace llvm;
using namespace LegalizeActions;
using namespace LegalizeMutations;
using namespace LegalityPredicates;

AArch64LegalizerInfo::AArch64LegalizerInfo(const AArch64Subtarget &ST)
    : ST(&ST) {
  using namespace TargetOpcode;
  const LLT p0 = LLT::pointer(0, 64);
  const LLT s1 = LLT::scalar(1);
  const LLT s8 = LLT::scalar(8);
  const LLT s16 = LLT::scalar(16);
  const LLT s32 = LLT::scalar(32);
  const LLT s64 = LLT::scalar(64);
  const LLT s128 = LLT::scalar(128);
  const LLT s256 = LLT::scalar(256);
  const LLT s512 = LLT::scalar(512);
  const LLT v16s8 = LLT::vector(16, 8);
  const LLT v8s8 = LLT::vector(8, 8);
  const LLT v4s8 = LLT::vector(4, 8);
  const LLT v8s16 = LLT::vector(8, 16);
  const LLT v4s16 = LLT::vector(4, 16);
  const LLT v2s16 = LLT::vector(2, 16);
  const LLT v2s32 = LLT::vector(2, 32);
  const LLT v4s32 = LLT::vector(4, 32);
  const LLT v2s64 = LLT::vector(2, 64);
  const LLT v2p0 = LLT::vector(2, p0);

  std::initializer_list<LLT> PackedVectorAllTypeList = {/* Begin 128bit types */
                                                        v16s8, v8s16, v4s32,
                                                        v2s64, v2p0,
                                                        /* End 128bit types */
                                                        /* Begin 64bit types */
                                                        v8s8, v4s16, v2s32};

  const TargetMachine &TM = ST.getTargetLowering()->getTargetMachine();

  // FIXME: support subtargets which have neon/fp-armv8 disabled.
  if (!ST.hasNEON() || !ST.hasFPARMv8()) {
    computeTables();
    return;
  }

  getActionDefinitionsBuilder({G_IMPLICIT_DEF, G_FREEZE})
      .legalFor({p0, s1, s8, s16, s32, s64})
      .legalFor(PackedVectorAllTypeList)
      .clampScalar(0, s1, s64)
      .widenScalarToNextPow2(0, 8)
      .fewerElementsIf(
          [=](const LegalityQuery &Query) {
            return Query.Types[0].isVector() &&
                   (Query.Types[0].getElementType() != s64 ||
                    Query.Types[0].getNumElements() != 2);
          },
          [=](const LegalityQuery &Query) {
            LLT EltTy = Query.Types[0].getElementType();
            if (EltTy == s64)
              return std::make_pair(0, LLT::vector(2, 64));
            return std::make_pair(0, EltTy);
          });

  getActionDefinitionsBuilder(G_PHI).legalFor({p0, s16, s32, s64})
      .legalFor(PackedVectorAllTypeList)
      .clampScalar(0, s16, s64)
      .widenScalarToNextPow2(0);

  getActionDefinitionsBuilder(G_BSWAP)
      .legalFor({s32, s64, v4s32, v2s32, v2s64})
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0);

  getActionDefinitionsBuilder({G_ADD, G_SUB, G_MUL, G_AND, G_OR, G_XOR})
      .legalFor({s32, s64, v2s32, v4s32, v4s16, v8s16, v16s8})
      .scalarizeIf(
          [=](const LegalityQuery &Query) {
            return Query.Opcode == G_MUL && Query.Types[0] == v2s64;
          },
          0)
      .legalFor({v2s64})
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0)
      .clampNumElements(0, v2s32, v4s32)
      .clampNumElements(0, v2s64, v2s64)
      .moreElementsToNextPow2(0);

  getActionDefinitionsBuilder(G_SHL)
      .customIf([=](const LegalityQuery &Query) {
        const auto &SrcTy = Query.Types[0];
        const auto &AmtTy = Query.Types[1];
        return !SrcTy.isVector() && SrcTy.getSizeInBits() == 32 &&
               AmtTy.getSizeInBits() == 32;
      })
      .legalFor({
          {s32, s32},
          {s32, s64},
          {s64, s64},
          {v16s8, v16s8},
          {v4s16, v4s16},
          {v8s16, v8s16},
          {v2s32, v2s32},
          {v4s32, v4s32},
          {v2s64, v2s64},

      })
      .clampScalar(1, s32, s64)
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0)
      .clampNumElements(0, v2s32, v4s32)
      .clampNumElements(0, v2s64, v2s64)
      .moreElementsToNextPow2(0)
      .minScalarSameAs(1, 0);

  getActionDefinitionsBuilder(G_PTR_ADD)
      .legalFor({{p0, s64}, {v2p0, v2s64}})
      .clampScalar(1, s64, s64);

  getActionDefinitionsBuilder(G_PTRMASK).legalFor({{p0, s64}});

  getActionDefinitionsBuilder({G_SDIV, G_UDIV})
      .legalFor({s32, s64})
      .libcallFor({s128})
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0)
      .scalarize(0);

  getActionDefinitionsBuilder({G_LSHR, G_ASHR})
      .customIf([=](const LegalityQuery &Query) {
        const auto &SrcTy = Query.Types[0];
        const auto &AmtTy = Query.Types[1];
        return !SrcTy.isVector() && SrcTy.getSizeInBits() == 32 &&
               AmtTy.getSizeInBits() == 32;
      })
      .legalFor({{s32, s32},
                 {s32, s64},
                 {s64, s64},
                 {v2s32, v2s32},
                 {v4s32, v4s32},
                 {v2s64, v2s64},
                 {v16s8, v16s8},
                 {v4s16, v4s16},
                 {v8s16, v8s16}})
      .clampScalar(1, s32, s64)
      .clampScalar(0, s32, s64)
      .minScalarSameAs(1, 0);

  getActionDefinitionsBuilder({G_SREM, G_UREM})
      .lowerFor({s1, s8, s16, s32, s64});

  getActionDefinitionsBuilder({G_SMULO, G_UMULO}).lowerFor({{s64, s1}});

  getActionDefinitionsBuilder({G_SMULH, G_UMULH}).legalFor({s32, s64});

  getActionDefinitionsBuilder({G_UADDE, G_USUBE, G_SADDO, G_SSUBO, G_UADDO})
      .legalFor({{s32, s1}, {s64, s1}})
      .minScalar(0, s32);

  getActionDefinitionsBuilder({G_FADD, G_FSUB, G_FMUL, G_FDIV, G_FNEG})
      .legalFor({s32, s64, v2s64, v4s32, v2s32})
      .clampNumElements(0, v2s32, v4s32)
      .clampNumElements(0, v2s64, v2s64);

  getActionDefinitionsBuilder(G_FREM).libcallFor({s32, s64});

  getActionDefinitionsBuilder({G_FCEIL, G_FABS, G_FSQRT, G_FFLOOR, G_FRINT,
                               G_FMA, G_INTRINSIC_TRUNC, G_INTRINSIC_ROUND,
                               G_FNEARBYINT, G_INTRINSIC_LRINT})
      // If we don't have full FP16 support, then scalarize the elements of
      // vectors containing fp16 types.
      .fewerElementsIf(
          [=, &ST](const LegalityQuery &Query) {
            const auto &Ty = Query.Types[0];
            return Ty.isVector() && Ty.getElementType() == s16 &&
                   !ST.hasFullFP16();
          },
          [=](const LegalityQuery &Query) { return std::make_pair(0, s16); })
      // If we don't have full FP16 support, then widen s16 to s32 if we
      // encounter it.
      .widenScalarIf(
          [=, &ST](const LegalityQuery &Query) {
            return Query.Types[0] == s16 && !ST.hasFullFP16();
          },
          [=](const LegalityQuery &Query) { return std::make_pair(0, s32); })
      .legalFor({s16, s32, s64, v2s32, v4s32, v2s64, v2s16, v4s16, v8s16});

  getActionDefinitionsBuilder(
      {G_FCOS, G_FSIN, G_FLOG10, G_FLOG, G_FLOG2, G_FEXP, G_FEXP2, G_FPOW})
      // We need a call for these, so we always need to scalarize.
      .scalarize(0)
      // Regardless of FP16 support, widen 16-bit elements to 32-bits.
      .minScalar(0, s32)
      .libcallFor({s32, s64, v2s32, v4s32, v2s64});

  getActionDefinitionsBuilder(G_INSERT)
      .unsupportedIf([=](const LegalityQuery &Query) {
        return Query.Types[0].getSizeInBits() <= Query.Types[1].getSizeInBits();
      })
      .legalIf([=](const LegalityQuery &Query) {
        const LLT &Ty0 = Query.Types[0];
        const LLT &Ty1 = Query.Types[1];
        if (Ty0 != s32 && Ty0 != s64 && Ty0 != p0)
          return false;
        return isPowerOf2_32(Ty1.getSizeInBits()) &&
               (Ty1.getSizeInBits() == 1 || Ty1.getSizeInBits() >= 8);
      })
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0)
      .maxScalarIf(typeInSet(0, {s32}), 1, s16)
      .maxScalarIf(typeInSet(0, {s64}), 1, s32)
      .widenScalarToNextPow2(1);

  getActionDefinitionsBuilder(G_EXTRACT)
      .unsupportedIf([=](const LegalityQuery &Query) {
        return Query.Types[0].getSizeInBits() >= Query.Types[1].getSizeInBits();
      })
      .legalIf([=](const LegalityQuery &Query) {
        const LLT &Ty0 = Query.Types[0];
        const LLT &Ty1 = Query.Types[1];
        if (Ty1 != s32 && Ty1 != s64 && Ty1 != s128)
          return false;
        if (Ty1 == p0)
          return true;
        return isPowerOf2_32(Ty0.getSizeInBits()) &&
               (Ty0.getSizeInBits() == 1 || Ty0.getSizeInBits() >= 8);
      })
      .clampScalar(1, s32, s128)
      .widenScalarToNextPow2(1)
      .maxScalarIf(typeInSet(1, {s32}), 0, s16)
      .maxScalarIf(typeInSet(1, {s64}), 0, s32)
      .widenScalarToNextPow2(0);

  getActionDefinitionsBuilder({G_SEXTLOAD, G_ZEXTLOAD})
      .legalForTypesWithMemDesc({{s32, p0, 8, 8},
                                 {s32, p0, 16, 8},
                                 {s32, p0, 32, 8},
                                 {s64, p0, 8, 2},
                                 {s64, p0, 16, 2},
                                 {s64, p0, 32, 4},
                                 {s64, p0, 64, 8},
                                 {p0, p0, 64, 8},
                                 {v2s32, p0, 64, 8}})
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0)
      // TODO: We could support sum-of-pow2's but the lowering code doesn't know
      //       how to do that yet.
      .unsupportedIfMemSizeNotPow2()
      // Lower anything left over into G_*EXT and G_LOAD
      .lower();

  auto IsPtrVecPred = [=](const LegalityQuery &Query) {
    const LLT &ValTy = Query.Types[0];
    if (!ValTy.isVector())
      return false;
    const LLT EltTy = ValTy.getElementType();
    return EltTy.isPointer() && EltTy.getAddressSpace() == 0;
  };

  getActionDefinitionsBuilder(G_LOAD)
      .legalForTypesWithMemDesc({{s8, p0, 8, 8},
                                 {s16, p0, 16, 8},
                                 {s32, p0, 32, 8},
                                 {s64, p0, 64, 8},
                                 {p0, p0, 64, 8},
                                 {s128, p0, 128, 8},
                                 {v8s8, p0, 64, 8},
                                 {v16s8, p0, 128, 8},
                                 {v4s16, p0, 64, 8},
                                 {v8s16, p0, 128, 8},
                                 {v2s32, p0, 64, 8},
                                 {v4s32, p0, 128, 8},
                                 {v2s64, p0, 128, 8}})
      // These extends are also legal
      .legalForTypesWithMemDesc({{s32, p0, 8, 8}, {s32, p0, 16, 8}})
      .clampScalar(0, s8, s64)
      .lowerIfMemSizeNotPow2()
      // Lower any any-extending loads left into G_ANYEXT and G_LOAD
      .lowerIf([=](const LegalityQuery &Query) {
        return Query.Types[0].getSizeInBits() != Query.MMODescrs[0].SizeInBits;
      })
      .widenScalarToNextPow2(0)
      .clampMaxNumElements(0, s32, 2)
      .clampMaxNumElements(0, s64, 1)
      .customIf(IsPtrVecPred);

  getActionDefinitionsBuilder(G_STORE)
      .legalForTypesWithMemDesc({{s8, p0, 8, 8},
                                 {s16, p0, 16, 8},
                                 {s32, p0, 8, 8},
                                 {s32, p0, 16, 8},
                                 {s32, p0, 32, 8},
                                 {s64, p0, 64, 8},
                                 {p0, p0, 64, 8},
                                 {s128, p0, 128, 8},
                                 {v16s8, p0, 128, 8},
                                 {v8s8, p0, 64, 8},
                                 {v4s16, p0, 64, 8},
                                 {v8s16, p0, 128, 8},
                                 {v2s32, p0, 64, 8},
                                 {v4s32, p0, 128, 8},
                                 {v2s64, p0, 128, 8}})
      .clampScalar(0, s8, s64)
      .lowerIfMemSizeNotPow2()
      .lowerIf([=](const LegalityQuery &Query) {
        return Query.Types[0].isScalar() &&
               Query.Types[0].getSizeInBits() != Query.MMODescrs[0].SizeInBits;
      })
      .clampMaxNumElements(0, s32, 2)
      .clampMaxNumElements(0, s64, 1)
      .customIf(IsPtrVecPred);

  // Constants
  getActionDefinitionsBuilder(G_CONSTANT)
      .legalFor({p0, s8, s16, s32, s64})
      .clampScalar(0, s8, s64)
      .widenScalarToNextPow2(0);
  getActionDefinitionsBuilder(G_FCONSTANT)
      .legalFor({s32, s64})
      .clampScalar(0, s32, s64);

  getActionDefinitionsBuilder(G_ICMP)
      .legalFor({{s32, s32},
                 {s32, s64},
                 {s32, p0},
                 {v4s32, v4s32},
                 {v2s32, v2s32},
                 {v2s64, v2s64},
                 {v2s64, v2p0},
                 {v4s16, v4s16},
                 {v8s16, v8s16},
                 {v8s8, v8s8},
                 {v16s8, v16s8}})
      .clampScalar(1, s32, s64)
      .clampScalar(0, s32, s32)
      .minScalarEltSameAsIf(
          [=](const LegalityQuery &Query) {
            const LLT &Ty = Query.Types[0];
            const LLT &SrcTy = Query.Types[1];
            return Ty.isVector() && !SrcTy.getElementType().isPointer() &&
                   Ty.getElementType() != SrcTy.getElementType();
          },
          0, 1)
      .minScalarOrEltIf(
          [=](const LegalityQuery &Query) { return Query.Types[1] == v2s16; },
          1, s32)
      .minScalarOrEltIf(
          [=](const LegalityQuery &Query) { return Query.Types[1] == v2p0; }, 0,
          s64)
      .widenScalarOrEltToNextPow2(1);

  getActionDefinitionsBuilder(G_FCMP)
      .legalFor({{s32, s32}, {s32, s64}})
      .clampScalar(0, s32, s32)
      .clampScalar(1, s32, s64)
      .widenScalarToNextPow2(1);

  // Extensions
  auto ExtLegalFunc = [=](const LegalityQuery &Query) {
    unsigned DstSize = Query.Types[0].getSizeInBits();

    if (DstSize == 128 && !Query.Types[0].isVector())
      return false; // Extending to a scalar s128 needs narrowing.

    // Make sure that we have something that will fit in a register, and
    // make sure it's a power of 2.
    if (DstSize < 8 || DstSize > 128 || !isPowerOf2_32(DstSize))
      return false;

    const LLT &SrcTy = Query.Types[1];

    // Special case for s1.
    if (SrcTy == s1)
      return true;

    // Make sure we fit in a register otherwise. Don't bother checking that
    // the source type is below 128 bits. We shouldn't be allowing anything
    // through which is wider than the destination in the first place.
    unsigned SrcSize = SrcTy.getSizeInBits();
    if (SrcSize < 8 || !isPowerOf2_32(SrcSize))
      return false;

    return true;
  };
  getActionDefinitionsBuilder({G_ZEXT, G_SEXT, G_ANYEXT})
      .legalIf(ExtLegalFunc)
      .clampScalar(0, s64, s64); // Just for s128, others are handled above.

  getActionDefinitionsBuilder(G_TRUNC).alwaysLegal();

  getActionDefinitionsBuilder(G_SEXT_INREG).legalFor({s32, s64}).lower();

  // FP conversions
  getActionDefinitionsBuilder(G_FPTRUNC)
      .legalFor(
          {{s16, s32}, {s16, s64}, {s32, s64}, {v4s16, v4s32}, {v2s32, v2s64}})
      .clampMaxNumElements(0, s32, 2);
  getActionDefinitionsBuilder(G_FPEXT).legalFor(
      {{s32, s16}, {s64, s16}, {s64, s32}, {v4s32, v4s16}, {v2s64, v2s32}});

  // Conversions
  getActionDefinitionsBuilder({G_FPTOSI, G_FPTOUI})
      .legalForCartesianProduct({s32, s64, v2s64, v4s32, v2s32})
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0)
      .clampScalar(1, s32, s64)
      .widenScalarToNextPow2(1);

  getActionDefinitionsBuilder({G_SITOFP, G_UITOFP})
      .legalForCartesianProduct({s32, s64, v2s64, v4s32, v2s32})
      .clampScalar(1, s32, s64)
      .minScalarSameAs(1, 0)
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0);

  // Control-flow
  getActionDefinitionsBuilder(G_BRCOND).legalFor({s1, s8, s16, s32});
  getActionDefinitionsBuilder(G_BRINDIRECT).legalFor({p0});

  getActionDefinitionsBuilder(G_SELECT)
      .legalFor({{s32, s1}, {s64, s1}, {p0, s1}})
      .clampScalar(0, s32, s64)
      .widenScalarToNextPow2(0)
      .minScalarEltSameAsIf(isVector(0), 1, 0)
      .lowerIf(isVector(0));

  // Pointer-handling
  getActionDefinitionsBuilder(G_FRAME_INDEX).legalFor({p0});

  if (TM.getCodeModel() == CodeModel::Small)
    getActionDefinitionsBuilder(G_GLOBAL_VALUE).custom();
  else
    getActionDefinitionsBuilder(G_GLOBAL_VALUE).legalFor({p0});

  getActionDefinitionsBuilder(G_PTRTOINT)
      .legalForCartesianProduct({s1, s8, s16, s32, s64}, {p0})
      .maxScalar(0, s64)
      .widenScalarToNextPow2(0, /*Min*/ 8);

  getActionDefinitionsBuilder(G_INTTOPTR)
      .unsupportedIf([&](const LegalityQuery &Query) {
        return Query.Types[0].getSizeInBits() != Query.Types[1].getSizeInBits();
      })
      .legalFor({{p0, s64}});

  // Casts for 32 and 64-bit width type are just copies.
  // Same for 128-bit width type, except they are on the FPR bank.
  getActionDefinitionsBuilder(G_BITCAST)
      // FIXME: This is wrong since G_BITCAST is not allowed to change the
      // number of bits but it's what the previous code described and fixing
      // it breaks tests.
      .legalForCartesianProduct({s1, s8, s16, s32, s64, s128, v16s8, v8s8, v4s8,
                                 v8s16, v4s16, v2s16, v4s32, v2s32, v2s64,
                                 v2p0});

  getActionDefinitionsBuilder(G_VASTART).legalFor({p0});

  // va_list must be a pointer, but most sized types are pretty easy to handle
  // as the destination.
  getActionDefinitionsBuilder(G_VAARG)
      .customForCartesianProduct({s8, s16, s32, s64, p0}, {p0})
      .clampScalar(0, s8, s64)
      .widenScalarToNextPow2(0, /*Min*/ 8);

  if (ST.hasLSE()) {
    getActionDefinitionsBuilder(G_ATOMIC_CMPXCHG_WITH_SUCCESS)
        .lowerIf(all(
            typeInSet(0, {s8, s16, s32, s64}), typeIs(1, s1), typeIs(2, p0),
            atomicOrderingAtLeastOrStrongerThan(0, AtomicOrdering::Monotonic)));

    getActionDefinitionsBuilder(
        {G_ATOMICRMW_XCHG, G_ATOMICRMW_ADD, G_ATOMICRMW_SUB, G_ATOMICRMW_AND,
         G_ATOMICRMW_OR, G_ATOMICRMW_XOR, G_ATOMICRMW_MIN, G_ATOMICRMW_MAX,
         G_ATOMICRMW_UMIN, G_ATOMICRMW_UMAX, G_ATOMIC_CMPXCHG})
        .legalIf(all(
            typeInSet(0, {s8, s16, s32, s64}), typeIs(1, p0),
            atomicOrderingAtLeastOrStrongerThan(0, AtomicOrdering::Monotonic)));
  }

  getActionDefinitionsBuilder(G_BLOCK_ADDR).legalFor({p0});

  // Merge/Unmerge
  for (unsigned Op : {G_MERGE_VALUES, G_UNMERGE_VALUES}) {
    unsigned BigTyIdx = Op == G_MERGE_VALUES ? 0 : 1;
    unsigned LitTyIdx = Op == G_MERGE_VALUES ? 1 : 0;

    auto notValidElt = [](const LegalityQuery &Query, unsigned TypeIdx) {
      const LLT &Ty = Query.Types[TypeIdx];
      if (Ty.isVector()) {
        const LLT &EltTy = Ty.getElementType();
        if (EltTy.getSizeInBits() < 8 || EltTy.getSizeInBits() > 64)
          return true;
        if (!isPowerOf2_32(EltTy.getSizeInBits()))
          return true;
      }
      return false;
    };

    // FIXME: This rule is horrible, but specifies the same as what we had
    // before with the particularly strange definitions removed (e.g.
    // s8 = G_MERGE_VALUES s32, s32).
    // Part of the complexity comes from these ops being extremely flexible. For
    // example, you can build/decompose vectors with it, concatenate vectors,
    // etc. and in addition to this you can also bitcast with it at the same
    // time. We've been considering breaking it up into multiple ops to make it
    // more manageable throughout the backend.
    getActionDefinitionsBuilder(Op)
        // Break up vectors with weird elements into scalars
        .fewerElementsIf(
            [=](const LegalityQuery &Query) { return notValidElt(Query, 0); },
            scalarize(0))
        .fewerElementsIf(
            [=](const LegalityQuery &Query) { return notValidElt(Query, 1); },
            scalarize(1))
        // Clamp the big scalar to s8-s512 and make it either a power of 2, 192,
        // or 384.
        .clampScalar(BigTyIdx, s8, s512)
        .widenScalarIf(
            [=](const LegalityQuery &Query) {
              const LLT &Ty = Query.Types[BigTyIdx];
              return !isPowerOf2_32(Ty.getSizeInBits()) &&
                     Ty.getSizeInBits() % 64 != 0;
            },
            [=](const LegalityQuery &Query) {
              // Pick the next power of 2, or a multiple of 64 over 128.
              // Whichever is smaller.
              const LLT &Ty = Query.Types[BigTyIdx];
              unsigned NewSizeInBits = 1
                                       << Log2_32_Ceil(Ty.getSizeInBits() + 1);
              if (NewSizeInBits >= 256) {
                unsigned RoundedTo = alignTo<64>(Ty.getSizeInBits() + 1);
                if (RoundedTo < NewSizeInBits)
                  NewSizeInBits = RoundedTo;
              }
              return std::make_pair(BigTyIdx, LLT::scalar(NewSizeInBits));
            })
        // Clamp the little scalar to s8-s256 and make it a power of 2. It's not
        // worth considering the multiples of 64 since 2*192 and 2*384 are not
        // valid.
        .clampScalar(LitTyIdx, s8, s256)
        .widenScalarToNextPow2(LitTyIdx, /*Min*/ 8)
        // So at this point, we have s8, s16, s32, s64, s128, s192, s256, s384,
        // s512, <X x s8>, <X x s16>, <X x s32>, or <X x s64>.
        // At this point it's simple enough to accept the legal types.
        .legalIf([=](const LegalityQuery &Query) {
          const LLT &BigTy = Query.Types[BigTyIdx];
          const LLT &LitTy = Query.Types[LitTyIdx];
          if (BigTy.isVector() && BigTy.getSizeInBits() < 32)
            return false;
          if (LitTy.isVector() && LitTy.getSizeInBits() < 32)
            return false;
          return BigTy.getSizeInBits() % LitTy.getSizeInBits() == 0;
        })
        // Any vectors left are the wrong size. Scalarize them.
        .scalarize(0)
        .scalarize(1);
  }

  getActionDefinitionsBuilder(G_EXTRACT_VECTOR_ELT)
      .unsupportedIf([=](const LegalityQuery &Query) {
        const LLT &EltTy = Query.Types[1].getElementType();
        return Query.Types[0] != EltTy;
      })
      .minScalar(2, s64)
      .legalIf([=](const LegalityQuery &Query) {
        const LLT &VecTy = Query.Types[1];
        return VecTy == v2s16 || VecTy == v4s16 || VecTy == v8s16 ||
               VecTy == v4s32 || VecTy == v2s64 || VecTy == v2s32 ||
               VecTy == v16s8 || VecTy == v2s32;
      })
      .minScalarOrEltIf(
          [=](const LegalityQuery &Query) {
            // We want to promote to <M x s1> to <M x s64> if that wouldn't
            // cause the total vec size to be > 128b.
            return Query.Types[1].getNumElements() <= 2;
          },
          0, s64)
      .minScalarOrEltIf(
          [=](const LegalityQuery &Query) {
            return Query.Types[1].getNumElements() <= 4;
          },
          0, s32)
      .minScalarOrEltIf(
          [=](const LegalityQuery &Query) {
            return Query.Types[1].getNumElements() <= 8;
          },
          0, s16)
      .minScalarOrEltIf(
          [=](const LegalityQuery &Query) {
            return Query.Types[1].getNumElements() <= 16;
          },
          0, s8)
      .minScalarOrElt(0, s8); // Worst case, we need at least s8.

  getActionDefinitionsBuilder(G_INSERT_VECTOR_ELT)
      .legalIf(typeInSet(0, {v8s16, v2s32, v4s32, v2s64}));

  getActionDefinitionsBuilder(G_BUILD_VECTOR)
      .legalFor({{v8s8, s8},
                 {v16s8, s8},
                 {v4s16, s16},
                 {v8s16, s16},
                 {v2s32, s32},
                 {v4s32, s32},
                 {v2p0, p0},
                 {v2s64, s64}})
      .clampNumElements(0, v4s32, v4s32)
      .clampNumElements(0, v2s64, v2s64)

      // Deal with larger scalar types, which will be implicitly truncated.
      .legalIf([=](const LegalityQuery &Query) {
        return Query.Types[0].getScalarSizeInBits() <
               Query.Types[1].getSizeInBits();
      })
      .minScalarSameAs(1, 0);

  getActionDefinitionsBuilder(G_CTLZ)
      .legalForCartesianProduct(
          {s32, s64, v8s8, v16s8, v4s16, v8s16, v2s32, v4s32})
      .scalarize(1);

  getActionDefinitionsBuilder(G_SHUFFLE_VECTOR)
      .legalIf([=](const LegalityQuery &Query) {
        const LLT &DstTy = Query.Types[0];
        const LLT &SrcTy = Query.Types[1];
        // For now just support the TBL2 variant which needs the source vectors
        // to be the same size as the dest.
        if (DstTy != SrcTy)
          return false;
        for (auto &Ty : {v2s32, v4s32, v2s64, v16s8, v8s16}) {
          if (DstTy == Ty)
            return true;
        }
        return false;
      })
      // G_SHUFFLE_VECTOR can have scalar sources (from 1 x s vectors), we
      // just want those lowered into G_BUILD_VECTOR
      .lowerIf([=](const LegalityQuery &Query) {
        return !Query.Types[1].isVector();
      })
      .clampNumElements(0, v4s32, v4s32)
      .clampNumElements(0, v2s64, v2s64);

  getActionDefinitionsBuilder(G_CONCAT_VECTORS)
      .legalFor({{v4s32, v2s32}, {v8s16, v4s16}});

  getActionDefinitionsBuilder(G_JUMP_TABLE).legalFor({{p0}, {s64}});

  getActionDefinitionsBuilder(G_BRJT).legalIf([=](const LegalityQuery &Query) {
    return Query.Types[0] == p0 && Query.Types[1] == s64;
  });

  getActionDefinitionsBuilder(G_DYN_STACKALLOC).lower();

  getActionDefinitionsBuilder({G_MEMCPY, G_MEMMOVE, G_MEMSET}).libcall();

  getActionDefinitionsBuilder(G_ABS).lowerIf(
      [=](const LegalityQuery &Query) { return Query.Types[0].isScalar(); });

  computeTables();
  verify(*ST.getInstrInfo());
}

bool AArch64LegalizerInfo::legalizeCustom(LegalizerHelper &Helper,
                                          MachineInstr &MI) const {
  MachineIRBuilder &MIRBuilder = Helper.MIRBuilder;
  MachineRegisterInfo &MRI = *MIRBuilder.getMRI();
  GISelChangeObserver &Observer = Helper.Observer;
  switch (MI.getOpcode()) {
  default:
    // No idea what to do.
    return false;
  case TargetOpcode::G_VAARG:
    return legalizeVaArg(MI, MRI, MIRBuilder);
  case TargetOpcode::G_LOAD:
  case TargetOpcode::G_STORE:
    return legalizeLoadStore(MI, MRI, MIRBuilder, Observer);
  case TargetOpcode::G_SHL:
  case TargetOpcode::G_ASHR:
  case TargetOpcode::G_LSHR:
    return legalizeShlAshrLshr(MI, MRI, MIRBuilder, Observer);
  case TargetOpcode::G_GLOBAL_VALUE:
    return legalizeSmallCMGlobalValue(MI, MRI, MIRBuilder, Observer);
  }

  llvm_unreachable("expected switch to return");
}

bool AArch64LegalizerInfo::legalizeSmallCMGlobalValue(
    MachineInstr &MI, MachineRegisterInfo &MRI, MachineIRBuilder &MIRBuilder,
    GISelChangeObserver &Observer) const {
  assert(MI.getOpcode() == TargetOpcode::G_GLOBAL_VALUE);
  // We do this custom legalization to convert G_GLOBAL_VALUE into target ADRP +
  // G_ADD_LOW instructions.
  // By splitting this here, we can optimize accesses in the small code model by
  // folding in the G_ADD_LOW into the load/store offset.
  auto GV = MI.getOperand(1).getGlobal();
  if (GV->isThreadLocal())
    return true; // Don't want to modify TLS vars.

  auto &TM = ST->getTargetLowering()->getTargetMachine();
  unsigned OpFlags = ST->ClassifyGlobalReference(GV, TM);

  if (OpFlags & AArch64II::MO_GOT)
    return true;

  Register DstReg = MI.getOperand(0).getReg();
  auto ADRP = MIRBuilder.buildInstr(AArch64::ADRP, {LLT::pointer(0, 64)}, {})
                  .addGlobalAddress(GV, 0, OpFlags | AArch64II::MO_PAGE);
  // Set the regclass on the dest reg too.
  MRI.setRegClass(ADRP.getReg(0), &AArch64::GPR64RegClass);

  // MO_TAGGED on the page indicates a tagged address. Set the tag now. We do so
  // by creating a MOVK that sets bits 48-63 of the register to (global address
  // + 0x100000000 - PC) >> 48. The additional 0x100000000 offset here is to
  // prevent an incorrect tag being generated during relocation when the the
  // global appears before the code section. Without the offset, a global at
  // `0x0f00'0000'0000'1000` (i.e. at `0x1000` with tag `0xf`) that's referenced
  // by code at `0x2000` would result in `0x0f00'0000'0000'1000 - 0x2000 =
  // 0x0eff'ffff'ffff'f000`, meaning the tag would be incorrectly set to `0xe`
  // instead of `0xf`.
  // This assumes that we're in the small code model so we can assume a binary
  // size of <= 4GB, which makes the untagged PC relative offset positive. The
  // binary must also be loaded into address range [0, 2^48). Both of these
  // properties need to be ensured at runtime when using tagged addresses.
  if (OpFlags & AArch64II::MO_TAGGED) {
    ADRP = MIRBuilder.buildInstr(AArch64::MOVKXi, {LLT::pointer(0, 64)}, {ADRP})
               .addGlobalAddress(GV, 0x100000000,
                                 AArch64II::MO_PREL | AArch64II::MO_G3)
               .addImm(48);
    MRI.setRegClass(ADRP.getReg(0), &AArch64::GPR64RegClass);
  }

  MIRBuilder.buildInstr(AArch64::G_ADD_LOW, {DstReg}, {ADRP})
      .addGlobalAddress(GV, 0,
                        OpFlags | AArch64II::MO_PAGEOFF | AArch64II::MO_NC);
  MI.eraseFromParent();
  return true;
}

bool AArch64LegalizerInfo::legalizeIntrinsic(LegalizerHelper &Helper,
                                             MachineInstr &MI) const {
  return true;
}

bool AArch64LegalizerInfo::legalizeShlAshrLshr(
    MachineInstr &MI, MachineRegisterInfo &MRI, MachineIRBuilder &MIRBuilder,
    GISelChangeObserver &Observer) const {
  assert(MI.getOpcode() == TargetOpcode::G_ASHR ||
         MI.getOpcode() == TargetOpcode::G_LSHR ||
         MI.getOpcode() == TargetOpcode::G_SHL);
  // If the shift amount is a G_CONSTANT, promote it to a 64 bit type so the
  // imported patterns can select it later. Either way, it will be legal.
  Register AmtReg = MI.getOperand(2).getReg();
  auto VRegAndVal = getConstantVRegValWithLookThrough(AmtReg, MRI);
  if (!VRegAndVal)
    return true;
  // Check the shift amount is in range for an immediate form.
  int64_t Amount = VRegAndVal->Value;
  if (Amount > 31)
    return true; // This will have to remain a register variant.
  auto ExtCst = MIRBuilder.buildConstant(LLT::scalar(64), Amount);
  MI.getOperand(2).setReg(ExtCst.getReg(0));
  return true;
}

bool AArch64LegalizerInfo::legalizeLoadStore(
    MachineInstr &MI, MachineRegisterInfo &MRI, MachineIRBuilder &MIRBuilder,
    GISelChangeObserver &Observer) const {
  assert(MI.getOpcode() == TargetOpcode::G_STORE ||
         MI.getOpcode() == TargetOpcode::G_LOAD);
  // Here we just try to handle vector loads/stores where our value type might
  // have pointer elements, which the SelectionDAG importer can't handle. To
  // allow the existing patterns for s64 to fire for p0, we just try to bitcast
  // the value to use s64 types.

  // Custom legalization requires the instruction, if not deleted, must be fully
  // legalized. In order to allow further legalization of the inst, we create
  // a new instruction and erase the existing one.

  Register ValReg = MI.getOperand(0).getReg();
  const LLT ValTy = MRI.getType(ValReg);

  if (!ValTy.isVector() || !ValTy.getElementType().isPointer() ||
      ValTy.getElementType().getAddressSpace() != 0) {
    LLVM_DEBUG(dbgs() << "Tried to do custom legalization on wrong load/store");
    return false;
  }

  unsigned PtrSize = ValTy.getElementType().getSizeInBits();
  const LLT NewTy = LLT::vector(ValTy.getNumElements(), PtrSize);
  auto &MMO = **MI.memoperands_begin();
  if (MI.getOpcode() == TargetOpcode::G_STORE) {
    auto Bitcast = MIRBuilder.buildBitcast(NewTy, ValReg);
    MIRBuilder.buildStore(Bitcast.getReg(0), MI.getOperand(1), MMO);
  } else {
    auto NewLoad = MIRBuilder.buildLoad(NewTy, MI.getOperand(1), MMO);
    MIRBuilder.buildBitcast(ValReg, NewLoad);
  }
  MI.eraseFromParent();
  return true;
}

bool AArch64LegalizerInfo::legalizeVaArg(MachineInstr &MI,
                                         MachineRegisterInfo &MRI,
                                         MachineIRBuilder &MIRBuilder) const {
  MachineFunction &MF = MIRBuilder.getMF();
  Align Alignment(MI.getOperand(2).getImm());
  Register Dst = MI.getOperand(0).getReg();
  Register ListPtr = MI.getOperand(1).getReg();

  LLT PtrTy = MRI.getType(ListPtr);
  LLT IntPtrTy = LLT::scalar(PtrTy.getSizeInBits());

  const unsigned PtrSize = PtrTy.getSizeInBits() / 8;
  const Align PtrAlign = Align(PtrSize);
  auto List = MIRBuilder.buildLoad(
      PtrTy, ListPtr,
      *MF.getMachineMemOperand(MachinePointerInfo(), MachineMemOperand::MOLoad,
                               PtrSize, PtrAlign));

  MachineInstrBuilder DstPtr;
  if (Alignment > PtrAlign) {
    // Realign the list to the actual required alignment.
    auto AlignMinus1 =
        MIRBuilder.buildConstant(IntPtrTy, Alignment.value() - 1);
    auto ListTmp = MIRBuilder.buildPtrAdd(PtrTy, List, AlignMinus1.getReg(0));
    DstPtr = MIRBuilder.buildMaskLowPtrBits(PtrTy, ListTmp, Log2(Alignment));
  } else
    DstPtr = List;

  uint64_t ValSize = MRI.getType(Dst).getSizeInBits() / 8;
  MIRBuilder.buildLoad(
      Dst, DstPtr,
      *MF.getMachineMemOperand(MachinePointerInfo(), MachineMemOperand::MOLoad,
                               ValSize, std::max(Alignment, PtrAlign)));

  auto Size = MIRBuilder.buildConstant(IntPtrTy, alignTo(ValSize, PtrAlign));

  auto NewList = MIRBuilder.buildPtrAdd(PtrTy, DstPtr, Size.getReg(0));

  MIRBuilder.buildStore(NewList, ListPtr,
                        *MF.getMachineMemOperand(MachinePointerInfo(),
                                                 MachineMemOperand::MOStore,
                                                 PtrSize, PtrAlign));

  MI.eraseFromParent();
  return true;
}
