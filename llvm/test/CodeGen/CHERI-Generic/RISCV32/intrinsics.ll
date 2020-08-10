; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --force-update --function-signature --scrub-attributes
; DO NOT EDIT -- This file was generated from test/CodeGen/CHERI-Generic/Inputs/intrinsics.ll
; RUN: %riscv32_cheri_purecap_llc %s -o - < %s | FileCheck %s --check-prefix=PURECAP
; RUN: %riscv32_cheri_llc -o - < %s | FileCheck %s --check-prefix=HYBRID
; Check that the target-independent CHERI intrinsics are support for all architectures
; The grouping/ordering in this test is based on the RISC-V instruction listing
; in the CHERI ISA specification (Appendix C.1 in ISAv7).

; Capability-Inspection Instructions

declare i32 @llvm.cheri.cap.perms.get.i32(i8 addrspace(200)*)
declare i32 @llvm.cheri.cap.type.get.i32(i8 addrspace(200)*)
declare i32 @llvm.cheri.cap.base.get.i32(i8 addrspace(200)*)
declare i32 @llvm.cheri.cap.length.get.i32(i8 addrspace(200)*)
declare i1 @llvm.cheri.cap.tag.get(i8 addrspace(200)*)
declare i1 @llvm.cheri.cap.sealed.get(i8 addrspace(200)*)
declare i32 @llvm.cheri.cap.offset.get.i32(i8 addrspace(200)*)
declare i32 @llvm.cheri.cap.flags.get.i32(i8 addrspace(200)*)
declare i32 @llvm.cheri.cap.address.get.i32(i8 addrspace(200)*)

define i32 @perms_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: perms_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgetperm a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: perms_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgetperm a0, ca0
; HYBRID-NEXT:    ret
  %perms = call i32 @llvm.cheri.cap.perms.get.i32(i8 addrspace(200)* %cap)
  ret i32 %perms
}

define i32 @type_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: type_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgettype a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: type_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgettype a0, ca0
; HYBRID-NEXT:    ret
  %type = call i32 @llvm.cheri.cap.type.get.i32(i8 addrspace(200)* %cap)
  ret i32 %type
}

define i32 @base_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: base_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgetbase a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: base_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgetbase a0, ca0
; HYBRID-NEXT:    ret
  %base = call i32 @llvm.cheri.cap.base.get.i32(i8 addrspace(200)* %cap)
  ret i32 %base
}

define i32 @length_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: length_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgetlen a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: length_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgetlen a0, ca0
; HYBRID-NEXT:    ret
  %length = call i32 @llvm.cheri.cap.length.get.i32(i8 addrspace(200)* %cap)
  ret i32 %length
}

define i32 @tag_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: tag_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgettag a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: tag_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgettag a0, ca0
; HYBRID-NEXT:    ret
  %tag = call i1 @llvm.cheri.cap.tag.get(i8 addrspace(200)* %cap)
  %tag.zext = zext i1 %tag to i32
  ret i32 %tag.zext
}

define i32 @sealed_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: sealed_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgetsealed a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: sealed_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgetsealed a0, ca0
; HYBRID-NEXT:    ret
  %sealed = call i1 @llvm.cheri.cap.sealed.get(i8 addrspace(200)* %cap)
  %sealed.zext = zext i1 %sealed to i32
  ret i32 %sealed.zext
}

define i32 @offset_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: offset_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgetoffset a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: offset_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgetoffset a0, ca0
; HYBRID-NEXT:    ret
  %offset = call i32 @llvm.cheri.cap.offset.get.i32(i8 addrspace(200)* %cap)
  ret i32 %offset
}

define i32 @flags_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: flags_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgetflags a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: flags_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgetflags a0, ca0
; HYBRID-NEXT:    ret
  %flags = call i32 @llvm.cheri.cap.flags.get.i32(i8 addrspace(200)* %cap)
  ret i32 %flags
}

define i32 @address_get(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: address_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cgetaddr a0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: address_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cgetaddr a0, ca0
; HYBRID-NEXT:    ret
  %address = call i32 @llvm.cheri.cap.address.get.i32(i8 addrspace(200)* %cap)
  ret i32 %address
}

; Capability-Modification Instructions

