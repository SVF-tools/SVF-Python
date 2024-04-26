; ModuleID = 'example2.c'
source_filename = "example2.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

@.str = private unnamed_addr constant [4 x i8] c"%d \00", align 1, !dbg !0
@.str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1, !dbg !7
@.str.2 = private unnamed_addr constant [49 x i8] c"Enter the number of rows for Pascal's Triangle: \00", align 1, !dbg !12
@.str.3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1, !dbg !17
@.str.4 = private unnamed_addr constant [33 x i8] c"Pascal's Triangle with %d rows:\0A\00", align 1, !dbg !22

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @binomialCoeff(i32 noundef %n, i32 noundef %k) #0 !dbg !37 {
entry:
  %n.addr = alloca i32, align 4
  %k.addr = alloca i32, align 4
  %res = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  call void @llvm.dbg.declare(metadata ptr %n.addr, metadata !42, metadata !DIExpression()), !dbg !43
  store i32 %k, ptr %k.addr, align 4
  call void @llvm.dbg.declare(metadata ptr %k.addr, metadata !44, metadata !DIExpression()), !dbg !45
  call void @llvm.dbg.declare(metadata ptr %res, metadata !46, metadata !DIExpression()), !dbg !47
  store i32 1, ptr %res, align 4, !dbg !47
  %0 = load i32, ptr %k.addr, align 4, !dbg !48
  %1 = load i32, ptr %n.addr, align 4, !dbg !50
  %2 = load i32, ptr %k.addr, align 4, !dbg !51
  %sub = sub nsw i32 %1, %2, !dbg !52
  %cmp = icmp sgt i32 %0, %sub, !dbg !53
  br i1 %cmp, label %if.then, label %if.end, !dbg !54

if.then:                                          ; preds = %entry
  %3 = load i32, ptr %n.addr, align 4, !dbg !55
  %4 = load i32, ptr %k.addr, align 4, !dbg !56
  %sub1 = sub nsw i32 %3, %4, !dbg !57
  store i32 %sub1, ptr %k.addr, align 4, !dbg !58
  br label %if.end, !dbg !59

if.end:                                           ; preds = %if.then, %entry
  call void @llvm.dbg.declare(metadata ptr %i, metadata !60, metadata !DIExpression()), !dbg !62
  store i32 0, ptr %i, align 4, !dbg !62
  br label %for.cond, !dbg !63

for.cond:                                         ; preds = %for.inc, %if.end
  %5 = load i32, ptr %i, align 4, !dbg !64
  %6 = load i32, ptr %k.addr, align 4, !dbg !66
  %cmp2 = icmp slt i32 %5, %6, !dbg !67
  br i1 %cmp2, label %for.body, label %for.end, !dbg !68

for.body:                                         ; preds = %for.cond
  %7 = load i32, ptr %n.addr, align 4, !dbg !69
  %8 = load i32, ptr %i, align 4, !dbg !71
  %sub3 = sub nsw i32 %7, %8, !dbg !72
  %9 = load i32, ptr %res, align 4, !dbg !73
  %mul = mul nsw i32 %9, %sub3, !dbg !73
  store i32 %mul, ptr %res, align 4, !dbg !73
  %10 = load i32, ptr %i, align 4, !dbg !74
  %add = add nsw i32 %10, 1, !dbg !75
  %11 = load i32, ptr %res, align 4, !dbg !76
  %div = sdiv i32 %11, %add, !dbg !76
  store i32 %div, ptr %res, align 4, !dbg !76
  br label %for.inc, !dbg !77

for.inc:                                          ; preds = %for.body
  %12 = load i32, ptr %i, align 4, !dbg !78
  %inc = add nsw i32 %12, 1, !dbg !78
  store i32 %inc, ptr %i, align 4, !dbg !78
  br label %for.cond, !dbg !79, !llvm.loop !80

for.end:                                          ; preds = %for.cond
  %13 = load i32, ptr %res, align 4, !dbg !83
  ret i32 %13, !dbg !84
}

; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define void @printPascalTriangle(i32 noundef %numRows) #0 !dbg !85 {
entry:
  %numRows.addr = alloca i32, align 4
  %line = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %numRows, ptr %numRows.addr, align 4
  call void @llvm.dbg.declare(metadata ptr %numRows.addr, metadata !88, metadata !DIExpression()), !dbg !89
  call void @llvm.dbg.declare(metadata ptr %line, metadata !90, metadata !DIExpression()), !dbg !92
  store i32 0, ptr %line, align 4, !dbg !92
  br label %for.cond, !dbg !93

