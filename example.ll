; ModuleID = 'example.c'
source_filename = "example.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx12.0.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @swap(ptr noundef %p, ptr noundef %q) #0 !dbg !9 {
entry:
  %p.addr = alloca ptr, align 8
  %q.addr = alloca ptr, align 8
  %t = alloca ptr, align 8
  store ptr %p, ptr %p.addr, align 8
  call void @llvm.dbg.declare(metadata ptr %p.addr, metadata !16, metadata !DIExpression()), !dbg !17
  store ptr %q, ptr %q.addr, align 8
  call void @llvm.dbg.declare(metadata ptr %q.addr, metadata !18, metadata !DIExpression()), !dbg !19
  call void @llvm.dbg.declare(metadata ptr %t, metadata !20, metadata !DIExpression()), !dbg !21
  %0 = load ptr, ptr %p.addr, align 8, !dbg !22
  %1 = load ptr, ptr %0, align 8, !dbg !23
  store ptr %1, ptr %t, align 8, !dbg !21
  %2 = load ptr, ptr %q.addr, align 8, !dbg !24
  %3 = load ptr, ptr %2, align 8, !dbg !25
  %4 = load ptr, ptr %p.addr, align 8, !dbg !26
  store ptr %3, ptr %4, align 8, !dbg !27
  %5 = load ptr, ptr %t, align 8, !dbg !28
  %6 = load ptr, ptr %q.addr, align 8, !dbg !29
  store ptr %5, ptr %6, align 8, !dbg !30
  ret void, !dbg !31
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 !dbg !32 {
entry:
  %a1 = alloca i8, align 1
  %b1 = alloca i8, align 1
  %a = alloca ptr, align 8
  %b = alloca ptr, align 8
  call void @llvm.dbg.declare(metadata ptr %a1, metadata !36, metadata !DIExpression()), !dbg !37
  call void @llvm.dbg.declare(metadata ptr %b1, metadata !38, metadata !DIExpression()), !dbg !39
  call void @llvm.dbg.declare(metadata ptr %a, metadata !40, metadata !DIExpression()), !dbg !41
  store ptr %a1, ptr %a, align 8, !dbg !41
  call void @llvm.dbg.declare(metadata ptr %b, metadata !42, metadata !DIExpression()), !dbg !43
  store ptr %b1, ptr %b, align 8, !dbg !43
  call void @swap(ptr noundef %a, ptr noundef %b), !dbg !44
  ret i32 0, !dbg !45
}

attributes #0 = { noinline nounwind optnone ssp uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+cx8,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "Homebrew clang version 16.0.6", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None, sysroot: "/Library/Developer/CommandLineTools/SDKs/MacOSX12.sdk", sdk: "MacOSX12.sdk")
!1 = !DIFile(filename: "example.c", directory: "/Users/jiaweideng/Documents/SVF/SVF-pybind11-master/SVF-Python")
!2 = !{i32 7, !"Dwarf Version", i32 4}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 8, !"PIC Level", i32 2}
!6 = !{i32 7, !"uwtable", i32 2}
!7 = !{i32 7, !"frame-pointer", i32 2}
!8 = !{!"Homebrew clang version 16.0.6"}
!9 = distinct !DISubprogram(name: "swap", scope: !1, file: !1, line: 1, type: !10, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !15)
!10 = !DISubroutineType(types: !11)
!11 = !{null, !12, !12}
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!14 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!15 = !{}
!16 = !DILocalVariable(name: "p", arg: 1, scope: !9, file: !1, line: 1, type: !12)
!17 = !DILocation(line: 1, column: 18, scope: !9)
!18 = !DILocalVariable(name: "q", arg: 2, scope: !9, file: !1, line: 1, type: !12)
!19 = !DILocation(line: 1, column: 28, scope: !9)
!20 = !DILocalVariable(name: "t", scope: !9, file: !1, line: 2, type: !13)
!21 = !DILocation(line: 2, column: 9, scope: !9)
!22 = !DILocation(line: 2, column: 14, scope: !9)
!23 = !DILocation(line: 2, column: 13, scope: !9)
!24 = !DILocation(line: 3, column: 14, scope: !9)
!25 = !DILocation(line: 3, column: 13, scope: !9)
!26 = !DILocation(line: 3, column: 9, scope: !9)
!27 = !DILocation(line: 3, column: 11, scope: !9)
!28 = !DILocation(line: 4, column: 13, scope: !9)
!29 = !DILocation(line: 4, column: 9, scope: !9)
!30 = !DILocation(line: 4, column: 11, scope: !9)
!31 = !DILocation(line: 5, column: 1, scope: !9)
!32 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 6, type: !33, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !15)
!33 = !DISubroutineType(types: !34)
!34 = !{!35}
!35 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!36 = !DILocalVariable(name: "a1", scope: !32, file: !1, line: 7, type: !14)
!37 = !DILocation(line: 7, column: 12, scope: !32)
!38 = !DILocalVariable(name: "b1", scope: !32, file: !1, line: 7, type: !14)
!39 = !DILocation(line: 7, column: 16, scope: !32)
!40 = !DILocalVariable(name: "a", scope: !32, file: !1, line: 8, type: !13)
!41 = !DILocation(line: 8, column: 13, scope: !32)
!42 = !DILocalVariable(name: "b", scope: !32, file: !1, line: 9, type: !13)
!43 = !DILocation(line: 9, column: 13, scope: !32)
!44 = !DILocation(line: 10, column: 7, scope: !32)
!45 = !DILocation(line: 11, column: 1, scope: !32)
