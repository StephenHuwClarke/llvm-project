; NOTE: Assertions have been autogenerated by utils/update_mir_test_checks.py
; RUN: llc -global-isel -mtriple=amdgcn-mesa-mesa3d -mcpu=tonga -stop-after=legalizer -o - %s | FileCheck -check-prefix=GCN %s

define amdgpu_ps float @image_load_f32(<8 x i32> inreg %rsrc, i32 %s, i32 %t) {
  ; GCN-LABEL: name: image_load_f32
  ; GCN: bb.1 (%ir-block.0):
  ; GCN:   liveins: $sgpr2, $sgpr3, $sgpr4, $sgpr5, $sgpr6, $sgpr7, $sgpr8, $sgpr9, $vgpr0, $vgpr1
  ; GCN:   [[COPY:%[0-9]+]]:_(s32) = COPY $sgpr2
  ; GCN:   [[COPY1:%[0-9]+]]:_(s32) = COPY $sgpr3
  ; GCN:   [[COPY2:%[0-9]+]]:_(s32) = COPY $sgpr4
  ; GCN:   [[COPY3:%[0-9]+]]:_(s32) = COPY $sgpr5
  ; GCN:   [[COPY4:%[0-9]+]]:_(s32) = COPY $sgpr6
  ; GCN:   [[COPY5:%[0-9]+]]:_(s32) = COPY $sgpr7
  ; GCN:   [[COPY6:%[0-9]+]]:_(s32) = COPY $sgpr8
  ; GCN:   [[COPY7:%[0-9]+]]:_(s32) = COPY $sgpr9
  ; GCN:   [[COPY8:%[0-9]+]]:_(s32) = COPY $vgpr0
  ; GCN:   [[COPY9:%[0-9]+]]:_(s32) = COPY $vgpr1
  ; GCN:   [[BUILD_VECTOR:%[0-9]+]]:_(<8 x s32>) = G_BUILD_VECTOR [[COPY]](s32), [[COPY1]](s32), [[COPY2]](s32), [[COPY3]](s32), [[COPY4]](s32), [[COPY5]](s32), [[COPY6]](s32), [[COPY7]](s32)
  ; GCN:   [[BUILD_VECTOR1:%[0-9]+]]:_(<2 x s32>) = G_BUILD_VECTOR [[COPY8]](s32), [[COPY9]](s32)
  ; GCN:   [[AMDGPU_INTRIN_IMAGE_LOAD:%[0-9]+]]:_(s32) = G_AMDGPU_INTRIN_IMAGE_LOAD intrinsic(@llvm.amdgcn.image.load.2d), 1, [[BUILD_VECTOR1]](<2 x s32>), $noreg, [[BUILD_VECTOR]](<8 x s32>), 0, 0 :: (dereferenceable load 4 from custom "TargetCustom8")
  ; GCN:   $vgpr0 = COPY [[AMDGPU_INTRIN_IMAGE_LOAD]](s32)
  ; GCN:   SI_RETURN_TO_EPILOG implicit $vgpr0
  %tex = call float @llvm.amdgcn.image.load.2d.f32.i32(i32 1, i32 %s, i32 %t, <8 x i32> %rsrc, i32 0, i32 0)
  ret float %tex
}