for.cond:                                         ; preds = %for.inc6, %entry
  %0 = load i32, ptr %line, align 4, !dbg !94
  %1 = load i32, ptr %numRows.addr, align 4, !dbg !96
  %cmp = icmp slt i32 %0, %1, !dbg !97
  br i1 %cmp, label %for.body, label %for.end8, !dbg !98

for.body:                                         ; preds = %for.cond
  call void @llvm.dbg.declare(metadata ptr %i, metadata !99, metadata !DIExpression()), !dbg !102
  store i32 0, ptr %i, align 4, !dbg !102
  br label %for.cond1, !dbg !103

for.cond1:                                        ; preds = %for.inc, %for.body
  %2 = load i32, ptr %i, align 4, !dbg !104
  %3 = load i32, ptr %line, align 4, !dbg !106
  %cmp2 = icmp sle i32 %2, %3, !dbg !107
  br i1 %cmp2, label %for.body3, label %for.end, !dbg !108

for.body3:                                        ; preds = %for.cond1
  %4 = load i32, ptr %line, align 4, !dbg !109
  %5 = load i32, ptr %i, align 4, !dbg !110
  %call = call i32 @binomialCoeff(i32 noundef %4, i32 noundef %5), !dbg !111
  %call4 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %call), !dbg !112
  br label %for.inc, !dbg !112

for.inc:                                          ; preds = %for.body3
  %6 = load i32, ptr %i, align 4, !dbg !113
  %inc = add nsw i32 %6, 1, !dbg !113
  store i32 %inc, ptr %i, align 4, !dbg !113
  br label %for.cond1, !dbg !114, !llvm.loop !115

for.end:                                          ; preds = %for.cond1
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str.1), !dbg !117
  br label %for.inc6, !dbg !118

for.inc6:                                         ; preds = %for.end
  %7 = load i32, ptr %line, align 4, !dbg !119
  %inc7 = add nsw i32 %7, 1, !dbg !119
  store i32 %inc7, ptr %line, align 4, !dbg !119
  br label %for.cond, !dbg !120, !llvm.loop !121

for.end8:                                         ; preds = %for.cond
  ret void, !dbg !123
}

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @main() #0 !dbg !124 {
entry:
  %retval = alloca i32, align 4
  %numRows = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  call void @llvm.dbg.declare(metadata ptr %numRows, metadata !127, metadata !DIExpression()), !dbg !128
  %call = call i32 (ptr, ...) @printf(ptr noundef @.str.2), !dbg !129
  %call1 = call i32 (ptr, ...) @scanf(ptr noundef @.str.3, ptr noundef %numRows), !dbg !130
  %0 = load i32, ptr %numRows, align 4, !dbg !131
  %call2 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %0), !dbg !132
  %1 = load i32, ptr %numRows, align 4, !dbg !133
  call void @printPascalTriangle(i32 noundef %1), !dbg !134
  ret i32 0, !dbg !135
}

