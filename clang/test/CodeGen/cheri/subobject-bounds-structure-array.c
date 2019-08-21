// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py
// REQUIRES: asserts
// RUN: %cheri128_purecap_cc1 -cheri-bounds=subobject-safe -O2 -emit-llvm %s -o - -Wcheri-subobject-bounds -Rcheri-subobject-bounds -verify | %cheri_FileCheck %s
// RUN: %cheri_purecap_cc1 -cheri-bounds=subobject-safe  -O0 -emit-llvm %s -o /dev/null \
// RUN:    -w -mllvm -debug-only="cheri-bounds" -mllvm -stats 2>&1 | FileCheck %s -check-prefix DBG -implicit-check-not cheri-bounds
// DBG:  3 cheri-bounds - Number of & operators checked for tightening bounds
// DBG:  4 cheri-bounds - Number of array-to-pointer-decays checked for tightening bounds
// DBG: 36 cheri-bounds - Number of [] operators checked for tightening bounds
// DBG:  1 cheri-bounds - Number of & operators where remaining allocation size was used
// DBG:  1 cheri-bounds - Number of array-to-pointer-decays where remaining allocation size was used
// DBG:  5 cheri-bounds - Number of [] operators where remaining allocation size was used
// DBG:  2 cheri-bounds - Number of & operators where bounds were tightened
// DBG:  3 cheri-bounds - Number of array-to-pointer-decays where bounds were tightened
// DBG: 22 cheri-bounds - Number of [] operators where bounds were tightened
// REQUIRES: asserts

void use_buf(void *);

typedef struct {
  long x;
  int buf[10];
  char c;
} struct_with_array;

// NOTE: bounds must be set before the GEP:
// CHECK-LABEL: @test_struct_with_array1(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[BUF:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_WITH_ARRAY:%.*]], [[STRUCT_STRUCT_WITH_ARRAY]] addrspace(200)* [[S:%.*]], i64 0, i32 1
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [10 x i32] addrspace(200)* [[BUF]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP0]], i64 40)
// CHECK-NEXT:    [[SUBSCRIPT_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP1]] to [10 x i32] addrspace(200)*
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [10 x i32], [10 x i32] addrspace(200)* [[SUBSCRIPT_WITH_BOUNDS]], i64 0, i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[TMP2:%.*]] = load i32, i32 addrspace(200)* [[ARRAYIDX]], align 4, !tbaa !2
// CHECK-NEXT:    ret i32 [[TMP2]]
//
int test_struct_with_array1(struct_with_array *s, long index) {
  // should set bounds to 10
  return s->buf[index]; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'int [10]') to 40 bytes}}
}

// CHECK-LABEL: @test_struct_with_array2(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_WITH_ARRAY:%.*]], [[STRUCT_STRUCT_WITH_ARRAY]] addrspace(200)* [[S:%.*]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [[STRUCT_STRUCT_WITH_ARRAY]] addrspace(200)* [[AGG_RESULT:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast [[STRUCT_STRUCT_WITH_ARRAY]] addrspace(200)* [[ARRAYIDX]] to i8 addrspace(200)*
// CHECK-NEXT:    tail call void @llvm.memcpy.p200i8.p200i8.i64(i8 addrspace(200)* align 8 [[TMP0]], i8 addrspace(200)* align 8 [[TMP1]], i64 56, i1 false), !tbaa.struct !6
// CHECK-NEXT:    ret void
//
struct_with_array test_struct_with_array2(struct_with_array *s, long index) {
  // can't set bounds here, have to trust the caller's bounds
  return s[index]; // expected-remark{{not setting bounds for array subscript on 'struct_with_array * __capability' (array subscript on non-array type)}}
}

typedef struct {
  long x;
  int *buf;
  char c;
} struct_with_ptr;

// CHECK-LABEL: @test_struct_with_ptr1(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[BUF:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_WITH_PTR:%.*]], [[STRUCT_STRUCT_WITH_PTR]] addrspace(200)* [[S:%.*]], i64 0, i32 1
// CHECK-NEXT:    [[TMP0:%.*]] = load i32 addrspace(200)*, i32 addrspace(200)* addrspace(200)* [[BUF]], align 16, !tbaa !10
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i32, i32 addrspace(200)* [[TMP0]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[TMP1:%.*]] = load i32, i32 addrspace(200)* [[ARRAYIDX]], align 4, !tbaa !2
// CHECK-NEXT:    ret i32 [[TMP1]]
//
int test_struct_with_ptr1(struct_with_ptr *s, long index) {
  return s->buf[index]; // expected-remark{{not setting bounds for array subscript on 'int * __capability' (array subscript on non-array type)}}
}