define amdgpu_ps <2 x float> @image_load_v2f32(<8 x i32> inreg %rsrc, i32 %s, i32 %t) {
  ; GCN-LABEL: name: image_load_v2f32
  ; GCN: bb.1 (%ir-block.0):
  ; GCN:   liveins: $sgpr2, $sgpr3, $sgpr4, $sgpr5, $sgpr6, $sgpr7, $sgpr8, $sgpr9, $vgpr0, $vgpr1
  ; GCN:   [[COPY:%[0-9]+]]:_(s32) = COPY $sgpr2
  ; GCN:   [[COPY1:%[0-9]+]]:_(s32) = COPY $sgpr3
  ; GCN:   [[COPY2:%[0-9]+]]:_(s32) = COPY $sgpr4
  ; GCN:   [[COPY3:%[0-9]+]]:_(s32) = COPY $sgpr5
  ; GCN:   [[COPY4:%[0-9]+]]:_(s32) = COPY $sgpr6
  ; GCN:   [[COPY5:%[0-9]+]]:_(s32) = COPY $sgpr7
  ; GCN:   [[COPY6:%[0-9]+]]:_(s32) = COPY $sgpr8
  ; GCN:   [[COPY7:%[0-9]+]]:_(s32) = COPY $sgpr9
  ; GCN:   [[COPY8:%[0-9]+]]:_(s32) = COPY $vgpr0
  ; GCN:   [[COPY9:%[0-9]+]]:_(s32) = COPY $vgpr1
  ; GCN:   [[BUILD_VECTOR:%[0-9]+]]:_(<8 x s32>) = G_BUILD_VECTOR [[COPY]](s32), [[COPY1]](s32), [[COPY2]](s32), [[COPY3]](s32), [[COPY4]](s32), [[COPY5]](s32), [[COPY6]](s32), [[COPY7]](s32)
  ; GCN:   [[BUILD_VECTOR1:%[0-9]+]]:_(<2 x s32>) = G_BUILD_VECTOR [[COPY8]](s32), [[COPY9]](s32)
  ; GCN:   [[AMDGPU_INTRIN_IMAGE_LOAD:%[0-9]+]]:_(<2 x s32>) = G_AMDGPU_INTRIN_IMAGE_LOAD intrinsic(@llvm.amdgcn.image.load.2d), 3, [[BUILD_VECTOR1]](<2 x s32>), $noreg, [[BUILD_VECTOR]](<8 x s32>), 0, 0 :: (dereferenceable load 8 from custom "TargetCustom8")
  ; GCN:   [[UV:%[0-9]+]]:_(s32), [[UV1:%[0-9]+]]:_(s32) = G_UNMERGE_VALUES [[AMDGPU_INTRIN_IMAGE_LOAD]](<2 x s32>)
  ; GCN:   $vgpr0 = COPY [[UV]](s32)
  ; GCN:   $vgpr1 = COPY [[UV1]](s32)
  ; GCN:   SI_RETURN_TO_EPILOG implicit $vgpr0, implicit $vgpr1
  %tex = call <2 x float> @llvm.amdgcn.image.load.2d.v2f32.i32(i32 3, i32 %s, i32 %t, <8 x i32> %rsrc, i32 0, i32 0)
  ret <2 x float> %tex
}

define amdgpu_ps <3 x float> @image_load_v3f32(<8 x i32> inreg %rsrc, i32 %s, i32 %t) {
  ; GCN-LABEL: name: image_load_v3f32
  ; GCN: bb.1 (%ir-block.0):
  ; GCN:   liveins: $sgpr2, $sgpr3, $sgpr4, $sgpr5, $sgpr6, $sgpr7, $sgpr8, $sgpr9, $vgpr0, $vgpr1
  ; GCN:   [[COPY:%[0-9]+]]:_(s32) = COPY $sgpr2
  ; GCN:   [[COPY1:%[0-9]+]]:_(s32) = COPY $sgpr3
  ; GCN:   [[COPY2:%[0-9]+]]:_(s32) = COPY $sgpr4
  ; GCN:   [[COPY3:%[0-9]+]]:_(s32) = COPY $sgpr5
  ; GCN:   [[COPY4:%[0-9]+]]:_(s32) = COPY $sgpr6
  ; GCN:   [[COPY5:%[0-9]+]]:_(s32) = COPY $sgpr7
  ; GCN:   [[COPY6:%[0-9]+]]:_(s32) = COPY $sgpr8
  ; GCN:   [[COPY7:%[0-9]+]]:_(s32) = COPY $sgpr9
  ; GCN:   [[COPY8:%[0-9]+]]:_(s32) = COPY $vgpr0
  ; GCN:   [[COPY9:%[0-9]+]]:_(s32) = COPY $vgpr1
  ; GCN:   [[BUILD_VECTOR:%[0-9]+]]:_(<8 x s32>) = G_BUILD_VECTOR [[COPY]](s32), [[COPY1]](s32), [[COPY2]](s32), [[COPY3]](s32), [[COPY4]](s32), [[COPY5]](s32), [[COPY6]](s32), [[COPY7]](s32)
  ; GCN:   [[BUILD_VECTOR1:%[0-9]+]]:_(<2 x s32>) = G_BUILD_VECTOR [[COPY8]](s32), [[COPY9]](s32)
  ; GCN:   [[AMDGPU_INTRIN_IMAGE_LOAD:%[0-9]+]]:_(<3 x s32>) = G_AMDGPU_INTRIN_IMAGE_LOAD intrinsic(@llvm.amdgcn.image.load.2d), 7, [[BUILD_VECTOR1]](<2 x s32>), $noreg, [[BUILD_VECTOR]](<8 x s32>), 0, 0 :: (dereferenceable load 12 from custom "TargetCustom8", align 16)
  ; GCN:   [[UV:%[0-9]+]]:_(s32), [[UV1:%[0-9]+]]:_(s32), [[UV2:%[0-9]+]]:_(s32) = G_UNMERGE_VALUES [[AMDGPU_INTRIN_IMAGE_LOAD]](<3 x s32>)
  ; GCN:   $vgpr0 = COPY [[UV]](s32)
  ; GCN:   $vgpr1 = COPY [[UV1]](s32)
  ; GCN:   $vgpr2 = COPY [[UV2]](s32)
  ; GCN:   SI_RETURN_TO_EPILOG implicit $vgpr0, implicit $vgpr1, implicit $vgpr2
  %tex = call <3 x float> @llvm.amdgcn.image.load.2d.v3f32.i32(i32 7, i32 %s, i32 %t, <8 x i32> %rsrc, i32 0, i32 0)
  ret <3 x float> %tex
}

