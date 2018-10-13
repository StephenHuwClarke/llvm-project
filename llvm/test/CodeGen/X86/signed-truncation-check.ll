; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=i686-unknown-linux-gnu   < %s | FileCheck %s --check-prefixes=CHECK,X86,NOBMI2,X86-NOBMI2,FALLBACK0,X86-FALLBACK0
; RUN: llc -mtriple=x86_64-unknown-linux-gnu < %s | FileCheck %s --check-prefixes=CHECK,X64,NOBMI2,X64-NOBMI2,FALLBACK0,X64-FALLBACK0

; https://bugs.llvm.org/show_bug.cgi?id=38149

; We are truncating from wider width, and then sign-extending
; back to the original width. Then we equality-comparing orig and src.
; If they don't match, then we had signed truncation during truncation.

; This can be expressed in a several ways in IR:
;   trunc + sext + icmp eq <- not canonical
;   shl   + ashr + icmp eq
;   add          + icmp uge/ugt
;   add          + icmp ult/ule
; However only the simplest form (with two shifts) gets lowered best.

; ---------------------------------------------------------------------------- ;
; shl + ashr + icmp eq
; ---------------------------------------------------------------------------- ;

define i1 @shifts_eqcmp_i16_i8(i16 %x) nounwind {
; X86-LABEL: shifts_eqcmp_i16_i8:
; X86:       # %bb.0:
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    cmpw %ax, %cx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: shifts_eqcmp_i16_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    cmpw %di, %ax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = shl i16 %x, 8 ; 16-8
  %tmp1 = ashr exact i16 %tmp0, 8 ; 16-8
  %tmp2 = icmp eq i16 %tmp1, %x
  ret i1 %tmp2
}

define i1 @shifts_eqcmp_i32_i16(i32 %x) nounwind {
; X86-LABEL: shifts_eqcmp_i32_i16:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movswl %ax, %ecx
; X86-NEXT:    cmpl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: shifts_eqcmp_i32_i16:
; X64:       # %bb.0:
; X64-NEXT:    movswl %di, %eax
; X64-NEXT:    cmpl %edi, %eax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = shl i32 %x, 16 ; 32-16
  %tmp1 = ashr exact i32 %tmp0, 16 ; 32-16
  %tmp2 = icmp eq i32 %tmp1, %x
  ret i1 %tmp2
}

define i1 @shifts_eqcmp_i32_i8(i32 %x) nounwind {
; X86-LABEL: shifts_eqcmp_i32_i8:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    cmpl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: shifts_eqcmp_i32_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    cmpl %edi, %eax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = shl i32 %x, 24 ; 32-8
  %tmp1 = ashr exact i32 %tmp0, 24 ; 32-8
  %tmp2 = icmp eq i32 %tmp1, %x
  ret i1 %tmp2
}

define i1 @shifts_eqcmp_i64_i32(i64 %x) nounwind {
; X86-LABEL: shifts_eqcmp_i64_i32:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    sarl $31, %eax
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: shifts_eqcmp_i64_i32:
; X64:       # %bb.0:
; X64-NEXT:    movslq %edi, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = shl i64 %x, 32 ; 64-32
  %tmp1 = ashr exact i64 %tmp0, 32 ; 64-32
  %tmp2 = icmp eq i64 %tmp1, %x
  ret i1 %tmp2
}

define i1 @shifts_eqcmp_i64_i16(i64 %x) nounwind {
; X86-LABEL: shifts_eqcmp_i64_i16:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movswl %ax, %ecx
; X86-NEXT:    movl %ecx, %edx
; X86-NEXT:    sarl $31, %edx
; X86-NEXT:    xorl %eax, %ecx
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    orl %ecx, %edx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: shifts_eqcmp_i64_i16:
; X64:       # %bb.0:
; X64-NEXT:    movswq %di, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = shl i64 %x, 48 ; 64-16
  %tmp1 = ashr exact i64 %tmp0, 48 ; 64-16
  %tmp2 = icmp eq i64 %tmp1, %x
  ret i1 %tmp2
}

define i1 @shifts_eqcmp_i64_i8(i64 %x) nounwind {
; X86-LABEL: shifts_eqcmp_i64_i8:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    movl %ecx, %edx
; X86-NEXT:    sarl $31, %edx
; X86-NEXT:    xorl %eax, %ecx
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    orl %ecx, %edx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: shifts_eqcmp_i64_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbq %dil, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = shl i64 %x, 56 ; 64-8
  %tmp1 = ashr exact i64 %tmp0, 56 ; 64-8
  %tmp2 = icmp eq i64 %tmp1, %x
  ret i1 %tmp2
}

; ---------------------------------------------------------------------------- ;
; add + icmp uge
; ---------------------------------------------------------------------------- ;