declare i8 addrspace(200)* @llvm.cheri.cap.seal(i8 addrspace(200)*, i8 addrspace(200)*)
declare i8 addrspace(200)* @llvm.cheri.cap.unseal(i8 addrspace(200)*, i8 addrspace(200)*)
declare i8 addrspace(200)* @llvm.cheri.cap.perms.and.i32(i8 addrspace(200)*, i32)
declare i8 addrspace(200)* @llvm.cheri.cap.flags.set.i32(i8 addrspace(200)*, i32)
declare i8 addrspace(200)* @llvm.cheri.cap.offset.set.i32(i8 addrspace(200)*, i32)
declare i8 addrspace(200)* @llvm.cheri.cap.address.set.i32(i8 addrspace(200)*, i32)
declare i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i32(i8 addrspace(200)*, i32)
declare i8 addrspace(200)* @llvm.cheri.cap.bounds.set.exact.i32(i8 addrspace(200)*, i32)
declare i8 addrspace(200)* @llvm.cheri.cap.tag.clear(i8 addrspace(200)*)
declare i8 addrspace(200)* @llvm.cheri.cap.build(i8 addrspace(200)*, i8 addrspace(200)*)
declare i8 addrspace(200)* @llvm.cheri.cap.type.copy(i8 addrspace(200)*, i8 addrspace(200)*)
declare i8 addrspace(200)* @llvm.cheri.cap.conditional.seal(i8 addrspace(200)*, i8 addrspace(200)*)
declare i8 addrspace(200)* @llvm.cheri.cap.seal.entry(i8 addrspace(200)*)

