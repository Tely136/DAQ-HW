; ModuleID = 'C:/DAQ-HW/hls/counter/stream_reader/stream_reader/hls/.autopilot/db/a.g.ld.5.gdce.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-i64:64-i128:128-i256:256-i512:512-i1024:1024-i2048:2048-i4096:4096-n8:16:32:64-S128-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "fpga64-xilinx-none"

%"class.hls::stream<int>" = type { i32 }

; Function Attrs: inaccessiblememonly nounwind willreturn
declare void @llvm.sideeffect() #0

; Function Attrs: inaccessiblemem_or_argmemonly noinline willreturn
define void @apatb_stream_reader_ir(%"class.hls::stream<int>"* noalias nocapture nonnull align 4 dereferenceable(4) %data_in, i32* noalias nocapture nonnull "maxi" %data_out, i32 %n_outer, i32 %n_inner) local_unnamed_addr #1 {
entry:
  %data_in_copy = alloca i32, align 512
  call void @llvm.sideeffect() #9 [ "stream_interface"(i32* %data_in_copy, i32 0) ]
  %0 = bitcast i32* %data_out to [128 x i32]*
  %data_out_copy = alloca [128 x i32], align 512
  call fastcc void @copy_in(%"class.hls::stream<int>"* nonnull %data_in, i32* nonnull align 512 %data_in_copy, [128 x i32]* nonnull %0, [128 x i32]* nonnull align 512 %data_out_copy)
  call void @apatb_stream_reader_hw(i32* %data_in_copy, [128 x i32]* %data_out_copy, i32 %n_outer, i32 %n_inner)
  call void @copy_back(%"class.hls::stream<int>"* %data_in, i32* %data_in_copy, [128 x i32]* %0, [128 x i32]* %data_out_copy)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_in(%"class.hls::stream<int>"* noalias "unpacked"="0", i32* noalias nocapture align 512 "unpacked"="1.0", [128 x i32]* noalias readonly "unpacked"="2", [128 x i32]* noalias align 512 "unpacked"="3") unnamed_addr #2 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<int>"(i32* align 512 %1, %"class.hls::stream<int>"* %0)
  call fastcc void @onebyonecpy_hls.p0a128i32([128 x i32]* align 512 %3, [128 x i32]* %2)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"onebyonecpy_hls.p0class.hls::stream<int>"(i32* noalias nocapture align 512 "unpacked"="0.0" %dst, %"class.hls::stream<int>"* noalias "unpacked"="1" %src) unnamed_addr #3 {
entry:
  %0 = icmp eq %"class.hls::stream<int>"* %src, null
  br i1 %0, label %ret, label %copy

copy:                                             ; preds = %entry
  call fastcc void @"streamcpy_hls.p0class.hls::stream<int>"(i32* align 512 %dst, %"class.hls::stream<int>"* nonnull %src)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"streamcpy_hls.p0class.hls::stream<int>"(i32* noalias nocapture align 512 "unpacked"="0.0", %"class.hls::stream<int>"* noalias nocapture "unpacked"="1") unnamed_addr #4 {
entry:
  %2 = alloca %"class.hls::stream<int>"
  %3 = alloca i32
  br label %empty

empty:                                            ; preds = %push, %entry
  %4 = bitcast %"class.hls::stream<int>"* %1 to i8*
  %5 = call i1 @fpga_fifo_not_empty_4(i8* %4)
  br i1 %5, label %push, label %ret

push:                                             ; preds = %empty
  %6 = bitcast %"class.hls::stream<int>"* %2 to i8*
  %7 = bitcast %"class.hls::stream<int>"* %1 to i8*
  call void @fpga_fifo_pop_4(i8* %6, i8* %7)
  %8 = load volatile %"class.hls::stream<int>", %"class.hls::stream<int>"* %2
  %.evi = extractvalue %"class.hls::stream<int>" %8, 0
  store i32 %.evi, i32* %3
  %9 = bitcast i32* %3 to i8*
  %10 = bitcast i32* %0 to i8*
  call void @fpga_fifo_push_4(i8* %9, i8* %10)
  br label %empty, !llvm.loop !6