// CHECK-LABEL: @test_struct_with_ptr2(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_WITH_PTR:%.*]], [[STRUCT_STRUCT_WITH_PTR]] addrspace(200)* [[S:%.*]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [[STRUCT_STRUCT_WITH_PTR]] addrspace(200)* [[AGG_RESULT:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast [[STRUCT_STRUCT_WITH_PTR]] addrspace(200)* [[ARRAYIDX]] to i8 addrspace(200)*
// CHECK-NEXT:    tail call void @llvm.memcpy.p200i8.p200i8.i64(i8 addrspace(200)* align 16 [[TMP0]], i8 addrspace(200)* align 16 [[TMP1]], i64 48, i1 false), !tbaa.struct !13
// CHECK-NEXT:    ret void
//
struct_with_ptr test_struct_with_ptr2(struct_with_ptr *s, long index) {
  return s[index]; // expected-remark{{not setting bounds for array subscript on 'struct_with_ptr * __capability' (array subscript on non-array type)}}
}

typedef struct {
  int x;
  char buf[];
} struct_vla;
// CHECK-LABEL: @test_vla_a(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_VLA:%.*]], [[STRUCT_STRUCT_VLA]] addrspace(200)* [[S:%.*]], i64 0, i32 1, i64 0
// CHECK-NEXT:    [[CUR_OFFSET:%.*]] = tail call i64 @llvm.cheri.cap.offset.get.i64(i8 addrspace(200)* nonnull [[TMP0]])
// CHECK-NEXT:    [[CUR_LEN:%.*]] = tail call i64 @llvm.cheri.cap.length.get.i64(i8 addrspace(200)* nonnull [[TMP0]])
// CHECK-NEXT:    [[REMAINING_BYTES:%.*]] = sub i64 [[CUR_LEN]], [[CUR_OFFSET]]
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP0]], i64 [[REMAINING_BYTES]])
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test_vla_a(struct_vla *s, long index) {
  // can't set bounds here, have to trust the caller's bounds
  s->buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char []') to remaining bytes (member is potential variable length array)}}
  return 0;
}
// CHECK-LABEL: @test_vla_b(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca [[STRUCT_STRUCT_VLA:%.*]], align 4, addrspace(200)
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_VLA]], [[STRUCT_STRUCT_VLA]] addrspace(200)* [[RETVAL]], i64 0, i32 1, i64 0
// CHECK-NEXT:    [[CUR_OFFSET:%.*]] = call i64 @llvm.cheri.cap.offset.get.i64(i8 addrspace(200)* nonnull [[TMP0]])
// CHECK-NEXT:    [[CUR_LEN:%.*]] = call i64 @llvm.cheri.cap.length.get.i64(i8 addrspace(200)* nonnull [[TMP0]])
// CHECK-NEXT:    [[REMAINING_BYTES:%.*]] = sub i64 [[CUR_LEN]], [[CUR_OFFSET]]
// CHECK-NEXT:    [[TMP1:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP0]], i64 [[REMAINING_BYTES]])
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    [[DOTFCA_0_GEP:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_VLA]], [[STRUCT_STRUCT_VLA]] addrspace(200)* [[RETVAL]], i64 0, i32 0
// CHECK-NEXT:    [[DOTFCA_0_LOAD:%.*]] = load i32, i32 addrspace(200)* [[DOTFCA_0_GEP]], align 4
// CHECK-NEXT:    [[DOTFCA_0_INSERT:%.*]] = insertvalue { i32 } undef, i32 [[DOTFCA_0_LOAD]], 0
// CHECK-NEXT:    ret { i32 } [[DOTFCA_0_INSERT]]
//
struct_vla test_vla_b(long index) {
  struct_vla s2;
  s2.buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char []') to remaining bytes (member is potential variable length array)}}
  return s2;           // prevent s2 from being optimzed out
}

typedef struct {
  int x;
  char buf[0];
} struct_fake_vla1;
// CHECK-LABEL: @test_fake_vla1(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_FAKE_VLA1:%.*]], [[STRUCT_STRUCT_FAKE_VLA1]] addrspace(200)* [[S:%.*]], i64 0, i32 1, i64 0
// CHECK-NEXT:    [[CUR_OFFSET:%.*]] = tail call i64 @llvm.cheri.cap.offset.get.i64(i8 addrspace(200)* nonnull [[TMP0]])
// CHECK-NEXT:    [[CUR_LEN:%.*]] = tail call i64 @llvm.cheri.cap.length.get.i64(i8 addrspace(200)* nonnull [[TMP0]])
// CHECK-NEXT:    [[REMAINING_BYTES:%.*]] = sub i64 [[CUR_LEN]], [[CUR_OFFSET]]
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP0]], i64 [[REMAINING_BYTES]])
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test_fake_vla1(struct_fake_vla1 *s, long index) {
  // can't set bounds here, have to trust the caller's bounds
  s->buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [0]') to remaining bytes (member is potential variable length array)}}
  return 0;
}