declare i32 @scanf(ptr noundef, ...) #2

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #1 = { nocallback nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!27, !28, !29, !30, !31, !32, !33}
!llvm.dbg.cu = !{!34}
!llvm.ident = !{!36}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(scope: null, file: !2, line: 19, type: !3, isLocal: true, isDefinition: true)
!2 = !DIFile(filename: "example2.c", directory: "/Users/den184/Documents/UNSW/SVF/SVF-Python")
!3 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 32, elements: !5)
!4 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!5 = !{!6}
!6 = !DISubrange(count: 4)
!7 = !DIGlobalVariableExpression(var: !8, expr: !DIExpression())
!8 = distinct !DIGlobalVariable(scope: null, file: !2, line: 20, type: !9, isLocal: true, isDefinition: true)
!9 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 16, elements: !10)
!10 = !{!11}
!11 = !DISubrange(count: 2)
!12 = !DIGlobalVariableExpression(var: !13, expr: !DIExpression())
!13 = distinct !DIGlobalVariable(scope: null, file: !2, line: 26, type: !14, isLocal: true, isDefinition: true)
!14 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 392, elements: !15)
!15 = !{!16}
!16 = !DISubrange(count: 49)
!17 = !DIGlobalVariableExpression(var: !18, expr: !DIExpression())
!18 = distinct !DIGlobalVariable(scope: null, file: !2, line: 27, type: !19, isLocal: true, isDefinition: true)
!19 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 24, elements: !20)
!20 = !{!21}
!21 = !DISubrange(count: 3)
!22 = !DIGlobalVariableExpression(var: !23, expr: !DIExpression())
!23 = distinct !DIGlobalVariable(scope: null, file: !2, line: 28, type: !24, isLocal: true, isDefinition: true)
!24 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 264, elements: !25)
!25 = !{!26}
!26 = !DISubrange(count: 33)
!27 = !{i32 2, !"SDK Version", [2 x i32] [i32 14, i32 4]}
!28 = !{i32 7, !"Dwarf Version", i32 4}
!29 = !{i32 2, !"Debug Info Version", i32 3}
!30 = !{i32 1, !"wchar_size", i32 4}
!31 = !{i32 8, !"PIC Level", i32 2}
!32 = !{i32 7, !"uwtable", i32 1}
!33 = !{i32 7, !"frame-pointer", i32 1}
!34 = distinct !DICompileUnit(language: DW_LANG_C99, file: !2, producer: "Apple clang version 15.0.0 (clang-1500.3.9.4)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !35, splitDebugInlining: false, nameTableKind: None, sysroot: "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk", sdk: "MacOSX.sdk")
!35 = !{!0, !7, !12, !17, !22}
!36 = !{!"Apple clang version 15.0.0 (clang-1500.3.9.4)"}
!37 = distinct !DISubprogram(name: "binomialCoeff", scope: !2, file: !2, line: 4, type: !38, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !34, retainedNodes: !41)
!38 = !DISubroutineType(types: !39)
!39 = !{!40, !40, !40}
!40 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!41 = !{}
!42 = !DILocalVariable(name: "n", arg: 1, scope: !37, file: !2, line: 4, type: !40)
!43 = !DILocation(line: 4, column: 23, scope: !37)
!44 = !DILocalVariable(name: "k", arg: 2, scope: !37, file: !2, line: 4, type: !40)
!45 = !DILocation(line: 4, column: 30, scope: !37)
!46 = !DILocalVariable(name: "res", scope: !37, file: !2, line: 5, type: !40)
!47 = !DILocation(line: 5, column: 9, scope: !37)
!48 = !DILocation(line: 6, column: 9, scope: !49)
!49 = distinct !DILexicalBlock(scope: !37, file: !2, line: 6, column: 9)
!50 = !DILocation(line: 6, column: 13, scope: !49)
!51 = !DILocation(line: 6, column: 17, scope: !49)
!52 = !DILocation(line: 6, column: 15, scope: !49)
!53 = !DILocation(line: 6, column: 11, scope: !49)
!54 = !DILocation(line: 6, column: 9, scope: !37)
!55 = !DILocation(line: 7, column: 13, scope: !49)
!56 = !DILocation(line: 7, column: 17, scope: !49)
!57 = !DILocation(line: 7, column: 15, scope: !49)
!58 = !DILocation(line: 7, column: 11, scope: !49)
!59 = !DILocation(line: 7, column: 9, scope: !49)
!60 = !DILocalVariable(name: "i", scope: !61, file: !2, line: 8, type: !40)
!61 = distinct !DILexicalBlock(scope: !37, file: !2, line: 8, column: 5)
!62 = !DILocation(line: 8, column: 14, scope: !61)
!63 = !DILocation(line: 8, column: 10, scope: !61)
!64 = !DILocation(line: 8, column: 21, scope: !65)
!65 = distinct !DILexicalBlock(scope: !61, file: !2, line: 8, column: 5)
!66 = !DILocation(line: 8, column: 25, scope: !65)
!67 = !DILocation(line: 8, column: 23, scope: !65)
!68 = !DILocation(line: 8, column: 5, scope: !61)
!69 = !DILocation(line: 9, column: 17, scope: !70)
!70 = distinct !DILexicalBlock(scope: !65, file: !2, line: 8, column: 33)
!71 = !DILocation(line: 9, column: 21, scope: !70)
!72 = !DILocation(line: 9, column: 19, scope: !70)
!73 = !DILocation(line: 9, column: 13, scope: !70)
!74 = !DILocation(line: 10, column: 17, scope: !70)
!75 = !DILocation(line: 10, column: 19, scope: !70)
!76 = !DILocation(line: 10, column: 13, scope: !70)
!77 = !DILocation(line: 11, column: 5, scope: !70)
!78 = !DILocation(line: 8, column: 28, scope: !65)
!79 = !DILocation(line: 8, column: 5, scope: !65)
!80 = distinct !{!80, !68, !81, !82}
!81 = !DILocation(line: 11, column: 5, scope: !61)
!82 = !{!"llvm.loop.mustprogress"}
!83 = !DILocation(line: 12, column: 12, scope: !37)
!84 = !DILocation(line: 12, column: 5, scope: !37)
!85 = distinct !DISubprogram(name: "printPascalTriangle", scope: !2, file: !2, line: 16, type: !86, scopeLine: 16, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !34, retainedNodes: !41)
!86 = !DISubroutineType(types: !87)
!87 = !{null, !40}
!88 = !DILocalVariable(name: "numRows", arg: 1, scope: !85, file: !2, line: 16, type: !40)
!89 = !DILocation(line: 16, column: 30, scope: !85)
!90 = !DILocalVariable(name: "line", scope: !91, file: !2, line: 17, type: !40)
!91 = distinct !DILexicalBlock(scope: !85, file: !2, line: 17, column: 5)
!92 = !DILocation(line: 17, column: 14, scope: !91)
!93 = !DILocation(line: 17, column: 10, scope: !91)
!94 = !DILocation(line: 17, column: 24, scope: !95)
!95 = distinct !DILexicalBlock(scope: !91, file: !2, line: 17, column: 5)
!96 = !DILocation(line: 17, column: 31, scope: !95)
!97 = !DILocation(line: 17, column: 29, scope: !95)
!98 = !DILocation(line: 17, column: 5, scope: !91)
!99 = !DILocalVariable(name: "i", scope: !100, file: !2, line: 18, type: !40)
!100 = distinct !DILexicalBlock(scope: !101, file: !2, line: 18, column: 9)
!101 = distinct !DILexicalBlock(scope: !95, file: !2, line: 17, column: 48)
!102 = !DILocation(line: 18, column: 18, scope: !100)
!103 = !DILocation(line: 18, column: 14, scope: !100)
!104 = !DILocation(line: 18, column: 25, scope: !105)
!105 = distinct !DILexicalBlock(scope: !100, file: !2, line: 18, column: 9)
!106 = !DILocation(line: 18, column: 30, scope: !105)
!107 = !DILocation(line: 18, column: 27, scope: !105)
!108 = !DILocation(line: 18, column: 9, scope: !100)
!109 = !DILocation(line: 19, column: 41, scope: !105)
!110 = !DILocation(line: 19, column: 47, scope: !105)
!111 = !DILocation(line: 19, column: 27, scope: !105)
!112 = !DILocation(line: 19, column: 13, scope: !105)
!113 = !DILocation(line: 18, column: 36, scope: !105)
!114 = !DILocation(line: 18, column: 9, scope: !105)
!115 = distinct !{!115, !108, !116, !82}
!116 = !DILocation(line: 19, column: 49, scope: !100)
!117 = !DILocation(line: 20, column: 9, scope: !101)
!118 = !DILocation(line: 21, column: 5, scope: !101)
!119 = !DILocation(line: 17, column: 40, scope: !95)
!120 = !DILocation(line: 17, column: 5, scope: !95)
!121 = distinct !{!121, !98, !122, !82}
!122 = !DILocation(line: 21, column: 5, scope: !91)
!123 = !DILocation(line: 22, column: 1, scope: !85)
!124 = distinct !DISubprogram(name: "main", scope: !2, file: !2, line: 24, type: !125, scopeLine: 24, spFlags: DISPFlagDefinition, unit: !34, retainedNodes: !41)
!125 = !DISubroutineType(types: !126)
!126 = !{!40}
!127 = !DILocalVariable(name: "numRows", scope: !124, file: !2, line: 25, type: !40)
!128 = !DILocation(line: 25, column: 9, scope: !124)
!129 = !DILocation(line: 26, column: 5, scope: !124)
!130 = !DILocation(line: 27, column: 5, scope: !124)
!131 = !DILocation(line: 28, column: 49, scope: !124)
!132 = !DILocation(line: 28, column: 5, scope: !124)
!133 = !DILocation(line: 29, column: 25, scope: !124)
!134 = !DILocation(line: 29, column: 5, scope: !124)
!135 = !DILocation(line: 30, column: 5, scope: !124)