ret:                                              ; preds = %empty
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define internal fastcc void @onebyonecpy_hls.p0a128i32([128 x i32]* noalias align 512 %dst, [128 x i32]* noalias readonly %src) unnamed_addr #5 {
entry:
  %0 = icmp eq [128 x i32]* %dst, null
  %1 = icmp eq [128 x i32]* %src, null
  %2 = or i1 %0, %1
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  call void @arraycpy_hls.p0a128i32([128 x i32]* nonnull %dst, [128 x i32]* nonnull %src, i64 128)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse willreturn
define void @arraycpy_hls.p0a128i32([128 x i32]* %dst, [128 x i32]* readonly %src, i64 %num) local_unnamed_addr #6 {
entry:
  %0 = icmp eq [128 x i32]* %src, null
  %1 = icmp eq [128 x i32]* %dst, null
  %2 = or i1 %1, %0
  br i1 %2, label %ret, label %copy

copy:                                             ; preds = %entry
  %for.loop.cond1 = icmp sgt i64 %num, 0
  br i1 %for.loop.cond1, label %for.loop.lr.ph, label %copy.split

for.loop.lr.ph:                                   ; preds = %copy
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %for.loop.lr.ph
  %for.loop.idx2 = phi i64 [ 0, %for.loop.lr.ph ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [128 x i32], [128 x i32]* %dst, i64 0, i64 %for.loop.idx2
  %src.addr = getelementptr [128 x i32], [128 x i32]* %src, i64 0, i64 %for.loop.idx2
  %3 = load i32, i32* %src.addr, align 4
  store i32 %3, i32* %dst.addr, align 4
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx2, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, %num
  br i1 %exitcond, label %for.loop, label %copy.split

copy.split:                                       ; preds = %for.loop, %copy
  br label %ret

ret:                                              ; preds = %copy.split, %entry
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_out(%"class.hls::stream<int>"* noalias "unpacked"="0", i32* noalias nocapture align 512 "unpacked"="1.0", [128 x i32]* noalias "unpacked"="2", [128 x i32]* noalias readonly align 512 "unpacked"="3") unnamed_addr #7 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<int>.4"(%"class.hls::stream<int>"* %0, i32* align 512 %1)
  call fastcc void @onebyonecpy_hls.p0a128i32([128 x i32]* %2, [128 x i32]* align 512 %3)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"onebyonecpy_hls.p0class.hls::stream<int>.4"(%"class.hls::stream<int>"* noalias "unpacked"="0" %dst, i32* noalias nocapture align 512 "unpacked"="1.0" %src) unnamed_addr #3 {
entry:
  %0 = icmp eq %"class.hls::stream<int>"* %dst, null
  br i1 %0, label %ret, label %copy

copy:                                             ; preds = %entry
  call fastcc void @"streamcpy_hls.p0class.hls::stream<int>.7"(%"class.hls::stream<int>"* nonnull %dst, i32* align 512 %src)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"streamcpy_hls.p0class.hls::stream<int>.7"(%"class.hls::stream<int>"* noalias nocapture "unpacked"="0", i32* noalias nocapture align 512 "unpacked"="1.0") unnamed_addr #4 {
entry:
  %2 = alloca i32
  %3 = alloca %"class.hls::stream<int>"
  br label %empty

empty:                                            ; preds = %push, %entry
  %4 = bitcast i32* %1 to i8*
  %5 = call i1 @fpga_fifo_not_empty_4(i8* %4)
  br i1 %5, label %push, label %ret

push:                                             ; preds = %empty
  %6 = bitcast i32* %2 to i8*
  %7 = bitcast i32* %1 to i8*
  call void @fpga_fifo_pop_4(i8* %6, i8* %7)
  %8 = load volatile i32, i32* %2
  %.ivi = insertvalue %"class.hls::stream<int>" undef, i32 %8, 0
  store %"class.hls::stream<int>" %.ivi, %"class.hls::stream<int>"* %3
  %9 = bitcast %"class.hls::stream<int>"* %3 to i8*
  %10 = bitcast %"class.hls::stream<int>"* %0 to i8*
  call void @fpga_fifo_push_4(i8* %9, i8* %10)
  br label %empty, !llvm.loop !8

ret:                                              ; preds = %empty
  ret void
}

