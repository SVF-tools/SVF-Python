; ModuleID = './input.ll'
source_filename = "input.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

; Function Attrs: noinline nounwind ssp uwtable(sync)
define void @swap(ptr noundef %p, ptr noundef %q) #0 !dbg !9 {
entry:
  call void @llvm.dbg.value(metadata ptr %p, metadata !16, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata ptr %q, metadata !18, metadata !DIExpression()), !dbg !17
  %0 = load ptr, ptr %p, align 8, !dbg !19
  call void @llvm.dbg.value(metadata ptr %0, metadata !20, metadata !DIExpression()), !dbg !17
  %1 = load ptr, ptr %q, align 8, !dbg !21
  store ptr %1, ptr %p, align 8, !dbg !22
  store ptr %0, ptr %q, align 8, !dbg !23
  ret void, !dbg !24
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind ssp uwtable(sync)
define i32 @main() #0 !dbg !25 {
entry:
  %a1 = alloca i8, align 1
  %b1 = alloca i8, align 1
  %a = alloca ptr, align 8
  %b = alloca ptr, align 8
  call void @llvm.dbg.declare(metadata ptr %a1, metadata !29, metadata !DIExpression()), !dbg !30
  call void @llvm.dbg.declare(metadata ptr %b1, metadata !31, metadata !DIExpression()), !dbg !32
  call void @llvm.dbg.declare(metadata ptr %a, metadata !33, metadata !DIExpression()), !dbg !34
  store ptr %a1, ptr %a, align 8, !dbg !34
  call void @llvm.dbg.declare(metadata ptr %b, metadata !35, metadata !DIExpression()), !dbg !36
  store ptr %b1, ptr %b, align 8, !dbg !36
  call void @swap(ptr noundef %a, ptr noundef %b), !dbg !37
  ret i32 0, !dbg !38
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.value(metadata, metadata, metadata) #1

attributes #0 = { noinline nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "Homebrew clang version 16.0.6", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None, sysroot: "/Library/Developer/CommandLineTools/SDKs/MacOSX14.sdk", sdk: "MacOSX14.sdk")
!1 = !DIFile(filename: "input.c", directory: "/Users/z5489735/2023/0424/SVF-Python/demo")
!2 = !{i32 7, !"Dwarf Version", i32 4}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 8, !"PIC Level", i32 2}
!6 = !{i32 7, !"uwtable", i32 1}
!7 = !{i32 7, !"frame-pointer", i32 1}
!8 = !{!"Homebrew clang version 16.0.6"}
!9 = distinct !DISubprogram(name: "swap", scope: !1, file: !1, line: 1, type: !10, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !15)
!10 = !DISubroutineType(types: !11)
!11 = !{null, !12, !12}
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!14 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!15 = !{}
!16 = !DILocalVariable(name: "p", arg: 1, scope: !9, file: !1, line: 1, type: !12)
!17 = !DILocation(line: 0, scope: !9)
!18 = !DILocalVariable(name: "q", arg: 2, scope: !9, file: !1, line: 1, type: !12)
!19 = !DILocation(line: 2, column: 13, scope: !9)
!20 = !DILocalVariable(name: "t", scope: !9, file: !1, line: 2, type: !13)
!21 = !DILocation(line: 3, column: 13, scope: !9)
!22 = !DILocation(line: 3, column: 11, scope: !9)
!23 = !DILocation(line: 4, column: 11, scope: !9)
!24 = !DILocation(line: 5, column: 1, scope: !9)
!25 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 6, type: !26, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !15)
!26 = !DISubroutineType(types: !27)
!27 = !{!28}
!28 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!29 = !DILocalVariable(name: "a1", scope: !25, file: !1, line: 7, type: !14)
!30 = !DILocation(line: 7, column: 12, scope: !25)
!31 = !DILocalVariable(name: "b1", scope: !25, file: !1, line: 7, type: !14)
!32 = !DILocation(line: 7, column: 16, scope: !25)
!33 = !DILocalVariable(name: "a", scope: !25, file: !1, line: 8, type: !13)
!34 = !DILocation(line: 8, column: 13, scope: !25)
!35 = !DILocalVariable(name: "b", scope: !25, file: !1, line: 9, type: !13)
!36 = !DILocation(line: 9, column: 13, scope: !25)
!37 = !DILocation(line: 10, column: 7, scope: !25)
!38 = !DILocation(line: 11, column: 1, scope: !25)
