; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@gval = dso_local global i32 3, align 4, !dbg !0
@buf = dso_local global [4 x i32] zeroinitializer, align 16, !dbg !41

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @fun(i32* %a) #0 !dbg !50 {
entry:
  %a.addr = alloca i32*, align 8
  store i32* %a, i32** %a.addr, align 8
  call void @llvm.dbg.declare(metadata i32** %a.addr, metadata !54, metadata !DIExpression()), !dbg !55
  %0 = load i32*, i32** %a.addr, align 8, !dbg !56
  %1 = load i32, i32* %0, align 4, !dbg !57
  ret i32 %1, !dbg !58
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @fun1(i32 %a, i32 %b) #0 !dbg !59 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !62, metadata !DIExpression()), !dbg !63
  store i32 %b, i32* %b.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %b.addr, metadata !64, metadata !DIExpression()), !dbg !65
  %0 = load i32, i32* %b.addr, align 4, !dbg !66
  %add = add nsw i32 %0, 1, !dbg !67
  store i32 %add, i32* %a.addr, align 4, !dbg !68
  %1 = load i32, i32* %a.addr, align 4, !dbg !69
  ret i32 %1, !dbg !70
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @fun2(i32* %buf, i32 %len) #0 !dbg !71 {
entry:
  %buf.addr = alloca i32*, align 8
  %len.addr = alloca i32, align 4
  %a = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %buf, i32** %buf.addr, align 8
  call void @llvm.dbg.declare(metadata i32** %buf.addr, metadata !74, metadata !DIExpression()), !dbg !75
  store i32 %len, i32* %len.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %len.addr, metadata !76, metadata !DIExpression()), !dbg !77
  call void @llvm.dbg.declare(metadata i32* %a, metadata !78, metadata !DIExpression()), !dbg !79
  store i32 0, i32* %a, align 4, !dbg !79
  call void @llvm.dbg.declare(metadata i32* %i, metadata !80, metadata !DIExpression()), !dbg !82
  store i32 0, i32* %i, align 4, !dbg !82
  br label %for.cond, !dbg !83

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4, !dbg !84
  %1 = load i32, i32* %len.addr, align 4, !dbg !86
  %cmp = icmp slt i32 %0, %1, !dbg !87
  br i1 %cmp, label %for.body, label %for.end, !dbg !88

for.body:                                         ; preds = %for.cond
  %2 = load i32, i32* %a, align 4, !dbg !89
  %3 = load i32*, i32** %buf.addr, align 8, !dbg !91
  %4 = load i32, i32* %i, align 4, !dbg !92
  %idxprom = sext i32 %4 to i64, !dbg !91
  %arrayidx = getelementptr inbounds i32, i32* %3, i64 %idxprom, !dbg !91
  %5 = load i32, i32* %arrayidx, align 4, !dbg !91
  %add = add nsw i32 %2, %5, !dbg !93
  store i32 %add, i32* %a, align 4, !dbg !94
  br label %for.inc, !dbg !95

for.inc:                                          ; preds = %for.body
  %6 = load i32, i32* %i, align 4, !dbg !96
  %inc = add nsw i32 %6, 1, !dbg !96
  store i32 %inc, i32* %i, align 4, !dbg !96
  br label %for.cond, !dbg !97, !llvm.loop !98

for.end:                                          ; preds = %for.cond
  ret i32 1, !dbg !101
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fun4() #0 !dbg !102 {
entry:
  %a = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %a, metadata !105, metadata !DIExpression()), !dbg !106
  store i32 2, i32* %a, align 4, !dbg !107
  ret void, !dbg !108
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !109 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %lbuf = alloca [4 x i32], align 16
  %_qzz_res = alloca i32, align 4
  %_zzq_args = alloca [6 x i64], align 16
  %_zzq_result = alloca i64, align 8
  %tmp = alloca i64, align 8
  %tmp7 = alloca i32, align 4
  %ptr = alloca i32*, align 8
  %c = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %a, metadata !112, metadata !DIExpression()), !dbg !113
  call void @llvm.dbg.declare(metadata i32* %b, metadata !114, metadata !DIExpression()), !dbg !115
  store i32 3, i32* %b, align 4, !dbg !115
  call void @llvm.dbg.declare(metadata [4 x i32]* %lbuf, metadata !116, metadata !DIExpression()), !dbg !117
  call void @llvm.dbg.declare(metadata i32* %_qzz_res, metadata !118, metadata !DIExpression()), !dbg !120
  store i32 0, i32* %_qzz_res, align 4, !dbg !120
  br label %do.body, !dbg !120

