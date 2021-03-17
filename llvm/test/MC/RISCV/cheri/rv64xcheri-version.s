# RUN: llvm-mc %s -triple=riscv64 -mattr=+xcheri,+xcherimemversion -show-encoding \
# RUN:     | FileCheck %s
# RUN: not llvm-mc %s -triple=riscv64 -mattr=+xcheri -show-encoding \
# RUN:    2>&1 | FileCheck %s -check-prefix=CHECK-DISABLED

# CHECK: cgetversion     gp, csp
# CHECK: encoding: [0xdb,0x01,0x31,0xff]
# CHECK-DISABLED: error: instruction requires the following: CHERI Extension with memory versioning
CGetVersion x3, c2
# CHECK: csetversion     cra, csp, gp
# CHECK: encoding: [0xdb,0x00,0x31,0x04]
# CHECK-DISABLED: error: instruction requires the following: CHERI Extension with memory versioning
CSetVersion c1, c2, x3
# CHECK: cloadversion    sp, cra
# CHECK: encoding: [0x5b,0x81,0x40,0xff]
# CHECK-DISABLED: error: instruction requires the following: CHERI Extension with memory versioning
CloadVersion x2, c1
# CHECK: cloadversions   sp, cra
# CHECK: encoding: [0x5b,0x81,0x50,0xff]
# CHECK-DISABLED: error: instruction requires the following: CHERI Extension with memory versioning
CloadVersions x2, c1
# CHECK: cstoreversion   ra, ctp
# CHECK: encoding: [0x5b,0x01,0x12,0xfc]
# CHECK-DISABLED: error: instruction requires the following: CHERI Extension with memory versioning
CStoreVersion x1, c4
# CHECK: camocdecversion ra, csp, cgp
# CHECK: encoding: [0xdb,0x00,0x31,0x06]
# CHECK-DISABLED: error: instruction requires the following: CHERI Extension with memory versioning
CAmoCDecVersion x1, c2, c3


