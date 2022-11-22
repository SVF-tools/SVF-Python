; ModuleID = 'example.c'
source_filename = "example.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @swap(i8** %p, i8** %q) #0 !dbg !7 {
entry:
  %p.addr = alloca i8**, align 8
  %q.addr = alloca i8**, align 8
  %t = alloca i8*, align 8
  store i8** %p, i8*** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8*** %p.addr, metadata !13, metadata !DIExpression()), !dbg !14
  store i8** %q, i8*** %q.addr, align 8
  call void @llvm.dbg.declare(metadata i8*** %q.addr, metadata !15, metadata !DIExpression()), !dbg !16
  call void @llvm.dbg.declare(metadata i8** %t, metadata !17, metadata !DIExpression()), !dbg !18
  %0 = load i8**, i8*** %p.addr, align 8, !dbg !19
  %1 = load i8*, i8** %0, align 8, !dbg !20
  store i8* %1, i8** %t, align 8, !dbg !18
  %2 = load i8**, i8*** %q.addr, align 8, !dbg !21
  %3 = load i8*, i8** %2, align 8, !dbg !22
  %4 = load i8**, i8*** %p.addr, align 8, !dbg !23
  store i8* %3, i8** %4, align 8, !dbg !24
  %5 = load i8*, i8** %t, align 8, !dbg !25
  %6 = load i8**, i8*** %q.addr, align 8, !dbg !26
  store i8* %5, i8** %6, align 8, !dbg !27
  ret void, !dbg !28
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !29 {
entry:
  %a1 = alloca i8, align 1
  %b1 = alloca i8, align 1
  %a = alloca i8*, align 8
  %b = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata i8* %a1, metadata !33, metadata !DIExpression()), !dbg !34
  call void @llvm.dbg.declare(metadata i8* %b1, metadata !35, metadata !DIExpression()), !dbg !36
  call void @llvm.dbg.declare(metadata i8** %a, metadata !37, metadata !DIExpression()), !dbg !38
  store i8* %a1, i8** %a, align 8, !dbg !38
  call void @llvm.dbg.declare(metadata i8** %b, metadata !39, metadata !DIExpression()), !dbg !40
  store i8* %b1, i8** %b, align 8, !dbg !40
  call void @swap(i8** %a, i8** %b), !dbg !41
  ret i32 0, !dbg !42
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 10.0.0-4ubuntu1 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "example.c", directory: "/home/chengwang/\E6\A1\8C\E9\9D\A2/test/SVF-Python-example-main")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 10.0.0-4ubuntu1 "}
!7 = distinct !DISubprogram(name: "swap", scope: !1, file: !1, line: 1, type: !8, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{null, !10, !10}
!10 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !11, size: 64)
!11 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !12, size: 64)
!12 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!13 = !DILocalVariable(name: "p", arg: 1, scope: !7, file: !1, line: 1, type: !10)
!14 = !DILocation(line: 1, column: 18, scope: !7)
!15 = !DILocalVariable(name: "q", arg: 2, scope: !7, file: !1, line: 1, type: !10)
!16 = !DILocation(line: 1, column: 28, scope: !7)
!17 = !DILocalVariable(name: "t", scope: !7, file: !1, line: 2, type: !11)
!18 = !DILocation(line: 2, column: 9, scope: !7)
!19 = !DILocation(line: 2, column: 14, scope: !7)
!20 = !DILocation(line: 2, column: 13, scope: !7)
!21 = !DILocation(line: 3, column: 14, scope: !7)
!22 = !DILocation(line: 3, column: 13, scope: !7)
!23 = !DILocation(line: 3, column: 9, scope: !7)
!24 = !DILocation(line: 3, column: 11, scope: !7)
!25 = !DILocation(line: 4, column: 13, scope: !7)
!26 = !DILocation(line: 4, column: 9, scope: !7)
!27 = !DILocation(line: 4, column: 11, scope: !7)
!28 = !DILocation(line: 5, column: 1, scope: !7)
!29 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 6, type: !30, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!30 = !DISubroutineType(types: !31)
!31 = !{!32}
!32 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!33 = !DILocalVariable(name: "a1", scope: !29, file: !1, line: 7, type: !12)
!34 = !DILocation(line: 7, column: 12, scope: !29)
!35 = !DILocalVariable(name: "b1", scope: !29, file: !1, line: 7, type: !12)
!36 = !DILocation(line: 7, column: 16, scope: !29)
!37 = !DILocalVariable(name: "a", scope: !29, file: !1, line: 8, type: !11)
!38 = !DILocation(line: 8, column: 13, scope: !29)
!39 = !DILocalVariable(name: "b", scope: !29, file: !1, line: 9, type: !11)
!40 = !DILocation(line: 9, column: 13, scope: !29)
!41 = !DILocation(line: 10, column: 7, scope: !29)
!42 = !DILocation(line: 11, column: 1, scope: !29)