define amdgpu_ps <4 x float> @image_load_v4f32(<8 x i32> inreg %rsrc, i32 %s, i32 %t) {
  ; GCN-LABEL: name: image_load_v4f32
  ; GCN: bb.1 (%ir-block.0):
  ; GCN:   liveins: $sgpr2, $sgpr3, $sgpr4, $sgpr5, $sgpr6, $sgpr7, $sgpr8, $sgpr9, $vgpr0, $vgpr1
  ; GCN:   [[COPY:%[0-9]+]]:_(s32) = COPY $sgpr2
  ; GCN:   [[COPY1:%[0-9]+]]:_(s32) = COPY $sgpr3
  ; GCN:   [[COPY2:%[0-9]+]]:_(s32) = COPY $sgpr4
  ; GCN:   [[COPY3:%[0-9]+]]:_(s32) = COPY $sgpr5
  ; GCN:   [[COPY4:%[0-9]+]]:_(s32) = COPY $sgpr6
  ; GCN:   [[COPY5:%[0-9]+]]:_(s32) = COPY $sgpr7
  ; GCN:   [[COPY6:%[0-9]+]]:_(s32) = COPY $sgpr8
  ; GCN:   [[COPY7:%[0-9]+]]:_(s32) = COPY $sgpr9
  ; GCN:   [[COPY8:%[0-9]+]]:_(s32) = COPY $vgpr0
  ; GCN:   [[COPY9:%[0-9]+]]:_(s32) = COPY $vgpr1
  ; GCN:   [[BUILD_VECTOR:%[0-9]+]]:_(<8 x s32>) = G_BUILD_VECTOR [[COPY]](s32), [[COPY1]](s32), [[COPY2]](s32), [[COPY3]](s32), [[COPY4]](s32), [[COPY5]](s32), [[COPY6]](s32), [[COPY7]](s32)
  ; GCN:   [[BUILD_VECTOR1:%[0-9]+]]:_(<2 x s32>) = G_BUILD_VECTOR [[COPY8]](s32), [[COPY9]](s32)
  ; GCN:   [[AMDGPU_INTRIN_IMAGE_LOAD:%[0-9]+]]:_(<4 x s32>) = G_AMDGPU_INTRIN_IMAGE_LOAD intrinsic(@llvm.amdgcn.image.load.2d), 15, [[BUILD_VECTOR1]](<2 x s32>), $noreg, [[BUILD_VECTOR]](<8 x s32>), 0, 0 :: (dereferenceable load 16 from custom "TargetCustom8")
  ; GCN:   [[UV:%[0-9]+]]:_(s32), [[UV1:%[0-9]+]]:_(s32), [[UV2:%[0-9]+]]:_(s32), [[UV3:%[0-9]+]]:_(s32) = G_UNMERGE_VALUES [[AMDGPU_INTRIN_IMAGE_LOAD]](<4 x s32>)
  ; GCN:   $vgpr0 = COPY [[UV]](s32)
  ; GCN:   $vgpr1 = COPY [[UV1]](s32)
  ; GCN:   $vgpr2 = COPY [[UV2]](s32)
  ; GCN:   $vgpr3 = COPY [[UV3]](s32)
  ; GCN:   SI_RETURN_TO_EPILOG implicit $vgpr0, implicit $vgpr1, implicit $vgpr2, implicit $vgpr3
  %tex = call <4 x float> @llvm.amdgcn.image.load.2d.v4f32.i32(i32 15, i32 %s, i32 %t, <8 x i32> %rsrc, i32 0, i32 0)
  ret <4 x float> %tex
}