typedef struct {
  int x;
  char buf[1];
} struct_fake_vla2;
// CHECK-LABEL: @test_fake_vla2(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_STRUCT_FAKE_VLA2:%.*]], [[STRUCT_STRUCT_FAKE_VLA2]] addrspace(200)* [[S:%.*]], i64 0, i32 1, i64 0
// CHECK-NEXT:    [[CUR_OFFSET:%.*]] = tail call i64 @llvm.cheri.cap.offset.get.i64(i8 addrspace(200)* nonnull [[TMP0]])
// CHECK-NEXT:    [[CUR_LEN:%.*]] = tail call i64 @llvm.cheri.cap.length.get.i64(i8 addrspace(200)* nonnull [[TMP0]])
// CHECK-NEXT:    [[REMAINING_BYTES:%.*]] = sub i64 [[CUR_LEN]], [[CUR_OFFSET]]
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP0]], i64 [[REMAINING_BYTES]])
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test_fake_vla2(struct_fake_vla2 *s, long index) {
  // can't set bounds here, have to trust the caller's bounds
  s->buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [1]') to remaining bytes (member is potential variable length array)}}
  return 0;
}

// CHECK-LABEL: @test_vla_c(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = zext i32 [[LEN:%.*]] to i64
// CHECK-NEXT:    [[VLA:%.*]] = alloca i32, i64 [[TMP0]], align 4, addrspace(200)
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32 addrspace(200)* [[VLA]] to i8 addrspace(200)*
// CHECK-NEXT:    [[CUR_OFFSET:%.*]] = call i64 @llvm.cheri.cap.offset.get.i64(i8 addrspace(200)* nonnull [[TMP1]])
// CHECK-NEXT:    [[CUR_LEN:%.*]] = call i64 @llvm.cheri.cap.length.get.i64(i8 addrspace(200)* nonnull [[TMP1]])
// CHECK-NEXT:    [[REMAINING_BYTES:%.*]] = sub i64 [[CUR_LEN]], [[CUR_OFFSET]]
// CHECK-NEXT:    [[TMP2:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP1]], i64 [[REMAINING_BYTES]])
// CHECK-NEXT:    call void @use_buf(i8 addrspace(200)* [[TMP2]]) #9
// CHECK-NEXT:    [[CUR_OFFSET4:%.*]] = call i64 @llvm.cheri.cap.offset.get.i64(i8 addrspace(200)* [[TMP2]])
// CHECK-NEXT:    [[CUR_LEN5:%.*]] = call i64 @llvm.cheri.cap.length.get.i64(i8 addrspace(200)* [[TMP2]])
// CHECK-NEXT:    [[REMAINING_BYTES6:%.*]] = sub i64 [[CUR_LEN5]], [[CUR_OFFSET4]]
// CHECK-NEXT:    [[TMP3:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP2]], i64 [[REMAINING_BYTES6]])
// CHECK-NEXT:    [[SUBSCRIPT_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP3]] to i32 addrspace(200)*
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i32, i32 addrspace(200)* [[SUBSCRIPT_WITH_BOUNDS]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[TMP4:%.*]] = load i32, i32 addrspace(200)* [[ARRAYIDX]], align 4, !tbaa !2
// CHECK-NEXT:    ret i32 [[TMP4]]
//
int test_vla_c(int len, long index) {
  int buf[len];
  use_buf(&buf);     // expected-remark{{setting bounds for pointer to 'int [len]' to remaining bytes (variable length array type)}}
  return buf[index]; // expected-remark{{setting bounds for array decay on 'int [len]' to remaining bytes (array decay on variable size type)}}
  // expected-remark@-1{{setting bounds for array subscript on 'int [len]' to remaining bytes (array subscript on variable size type)}}
  // TODO: should avoid setting bounds for array decay inside array subscript (or just optimize away the redundant csetbounds)
}

typedef signed char v4i8 __attribute__ ((vector_size(4)));
v4i8 global_vector = {1, 2, 3, 4};
typedef int ext_vector_size_int32_8 __attribute__((ext_vector_type(8)));

