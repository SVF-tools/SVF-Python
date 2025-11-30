; ModuleID = 'demo_mta.c'
source_filename = "demo_mta.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx13.0.0"

%struct._opaque_pthread_t = type { i64, %struct.__darwin_pthread_handler_rec*, [8176 x i8] }
%struct.__darwin_pthread_handler_rec = type { void (i8*)*, i8*, %struct.__darwin_pthread_handler_rec* }
%struct._opaque_pthread_attr_t = type { i64, [56 x i8] }

; Function Attrs: noinline nounwind optnone ssp uwtable
define i8* @thread_function0(i8* noundef %0) #0 !dbg !15 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  call void @llvm.dbg.declare(metadata i8** %2, metadata !19, metadata !DIExpression()), !dbg !20
  ret i8* null, !dbg !21
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone ssp uwtable
define i8* @thread_function1(i8* noundef %0) #0 !dbg !22 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  call void @llvm.dbg.declare(metadata i8** %2, metadata !23, metadata !DIExpression()), !dbg !24
  ret i8* null, !dbg !25
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 !dbg !26 {
  %1 = alloca %struct._opaque_pthread_t*, align 8
  %2 = alloca %struct._opaque_pthread_t*, align 8
  call void @llvm.dbg.declare(metadata %struct._opaque_pthread_t** %1, metadata !30, metadata !DIExpression()), !dbg !55
  call void @llvm.dbg.declare(metadata %struct._opaque_pthread_t** %2, metadata !56, metadata !DIExpression()), !dbg !57
  %3 = call i32 @pthread_create(%struct._opaque_pthread_t** noundef %1, %struct._opaque_pthread_attr_t* noundef null, i8* (i8*)* noundef @thread_function0, i8* noundef null), !dbg !58
  %4 = call i32 @pthread_create(%struct._opaque_pthread_t** noundef %2, %struct._opaque_pthread_attr_t* noundef null, i8* (i8*)* noundef @thread_function1, i8* noundef null), !dbg !59
  %5 = load %struct._opaque_pthread_t*, %struct._opaque_pthread_t** %1, align 8, !dbg !60
  %6 = call i32 @"\01_pthread_join"(%struct._opaque_pthread_t* noundef %5, i8** noundef null), !dbg !61
  %7 = load %struct._opaque_pthread_t*, %struct._opaque_pthread_t** %2, align 8, !dbg !62
  %8 = call i32 @"\01_pthread_join"(%struct._opaque_pthread_t* noundef %7, i8** noundef null), !dbg !63
  ret i32 0, !dbg !64
}

declare i32 @pthread_create(%struct._opaque_pthread_t** noundef, %struct._opaque_pthread_attr_t* noundef, i8* (i8*)* noundef, i8* noundef) #2

declare i32 @"\01_pthread_join"(%struct._opaque_pthread_t* noundef, i8** noundef) #2