define amdgpu_ps float @image_load_tfe_f32(<8 x i32> inreg %rsrc, i32 %s, i32 %t) {
  ; GCN-LABEL: name: image_load_tfe_f32
  ; GCN: bb.1 (%ir-block.0):
  ; GCN:   liveins: $sgpr2, $sgpr3, $sgpr4, $sgpr5, $sgpr6, $sgpr7, $sgpr8, $sgpr9, $vgpr0, $vgpr1
  ; GCN:   [[COPY:%[0-9]+]]:_(s32) = COPY $sgpr2
  ; GCN:   [[COPY1:%[0-9]+]]:_(s32) = COPY $sgpr3
  ; GCN:   [[COPY2:%[0-9]+]]:_(s32) = COPY $sgpr4
  ; GCN:   [[COPY3:%[0-9]+]]:_(s32) = COPY $sgpr5
  ; GCN:   [[COPY4:%[0-9]+]]:_(s32) = COPY $sgpr6
  ; GCN:   [[COPY5:%[0-9]+]]:_(s32) = COPY $sgpr7
  ; GCN:   [[COPY6:%[0-9]+]]:_(s32) = COPY $sgpr8
  ; GCN:   [[COPY7:%[0-9]+]]:_(s32) = COPY $sgpr9
  ; GCN:   [[COPY8:%[0-9]+]]:_(s32) = COPY $vgpr0
  ; GCN:   [[COPY9:%[0-9]+]]:_(s32) = COPY $vgpr1
  ; GCN:   [[BUILD_VECTOR:%[0-9]+]]:_(<8 x s32>) = G_BUILD_VECTOR [[COPY]](s32), [[COPY1]](s32), [[COPY2]](s32), [[COPY3]](s32), [[COPY4]](s32), [[COPY5]](s32), [[COPY6]](s32), [[COPY7]](s32)
  ; GCN:   [[DEF:%[0-9]+]]:_(p1) = G_IMPLICIT_DEF
  ; GCN:   [[BUILD_VECTOR1:%[0-9]+]]:_(<2 x s32>) = G_BUILD_VECTOR [[COPY8]](s32), [[COPY9]](s32)
  ; GCN:   [[AMDGPU_INTRIN_IMAGE_LOAD:%[0-9]+]]:_(<2 x s32>) = G_AMDGPU_INTRIN_IMAGE_LOAD intrinsic(@llvm.amdgcn.image.load.2d), 1, [[BUILD_VECTOR1]](<2 x s32>), $noreg, [[BUILD_VECTOR]](<8 x s32>), 1, 0 :: (dereferenceable load 4 from custom "TargetCustom8")
  ; GCN:   [[UV:%[0-9]+]]:_(s32), [[UV1:%[0-9]+]]:_(s32) = G_UNMERGE_VALUES [[AMDGPU_INTRIN_IMAGE_LOAD]](<2 x s32>)
  ; GCN:   G_STORE [[UV1]](s32), [[DEF]](p1) :: (store 4 into `i32 addrspace(1)* undef`, addrspace 1)
  ; GCN:   $vgpr0 = COPY [[UV]](s32)
  ; GCN:   SI_RETURN_TO_EPILOG implicit $vgpr0
  %res = call { float, i32 } @llvm.amdgcn.image.load.2d.sl_f32i32s.i32(i32 1, i32 %s, i32 %t, <8 x i32> %rsrc, i32 1, i32 0)
  %tex = extractvalue { float, i32 } %res, 0
  %tfe = extractvalue { float, i32 } %res, 1
  store i32 %tfe, i32 addrspace(1)* undef
  ret float %tex
}