// No bounds on vector indexing:
// CHECK-LABEL: @test_vector(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32 [[V4_COERCE:%.*]] to <4 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast i64 [[V8_COERCE3:%.*]] to <2 x i32>
// CHECK-NEXT:    [[VECEXT:%.*]] = extractelement <4 x i8> [[TMP1]], i32 3
// CHECK-NEXT:    [[VECEXT3:%.*]] = extractelement <2 x i32> [[TMP2]], i32 1
// CHECK-NEXT:    [[CONV:%.*]] = sext i8 [[VECEXT]] to i32
// CHECK-NEXT:    [[ADD:%.*]] = add nsw i32 [[VECEXT3]], [[CONV]]
// CHECK-NEXT:    ret i32 [[ADD]]
//
int test_vector(v4i8 v4, ext_vector_size_int32_8 v8) {
  char c = v4[3];
  int i = v8[7];
  return c + i;
}

// Even with an arbitraty index (backend should guarantee that it is never out of bounds)
// CHECK-LABEL: @test_vector2(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast i32 [[V4_COERCE:%.*]] to <4 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast i64 [[V8_COERCE0:%.*]] to <2 x i32>
// CHECK-NEXT:    [[V8_0_VEC_EXPAND:%.*]] = shufflevector <2 x i32> [[TMP2]], <2 x i32> undef, <8 x i32> <i32 0, i32 1, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast i64 [[V8_COERCE1:%.*]] to <2 x i32>
// CHECK-NEXT:    [[V8_8_VEC_EXPAND:%.*]] = shufflevector <2 x i32> [[TMP3]], <2 x i32> undef, <8 x i32> <i32 undef, i32 undef, i32 0, i32 1, i32 undef, i32 undef, i32 undef, i32 undef>
// CHECK-NEXT:    [[V8_8_VECBLEND:%.*]] = shufflevector <8 x i32> [[V8_0_VEC_EXPAND]], <8 x i32> [[V8_8_VEC_EXPAND]], <8 x i32> <i32 0, i32 1, i32 10, i32 11, i32 undef, i32 undef, i32 undef, i32 undef>
// CHECK-NEXT:    [[TMP4:%.*]] = bitcast i64 [[V8_COERCE2:%.*]] to <2 x i32>
// CHECK-NEXT:    [[V8_16_VEC_EXPAND:%.*]] = shufflevector <2 x i32> [[TMP4]], <2 x i32> undef, <8 x i32> <i32 undef, i32 undef, i32 undef, i32 undef, i32 0, i32 1, i32 undef, i32 undef>
// CHECK-NEXT:    [[V8_16_VECBLEND:%.*]] = shufflevector <8 x i32> [[V8_8_VECBLEND]], <8 x i32> [[V8_16_VEC_EXPAND]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 12, i32 13, i32 undef, i32 undef>
// CHECK-NEXT:    [[TMP5:%.*]] = bitcast i64 [[V8_COERCE3:%.*]] to <2 x i32>
// CHECK-NEXT:    [[V8_24_VEC_EXPAND:%.*]] = shufflevector <2 x i32> [[TMP5]], <2 x i32> undef, <8 x i32> <i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 undef, i32 0, i32 1>
// CHECK-NEXT:    [[V8_24_VECBLEND:%.*]] = shufflevector <8 x i32> [[V8_16_VECBLEND]], <8 x i32> [[V8_24_VEC_EXPAND]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 14, i32 15>
// CHECK-NEXT:    [[VECEXT:%.*]] = extractelement <4 x i8> [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[VECEXT3:%.*]] = extractelement <8 x i32> [[V8_24_VECBLEND]], i64 [[INDEX]]
// CHECK-NEXT:    [[CONV:%.*]] = sext i8 [[VECEXT]] to i32
// CHECK-NEXT:    [[ADD:%.*]] = add nsw i32 [[VECEXT3]], [[CONV]]
// CHECK-NEXT:    ret i32 [[ADD]]
//
int test_vector2(long index, v4i8 v4, ext_vector_size_int32_8 v8) {
  char c = v4[index];
  int i = v8[index];
  return c + i;
}