define i8 addrspace(200)* @seal(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: seal:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cseal ca0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: seal:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cseal ca0, ca0, ca1
; HYBRID-NEXT:    ret
  %sealed = call i8 addrspace(200)* @llvm.cheri.cap.seal(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  ret i8 addrspace(200)* %sealed
}

define i8 addrspace(200)* @unseal(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: unseal:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cunseal ca0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: unseal:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cunseal ca0, ca0, ca1
; HYBRID-NEXT:    ret
  %unsealed = call i8 addrspace(200)* @llvm.cheri.cap.unseal(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  ret i8 addrspace(200)* %unsealed
}

define i8 addrspace(200)* @perms_and(i8 addrspace(200)* %cap, i32 %perms) nounwind {
; PURECAP-LABEL: perms_and:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    candperm ca0, ca0, a1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: perms_and:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    candperm ca0, ca0, a1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.perms.and.i32(i8 addrspace(200)* %cap, i32 %perms)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @flags_set(i8 addrspace(200)* %cap, i32 %flags) nounwind {
; PURECAP-LABEL: flags_set:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    csetflags ca0, ca0, a1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: flags_set:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    csetflags ca0, ca0, a1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.flags.set.i32(i8 addrspace(200)* %cap, i32 %flags)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @offset_set(i8 addrspace(200)* %cap, i32 %offset) nounwind {
; PURECAP-LABEL: offset_set:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    csetoffset ca0, ca0, a1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: offset_set:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    csetoffset ca0, ca0, a1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.offset.set.i32(i8 addrspace(200)* %cap, i32 %offset)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @address_set(i8 addrspace(200)* %cap, i32 %address) nounwind {
; PURECAP-LABEL: address_set:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    csetaddr ca0, ca0, a1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: address_set:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    csetaddr ca0, ca0, a1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.address.set.i32(i8 addrspace(200)* %cap, i32 %address)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @bounds_set(i8 addrspace(200)* %cap, i32 %bounds) nounwind {
; PURECAP-LABEL: bounds_set:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    csetbounds ca0, ca0, a1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: bounds_set:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    csetbounds ca0, ca0, a1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i32(i8 addrspace(200)* %cap, i32 %bounds)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @bounds_set_exact(i8 addrspace(200)* %cap, i32 %bounds) nounwind {
; PURECAP-LABEL: bounds_set_exact:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    csetboundsexact ca0, ca0, a1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: bounds_set_exact:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    csetboundsexact ca0, ca0, a1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.exact.i32(i8 addrspace(200)* %cap, i32 %bounds)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @bounds_set_immediate(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: bounds_set_immediate:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    csetbounds ca0, ca0, 42
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: bounds_set_immediate:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    csetbounds ca0, ca0, 42
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i32(i8 addrspace(200)* %cap, i32 42)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @tag_clear(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: tag_clear:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    ccleartag ca0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: tag_clear:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    ccleartag ca0, ca0
; HYBRID-NEXT:    ret
  %untagged = call i8 addrspace(200)* @llvm.cheri.cap.tag.clear(i8 addrspace(200)* %cap)
  ret i8 addrspace(200)* %untagged
}

define i8 addrspace(200)* @build(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: build:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cbuildcap ca0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: build:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cbuildcap ca0, ca0, ca1
; HYBRID-NEXT:    ret
  %built = call i8 addrspace(200)* @llvm.cheri.cap.build(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  ret i8 addrspace(200)* %built
}

define i8 addrspace(200)* @type_copy(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: type_copy:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    ccopytype ca0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: type_copy:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    ccopytype ca0, ca0, ca1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.type.copy(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @conditional_seal(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: conditional_seal:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    ccseal ca0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: conditional_seal:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    ccseal ca0, ca0, ca1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.conditional.seal(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @seal_entry(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: seal_entry:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    csealentry ca0, ca0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: seal_entry:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    csealentry ca0, ca0
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.seal.entry(i8 addrspace(200)* %cap)
  ret i8 addrspace(200)* %newcap
}

; Pointer-Arithmetic Instructions

declare i32 @llvm.cheri.cap.to.pointer(i8 addrspace(200)*, i8 addrspace(200)*)
declare i8 addrspace(200)* @llvm.cheri.cap.from.pointer(i8 addrspace(200)*, i32)
declare i32 @llvm.cheri.cap.diff(i8 addrspace(200)*, i8 addrspace(200)*)
declare i8 addrspace(200)* @llvm.cheri.ddc.get()
declare i8 addrspace(200)* @llvm.cheri.pcc.get()

define i32 @to_pointer(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: to_pointer:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    ctoptr a0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: to_pointer:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    ctoptr a0, ca0, ca1
; HYBRID-NEXT:    ret
  %ptr = call i32 @llvm.cheri.cap.to.pointer(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  ret i32 %ptr
}

define i32 @to_pointer_ddc_relative(i8 addrspace(200)* %cap) nounwind {
; PURECAP-LABEL: to_pointer_ddc_relative:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    ctoptr a0, ca0, ddc
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: to_pointer_ddc_relative:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    ctoptr a0, ca0, ddc
; HYBRID-NEXT:    ret
  %ddc = call i8 addrspace(200)* @llvm.cheri.ddc.get()
  %ptr = call i32 @llvm.cheri.cap.to.pointer(i8 addrspace(200)* %ddc, i8 addrspace(200)* %cap)
  ret i32 %ptr
}

define i8 addrspace(200)* @from_pointer(i8 addrspace(200)* %cap, i32 %ptr) nounwind {
; PURECAP-LABEL: from_pointer:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cfromptr ca0, ca0, a1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: from_pointer:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cfromptr ca0, ca0, a1
; HYBRID-NEXT:    ret
  %newcap = call i8 addrspace(200)* @llvm.cheri.cap.from.pointer(i8 addrspace(200)* %cap, i32 %ptr)
  ret i8 addrspace(200)* %newcap
}

define i8 addrspace(200)* @from_ddc(i32 %ptr) nounwind {
; PURECAP-LABEL: from_ddc:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cfromptr ca0, ddc, a0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: from_ddc:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cfromptr ca0, ddc, a0
; HYBRID-NEXT:    ret
  %ddc = call i8 addrspace(200)* @llvm.cheri.ddc.get()
  %cap = call i8 addrspace(200)* @llvm.cheri.cap.from.pointer(i8 addrspace(200)* %ddc, i32 %ptr)
  ret i8 addrspace(200)* %cap
}

define i32 @diff(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: diff:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    csub a0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: diff:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    csub a0, ca0, ca1
; HYBRID-NEXT:    ret
  %diff = call i32 @llvm.cheri.cap.diff(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  ret i32 %diff
}

define i8 addrspace(200)* @ddc_get() nounwind {
; PURECAP-LABEL: ddc_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cspecialr ca0, ddc
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: ddc_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cspecialr ca0, ddc
; HYBRID-NEXT:    ret
  %cap = call i8 addrspace(200)* @llvm.cheri.ddc.get()
  ret i8 addrspace(200)* %cap
}

define i8 addrspace(200)* @pcc_get() nounwind {
; PURECAP-LABEL: pcc_get:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    auipcc ca0, 0
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: pcc_get:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cspecialr ca0, pcc
; HYBRID-NEXT:    ret
  %cap = call i8 addrspace(200)* @llvm.cheri.pcc.get()
  ret i8 addrspace(200)* %cap
}

; Assertion Instructions

declare i1 @llvm.cheri.cap.subset.test(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)

define i32 @subset_test(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: subset_test:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    ctestsubset a0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: subset_test:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    ctestsubset a0, ca0, ca1
; HYBRID-NEXT:    ret
  %subset = call i1 @llvm.cheri.cap.subset.test(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  %subset.zext = zext i1 %subset to i32
  ret i32 %subset.zext
}

declare i1 @llvm.cheri.cap.equal.exact(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)

define i32 @equal_exact(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2) nounwind {
; PURECAP-LABEL: equal_exact:
; PURECAP:       # %bb.0:
; PURECAP-NEXT:    cseqx a0, ca0, ca1
; PURECAP-NEXT:    cret
;
; HYBRID-LABEL: equal_exact:
; HYBRID:       # %bb.0:
; HYBRID-NEXT:    cseqx a0, ca0, ca1
; HYBRID-NEXT:    ret
  %eqex = call i1 @llvm.cheri.cap.equal.exact(i8 addrspace(200)* %cap1, i8 addrspace(200)* %cap2)
  %eqex.zext = zext i1 %eqex to i32
  ret i32 %eqex.zext
}