define amdgpu_ps <2 x float> @image_load_tfe_v2f32(<8 x i32> inreg %rsrc, i32 %s, i32 %t) {
  ; GCN-LABEL: name: image_load_tfe_v2f32
  ; GCN: bb.1 (%ir-block.0):
  ; GCN:   liveins: $sgpr2, $sgpr3, $sgpr4, $sgpr5, $sgpr6, $sgpr7, $sgpr8, $sgpr9, $vgpr0, $vgpr1
  ; GCN:   [[COPY:%[0-9]+]]:_(s32) = COPY $sgpr2
  ; GCN:   [[COPY1:%[0-9]+]]:_(s32) = COPY $sgpr3
  ; GCN:   [[COPY2:%[0-9]+]]:_(s32) = COPY $sgpr4
  ; GCN:   [[COPY3:%[0-9]+]]:_(s32) = COPY $sgpr5
  ; GCN:   [[COPY4:%[0-9]+]]:_(s32) = COPY $sgpr6
  ; GCN:   [[COPY5:%[0-9]+]]:_(s32) = COPY $sgpr7
  ; GCN:   [[COPY6:%[0-9]+]]:_(s32) = COPY $sgpr8
  ; GCN:   [[COPY7:%[0-9]+]]:_(s32) = COPY $sgpr9
  ; GCN:   [[COPY8:%[0-9]+]]:_(s32) = COPY $vgpr0
  ; GCN:   [[COPY9:%[0-9]+]]:_(s32) = COPY $vgpr1
  ; GCN:   [[BUILD_VECTOR:%[0-9]+]]:_(<8 x s32>) = G_BUILD_VECTOR [[COPY]](s32), [[COPY1]](s32), [[COPY2]](s32), [[COPY3]](s32), [[COPY4]](s32), [[COPY5]](s32), [[COPY6]](s32), [[COPY7]](s32)
  ; GCN:   [[DEF:%[0-9]+]]:_(p1) = G_IMPLICIT_DEF
  ; GCN:   [[BUILD_VECTOR1:%[0-9]+]]:_(<2 x s32>) = G_BUILD_VECTOR [[COPY8]](s32), [[COPY9]](s32)
  ; GCN:   [[AMDGPU_INTRIN_IMAGE_LOAD:%[0-9]+]]:_(<3 x s32>) = G_AMDGPU_INTRIN_IMAGE_LOAD intrinsic(@llvm.amdgcn.image.load.2d), 3, [[BUILD_VECTOR1]](<2 x s32>), $noreg, [[BUILD_VECTOR]](<8 x s32>), 1, 0 :: (dereferenceable load 8 from custom "TargetCustom8")
  ; GCN:   [[UV:%[0-9]+]]:_(s32), [[UV1:%[0-9]+]]:_(s32), [[UV2:%[0-9]+]]:_(s32) = G_UNMERGE_VALUES [[AMDGPU_INTRIN_IMAGE_LOAD]](<3 x s32>)
  ; GCN:   G_STORE [[UV2]](s32), [[DEF]](p1) :: (store 4 into `i32 addrspace(1)* undef`, addrspace 1)
  ; GCN:   $vgpr0 = COPY [[UV]](s32)
  ; GCN:   $vgpr1 = COPY [[UV1]](s32)
  ; GCN:   SI_RETURN_TO_EPILOG implicit $vgpr0, implicit $vgpr1
  %res = call { <2 x float>, i32 } @llvm.amdgcn.image.load.2d.sl_v2f32i32s.i32(i32 3, i32 %s, i32 %t, <8 x i32> %rsrc, i32 1, i32 0)
  %tex = extractvalue { <2 x float>, i32 } %res, 0
  %tfe = extractvalue { <2 x float>, i32 } %res, 1
  store i32 %tfe, i32 addrspace(1)* undef
  ret <2 x float> %tex
}