define i1 @add_ugecmp_i16_i8(i16 %x) nounwind {
; X86-LABEL: add_ugecmp_i16_i8:
; X86:       # %bb.0:
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    cmpw %ax, %cx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ugecmp_i16_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    cmpw %di, %ax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, -128 ; ~0U << (8-1)
  %tmp1 = icmp uge i16 %tmp0, -256 ; ~0U << 8
  ret i1 %tmp1
}

define i1 @add_ugecmp_i32_i16(i32 %x) nounwind {
; X86-LABEL: add_ugecmp_i32_i16:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movswl %ax, %ecx
; X86-NEXT:    cmpl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ugecmp_i32_i16:
; X64:       # %bb.0:
; X64-NEXT:    movswl %di, %eax
; X64-NEXT:    cmpl %edi, %eax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i32 %x, -32768 ; ~0U << (16-1)
  %tmp1 = icmp uge i32 %tmp0, -65536 ; ~0U << 16
  ret i1 %tmp1
}

define i1 @add_ugecmp_i32_i8(i32 %x) nounwind {
; X86-LABEL: add_ugecmp_i32_i8:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    cmpl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ugecmp_i32_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    cmpl %edi, %eax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i32 %x, -128 ; ~0U << (8-1)
  %tmp1 = icmp uge i32 %tmp0, -256 ; ~0U << 8
  ret i1 %tmp1
}

define i1 @add_ugecmp_i64_i32(i64 %x) nounwind {
; X86-LABEL: add_ugecmp_i64_i32:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    sarl $31, %eax
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ugecmp_i64_i32:
; X64:       # %bb.0:
; X64-NEXT:    movslq %edi, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i64 %x, -2147483648 ; ~0U << (32-1)
  %tmp1 = icmp uge i64 %tmp0, -4294967296 ; ~0U << 32
  ret i1 %tmp1
}

define i1 @add_ugecmp_i64_i16(i64 %x) nounwind {
; X86-LABEL: add_ugecmp_i64_i16:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movswl %ax, %ecx
; X86-NEXT:    xorl %ecx, %eax
; X86-NEXT:    sarl $31, %ecx
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    orl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ugecmp_i64_i16:
; X64:       # %bb.0:
; X64-NEXT:    movswq %di, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i64 %x, -32768 ; ~0U << (16-1)
  %tmp1 = icmp uge i64 %tmp0, -65536 ; ~0U << 16
  ret i1 %tmp1
}

define i1 @add_ugecmp_i64_i8(i64 %x) nounwind {
; X86-LABEL: add_ugecmp_i64_i8:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    xorl %ecx, %eax
; X86-NEXT:    sarl $31, %ecx
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    orl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ugecmp_i64_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbq %dil, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i64 %x, -128 ; ~0U << (8-1)
  %tmp1 = icmp uge i64 %tmp0, -256 ; ~0U << 8
  ret i1 %tmp1
}

; Slightly more canonical variant
define i1 @add_ugtcmp_i16_i8(i16 %x) nounwind {
; X86-LABEL: add_ugtcmp_i16_i8:
; X86:       # %bb.0:
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    cmpw %ax, %cx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ugtcmp_i16_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    cmpw %di, %ax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, -128 ; ~0U << (8-1)
  %tmp1 = icmp ugt i16 %tmp0, -257 ; ~0U << 8 - 1
  ret i1 %tmp1
}

; ---------------------------------------------------------------------------- ;
; add + icmp ult
; ---------------------------------------------------------------------------- ;

define i1 @add_ultcmp_i16_i8(i16 %x) nounwind {
; X86-LABEL: add_ultcmp_i16_i8:
; X86:       # %bb.0:
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    cmpw %ax, %cx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_i16_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    cmpw %di, %ax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ult i16 %tmp0, 256 ; 1U << 8
  ret i1 %tmp1
}

define i1 @add_ultcmp_i32_i16(i32 %x) nounwind {
; X86-LABEL: add_ultcmp_i32_i16:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movswl %ax, %ecx
; X86-NEXT:    cmpl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_i32_i16:
; X64:       # %bb.0:
; X64-NEXT:    movswl %di, %eax
; X64-NEXT:    cmpl %edi, %eax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i32 %x, 32768 ; 1U << (16-1)
  %tmp1 = icmp ult i32 %tmp0, 65536 ; 1U << 16
  ret i1 %tmp1
}

define i1 @add_ultcmp_i32_i8(i32 %x) nounwind {
; X86-LABEL: add_ultcmp_i32_i8:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    cmpl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_i32_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    cmpl %edi, %eax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i32 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ult i32 %tmp0, 256 ; 1U << 8
  ret i1 %tmp1
}