// CHECK-LABEL: @test_ptr_to_array1(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [10 x i32] addrspace(200)* [[ARRAY:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 40)
// CHECK-NEXT:    [[SUBSCRIPT_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP1]] to [10 x i32] addrspace(200)*
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [10 x i32], [10 x i32] addrspace(200)* [[SUBSCRIPT_WITH_BOUNDS]], i64 0, i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[TMP2:%.*]] = load i32, i32 addrspace(200)* [[ARRAYIDX]], align 4, !tbaa !2
// CHECK-NEXT:    ret i32 [[TMP2]]
//
int test_ptr_to_array1(int (*array)[10], long index) {
  return (*array)[index]; // expected-remark{{setting sub-object bounds for array subscript on 'int [10]' to 40 bytes}}
}
// CHECK-LABEL: @test_ptr_to_array2(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARRAYDECAY:%.*]] = getelementptr inbounds [10 x i32], [10 x i32] addrspace(200)* [[ARRAY:%.*]], i64 [[INDEX:%.*]], i64 0
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast i32 addrspace(200)* [[ARRAYDECAY]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 40)
// CHECK-NEXT:    [[DECAY_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP1]] to i32 addrspace(200)*
// CHECK-NEXT:    ret i32 addrspace(200)* [[DECAY_WITH_BOUNDS]]
//
int* test_ptr_to_array2(int (*array)[10], long index) {
  return array[index]; // expected-remark{{not setting bounds for array subscript on 'int (* __capability)[10]' (array subscript on non-array type)}}
  // expected-remark@-1{{setting sub-object bounds for array decay on 'int [10]' to 40 bytes}}
  // TODO: should avoid setting bounds for array decay inside array subscript (or just optimize away the redundant csetbounds)
}
// CHECK-LABEL: @test_ptr_to_array3(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [10 x [5 x i32]] addrspace(200)* [[ARRAY:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 200)
// CHECK-NEXT:    [[SUBSCRIPT_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP1]] to [10 x [5 x i32]] addrspace(200)*
// CHECK-NEXT:    [[ARRAYDECAY:%.*]] = getelementptr inbounds [10 x [5 x i32]], [10 x [5 x i32]] addrspace(200)* [[SUBSCRIPT_WITH_BOUNDS]], i64 0, i64 [[INDEX:%.*]], i64 0
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast i32 addrspace(200)* [[ARRAYDECAY]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP3:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP2]], i64 20)
// CHECK-NEXT:    [[DECAY_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP3]] to i32 addrspace(200)*
// CHECK-NEXT:    ret i32 addrspace(200)* [[DECAY_WITH_BOUNDS]]
//
int* test_ptr_to_array3(int (*array)[10][5], long index) {
  return (*array)[index]; // expected-remark{{setting sub-object bounds for array subscript on 'int [10][5]' to 200 bytes}}
  // expected-remark@-1{{setting sub-object bounds for array decay on 'int [5]' to 20 bytes}}
  // TODO: should avoid setting bounds for array decay inside array subscript (or just optimize away the redundant csetbounds)
}
// CHECK-LABEL: @test_ptr_to_array4(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [10 x [5 x i32]], [10 x [5 x i32]] addrspace(200)* [[ARRAY:%.*]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [10 x [5 x i32]] addrspace(200)* [[ARRAYIDX]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 200)
// CHECK-NEXT:    [[SUBSCRIPT_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP1]] to [10 x [5 x i32]] addrspace(200)*
// CHECK-NEXT:    [[ARRAYDECAY:%.*]] = getelementptr inbounds [10 x [5 x i32]], [10 x [5 x i32]] addrspace(200)* [[SUBSCRIPT_WITH_BOUNDS]], i64 0, i64 [[INDEX]], i64 0
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast i32 addrspace(200)* [[ARRAYDECAY]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP3:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP2]], i64 20)
// CHECK-NEXT:    [[DECAY_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP3]] to i32 addrspace(200)*
// CHECK-NEXT:    ret i32 addrspace(200)* [[DECAY_WITH_BOUNDS]]
//
int* test_ptr_to_array4(int (*array)[10][5], long index) {
  return array[index][index];
  // expected-remark@-1{{not setting bounds for array subscript on 'int (* __capability)[10][5]' (array subscript on non-array type)}}
  // expected-remark@-2{{setting sub-object bounds for array subscript on 'int [10][5]' to 200 bytes}}
  // expected-remark@-3{{setting sub-object bounds for array decay on 'int [5]' to 20 bytes}}
  // TODO: should avoid setting bounds for array decay inside array subscript (or just optimize away the redundant csetbounds)
}
// CHECK-LABEL: @test_ptr_to_array5(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [10 x [5 x i32]] addrspace(200)* [[ARRAY:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 200)
// CHECK-NEXT:    [[SUBSCRIPT_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP1]] to [10 x [5 x i32]] addrspace(200)*
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [10 x [5 x i32]], [10 x [5 x i32]] addrspace(200)* [[SUBSCRIPT_WITH_BOUNDS]], i64 0, i64 [[INDEX:%.*]]
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast [5 x i32] addrspace(200)* [[ARRAYIDX]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP3:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP2]], i64 20)
// CHECK-NEXT:    [[SUBSCRIPT_WITH_BOUNDS1:%.*]] = bitcast i8 addrspace(200)* [[TMP3]] to [5 x i32] addrspace(200)*
// CHECK-NEXT:    [[ARRAYIDX2:%.*]] = getelementptr inbounds [5 x i32], [5 x i32] addrspace(200)* [[SUBSCRIPT_WITH_BOUNDS1]], i64 0, i64 [[INDEX]]
// CHECK-NEXT:    [[TMP4:%.*]] = load i32, i32 addrspace(200)* [[ARRAYIDX2]], align 4, !tbaa !2
// CHECK-NEXT:    ret i32 [[TMP4]]
//
int test_ptr_to_array5(int (*array)[10][5], long index) {
  return (*array)[index][index];
  // expected-remark@-1{{setting sub-object bounds for array subscript on 'int [10][5]' to 200 bytes}}
  // expected-remark@-2{{setting sub-object bounds for array subscript on 'int [5]' to 20 bytes}}
}