define amdgpu_ps <3 x float> @image_load_tfe_v3f32(<8 x i32> inreg %rsrc, i32 %s, i32 %t) {
  ; GCN-LABEL: name: image_load_tfe_v3f32
  ; GCN: bb.1 (%ir-block.0):
  ; GCN:   liveins: $sgpr2, $sgpr3, $sgpr4, $sgpr5, $sgpr6, $sgpr7, $sgpr8, $sgpr9, $vgpr0, $vgpr1
  ; GCN:   [[COPY:%[0-9]+]]:_(s32) = COPY $sgpr2
  ; GCN:   [[COPY1:%[0-9]+]]:_(s32) = COPY $sgpr3
  ; GCN:   [[COPY2:%[0-9]+]]:_(s32) = COPY $sgpr4
  ; GCN:   [[COPY3:%[0-9]+]]:_(s32) = COPY $sgpr5
  ; GCN:   [[COPY4:%[0-9]+]]:_(s32) = COPY $sgpr6
  ; GCN:   [[COPY5:%[0-9]+]]:_(s32) = COPY $sgpr7
  ; GCN:   [[COPY6:%[0-9]+]]:_(s32) = COPY $sgpr8
  ; GCN:   [[COPY7:%[0-9]+]]:_(s32) = COPY $sgpr9
  ; GCN:   [[COPY8:%[0-9]+]]:_(s32) = COPY $vgpr0
  ; GCN:   [[COPY9:%[0-9]+]]:_(s32) = COPY $vgpr1
  ; GCN:   [[BUILD_VECTOR:%[0-9]+]]:_(<8 x s32>) = G_BUILD_VECTOR [[COPY]](s32), [[COPY1]](s32), [[COPY2]](s32), [[COPY3]](s32), [[COPY4]](s32), [[COPY5]](s32), [[COPY6]](s32), [[COPY7]](s32)
  ; GCN:   [[DEF:%[0-9]+]]:_(p1) = G_IMPLICIT_DEF
  ; GCN:   [[BUILD_VECTOR1:%[0-9]+]]:_(<2 x s32>) = G_BUILD_VECTOR [[COPY8]](s32), [[COPY9]](s32)
  ; GCN:   [[AMDGPU_INTRIN_IMAGE_LOAD:%[0-9]+]]:_(<4 x s32>) = G_AMDGPU_INTRIN_IMAGE_LOAD intrinsic(@llvm.amdgcn.image.load.2d), 7, [[BUILD_VECTOR1]](<2 x s32>), $noreg, [[BUILD_VECTOR]](<8 x s32>), 1, 0 :: (dereferenceable load 12 from custom "TargetCustom8", align 16)
  ; GCN:   [[UV:%[0-9]+]]:_(s32), [[UV1:%[0-9]+]]:_(s32), [[UV2:%[0-9]+]]:_(s32), [[UV3:%[0-9]+]]:_(s32) = G_UNMERGE_VALUES [[AMDGPU_INTRIN_IMAGE_LOAD]](<4 x s32>)
  ; GCN:   G_STORE [[UV3]](s32), [[DEF]](p1) :: (store 4 into `i32 addrspace(1)* undef`, addrspace 1)
  ; GCN:   $vgpr0 = COPY [[UV]](s32)
  ; GCN:   $vgpr1 = COPY [[UV1]](s32)
  ; GCN:   $vgpr2 = COPY [[UV2]](s32)
  ; GCN:   SI_RETURN_TO_EPILOG implicit $vgpr0, implicit $vgpr1, implicit $vgpr2
  %res = call { <3 x float>, i32 } @llvm.amdgcn.image.load.2d.sl_v3f32i32s.i32(i32 7, i32 %s, i32 %t, <8 x i32> %rsrc, i32 1, i32 0)
  %tex = extractvalue { <3 x float>, i32 } %res, 0
  %tfe = extractvalue { <3 x float>, i32 } %res, 1
  store i32 %tfe, i32 addrspace(1)* undef
  ret <3 x float> %tex
}