define i1 @add_ultcmp_i64_i32(i64 %x) nounwind {
; X86-LABEL: add_ultcmp_i64_i32:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    sarl $31, %eax
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_i64_i32:
; X64:       # %bb.0:
; X64-NEXT:    movslq %edi, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i64 %x, 2147483648 ; 1U << (32-1)
  %tmp1 = icmp ult i64 %tmp0, 4294967296 ; 1U << 32
  ret i1 %tmp1
}

define i1 @add_ultcmp_i64_i16(i64 %x) nounwind {
; X86-LABEL: add_ultcmp_i64_i16:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movswl %ax, %ecx
; X86-NEXT:    xorl %ecx, %eax
; X86-NEXT:    sarl $31, %ecx
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    orl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_i64_i16:
; X64:       # %bb.0:
; X64-NEXT:    movswq %di, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i64 %x, 32768 ; 1U << (16-1)
  %tmp1 = icmp ult i64 %tmp0, 65536 ; 1U << 16
  ret i1 %tmp1
}

define i1 @add_ultcmp_i64_i8(i64 %x) nounwind {
; X86-LABEL: add_ultcmp_i64_i8:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    xorl %ecx, %eax
; X86-NEXT:    sarl $31, %ecx
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    orl %eax, %ecx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_i64_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbq %dil, %rax
; X64-NEXT:    cmpq %rdi, %rax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i64 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ult i64 %tmp0, 256 ; 1U << 8
  ret i1 %tmp1
}

; Slightly more canonical variant
define i1 @add_ulecmp_i16_i8(i16 %x) nounwind {
; X86-LABEL: add_ulecmp_i16_i8:
; X86:       # %bb.0:
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movsbl %al, %ecx
; X86-NEXT:    cmpw %ax, %cx
; X86-NEXT:    sete %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ulecmp_i16_i8:
; X64:       # %bb.0:
; X64-NEXT:    movsbl %dil, %eax
; X64-NEXT:    cmpw %di, %ax
; X64-NEXT:    sete %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ule i16 %tmp0, 255 ; (1U << 8) - 1
  ret i1 %tmp1
}

; Negative tests
; ---------------------------------------------------------------------------- ;

; Adding not a constant
define i1 @add_ultcmp_bad_i16_i8_add(i16 %x, i16 %y) nounwind {
; X86-LABEL: add_ultcmp_bad_i16_i8_add:
; X86:       # %bb.0:
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    addw {{[0-9]+}}(%esp), %ax
; X86-NEXT:    movzwl %ax, %eax
; X86-NEXT:    cmpl $256, %eax # imm = 0x100
; X86-NEXT:    setb %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_bad_i16_i8_add:
; X64:       # %bb.0:
; X64-NEXT:    addl %esi, %edi
; X64-NEXT:    movzwl %di, %eax
; X64-NEXT:    cmpl $256, %eax # imm = 0x100
; X64-NEXT:    setb %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, %y
  %tmp1 = icmp ult i16 %tmp0, 256 ; 1U << 8
  ret i1 %tmp1
}

; Comparing not with a constant
define i1 @add_ultcmp_bad_i16_i8_cmp(i16 %x, i16 %y) nounwind {
; X86-LABEL: add_ultcmp_bad_i16_i8_cmp:
; X86:       # %bb.0:
; X86-NEXT:    movl $128, %eax
; X86-NEXT:    addl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    cmpw {{[0-9]+}}(%esp), %ax
; X86-NEXT:    setb %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_bad_i16_i8_cmp:
; X64:       # %bb.0:
; X64-NEXT:    subl $-128, %edi
; X64-NEXT:    cmpw %si, %di
; X64-NEXT:    setb %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ult i16 %tmp0, %y
  ret i1 %tmp1
}

; Second constant is not larger than the first one
define i1 @add_ultcmp_bad_i8_i16(i16 %x) nounwind {
; X86-LABEL: add_ultcmp_bad_i8_i16:
; X86:       # %bb.0:
; X86-NEXT:    movw $128, %ax
; X86-NEXT:    addw {{[0-9]+}}(%esp), %ax
; X86-NEXT:    setb %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_bad_i8_i16:
; X64:       # %bb.0:
; X64-NEXT:    addw $128, %di
; X64-NEXT:    setb %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ult i16 %tmp0, 128 ; 1U << (8-1)
  ret i1 %tmp1
}

