; ModuleID = 'example.c'
source_filename = "example.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define void @swap(ptr noundef %p, ptr noundef %q) #0 !dbg !10 {
entry:
  %p.addr = alloca ptr, align 8
  %q.addr = alloca ptr, align 8
  %t = alloca ptr, align 8
  store ptr %p, ptr %p.addr, align 8
  call void @llvm.dbg.declare(metadata ptr %p.addr, metadata !17, metadata !DIExpression()), !dbg !18
  store ptr %q, ptr %q.addr, align 8
  call void @llvm.dbg.declare(metadata ptr %q.addr, metadata !19, metadata !DIExpression()), !dbg !20
  call void @llvm.dbg.declare(metadata ptr %t, metadata !21, metadata !DIExpression()), !dbg !22
  %0 = load ptr, ptr %p.addr, align 8, !dbg !23
  %1 = load ptr, ptr %0, align 8, !dbg !24
  store ptr %1, ptr %t, align 8, !dbg !22
  %2 = load ptr, ptr %q.addr, align 8, !dbg !25
  %3 = load ptr, ptr %2, align 8, !dbg !26
  %4 = load ptr, ptr %p.addr, align 8, !dbg !27
  store ptr %3, ptr %4, align 8, !dbg !28
  %5 = load ptr, ptr %t, align 8, !dbg !29
  %6 = load ptr, ptr %q.addr, align 8, !dbg !30
  store ptr %5, ptr %6, align 8, !dbg !31
  ret void, !dbg !32
}

; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @main() #0 !dbg !33 {
entry:
  %a1 = alloca i8, align 1
  %b1 = alloca i8, align 1
  %a = alloca ptr, align 8
  %b = alloca ptr, align 8
  call void @llvm.dbg.declare(metadata ptr %a1, metadata !37, metadata !DIExpression()), !dbg !38
  call void @llvm.dbg.declare(metadata ptr %b1, metadata !39, metadata !DIExpression()), !dbg !40
  call void @llvm.dbg.declare(metadata ptr %a, metadata !41, metadata !DIExpression()), !dbg !42
  store ptr %a1, ptr %a, align 8, !dbg !42
  call void @llvm.dbg.declare(metadata ptr %b, metadata !43, metadata !DIExpression()), !dbg !44
  store ptr %b1, ptr %b, align 8, !dbg !44
  call void @swap(ptr noundef %a, ptr noundef %b), !dbg !45
  ret i32 0, !dbg !46
}

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #1 = { nocallback nofree nosync nounwind readnone speculatable willreturn }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6}
!llvm.dbg.cu = !{!7}
!llvm.ident = !{!9}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 14, i32 4]}
!1 = !{i32 7, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{i32 8, !"PIC Level", i32 2}
!5 = !{i32 7, !"uwtable", i32 1}
!6 = !{i32 7, !"frame-pointer", i32 1}
!7 = distinct !DICompileUnit(language: DW_LANG_C99, file: !8, producer: "Apple clang version 15.0.0 (clang-1500.3.9.4)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None, sysroot: "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk", sdk: "MacOSX.sdk")
!8 = !DIFile(filename: "example.c", directory: "/Users/den184/Documents/UNSW/SVF/SVF-Python")
!9 = !{!"Apple clang version 15.0.0 (clang-1500.3.9.4)"}
!10 = distinct !DISubprogram(name: "swap", scope: !8, file: !8, line: 1, type: !11, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7, retainedNodes: !16)
!11 = !DISubroutineType(types: !12)
!12 = !{null, !13, !13}
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!14 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !15, size: 64)
!15 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!16 = !{}
!17 = !DILocalVariable(name: "p", arg: 1, scope: !10, file: !8, line: 1, type: !13)
!18 = !DILocation(line: 1, column: 18, scope: !10)
!19 = !DILocalVariable(name: "q", arg: 2, scope: !10, file: !8, line: 1, type: !13)
!20 = !DILocation(line: 1, column: 28, scope: !10)
!21 = !DILocalVariable(name: "t", scope: !10, file: !8, line: 2, type: !14)
!22 = !DILocation(line: 2, column: 9, scope: !10)
!23 = !DILocation(line: 2, column: 14, scope: !10)
!24 = !DILocation(line: 2, column: 13, scope: !10)
!25 = !DILocation(line: 3, column: 14, scope: !10)
!26 = !DILocation(line: 3, column: 13, scope: !10)
!27 = !DILocation(line: 3, column: 9, scope: !10)
!28 = !DILocation(line: 3, column: 11, scope: !10)
!29 = !DILocation(line: 4, column: 13, scope: !10)
!30 = !DILocation(line: 4, column: 9, scope: !10)
!31 = !DILocation(line: 4, column: 11, scope: !10)
!32 = !DILocation(line: 5, column: 1, scope: !10)
!33 = distinct !DISubprogram(name: "main", scope: !8, file: !8, line: 6, type: !34, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !7, retainedNodes: !16)
!34 = !DISubroutineType(types: !35)
!35 = !{!36}
!36 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!37 = !DILocalVariable(name: "a1", scope: !33, file: !8, line: 7, type: !15)
!38 = !DILocation(line: 7, column: 12, scope: !33)
!39 = !DILocalVariable(name: "b1", scope: !33, file: !8, line: 7, type: !15)
!40 = !DILocation(line: 7, column: 16, scope: !33)
!41 = !DILocalVariable(name: "a", scope: !33, file: !8, line: 8, type: !14)
!42 = !DILocation(line: 8, column: 13, scope: !33)
!43 = !DILocalVariable(name: "b", scope: !33, file: !8, line: 9, type: !14)
!44 = !DILocation(line: 9, column: 13, scope: !33)
!45 = !DILocation(line: 10, column: 7, scope: !33)
!46 = !DILocation(line: 11, column: 1, scope: !33)