// TODO: C++ array references
// Some tests derived from BODiagsuite (but modified to handle a few more complicated cases):

// Overflow into next struct member
typedef struct {
  char buf[10];
  int int_field;
} my_struct21;

// CHECK-LABEL: @test21a(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT21:%.*]], [[STRUCT_MY_STRUCT21]] addrspace(200)* [[S:%.*]], i64 0, i32 0, i64 0
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test21a(my_struct21 *s, long index) {
  s->buf[index] = 'A';  // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  return 0;
}
// CHECK-LABEL: @test21b(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca { i64, i64 }, align 8, addrspace(200)
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast { i64, i64 } addrspace(200)* [[RETVAL]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    [[DOTFCA_0_GEP:%.*]] = getelementptr inbounds { i64, i64 }, { i64, i64 } addrspace(200)* [[RETVAL]], i64 0, i32 0
// CHECK-NEXT:    [[DOTFCA_0_LOAD:%.*]] = load i64, i64 addrspace(200)* [[DOTFCA_0_GEP]], align 8
// CHECK-NEXT:    [[DOTFCA_0_INSERT:%.*]] = insertvalue { i64, i64 } undef, i64 [[DOTFCA_0_LOAD]], 0
// CHECK-NEXT:    [[DOTFCA_1_GEP:%.*]] = getelementptr inbounds { i64, i64 }, { i64, i64 } addrspace(200)* [[RETVAL]], i64 0, i32 1
// CHECK-NEXT:    [[DOTFCA_1_LOAD:%.*]] = load i64, i64 addrspace(200)* [[DOTFCA_1_GEP]], align 8
// CHECK-NEXT:    [[DOTFCA_1_INSERT:%.*]] = insertvalue { i64, i64 } [[DOTFCA_0_INSERT]], i64 [[DOTFCA_1_LOAD]], 1
// CHECK-NEXT:    ret { i64, i64 } [[DOTFCA_1_INSERT]]
//
my_struct21 test21b(my_struct21 *s, long index) {
  my_struct21 s2;
  s2.buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  return s2; // prevent s2 from being optimzed out
}

// CHECK-LABEL: @test21c(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT21:%.*]], [[STRUCT_MY_STRUCT21]] addrspace(200)* [[S:%.*]], i64 0, i32 0, i64 0
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX1:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX1]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test21c(my_struct21 *s, long index) {
  // Should not set bounds on s here, but should do for buf
  s[0].buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  // expected-remark@-1{{not setting bounds for array subscript on 'my_struct21 * __capability' (array subscript on non-array type)}}
  return 0;
}

// CHECK-LABEL: @test21d(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT21:%.*]], [[STRUCT_MY_STRUCT21]] addrspace(200)* [[S:%.*]], i64 0, i32 0, i64 0
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test21d(my_struct21 *s, long index) {
  // Should not set bounds on s here, but should do for buf
  (*s).buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  return 0;
}

