; ModuleID = 'demo.ll'
source_filename = "demo.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind ssp uwtable(sync)
define i32 @add_or_sub(i32 noundef %a, i32 noundef %b, i32 noundef %flag) #0 {
entry:
  %tobool = icmp ne i32 %flag, 0
  br i1 %tobool, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %add = add nsw i32 %a, %b
  br label %if.end

if.else:                                          ; preds = %entry
  %sub = sub nsw i32 %a, %b
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %result.0 = phi i32 [ %add, %if.then ], [ %sub, %if.else ]
  ret i32 %result.0
}

; Function Attrs: noinline nounwind ssp uwtable(sync)
define i32 @main() #0 {
entry:
  %0 = alloca i8, i64 8, align 8
  %arrayidx = getelementptr inbounds i32, ptr %0, i64 0
  store i32 5, ptr %arrayidx, align 4
  %arrayidx1 = getelementptr inbounds i32, ptr %0, i64 1
  store i32 3, ptr %arrayidx1, align 4
  %arrayidx2 = getelementptr inbounds i32, ptr %0, i64 0
  %1 = load i32, ptr %arrayidx2, align 4
  %arrayidx3 = getelementptr inbounds i32, ptr %0, i64 1
  %2 = load i32, ptr %arrayidx3, align 4
  %call = call i32 @add_or_sub(i32 noundef %1, i32 noundef %2, i32 noundef 1)
  %3 = alloca i8, i64 8, align 8
  %4 = call i64 @llvm.objectsize.i64.p0(ptr %3, i1 false, i1 true, i1 false)
  %call4 = call ptr @__memcpy_chk(ptr noundef %3, ptr noundef %0, i64 noundef 8, i64 noundef %4) #4
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %call)
  ret i32 0
}

; Function Attrs: nounwind
declare ptr @__memcpy_chk(ptr noundef, ptr noundef, i64 noundef, i64 noundef) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.objectsize.i64.p0(ptr, i1 immarg, i1 immarg, i1 immarg) #2

declare i32 @printf(ptr noundef, ...) #3

attributes #0 = { noinline nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #1 = { nounwind "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 1}
!3 = !{i32 7, !"frame-pointer", i32 1}
!4 = !{!"Homebrew clang version 16.0.6"}