attributes #0 = { noinline nounwind optnone ssp uwtable "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+v8.5a,+zcm,+zcz" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+v8.5a,+zcm,+zcz" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!4, !5, !6, !7, !8, !9, !10, !11, !12, !13}
!llvm.ident = !{!14}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Homebrew clang version 14.0.6", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !2, splitDebugInlining: false, nameTableKind: None, sysroot: "/Library/Developer/CommandLineTools/SDKs/MacOSX13.sdk", sdk: "MacOSX13.sdk")
!1 = !DIFile(filename: "demo_mta.c", directory: "/Users/joel/Projects/SVF-Python/demo")
!2 = !{!3}
!3 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!4 = !{i32 7, !"Dwarf Version", i32 4}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{i32 1, !"wchar_size", i32 4}
!7 = !{i32 1, !"branch-target-enforcement", i32 0}
!8 = !{i32 1, !"sign-return-address", i32 0}
!9 = !{i32 1, !"sign-return-address-all", i32 0}
!10 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!11 = !{i32 7, !"PIC Level", i32 2}
!12 = !{i32 7, !"uwtable", i32 1}
!13 = !{i32 7, !"frame-pointer", i32 1}
!14 = !{!"Homebrew clang version 14.0.6"}
!15 = distinct !DISubprogram(name: "thread_function0", scope: !1, file: !1, line: 3, type: !16, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !18)
!16 = !DISubroutineType(types: !17)
!17 = !{!3, !3}
!18 = !{}
!19 = !DILocalVariable(name: "arg", arg: 1, scope: !15, file: !1, line: 3, type: !3)
!20 = !DILocation(line: 3, column: 30, scope: !15)
!21 = !DILocation(line: 5, column: 5, scope: !15)
!22 = distinct !DISubprogram(name: "thread_function1", scope: !1, file: !1, line: 8, type: !16, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !18)
!23 = !DILocalVariable(name: "arg", arg: 1, scope: !22, file: !1, line: 8, type: !3)
!24 = !DILocation(line: 8, column: 30, scope: !22)
!25 = !DILocation(line: 10, column: 5, scope: !22)
!26 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 13, type: !27, scopeLine: 14, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !18)
!27 = !DISubroutineType(types: !28)
!28 = !{!29}
!29 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!30 = !DILocalVariable(name: "thread1", scope: !26, file: !1, line: 15, type: !31)
!31 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !32, line: 31, baseType: !33)
!32 = !DIFile(filename: "/Library/Developer/CommandLineTools/SDKs/MacOSX13.sdk/usr/include/sys/_pthread/_pthread_t.h", directory: "")
!33 = !DIDerivedType(tag: DW_TAG_typedef, name: "__darwin_pthread_t", file: !34, line: 118, baseType: !35)
!34 = !DIFile(filename: "/Library/Developer/CommandLineTools/SDKs/MacOSX13.sdk/usr/include/sys/_pthread/_pthread_types.h", directory: "")
!35 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !36, size: 64)
!36 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_opaque_pthread_t", file: !34, line: 103, size: 65536, elements: !37)
!37 = !{!38, !40, !50}
!38 = !DIDerivedType(tag: DW_TAG_member, name: "__sig", scope: !36, file: !34, line: 104, baseType: !39, size: 64)
!39 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "__cleanup_stack", scope: !36, file: !34, line: 105, baseType: !41, size: 64, offset: 64)
!41 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !42, size: 64)
!42 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__darwin_pthread_handler_rec", file: !34, line: 57, size: 192, elements: !43)
!43 = !{!44, !48, !49}
!44 = !DIDerivedType(tag: DW_TAG_member, name: "__routine", scope: !42, file: !34, line: 58, baseType: !45, size: 64)
!45 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !46, size: 64)
!46 = !DISubroutineType(types: !47)
!47 = !{null, !3}
!48 = !DIDerivedType(tag: DW_TAG_member, name: "__arg", scope: !42, file: !34, line: 59, baseType: !3, size: 64, offset: 64)
!49 = !DIDerivedType(tag: DW_TAG_member, name: "__next", scope: !42, file: !34, line: 60, baseType: !41, size: 64, offset: 128)
!50 = !DIDerivedType(tag: DW_TAG_member, name: "__opaque", scope: !36, file: !34, line: 106, baseType: !51, size: 65408, offset: 128)
!51 = !DICompositeType(tag: DW_TAG_array_type, baseType: !52, size: 65408, elements: !53)
!52 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!53 = !{!54}
!54 = !DISubrange(count: 8176)
!55 = !DILocation(line: 15, column: 15, scope: !26)
!56 = !DILocalVariable(name: "thread2", scope: !26, file: !1, line: 15, type: !31)
!57 = !DILocation(line: 15, column: 24, scope: !26)
!58 = !DILocation(line: 17, column: 5, scope: !26)
!59 = !DILocation(line: 18, column: 5, scope: !26)
!60 = !DILocation(line: 20, column: 18, scope: !26)
!61 = !DILocation(line: 20, column: 5, scope: !26)
!62 = !DILocation(line: 21, column: 18, scope: !26)
!63 = !DILocation(line: 21, column: 5, scope: !26)
!64 = !DILocation(line: 22, column: 1, scope: !26)