do.body:                                          ; preds = %entry
  call void @llvm.dbg.declare(metadata [6 x i64]* %_zzq_args, metadata !121, metadata !DIExpression()), !dbg !128
  call void @llvm.dbg.declare(metadata i64* %_zzq_result, metadata !129, metadata !DIExpression()), !dbg !128
  %arrayidx = getelementptr inbounds [6 x i64], [6 x i64]* %_zzq_args, i64 0, i64 0, !dbg !128
  store volatile i64 1178796048, i64* %arrayidx, align 16, !dbg !128
  %0 = ptrtoint i32* %b to i64, !dbg !128
  %arrayidx1 = getelementptr inbounds [6 x i64], [6 x i64]* %_zzq_args, i64 0, i64 1, !dbg !128
  store volatile i64 %0, i64* %arrayidx1, align 8, !dbg !128
  %arrayidx2 = getelementptr inbounds [6 x i64], [6 x i64]* %_zzq_args, i64 0, i64 2, !dbg !128
  store volatile i64 0, i64* %arrayidx2, align 16, !dbg !128
  %arrayidx3 = getelementptr inbounds [6 x i64], [6 x i64]* %_zzq_args, i64 0, i64 3, !dbg !128
  store volatile i64 0, i64* %arrayidx3, align 8, !dbg !128
  %arrayidx4 = getelementptr inbounds [6 x i64], [6 x i64]* %_zzq_args, i64 0, i64 4, !dbg !128
  store volatile i64 0, i64* %arrayidx4, align 16, !dbg !128
  %arrayidx5 = getelementptr inbounds [6 x i64], [6 x i64]* %_zzq_args, i64 0, i64 5, !dbg !128
  store volatile i64 0, i64* %arrayidx5, align 8, !dbg !128
  %arrayidx6 = getelementptr inbounds [6 x i64], [6 x i64]* %_zzq_args, i64 0, i64 0, !dbg !128
  %1 = call i64 asm sideeffect "rolq $$3,  %rdi ; rolq $$13, %rdi\0A\09rolq $$61, %rdi ; rolq $$51, %rdi\0A\09xchgq %rbx,%rbx", "={dx},{ax},0,~{cc},~{memory},~{dirflag},~{fpsr},~{flags}"(i64* %arrayidx6, i64 0) #3, !dbg !128, !srcloc !130
  store volatile i64 %1, i64* %_zzq_result, align 8, !dbg !128
  %2 = load volatile i64, i64* %_zzq_result, align 8, !dbg !128
  store i64 %2, i64* %tmp, align 8, !dbg !128
  %3 = load i64, i64* %tmp, align 8, !dbg !128
  %conv = trunc i64 %3 to i32, !dbg !131
  store i32 %conv, i32* %_qzz_res, align 4, !dbg !131
  br label %do.end, !dbg !131

do.end:                                           ; preds = %do.body
  %4 = load i32, i32* %_qzz_res, align 4, !dbg !120
  store i32 %4, i32* %tmp7, align 4, !dbg !131
  %5 = load i32, i32* %tmp7, align 4, !dbg !120
  %6 = load i32, i32* %b, align 4, !dbg !132
  %add = add nsw i32 %6, 1, !dbg !133
  store i32 %add, i32* %a, align 4, !dbg !134
  %7 = load i32, i32* %a, align 4, !dbg !135
  %sub = sub nsw i32 %7, 1, !dbg !136
  store i32 %sub, i32* %a, align 4, !dbg !137
  %8 = load i32, i32* %a, align 4, !dbg !138
  %9 = load i32, i32* %a, align 4, !dbg !139
  %mul = mul nsw i32 %8, %9, !dbg !140
  store i32 %mul, i32* %a, align 4, !dbg !141
  store i32 3, i32* %a, align 4, !dbg !142
  call void @llvm.dbg.declare(metadata i32** %ptr, metadata !143, metadata !DIExpression()), !dbg !144
  %call = call i8* @malloc(i64 8), !dbg !145
  %10 = bitcast i8* %call to i32*, !dbg !146
  store i32* %10, i32** %ptr, align 8, !dbg !144
  %11 = load i32, i32* %a, align 4, !dbg !147
  %12 = load i32*, i32** %ptr, align 8, !dbg !148
  %arrayidx8 = getelementptr inbounds i32, i32* %12, i64 0, !dbg !148
  store i32 %11, i32* %arrayidx8, align 4, !dbg !149
  %13 = load i32, i32* %b, align 4, !dbg !150
  %14 = load i32*, i32** %ptr, align 8, !dbg !151
  %arrayidx9 = getelementptr inbounds i32, i32* %14, i64 1, !dbg !151
  store i32 %13, i32* %arrayidx9, align 4, !dbg !152
  %15 = load i32, i32* %a, align 4, !dbg !153
  %tobool = icmp ne i32 %15, 0, !dbg !153
  br i1 %tobool, label %if.then, label %if.end, !dbg !155