; First constant is not power of two
define i1 @add_ultcmp_bad_i16_i8_c0notpoweroftwo(i16 %x) nounwind {
; X86-LABEL: add_ultcmp_bad_i16_i8_c0notpoweroftwo:
; X86:       # %bb.0:
; X86-NEXT:    movl $192, %eax
; X86-NEXT:    addl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movzwl %ax, %eax
; X86-NEXT:    cmpl $256, %eax # imm = 0x100
; X86-NEXT:    setb %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_bad_i16_i8_c0notpoweroftwo:
; X64:       # %bb.0:
; X64-NEXT:    addl $192, %edi
; X64-NEXT:    movzwl %di, %eax
; X64-NEXT:    cmpl $256, %eax # imm = 0x100
; X64-NEXT:    setb %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, 192 ; (1U << (8-1)) + (1U << (8-1-1))
  %tmp1 = icmp ult i16 %tmp0, 256 ; 1U << 8
  ret i1 %tmp1
}

; Second constant is not power of two
define i1 @add_ultcmp_bad_i16_i8_c1notpoweroftwo(i16 %x) nounwind {
; X86-LABEL: add_ultcmp_bad_i16_i8_c1notpoweroftwo:
; X86:       # %bb.0:
; X86-NEXT:    movl $128, %eax
; X86-NEXT:    addl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movzwl %ax, %eax
; X86-NEXT:    cmpl $768, %eax # imm = 0x300
; X86-NEXT:    setb %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_bad_i16_i8_c1notpoweroftwo:
; X64:       # %bb.0:
; X64-NEXT:    subl $-128, %edi
; X64-NEXT:    movzwl %di, %eax
; X64-NEXT:    cmpl $768, %eax # imm = 0x300
; X64-NEXT:    setb %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ult i16 %tmp0, 768 ; (1U << 8)) + (1U << (8+1))
  ret i1 %tmp1
}

; Magic check fails, 64 << 1 != 256
define i1 @add_ultcmp_bad_i16_i8_magic(i16 %x) nounwind {
; X86-LABEL: add_ultcmp_bad_i16_i8_magic:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    addl $64, %eax
; X86-NEXT:    movzwl %ax, %eax
; X86-NEXT:    cmpl $256, %eax # imm = 0x100
; X86-NEXT:    setb %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_bad_i16_i8_magic:
; X64:       # %bb.0:
; X64-NEXT:    addl $64, %edi
; X64-NEXT:    movzwl %di, %eax
; X64-NEXT:    cmpl $256, %eax # imm = 0x100
; X64-NEXT:    setb %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, 64 ; 1U << (8-1-1)
  %tmp1 = icmp ult i16 %tmp0, 256 ; 1U << 8
  ret i1 %tmp1
}

; Bad 'destination type'
define i1 @add_ultcmp_bad_i16_i4(i16 %x) nounwind {
; X86-LABEL: add_ultcmp_bad_i16_i4:
; X86:       # %bb.0:
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    addl $8, %eax
; X86-NEXT:    cmpw $16, %ax
; X86-NEXT:    setb %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_bad_i16_i4:
; X64:       # %bb.0:
; X64-NEXT:    addl $8, %edi
; X64-NEXT:    cmpw $16, %di
; X64-NEXT:    setb %al
; X64-NEXT:    retq
  %tmp0 = add i16 %x, 8 ; 1U << (4-1)
  %tmp1 = icmp ult i16 %tmp0, 16 ; 1U << 4
  ret i1 %tmp1
}

; Bad storage type
define i1 @add_ultcmp_bad_i24_i8(i24 %x) nounwind {
; X86-LABEL: add_ultcmp_bad_i24_i8:
; X86:       # %bb.0:
; X86-NEXT:    movl $128, %eax
; X86-NEXT:    addl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    andl $16777215, %eax # imm = 0xFFFFFF
; X86-NEXT:    cmpl $256, %eax # imm = 0x100
; X86-NEXT:    setb %al
; X86-NEXT:    retl
;
; X64-LABEL: add_ultcmp_bad_i24_i8:
; X64:       # %bb.0:
; X64-NEXT:    subl $-128, %edi
; X64-NEXT:    andl $16777215, %edi # imm = 0xFFFFFF
; X64-NEXT:    cmpl $256, %edi # imm = 0x100
; X64-NEXT:    setb %al
; X64-NEXT:    retq
  %tmp0 = add i24 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ult i24 %tmp0, 256 ; 1U << 8
  ret i1 %tmp1
}

define i1 @add_ulecmp_bad_i16_i8(i16 %x) nounwind {
; CHECK-LABEL: add_ulecmp_bad_i16_i8:
; CHECK:       # %bb.0:
; CHECK-NEXT:    movb $1, %al
; CHECK-NEXT:    ret{{[l|q]}}
  %tmp0 = add i16 %x, 128 ; 1U << (8-1)
  %tmp1 = icmp ule i16 %tmp0, -1 ; when we +1 it, it will wrap to 0
  ret i1 %tmp1
}