define amdgpu_ps <4 x float> @image_load_tfe_v4f32(<8 x i32> inreg %rsrc, i32 %s, i32 %t) {
  ; GCN-LABEL: name: image_load_tfe_v4f32
  ; GCN: bb.1 (%ir-block.0):
  ; GCN:   liveins: $sgpr2, $sgpr3, $sgpr4, $sgpr5, $sgpr6, $sgpr7, $sgpr8, $sgpr9, $vgpr0, $vgpr1
  ; GCN:   [[COPY:%[0-9]+]]:_(s32) = COPY $sgpr2
  ; GCN:   [[COPY1:%[0-9]+]]:_(s32) = COPY $sgpr3
  ; GCN:   [[COPY2:%[0-9]+]]:_(s32) = COPY $sgpr4
  ; GCN:   [[COPY3:%[0-9]+]]:_(s32) = COPY $sgpr5
  ; GCN:   [[COPY4:%[0-9]+]]:_(s32) = COPY $sgpr6
  ; GCN:   [[COPY5:%[0-9]+]]:_(s32) = COPY $sgpr7
  ; GCN:   [[COPY6:%[0-9]+]]:_(s32) = COPY $sgpr8
  ; GCN:   [[COPY7:%[0-9]+]]:_(s32) = COPY $sgpr9
  ; GCN:   [[COPY8:%[0-9]+]]:_(s32) = COPY $vgpr0
  ; GCN:   [[COPY9:%[0-9]+]]:_(s32) = COPY $vgpr1
  ; GCN:   [[BUILD_VECTOR:%[0-9]+]]:_(<8 x s32>) = G_BUILD_VECTOR [[COPY]](s32), [[COPY1]](s32), [[COPY2]](s32), [[COPY3]](s32), [[COPY4]](s32), [[COPY5]](s32), [[COPY6]](s32), [[COPY7]](s32)
  ; GCN:   [[DEF:%[0-9]+]]:_(p1) = G_IMPLICIT_DEF
  ; GCN:   [[BUILD_VECTOR1:%[0-9]+]]:_(<2 x s32>) = G_BUILD_VECTOR [[COPY8]](s32), [[COPY9]](s32)
  ; GCN:   [[AMDGPU_INTRIN_IMAGE_LOAD:%[0-9]+]]:_(<5 x s32>) = G_AMDGPU_INTRIN_IMAGE_LOAD intrinsic(@llvm.amdgcn.image.load.2d), 15, [[BUILD_VECTOR1]](<2 x s32>), $noreg, [[BUILD_VECTOR]](<8 x s32>), 1, 0 :: (dereferenceable load 16 from custom "TargetCustom8")
  ; GCN:   [[UV:%[0-9]+]]:_(s32), [[UV1:%[0-9]+]]:_(s32), [[UV2:%[0-9]+]]:_(s32), [[UV3:%[0-9]+]]:_(s32), [[UV4:%[0-9]+]]:_(s32) = G_UNMERGE_VALUES [[AMDGPU_INTRIN_IMAGE_LOAD]](<5 x s32>)
  ; GCN:   G_STORE [[UV4]](s32), [[DEF]](p1) :: (store 4 into `i32 addrspace(1)* undef`, addrspace 1)
  ; GCN:   $vgpr0 = COPY [[UV]](s32)
  ; GCN:   $vgpr1 = COPY [[UV1]](s32)
  ; GCN:   $vgpr2 = COPY [[UV2]](s32)
  ; GCN:   $vgpr3 = COPY [[UV3]](s32)
  ; GCN:   SI_RETURN_TO_EPILOG implicit $vgpr0, implicit $vgpr1, implicit $vgpr2, implicit $vgpr3
  %res = call { <4 x float>, i32 } @llvm.amdgcn.image.load.2d.sl_v4f32i32s.i32(i32 15, i32 %s, i32 %t, <8 x i32> %rsrc, i32 1, i32 0)
  %tex = extractvalue { <4 x float>, i32 } %res, 0
  %tfe = extractvalue { <4 x float>, i32 } %res, 1
  store i32 %tfe, i32 addrspace(1)* undef
  ret <4 x float> %tex
}

declare float @llvm.amdgcn.image.load.2d.f32.i32(i32 immarg, i32, i32, <8 x i32>, i32 immarg, i32 immarg) #0
declare <2 x float> @llvm.amdgcn.image.load.2d.v2f32.i32(i32 immarg, i32, i32, <8 x i32>, i32 immarg, i32 immarg) #0
declare <3 x float> @llvm.amdgcn.image.load.2d.v3f32.i32(i32 immarg, i32, i32, <8 x i32>, i32 immarg, i32 immarg) #0
declare <4 x float> @llvm.amdgcn.image.load.2d.v4f32.i32(i32 immarg, i32, i32, <8 x i32>, i32 immarg, i32 immarg) #0
declare { float, i32 } @llvm.amdgcn.image.load.2d.sl_f32i32s.i32(i32 immarg, i32, i32, <8 x i32>, i32 immarg, i32 immarg) #0
declare { <2 x float>, i32 } @llvm.amdgcn.image.load.2d.sl_v2f32i32s.i32(i32 immarg, i32, i32, <8 x i32>, i32 immarg, i32 immarg) #0
declare { <3 x float>, i32 } @llvm.amdgcn.image.load.2d.sl_v3f32i32s.i32(i32 immarg, i32, i32, <8 x i32>, i32 immarg, i32 immarg) #0
declare { <4 x float>, i32 } @llvm.amdgcn.image.load.2d.sl_v4f32i32s.i32(i32 immarg, i32, i32, <8 x i32>, i32 immarg, i32 immarg) #0

attributes #0 = { nounwind readonly }