// Overflow into padding
typedef struct {
  int int_field;
  char buf[10];
} my_struct22;
// CHECK-LABEL: @test22a(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT22:%.*]], [[STRUCT_MY_STRUCT22]] addrspace(200)* [[S:%.*]], i64 0, i32 1, i64 0
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test22a(my_struct22 *s, long index) {
  s->buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  return 0;
}
// CHECK-LABEL: @test22b(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca { i64, i64 }, align 8, addrspace(200)
// CHECK-NEXT:    [[TMPCAST:%.*]] = bitcast { i64, i64 } addrspace(200)* [[RETVAL]] to [[STRUCT_MY_STRUCT22:%.*]] addrspace(200)*
// CHECK-NEXT:    [[TMP0:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT22]], [[STRUCT_MY_STRUCT22]] addrspace(200)* [[TMPCAST]], i64 0, i32 1, i64 0
// CHECK-NEXT:    [[TMP1:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    [[DOTFCA_0_GEP:%.*]] = getelementptr inbounds { i64, i64 }, { i64, i64 } addrspace(200)* [[RETVAL]], i64 0, i32 0
// CHECK-NEXT:    [[DOTFCA_0_LOAD:%.*]] = load i64, i64 addrspace(200)* [[DOTFCA_0_GEP]], align 8
// CHECK-NEXT:    [[DOTFCA_0_INSERT:%.*]] = insertvalue { i64, i64 } undef, i64 [[DOTFCA_0_LOAD]], 0
// CHECK-NEXT:    [[DOTFCA_1_GEP:%.*]] = getelementptr inbounds { i64, i64 }, { i64, i64 } addrspace(200)* [[RETVAL]], i64 0, i32 1
// CHECK-NEXT:    [[DOTFCA_1_LOAD:%.*]] = load i64, i64 addrspace(200)* [[DOTFCA_1_GEP]], align 8
// CHECK-NEXT:    [[DOTFCA_1_INSERT:%.*]] = insertvalue { i64, i64 } [[DOTFCA_0_INSERT]], i64 [[DOTFCA_1_LOAD]], 1
// CHECK-NEXT:    ret { i64, i64 } [[DOTFCA_1_INSERT]]
//
my_struct22 test22b(long index) {
  my_struct22 s2;
  s2.buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  return s2; // prevent s2 from being optimzed out
}

// Overflow in union member
typedef union {
  char buf[10];
  long otherbuf[10];
} my_union25;

// CHECK-LABEL: @test25a(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [[UNION_MY_UNION25:%.*]] addrspace(200)* [[U:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test25a(my_union25 *u, long index) {
  u->buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  return 0;
}
// CHECK-LABEL: @test25b(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [[UNION_MY_UNION25:%.*]] addrspace(200)* [[AGG_RESULT:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret void
//
my_union25 test25b(long index) {
  my_union25 u2;
  u2.buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  return u2; // prevent u2 from being optimzed out
}
// CHECK-LABEL: @test25c(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [[UNION_MY_UNION25:%.*]] addrspace(200)* [[U:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX1:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX1]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test25c(my_union25 *u, long index) {
  // Should not set bounds on u here, but should do for buf
  u[0].buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  // expected-remark@-1{{not setting bounds for array subscript on 'my_union25 * __capability' (array subscript on non-array type)}}
  return 0;
}
// CHECK-LABEL: @test25d(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast [[UNION_MY_UNION25:%.*]] addrspace(200)* [[U:%.*]] to i8 addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP0]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test25d(my_union25 *u, long index) {
  // Should not set bounds on u here, but should do for buf
  (*u).buf[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf' (array subscript on 'char [10]') to 10 bytes}}
  return 0;
}

// simple multidimensional array overflow
typedef struct {
  char buf1[10];
  char buf2[10];
} my_struct28;

// CHECK-LABEL: @test28a(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARRAY23:%.*]] = alloca [100 x i8], align 1, addrspace(200)
// CHECK-NEXT:    [[ARRAY23_SUB:%.*]] = getelementptr inbounds [100 x i8], [100 x i8] addrspace(200)* [[ARRAY23]], i64 0, i64 0
// CHECK-NEXT:    call void @llvm.lifetime.start.p200i8(i64 100, i8 addrspace(200)* nonnull [[ARRAY23_SUB]]) #9
// CHECK-NEXT:    [[TMP0:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[ARRAY23_SUB]], i64 100)
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [100 x i8], [100 x i8] addrspace(200)* [[ARRAY23]], i64 0, i64 80
// CHECK-NEXT:    [[TMP1:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[ARRAYIDX]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX2:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP1]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX2]], align 1, !tbaa !9
// CHECK-NEXT:    call void @use_buf(i8 addrspace(200)* [[TMP0]]) #9
// CHECK-NEXT:    call void @llvm.lifetime.end.p200i8(i64 100, i8 addrspace(200)* nonnull [[ARRAY23_SUB]]) #9
// CHECK-NEXT:    ret i32 0
//
int test28a(long index) {
  my_struct28 array2[5];
  array2[4].buf1[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf1' (array subscript on 'char [10]') to 10 bytes}}
  // expected-remark@-1{{setting bounds for array subscript on 'my_struct28 [5]' to 100 bytes}}
  use_buf(&array2); // prevent array from being optimized away
  // expected-remark@-1{{setting bounds for pointer to 'my_struct28 [5]' to 100 bytes}}
  return 0;
}

// CHECK-LABEL: @test28b(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT28:%.*]] addrspace(200)*, [[STRUCT_MY_STRUCT28]] addrspace(200)* addrspace(200)* [[ARRAY1:%.*]], i64 4
// CHECK-NEXT:    [[TMP0:%.*]] = load [[STRUCT_MY_STRUCT28]] addrspace(200)*, [[STRUCT_MY_STRUCT28]] addrspace(200)* addrspace(200)* [[ARRAYIDX]], align 16, !tbaa !14
// CHECK-NEXT:    [[TMP1:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT28]], [[STRUCT_MY_STRUCT28]] addrspace(200)* [[TMP0]], i64 0, i32 0, i64 0
// CHECK-NEXT:    [[TMP2:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP1]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX1:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP2]], i64 [[INDEX:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX1]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test28b(my_struct28 **array1, long index) {
  array1[4]->buf1[index] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf1' (array subscript on 'char [10]') to 10 bytes}}
  // expected-remark@-1{{not setting bounds for array subscript on 'my_struct28 * __capability * __capability' (array subscript on non-array type)}}
  return 0;
}