if.then:                                          ; preds = %do.end
  %16 = load i32, i32* %a, align 4, !dbg !156
  %add10 = add nsw i32 %16, 2, !dbg !158
  store i32 %add10, i32* %a, align 4, !dbg !159
  %17 = load i32, i32* %b, align 4, !dbg !160
  %add11 = add nsw i32 %17, 3, !dbg !161
  store i32 %add11, i32* @gval, align 4, !dbg !162
  %18 = load i32, i32* %a, align 4, !dbg !163
  store i32 %18, i32* getelementptr inbounds ([4 x i32], [4 x i32]* @buf, i64 0, i64 0), align 16, !dbg !164
  %19 = load i32, i32* %b, align 4, !dbg !165
  store i32 %19, i32* getelementptr inbounds ([4 x i32], [4 x i32]* @buf, i64 0, i64 1), align 4, !dbg !166
  store i32 0, i32* getelementptr inbounds ([4 x i32], [4 x i32]* @buf, i64 0, i64 2), align 8, !dbg !167
  %20 = load i32, i32* @gval, align 4, !dbg !168
  store i32 %20, i32* getelementptr inbounds ([4 x i32], [4 x i32]* @buf, i64 0, i64 3), align 4, !dbg !169
  %21 = load i32, i32* %a, align 4, !dbg !170
  %arrayidx12 = getelementptr inbounds [4 x i32], [4 x i32]* %lbuf, i64 0, i64 0, !dbg !171
  store i32 %21, i32* %arrayidx12, align 16, !dbg !172
  %22 = load i32, i32* %b, align 4, !dbg !173
  %arrayidx13 = getelementptr inbounds [4 x i32], [4 x i32]* %lbuf, i64 0, i64 1, !dbg !174
  store i32 %22, i32* %arrayidx13, align 4, !dbg !175
  %23 = load i32, i32* %a, align 4, !dbg !176
  %add14 = add nsw i32 %23, 3, !dbg !177
  store i32 %add14, i32* %a, align 4, !dbg !178
  call void @llvm.dbg.declare(metadata i32* %c, metadata !179, metadata !DIExpression()), !dbg !180
  %24 = load i32, i32* %a, align 4, !dbg !181
  store i32 %24, i32* %c, align 4, !dbg !180
  %25 = load i32, i32* @gval, align 4, !dbg !182
  %26 = load i32, i32* %c, align 4, !dbg !183
  %add15 = add nsw i32 %25, %26, !dbg !184
  store i32 %add15, i32* %c, align 4, !dbg !185
  br label %if.end, !dbg !186

if.end:                                           ; preds = %if.then, %do.end
  call void @fun4(), !dbg !187
  %call16 = call i32 @fun(i32* %b), !dbg !188
  store i32 %call16, i32* %a, align 4, !dbg !189
  %27 = load i32, i32* %a, align 4, !dbg !190
  %28 = load i32, i32* %b, align 4, !dbg !191
  %call17 = call i32 @fun1(i32 %27, i32 %28), !dbg !192
  store i32 %call17, i32* %a, align 4, !dbg !193
  %call18 = call i32 @fun2(i32* getelementptr inbounds ([4 x i32], [4 x i32]* @buf, i64 0, i64 0), i32 4), !dbg !194
  store i32 %call18, i32* %a, align 4, !dbg !195
  %29 = load i32*, i32** %ptr, align 8, !dbg !196
  %call19 = call i32 @fun2(i32* %29, i32 2), !dbg !197
  store i32 %call19, i32* %a, align 4, !dbg !198
  %call20 = call i32 @fun(i32* %a), !dbg !199
  store i32 %call20, i32* %b, align 4, !dbg !200
  %30 = load i32, i32* %retval, align 4, !dbg !201
  ret i32 %30, !dbg !201
}