declare i8* @malloc(i64)

declare void @free(i8*)

declare void @apatb_stream_reader_hw(i32* %data_in, [128 x i32]* %data_out, i32 %n_outer, i32 %n_inner)

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_back(%"class.hls::stream<int>"* noalias "unpacked"="0", i32* noalias nocapture align 512 "unpacked"="1.0", [128 x i32]* noalias "unpacked"="2", [128 x i32]* noalias readonly align 512 "unpacked"="3") unnamed_addr #7 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<int>.4"(%"class.hls::stream<int>"* %0, i32* align 512 %1)
  call fastcc void @onebyonecpy_hls.p0a128i32([128 x i32]* %2, [128 x i32]* align 512 %3)
  ret void
}

declare void @stream_reader_hw_stub(%"class.hls::stream<int>"* noalias nocapture nonnull, i32* noalias nocapture nonnull, i32, i32)

define void @stream_reader_hw_stub_wrapper(i32* %data_in, [128 x i32]* %data_out, i32 %n_outer, i32 %n_inner) #8 {
entry:
  %0 = call i8* @malloc(i64 4)
  %1 = bitcast i8* %0 to %"class.hls::stream<int>"*
  call void @copy_out(%"class.hls::stream<int>"* %1, i32* %data_in, [128 x i32]* null, [128 x i32]* %data_out)
  %2 = bitcast [128 x i32]* %data_out to i32*
  call void @stream_reader_hw_stub(%"class.hls::stream<int>"* %1, i32* %2, i32 %n_outer, i32 %n_inner)
  call void @copy_in(%"class.hls::stream<int>"* %1, i32* %data_in, [128 x i32]* null, [128 x i32]* %data_out)
  call void @free(i8* %0)
  ret void
}

declare i1 @fpga_fifo_not_empty_4(i8*)

declare void @fpga_fifo_pop_4(i8*, i8*)

declare void @fpga_fifo_push_4(i8*, i8*)

attributes #0 = { inaccessiblememonly nounwind willreturn }
attributes #1 = { inaccessiblemem_or_argmemonly noinline willreturn "fpga.wrapper.func"="wrapper" }
attributes #2 = { argmemonly noinline willreturn "fpga.wrapper.func"="copyin" }
attributes #3 = { argmemonly noinline willreturn "fpga.wrapper.func"="onebyonecpy_hls" }
attributes #4 = { argmemonly noinline willreturn "fpga.wrapper.func"="streamcpy_hls" }
attributes #5 = { argmemonly noinline norecurse willreturn "fpga.wrapper.func"="onebyonecpy_hls" }
attributes #6 = { argmemonly noinline norecurse willreturn "fpga.wrapper.func"="arraycpy_hls" }
attributes #7 = { argmemonly noinline willreturn "fpga.wrapper.func"="copyout" }
attributes #8 = { "fpga.wrapper.func"="stub" }
attributes #9 = { inaccessiblememonly nounwind willreturn "xlx.port.bitwidth"="32" "xlx.source"="user" }

!llvm.dbg.cu = !{}
!llvm.ident = !{!0, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1, !1}
!llvm.module.flags = !{!2, !3, !4}
!blackbox_cfg = !{!5}

!0 = !{!"AMD/Xilinx clang version 16.0.6"}
!1 = !{!"clang version 7.0.0 "}
!2 = !{i32 2, !"Dwarf Version", i32 4}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.rotate.disable"}
!8 = distinct !{!8, !7}