// CHECK-LABEL: @test28c(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARRAY23:%.*]] = alloca [100 x i8], align 1, addrspace(200)
// CHECK-NEXT:    [[ARRAY23_SUB:%.*]] = getelementptr inbounds [100 x i8], [100 x i8] addrspace(200)* [[ARRAY23]], i64 0, i64 0
// CHECK-NEXT:    call void @llvm.lifetime.start.p200i8(i64 100, i8 addrspace(200)* nonnull [[ARRAY23_SUB]]) #9
// CHECK-NEXT:    [[TMP0:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* nonnull [[ARRAY23_SUB]], i64 100)
// CHECK-NEXT:    [[SUBSCRIPT_WITH_BOUNDS:%.*]] = bitcast i8 addrspace(200)* [[TMP0]] to [5 x %struct.my_struct28] addrspace(200)*
// CHECK-NEXT:    [[TMP1:%.*]] = getelementptr inbounds [5 x %struct.my_struct28], [5 x %struct.my_struct28] addrspace(200)* [[SUBSCRIPT_WITH_BOUNDS]], i64 0, i64 [[INDEX1:%.*]], i32 0, i64 0
// CHECK-NEXT:    [[TMP2:%.*]] = call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP1]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX2:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP2]], i64 [[INDEX2:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX2]], align 1, !tbaa !9
// CHECK-NEXT:    call void @use_buf(i8 addrspace(200)* [[TMP0]]) #9
// CHECK-NEXT:    call void @llvm.lifetime.end.p200i8(i64 100, i8 addrspace(200)* nonnull [[ARRAY23_SUB]]) #9
// CHECK-NEXT:    ret i32 0
//
int test28c(long index1, long index2) {
  my_struct28 array2[5];
  array2[index1].buf1[index2] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf1' (array subscript on 'char [10]') to 10 bytes}}
  // expected-remark@-1{{setting bounds for array subscript on 'my_struct28 [5]' to 100 bytes}}
  use_buf(&array2); // prevent array from being optimized away
  // expected-remark@-1{{setting bounds for pointer to 'my_struct28 [5]' to 100 bytes}}
  return 0;
}

// CHECK-LABEL: @test28d(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARRAYIDX:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT28:%.*]] addrspace(200)*, [[STRUCT_MY_STRUCT28]] addrspace(200)* addrspace(200)* [[ARRAY1:%.*]], i64 [[INDEX1:%.*]]
// CHECK-NEXT:    [[TMP0:%.*]] = load [[STRUCT_MY_STRUCT28]] addrspace(200)*, [[STRUCT_MY_STRUCT28]] addrspace(200)* addrspace(200)* [[ARRAYIDX]], align 16, !tbaa !14
// CHECK-NEXT:    [[TMP1:%.*]] = getelementptr inbounds [[STRUCT_MY_STRUCT28]], [[STRUCT_MY_STRUCT28]] addrspace(200)* [[TMP0]], i64 0, i32 0, i64 0
// CHECK-NEXT:    [[TMP2:%.*]] = tail call i8 addrspace(200)* @llvm.cheri.cap.bounds.set.i64(i8 addrspace(200)* [[TMP1]], i64 10)
// CHECK-NEXT:    [[ARRAYIDX1:%.*]] = getelementptr inbounds i8, i8 addrspace(200)* [[TMP2]], i64 [[INDEX2:%.*]]
// CHECK-NEXT:    store i8 65, i8 addrspace(200)* [[ARRAYIDX1]], align 1, !tbaa !9
// CHECK-NEXT:    ret i32 0
//
int test28d(my_struct28 **array1, long index1, long index2) {
  array1[index1]->buf1[index2] = 'A'; // expected-remark{{setting sub-object bounds for field 'buf1' (array subscript on 'char [10]') to 10 bytes}}
  // expected-remark@-1{{not setting bounds for array subscript on 'my_struct28 * __capability * __capability' (array subscript on non-array type)}}
  return 0;
}