declare dso_local i8* @malloc(i64) #2

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!46, !47, !48}
!llvm.ident = !{!49}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "gval", scope: !2, file: !3, line: 2, type: !39, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "Ubuntu clang version 12.0.0-3ubuntu1~20.04.5", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !36, globals: !40, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "test.c", directory: "/home/raoxue/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp")
!4 = !{!5}
!5 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !6, line: 82, baseType: !7, size: 32, elements: !8)
!6 = !DIFile(filename: "../include/valgrind/flowcheck.h", directory: "/home/raoxue/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/my_examp")
!7 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!8 = !{!9, !10, !11, !12, !13, !14, !15, !16, !17, !18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28, !29, !30, !31, !32, !33, !34, !35}
!9 = !DIEnumerator(name: "VG_USERREQ__MAKE_MEM_NOACCESS", value: 1178796032, isUnsigned: true)
!10 = !DIEnumerator(name: "VG_USERREQ__MAKE_MEM_UNDEFINED", value: 1178796033, isUnsigned: true)
!11 = !DIEnumerator(name: "VG_USERREQ__MAKE_MEM_DEFINED", value: 1178796034, isUnsigned: true)
!12 = !DIEnumerator(name: "VG_USERREQ__DISCARD", value: 1178796035, isUnsigned: true)
!13 = !DIEnumerator(name: "VG_USERREQ__CHECK_MEM_IS_ADDRESSABLE", value: 1178796036, isUnsigned: true)
!14 = !DIEnumerator(name: "VG_USERREQ__CHECK_MEM_IS_DEFINED", value: 1178796037, isUnsigned: true)
!15 = !DIEnumerator(name: "VG_USERREQ__DO_LEAK_CHECK", value: 1178796038, isUnsigned: true)
!16 = !DIEnumerator(name: "VG_USERREQ__COUNT_LEAKS", value: 1178796039, isUnsigned: true)
!17 = !DIEnumerator(name: "VG_USERREQ__GET_VBITS", value: 1178796040, isUnsigned: true)
!18 = !DIEnumerator(name: "VG_USERREQ__SET_VBITS", value: 1178796041, isUnsigned: true)
!19 = !DIEnumerator(name: "VG_USERREQ__CREATE_BLOCK", value: 1178796042, isUnsigned: true)
!20 = !DIEnumerator(name: "VG_USERREQ__MAKE_MEM_DEFINED_IF_ADDRESSABLE", value: 1178796043, isUnsigned: true)
!21 = !DIEnumerator(name: "VG_USERREQ__PUSH_ENCLOSE", value: 1178796044, isUnsigned: true)
!22 = !DIEnumerator(name: "VG_USERREQ__POP_ENCLOSE", value: 1178796045, isUnsigned: true)
!23 = !DIEnumerator(name: "VG_USERREQ__LEAK_WORD", value: 1178796046, isUnsigned: true)
!24 = !DIEnumerator(name: "VG_USERREQ__MAYBE_LEAK_WORD", value: 1178796047, isUnsigned: true)
!25 = !DIEnumerator(name: "VG_USERREQ__TAINT_WORD", value: 1178796048, isUnsigned: true)
!26 = !DIEnumerator(name: "VG_USERREQ__UNTAINT_WORD", value: 1178796049, isUnsigned: true)
!27 = !DIEnumerator(name: "VG_USERREQ__MD5SUM_BLOCK", value: 1178796050, isUnsigned: true)
!28 = !DIEnumerator(name: "VG_USERREQ__PREPARE_ROLLBACK", value: 1178796051, isUnsigned: true)
!29 = !DIEnumerator(name: "VG_USERREQ__PREPARE_ESCAPEE", value: 1178796052, isUnsigned: true)
!30 = !DIEnumerator(name: "VG_USERREQ__DO_ROLLBACK", value: 1178796053, isUnsigned: true)
!31 = !DIEnumerator(name: "VG_USERREQ__NOTE_ITERATION", value: 1178796054, isUnsigned: true)
!32 = !DIEnumerator(name: "VG_USERREQ__COUNT_LEAK_BLOCKS", value: 1178796055, isUnsigned: true)
!33 = !DIEnumerator(name: "VG_USERREQ__ENABLE_ADDR_ERROR_REPORTING_IN_RANGE", value: 1178796056, isUnsigned: true)
!34 = !DIEnumerator(name: "VG_USERREQ__DISABLE_ADDR_ERROR_REPORTING_IN_RANGE", value: 1178796057, isUnsigned: true)
!35 = !DIEnumerator(name: "_VG_USERREQ__FLOWCHECK_RECORD_OVERLAP_ERROR", value: 1178796288, isUnsigned: true)
!36 = !{!37, !38}
!37 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!38 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !39, size: 64)
!39 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!40 = !{!0, !41}
!41 = !DIGlobalVariableExpression(var: !42, expr: !DIExpression())
!42 = distinct !DIGlobalVariable(name: "buf", scope: !2, file: !3, line: 3, type: !43, isLocal: false, isDefinition: true)
!43 = !DICompositeType(tag: DW_TAG_array_type, baseType: !39, size: 128, elements: !44)
!44 = !{!45}
!45 = !DISubrange(count: 4)
!46 = !{i32 7, !"Dwarf Version", i32 4}
!47 = !{i32 2, !"Debug Info Version", i32 3}
!48 = !{i32 1, !"wchar_size", i32 4}
!49 = !{!"Ubuntu clang version 12.0.0-3ubuntu1~20.04.5"}
!50 = distinct !DISubprogram(name: "fun", scope: !3, file: !3, line: 4, type: !51, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !53)
!51 = !DISubroutineType(types: !52)
!52 = !{!39, !38}
!53 = !{}
!54 = !DILocalVariable(name: "a", arg: 1, scope: !50, file: !3, line: 4, type: !38)
!55 = !DILocation(line: 4, column: 14, scope: !50)
!56 = !DILocation(line: 5, column: 13, scope: !50)
!57 = !DILocation(line: 5, column: 12, scope: !50)
!58 = !DILocation(line: 5, column: 5, scope: !50)
!59 = distinct !DISubprogram(name: "fun1", scope: !3, file: !3, line: 7, type: !60, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !53)
!60 = !DISubroutineType(types: !61)
!61 = !{!39, !39, !39}
!62 = !DILocalVariable(name: "a", arg: 1, scope: !59, file: !3, line: 7, type: !39)
!63 = !DILocation(line: 7, column: 14, scope: !59)
!64 = !DILocalVariable(name: "b", arg: 2, scope: !59, file: !3, line: 7, type: !39)
!65 = !DILocation(line: 7, column: 21, scope: !59)
!66 = !DILocation(line: 8, column: 7, scope: !59)
!67 = !DILocation(line: 8, column: 8, scope: !59)
!68 = !DILocation(line: 8, column: 6, scope: !59)
!69 = !DILocation(line: 9, column: 8, scope: !59)
!70 = !DILocation(line: 9, column: 1, scope: !59)
!71 = distinct !DISubprogram(name: "fun2", scope: !3, file: !3, line: 12, type: !72, scopeLine: 13, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !53)
!72 = !DISubroutineType(types: !73)
!73 = !{!39, !38, !39}
!74 = !DILocalVariable(name: "buf", arg: 1, scope: !71, file: !3, line: 12, type: !38)
!75 = !DILocation(line: 12, column: 15, scope: !71)
!76 = !DILocalVariable(name: "len", arg: 2, scope: !71, file: !3, line: 12, type: !39)
!77 = !DILocation(line: 12, column: 23, scope: !71)
!78 = !DILocalVariable(name: "a", scope: !71, file: !3, line: 14, type: !39)
!79 = !DILocation(line: 14, column: 9, scope: !71)
!80 = !DILocalVariable(name: "i", scope: !81, file: !3, line: 15, type: !39)
!81 = distinct !DILexicalBlock(scope: !71, file: !3, line: 15, column: 1)
!82 = !DILocation(line: 15, column: 10, scope: !81)
!83 = !DILocation(line: 15, column: 6, scope: !81)
!84 = !DILocation(line: 15, column: 14, scope: !85)
!85 = distinct !DILexicalBlock(scope: !81, file: !3, line: 15, column: 1)
!86 = !DILocation(line: 15, column: 16, scope: !85)
!87 = !DILocation(line: 15, column: 15, scope: !85)
!88 = !DILocation(line: 15, column: 1, scope: !81)
!89 = !DILocation(line: 17, column: 3, scope: !90)
!90 = distinct !DILexicalBlock(scope: !85, file: !3, line: 16, column: 1)
!91 = !DILocation(line: 17, column: 5, scope: !90)
!92 = !DILocation(line: 17, column: 9, scope: !90)
!93 = !DILocation(line: 17, column: 4, scope: !90)
!94 = !DILocation(line: 17, column: 2, scope: !90)
!95 = !DILocation(line: 18, column: 1, scope: !90)
!96 = !DILocation(line: 15, column: 21, scope: !85)
!97 = !DILocation(line: 15, column: 1, scope: !85)
!98 = distinct !{!98, !88, !99, !100}
!99 = !DILocation(line: 18, column: 1, scope: !81)
!100 = !{!"llvm.loop.mustprogress"}
!101 = !DILocation(line: 19, column: 1, scope: !71)
!102 = distinct !DISubprogram(name: "fun4", scope: !3, file: !3, line: 22, type: !103, scopeLine: 23, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !53)
!103 = !DISubroutineType(types: !104)
!104 = !{null}
!105 = !DILocalVariable(name: "a", scope: !102, file: !3, line: 24, type: !39)
!106 = !DILocation(line: 24, column: 9, scope: !102)
!107 = !DILocation(line: 25, column: 6, scope: !102)
!108 = !DILocation(line: 26, column: 1, scope: !102)
!109 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 28, type: !110, scopeLine: 28, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !53)
!110 = !DISubroutineType(types: !111)
!111 = !{!39}
!112 = !DILocalVariable(name: "a", scope: !109, file: !3, line: 29, type: !39)
!113 = !DILocation(line: 29, column: 9, scope: !109)
!114 = !DILocalVariable(name: "b", scope: !109, file: !3, line: 30, type: !39)
!115 = !DILocation(line: 30, column: 8, scope: !109)
!116 = !DILocalVariable(name: "lbuf", scope: !109, file: !3, line: 31, type: !43)
!117 = !DILocation(line: 31, column: 7, scope: !109)
!118 = !DILocalVariable(name: "_qzz_res", scope: !119, file: !3, line: 32, type: !7)
!119 = distinct !DILexicalBlock(scope: !109, file: !3, line: 32, column: 1)
!120 = !DILocation(line: 32, column: 1, scope: !119)
!121 = !DILocalVariable(name: "_zzq_args", scope: !122, file: !3, line: 32, type: !124)
!122 = distinct !DILexicalBlock(scope: !123, file: !3, line: 32, column: 1)
!123 = distinct !DILexicalBlock(scope: !119, file: !3, line: 32, column: 1)
!124 = !DICompositeType(tag: DW_TAG_array_type, baseType: !125, size: 384, elements: !126)
!125 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !37)
!126 = !{!127}
!127 = !DISubrange(count: 6)
!128 = !DILocation(line: 32, column: 1, scope: !122)
!129 = !DILocalVariable(name: "_zzq_result", scope: !122, file: !3, line: 32, type: !125)
!130 = !{i32 -2147022170, i32 -2147022134, i32 -2147022066}
!131 = !DILocation(line: 32, column: 1, scope: !123)
!132 = !DILocation(line: 35, column: 3, scope: !109)
!133 = !DILocation(line: 35, column: 4, scope: !109)
!134 = !DILocation(line: 35, column: 2, scope: !109)
!135 = !DILocation(line: 36, column: 3, scope: !109)
!136 = !DILocation(line: 36, column: 4, scope: !109)
!137 = !DILocation(line: 36, column: 2, scope: !109)
!138 = !DILocation(line: 37, column: 3, scope: !109)
!139 = !DILocation(line: 37, column: 5, scope: !109)
!140 = !DILocation(line: 37, column: 4, scope: !109)
!141 = !DILocation(line: 37, column: 2, scope: !109)
!142 = !DILocation(line: 38, column: 2, scope: !109)
!143 = !DILocalVariable(name: "ptr", scope: !109, file: !3, line: 39, type: !38)
!144 = !DILocation(line: 39, column: 6, scope: !109)
!145 = !DILocation(line: 39, column: 19, scope: !109)
!146 = !DILocation(line: 39, column: 12, scope: !109)
!147 = !DILocation(line: 40, column: 8, scope: !109)
!148 = !DILocation(line: 40, column: 1, scope: !109)
!149 = !DILocation(line: 40, column: 7, scope: !109)
!150 = !DILocation(line: 41, column: 8, scope: !109)
!151 = !DILocation(line: 41, column: 1, scope: !109)
!152 = !DILocation(line: 41, column: 7, scope: !109)
!153 = !DILocation(line: 42, column: 5, scope: !154)
!154 = distinct !DILexicalBlock(scope: !109, file: !3, line: 42, column: 5)
!155 = !DILocation(line: 42, column: 5, scope: !109)
!156 = !DILocation(line: 44, column: 3, scope: !157)
!157 = distinct !DILexicalBlock(scope: !154, file: !3, line: 43, column: 1)
!158 = !DILocation(line: 44, column: 4, scope: !157)
!159 = !DILocation(line: 44, column: 2, scope: !157)
!160 = !DILocation(line: 46, column: 6, scope: !157)
!161 = !DILocation(line: 46, column: 7, scope: !157)
!162 = !DILocation(line: 46, column: 5, scope: !157)
!163 = !DILocation(line: 47, column: 8, scope: !157)
!164 = !DILocation(line: 47, column: 7, scope: !157)
!165 = !DILocation(line: 48, column: 8, scope: !157)
!166 = !DILocation(line: 48, column: 7, scope: !157)
!167 = !DILocation(line: 49, column: 7, scope: !157)
!168 = !DILocation(line: 50, column: 8, scope: !157)
!169 = !DILocation(line: 50, column: 7, scope: !157)
!170 = !DILocation(line: 51, column: 9, scope: !157)
!171 = !DILocation(line: 51, column: 1, scope: !157)
!172 = !DILocation(line: 51, column: 8, scope: !157)
!173 = !DILocation(line: 52, column: 9, scope: !157)
!174 = !DILocation(line: 52, column: 1, scope: !157)
!175 = !DILocation(line: 52, column: 8, scope: !157)
!176 = !DILocation(line: 54, column: 3, scope: !157)
!177 = !DILocation(line: 54, column: 4, scope: !157)
!178 = !DILocation(line: 54, column: 2, scope: !157)
!179 = !DILocalVariable(name: "c", scope: !157, file: !3, line: 55, type: !39)
!180 = !DILocation(line: 55, column: 5, scope: !157)
!181 = !DILocation(line: 55, column: 7, scope: !157)
!182 = !DILocation(line: 56, column: 3, scope: !157)
!183 = !DILocation(line: 56, column: 8, scope: !157)
!184 = !DILocation(line: 56, column: 7, scope: !157)
!185 = !DILocation(line: 56, column: 2, scope: !157)
!186 = !DILocation(line: 57, column: 1, scope: !157)
!187 = !DILocation(line: 58, column: 1, scope: !109)
!188 = !DILocation(line: 59, column: 3, scope: !109)
!189 = !DILocation(line: 59, column: 2, scope: !109)
!190 = !DILocation(line: 60, column: 8, scope: !109)
!191 = !DILocation(line: 60, column: 10, scope: !109)
!192 = !DILocation(line: 60, column: 3, scope: !109)
!193 = !DILocation(line: 60, column: 2, scope: !109)
!194 = !DILocation(line: 61, column: 3, scope: !109)
!195 = !DILocation(line: 61, column: 2, scope: !109)
!196 = !DILocation(line: 62, column: 8, scope: !109)
!197 = !DILocation(line: 62, column: 3, scope: !109)
!198 = !DILocation(line: 62, column: 2, scope: !109)
!199 = !DILocation(line: 63, column: 3, scope: !109)
!200 = !DILocation(line: 63, column: 2, scope: !109)
!201 = !DILocation(line: 64, column: 1, scope: !109)
