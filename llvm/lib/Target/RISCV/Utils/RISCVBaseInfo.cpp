#include "RISCVBaseInfo.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
namespace RISCVSysReg {
#define GET_SysRegsList_IMPL
#include "RISCVGenSystemOperands.inc"
} // namespace RISCVSysReg

namespace RISCVSpecialCapReg {
#define GET_SpecialCapRegsList_IMPL
#include "RISCVGenSystemOperands.inc"
} // namespace RISCVSpecialCapReg

namespace RISCVABI {
ABI computeTargetABI(const Triple &TT, FeatureBitset FeatureBits,
                     StringRef ABIName) {
  auto TargetABI = StringSwitch<ABI>(ABIName)
                       .Case("ilp32", ABI_ILP32)
                       .Case("ilp32f", ABI_ILP32F)
                       .Case("ilp32d", ABI_ILP32D)
                       .Case("ilp32e", ABI_ILP32E)
                       .Case("il32pc64", ABI_IL32PC64)
                       .Case("il32pc64f", ABI_IL32PC64F)
                       .Case("il32pc64d", ABI_IL32PC64D)
                       .Case("il32pc64e", ABI_IL32PC64E)
                       .Case("lp64", ABI_LP64)
                       .Case("lp64f", ABI_LP64F)
                       .Case("lp64d", ABI_LP64D)
                       .Case("l64pc128", ABI_L64PC128)
                       .Case("l64pc128f", ABI_L64PC128F)
                       .Case("l64pc128d", ABI_L64PC128D)
                       .Default(ABI_Unknown);

  bool IsRV64 = TT.isArch64Bit();
  bool IsRV32E = FeatureBits[RISCV::FeatureRV32E];

  if (!ABIName.empty() && TargetABI == ABI_Unknown) {
    errs()
        << "'" << ABIName
        << "' is not a recognized ABI for this target (ignoring target-abi)\n";
  } else if ((ABIName.startswith("ilp32") || ABIName.startswith("il32")) &&
             IsRV64) {
    errs() << "32-bit ABIs are not supported for 64-bit targets (ignoring "
              "target-abi)\n";
    TargetABI = ABI_Unknown;
  } else if ((ABIName.startswith("lp64") || ABIName.startswith("l64")) &&
             !IsRV64) {
    errs() << "64-bit ABIs are not supported for 32-bit targets (ignoring "
              "target-abi)\n";
    TargetABI = ABI_Unknown;
  } else if (ABIName.endswith("f") && !FeatureBits[RISCV::FeatureStdExtF]) {
    errs() << "Hard-float 'f' ABI can't be used for a target that "
              "doesn't support the F instruction set extension (ignoring "
              "target-abi)\n";
    TargetABI = ABI_Unknown;
  } else if (ABIName.endswith("d") && !FeatureBits[RISCV::FeatureStdExtD]) {
    errs() << "Hard-float 'd' ABI can't be used for a target that "
              "doesn't support the D instruction set extension (ignoring "
              "target-abi)\n";
    TargetABI = ABI_Unknown;
  } else if ((ABIName.startswith("il32pc") || ABIName.startswith("l64pc")) &&
             !FeatureBits[RISCV::FeatureCheri]) {
    errs() << "Pure-capability ABI can't be used for a target that "
              "doesn't support the XCheri instruction set extension (ignoring "
              "target-abi)\n";
    TargetABI = ABI_Unknown;
  } else if (IsRV32E && TargetABI != ABI_ILP32E &&
             TargetABI != ABI_IL32PC64E && TargetABI != ABI_Unknown) {
    errs() << "Only the ilp32e and il32pc64e ABIs are supported for RV32E "
              "(ignoring target-abi)\n";
    TargetABI = ABI_Unknown;
  }

  if (TargetABI != ABI_Unknown)
    return TargetABI;

  // For now, default to the ilp32/ilp32e/lp64 ABI if no explicit ABI is given
  // or an invalid/unrecognised string is given. In the future, it might be
  // worth changing this to default to ilp32f/lp64f and ilp32d/lp64d when
  // hardware support for floating point is present.
  if (IsRV32E)
    return ABI_ILP32E;
  if (IsRV64)
    return ABI_LP64;
  return ABI_ILP32;
}
} // namespace RISCVABI

namespace RISCVFeatures {

void validate(const Triple &TT, const FeatureBitset &FeatureBits) {
  if (TT.isArch64Bit() && FeatureBits[RISCV::FeatureRV32E])
    report_fatal_error("RV32E can't be enabled for an RV64 target");
}

} // namespace RISCVFeatures

} // namespace llvm
