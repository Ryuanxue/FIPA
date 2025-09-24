; ModuleID = 'mavlink_client.c'
source_filename = "mavlink_client.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.__mavlink_status = type { i8, i8, i8, i32, i8, i8, i8, i16, i16, i8, i8, %struct.__mavlink_signing*, %struct.__mavlink_signing_streams* }
%struct.__mavlink_signing = type { i8, i8, i64, [32 x i8], i1 (%struct.__mavlink_status*, i32)* }
%struct.__mavlink_signing_streams = type { i16, [16 x %struct.__mavlink_signing_stream] }
%struct.__mavlink_signing_stream = type { i8, i8, i8, [6 x i8] }
%struct.sockaddr_in = type { i16, i16, %struct.in_addr, [8 x i8] }
%struct.in_addr = type { i32 }
%struct.__mavlink_message = type <{ i16, i8, i8, i8, i8, i8, i8, i8, [3 x i8], [33 x i64], [2 x i8], [13 x i8] }>
%struct.sockaddr = type { i16, [14 x i8] }
%struct.__mavlink_heartbeat_t = type { i32, i8, i8, i8, i8, i8 }
%struct.mavlink_sha256_ctx = type { [2 x i32], [8 x i32], %union.anon }
%union.anon = type { [16 x i32] }
%union.anon.0 = type { i64 }

@.str = private unnamed_addr constant [24 x i8] c"Error sending heartbeat\00", align 1
@.str.1 = private unnamed_addr constant [37 x i8] c"Sent heartbeat: sysid=%d, compid=%d\0A\00", align 1
@.str.2 = private unnamed_addr constant [22 x i8] c"Error creating socket\00", align 1
@.str.3 = private unnamed_addr constant [10 x i8] c"127.0.0.1\00", align 1
@mavlink_get_channel_status.m_mavlink_status = internal global [16 x %struct.__mavlink_status] zeroinitializer, align 16, !dbg !0
@mavlink_sha256_constant_256 = internal constant [64 x i32] [i32 1116352408, i32 1899447441, i32 -1245643825, i32 -373957723, i32 961987163, i32 1508970993, i32 -1841331548, i32 -1424204075, i32 -670586216, i32 310598401, i32 607225278, i32 1426881987, i32 1925078388, i32 -2132889090, i32 -1680079193, i32 -1046744716, i32 -459576895, i32 -272742522, i32 264347078, i32 604807628, i32 770255983, i32 1249150122, i32 1555081692, i32 1996064986, i32 -1740746414, i32 -1473132947, i32 -1341970488, i32 -1084653625, i32 -958395405, i32 -710438585, i32 113926993, i32 338241895, i32 666307205, i32 773529912, i32 1294757372, i32 1396182291, i32 1695183700, i32 1986661051, i32 -2117940946, i32 -1838011259, i32 -1564481375, i32 -1474664885, i32 -1035236496, i32 -949202525, i32 -778901479, i32 -694614492, i32 -200395387, i32 275423344, i32 430227734, i32 506948616, i32 659060556, i32 883997877, i32 958139571, i32 1322822218, i32 1537002063, i32 1747873779, i32 1955562222, i32 2024104815, i32 -2067236844, i32 -1933114872, i32 -1866530822, i32 -1538233109, i32 -1090935817, i32 -965641998], align 16, !dbg !263

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @send_heartbeat(i32 %sock, %struct.sockaddr_in* %dest_addr) #0 !dbg !276 {
entry:
  %sock.addr = alloca i32, align 4
  %dest_addr.addr = alloca %struct.sockaddr_in*, align 8
  %msg = alloca %struct.__mavlink_message, align 1
  %buf = alloca [2041 x i8], align 16
  %len = alloca i16, align 2
  %sysid = alloca i8, align 1
  %compid = alloca i8, align 1
  %type = alloca i8, align 1
  %autopilot = alloca i8, align 1
  %base_mode = alloca i8, align 1
  %custom_mode = alloca i32, align 4
  %system_status = alloca i8, align 1
  store i32 %sock, i32* %sock.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %sock.addr, metadata !295, metadata !DIExpression()), !dbg !296
  store %struct.sockaddr_in* %dest_addr, %struct.sockaddr_in** %dest_addr.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.sockaddr_in** %dest_addr.addr, metadata !297, metadata !DIExpression()), !dbg !298
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message* %msg, metadata !299, metadata !DIExpression()), !dbg !324
  call void @llvm.dbg.declare(metadata [2041 x i8]* %buf, metadata !325, metadata !DIExpression()), !dbg !329
  call void @llvm.dbg.declare(metadata i16* %len, metadata !330, metadata !DIExpression()), !dbg !331
  call void @llvm.dbg.declare(metadata i8* %sysid, metadata !332, metadata !DIExpression()), !dbg !333
  store i8 1, i8* %sysid, align 1, !dbg !333
  call void @llvm.dbg.declare(metadata i8* %compid, metadata !334, metadata !DIExpression()), !dbg !335
  store i8 -56, i8* %compid, align 1, !dbg !335
  call void @llvm.dbg.declare(metadata i8* %type, metadata !336, metadata !DIExpression()), !dbg !337
  store i8 2, i8* %type, align 1, !dbg !337
  call void @llvm.dbg.declare(metadata i8* %autopilot, metadata !338, metadata !DIExpression()), !dbg !339
  store i8 12, i8* %autopilot, align 1, !dbg !339
  call void @llvm.dbg.declare(metadata i8* %base_mode, metadata !340, metadata !DIExpression()), !dbg !341
  store i8 -64, i8* %base_mode, align 1, !dbg !341
  call void @llvm.dbg.declare(metadata i32* %custom_mode, metadata !342, metadata !DIExpression()), !dbg !343
  store i32 0, i32* %custom_mode, align 4, !dbg !343
  call void @llvm.dbg.declare(metadata i8* %system_status, metadata !344, metadata !DIExpression()), !dbg !345
  store i8 4, i8* %system_status, align 1, !dbg !345
  %0 = load i8, i8* %sysid, align 1, !dbg !346
  %1 = load i8, i8* %compid, align 1, !dbg !347
  %2 = load i8, i8* %type, align 1, !dbg !348
  %3 = load i8, i8* %autopilot, align 1, !dbg !349
  %4 = load i8, i8* %base_mode, align 1, !dbg !350
  %5 = load i32, i32* %custom_mode, align 4, !dbg !351
  %6 = load i8, i8* %system_status, align 1, !dbg !352
  %call = call zeroext i16 @mavlink_msg_heartbeat_pack(i8 zeroext %0, i8 zeroext %1, %struct.__mavlink_message* %msg, i8 zeroext %2, i8 zeroext %3, i8 zeroext %4, i32 %5, i8 zeroext %6), !dbg !353
  %arraydecay = getelementptr inbounds [2041 x i8], [2041 x i8]* %buf, i64 0, i64 0, !dbg !354
  %call1 = call zeroext i16 @mavlink_msg_to_send_buffer(i8* %arraydecay, %struct.__mavlink_message* %msg), !dbg !355
  store i16 %call1, i16* %len, align 2, !dbg !356
  %7 = load i32, i32* %sock.addr, align 4, !dbg !357
  %arraydecay2 = getelementptr inbounds [2041 x i8], [2041 x i8]* %buf, i64 0, i64 0, !dbg !359
  %8 = load i16, i16* %len, align 2, !dbg !360
  %conv = zext i16 %8 to i64, !dbg !360
  %9 = load %struct.sockaddr_in*, %struct.sockaddr_in** %dest_addr.addr, align 8, !dbg !361
  %10 = bitcast %struct.sockaddr_in* %9 to %struct.sockaddr*, !dbg !362
  %call3 = call i64 @sendto(i32 %7, i8* %arraydecay2, i64 %conv, i32 0, %struct.sockaddr* %10, i32 16), !dbg !363
  %cmp = icmp slt i64 %call3, 0, !dbg !364
  br i1 %cmp, label %if.then, label %if.else, !dbg !365

if.then:                                          ; preds = %entry
  call void @perror(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str, i64 0, i64 0)), !dbg !366
  br label %if.end, !dbg !368

if.else:                                          ; preds = %entry
  %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i64 0, i64 0), i32 1, i32 200), !dbg !369
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void, !dbg !371
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i16 @mavlink_msg_heartbeat_pack(i8 zeroext %system_id, i8 zeroext %component_id, %struct.__mavlink_message* %msg, i8 zeroext %type, i8 zeroext %autopilot, i8 zeroext %base_mode, i32 %custom_mode, i8 zeroext %system_status) #0 !dbg !372 {
entry:
  %system_id.addr = alloca i8, align 1
  %component_id.addr = alloca i8, align 1
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %type.addr = alloca i8, align 1
  %autopilot.addr = alloca i8, align 1
  %base_mode.addr = alloca i8, align 1
  %custom_mode.addr = alloca i32, align 4
  %system_status.addr = alloca i8, align 1
  %packet = alloca %struct.__mavlink_heartbeat_t, align 4
  store i8 %system_id, i8* %system_id.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %system_id.addr, metadata !377, metadata !DIExpression()), !dbg !378
  store i8 %component_id, i8* %component_id.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %component_id.addr, metadata !379, metadata !DIExpression()), !dbg !380
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !381, metadata !DIExpression()), !dbg !382
  store i8 %type, i8* %type.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %type.addr, metadata !383, metadata !DIExpression()), !dbg !384
  store i8 %autopilot, i8* %autopilot.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %autopilot.addr, metadata !385, metadata !DIExpression()), !dbg !386
  store i8 %base_mode, i8* %base_mode.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %base_mode.addr, metadata !387, metadata !DIExpression()), !dbg !388
  store i32 %custom_mode, i32* %custom_mode.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %custom_mode.addr, metadata !389, metadata !DIExpression()), !dbg !390
  store i8 %system_status, i8* %system_status.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %system_status.addr, metadata !391, metadata !DIExpression()), !dbg !392
  call void @llvm.dbg.declare(metadata %struct.__mavlink_heartbeat_t* %packet, metadata !393, metadata !DIExpression()), !dbg !403
  %0 = load i32, i32* %custom_mode.addr, align 4, !dbg !404
  %custom_mode1 = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %packet, i32 0, i32 0, !dbg !405
  store i32 %0, i32* %custom_mode1, align 4, !dbg !406
  %1 = load i8, i8* %type.addr, align 1, !dbg !407
  %type2 = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %packet, i32 0, i32 1, !dbg !408
  store i8 %1, i8* %type2, align 4, !dbg !409
  %2 = load i8, i8* %autopilot.addr, align 1, !dbg !410
  %autopilot3 = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %packet, i32 0, i32 2, !dbg !411
  store i8 %2, i8* %autopilot3, align 1, !dbg !412
  %3 = load i8, i8* %base_mode.addr, align 1, !dbg !413
  %base_mode4 = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %packet, i32 0, i32 3, !dbg !414
  store i8 %3, i8* %base_mode4, align 2, !dbg !415
  %4 = load i8, i8* %system_status.addr, align 1, !dbg !416
  %system_status5 = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %packet, i32 0, i32 4, !dbg !417
  store i8 %4, i8* %system_status5, align 1, !dbg !418
  %mavlink_version = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %packet, i32 0, i32 5, !dbg !419
  store i8 3, i8* %mavlink_version, align 4, !dbg !420
  %5 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !421
  %payload64 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %5, i32 0, i32 9, !dbg !421
  %arrayidx = getelementptr inbounds [33 x i64], [33 x i64]* %payload64, i64 0, i64 0, !dbg !421
  %6 = bitcast i64* %arrayidx to i8*, !dbg !422
  %7 = bitcast %struct.__mavlink_heartbeat_t* %packet to i8*, !dbg !422
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %6, i8* align 4 %7, i64 9, i1 false), !dbg !422
  %8 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !423
  %msgid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %8, i32 0, i32 8, !dbg !424
  %9 = bitcast [3 x i8]* %msgid to i24*, !dbg !424
  store i24 0, i24* %9, align 1, !dbg !425
  %10 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !426
  %11 = load i8, i8* %system_id.addr, align 1, !dbg !427
  %12 = load i8, i8* %component_id.addr, align 1, !dbg !428
  %call = call zeroext i16 @mavlink_finalize_message(%struct.__mavlink_message* %10, i8 zeroext %11, i8 zeroext %12, i8 zeroext 9, i8 zeroext 9, i8 zeroext 50), !dbg !429
  ret i16 %call, !dbg !430
}

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i16 @mavlink_msg_to_send_buffer(i8* %buf, %struct.__mavlink_message* %msg) #0 !dbg !431 {
entry:
  %buf.addr = alloca i8*, align 8
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %signature_len = alloca i8, align 1
  %header_len = alloca i8, align 1
  %ck = alloca i8*, align 8
  %length = alloca i8, align 1
  store i8* %buf, i8** %buf.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %buf.addr, metadata !436, metadata !DIExpression()), !dbg !437
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !438, metadata !DIExpression()), !dbg !439
  call void @llvm.dbg.declare(metadata i8* %signature_len, metadata !440, metadata !DIExpression()), !dbg !441
  call void @llvm.dbg.declare(metadata i8* %header_len, metadata !442, metadata !DIExpression()), !dbg !443
  call void @llvm.dbg.declare(metadata i8** %ck, metadata !444, metadata !DIExpression()), !dbg !445
  call void @llvm.dbg.declare(metadata i8* %length, metadata !446, metadata !DIExpression()), !dbg !447
  %0 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !448
  %len = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %0, i32 0, i32 2, !dbg !449
  %1 = load i8, i8* %len, align 1, !dbg !449
  store i8 %1, i8* %length, align 1, !dbg !447
  %2 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !450
  %magic = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %2, i32 0, i32 1, !dbg !452
  %3 = load i8, i8* %magic, align 1, !dbg !452
  %conv = zext i8 %3 to i32, !dbg !450
  %cmp = icmp eq i32 %conv, 254, !dbg !453
  br i1 %cmp, label %if.then, label %if.else, !dbg !454

if.then:                                          ; preds = %entry
  store i8 0, i8* %signature_len, align 1, !dbg !455
  store i8 5, i8* %header_len, align 1, !dbg !457
  %4 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !458
  %magic2 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %4, i32 0, i32 1, !dbg !459
  %5 = load i8, i8* %magic2, align 1, !dbg !459
  %6 = load i8*, i8** %buf.addr, align 8, !dbg !460
  %arrayidx = getelementptr inbounds i8, i8* %6, i64 0, !dbg !460
  store i8 %5, i8* %arrayidx, align 1, !dbg !461
  %7 = load i8, i8* %length, align 1, !dbg !462
  %8 = load i8*, i8** %buf.addr, align 8, !dbg !463
  %arrayidx3 = getelementptr inbounds i8, i8* %8, i64 1, !dbg !463
  store i8 %7, i8* %arrayidx3, align 1, !dbg !464
  %9 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !465
  %seq = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %9, i32 0, i32 5, !dbg !466
  %10 = load i8, i8* %seq, align 1, !dbg !466
  %11 = load i8*, i8** %buf.addr, align 8, !dbg !467
  %arrayidx4 = getelementptr inbounds i8, i8* %11, i64 2, !dbg !467
  store i8 %10, i8* %arrayidx4, align 1, !dbg !468
  %12 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !469
  %sysid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %12, i32 0, i32 6, !dbg !470
  %13 = load i8, i8* %sysid, align 1, !dbg !470
  %14 = load i8*, i8** %buf.addr, align 8, !dbg !471
  %arrayidx5 = getelementptr inbounds i8, i8* %14, i64 3, !dbg !471
  store i8 %13, i8* %arrayidx5, align 1, !dbg !472
  %15 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !473
  %compid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %15, i32 0, i32 7, !dbg !474
  %16 = load i8, i8* %compid, align 1, !dbg !474
  %17 = load i8*, i8** %buf.addr, align 8, !dbg !475
  %arrayidx6 = getelementptr inbounds i8, i8* %17, i64 4, !dbg !475
  store i8 %16, i8* %arrayidx6, align 1, !dbg !476
  %18 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !477
  %msgid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %18, i32 0, i32 8, !dbg !478
  %19 = bitcast [3 x i8]* %msgid to i24*, !dbg !478
  %bf.load = load i24, i24* %19, align 1, !dbg !478
  %bf.cast = zext i24 %bf.load to i32, !dbg !478
  %and = and i32 %bf.cast, 255, !dbg !479
  %conv7 = trunc i32 %and to i8, !dbg !477
  %20 = load i8*, i8** %buf.addr, align 8, !dbg !480
  %arrayidx8 = getelementptr inbounds i8, i8* %20, i64 5, !dbg !480
  store i8 %conv7, i8* %arrayidx8, align 1, !dbg !481
  %21 = load i8*, i8** %buf.addr, align 8, !dbg !482
  %arrayidx9 = getelementptr inbounds i8, i8* %21, i64 6, !dbg !482
  %22 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !483
  %payload64 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %22, i32 0, i32 9, !dbg !483
  %arrayidx10 = getelementptr inbounds [33 x i64], [33 x i64]* %payload64, i64 0, i64 0, !dbg !483
  %23 = bitcast i64* %arrayidx10 to i8*, !dbg !484
  %24 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !485
  %len11 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %24, i32 0, i32 2, !dbg !486
  %25 = load i8, i8* %len11, align 1, !dbg !486
  %conv12 = zext i8 %25 to i64, !dbg !485
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %arrayidx9, i8* align 1 %23, i64 %conv12, i1 false), !dbg !484
  %26 = load i8*, i8** %buf.addr, align 8, !dbg !487
  %27 = load i8, i8* %header_len, align 1, !dbg !488
  %conv13 = zext i8 %27 to i32, !dbg !488
  %idx.ext = sext i32 %conv13 to i64, !dbg !489
  %add.ptr = getelementptr inbounds i8, i8* %26, i64 %idx.ext, !dbg !489
  %add.ptr14 = getelementptr inbounds i8, i8* %add.ptr, i64 1, !dbg !490
  %28 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !491
  %len15 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %28, i32 0, i32 2, !dbg !492
  %29 = load i8, i8* %len15, align 1, !dbg !492
  %conv16 = zext i8 %29 to i16, !dbg !493
  %conv17 = zext i16 %conv16 to i32, !dbg !493
  %idx.ext18 = sext i32 %conv17 to i64, !dbg !494
  %add.ptr19 = getelementptr inbounds i8, i8* %add.ptr14, i64 %idx.ext18, !dbg !494
  store i8* %add.ptr19, i8** %ck, align 8, !dbg !495
  br label %if.end, !dbg !496

if.else:                                          ; preds = %entry
  %30 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !497
  %payload6420 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %30, i32 0, i32 9, !dbg !497
  %arrayidx21 = getelementptr inbounds [33 x i64], [33 x i64]* %payload6420, i64 0, i64 0, !dbg !497
  %31 = bitcast i64* %arrayidx21 to i8*, !dbg !497
  %32 = load i8, i8* %length, align 1, !dbg !499
  %call = call zeroext i8 @_mav_trim_payload(i8* %31, i8 zeroext %32), !dbg !500
  store i8 %call, i8* %length, align 1, !dbg !501
  store i8 9, i8* %header_len, align 1, !dbg !502
  %33 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !503
  %magic22 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %33, i32 0, i32 1, !dbg !504
  %34 = load i8, i8* %magic22, align 1, !dbg !504
  %35 = load i8*, i8** %buf.addr, align 8, !dbg !505
  %arrayidx23 = getelementptr inbounds i8, i8* %35, i64 0, !dbg !505
  store i8 %34, i8* %arrayidx23, align 1, !dbg !506
  %36 = load i8, i8* %length, align 1, !dbg !507
  %37 = load i8*, i8** %buf.addr, align 8, !dbg !508
  %arrayidx24 = getelementptr inbounds i8, i8* %37, i64 1, !dbg !508
  store i8 %36, i8* %arrayidx24, align 1, !dbg !509
  %38 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !510
  %incompat_flags = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %38, i32 0, i32 3, !dbg !511
  %39 = load i8, i8* %incompat_flags, align 1, !dbg !511
  %40 = load i8*, i8** %buf.addr, align 8, !dbg !512
  %arrayidx25 = getelementptr inbounds i8, i8* %40, i64 2, !dbg !512
  store i8 %39, i8* %arrayidx25, align 1, !dbg !513
  %41 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !514
  %compat_flags = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %41, i32 0, i32 4, !dbg !515
  %42 = load i8, i8* %compat_flags, align 1, !dbg !515
  %43 = load i8*, i8** %buf.addr, align 8, !dbg !516
  %arrayidx26 = getelementptr inbounds i8, i8* %43, i64 3, !dbg !516
  store i8 %42, i8* %arrayidx26, align 1, !dbg !517
  %44 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !518
  %seq27 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %44, i32 0, i32 5, !dbg !519
  %45 = load i8, i8* %seq27, align 1, !dbg !519
  %46 = load i8*, i8** %buf.addr, align 8, !dbg !520
  %arrayidx28 = getelementptr inbounds i8, i8* %46, i64 4, !dbg !520
  store i8 %45, i8* %arrayidx28, align 1, !dbg !521
  %47 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !522
  %sysid29 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %47, i32 0, i32 6, !dbg !523
  %48 = load i8, i8* %sysid29, align 1, !dbg !523
  %49 = load i8*, i8** %buf.addr, align 8, !dbg !524
  %arrayidx30 = getelementptr inbounds i8, i8* %49, i64 5, !dbg !524
  store i8 %48, i8* %arrayidx30, align 1, !dbg !525
  %50 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !526
  %compid31 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %50, i32 0, i32 7, !dbg !527
  %51 = load i8, i8* %compid31, align 1, !dbg !527
  %52 = load i8*, i8** %buf.addr, align 8, !dbg !528
  %arrayidx32 = getelementptr inbounds i8, i8* %52, i64 6, !dbg !528
  store i8 %51, i8* %arrayidx32, align 1, !dbg !529
  %53 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !530
  %msgid33 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %53, i32 0, i32 8, !dbg !531
  %54 = bitcast [3 x i8]* %msgid33 to i24*, !dbg !531
  %bf.load34 = load i24, i24* %54, align 1, !dbg !531
  %bf.cast35 = zext i24 %bf.load34 to i32, !dbg !531
  %and36 = and i32 %bf.cast35, 255, !dbg !532
  %conv37 = trunc i32 %and36 to i8, !dbg !530
  %55 = load i8*, i8** %buf.addr, align 8, !dbg !533
  %arrayidx38 = getelementptr inbounds i8, i8* %55, i64 7, !dbg !533
  store i8 %conv37, i8* %arrayidx38, align 1, !dbg !534
  %56 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !535
  %msgid39 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %56, i32 0, i32 8, !dbg !536
  %57 = bitcast [3 x i8]* %msgid39 to i24*, !dbg !536
  %bf.load40 = load i24, i24* %57, align 1, !dbg !536
  %bf.cast41 = zext i24 %bf.load40 to i32, !dbg !536
  %shr = ashr i32 %bf.cast41, 8, !dbg !537
  %and42 = and i32 %shr, 255, !dbg !538
  %conv43 = trunc i32 %and42 to i8, !dbg !539
  %58 = load i8*, i8** %buf.addr, align 8, !dbg !540
  %arrayidx44 = getelementptr inbounds i8, i8* %58, i64 8, !dbg !540
  store i8 %conv43, i8* %arrayidx44, align 1, !dbg !541
  %59 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !542
  %msgid45 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %59, i32 0, i32 8, !dbg !543
  %60 = bitcast [3 x i8]* %msgid45 to i24*, !dbg !543
  %bf.load46 = load i24, i24* %60, align 1, !dbg !543
  %bf.cast47 = zext i24 %bf.load46 to i32, !dbg !543
  %shr48 = ashr i32 %bf.cast47, 16, !dbg !544
  %and49 = and i32 %shr48, 255, !dbg !545
  %conv50 = trunc i32 %and49 to i8, !dbg !546
  %61 = load i8*, i8** %buf.addr, align 8, !dbg !547
  %arrayidx51 = getelementptr inbounds i8, i8* %61, i64 9, !dbg !547
  store i8 %conv50, i8* %arrayidx51, align 1, !dbg !548
  %62 = load i8*, i8** %buf.addr, align 8, !dbg !549
  %arrayidx52 = getelementptr inbounds i8, i8* %62, i64 10, !dbg !549
  %63 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !550
  %payload6453 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %63, i32 0, i32 9, !dbg !550
  %arrayidx54 = getelementptr inbounds [33 x i64], [33 x i64]* %payload6453, i64 0, i64 0, !dbg !550
  %64 = bitcast i64* %arrayidx54 to i8*, !dbg !551
  %65 = load i8, i8* %length, align 1, !dbg !552
  %conv55 = zext i8 %65 to i64, !dbg !552
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %arrayidx52, i8* align 1 %64, i64 %conv55, i1 false), !dbg !551
  %66 = load i8*, i8** %buf.addr, align 8, !dbg !553
  %67 = load i8, i8* %header_len, align 1, !dbg !554
  %conv56 = zext i8 %67 to i32, !dbg !554
  %idx.ext57 = sext i32 %conv56 to i64, !dbg !555
  %add.ptr58 = getelementptr inbounds i8, i8* %66, i64 %idx.ext57, !dbg !555
  %add.ptr59 = getelementptr inbounds i8, i8* %add.ptr58, i64 1, !dbg !556
  %68 = load i8, i8* %length, align 1, !dbg !557
  %conv60 = zext i8 %68 to i16, !dbg !558
  %conv61 = zext i16 %conv60 to i32, !dbg !558
  %idx.ext62 = sext i32 %conv61 to i64, !dbg !559
  %add.ptr63 = getelementptr inbounds i8, i8* %add.ptr59, i64 %idx.ext62, !dbg !559
  store i8* %add.ptr63, i8** %ck, align 8, !dbg !560
  %69 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !561
  %incompat_flags64 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %69, i32 0, i32 3, !dbg !562
  %70 = load i8, i8* %incompat_flags64, align 1, !dbg !562
  %conv65 = zext i8 %70 to i32, !dbg !561
  %and66 = and i32 %conv65, 1, !dbg !563
  %tobool = icmp ne i32 %and66, 0, !dbg !564
  %71 = zext i1 %tobool to i64, !dbg !564
  %cond = select i1 %tobool, i32 13, i32 0, !dbg !564
  %conv67 = trunc i32 %cond to i8, !dbg !564
  store i8 %conv67, i8* %signature_len, align 1, !dbg !565
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %72 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !566
  %checksum = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %72, i32 0, i32 0, !dbg !567
  %73 = load i16, i16* %checksum, align 1, !dbg !567
  %conv68 = zext i16 %73 to i32, !dbg !566
  %and69 = and i32 %conv68, 255, !dbg !568
  %conv70 = trunc i32 %and69 to i8, !dbg !569
  %74 = load i8*, i8** %ck, align 8, !dbg !570
  %arrayidx71 = getelementptr inbounds i8, i8* %74, i64 0, !dbg !570
  store i8 %conv70, i8* %arrayidx71, align 1, !dbg !571
  %75 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !572
  %checksum72 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %75, i32 0, i32 0, !dbg !573
  %76 = load i16, i16* %checksum72, align 1, !dbg !573
  %conv73 = zext i16 %76 to i32, !dbg !572
  %shr74 = ashr i32 %conv73, 8, !dbg !574
  %conv75 = trunc i32 %shr74 to i8, !dbg !575
  %77 = load i8*, i8** %ck, align 8, !dbg !576
  %arrayidx76 = getelementptr inbounds i8, i8* %77, i64 1, !dbg !576
  store i8 %conv75, i8* %arrayidx76, align 1, !dbg !577
  %78 = load i8, i8* %signature_len, align 1, !dbg !578
  %conv77 = zext i8 %78 to i32, !dbg !578
  %cmp78 = icmp sgt i32 %conv77, 0, !dbg !580
  br i1 %cmp78, label %if.then80, label %if.end83, !dbg !581

if.then80:                                        ; preds = %if.end
  %79 = load i8*, i8** %ck, align 8, !dbg !582
  %arrayidx81 = getelementptr inbounds i8, i8* %79, i64 2, !dbg !582
  %80 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !584
  %signature = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %80, i32 0, i32 11, !dbg !585
  %arraydecay = getelementptr inbounds [13 x i8], [13 x i8]* %signature, i64 0, i64 0, !dbg !586
  %81 = load i8, i8* %signature_len, align 1, !dbg !587
  %conv82 = zext i8 %81 to i64, !dbg !587
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %arrayidx81, i8* align 1 %arraydecay, i64 %conv82, i1 false), !dbg !586
  br label %if.end83, !dbg !588

if.end83:                                         ; preds = %if.then80, %if.end
  %82 = load i8, i8* %header_len, align 1, !dbg !589
  %conv84 = zext i8 %82 to i32, !dbg !589
  %add = add nsw i32 %conv84, 1, !dbg !590
  %add85 = add nsw i32 %add, 2, !dbg !591
  %83 = load i8, i8* %length, align 1, !dbg !592
  %conv86 = zext i8 %83 to i16, !dbg !593
  %conv87 = zext i16 %conv86 to i32, !dbg !593
  %add88 = add nsw i32 %add85, %conv87, !dbg !594
  %84 = load i8, i8* %signature_len, align 1, !dbg !595
  %conv89 = zext i8 %84 to i16, !dbg !596
  %conv90 = zext i16 %conv89 to i32, !dbg !596
  %add91 = add nsw i32 %add88, %conv90, !dbg !597
  %conv92 = trunc i32 %add91 to i16, !dbg !589
  ret i16 %conv92, !dbg !598
}

declare dso_local i64 @sendto(i32, i8*, i64, i32, %struct.sockaddr*, i32) #2

declare dso_local void @perror(i8*) #2

declare dso_local i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !599 {
entry:
  %retval = alloca i32, align 4
  %sock = alloca i32, align 4
  %flags = alloca i32, align 4
  %dest_addr = alloca %struct.sockaddr_in, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %sock, metadata !602, metadata !DIExpression()), !dbg !603
  %call = call i32 @socket(i32 2, i32 2, i32 17) #6, !dbg !604
  store i32 %call, i32* %sock, align 4, !dbg !603
  %0 = load i32, i32* %sock, align 4, !dbg !605
  %cmp = icmp slt i32 %0, 0, !dbg !607
  br i1 %cmp, label %if.then, label %if.end, !dbg !608

if.then:                                          ; preds = %entry
  call void @perror(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i64 0, i64 0)), !dbg !609
  ret i32 1, !dbg !611

if.end:                                           ; preds = %entry
  call void @llvm.dbg.declare(metadata i32* %flags, metadata !612, metadata !DIExpression()), !dbg !613
  %1 = load i32, i32* %sock, align 4, !dbg !614
  %call1 = call i32 (i32, i32, ...) @fcntl(i32 %1, i32 3, i32 0), !dbg !615
  store i32 %call1, i32* %flags, align 4, !dbg !613
  %2 = load i32, i32* %sock, align 4, !dbg !616
  %3 = load i32, i32* %flags, align 4, !dbg !617
  %or = or i32 %3, 2048, !dbg !618
  %call2 = call i32 (i32, i32, ...) @fcntl(i32 %2, i32 4, i32 %or), !dbg !619
  call void @llvm.dbg.declare(metadata %struct.sockaddr_in* %dest_addr, metadata !620, metadata !DIExpression()), !dbg !621
  %4 = bitcast %struct.sockaddr_in* %dest_addr to i8*, !dbg !622
  call void @llvm.memset.p0i8.i64(i8* align 4 %4, i8 0, i64 16, i1 false), !dbg !622
  %sin_family = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %dest_addr, i32 0, i32 0, !dbg !623
  store i16 2, i16* %sin_family, align 4, !dbg !624
  %call3 = call zeroext i16 @htons(i16 zeroext 14550) #7, !dbg !625
  %sin_port = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %dest_addr, i32 0, i32 1, !dbg !626
  store i16 %call3, i16* %sin_port, align 2, !dbg !627
  %sin_addr = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %dest_addr, i32 0, i32 2, !dbg !628
  %5 = bitcast %struct.in_addr* %sin_addr to i8*, !dbg !629
  %call4 = call i32 @inet_pton(i32 2, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.3, i64 0, i64 0), i8* %5) #6, !dbg !630
  br label %while.body, !dbg !631

while.body:                                       ; preds = %if.end, %while.body
  %6 = load i32, i32* %sock, align 4, !dbg !632
  call void @send_heartbeat(i32 %6, %struct.sockaddr_in* %dest_addr), !dbg !634
  %call5 = call i32 @sleep(i32 1), !dbg !635
  br label %while.body, !dbg !631, !llvm.loop !636
}

; Function Attrs: nounwind
declare dso_local i32 @socket(i32, i32, i32) #3

declare dso_local i32 @fcntl(i32, i32, ...) #2

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #4

; Function Attrs: nounwind readnone
declare dso_local zeroext i16 @htons(i16 zeroext) #5

; Function Attrs: nounwind
declare dso_local i32 @inet_pton(i32, i8*, i8*) #3

declare dso_local i32 @sleep(i32) #2

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #4

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i16 @mavlink_finalize_message(%struct.__mavlink_message* %msg, i8 zeroext %system_id, i8 zeroext %component_id, i8 zeroext %min_length, i8 zeroext %length, i8 zeroext %crc_extra) #0 !dbg !638 {
entry:
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %system_id.addr = alloca i8, align 1
  %component_id.addr = alloca i8, align 1
  %min_length.addr = alloca i8, align 1
  %length.addr = alloca i8, align 1
  %crc_extra.addr = alloca i8, align 1
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !641, metadata !DIExpression()), !dbg !642
  store i8 %system_id, i8* %system_id.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %system_id.addr, metadata !643, metadata !DIExpression()), !dbg !644
  store i8 %component_id, i8* %component_id.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %component_id.addr, metadata !645, metadata !DIExpression()), !dbg !646
  store i8 %min_length, i8* %min_length.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %min_length.addr, metadata !647, metadata !DIExpression()), !dbg !648
  store i8 %length, i8* %length.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %length.addr, metadata !649, metadata !DIExpression()), !dbg !650
  store i8 %crc_extra, i8* %crc_extra.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %crc_extra.addr, metadata !651, metadata !DIExpression()), !dbg !652
  %0 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !653
  %1 = load i8, i8* %system_id.addr, align 1, !dbg !654
  %2 = load i8, i8* %component_id.addr, align 1, !dbg !655
  %3 = load i8, i8* %min_length.addr, align 1, !dbg !656
  %4 = load i8, i8* %length.addr, align 1, !dbg !657
  %5 = load i8, i8* %crc_extra.addr, align 1, !dbg !658
  %call = call zeroext i16 @mavlink_finalize_message_chan(%struct.__mavlink_message* %0, i8 zeroext %1, i8 zeroext %2, i8 zeroext 0, i8 zeroext %3, i8 zeroext %4, i8 zeroext %5), !dbg !659
  ret i16 %call, !dbg !660
}

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i16 @mavlink_finalize_message_chan(%struct.__mavlink_message* %msg, i8 zeroext %system_id, i8 zeroext %component_id, i8 zeroext %chan, i8 zeroext %min_length, i8 zeroext %length, i8 zeroext %crc_extra) #0 !dbg !661 {
entry:
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %system_id.addr = alloca i8, align 1
  %component_id.addr = alloca i8, align 1
  %chan.addr = alloca i8, align 1
  %min_length.addr = alloca i8, align 1
  %length.addr = alloca i8, align 1
  %crc_extra.addr = alloca i8, align 1
  %status = alloca %struct.__mavlink_status*, align 8
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !664, metadata !DIExpression()), !dbg !665
  store i8 %system_id, i8* %system_id.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %system_id.addr, metadata !666, metadata !DIExpression()), !dbg !667
  store i8 %component_id, i8* %component_id.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %component_id.addr, metadata !668, metadata !DIExpression()), !dbg !669
  store i8 %chan, i8* %chan.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %chan.addr, metadata !670, metadata !DIExpression()), !dbg !671
  store i8 %min_length, i8* %min_length.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %min_length.addr, metadata !672, metadata !DIExpression()), !dbg !673
  store i8 %length, i8* %length.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %length.addr, metadata !674, metadata !DIExpression()), !dbg !675
  store i8 %crc_extra, i8* %crc_extra.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %crc_extra.addr, metadata !676, metadata !DIExpression()), !dbg !677
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status** %status, metadata !678, metadata !DIExpression()), !dbg !679
  %0 = load i8, i8* %chan.addr, align 1, !dbg !680
  %call = call %struct.__mavlink_status* @mavlink_get_channel_status(i8 zeroext %0), !dbg !681
  store %struct.__mavlink_status* %call, %struct.__mavlink_status** %status, align 8, !dbg !679
  %1 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !682
  %2 = load i8, i8* %system_id.addr, align 1, !dbg !683
  %3 = load i8, i8* %component_id.addr, align 1, !dbg !684
  %4 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status, align 8, !dbg !685
  %5 = load i8, i8* %min_length.addr, align 1, !dbg !686
  %6 = load i8, i8* %length.addr, align 1, !dbg !687
  %7 = load i8, i8* %crc_extra.addr, align 1, !dbg !688
  %call1 = call zeroext i16 @mavlink_finalize_message_buffer(%struct.__mavlink_message* %1, i8 zeroext %2, i8 zeroext %3, %struct.__mavlink_status* %4, i8 zeroext %5, i8 zeroext %6, i8 zeroext %7), !dbg !689
  ret i16 %call1, !dbg !690
}

; Function Attrs: noinline nounwind optnone uwtable
define internal %struct.__mavlink_status* @mavlink_get_channel_status(i8 zeroext %chan) #0 !dbg !2 {
entry:
  %chan.addr = alloca i8, align 1
  store i8 %chan, i8* %chan.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %chan.addr, metadata !691, metadata !DIExpression()), !dbg !692
  %0 = load i8, i8* %chan.addr, align 1, !dbg !693
  %idxprom = zext i8 %0 to i64, !dbg !694
  %arrayidx = getelementptr inbounds [16 x %struct.__mavlink_status], [16 x %struct.__mavlink_status]* @mavlink_get_channel_status.m_mavlink_status, i64 0, i64 %idxprom, !dbg !694
  ret %struct.__mavlink_status* %arrayidx, !dbg !695
}

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i16 @mavlink_finalize_message_buffer(%struct.__mavlink_message* %msg, i8 zeroext %system_id, i8 zeroext %component_id, %struct.__mavlink_status* %status, i8 zeroext %min_length, i8 zeroext %length, i8 zeroext %crc_extra) #0 !dbg !696 {
entry:
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %system_id.addr = alloca i8, align 1
  %component_id.addr = alloca i8, align 1
  %status.addr = alloca %struct.__mavlink_status*, align 8
  %min_length.addr = alloca i8, align 1
  %length.addr = alloca i8, align 1
  %crc_extra.addr = alloca i8, align 1
  %mavlink1 = alloca i8, align 1
  %signing = alloca i8, align 1
  %signature_len = alloca i8, align 1
  %header_len = alloca i8, align 1
  %buf = alloca [10 x i8], align 1
  %checksum = alloca i16, align 2
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !699, metadata !DIExpression()), !dbg !700
  store i8 %system_id, i8* %system_id.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %system_id.addr, metadata !701, metadata !DIExpression()), !dbg !702
  store i8 %component_id, i8* %component_id.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %component_id.addr, metadata !703, metadata !DIExpression()), !dbg !704
  store %struct.__mavlink_status* %status, %struct.__mavlink_status** %status.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status** %status.addr, metadata !705, metadata !DIExpression()), !dbg !706
  store i8 %min_length, i8* %min_length.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %min_length.addr, metadata !707, metadata !DIExpression()), !dbg !708
  store i8 %length, i8* %length.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %length.addr, metadata !709, metadata !DIExpression()), !dbg !710
  store i8 %crc_extra, i8* %crc_extra.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %crc_extra.addr, metadata !711, metadata !DIExpression()), !dbg !712
  call void @llvm.dbg.declare(metadata i8* %mavlink1, metadata !713, metadata !DIExpression()), !dbg !714
  %0 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !715
  %flags = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %0, i32 0, i32 9, !dbg !716
  %1 = load i8, i8* %flags, align 8, !dbg !716
  %conv = zext i8 %1 to i32, !dbg !715
  %and = and i32 %conv, 2, !dbg !717
  %cmp = icmp ne i32 %and, 0, !dbg !718
  %frombool = zext i1 %cmp to i8, !dbg !714
  store i8 %frombool, i8* %mavlink1, align 1, !dbg !714
  call void @llvm.dbg.declare(metadata i8* %signing, metadata !719, metadata !DIExpression()), !dbg !720
  %2 = load i8, i8* %mavlink1, align 1, !dbg !721
  %tobool = trunc i8 %2 to i1, !dbg !721
  br i1 %tobool, label %land.end, label %land.lhs.true, !dbg !722

land.lhs.true:                                    ; preds = %entry
  %3 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !723
  %signing2 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %3, i32 0, i32 11, !dbg !724
  %4 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing2, align 8, !dbg !724
  %tobool3 = icmp ne %struct.__mavlink_signing* %4, null, !dbg !723
  br i1 %tobool3, label %land.rhs, label %land.end, !dbg !725

land.rhs:                                         ; preds = %land.lhs.true
  %5 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !726
  %signing4 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %5, i32 0, i32 11, !dbg !727
  %6 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing4, align 8, !dbg !727
  %flags5 = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %6, i32 0, i32 0, !dbg !728
  %7 = load i8, i8* %flags5, align 8, !dbg !728
  %conv6 = zext i8 %7 to i32, !dbg !726
  %and7 = and i32 %conv6, 1, !dbg !729
  %tobool8 = icmp ne i32 %and7, 0, !dbg !725
  br label %land.end

land.end:                                         ; preds = %land.rhs, %land.lhs.true, %entry
  %8 = phi i1 [ false, %land.lhs.true ], [ false, %entry ], [ %tobool8, %land.rhs ], !dbg !730
  %frombool9 = zext i1 %8 to i8, !dbg !720
  store i8 %frombool9, i8* %signing, align 1, !dbg !720
  call void @llvm.dbg.declare(metadata i8* %signature_len, metadata !731, metadata !DIExpression()), !dbg !732
  %9 = load i8, i8* %signing, align 1, !dbg !733
  %tobool10 = trunc i8 %9 to i1, !dbg !733
  %10 = zext i1 %tobool10 to i64, !dbg !733
  %cond = select i1 %tobool10, i32 13, i32 0, !dbg !733
  %conv12 = trunc i32 %cond to i8, !dbg !733
  store i8 %conv12, i8* %signature_len, align 1, !dbg !732
  call void @llvm.dbg.declare(metadata i8* %header_len, metadata !734, metadata !DIExpression()), !dbg !735
  store i8 10, i8* %header_len, align 1, !dbg !735
  call void @llvm.dbg.declare(metadata [10 x i8]* %buf, metadata !736, metadata !DIExpression()), !dbg !740
  %11 = load i8, i8* %mavlink1, align 1, !dbg !741
  %tobool13 = trunc i8 %11 to i1, !dbg !741
  br i1 %tobool13, label %if.then, label %if.else, !dbg !743

if.then:                                          ; preds = %land.end
  %12 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !744
  %magic = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %12, i32 0, i32 1, !dbg !746
  store i8 -2, i8* %magic, align 1, !dbg !747
  store i8 6, i8* %header_len, align 1, !dbg !748
  br label %if.end, !dbg !749

if.else:                                          ; preds = %land.end
  %13 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !750
  %magic14 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %13, i32 0, i32 1, !dbg !752
  store i8 -3, i8* %magic14, align 1, !dbg !753
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %14 = load i8, i8* %mavlink1, align 1, !dbg !754
  %tobool15 = trunc i8 %14 to i1, !dbg !754
  br i1 %tobool15, label %cond.true, label %cond.false, !dbg !754

cond.true:                                        ; preds = %if.end
  %15 = load i8, i8* %min_length.addr, align 1, !dbg !755
  %conv17 = zext i8 %15 to i32, !dbg !755
  br label %cond.end, !dbg !754

cond.false:                                       ; preds = %if.end
  %16 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !756
  %payload64 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %16, i32 0, i32 9, !dbg !756
  %arrayidx = getelementptr inbounds [33 x i64], [33 x i64]* %payload64, i64 0, i64 0, !dbg !756
  %17 = bitcast i64* %arrayidx to i8*, !dbg !756
  %18 = load i8, i8* %length.addr, align 1, !dbg !757
  %call = call zeroext i8 @_mav_trim_payload(i8* %17, i8 zeroext %18), !dbg !758
  %conv18 = zext i8 %call to i32, !dbg !758
  br label %cond.end, !dbg !754

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond19 = phi i32 [ %conv17, %cond.true ], [ %conv18, %cond.false ], !dbg !754
  %conv20 = trunc i32 %cond19 to i8, !dbg !754
  %19 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !759
  %len = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %19, i32 0, i32 2, !dbg !760
  store i8 %conv20, i8* %len, align 1, !dbg !761
  %20 = load i8, i8* %system_id.addr, align 1, !dbg !762
  %21 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !763
  %sysid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %21, i32 0, i32 6, !dbg !764
  store i8 %20, i8* %sysid, align 1, !dbg !765
  %22 = load i8, i8* %component_id.addr, align 1, !dbg !766
  %23 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !767
  %compid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %23, i32 0, i32 7, !dbg !768
  store i8 %22, i8* %compid, align 1, !dbg !769
  %24 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !770
  %incompat_flags = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %24, i32 0, i32 3, !dbg !771
  store i8 0, i8* %incompat_flags, align 1, !dbg !772
  %25 = load i8, i8* %signing, align 1, !dbg !773
  %tobool21 = trunc i8 %25 to i1, !dbg !773
  br i1 %tobool21, label %if.then22, label %if.end26, !dbg !775

if.then22:                                        ; preds = %cond.end
  %26 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !776
  %incompat_flags23 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %26, i32 0, i32 3, !dbg !778
  %27 = load i8, i8* %incompat_flags23, align 1, !dbg !779
  %conv24 = zext i8 %27 to i32, !dbg !779
  %or = or i32 %conv24, 1, !dbg !779
  %conv25 = trunc i32 %or to i8, !dbg !779
  store i8 %conv25, i8* %incompat_flags23, align 1, !dbg !779
  br label %if.end26, !dbg !780

if.end26:                                         ; preds = %if.then22, %cond.end
  %28 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !781
  %compat_flags = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %28, i32 0, i32 4, !dbg !782
  store i8 0, i8* %compat_flags, align 1, !dbg !783
  %29 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !784
  %current_tx_seq = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %29, i32 0, i32 6, !dbg !785
  %30 = load i8, i8* %current_tx_seq, align 2, !dbg !785
  %31 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !786
  %seq = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %31, i32 0, i32 5, !dbg !787
  store i8 %30, i8* %seq, align 1, !dbg !788
  %32 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !789
  %current_tx_seq27 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %32, i32 0, i32 6, !dbg !790
  %33 = load i8, i8* %current_tx_seq27, align 2, !dbg !790
  %conv28 = zext i8 %33 to i32, !dbg !789
  %add = add nsw i32 %conv28, 1, !dbg !791
  %conv29 = trunc i32 %add to i8, !dbg !789
  %34 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !792
  %current_tx_seq30 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %34, i32 0, i32 6, !dbg !793
  store i8 %conv29, i8* %current_tx_seq30, align 2, !dbg !794
  %35 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !795
  %magic31 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %35, i32 0, i32 1, !dbg !796
  %36 = load i8, i8* %magic31, align 1, !dbg !796
  %arrayidx32 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 0, !dbg !797
  store i8 %36, i8* %arrayidx32, align 1, !dbg !798
  %37 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !799
  %len33 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %37, i32 0, i32 2, !dbg !800
  %38 = load i8, i8* %len33, align 1, !dbg !800
  %arrayidx34 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 1, !dbg !801
  store i8 %38, i8* %arrayidx34, align 1, !dbg !802
  %39 = load i8, i8* %mavlink1, align 1, !dbg !803
  %tobool35 = trunc i8 %39 to i1, !dbg !803
  br i1 %tobool35, label %if.then36, label %if.else46, !dbg !805

if.then36:                                        ; preds = %if.end26
  %40 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !806
  %seq37 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %40, i32 0, i32 5, !dbg !808
  %41 = load i8, i8* %seq37, align 1, !dbg !808
  %arrayidx38 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 2, !dbg !809
  store i8 %41, i8* %arrayidx38, align 1, !dbg !810
  %42 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !811
  %sysid39 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %42, i32 0, i32 6, !dbg !812
  %43 = load i8, i8* %sysid39, align 1, !dbg !812
  %arrayidx40 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 3, !dbg !813
  store i8 %43, i8* %arrayidx40, align 1, !dbg !814
  %44 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !815
  %compid41 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %44, i32 0, i32 7, !dbg !816
  %45 = load i8, i8* %compid41, align 1, !dbg !816
  %arrayidx42 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 4, !dbg !817
  store i8 %45, i8* %arrayidx42, align 1, !dbg !818
  %46 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !819
  %msgid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %46, i32 0, i32 8, !dbg !820
  %47 = bitcast [3 x i8]* %msgid to i24*, !dbg !820
  %bf.load = load i24, i24* %47, align 1, !dbg !820
  %bf.cast = zext i24 %bf.load to i32, !dbg !820
  %and43 = and i32 %bf.cast, 255, !dbg !821
  %conv44 = trunc i32 %and43 to i8, !dbg !819
  %arrayidx45 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 5, !dbg !822
  store i8 %conv44, i8* %arrayidx45, align 1, !dbg !823
  br label %if.end76, !dbg !824

if.else46:                                        ; preds = %if.end26
  %48 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !825
  %incompat_flags47 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %48, i32 0, i32 3, !dbg !827
  %49 = load i8, i8* %incompat_flags47, align 1, !dbg !827
  %arrayidx48 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 2, !dbg !828
  store i8 %49, i8* %arrayidx48, align 1, !dbg !829
  %50 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !830
  %compat_flags49 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %50, i32 0, i32 4, !dbg !831
  %51 = load i8, i8* %compat_flags49, align 1, !dbg !831
  %arrayidx50 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 3, !dbg !832
  store i8 %51, i8* %arrayidx50, align 1, !dbg !833
  %52 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !834
  %seq51 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %52, i32 0, i32 5, !dbg !835
  %53 = load i8, i8* %seq51, align 1, !dbg !835
  %arrayidx52 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 4, !dbg !836
  store i8 %53, i8* %arrayidx52, align 1, !dbg !837
  %54 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !838
  %sysid53 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %54, i32 0, i32 6, !dbg !839
  %55 = load i8, i8* %sysid53, align 1, !dbg !839
  %arrayidx54 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 5, !dbg !840
  store i8 %55, i8* %arrayidx54, align 1, !dbg !841
  %56 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !842
  %compid55 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %56, i32 0, i32 7, !dbg !843
  %57 = load i8, i8* %compid55, align 1, !dbg !843
  %arrayidx56 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 6, !dbg !844
  store i8 %57, i8* %arrayidx56, align 1, !dbg !845
  %58 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !846
  %msgid57 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %58, i32 0, i32 8, !dbg !847
  %59 = bitcast [3 x i8]* %msgid57 to i24*, !dbg !847
  %bf.load58 = load i24, i24* %59, align 1, !dbg !847
  %bf.cast59 = zext i24 %bf.load58 to i32, !dbg !847
  %and60 = and i32 %bf.cast59, 255, !dbg !848
  %conv61 = trunc i32 %and60 to i8, !dbg !846
  %arrayidx62 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 7, !dbg !849
  store i8 %conv61, i8* %arrayidx62, align 1, !dbg !850
  %60 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !851
  %msgid63 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %60, i32 0, i32 8, !dbg !852
  %61 = bitcast [3 x i8]* %msgid63 to i24*, !dbg !852
  %bf.load64 = load i24, i24* %61, align 1, !dbg !852
  %bf.cast65 = zext i24 %bf.load64 to i32, !dbg !852
  %shr = ashr i32 %bf.cast65, 8, !dbg !853
  %and66 = and i32 %shr, 255, !dbg !854
  %conv67 = trunc i32 %and66 to i8, !dbg !855
  %arrayidx68 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 8, !dbg !856
  store i8 %conv67, i8* %arrayidx68, align 1, !dbg !857
  %62 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !858
  %msgid69 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %62, i32 0, i32 8, !dbg !859
  %63 = bitcast [3 x i8]* %msgid69 to i24*, !dbg !859
  %bf.load70 = load i24, i24* %63, align 1, !dbg !859
  %bf.cast71 = zext i24 %bf.load70 to i32, !dbg !859
  %shr72 = ashr i32 %bf.cast71, 16, !dbg !860
  %and73 = and i32 %shr72, 255, !dbg !861
  %conv74 = trunc i32 %and73 to i8, !dbg !862
  %arrayidx75 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 9, !dbg !863
  store i8 %conv74, i8* %arrayidx75, align 1, !dbg !864
  br label %if.end76

if.end76:                                         ; preds = %if.else46, %if.then36
  call void @llvm.dbg.declare(metadata i16* %checksum, metadata !865, metadata !DIExpression()), !dbg !866
  %arrayidx77 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 1, !dbg !867
  %64 = load i8, i8* %header_len, align 1, !dbg !868
  %conv78 = zext i8 %64 to i32, !dbg !868
  %sub = sub nsw i32 %conv78, 1, !dbg !869
  %conv79 = trunc i32 %sub to i16, !dbg !868
  %call80 = call zeroext i16 @crc_calculate(i8* %arrayidx77, i16 zeroext %conv79), !dbg !870
  store i16 %call80, i16* %checksum, align 2, !dbg !866
  %65 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !871
  %payload6481 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %65, i32 0, i32 9, !dbg !871
  %arrayidx82 = getelementptr inbounds [33 x i64], [33 x i64]* %payload6481, i64 0, i64 0, !dbg !871
  %66 = bitcast i64* %arrayidx82 to i8*, !dbg !871
  %67 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !872
  %len83 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %67, i32 0, i32 2, !dbg !873
  %68 = load i8, i8* %len83, align 1, !dbg !873
  %conv84 = zext i8 %68 to i16, !dbg !872
  call void @crc_accumulate_buffer(i16* %checksum, i8* %66, i16 zeroext %conv84), !dbg !874
  %69 = load i8, i8* %crc_extra.addr, align 1, !dbg !875
  call void @crc_accumulate(i8 zeroext %69, i16* %checksum), !dbg !876
  %70 = load i16, i16* %checksum, align 2, !dbg !877
  %conv85 = zext i16 %70 to i32, !dbg !877
  %and86 = and i32 %conv85, 255, !dbg !878
  %conv87 = trunc i32 %and86 to i8, !dbg !879
  %71 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !880
  %len88 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %71, i32 0, i32 2, !dbg !880
  %72 = load i8, i8* %len88, align 1, !dbg !880
  %conv89 = zext i8 %72 to i32, !dbg !880
  %73 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !880
  %payload6490 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %73, i32 0, i32 9, !dbg !880
  %arrayidx91 = getelementptr inbounds [33 x i64], [33 x i64]* %payload6490, i64 0, i64 0, !dbg !880
  %74 = bitcast i64* %arrayidx91 to i8*, !dbg !880
  %idx.ext = sext i32 %conv89 to i64, !dbg !880
  %add.ptr = getelementptr inbounds i8, i8* %74, i64 %idx.ext, !dbg !880
  store i8 %conv87, i8* %add.ptr, align 1, !dbg !881
  %75 = load i16, i16* %checksum, align 2, !dbg !882
  %conv92 = zext i16 %75 to i32, !dbg !882
  %shr93 = ashr i32 %conv92, 8, !dbg !883
  %conv94 = trunc i32 %shr93 to i8, !dbg !884
  %76 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !885
  %len95 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %76, i32 0, i32 2, !dbg !885
  %77 = load i8, i8* %len95, align 1, !dbg !885
  %conv96 = zext i8 %77 to i32, !dbg !885
  %add97 = add nsw i32 %conv96, 1, !dbg !885
  %78 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !885
  %payload6498 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %78, i32 0, i32 9, !dbg !885
  %arrayidx99 = getelementptr inbounds [33 x i64], [33 x i64]* %payload6498, i64 0, i64 0, !dbg !885
  %79 = bitcast i64* %arrayidx99 to i8*, !dbg !885
  %idx.ext100 = sext i32 %add97 to i64, !dbg !885
  %add.ptr101 = getelementptr inbounds i8, i8* %79, i64 %idx.ext100, !dbg !885
  store i8 %conv94, i8* %add.ptr101, align 1, !dbg !886
  %80 = load i16, i16* %checksum, align 2, !dbg !887
  %81 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !888
  %checksum102 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %81, i32 0, i32 0, !dbg !889
  store i16 %80, i16* %checksum102, align 1, !dbg !890
  %82 = load i8, i8* %signing, align 1, !dbg !891
  %tobool103 = trunc i8 %82 to i1, !dbg !891
  br i1 %tobool103, label %if.then104, label %if.end118, !dbg !893

if.then104:                                       ; preds = %if.end76
  %83 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !894
  %signing105 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %83, i32 0, i32 11, !dbg !896
  %84 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing105, align 8, !dbg !896
  %85 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !897
  %signature = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %85, i32 0, i32 11, !dbg !898
  %arraydecay = getelementptr inbounds [13 x i8], [13 x i8]* %signature, i64 0, i64 0, !dbg !897
  %arraydecay106 = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i64 0, i64 0, !dbg !899
  %86 = load i8, i8* %header_len, align 1, !dbg !900
  %87 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !901
  %payload64107 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %87, i32 0, i32 9, !dbg !901
  %arrayidx108 = getelementptr inbounds [33 x i64], [33 x i64]* %payload64107, i64 0, i64 0, !dbg !901
  %88 = bitcast i64* %arrayidx108 to i8*, !dbg !901
  %89 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !902
  %len109 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %89, i32 0, i32 2, !dbg !903
  %90 = load i8, i8* %len109, align 1, !dbg !903
  %91 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !904
  %payload64110 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %91, i32 0, i32 9, !dbg !904
  %arrayidx111 = getelementptr inbounds [33 x i64], [33 x i64]* %payload64110, i64 0, i64 0, !dbg !904
  %92 = bitcast i64* %arrayidx111 to i8*, !dbg !904
  %93 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !905
  %len112 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %93, i32 0, i32 2, !dbg !906
  %94 = load i8, i8* %len112, align 1, !dbg !906
  %conv113 = zext i8 %94 to i16, !dbg !907
  %conv114 = zext i16 %conv113 to i32, !dbg !907
  %idx.ext115 = sext i32 %conv114 to i64, !dbg !908
  %add.ptr116 = getelementptr inbounds i8, i8* %92, i64 %idx.ext115, !dbg !908
  %call117 = call zeroext i8 @mavlink_sign_packet(%struct.__mavlink_signing* %84, i8* %arraydecay, i8* %arraydecay106, i8 zeroext %86, i8* %88, i8 zeroext %90, i8* %add.ptr116), !dbg !909
  br label %if.end118, !dbg !910

if.end118:                                        ; preds = %if.then104, %if.end76
  %95 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !911
  %len119 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %95, i32 0, i32 2, !dbg !912
  %96 = load i8, i8* %len119, align 1, !dbg !912
  %conv120 = zext i8 %96 to i32, !dbg !911
  %97 = load i8, i8* %header_len, align 1, !dbg !913
  %conv121 = zext i8 %97 to i32, !dbg !913
  %add122 = add nsw i32 %conv120, %conv121, !dbg !914
  %add123 = add nsw i32 %add122, 2, !dbg !915
  %98 = load i8, i8* %signature_len, align 1, !dbg !916
  %conv124 = zext i8 %98 to i32, !dbg !916
  %add125 = add nsw i32 %add123, %conv124, !dbg !917
  %conv126 = trunc i32 %add125 to i16, !dbg !911
  ret i16 %conv126, !dbg !918
}

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i8 @_mav_trim_payload(i8* %payload, i8 zeroext %length) #0 !dbg !919 {
entry:
  %payload.addr = alloca i8*, align 8
  %length.addr = alloca i8, align 1
  store i8* %payload, i8** %payload.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %payload.addr, metadata !922, metadata !DIExpression()), !dbg !923
  store i8 %length, i8* %length.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %length.addr, metadata !924, metadata !DIExpression()), !dbg !925
  br label %while.cond, !dbg !926

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i8, i8* %length.addr, align 1, !dbg !927
  %conv = zext i8 %0 to i32, !dbg !927
  %cmp = icmp sgt i32 %conv, 1, !dbg !928
  br i1 %cmp, label %land.rhs, label %land.end, !dbg !929

land.rhs:                                         ; preds = %while.cond
  %1 = load i8*, i8** %payload.addr, align 8, !dbg !930
  %2 = load i8, i8* %length.addr, align 1, !dbg !931
  %conv2 = zext i8 %2 to i32, !dbg !931
  %sub = sub nsw i32 %conv2, 1, !dbg !932
  %idxprom = sext i32 %sub to i64, !dbg !930
  %arrayidx = getelementptr inbounds i8, i8* %1, i64 %idxprom, !dbg !930
  %3 = load i8, i8* %arrayidx, align 1, !dbg !930
  %conv3 = sext i8 %3 to i32, !dbg !930
  %cmp4 = icmp eq i32 %conv3, 0, !dbg !933
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond
  %4 = phi i1 [ false, %while.cond ], [ %cmp4, %land.rhs ], !dbg !934
  br i1 %4, label %while.body, label %while.end, !dbg !926

while.body:                                       ; preds = %land.end
  %5 = load i8, i8* %length.addr, align 1, !dbg !935
  %dec = add i8 %5, -1, !dbg !935
  store i8 %dec, i8* %length.addr, align 1, !dbg !935
  br label %while.cond, !dbg !926, !llvm.loop !937

while.end:                                        ; preds = %land.end
  %6 = load i8, i8* %length.addr, align 1, !dbg !939
  ret i8 %6, !dbg !940
}

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i16 @crc_calculate(i8* %pBuffer, i16 zeroext %length) #0 !dbg !941 {
entry:
  %pBuffer.addr = alloca i8*, align 8
  %length.addr = alloca i16, align 2
  %crcTmp = alloca i16, align 2
  store i8* %pBuffer, i8** %pBuffer.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %pBuffer.addr, metadata !945, metadata !DIExpression()), !dbg !946
  store i16 %length, i16* %length.addr, align 2
  call void @llvm.dbg.declare(metadata i16* %length.addr, metadata !947, metadata !DIExpression()), !dbg !948
  call void @llvm.dbg.declare(metadata i16* %crcTmp, metadata !949, metadata !DIExpression()), !dbg !950
  call void @crc_init(i16* %crcTmp), !dbg !951
  br label %while.cond, !dbg !952

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i16, i16* %length.addr, align 2, !dbg !953
  %dec = add i16 %0, -1, !dbg !953
  store i16 %dec, i16* %length.addr, align 2, !dbg !953
  %tobool = icmp ne i16 %0, 0, !dbg !952
  br i1 %tobool, label %while.body, label %while.end, !dbg !952

while.body:                                       ; preds = %while.cond
  %1 = load i8*, i8** %pBuffer.addr, align 8, !dbg !954
  %incdec.ptr = getelementptr inbounds i8, i8* %1, i32 1, !dbg !954
  store i8* %incdec.ptr, i8** %pBuffer.addr, align 8, !dbg !954
  %2 = load i8, i8* %1, align 1, !dbg !956
  call void @crc_accumulate(i8 zeroext %2, i16* %crcTmp), !dbg !957
  br label %while.cond, !dbg !952, !llvm.loop !958

while.end:                                        ; preds = %while.cond
  %3 = load i16, i16* %crcTmp, align 2, !dbg !960
  ret i16 %3, !dbg !961
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @crc_accumulate_buffer(i16* %crcAccum, i8* %pBuffer, i16 zeroext %length) #0 !dbg !962 {
entry:
  %crcAccum.addr = alloca i16*, align 8
  %pBuffer.addr = alloca i8*, align 8
  %length.addr = alloca i16, align 2
  %p = alloca i8*, align 8
  store i16* %crcAccum, i16** %crcAccum.addr, align 8
  call void @llvm.dbg.declare(metadata i16** %crcAccum.addr, metadata !966, metadata !DIExpression()), !dbg !967
  store i8* %pBuffer, i8** %pBuffer.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %pBuffer.addr, metadata !968, metadata !DIExpression()), !dbg !969
  store i16 %length, i16* %length.addr, align 2
  call void @llvm.dbg.declare(metadata i16* %length.addr, metadata !970, metadata !DIExpression()), !dbg !971
  call void @llvm.dbg.declare(metadata i8** %p, metadata !972, metadata !DIExpression()), !dbg !973
  %0 = load i8*, i8** %pBuffer.addr, align 8, !dbg !974
  store i8* %0, i8** %p, align 8, !dbg !973
  br label %while.cond, !dbg !975

while.cond:                                       ; preds = %while.body, %entry
  %1 = load i16, i16* %length.addr, align 2, !dbg !976
  %dec = add i16 %1, -1, !dbg !976
  store i16 %dec, i16* %length.addr, align 2, !dbg !976
  %tobool = icmp ne i16 %1, 0, !dbg !975
  br i1 %tobool, label %while.body, label %while.end, !dbg !975

while.body:                                       ; preds = %while.cond
  %2 = load i8*, i8** %p, align 8, !dbg !977
  %incdec.ptr = getelementptr inbounds i8, i8* %2, i32 1, !dbg !977
  store i8* %incdec.ptr, i8** %p, align 8, !dbg !977
  %3 = load i8, i8* %2, align 1, !dbg !979
  %4 = load i16*, i16** %crcAccum.addr, align 8, !dbg !980
  call void @crc_accumulate(i8 zeroext %3, i16* %4), !dbg !981
  br label %while.cond, !dbg !975, !llvm.loop !982

while.end:                                        ; preds = %while.cond
  ret void, !dbg !984
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @crc_accumulate(i8 zeroext %data, i16* %crcAccum) #0 !dbg !985 {
entry:
  %data.addr = alloca i8, align 1
  %crcAccum.addr = alloca i16*, align 8
  %tmp = alloca i8, align 1
  store i8 %data, i8* %data.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %data.addr, metadata !988, metadata !DIExpression()), !dbg !989
  store i16* %crcAccum, i16** %crcAccum.addr, align 8
  call void @llvm.dbg.declare(metadata i16** %crcAccum.addr, metadata !990, metadata !DIExpression()), !dbg !991
  call void @llvm.dbg.declare(metadata i8* %tmp, metadata !992, metadata !DIExpression()), !dbg !993
  %0 = load i8, i8* %data.addr, align 1, !dbg !994
  %conv = zext i8 %0 to i32, !dbg !994
  %1 = load i16*, i16** %crcAccum.addr, align 8, !dbg !995
  %2 = load i16, i16* %1, align 2, !dbg !996
  %conv1 = zext i16 %2 to i32, !dbg !996
  %and = and i32 %conv1, 255, !dbg !997
  %conv2 = trunc i32 %and to i8, !dbg !998
  %conv3 = zext i8 %conv2 to i32, !dbg !998
  %xor = xor i32 %conv, %conv3, !dbg !999
  %conv4 = trunc i32 %xor to i8, !dbg !994
  store i8 %conv4, i8* %tmp, align 1, !dbg !1000
  %3 = load i8, i8* %tmp, align 1, !dbg !1001
  %conv5 = zext i8 %3 to i32, !dbg !1001
  %shl = shl i32 %conv5, 4, !dbg !1002
  %4 = load i8, i8* %tmp, align 1, !dbg !1003
  %conv6 = zext i8 %4 to i32, !dbg !1003
  %xor7 = xor i32 %conv6, %shl, !dbg !1003
  %conv8 = trunc i32 %xor7 to i8, !dbg !1003
  store i8 %conv8, i8* %tmp, align 1, !dbg !1003
  %5 = load i16*, i16** %crcAccum.addr, align 8, !dbg !1004
  %6 = load i16, i16* %5, align 2, !dbg !1005
  %conv9 = zext i16 %6 to i32, !dbg !1005
  %shr = ashr i32 %conv9, 8, !dbg !1006
  %7 = load i8, i8* %tmp, align 1, !dbg !1007
  %conv10 = zext i8 %7 to i32, !dbg !1007
  %shl11 = shl i32 %conv10, 8, !dbg !1008
  %xor12 = xor i32 %shr, %shl11, !dbg !1009
  %8 = load i8, i8* %tmp, align 1, !dbg !1010
  %conv13 = zext i8 %8 to i32, !dbg !1010
  %shl14 = shl i32 %conv13, 3, !dbg !1011
  %xor15 = xor i32 %xor12, %shl14, !dbg !1012
  %9 = load i8, i8* %tmp, align 1, !dbg !1013
  %conv16 = zext i8 %9 to i32, !dbg !1013
  %shr17 = ashr i32 %conv16, 4, !dbg !1014
  %xor18 = xor i32 %xor15, %shr17, !dbg !1015
  %conv19 = trunc i32 %xor18 to i16, !dbg !1016
  %10 = load i16*, i16** %crcAccum.addr, align 8, !dbg !1017
  store i16 %conv19, i16* %10, align 2, !dbg !1018
  ret void, !dbg !1019
}

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i8 @mavlink_sign_packet(%struct.__mavlink_signing* %signing, i8* %signature, i8* %header, i8 zeroext %header_len, i8* %packet, i8 zeroext %packet_len, i8* %crc) #0 !dbg !1020 {
entry:
  %retval = alloca i8, align 1
  %signing.addr = alloca %struct.__mavlink_signing*, align 8
  %signature.addr = alloca i8*, align 8
  %header.addr = alloca i8*, align 8
  %header_len.addr = alloca i8, align 1
  %packet.addr = alloca i8*, align 8
  %packet_len.addr = alloca i8, align 1
  %crc.addr = alloca i8*, align 8
  %ctx = alloca %struct.mavlink_sha256_ctx, align 4
  %tstamp = alloca %union.anon.0, align 8
  store %struct.__mavlink_signing* %signing, %struct.__mavlink_signing** %signing.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_signing** %signing.addr, metadata !1025, metadata !DIExpression()), !dbg !1026
  store i8* %signature, i8** %signature.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %signature.addr, metadata !1027, metadata !DIExpression()), !dbg !1028
  store i8* %header, i8** %header.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %header.addr, metadata !1029, metadata !DIExpression()), !dbg !1030
  store i8 %header_len, i8* %header_len.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %header_len.addr, metadata !1031, metadata !DIExpression()), !dbg !1032
  store i8* %packet, i8** %packet.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %packet.addr, metadata !1033, metadata !DIExpression()), !dbg !1034
  store i8 %packet_len, i8* %packet_len.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %packet_len.addr, metadata !1035, metadata !DIExpression()), !dbg !1036
  store i8* %crc, i8** %crc.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %crc.addr, metadata !1037, metadata !DIExpression()), !dbg !1038
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx* %ctx, metadata !1039, metadata !DIExpression()), !dbg !1054
  call void @llvm.dbg.declare(metadata %union.anon.0* %tstamp, metadata !1055, metadata !DIExpression()), !dbg !1061
  %0 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1062
  %cmp = icmp eq %struct.__mavlink_signing* %0, null, !dbg !1064
  br i1 %cmp, label %if.then, label %lor.lhs.false, !dbg !1065

lor.lhs.false:                                    ; preds = %entry
  %1 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1066
  %flags = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %1, i32 0, i32 0, !dbg !1067
  %2 = load i8, i8* %flags, align 8, !dbg !1067
  %conv = zext i8 %2 to i32, !dbg !1066
  %and = and i32 %conv, 1, !dbg !1068
  %tobool = icmp ne i32 %and, 0, !dbg !1068
  br i1 %tobool, label %if.end, label %if.then, !dbg !1069

if.then:                                          ; preds = %lor.lhs.false, %entry
  store i8 0, i8* %retval, align 1, !dbg !1070
  br label %return, !dbg !1070

if.end:                                           ; preds = %lor.lhs.false
  %3 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1072
  %link_id = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %3, i32 0, i32 1, !dbg !1073
  %4 = load i8, i8* %link_id, align 1, !dbg !1073
  %5 = load i8*, i8** %signature.addr, align 8, !dbg !1074
  %arrayidx = getelementptr inbounds i8, i8* %5, i64 0, !dbg !1074
  store i8 %4, i8* %arrayidx, align 1, !dbg !1075
  %6 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1076
  %timestamp = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %6, i32 0, i32 2, !dbg !1077
  %7 = load i64, i64* %timestamp, align 8, !dbg !1077
  %t64 = bitcast %union.anon.0* %tstamp to i64*, !dbg !1078
  store i64 %7, i64* %t64, align 8, !dbg !1079
  %8 = load i8*, i8** %signature.addr, align 8, !dbg !1080
  %arrayidx1 = getelementptr inbounds i8, i8* %8, i64 1, !dbg !1080
  %t8 = bitcast %union.anon.0* %tstamp to [8 x i8]*, !dbg !1081
  %arraydecay = getelementptr inbounds [8 x i8], [8 x i8]* %t8, i64 0, i64 0, !dbg !1082
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %arrayidx1, i8* align 8 %arraydecay, i64 6, i1 false), !dbg !1082
  %9 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1083
  %timestamp2 = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %9, i32 0, i32 2, !dbg !1084
  %10 = load i64, i64* %timestamp2, align 8, !dbg !1085
  %inc = add i64 %10, 1, !dbg !1085
  store i64 %inc, i64* %timestamp2, align 8, !dbg !1085
  call void @mavlink_sha256_init(%struct.mavlink_sha256_ctx* %ctx), !dbg !1086
  %11 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1087
  %secret_key = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %11, i32 0, i32 3, !dbg !1088
  %arraydecay3 = getelementptr inbounds [32 x i8], [32 x i8]* %secret_key, i64 0, i64 0, !dbg !1087
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %arraydecay3, i32 32), !dbg !1089
  %12 = load i8*, i8** %header.addr, align 8, !dbg !1090
  %13 = load i8, i8* %header_len.addr, align 1, !dbg !1091
  %conv4 = zext i8 %13 to i32, !dbg !1091
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %12, i32 %conv4), !dbg !1092
  %14 = load i8*, i8** %packet.addr, align 8, !dbg !1093
  %15 = load i8, i8* %packet_len.addr, align 1, !dbg !1094
  %conv5 = zext i8 %15 to i32, !dbg !1094
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %14, i32 %conv5), !dbg !1095
  %16 = load i8*, i8** %crc.addr, align 8, !dbg !1096
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %16, i32 2), !dbg !1097
  %17 = load i8*, i8** %signature.addr, align 8, !dbg !1098
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %17, i32 7), !dbg !1099
  %18 = load i8*, i8** %signature.addr, align 8, !dbg !1100
  %arrayidx6 = getelementptr inbounds i8, i8* %18, i64 7, !dbg !1100
  call void @mavlink_sha256_final_48(%struct.mavlink_sha256_ctx* %ctx, i8* %arrayidx6), !dbg !1101
  store i8 13, i8* %retval, align 1, !dbg !1102
  br label %return, !dbg !1102

return:                                           ; preds = %if.end, %if.then
  %19 = load i8, i8* %retval, align 1, !dbg !1103
  ret i8 %19, !dbg !1103
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @crc_init(i16* %crcAccum) #0 !dbg !1104 {
entry:
  %crcAccum.addr = alloca i16*, align 8
  store i16* %crcAccum, i16** %crcAccum.addr, align 8
  call void @llvm.dbg.declare(metadata i16** %crcAccum.addr, metadata !1107, metadata !DIExpression()), !dbg !1108
  %0 = load i16*, i16** %crcAccum.addr, align 8, !dbg !1109
  store i16 -1, i16* %0, align 2, !dbg !1110
  ret void, !dbg !1111
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_sha256_init(%struct.mavlink_sha256_ctx* %m) #0 !dbg !1112 {
entry:
  %m.addr = alloca %struct.mavlink_sha256_ctx*, align 8
  store %struct.mavlink_sha256_ctx* %m, %struct.mavlink_sha256_ctx** %m.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx** %m.addr, metadata !1116, metadata !DIExpression()), !dbg !1117
  %0 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1118
  %sz = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %0, i32 0, i32 0, !dbg !1119
  %arrayidx = getelementptr inbounds [2 x i32], [2 x i32]* %sz, i64 0, i64 0, !dbg !1118
  store i32 0, i32* %arrayidx, align 4, !dbg !1120
  %1 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1121
  %sz1 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %1, i32 0, i32 0, !dbg !1122
  %arrayidx2 = getelementptr inbounds [2 x i32], [2 x i32]* %sz1, i64 0, i64 1, !dbg !1121
  store i32 0, i32* %arrayidx2, align 4, !dbg !1123
  %2 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1124
  %counter = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %2, i32 0, i32 1, !dbg !1125
  %arrayidx3 = getelementptr inbounds [8 x i32], [8 x i32]* %counter, i64 0, i64 0, !dbg !1124
  store i32 1779033703, i32* %arrayidx3, align 4, !dbg !1126
  %3 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1127
  %counter4 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %3, i32 0, i32 1, !dbg !1128
  %arrayidx5 = getelementptr inbounds [8 x i32], [8 x i32]* %counter4, i64 0, i64 1, !dbg !1127
  store i32 -1150833019, i32* %arrayidx5, align 4, !dbg !1129
  %4 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1130
  %counter6 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %4, i32 0, i32 1, !dbg !1131
  %arrayidx7 = getelementptr inbounds [8 x i32], [8 x i32]* %counter6, i64 0, i64 2, !dbg !1130
  store i32 1013904242, i32* %arrayidx7, align 4, !dbg !1132
  %5 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1133
  %counter8 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %5, i32 0, i32 1, !dbg !1134
  %arrayidx9 = getelementptr inbounds [8 x i32], [8 x i32]* %counter8, i64 0, i64 3, !dbg !1133
  store i32 -1521486534, i32* %arrayidx9, align 4, !dbg !1135
  %6 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1136
  %counter10 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %6, i32 0, i32 1, !dbg !1137
  %arrayidx11 = getelementptr inbounds [8 x i32], [8 x i32]* %counter10, i64 0, i64 4, !dbg !1136
  store i32 1359893119, i32* %arrayidx11, align 4, !dbg !1138
  %7 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1139
  %counter12 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %7, i32 0, i32 1, !dbg !1140
  %arrayidx13 = getelementptr inbounds [8 x i32], [8 x i32]* %counter12, i64 0, i64 5, !dbg !1139
  store i32 -1694144372, i32* %arrayidx13, align 4, !dbg !1141
  %8 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1142
  %counter14 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %8, i32 0, i32 1, !dbg !1143
  %arrayidx15 = getelementptr inbounds [8 x i32], [8 x i32]* %counter14, i64 0, i64 6, !dbg !1142
  store i32 528734635, i32* %arrayidx15, align 4, !dbg !1144
  %9 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1145
  %counter16 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %9, i32 0, i32 1, !dbg !1146
  %arrayidx17 = getelementptr inbounds [8 x i32], [8 x i32]* %counter16, i64 0, i64 7, !dbg !1145
  store i32 1541459225, i32* %arrayidx17, align 4, !dbg !1147
  ret void, !dbg !1148
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %m, i8* %v, i32 %len) #0 !dbg !1149 {
entry:
  %m.addr = alloca %struct.mavlink_sha256_ctx*, align 8
  %v.addr = alloca i8*, align 8
  %len.addr = alloca i32, align 4
  %p = alloca i8*, align 8
  %old_sz = alloca i32, align 4
  %offset = alloca i32, align 4
  %l = alloca i32, align 4
  %i = alloca i32, align 4
  %current = alloca [16 x i32], align 16
  %u18 = alloca i32*, align 8
  %p1 = alloca i8*, align 8
  %p2 = alloca i8*, align 8
  store %struct.mavlink_sha256_ctx* %m, %struct.mavlink_sha256_ctx** %m.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx** %m.addr, metadata !1154, metadata !DIExpression()), !dbg !1155
  store i8* %v, i8** %v.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %v.addr, metadata !1156, metadata !DIExpression()), !dbg !1157
  store i32 %len, i32* %len.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %len.addr, metadata !1158, metadata !DIExpression()), !dbg !1159
  call void @llvm.dbg.declare(metadata i8** %p, metadata !1160, metadata !DIExpression()), !dbg !1161
  %0 = load i8*, i8** %v.addr, align 8, !dbg !1162
  store i8* %0, i8** %p, align 8, !dbg !1161
  call void @llvm.dbg.declare(metadata i32* %old_sz, metadata !1163, metadata !DIExpression()), !dbg !1164
  %1 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1165
  %sz = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %1, i32 0, i32 0, !dbg !1166
  %arrayidx = getelementptr inbounds [2 x i32], [2 x i32]* %sz, i64 0, i64 0, !dbg !1165
  %2 = load i32, i32* %arrayidx, align 4, !dbg !1165
  store i32 %2, i32* %old_sz, align 4, !dbg !1164
  call void @llvm.dbg.declare(metadata i32* %offset, metadata !1167, metadata !DIExpression()), !dbg !1168
  %3 = load i32, i32* %len.addr, align 4, !dbg !1169
  %mul = mul i32 %3, 8, !dbg !1170
  %4 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1171
  %sz1 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %4, i32 0, i32 0, !dbg !1172
  %arrayidx2 = getelementptr inbounds [2 x i32], [2 x i32]* %sz1, i64 0, i64 0, !dbg !1171
  %5 = load i32, i32* %arrayidx2, align 4, !dbg !1173
  %add = add i32 %5, %mul, !dbg !1173
  store i32 %add, i32* %arrayidx2, align 4, !dbg !1173
  %6 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1174
  %sz3 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %6, i32 0, i32 0, !dbg !1176
  %arrayidx4 = getelementptr inbounds [2 x i32], [2 x i32]* %sz3, i64 0, i64 0, !dbg !1174
  %7 = load i32, i32* %arrayidx4, align 4, !dbg !1174
  %8 = load i32, i32* %old_sz, align 4, !dbg !1177
  %cmp = icmp ult i32 %7, %8, !dbg !1178
  br i1 %cmp, label %if.then, label %if.end, !dbg !1179

if.then:                                          ; preds = %entry
  %9 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1180
  %sz5 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %9, i32 0, i32 0, !dbg !1181
  %arrayidx6 = getelementptr inbounds [2 x i32], [2 x i32]* %sz5, i64 0, i64 1, !dbg !1180
  %10 = load i32, i32* %arrayidx6, align 4, !dbg !1182
  %inc = add i32 %10, 1, !dbg !1182
  store i32 %inc, i32* %arrayidx6, align 4, !dbg !1182
  br label %if.end, !dbg !1182

if.end:                                           ; preds = %if.then, %entry
  %11 = load i32, i32* %old_sz, align 4, !dbg !1183
  %div = udiv i32 %11, 8, !dbg !1184
  %rem = urem i32 %div, 64, !dbg !1185
  store i32 %rem, i32* %offset, align 4, !dbg !1186
  br label %while.cond, !dbg !1187

while.cond:                                       ; preds = %if.end36, %if.end
  %12 = load i32, i32* %len.addr, align 4, !dbg !1188
  %cmp7 = icmp ugt i32 %12, 0, !dbg !1189
  br i1 %cmp7, label %while.body, label %while.end, !dbg !1187

while.body:                                       ; preds = %while.cond
  call void @llvm.dbg.declare(metadata i32* %l, metadata !1190, metadata !DIExpression()), !dbg !1192
  %13 = load i32, i32* %offset, align 4, !dbg !1193
  %sub = sub i32 64, %13, !dbg !1194
  store i32 %sub, i32* %l, align 4, !dbg !1192
  %14 = load i32, i32* %len.addr, align 4, !dbg !1195
  %15 = load i32, i32* %l, align 4, !dbg !1197
  %cmp8 = icmp ult i32 %14, %15, !dbg !1198
  br i1 %cmp8, label %if.then9, label %if.end10, !dbg !1199

if.then9:                                         ; preds = %while.body
  %16 = load i32, i32* %len.addr, align 4, !dbg !1200
  store i32 %16, i32* %l, align 4, !dbg !1202
  br label %if.end10, !dbg !1203

if.end10:                                         ; preds = %if.then9, %while.body
  %17 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1204
  %u = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %17, i32 0, i32 2, !dbg !1205
  %save_bytes = bitcast %union.anon* %u to [64 x i8]*, !dbg !1206
  %arraydecay = getelementptr inbounds [64 x i8], [64 x i8]* %save_bytes, i64 0, i64 0, !dbg !1204
  %18 = load i32, i32* %offset, align 4, !dbg !1207
  %idx.ext = zext i32 %18 to i64, !dbg !1208
  %add.ptr = getelementptr inbounds i8, i8* %arraydecay, i64 %idx.ext, !dbg !1208
  %19 = load i8*, i8** %p, align 8, !dbg !1209
  %20 = load i32, i32* %l, align 4, !dbg !1210
  %conv = zext i32 %20 to i64, !dbg !1210
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %add.ptr, i8* align 1 %19, i64 %conv, i1 false), !dbg !1211
  %21 = load i32, i32* %l, align 4, !dbg !1212
  %22 = load i32, i32* %offset, align 4, !dbg !1213
  %add11 = add i32 %22, %21, !dbg !1213
  store i32 %add11, i32* %offset, align 4, !dbg !1213
  %23 = load i32, i32* %l, align 4, !dbg !1214
  %24 = load i8*, i8** %p, align 8, !dbg !1215
  %idx.ext12 = zext i32 %23 to i64, !dbg !1215
  %add.ptr13 = getelementptr inbounds i8, i8* %24, i64 %idx.ext12, !dbg !1215
  store i8* %add.ptr13, i8** %p, align 8, !dbg !1215
  %25 = load i32, i32* %l, align 4, !dbg !1216
  %26 = load i32, i32* %len.addr, align 4, !dbg !1217
  %sub14 = sub i32 %26, %25, !dbg !1217
  store i32 %sub14, i32* %len.addr, align 4, !dbg !1217
  %27 = load i32, i32* %offset, align 4, !dbg !1218
  %cmp15 = icmp eq i32 %27, 64, !dbg !1220
  br i1 %cmp15, label %if.then17, label %if.end36, !dbg !1221

if.then17:                                        ; preds = %if.end10
  call void @llvm.dbg.declare(metadata i32* %i, metadata !1222, metadata !DIExpression()), !dbg !1224
  call void @llvm.dbg.declare(metadata [16 x i32]* %current, metadata !1225, metadata !DIExpression()), !dbg !1226
  call void @llvm.dbg.declare(metadata i32** %u18, metadata !1227, metadata !DIExpression()), !dbg !1229
  %28 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1230
  %u19 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %28, i32 0, i32 2, !dbg !1231
  %save_u32 = bitcast %union.anon* %u19 to [16 x i32]*, !dbg !1232
  %arraydecay20 = getelementptr inbounds [16 x i32], [16 x i32]* %save_u32, i64 0, i64 0, !dbg !1230
  store i32* %arraydecay20, i32** %u18, align 8, !dbg !1229
  store i32 0, i32* %i, align 4, !dbg !1233
  br label %for.cond, !dbg !1235

for.cond:                                         ; preds = %for.inc, %if.then17
  %29 = load i32, i32* %i, align 4, !dbg !1236
  %cmp21 = icmp slt i32 %29, 16, !dbg !1238
  br i1 %cmp21, label %for.body, label %for.end, !dbg !1239

for.body:                                         ; preds = %for.cond
  call void @llvm.dbg.declare(metadata i8** %p1, metadata !1240, metadata !DIExpression()), !dbg !1242
  %30 = load i32*, i32** %u18, align 8, !dbg !1243
  %31 = load i32, i32* %i, align 4, !dbg !1244
  %idxprom = sext i32 %31 to i64, !dbg !1243
  %arrayidx23 = getelementptr inbounds i32, i32* %30, i64 %idxprom, !dbg !1243
  %32 = bitcast i32* %arrayidx23 to i8*, !dbg !1245
  store i8* %32, i8** %p1, align 8, !dbg !1242
  call void @llvm.dbg.declare(metadata i8** %p2, metadata !1246, metadata !DIExpression()), !dbg !1247
  %33 = load i32, i32* %i, align 4, !dbg !1248
  %idxprom24 = sext i32 %33 to i64, !dbg !1249
  %arrayidx25 = getelementptr inbounds [16 x i32], [16 x i32]* %current, i64 0, i64 %idxprom24, !dbg !1249
  %34 = bitcast i32* %arrayidx25 to i8*, !dbg !1250
  store i8* %34, i8** %p2, align 8, !dbg !1247
  %35 = load i8*, i8** %p1, align 8, !dbg !1251
  %arrayidx26 = getelementptr inbounds i8, i8* %35, i64 3, !dbg !1251
  %36 = load i8, i8* %arrayidx26, align 1, !dbg !1251
  %37 = load i8*, i8** %p2, align 8, !dbg !1252
  %arrayidx27 = getelementptr inbounds i8, i8* %37, i64 0, !dbg !1252
  store i8 %36, i8* %arrayidx27, align 1, !dbg !1253
  %38 = load i8*, i8** %p1, align 8, !dbg !1254
  %arrayidx28 = getelementptr inbounds i8, i8* %38, i64 2, !dbg !1254
  %39 = load i8, i8* %arrayidx28, align 1, !dbg !1254
  %40 = load i8*, i8** %p2, align 8, !dbg !1255
  %arrayidx29 = getelementptr inbounds i8, i8* %40, i64 1, !dbg !1255
  store i8 %39, i8* %arrayidx29, align 1, !dbg !1256
  %41 = load i8*, i8** %p1, align 8, !dbg !1257
  %arrayidx30 = getelementptr inbounds i8, i8* %41, i64 1, !dbg !1257
  %42 = load i8, i8* %arrayidx30, align 1, !dbg !1257
  %43 = load i8*, i8** %p2, align 8, !dbg !1258
  %arrayidx31 = getelementptr inbounds i8, i8* %43, i64 2, !dbg !1258
  store i8 %42, i8* %arrayidx31, align 1, !dbg !1259
  %44 = load i8*, i8** %p1, align 8, !dbg !1260
  %arrayidx32 = getelementptr inbounds i8, i8* %44, i64 0, !dbg !1260
  %45 = load i8, i8* %arrayidx32, align 1, !dbg !1260
  %46 = load i8*, i8** %p2, align 8, !dbg !1261
  %arrayidx33 = getelementptr inbounds i8, i8* %46, i64 3, !dbg !1261
  store i8 %45, i8* %arrayidx33, align 1, !dbg !1262
  br label %for.inc, !dbg !1263

for.inc:                                          ; preds = %for.body
  %47 = load i32, i32* %i, align 4, !dbg !1264
  %inc34 = add nsw i32 %47, 1, !dbg !1264
  store i32 %inc34, i32* %i, align 4, !dbg !1264
  br label %for.cond, !dbg !1265, !llvm.loop !1266

for.end:                                          ; preds = %for.cond
  %48 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1268
  %arraydecay35 = getelementptr inbounds [16 x i32], [16 x i32]* %current, i64 0, i64 0, !dbg !1269
  call void @mavlink_sha256_calc(%struct.mavlink_sha256_ctx* %48, i32* %arraydecay35), !dbg !1270
  store i32 0, i32* %offset, align 4, !dbg !1271
  br label %if.end36, !dbg !1272

if.end36:                                         ; preds = %for.end, %if.end10
  br label %while.cond, !dbg !1187, !llvm.loop !1273

while.end:                                        ; preds = %while.cond
  ret void, !dbg !1275
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_sha256_final_48(%struct.mavlink_sha256_ctx* %m, i8* %result) #0 !dbg !1276 {
entry:
  %m.addr = alloca %struct.mavlink_sha256_ctx*, align 8
  %result.addr = alloca i8*, align 8
  %zeros = alloca [72 x i8], align 16
  %offset = alloca i32, align 4
  %dstart = alloca i32, align 4
  %p = alloca i8*, align 8
  store %struct.mavlink_sha256_ctx* %m, %struct.mavlink_sha256_ctx** %m.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx** %m.addr, metadata !1279, metadata !DIExpression()), !dbg !1280
  store i8* %result, i8** %result.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %result.addr, metadata !1281, metadata !DIExpression()), !dbg !1282
  call void @llvm.dbg.declare(metadata [72 x i8]* %zeros, metadata !1283, metadata !DIExpression()), !dbg !1287
  call void @llvm.dbg.declare(metadata i32* %offset, metadata !1288, metadata !DIExpression()), !dbg !1289
  %0 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1290
  %sz = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %0, i32 0, i32 0, !dbg !1291
  %arrayidx = getelementptr inbounds [2 x i32], [2 x i32]* %sz, i64 0, i64 0, !dbg !1290
  %1 = load i32, i32* %arrayidx, align 4, !dbg !1290
  %div = udiv i32 %1, 8, !dbg !1292
  %rem = urem i32 %div, 64, !dbg !1293
  store i32 %rem, i32* %offset, align 4, !dbg !1289
  call void @llvm.dbg.declare(metadata i32* %dstart, metadata !1294, metadata !DIExpression()), !dbg !1295
  %2 = load i32, i32* %offset, align 4, !dbg !1296
  %sub = sub i32 120, %2, !dbg !1297
  %sub1 = sub i32 %sub, 1, !dbg !1298
  %rem2 = urem i32 %sub1, 64, !dbg !1299
  %add = add i32 %rem2, 1, !dbg !1300
  store i32 %add, i32* %dstart, align 4, !dbg !1295
  call void @llvm.dbg.declare(metadata i8** %p, metadata !1301, metadata !DIExpression()), !dbg !1302
  %3 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1303
  %counter = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %3, i32 0, i32 1, !dbg !1304
  %arrayidx3 = getelementptr inbounds [8 x i32], [8 x i32]* %counter, i64 0, i64 0, !dbg !1303
  %4 = bitcast i32* %arrayidx3 to i8*, !dbg !1305
  store i8* %4, i8** %p, align 8, !dbg !1302
  %arraydecay = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 0, !dbg !1306
  store i8 -128, i8* %arraydecay, align 16, !dbg !1307
  %arraydecay4 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 0, !dbg !1308
  %add.ptr = getelementptr inbounds i8, i8* %arraydecay4, i64 1, !dbg !1309
  call void @llvm.memset.p0i8.i64(i8* align 1 %add.ptr, i8 0, i64 71, i1 false), !dbg !1310
  %5 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1311
  %sz5 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %5, i32 0, i32 0, !dbg !1312
  %arrayidx6 = getelementptr inbounds [2 x i32], [2 x i32]* %sz5, i64 0, i64 0, !dbg !1311
  %6 = load i32, i32* %arrayidx6, align 4, !dbg !1311
  %shr = lshr i32 %6, 0, !dbg !1313
  %and = and i32 %shr, 255, !dbg !1314
  %conv = trunc i32 %and to i8, !dbg !1315
  %7 = load i32, i32* %dstart, align 4, !dbg !1316
  %add7 = add i32 %7, 7, !dbg !1317
  %idxprom = zext i32 %add7 to i64, !dbg !1318
  %arrayidx8 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom, !dbg !1318
  store i8 %conv, i8* %arrayidx8, align 1, !dbg !1319
  %8 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1320
  %sz9 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %8, i32 0, i32 0, !dbg !1321
  %arrayidx10 = getelementptr inbounds [2 x i32], [2 x i32]* %sz9, i64 0, i64 0, !dbg !1320
  %9 = load i32, i32* %arrayidx10, align 4, !dbg !1320
  %shr11 = lshr i32 %9, 8, !dbg !1322
  %and12 = and i32 %shr11, 255, !dbg !1323
  %conv13 = trunc i32 %and12 to i8, !dbg !1324
  %10 = load i32, i32* %dstart, align 4, !dbg !1325
  %add14 = add i32 %10, 6, !dbg !1326
  %idxprom15 = zext i32 %add14 to i64, !dbg !1327
  %arrayidx16 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom15, !dbg !1327
  store i8 %conv13, i8* %arrayidx16, align 1, !dbg !1328
  %11 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1329
  %sz17 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %11, i32 0, i32 0, !dbg !1330
  %arrayidx18 = getelementptr inbounds [2 x i32], [2 x i32]* %sz17, i64 0, i64 0, !dbg !1329
  %12 = load i32, i32* %arrayidx18, align 4, !dbg !1329
  %shr19 = lshr i32 %12, 16, !dbg !1331
  %and20 = and i32 %shr19, 255, !dbg !1332
  %conv21 = trunc i32 %and20 to i8, !dbg !1333
  %13 = load i32, i32* %dstart, align 4, !dbg !1334
  %add22 = add i32 %13, 5, !dbg !1335
  %idxprom23 = zext i32 %add22 to i64, !dbg !1336
  %arrayidx24 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom23, !dbg !1336
  store i8 %conv21, i8* %arrayidx24, align 1, !dbg !1337
  %14 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1338
  %sz25 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %14, i32 0, i32 0, !dbg !1339
  %arrayidx26 = getelementptr inbounds [2 x i32], [2 x i32]* %sz25, i64 0, i64 0, !dbg !1338
  %15 = load i32, i32* %arrayidx26, align 4, !dbg !1338
  %shr27 = lshr i32 %15, 24, !dbg !1340
  %and28 = and i32 %shr27, 255, !dbg !1341
  %conv29 = trunc i32 %and28 to i8, !dbg !1342
  %16 = load i32, i32* %dstart, align 4, !dbg !1343
  %add30 = add i32 %16, 4, !dbg !1344
  %idxprom31 = zext i32 %add30 to i64, !dbg !1345
  %arrayidx32 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom31, !dbg !1345
  store i8 %conv29, i8* %arrayidx32, align 1, !dbg !1346
  %17 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1347
  %sz33 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %17, i32 0, i32 0, !dbg !1348
  %arrayidx34 = getelementptr inbounds [2 x i32], [2 x i32]* %sz33, i64 0, i64 1, !dbg !1347
  %18 = load i32, i32* %arrayidx34, align 4, !dbg !1347
  %shr35 = lshr i32 %18, 0, !dbg !1349
  %and36 = and i32 %shr35, 255, !dbg !1350
  %conv37 = trunc i32 %and36 to i8, !dbg !1351
  %19 = load i32, i32* %dstart, align 4, !dbg !1352
  %add38 = add i32 %19, 3, !dbg !1353
  %idxprom39 = zext i32 %add38 to i64, !dbg !1354
  %arrayidx40 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom39, !dbg !1354
  store i8 %conv37, i8* %arrayidx40, align 1, !dbg !1355
  %20 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1356
  %sz41 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %20, i32 0, i32 0, !dbg !1357
  %arrayidx42 = getelementptr inbounds [2 x i32], [2 x i32]* %sz41, i64 0, i64 1, !dbg !1356
  %21 = load i32, i32* %arrayidx42, align 4, !dbg !1356
  %shr43 = lshr i32 %21, 8, !dbg !1358
  %and44 = and i32 %shr43, 255, !dbg !1359
  %conv45 = trunc i32 %and44 to i8, !dbg !1360
  %22 = load i32, i32* %dstart, align 4, !dbg !1361
  %add46 = add i32 %22, 2, !dbg !1362
  %idxprom47 = zext i32 %add46 to i64, !dbg !1363
  %arrayidx48 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom47, !dbg !1363
  store i8 %conv45, i8* %arrayidx48, align 1, !dbg !1364
  %23 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1365
  %sz49 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %23, i32 0, i32 0, !dbg !1366
  %arrayidx50 = getelementptr inbounds [2 x i32], [2 x i32]* %sz49, i64 0, i64 1, !dbg !1365
  %24 = load i32, i32* %arrayidx50, align 4, !dbg !1365
  %shr51 = lshr i32 %24, 16, !dbg !1367
  %and52 = and i32 %shr51, 255, !dbg !1368
  %conv53 = trunc i32 %and52 to i8, !dbg !1369
  %25 = load i32, i32* %dstart, align 4, !dbg !1370
  %add54 = add i32 %25, 1, !dbg !1371
  %idxprom55 = zext i32 %add54 to i64, !dbg !1372
  %arrayidx56 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom55, !dbg !1372
  store i8 %conv53, i8* %arrayidx56, align 1, !dbg !1373
  %26 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1374
  %sz57 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %26, i32 0, i32 0, !dbg !1375
  %arrayidx58 = getelementptr inbounds [2 x i32], [2 x i32]* %sz57, i64 0, i64 1, !dbg !1374
  %27 = load i32, i32* %arrayidx58, align 4, !dbg !1374
  %shr59 = lshr i32 %27, 24, !dbg !1376
  %and60 = and i32 %shr59, 255, !dbg !1377
  %conv61 = trunc i32 %and60 to i8, !dbg !1378
  %28 = load i32, i32* %dstart, align 4, !dbg !1379
  %add62 = add i32 %28, 0, !dbg !1380
  %idxprom63 = zext i32 %add62 to i64, !dbg !1381
  %arrayidx64 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom63, !dbg !1381
  store i8 %conv61, i8* %arrayidx64, align 1, !dbg !1382
  %29 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1383
  %arraydecay65 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 0, !dbg !1384
  %30 = load i32, i32* %dstart, align 4, !dbg !1385
  %add66 = add i32 %30, 8, !dbg !1386
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %29, i8* %arraydecay65, i32 %add66), !dbg !1387
  %31 = load i8*, i8** %p, align 8, !dbg !1388
  %arrayidx67 = getelementptr inbounds i8, i8* %31, i64 3, !dbg !1388
  %32 = load i8, i8* %arrayidx67, align 1, !dbg !1388
  %33 = load i8*, i8** %result.addr, align 8, !dbg !1389
  %arrayidx68 = getelementptr inbounds i8, i8* %33, i64 0, !dbg !1389
  store i8 %32, i8* %arrayidx68, align 1, !dbg !1390
  %34 = load i8*, i8** %p, align 8, !dbg !1391
  %arrayidx69 = getelementptr inbounds i8, i8* %34, i64 2, !dbg !1391
  %35 = load i8, i8* %arrayidx69, align 1, !dbg !1391
  %36 = load i8*, i8** %result.addr, align 8, !dbg !1392
  %arrayidx70 = getelementptr inbounds i8, i8* %36, i64 1, !dbg !1392
  store i8 %35, i8* %arrayidx70, align 1, !dbg !1393
  %37 = load i8*, i8** %p, align 8, !dbg !1394
  %arrayidx71 = getelementptr inbounds i8, i8* %37, i64 1, !dbg !1394
  %38 = load i8, i8* %arrayidx71, align 1, !dbg !1394
  %39 = load i8*, i8** %result.addr, align 8, !dbg !1395
  %arrayidx72 = getelementptr inbounds i8, i8* %39, i64 2, !dbg !1395
  store i8 %38, i8* %arrayidx72, align 1, !dbg !1396
  %40 = load i8*, i8** %p, align 8, !dbg !1397
  %arrayidx73 = getelementptr inbounds i8, i8* %40, i64 0, !dbg !1397
  %41 = load i8, i8* %arrayidx73, align 1, !dbg !1397
  %42 = load i8*, i8** %result.addr, align 8, !dbg !1398
  %arrayidx74 = getelementptr inbounds i8, i8* %42, i64 3, !dbg !1398
  store i8 %41, i8* %arrayidx74, align 1, !dbg !1399
  %43 = load i8*, i8** %p, align 8, !dbg !1400
  %arrayidx75 = getelementptr inbounds i8, i8* %43, i64 7, !dbg !1400
  %44 = load i8, i8* %arrayidx75, align 1, !dbg !1400
  %45 = load i8*, i8** %result.addr, align 8, !dbg !1401
  %arrayidx76 = getelementptr inbounds i8, i8* %45, i64 4, !dbg !1401
  store i8 %44, i8* %arrayidx76, align 1, !dbg !1402
  %46 = load i8*, i8** %p, align 8, !dbg !1403
  %arrayidx77 = getelementptr inbounds i8, i8* %46, i64 6, !dbg !1403
  %47 = load i8, i8* %arrayidx77, align 1, !dbg !1403
  %48 = load i8*, i8** %result.addr, align 8, !dbg !1404
  %arrayidx78 = getelementptr inbounds i8, i8* %48, i64 5, !dbg !1404
  store i8 %47, i8* %arrayidx78, align 1, !dbg !1405
  ret void, !dbg !1406
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_sha256_calc(%struct.mavlink_sha256_ctx* %m, i32* %in) #0 !dbg !1407 {
entry:
  %m.addr = alloca %struct.mavlink_sha256_ctx*, align 8
  %in.addr = alloca i32*, align 8
  %AA = alloca i32, align 4
  %BB = alloca i32, align 4
  %CC = alloca i32, align 4
  %DD = alloca i32, align 4
  %EE = alloca i32, align 4
  %FF = alloca i32, align 4
  %GG = alloca i32, align 4
  %HH = alloca i32, align 4
  %data = alloca [64 x i32], align 16
  %i = alloca i32, align 4
  %T1 = alloca i32, align 4
  %T2 = alloca i32, align 4
  store %struct.mavlink_sha256_ctx* %m, %struct.mavlink_sha256_ctx** %m.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx** %m.addr, metadata !1411, metadata !DIExpression()), !dbg !1412
  store i32* %in, i32** %in.addr, align 8
  call void @llvm.dbg.declare(metadata i32** %in.addr, metadata !1413, metadata !DIExpression()), !dbg !1414
  call void @llvm.dbg.declare(metadata i32* %AA, metadata !1415, metadata !DIExpression()), !dbg !1416
  call void @llvm.dbg.declare(metadata i32* %BB, metadata !1417, metadata !DIExpression()), !dbg !1418
  call void @llvm.dbg.declare(metadata i32* %CC, metadata !1419, metadata !DIExpression()), !dbg !1420
  call void @llvm.dbg.declare(metadata i32* %DD, metadata !1421, metadata !DIExpression()), !dbg !1422
  call void @llvm.dbg.declare(metadata i32* %EE, metadata !1423, metadata !DIExpression()), !dbg !1424
  call void @llvm.dbg.declare(metadata i32* %FF, metadata !1425, metadata !DIExpression()), !dbg !1426
  call void @llvm.dbg.declare(metadata i32* %GG, metadata !1427, metadata !DIExpression()), !dbg !1428
  call void @llvm.dbg.declare(metadata i32* %HH, metadata !1429, metadata !DIExpression()), !dbg !1430
  call void @llvm.dbg.declare(metadata [64 x i32]* %data, metadata !1431, metadata !DIExpression()), !dbg !1433
  call void @llvm.dbg.declare(metadata i32* %i, metadata !1434, metadata !DIExpression()), !dbg !1435
  %0 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1436
  %counter = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %0, i32 0, i32 1, !dbg !1437
  %arrayidx = getelementptr inbounds [8 x i32], [8 x i32]* %counter, i64 0, i64 0, !dbg !1436
  %1 = load i32, i32* %arrayidx, align 4, !dbg !1436
  store i32 %1, i32* %AA, align 4, !dbg !1438
  %2 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1439
  %counter1 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %2, i32 0, i32 1, !dbg !1440
  %arrayidx2 = getelementptr inbounds [8 x i32], [8 x i32]* %counter1, i64 0, i64 1, !dbg !1439
  %3 = load i32, i32* %arrayidx2, align 4, !dbg !1439
  store i32 %3, i32* %BB, align 4, !dbg !1441
  %4 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1442
  %counter3 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %4, i32 0, i32 1, !dbg !1443
  %arrayidx4 = getelementptr inbounds [8 x i32], [8 x i32]* %counter3, i64 0, i64 2, !dbg !1442
  %5 = load i32, i32* %arrayidx4, align 4, !dbg !1442
  store i32 %5, i32* %CC, align 4, !dbg !1444
  %6 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1445
  %counter5 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %6, i32 0, i32 1, !dbg !1446
  %arrayidx6 = getelementptr inbounds [8 x i32], [8 x i32]* %counter5, i64 0, i64 3, !dbg !1445
  %7 = load i32, i32* %arrayidx6, align 4, !dbg !1445
  store i32 %7, i32* %DD, align 4, !dbg !1447
  %8 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1448
  %counter7 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %8, i32 0, i32 1, !dbg !1449
  %arrayidx8 = getelementptr inbounds [8 x i32], [8 x i32]* %counter7, i64 0, i64 4, !dbg !1448
  %9 = load i32, i32* %arrayidx8, align 4, !dbg !1448
  store i32 %9, i32* %EE, align 4, !dbg !1450
  %10 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1451
  %counter9 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %10, i32 0, i32 1, !dbg !1452
  %arrayidx10 = getelementptr inbounds [8 x i32], [8 x i32]* %counter9, i64 0, i64 5, !dbg !1451
  %11 = load i32, i32* %arrayidx10, align 4, !dbg !1451
  store i32 %11, i32* %FF, align 4, !dbg !1453
  %12 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1454
  %counter11 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %12, i32 0, i32 1, !dbg !1455
  %arrayidx12 = getelementptr inbounds [8 x i32], [8 x i32]* %counter11, i64 0, i64 6, !dbg !1454
  %13 = load i32, i32* %arrayidx12, align 4, !dbg !1454
  store i32 %13, i32* %GG, align 4, !dbg !1456
  %14 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1457
  %counter13 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %14, i32 0, i32 1, !dbg !1458
  %arrayidx14 = getelementptr inbounds [8 x i32], [8 x i32]* %counter13, i64 0, i64 7, !dbg !1457
  %15 = load i32, i32* %arrayidx14, align 4, !dbg !1457
  store i32 %15, i32* %HH, align 4, !dbg !1459
  store i32 0, i32* %i, align 4, !dbg !1460
  br label %for.cond, !dbg !1462

for.cond:                                         ; preds = %for.inc, %entry
  %16 = load i32, i32* %i, align 4, !dbg !1463
  %cmp = icmp slt i32 %16, 16, !dbg !1465
  br i1 %cmp, label %for.body, label %for.end, !dbg !1466

for.body:                                         ; preds = %for.cond
  %17 = load i32*, i32** %in.addr, align 8, !dbg !1467
  %18 = load i32, i32* %i, align 4, !dbg !1468
  %idxprom = sext i32 %18 to i64, !dbg !1467
  %arrayidx15 = getelementptr inbounds i32, i32* %17, i64 %idxprom, !dbg !1467
  %19 = load i32, i32* %arrayidx15, align 4, !dbg !1467
  %20 = load i32, i32* %i, align 4, !dbg !1469
  %idxprom16 = sext i32 %20 to i64, !dbg !1470
  %arrayidx17 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom16, !dbg !1470
  store i32 %19, i32* %arrayidx17, align 4, !dbg !1471
  br label %for.inc, !dbg !1470

for.inc:                                          ; preds = %for.body
  %21 = load i32, i32* %i, align 4, !dbg !1472
  %inc = add nsw i32 %21, 1, !dbg !1472
  store i32 %inc, i32* %i, align 4, !dbg !1472
  br label %for.cond, !dbg !1473, !llvm.loop !1474

for.end:                                          ; preds = %for.cond
  store i32 16, i32* %i, align 4, !dbg !1476
  br label %for.cond18, !dbg !1478

for.cond18:                                       ; preds = %for.inc74, %for.end
  %22 = load i32, i32* %i, align 4, !dbg !1479
  %cmp19 = icmp slt i32 %22, 64, !dbg !1481
  br i1 %cmp19, label %for.body20, label %for.end76, !dbg !1482

for.body20:                                       ; preds = %for.cond18
  %23 = load i32, i32* %i, align 4, !dbg !1483
  %sub = sub nsw i32 %23, 2, !dbg !1483
  %idxprom21 = sext i32 %sub to i64, !dbg !1483
  %arrayidx22 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom21, !dbg !1483
  %24 = load i32, i32* %arrayidx22, align 4, !dbg !1483
  %shr = lshr i32 %24, 17, !dbg !1483
  %25 = load i32, i32* %i, align 4, !dbg !1483
  %sub23 = sub nsw i32 %25, 2, !dbg !1483
  %idxprom24 = sext i32 %sub23 to i64, !dbg !1483
  %arrayidx25 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom24, !dbg !1483
  %26 = load i32, i32* %arrayidx25, align 4, !dbg !1483
  %shl = shl i32 %26, 15, !dbg !1483
  %or = or i32 %shr, %shl, !dbg !1483
  %27 = load i32, i32* %i, align 4, !dbg !1483
  %sub26 = sub nsw i32 %27, 2, !dbg !1483
  %idxprom27 = sext i32 %sub26 to i64, !dbg !1483
  %arrayidx28 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom27, !dbg !1483
  %28 = load i32, i32* %arrayidx28, align 4, !dbg !1483
  %shr29 = lshr i32 %28, 19, !dbg !1483
  %29 = load i32, i32* %i, align 4, !dbg !1483
  %sub30 = sub nsw i32 %29, 2, !dbg !1483
  %idxprom31 = sext i32 %sub30 to i64, !dbg !1483
  %arrayidx32 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom31, !dbg !1483
  %30 = load i32, i32* %arrayidx32, align 4, !dbg !1483
  %shl33 = shl i32 %30, 13, !dbg !1483
  %or34 = or i32 %shr29, %shl33, !dbg !1483
  %xor = xor i32 %or, %or34, !dbg !1483
  %31 = load i32, i32* %i, align 4, !dbg !1483
  %sub35 = sub nsw i32 %31, 2, !dbg !1483
  %idxprom36 = sext i32 %sub35 to i64, !dbg !1483
  %arrayidx37 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom36, !dbg !1483
  %32 = load i32, i32* %arrayidx37, align 4, !dbg !1483
  %shr38 = lshr i32 %32, 10, !dbg !1483
  %xor39 = xor i32 %xor, %shr38, !dbg !1483
  %33 = load i32, i32* %i, align 4, !dbg !1484
  %sub40 = sub nsw i32 %33, 7, !dbg !1485
  %idxprom41 = sext i32 %sub40 to i64, !dbg !1486
  %arrayidx42 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom41, !dbg !1486
  %34 = load i32, i32* %arrayidx42, align 4, !dbg !1486
  %add = add i32 %xor39, %34, !dbg !1487
  %35 = load i32, i32* %i, align 4, !dbg !1488
  %sub43 = sub nsw i32 %35, 15, !dbg !1488
  %idxprom44 = sext i32 %sub43 to i64, !dbg !1488
  %arrayidx45 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom44, !dbg !1488
  %36 = load i32, i32* %arrayidx45, align 4, !dbg !1488
  %shr46 = lshr i32 %36, 7, !dbg !1488
  %37 = load i32, i32* %i, align 4, !dbg !1488
  %sub47 = sub nsw i32 %37, 15, !dbg !1488
  %idxprom48 = sext i32 %sub47 to i64, !dbg !1488
  %arrayidx49 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom48, !dbg !1488
  %38 = load i32, i32* %arrayidx49, align 4, !dbg !1488
  %shl50 = shl i32 %38, 25, !dbg !1488
  %or51 = or i32 %shr46, %shl50, !dbg !1488
  %39 = load i32, i32* %i, align 4, !dbg !1488
  %sub52 = sub nsw i32 %39, 15, !dbg !1488
  %idxprom53 = sext i32 %sub52 to i64, !dbg !1488
  %arrayidx54 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom53, !dbg !1488
  %40 = load i32, i32* %arrayidx54, align 4, !dbg !1488
  %shr55 = lshr i32 %40, 18, !dbg !1488
  %41 = load i32, i32* %i, align 4, !dbg !1488
  %sub56 = sub nsw i32 %41, 15, !dbg !1488
  %idxprom57 = sext i32 %sub56 to i64, !dbg !1488
  %arrayidx58 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom57, !dbg !1488
  %42 = load i32, i32* %arrayidx58, align 4, !dbg !1488
  %shl59 = shl i32 %42, 14, !dbg !1488
  %or60 = or i32 %shr55, %shl59, !dbg !1488
  %xor61 = xor i32 %or51, %or60, !dbg !1488
  %43 = load i32, i32* %i, align 4, !dbg !1488
  %sub62 = sub nsw i32 %43, 15, !dbg !1488
  %idxprom63 = sext i32 %sub62 to i64, !dbg !1488
  %arrayidx64 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom63, !dbg !1488
  %44 = load i32, i32* %arrayidx64, align 4, !dbg !1488
  %shr65 = lshr i32 %44, 3, !dbg !1488
  %xor66 = xor i32 %xor61, %shr65, !dbg !1488
  %add67 = add i32 %add, %xor66, !dbg !1489
  %45 = load i32, i32* %i, align 4, !dbg !1490
  %sub68 = sub nsw i32 %45, 16, !dbg !1491
  %idxprom69 = sext i32 %sub68 to i64, !dbg !1492
  %arrayidx70 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom69, !dbg !1492
  %46 = load i32, i32* %arrayidx70, align 4, !dbg !1492
  %add71 = add i32 %add67, %46, !dbg !1493
  %47 = load i32, i32* %i, align 4, !dbg !1494
  %idxprom72 = sext i32 %47 to i64, !dbg !1495
  %arrayidx73 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom72, !dbg !1495
  store i32 %add71, i32* %arrayidx73, align 4, !dbg !1496
  br label %for.inc74, !dbg !1495

for.inc74:                                        ; preds = %for.body20
  %48 = load i32, i32* %i, align 4, !dbg !1497
  %inc75 = add nsw i32 %48, 1, !dbg !1497
  store i32 %inc75, i32* %i, align 4, !dbg !1497
  br label %for.cond18, !dbg !1498, !llvm.loop !1499

for.end76:                                        ; preds = %for.cond18
  store i32 0, i32* %i, align 4, !dbg !1501
  br label %for.cond77, !dbg !1503

for.cond77:                                       ; preds = %for.inc120, %for.end76
  %49 = load i32, i32* %i, align 4, !dbg !1504
  %cmp78 = icmp slt i32 %49, 64, !dbg !1506
  br i1 %cmp78, label %for.body79, label %for.end122, !dbg !1507

for.body79:                                       ; preds = %for.cond77
  call void @llvm.dbg.declare(metadata i32* %T1, metadata !1508, metadata !DIExpression()), !dbg !1510
  call void @llvm.dbg.declare(metadata i32* %T2, metadata !1511, metadata !DIExpression()), !dbg !1512
  %50 = load i32, i32* %HH, align 4, !dbg !1513
  %51 = load i32, i32* %EE, align 4, !dbg !1514
  %shr80 = lshr i32 %51, 6, !dbg !1514
  %52 = load i32, i32* %EE, align 4, !dbg !1514
  %shl81 = shl i32 %52, 26, !dbg !1514
  %or82 = or i32 %shr80, %shl81, !dbg !1514
  %53 = load i32, i32* %EE, align 4, !dbg !1514
  %shr83 = lshr i32 %53, 11, !dbg !1514
  %54 = load i32, i32* %EE, align 4, !dbg !1514
  %shl84 = shl i32 %54, 21, !dbg !1514
  %or85 = or i32 %shr83, %shl84, !dbg !1514
  %xor86 = xor i32 %or82, %or85, !dbg !1514
  %55 = load i32, i32* %EE, align 4, !dbg !1514
  %shr87 = lshr i32 %55, 25, !dbg !1514
  %56 = load i32, i32* %EE, align 4, !dbg !1514
  %shl88 = shl i32 %56, 7, !dbg !1514
  %or89 = or i32 %shr87, %shl88, !dbg !1514
  %xor90 = xor i32 %xor86, %or89, !dbg !1514
  %add91 = add i32 %50, %xor90, !dbg !1515
  %57 = load i32, i32* %EE, align 4, !dbg !1516
  %58 = load i32, i32* %FF, align 4, !dbg !1516
  %and = and i32 %57, %58, !dbg !1516
  %59 = load i32, i32* %EE, align 4, !dbg !1516
  %neg = xor i32 %59, -1, !dbg !1516
  %60 = load i32, i32* %GG, align 4, !dbg !1516
  %and92 = and i32 %neg, %60, !dbg !1516
  %xor93 = xor i32 %and, %and92, !dbg !1516
  %add94 = add i32 %add91, %xor93, !dbg !1517
  %61 = load i32, i32* %i, align 4, !dbg !1518
  %idxprom95 = sext i32 %61 to i64, !dbg !1519
  %arrayidx96 = getelementptr inbounds [64 x i32], [64 x i32]* @mavlink_sha256_constant_256, i64 0, i64 %idxprom95, !dbg !1519
  %62 = load i32, i32* %arrayidx96, align 4, !dbg !1519
  %add97 = add i32 %add94, %62, !dbg !1520
  %63 = load i32, i32* %i, align 4, !dbg !1521
  %idxprom98 = sext i32 %63 to i64, !dbg !1522
  %arrayidx99 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom98, !dbg !1522
  %64 = load i32, i32* %arrayidx99, align 4, !dbg !1522
  %add100 = add i32 %add97, %64, !dbg !1523
  store i32 %add100, i32* %T1, align 4, !dbg !1524
  %65 = load i32, i32* %AA, align 4, !dbg !1525
  %shr101 = lshr i32 %65, 2, !dbg !1525
  %66 = load i32, i32* %AA, align 4, !dbg !1525
  %shl102 = shl i32 %66, 30, !dbg !1525
  %or103 = or i32 %shr101, %shl102, !dbg !1525
  %67 = load i32, i32* %AA, align 4, !dbg !1525
  %shr104 = lshr i32 %67, 13, !dbg !1525
  %68 = load i32, i32* %AA, align 4, !dbg !1525
  %shl105 = shl i32 %68, 19, !dbg !1525
  %or106 = or i32 %shr104, %shl105, !dbg !1525
  %xor107 = xor i32 %or103, %or106, !dbg !1525
  %69 = load i32, i32* %AA, align 4, !dbg !1525
  %shr108 = lshr i32 %69, 22, !dbg !1525
  %70 = load i32, i32* %AA, align 4, !dbg !1525
  %shl109 = shl i32 %70, 10, !dbg !1525
  %or110 = or i32 %shr108, %shl109, !dbg !1525
  %xor111 = xor i32 %xor107, %or110, !dbg !1525
  %71 = load i32, i32* %AA, align 4, !dbg !1526
  %72 = load i32, i32* %BB, align 4, !dbg !1526
  %and112 = and i32 %71, %72, !dbg !1526
  %73 = load i32, i32* %AA, align 4, !dbg !1526
  %74 = load i32, i32* %CC, align 4, !dbg !1526
  %and113 = and i32 %73, %74, !dbg !1526
  %xor114 = xor i32 %and112, %and113, !dbg !1526
  %75 = load i32, i32* %BB, align 4, !dbg !1526
  %76 = load i32, i32* %CC, align 4, !dbg !1526
  %and115 = and i32 %75, %76, !dbg !1526
  %xor116 = xor i32 %xor114, %and115, !dbg !1526
  %add117 = add i32 %xor111, %xor116, !dbg !1527
  store i32 %add117, i32* %T2, align 4, !dbg !1528
  %77 = load i32, i32* %GG, align 4, !dbg !1529
  store i32 %77, i32* %HH, align 4, !dbg !1530
  %78 = load i32, i32* %FF, align 4, !dbg !1531
  store i32 %78, i32* %GG, align 4, !dbg !1532
  %79 = load i32, i32* %EE, align 4, !dbg !1533
  store i32 %79, i32* %FF, align 4, !dbg !1534
  %80 = load i32, i32* %DD, align 4, !dbg !1535
  %81 = load i32, i32* %T1, align 4, !dbg !1536
  %add118 = add i32 %80, %81, !dbg !1537
  store i32 %add118, i32* %EE, align 4, !dbg !1538
  %82 = load i32, i32* %CC, align 4, !dbg !1539
  store i32 %82, i32* %DD, align 4, !dbg !1540
  %83 = load i32, i32* %BB, align 4, !dbg !1541
  store i32 %83, i32* %CC, align 4, !dbg !1542
  %84 = load i32, i32* %AA, align 4, !dbg !1543
  store i32 %84, i32* %BB, align 4, !dbg !1544
  %85 = load i32, i32* %T1, align 4, !dbg !1545
  %86 = load i32, i32* %T2, align 4, !dbg !1546
  %add119 = add i32 %85, %86, !dbg !1547
  store i32 %add119, i32* %AA, align 4, !dbg !1548
  br label %for.inc120, !dbg !1549

for.inc120:                                       ; preds = %for.body79
  %87 = load i32, i32* %i, align 4, !dbg !1550
  %inc121 = add nsw i32 %87, 1, !dbg !1550
  store i32 %inc121, i32* %i, align 4, !dbg !1550
  br label %for.cond77, !dbg !1551, !llvm.loop !1552

for.end122:                                       ; preds = %for.cond77
  %88 = load i32, i32* %AA, align 4, !dbg !1554
  %89 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1555
  %counter123 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %89, i32 0, i32 1, !dbg !1556
  %arrayidx124 = getelementptr inbounds [8 x i32], [8 x i32]* %counter123, i64 0, i64 0, !dbg !1555
  %90 = load i32, i32* %arrayidx124, align 4, !dbg !1557
  %add125 = add i32 %90, %88, !dbg !1557
  store i32 %add125, i32* %arrayidx124, align 4, !dbg !1557
  %91 = load i32, i32* %BB, align 4, !dbg !1558
  %92 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1559
  %counter126 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %92, i32 0, i32 1, !dbg !1560
  %arrayidx127 = getelementptr inbounds [8 x i32], [8 x i32]* %counter126, i64 0, i64 1, !dbg !1559
  %93 = load i32, i32* %arrayidx127, align 4, !dbg !1561
  %add128 = add i32 %93, %91, !dbg !1561
  store i32 %add128, i32* %arrayidx127, align 4, !dbg !1561
  %94 = load i32, i32* %CC, align 4, !dbg !1562
  %95 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1563
  %counter129 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %95, i32 0, i32 1, !dbg !1564
  %arrayidx130 = getelementptr inbounds [8 x i32], [8 x i32]* %counter129, i64 0, i64 2, !dbg !1563
  %96 = load i32, i32* %arrayidx130, align 4, !dbg !1565
  %add131 = add i32 %96, %94, !dbg !1565
  store i32 %add131, i32* %arrayidx130, align 4, !dbg !1565
  %97 = load i32, i32* %DD, align 4, !dbg !1566
  %98 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1567
  %counter132 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %98, i32 0, i32 1, !dbg !1568
  %arrayidx133 = getelementptr inbounds [8 x i32], [8 x i32]* %counter132, i64 0, i64 3, !dbg !1567
  %99 = load i32, i32* %arrayidx133, align 4, !dbg !1569
  %add134 = add i32 %99, %97, !dbg !1569
  store i32 %add134, i32* %arrayidx133, align 4, !dbg !1569
  %100 = load i32, i32* %EE, align 4, !dbg !1570
  %101 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1571
  %counter135 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %101, i32 0, i32 1, !dbg !1572
  %arrayidx136 = getelementptr inbounds [8 x i32], [8 x i32]* %counter135, i64 0, i64 4, !dbg !1571
  %102 = load i32, i32* %arrayidx136, align 4, !dbg !1573
  %add137 = add i32 %102, %100, !dbg !1573
  store i32 %add137, i32* %arrayidx136, align 4, !dbg !1573
  %103 = load i32, i32* %FF, align 4, !dbg !1574
  %104 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1575
  %counter138 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %104, i32 0, i32 1, !dbg !1576
  %arrayidx139 = getelementptr inbounds [8 x i32], [8 x i32]* %counter138, i64 0, i64 5, !dbg !1575
  %105 = load i32, i32* %arrayidx139, align 4, !dbg !1577
  %add140 = add i32 %105, %103, !dbg !1577
  store i32 %add140, i32* %arrayidx139, align 4, !dbg !1577
  %106 = load i32, i32* %GG, align 4, !dbg !1578
  %107 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1579
  %counter141 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %107, i32 0, i32 1, !dbg !1580
  %arrayidx142 = getelementptr inbounds [8 x i32], [8 x i32]* %counter141, i64 0, i64 6, !dbg !1579
  %108 = load i32, i32* %arrayidx142, align 4, !dbg !1581
  %add143 = add i32 %108, %106, !dbg !1581
  store i32 %add143, i32* %arrayidx142, align 4, !dbg !1581
  %109 = load i32, i32* %HH, align 4, !dbg !1582
  %110 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1583
  %counter144 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %110, i32 0, i32 1, !dbg !1584
  %arrayidx145 = getelementptr inbounds [8 x i32], [8 x i32]* %counter144, i64 0, i64 7, !dbg !1583
  %111 = load i32, i32* %arrayidx145, align 4, !dbg !1585
  %add146 = add i32 %111, %109, !dbg !1585
  store i32 %add146, i32* %arrayidx145, align 4, !dbg !1585
  ret void, !dbg !1586
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind willreturn }
attributes #5 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nounwind }
attributes #7 = { nounwind readnone }

!llvm.dbg.cu = !{!92}
!llvm.module.flags = !{!272, !273, !274}
!llvm.ident = !{!275}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "m_mavlink_status", scope: !2, file: !3, line: 29, type: !271, isLocal: true, isDefinition: true)
!2 = distinct !DISubprogram(name: "mavlink_get_channel_status", scope: !3, file: !3, line: 23, type: !4, scopeLine: 24, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!3 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../mavlink_helpers.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-client/input/source_code/mavlink")
!4 = !DISubroutineType(types: !5)
!5 = !{!6, !12}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64)
!7 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_status_t", file: !8, line: 233, baseType: !9)
!8 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../mavlink_types.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-client/input/source_code/mavlink")
!9 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_status", file: !8, line: 219, size: 320, elements: !10)
!10 = !{!11, !17, !18, !19, !40, !41, !42, !43, !47, !48, !49, !50, !74}
!11 = !DIDerivedType(tag: DW_TAG_member, name: "msg_received", scope: !9, file: !8, line: 220, baseType: !12, size: 8)
!12 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint8_t", file: !13, line: 24, baseType: !14)
!13 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/stdint-uintn.h", directory: "")
!14 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint8_t", file: !15, line: 38, baseType: !16)
!15 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types.h", directory: "")
!16 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!17 = !DIDerivedType(tag: DW_TAG_member, name: "buffer_overrun", scope: !9, file: !8, line: 221, baseType: !12, size: 8, offset: 8)
!18 = !DIDerivedType(tag: DW_TAG_member, name: "parse_error", scope: !9, file: !8, line: 222, baseType: !12, size: 8, offset: 16)
!19 = !DIDerivedType(tag: DW_TAG_member, name: "parse_state", scope: !9, file: !8, line: 223, baseType: !20, size: 32, offset: 32)
!20 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_parse_state_t", file: !8, line: 203, baseType: !21)
!21 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !8, line: 186, baseType: !22, size: 32, elements: !23)
!22 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!23 = !{!24, !25, !26, !27, !28, !29, !30, !31, !32, !33, !34, !35, !36, !37, !38, !39}
!24 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_UNINIT", value: 0, isUnsigned: true)
!25 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_IDLE", value: 1, isUnsigned: true)
!26 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_STX", value: 2, isUnsigned: true)
!27 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_LENGTH", value: 3, isUnsigned: true)
!28 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_INCOMPAT_FLAGS", value: 4, isUnsigned: true)
!29 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_COMPAT_FLAGS", value: 5, isUnsigned: true)
!30 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_SEQ", value: 6, isUnsigned: true)
!31 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_SYSID", value: 7, isUnsigned: true)
!32 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_COMPID", value: 8, isUnsigned: true)
!33 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_MSGID1", value: 9, isUnsigned: true)
!34 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_MSGID2", value: 10, isUnsigned: true)
!35 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_MSGID3", value: 11, isUnsigned: true)
!36 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_PAYLOAD", value: 12, isUnsigned: true)
!37 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_CRC1", value: 13, isUnsigned: true)
!38 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_BAD_CRC1", value: 14, isUnsigned: true)
!39 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_SIGNATURE_WAIT", value: 15, isUnsigned: true)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "packet_idx", scope: !9, file: !8, line: 224, baseType: !12, size: 8, offset: 64)
!41 = !DIDerivedType(tag: DW_TAG_member, name: "current_rx_seq", scope: !9, file: !8, line: 225, baseType: !12, size: 8, offset: 72)
!42 = !DIDerivedType(tag: DW_TAG_member, name: "current_tx_seq", scope: !9, file: !8, line: 226, baseType: !12, size: 8, offset: 80)
!43 = !DIDerivedType(tag: DW_TAG_member, name: "packet_rx_success_count", scope: !9, file: !8, line: 227, baseType: !44, size: 16, offset: 96)
!44 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint16_t", file: !13, line: 25, baseType: !45)
!45 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint16_t", file: !15, line: 40, baseType: !46)
!46 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!47 = !DIDerivedType(tag: DW_TAG_member, name: "packet_rx_drop_count", scope: !9, file: !8, line: 228, baseType: !44, size: 16, offset: 112)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "flags", scope: !9, file: !8, line: 229, baseType: !12, size: 8, offset: 128)
!49 = !DIDerivedType(tag: DW_TAG_member, name: "signature_wait", scope: !9, file: !8, line: 230, baseType: !12, size: 8, offset: 136)
!50 = !DIDerivedType(tag: DW_TAG_member, name: "signing", scope: !9, file: !8, line: 231, baseType: !51, size: 64, offset: 192)
!51 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !52, size: 64)
!52 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_signing", file: !8, line: 248, size: 448, elements: !53)
!53 = !{!54, !55, !56, !60, !64}
!54 = !DIDerivedType(tag: DW_TAG_member, name: "flags", scope: !52, file: !8, line: 249, baseType: !12, size: 8)
!55 = !DIDerivedType(tag: DW_TAG_member, name: "link_id", scope: !52, file: !8, line: 250, baseType: !12, size: 8, offset: 8)
!56 = !DIDerivedType(tag: DW_TAG_member, name: "timestamp", scope: !52, file: !8, line: 251, baseType: !57, size: 64, offset: 64)
!57 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint64_t", file: !13, line: 27, baseType: !58)
!58 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint64_t", file: !15, line: 45, baseType: !59)
!59 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!60 = !DIDerivedType(tag: DW_TAG_member, name: "secret_key", scope: !52, file: !8, line: 252, baseType: !61, size: 256, offset: 128)
!61 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, size: 256, elements: !62)
!62 = !{!63}
!63 = !DISubrange(count: 32)
!64 = !DIDerivedType(tag: DW_TAG_member, name: "accept_unsigned_callback", scope: !52, file: !8, line: 253, baseType: !65, size: 64, offset: 384)
!65 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_accept_unsigned_t", file: !8, line: 238, baseType: !66)
!66 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !67, size: 64)
!67 = !DISubroutineType(types: !68)
!68 = !{!69, !70, !72}
!69 = !DIBasicType(name: "_Bool", size: 8, encoding: DW_ATE_boolean)
!70 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !71, size: 64)
!71 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !7)
!72 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint32_t", file: !13, line: 26, baseType: !73)
!73 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint32_t", file: !15, line: 42, baseType: !22)
!74 = !DIDerivedType(tag: DW_TAG_member, name: "signing_streams", scope: !9, file: !8, line: 232, baseType: !75, size: 64, offset: 256)
!75 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !76, size: 64)
!76 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_signing_streams", file: !8, line: 263, size: 1168, elements: !77)
!77 = !{!78, !79}
!78 = !DIDerivedType(tag: DW_TAG_member, name: "num_signing_streams", scope: !76, file: !8, line: 264, baseType: !44, size: 16)
!79 = !DIDerivedType(tag: DW_TAG_member, name: "stream", scope: !76, file: !8, line: 270, baseType: !80, size: 1152, offset: 16)
!80 = !DICompositeType(tag: DW_TAG_array_type, baseType: !81, size: 1152, elements: !90)
!81 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_signing_stream", file: !8, line: 265, size: 72, elements: !82)
!82 = !{!83, !84, !85, !86}
!83 = !DIDerivedType(tag: DW_TAG_member, name: "link_id", scope: !81, file: !8, line: 266, baseType: !12, size: 8)
!84 = !DIDerivedType(tag: DW_TAG_member, name: "sysid", scope: !81, file: !8, line: 267, baseType: !12, size: 8, offset: 8)
!85 = !DIDerivedType(tag: DW_TAG_member, name: "compid", scope: !81, file: !8, line: 268, baseType: !12, size: 8, offset: 16)
!86 = !DIDerivedType(tag: DW_TAG_member, name: "timestamp_bytes", scope: !81, file: !8, line: 269, baseType: !87, size: 48, offset: 24)
!87 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, size: 48, elements: !88)
!88 = !{!89}
!89 = !DISubrange(count: 6)
!90 = !{!91}
!91 = !DISubrange(count: 16)
!92 = distinct !DICompileUnit(language: DW_LANG_C99, file: !93, producer: "clang version 10.0.0-4ubuntu1 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !94, retainedTypes: !240, globals: !262, splitDebugInlining: false, nameTableKind: None)
!93 = !DIFile(filename: "mavlink_client.c", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-client/input/source_code/mavlink")
!94 = !{!95, !142, !166, !181, !193, !205, !234, !21}
!95 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "MAV_TYPE", file: !96, line: 69, baseType: !22, size: 32, elements: !97)
!96 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../minimal/minimal.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-client/input/source_code/mavlink")
!97 = !{!98, !99, !100, !101, !102, !103, !104, !105, !106, !107, !108, !109, !110, !111, !112, !113, !114, !115, !116, !117, !118, !119, !120, !121, !122, !123, !124, !125, !126, !127, !128, !129, !130, !131, !132, !133, !134, !135, !136, !137, !138, !139, !140, !141}
!98 = !DIEnumerator(name: "MAV_TYPE_GENERIC", value: 0, isUnsigned: true)
!99 = !DIEnumerator(name: "MAV_TYPE_FIXED_WING", value: 1, isUnsigned: true)
!100 = !DIEnumerator(name: "MAV_TYPE_QUADROTOR", value: 2, isUnsigned: true)
!101 = !DIEnumerator(name: "MAV_TYPE_COAXIAL", value: 3, isUnsigned: true)
!102 = !DIEnumerator(name: "MAV_TYPE_HELICOPTER", value: 4, isUnsigned: true)
!103 = !DIEnumerator(name: "MAV_TYPE_ANTENNA_TRACKER", value: 5, isUnsigned: true)
!104 = !DIEnumerator(name: "MAV_TYPE_GCS", value: 6, isUnsigned: true)
!105 = !DIEnumerator(name: "MAV_TYPE_AIRSHIP", value: 7, isUnsigned: true)
!106 = !DIEnumerator(name: "MAV_TYPE_FREE_BALLOON", value: 8, isUnsigned: true)
!107 = !DIEnumerator(name: "MAV_TYPE_ROCKET", value: 9, isUnsigned: true)
!108 = !DIEnumerator(name: "MAV_TYPE_GROUND_ROVER", value: 10, isUnsigned: true)
!109 = !DIEnumerator(name: "MAV_TYPE_SURFACE_BOAT", value: 11, isUnsigned: true)
!110 = !DIEnumerator(name: "MAV_TYPE_SUBMARINE", value: 12, isUnsigned: true)
!111 = !DIEnumerator(name: "MAV_TYPE_HEXAROTOR", value: 13, isUnsigned: true)
!112 = !DIEnumerator(name: "MAV_TYPE_OCTOROTOR", value: 14, isUnsigned: true)
!113 = !DIEnumerator(name: "MAV_TYPE_TRICOPTER", value: 15, isUnsigned: true)
!114 = !DIEnumerator(name: "MAV_TYPE_FLAPPING_WING", value: 16, isUnsigned: true)
!115 = !DIEnumerator(name: "MAV_TYPE_KITE", value: 17, isUnsigned: true)
!116 = !DIEnumerator(name: "MAV_TYPE_ONBOARD_CONTROLLER", value: 18, isUnsigned: true)
!117 = !DIEnumerator(name: "MAV_TYPE_VTOL_DUOROTOR", value: 19, isUnsigned: true)
!118 = !DIEnumerator(name: "MAV_TYPE_VTOL_QUADROTOR", value: 20, isUnsigned: true)
!119 = !DIEnumerator(name: "MAV_TYPE_VTOL_TILTROTOR", value: 21, isUnsigned: true)
!120 = !DIEnumerator(name: "MAV_TYPE_VTOL_RESERVED2", value: 22, isUnsigned: true)
!121 = !DIEnumerator(name: "MAV_TYPE_VTOL_RESERVED3", value: 23, isUnsigned: true)
!122 = !DIEnumerator(name: "MAV_TYPE_VTOL_RESERVED4", value: 24, isUnsigned: true)
!123 = !DIEnumerator(name: "MAV_TYPE_VTOL_RESERVED5", value: 25, isUnsigned: true)
!124 = !DIEnumerator(name: "MAV_TYPE_GIMBAL", value: 26, isUnsigned: true)
!125 = !DIEnumerator(name: "MAV_TYPE_ADSB", value: 27, isUnsigned: true)
!126 = !DIEnumerator(name: "MAV_TYPE_PARAFOIL", value: 28, isUnsigned: true)
!127 = !DIEnumerator(name: "MAV_TYPE_DODECAROTOR", value: 29, isUnsigned: true)
!128 = !DIEnumerator(name: "MAV_TYPE_CAMERA", value: 30, isUnsigned: true)
!129 = !DIEnumerator(name: "MAV_TYPE_CHARGING_STATION", value: 31, isUnsigned: true)
!130 = !DIEnumerator(name: "MAV_TYPE_FLARM", value: 32, isUnsigned: true)
!131 = !DIEnumerator(name: "MAV_TYPE_SERVO", value: 33, isUnsigned: true)
!132 = !DIEnumerator(name: "MAV_TYPE_ODID", value: 34, isUnsigned: true)
!133 = !DIEnumerator(name: "MAV_TYPE_DECAROTOR", value: 35, isUnsigned: true)
!134 = !DIEnumerator(name: "MAV_TYPE_BATTERY", value: 36, isUnsigned: true)
!135 = !DIEnumerator(name: "MAV_TYPE_PARACHUTE", value: 37, isUnsigned: true)
!136 = !DIEnumerator(name: "MAV_TYPE_LOG", value: 38, isUnsigned: true)
!137 = !DIEnumerator(name: "MAV_TYPE_OSD", value: 39, isUnsigned: true)
!138 = !DIEnumerator(name: "MAV_TYPE_IMU", value: 40, isUnsigned: true)
!139 = !DIEnumerator(name: "MAV_TYPE_GPS", value: 41, isUnsigned: true)
!140 = !DIEnumerator(name: "MAV_TYPE_WINCH", value: 42, isUnsigned: true)
!141 = !DIEnumerator(name: "MAV_TYPE_ENUM_END", value: 43, isUnsigned: true)
!142 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "MAV_AUTOPILOT", file: !96, line: 39, baseType: !22, size: 32, elements: !143)
!143 = !{!144, !145, !146, !147, !148, !149, !150, !151, !152, !153, !154, !155, !156, !157, !158, !159, !160, !161, !162, !163, !164, !165}
!144 = !DIEnumerator(name: "MAV_AUTOPILOT_GENERIC", value: 0, isUnsigned: true)
!145 = !DIEnumerator(name: "MAV_AUTOPILOT_RESERVED", value: 1, isUnsigned: true)
!146 = !DIEnumerator(name: "MAV_AUTOPILOT_SLUGS", value: 2, isUnsigned: true)
!147 = !DIEnumerator(name: "MAV_AUTOPILOT_ARDUPILOTMEGA", value: 3, isUnsigned: true)
!148 = !DIEnumerator(name: "MAV_AUTOPILOT_OPENPILOT", value: 4, isUnsigned: true)
!149 = !DIEnumerator(name: "MAV_AUTOPILOT_GENERIC_WAYPOINTS_ONLY", value: 5, isUnsigned: true)
!150 = !DIEnumerator(name: "MAV_AUTOPILOT_GENERIC_WAYPOINTS_AND_SIMPLE_NAVIGATION_ONLY", value: 6, isUnsigned: true)
!151 = !DIEnumerator(name: "MAV_AUTOPILOT_GENERIC_MISSION_FULL", value: 7, isUnsigned: true)
!152 = !DIEnumerator(name: "MAV_AUTOPILOT_INVALID", value: 8, isUnsigned: true)
!153 = !DIEnumerator(name: "MAV_AUTOPILOT_PPZ", value: 9, isUnsigned: true)
!154 = !DIEnumerator(name: "MAV_AUTOPILOT_UDB", value: 10, isUnsigned: true)
!155 = !DIEnumerator(name: "MAV_AUTOPILOT_FP", value: 11, isUnsigned: true)
!156 = !DIEnumerator(name: "MAV_AUTOPILOT_PX4", value: 12, isUnsigned: true)
!157 = !DIEnumerator(name: "MAV_AUTOPILOT_SMACCMPILOT", value: 13, isUnsigned: true)
!158 = !DIEnumerator(name: "MAV_AUTOPILOT_AUTOQUAD", value: 14, isUnsigned: true)
!159 = !DIEnumerator(name: "MAV_AUTOPILOT_ARMAZILA", value: 15, isUnsigned: true)
!160 = !DIEnumerator(name: "MAV_AUTOPILOT_AEROB", value: 16, isUnsigned: true)
!161 = !DIEnumerator(name: "MAV_AUTOPILOT_ASLUAV", value: 17, isUnsigned: true)
!162 = !DIEnumerator(name: "MAV_AUTOPILOT_SMARTAP", value: 18, isUnsigned: true)
!163 = !DIEnumerator(name: "MAV_AUTOPILOT_AIRRAILS", value: 19, isUnsigned: true)
!164 = !DIEnumerator(name: "MAV_AUTOPILOT_REFLEX", value: 20, isUnsigned: true)
!165 = !DIEnumerator(name: "MAV_AUTOPILOT_ENUM_END", value: 21, isUnsigned: true)
!166 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "MAV_MODE", file: !167, line: 90, baseType: !22, size: 32, elements: !168)
!167 = !DIFile(filename: "generated/include/mavlink/v2.0/common/common.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-client/input/source_code/mavlink")
!168 = !{!169, !170, !171, !172, !173, !174, !175, !176, !177, !178, !179, !180}
!169 = !DIEnumerator(name: "MAV_MODE_PREFLIGHT", value: 0, isUnsigned: true)
!170 = !DIEnumerator(name: "MAV_MODE_MANUAL_DISARMED", value: 64, isUnsigned: true)
!171 = !DIEnumerator(name: "MAV_MODE_TEST_DISARMED", value: 66, isUnsigned: true)
!172 = !DIEnumerator(name: "MAV_MODE_STABILIZE_DISARMED", value: 80, isUnsigned: true)
!173 = !DIEnumerator(name: "MAV_MODE_GUIDED_DISARMED", value: 88, isUnsigned: true)
!174 = !DIEnumerator(name: "MAV_MODE_AUTO_DISARMED", value: 92, isUnsigned: true)
!175 = !DIEnumerator(name: "MAV_MODE_MANUAL_ARMED", value: 192, isUnsigned: true)
!176 = !DIEnumerator(name: "MAV_MODE_TEST_ARMED", value: 194, isUnsigned: true)
!177 = !DIEnumerator(name: "MAV_MODE_STABILIZE_ARMED", value: 208, isUnsigned: true)
!178 = !DIEnumerator(name: "MAV_MODE_GUIDED_ARMED", value: 216, isUnsigned: true)
!179 = !DIEnumerator(name: "MAV_MODE_AUTO_ARMED", value: 220, isUnsigned: true)
!180 = !DIEnumerator(name: "MAV_MODE_ENUM_END", value: 221, isUnsigned: true)
!181 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "MAV_STATE", file: !96, line: 155, baseType: !22, size: 32, elements: !182)
!182 = !{!183, !184, !185, !186, !187, !188, !189, !190, !191, !192}
!183 = !DIEnumerator(name: "MAV_STATE_UNINIT", value: 0, isUnsigned: true)
!184 = !DIEnumerator(name: "MAV_STATE_BOOT", value: 1, isUnsigned: true)
!185 = !DIEnumerator(name: "MAV_STATE_CALIBRATING", value: 2, isUnsigned: true)
!186 = !DIEnumerator(name: "MAV_STATE_STANDBY", value: 3, isUnsigned: true)
!187 = !DIEnumerator(name: "MAV_STATE_ACTIVE", value: 4, isUnsigned: true)
!188 = !DIEnumerator(name: "MAV_STATE_CRITICAL", value: 5, isUnsigned: true)
!189 = !DIEnumerator(name: "MAV_STATE_EMERGENCY", value: 6, isUnsigned: true)
!190 = !DIEnumerator(name: "MAV_STATE_POWEROFF", value: 7, isUnsigned: true)
!191 = !DIEnumerator(name: "MAV_STATE_FLIGHT_TERMINATION", value: 8, isUnsigned: true)
!192 = !DIEnumerator(name: "MAV_STATE_ENUM_END", value: 9, isUnsigned: true)
!193 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "__socket_type", file: !194, line: 24, baseType: !22, size: 32, elements: !195)
!194 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/socket_type.h", directory: "")
!195 = !{!196, !197, !198, !199, !200, !201, !202, !203, !204}
!196 = !DIEnumerator(name: "SOCK_STREAM", value: 1, isUnsigned: true)
!197 = !DIEnumerator(name: "SOCK_DGRAM", value: 2, isUnsigned: true)
!198 = !DIEnumerator(name: "SOCK_RAW", value: 3, isUnsigned: true)
!199 = !DIEnumerator(name: "SOCK_RDM", value: 4, isUnsigned: true)
!200 = !DIEnumerator(name: "SOCK_SEQPACKET", value: 5, isUnsigned: true)
!201 = !DIEnumerator(name: "SOCK_DCCP", value: 6, isUnsigned: true)
!202 = !DIEnumerator(name: "SOCK_PACKET", value: 10, isUnsigned: true)
!203 = !DIEnumerator(name: "SOCK_CLOEXEC", value: 524288, isUnsigned: true)
!204 = !DIEnumerator(name: "SOCK_NONBLOCK", value: 2048, isUnsigned: true)
!205 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !206, line: 40, baseType: !22, size: 32, elements: !207)
!206 = !DIFile(filename: "/usr/include/netinet/in.h", directory: "")
!207 = !{!208, !209, !210, !211, !212, !213, !214, !215, !216, !217, !218, !219, !220, !221, !222, !223, !224, !225, !226, !227, !228, !229, !230, !231, !232, !233}
!208 = !DIEnumerator(name: "IPPROTO_IP", value: 0, isUnsigned: true)
!209 = !DIEnumerator(name: "IPPROTO_ICMP", value: 1, isUnsigned: true)
!210 = !DIEnumerator(name: "IPPROTO_IGMP", value: 2, isUnsigned: true)
!211 = !DIEnumerator(name: "IPPROTO_IPIP", value: 4, isUnsigned: true)
!212 = !DIEnumerator(name: "IPPROTO_TCP", value: 6, isUnsigned: true)
!213 = !DIEnumerator(name: "IPPROTO_EGP", value: 8, isUnsigned: true)
!214 = !DIEnumerator(name: "IPPROTO_PUP", value: 12, isUnsigned: true)
!215 = !DIEnumerator(name: "IPPROTO_UDP", value: 17, isUnsigned: true)
!216 = !DIEnumerator(name: "IPPROTO_IDP", value: 22, isUnsigned: true)
!217 = !DIEnumerator(name: "IPPROTO_TP", value: 29, isUnsigned: true)
!218 = !DIEnumerator(name: "IPPROTO_DCCP", value: 33, isUnsigned: true)
!219 = !DIEnumerator(name: "IPPROTO_IPV6", value: 41, isUnsigned: true)
!220 = !DIEnumerator(name: "IPPROTO_RSVP", value: 46, isUnsigned: true)
!221 = !DIEnumerator(name: "IPPROTO_GRE", value: 47, isUnsigned: true)
!222 = !DIEnumerator(name: "IPPROTO_ESP", value: 50, isUnsigned: true)
!223 = !DIEnumerator(name: "IPPROTO_AH", value: 51, isUnsigned: true)
!224 = !DIEnumerator(name: "IPPROTO_MTP", value: 92, isUnsigned: true)
!225 = !DIEnumerator(name: "IPPROTO_BEETPH", value: 94, isUnsigned: true)
!226 = !DIEnumerator(name: "IPPROTO_ENCAP", value: 98, isUnsigned: true)
!227 = !DIEnumerator(name: "IPPROTO_PIM", value: 103, isUnsigned: true)
!228 = !DIEnumerator(name: "IPPROTO_COMP", value: 108, isUnsigned: true)
!229 = !DIEnumerator(name: "IPPROTO_SCTP", value: 132, isUnsigned: true)
!230 = !DIEnumerator(name: "IPPROTO_UDPLITE", value: 136, isUnsigned: true)
!231 = !DIEnumerator(name: "IPPROTO_MPLS", value: 137, isUnsigned: true)
!232 = !DIEnumerator(name: "IPPROTO_RAW", value: 255, isUnsigned: true)
!233 = !DIEnumerator(name: "IPPROTO_MAX", value: 256, isUnsigned: true)
!234 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !8, line: 165, baseType: !22, size: 32, elements: !235)
!235 = !{!236, !237, !238, !239}
!236 = !DIEnumerator(name: "MAVLINK_COMM_0", value: 0, isUnsigned: true)
!237 = !DIEnumerator(name: "MAVLINK_COMM_1", value: 1, isUnsigned: true)
!238 = !DIEnumerator(name: "MAVLINK_COMM_2", value: 2, isUnsigned: true)
!239 = !DIEnumerator(name: "MAVLINK_COMM_3", value: 3, isUnsigned: true)
!240 = !{!241, !253, !254, !12, !256, !44, !257, !259, !260}
!241 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !242, size: 64)
!242 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sockaddr", file: !243, line: 178, size: 128, elements: !244)
!243 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/socket.h", directory: "")
!244 = !{!245, !248}
!245 = !DIDerivedType(tag: DW_TAG_member, name: "sa_family", scope: !242, file: !243, line: 180, baseType: !246, size: 16)
!246 = !DIDerivedType(tag: DW_TAG_typedef, name: "sa_family_t", file: !247, line: 28, baseType: !46)
!247 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/sockaddr.h", directory: "")
!248 = !DIDerivedType(tag: DW_TAG_member, name: "sa_data", scope: !242, file: !243, line: 181, baseType: !249, size: 112, offset: 16)
!249 = !DICompositeType(tag: DW_TAG_array_type, baseType: !250, size: 112, elements: !251)
!250 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!251 = !{!252}
!252 = !DISubrange(count: 14)
!253 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !250, size: 64)
!254 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !255, size: 64)
!255 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !250)
!256 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !12, size: 64)
!257 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !258, size: 64)
!258 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !12)
!259 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!260 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !261, size: 64)
!261 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !16)
!262 = !{!0, !263}
!263 = !DIGlobalVariableExpression(var: !264, expr: !DIExpression())
!264 = distinct !DIGlobalVariable(name: "mavlink_sha256_constant_256", scope: !92, file: !265, line: 72, type: !266, isLocal: true, isDefinition: true)
!265 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../mavlink_sha256.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-client/input/source_code/mavlink")
!266 = !DICompositeType(tag: DW_TAG_array_type, baseType: !267, size: 2048, elements: !268)
!267 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !72)
!268 = !{!269}
!269 = !DISubrange(count: 64)
!270 = !{}
!271 = !DICompositeType(tag: DW_TAG_array_type, baseType: !7, size: 5120, elements: !90)
!272 = !{i32 7, !"Dwarf Version", i32 4}
!273 = !{i32 2, !"Debug Info Version", i32 3}
!274 = !{i32 1, !"wchar_size", i32 4}
!275 = !{!"clang version 10.0.0-4ubuntu1 "}
!276 = distinct !DISubprogram(name: "send_heartbeat", scope: !93, file: !93, line: 20, type: !277, scopeLine: 20, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !92, retainedNodes: !270)
!277 = !DISubroutineType(types: !278)
!278 = !{null, !279, !280}
!279 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!280 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !281, size: 64)
!281 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sockaddr_in", file: !206, line: 238, size: 128, elements: !282)
!282 = !{!283, !284, !286, !291}
!283 = !DIDerivedType(tag: DW_TAG_member, name: "sin_family", scope: !281, file: !206, line: 240, baseType: !246, size: 16)
!284 = !DIDerivedType(tag: DW_TAG_member, name: "sin_port", scope: !281, file: !206, line: 241, baseType: !285, size: 16, offset: 16)
!285 = !DIDerivedType(tag: DW_TAG_typedef, name: "in_port_t", file: !206, line: 119, baseType: !44)
!286 = !DIDerivedType(tag: DW_TAG_member, name: "sin_addr", scope: !281, file: !206, line: 242, baseType: !287, size: 32, offset: 32)
!287 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "in_addr", file: !206, line: 31, size: 32, elements: !288)
!288 = !{!289}
!289 = !DIDerivedType(tag: DW_TAG_member, name: "s_addr", scope: !287, file: !206, line: 33, baseType: !290, size: 32)
!290 = !DIDerivedType(tag: DW_TAG_typedef, name: "in_addr_t", file: !206, line: 30, baseType: !72)
!291 = !DIDerivedType(tag: DW_TAG_member, name: "sin_zero", scope: !281, file: !206, line: 245, baseType: !292, size: 64, offset: 64)
!292 = !DICompositeType(tag: DW_TAG_array_type, baseType: !16, size: 64, elements: !293)
!293 = !{!294}
!294 = !DISubrange(count: 8)
!295 = !DILocalVariable(name: "sock", arg: 1, scope: !276, file: !93, line: 20, type: !279)
!296 = !DILocation(line: 20, column: 25, scope: !276)
!297 = !DILocalVariable(name: "dest_addr", arg: 2, scope: !276, file: !93, line: 20, type: !280)
!298 = !DILocation(line: 20, column: 51, scope: !276)
!299 = !DILocalVariable(name: "msg", scope: !276, file: !93, line: 21, type: !300)
!300 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_message_t", file: !8, line: 121, baseType: !301)
!301 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_message", file: !8, line: 107, size: 2328, elements: !302)
!302 = !{!303, !304, !305, !306, !307, !308, !309, !310, !311, !312, !316, !320}
!303 = !DIDerivedType(tag: DW_TAG_member, name: "checksum", scope: !301, file: !8, line: 107, baseType: !44, size: 16)
!304 = !DIDerivedType(tag: DW_TAG_member, name: "magic", scope: !301, file: !8, line: 107, baseType: !12, size: 8, offset: 16)
!305 = !DIDerivedType(tag: DW_TAG_member, name: "len", scope: !301, file: !8, line: 107, baseType: !12, size: 8, offset: 24)
!306 = !DIDerivedType(tag: DW_TAG_member, name: "incompat_flags", scope: !301, file: !8, line: 107, baseType: !12, size: 8, offset: 32)
!307 = !DIDerivedType(tag: DW_TAG_member, name: "compat_flags", scope: !301, file: !8, line: 107, baseType: !12, size: 8, offset: 40)
!308 = !DIDerivedType(tag: DW_TAG_member, name: "seq", scope: !301, file: !8, line: 107, baseType: !12, size: 8, offset: 48)
!309 = !DIDerivedType(tag: DW_TAG_member, name: "sysid", scope: !301, file: !8, line: 107, baseType: !12, size: 8, offset: 56)
!310 = !DIDerivedType(tag: DW_TAG_member, name: "compid", scope: !301, file: !8, line: 107, baseType: !12, size: 8, offset: 64)
!311 = !DIDerivedType(tag: DW_TAG_member, name: "msgid", scope: !301, file: !8, line: 107, baseType: !72, size: 24, offset: 72, flags: DIFlagBitField, extraData: i64 72)
!312 = !DIDerivedType(tag: DW_TAG_member, name: "payload64", scope: !301, file: !8, line: 107, baseType: !313, size: 2112, offset: 96)
!313 = !DICompositeType(tag: DW_TAG_array_type, baseType: !57, size: 2112, elements: !314)
!314 = !{!315}
!315 = !DISubrange(count: 33)
!316 = !DIDerivedType(tag: DW_TAG_member, name: "ck", scope: !301, file: !8, line: 107, baseType: !317, size: 16, offset: 2208)
!317 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, size: 16, elements: !318)
!318 = !{!319}
!319 = !DISubrange(count: 2)
!320 = !DIDerivedType(tag: DW_TAG_member, name: "signature", scope: !301, file: !8, line: 107, baseType: !321, size: 104, offset: 2224)
!321 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, size: 104, elements: !322)
!322 = !{!323}
!323 = !DISubrange(count: 13)
!324 = !DILocation(line: 21, column: 23, scope: !276)
!325 = !DILocalVariable(name: "buf", scope: !276, file: !93, line: 22, type: !326)
!326 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, size: 16328, elements: !327)
!327 = !{!328}
!328 = !DISubrange(count: 2041)
!329 = !DILocation(line: 22, column: 13, scope: !276)
!330 = !DILocalVariable(name: "len", scope: !276, file: !93, line: 23, type: !44)
!331 = !DILocation(line: 23, column: 14, scope: !276)
!332 = !DILocalVariable(name: "sysid", scope: !276, file: !93, line: 25, type: !12)
!333 = !DILocation(line: 25, column: 13, scope: !276)
!334 = !DILocalVariable(name: "compid", scope: !276, file: !93, line: 26, type: !12)
!335 = !DILocation(line: 26, column: 13, scope: !276)
!336 = !DILocalVariable(name: "type", scope: !276, file: !93, line: 27, type: !12)
!337 = !DILocation(line: 27, column: 13, scope: !276)
!338 = !DILocalVariable(name: "autopilot", scope: !276, file: !93, line: 28, type: !12)
!339 = !DILocation(line: 28, column: 13, scope: !276)
!340 = !DILocalVariable(name: "base_mode", scope: !276, file: !93, line: 29, type: !12)
!341 = !DILocation(line: 29, column: 13, scope: !276)
!342 = !DILocalVariable(name: "custom_mode", scope: !276, file: !93, line: 30, type: !72)
!343 = !DILocation(line: 30, column: 14, scope: !276)
!344 = !DILocalVariable(name: "system_status", scope: !276, file: !93, line: 31, type: !12)
!345 = !DILocation(line: 31, column: 13, scope: !276)
!346 = !DILocation(line: 35, column: 9, scope: !276)
!347 = !DILocation(line: 36, column: 9, scope: !276)
!348 = !DILocation(line: 43, column: 9, scope: !276)
!349 = !DILocation(line: 44, column: 9, scope: !276)
!350 = !DILocation(line: 45, column: 9, scope: !276)
!351 = !DILocation(line: 46, column: 9, scope: !276)
!352 = !DILocation(line: 47, column: 9, scope: !276)
!353 = !DILocation(line: 34, column: 5, scope: !276)
!354 = !DILocation(line: 51, column: 38, scope: !276)
!355 = !DILocation(line: 51, column: 11, scope: !276)
!356 = !DILocation(line: 51, column: 9, scope: !276)
!357 = !DILocation(line: 54, column: 16, scope: !358)
!358 = distinct !DILexicalBlock(scope: !276, file: !93, line: 54, column: 9)
!359 = !DILocation(line: 54, column: 22, scope: !358)
!360 = !DILocation(line: 54, column: 27, scope: !358)
!361 = !DILocation(line: 54, column: 54, scope: !358)
!362 = !DILocation(line: 54, column: 35, scope: !358)
!363 = !DILocation(line: 54, column: 9, scope: !358)
!364 = !DILocation(line: 54, column: 93, scope: !358)
!365 = !DILocation(line: 54, column: 9, scope: !276)
!366 = !DILocation(line: 55, column: 9, scope: !367)
!367 = distinct !DILexicalBlock(scope: !358, file: !93, line: 54, column: 98)
!368 = !DILocation(line: 56, column: 5, scope: !367)
!369 = !DILocation(line: 57, column: 9, scope: !370)
!370 = distinct !DILexicalBlock(scope: !358, file: !93, line: 56, column: 12)
!371 = !DILocation(line: 59, column: 1, scope: !276)
!372 = distinct !DISubprogram(name: "mavlink_msg_heartbeat_pack", scope: !373, file: !373, line: 68, type: !374, scopeLine: 70, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!373 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../minimal/./mavlink_msg_heartbeat.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-client/input/source_code/mavlink")
!374 = !DISubroutineType(types: !375)
!375 = !{!44, !12, !12, !376, !12, !12, !12, !72, !12}
!376 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !300, size: 64)
!377 = !DILocalVariable(name: "system_id", arg: 1, scope: !372, file: !373, line: 68, type: !12)
!378 = !DILocation(line: 68, column: 59, scope: !372)
!379 = !DILocalVariable(name: "component_id", arg: 2, scope: !372, file: !373, line: 68, type: !12)
!380 = !DILocation(line: 68, column: 78, scope: !372)
!381 = !DILocalVariable(name: "msg", arg: 3, scope: !372, file: !373, line: 68, type: !376)
!382 = !DILocation(line: 68, column: 111, scope: !372)
!383 = !DILocalVariable(name: "type", arg: 4, scope: !372, file: !373, line: 69, type: !12)
!384 = !DILocation(line: 69, column: 40, scope: !372)
!385 = !DILocalVariable(name: "autopilot", arg: 5, scope: !372, file: !373, line: 69, type: !12)
!386 = !DILocation(line: 69, column: 54, scope: !372)
!387 = !DILocalVariable(name: "base_mode", arg: 6, scope: !372, file: !373, line: 69, type: !12)
!388 = !DILocation(line: 69, column: 73, scope: !372)
!389 = !DILocalVariable(name: "custom_mode", arg: 7, scope: !372, file: !373, line: 69, type: !72)
!390 = !DILocation(line: 69, column: 93, scope: !372)
!391 = !DILocalVariable(name: "system_status", arg: 8, scope: !372, file: !373, line: 69, type: !12)
!392 = !DILocation(line: 69, column: 114, scope: !372)
!393 = !DILocalVariable(name: "packet", scope: !372, file: !373, line: 91, type: !394)
!394 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_heartbeat_t", file: !373, line: 16, baseType: !395)
!395 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_heartbeat_t", file: !373, line: 9, size: 96, elements: !396)
!396 = !{!397, !398, !399, !400, !401, !402}
!397 = !DIDerivedType(tag: DW_TAG_member, name: "custom_mode", scope: !395, file: !373, line: 10, baseType: !72, size: 32)
!398 = !DIDerivedType(tag: DW_TAG_member, name: "type", scope: !395, file: !373, line: 11, baseType: !12, size: 8, offset: 32)
!399 = !DIDerivedType(tag: DW_TAG_member, name: "autopilot", scope: !395, file: !373, line: 12, baseType: !12, size: 8, offset: 40)
!400 = !DIDerivedType(tag: DW_TAG_member, name: "base_mode", scope: !395, file: !373, line: 13, baseType: !12, size: 8, offset: 48)
!401 = !DIDerivedType(tag: DW_TAG_member, name: "system_status", scope: !395, file: !373, line: 14, baseType: !12, size: 8, offset: 56)
!402 = !DIDerivedType(tag: DW_TAG_member, name: "mavlink_version", scope: !395, file: !373, line: 15, baseType: !12, size: 8, offset: 64)
!403 = !DILocation(line: 91, column: 25, scope: !372)
!404 = !DILocation(line: 92, column: 26, scope: !372)
!405 = !DILocation(line: 92, column: 12, scope: !372)
!406 = !DILocation(line: 92, column: 24, scope: !372)
!407 = !DILocation(line: 93, column: 19, scope: !372)
!408 = !DILocation(line: 93, column: 12, scope: !372)
!409 = !DILocation(line: 93, column: 17, scope: !372)
!410 = !DILocation(line: 94, column: 24, scope: !372)
!411 = !DILocation(line: 94, column: 12, scope: !372)
!412 = !DILocation(line: 94, column: 22, scope: !372)
!413 = !DILocation(line: 95, column: 24, scope: !372)
!414 = !DILocation(line: 95, column: 12, scope: !372)
!415 = !DILocation(line: 95, column: 22, scope: !372)
!416 = !DILocation(line: 96, column: 28, scope: !372)
!417 = !DILocation(line: 96, column: 12, scope: !372)
!418 = !DILocation(line: 96, column: 26, scope: !372)
!419 = !DILocation(line: 97, column: 12, scope: !372)
!420 = !DILocation(line: 97, column: 28, scope: !372)
!421 = !DILocation(line: 99, column: 16, scope: !372)
!422 = !DILocation(line: 99, column: 9, scope: !372)
!423 = !DILocation(line: 102, column: 5, scope: !372)
!424 = !DILocation(line: 102, column: 10, scope: !372)
!425 = !DILocation(line: 102, column: 16, scope: !372)
!426 = !DILocation(line: 103, column: 37, scope: !372)
!427 = !DILocation(line: 103, column: 42, scope: !372)
!428 = !DILocation(line: 103, column: 53, scope: !372)
!429 = !DILocation(line: 103, column: 12, scope: !372)
!430 = !DILocation(line: 103, column: 5, scope: !372)
!431 = distinct !DISubprogram(name: "mavlink_msg_to_send_buffer", scope: !3, file: !3, line: 439, type: !432, scopeLine: 440, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!432 = !DISubroutineType(types: !433)
!433 = !{!44, !256, !434}
!434 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !435, size: 64)
!435 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !300)
!436 = !DILocalVariable(name: "buf", arg: 1, scope: !431, file: !3, line: 439, type: !256)
!437 = !DILocation(line: 439, column: 61, scope: !431)
!438 = !DILocalVariable(name: "msg", arg: 2, scope: !431, file: !3, line: 439, type: !434)
!439 = !DILocation(line: 439, column: 91, scope: !431)
!440 = !DILocalVariable(name: "signature_len", scope: !431, file: !3, line: 441, type: !12)
!441 = !DILocation(line: 441, column: 10, scope: !431)
!442 = !DILocalVariable(name: "header_len", scope: !431, file: !3, line: 441, type: !12)
!443 = !DILocation(line: 441, column: 25, scope: !431)
!444 = !DILocalVariable(name: "ck", scope: !431, file: !3, line: 442, type: !256)
!445 = !DILocation(line: 442, column: 11, scope: !431)
!446 = !DILocalVariable(name: "length", scope: !431, file: !3, line: 443, type: !12)
!447 = !DILocation(line: 443, column: 17, scope: !431)
!448 = !DILocation(line: 443, column: 26, scope: !431)
!449 = !DILocation(line: 443, column: 31, scope: !431)
!450 = !DILocation(line: 445, column: 6, scope: !451)
!451 = distinct !DILexicalBlock(scope: !431, file: !3, line: 445, column: 6)
!452 = !DILocation(line: 445, column: 11, scope: !451)
!453 = !DILocation(line: 445, column: 17, scope: !451)
!454 = !DILocation(line: 445, column: 6, scope: !431)
!455 = !DILocation(line: 446, column: 17, scope: !456)
!456 = distinct !DILexicalBlock(scope: !451, file: !3, line: 445, column: 42)
!457 = !DILocation(line: 447, column: 14, scope: !456)
!458 = !DILocation(line: 448, column: 12, scope: !456)
!459 = !DILocation(line: 448, column: 17, scope: !456)
!460 = !DILocation(line: 448, column: 3, scope: !456)
!461 = !DILocation(line: 448, column: 10, scope: !456)
!462 = !DILocation(line: 449, column: 12, scope: !456)
!463 = !DILocation(line: 449, column: 3, scope: !456)
!464 = !DILocation(line: 449, column: 10, scope: !456)
!465 = !DILocation(line: 450, column: 12, scope: !456)
!466 = !DILocation(line: 450, column: 17, scope: !456)
!467 = !DILocation(line: 450, column: 3, scope: !456)
!468 = !DILocation(line: 450, column: 10, scope: !456)
!469 = !DILocation(line: 451, column: 12, scope: !456)
!470 = !DILocation(line: 451, column: 17, scope: !456)
!471 = !DILocation(line: 451, column: 3, scope: !456)
!472 = !DILocation(line: 451, column: 10, scope: !456)
!473 = !DILocation(line: 452, column: 12, scope: !456)
!474 = !DILocation(line: 452, column: 17, scope: !456)
!475 = !DILocation(line: 452, column: 3, scope: !456)
!476 = !DILocation(line: 452, column: 10, scope: !456)
!477 = !DILocation(line: 453, column: 12, scope: !456)
!478 = !DILocation(line: 453, column: 17, scope: !456)
!479 = !DILocation(line: 453, column: 23, scope: !456)
!480 = !DILocation(line: 453, column: 3, scope: !456)
!481 = !DILocation(line: 453, column: 10, scope: !456)
!482 = !DILocation(line: 454, column: 11, scope: !456)
!483 = !DILocation(line: 454, column: 19, scope: !456)
!484 = !DILocation(line: 454, column: 3, scope: !456)
!485 = !DILocation(line: 454, column: 38, scope: !456)
!486 = !DILocation(line: 454, column: 43, scope: !456)
!487 = !DILocation(line: 455, column: 8, scope: !456)
!488 = !DILocation(line: 455, column: 14, scope: !456)
!489 = !DILocation(line: 455, column: 12, scope: !456)
!490 = !DILocation(line: 455, column: 25, scope: !456)
!491 = !DILocation(line: 455, column: 41, scope: !456)
!492 = !DILocation(line: 455, column: 46, scope: !456)
!493 = !DILocation(line: 455, column: 31, scope: !456)
!494 = !DILocation(line: 455, column: 29, scope: !456)
!495 = !DILocation(line: 455, column: 6, scope: !456)
!496 = !DILocation(line: 456, column: 2, scope: !456)
!497 = !DILocation(line: 457, column: 30, scope: !498)
!498 = distinct !DILexicalBlock(scope: !451, file: !3, line: 456, column: 9)
!499 = !DILocation(line: 457, column: 49, scope: !498)
!500 = !DILocation(line: 457, column: 12, scope: !498)
!501 = !DILocation(line: 457, column: 10, scope: !498)
!502 = !DILocation(line: 458, column: 14, scope: !498)
!503 = !DILocation(line: 459, column: 12, scope: !498)
!504 = !DILocation(line: 459, column: 17, scope: !498)
!505 = !DILocation(line: 459, column: 3, scope: !498)
!506 = !DILocation(line: 459, column: 10, scope: !498)
!507 = !DILocation(line: 460, column: 12, scope: !498)
!508 = !DILocation(line: 460, column: 3, scope: !498)
!509 = !DILocation(line: 460, column: 10, scope: !498)
!510 = !DILocation(line: 461, column: 12, scope: !498)
!511 = !DILocation(line: 461, column: 17, scope: !498)
!512 = !DILocation(line: 461, column: 3, scope: !498)
!513 = !DILocation(line: 461, column: 10, scope: !498)
!514 = !DILocation(line: 462, column: 12, scope: !498)
!515 = !DILocation(line: 462, column: 17, scope: !498)
!516 = !DILocation(line: 462, column: 3, scope: !498)
!517 = !DILocation(line: 462, column: 10, scope: !498)
!518 = !DILocation(line: 463, column: 12, scope: !498)
!519 = !DILocation(line: 463, column: 17, scope: !498)
!520 = !DILocation(line: 463, column: 3, scope: !498)
!521 = !DILocation(line: 463, column: 10, scope: !498)
!522 = !DILocation(line: 464, column: 12, scope: !498)
!523 = !DILocation(line: 464, column: 17, scope: !498)
!524 = !DILocation(line: 464, column: 3, scope: !498)
!525 = !DILocation(line: 464, column: 10, scope: !498)
!526 = !DILocation(line: 465, column: 12, scope: !498)
!527 = !DILocation(line: 465, column: 17, scope: !498)
!528 = !DILocation(line: 465, column: 3, scope: !498)
!529 = !DILocation(line: 465, column: 10, scope: !498)
!530 = !DILocation(line: 466, column: 12, scope: !498)
!531 = !DILocation(line: 466, column: 17, scope: !498)
!532 = !DILocation(line: 466, column: 23, scope: !498)
!533 = !DILocation(line: 466, column: 3, scope: !498)
!534 = !DILocation(line: 466, column: 10, scope: !498)
!535 = !DILocation(line: 467, column: 13, scope: !498)
!536 = !DILocation(line: 467, column: 18, scope: !498)
!537 = !DILocation(line: 467, column: 24, scope: !498)
!538 = !DILocation(line: 467, column: 30, scope: !498)
!539 = !DILocation(line: 467, column: 12, scope: !498)
!540 = !DILocation(line: 467, column: 3, scope: !498)
!541 = !DILocation(line: 467, column: 10, scope: !498)
!542 = !DILocation(line: 468, column: 13, scope: !498)
!543 = !DILocation(line: 468, column: 18, scope: !498)
!544 = !DILocation(line: 468, column: 24, scope: !498)
!545 = !DILocation(line: 468, column: 31, scope: !498)
!546 = !DILocation(line: 468, column: 12, scope: !498)
!547 = !DILocation(line: 468, column: 3, scope: !498)
!548 = !DILocation(line: 468, column: 10, scope: !498)
!549 = !DILocation(line: 469, column: 11, scope: !498)
!550 = !DILocation(line: 469, column: 20, scope: !498)
!551 = !DILocation(line: 469, column: 3, scope: !498)
!552 = !DILocation(line: 469, column: 39, scope: !498)
!553 = !DILocation(line: 470, column: 8, scope: !498)
!554 = !DILocation(line: 470, column: 14, scope: !498)
!555 = !DILocation(line: 470, column: 12, scope: !498)
!556 = !DILocation(line: 470, column: 25, scope: !498)
!557 = !DILocation(line: 470, column: 41, scope: !498)
!558 = !DILocation(line: 470, column: 31, scope: !498)
!559 = !DILocation(line: 470, column: 29, scope: !498)
!560 = !DILocation(line: 470, column: 6, scope: !498)
!561 = !DILocation(line: 471, column: 20, scope: !498)
!562 = !DILocation(line: 471, column: 25, scope: !498)
!563 = !DILocation(line: 471, column: 40, scope: !498)
!564 = !DILocation(line: 471, column: 19, scope: !498)
!565 = !DILocation(line: 471, column: 17, scope: !498)
!566 = !DILocation(line: 473, column: 20, scope: !431)
!567 = !DILocation(line: 473, column: 25, scope: !431)
!568 = !DILocation(line: 473, column: 34, scope: !431)
!569 = !DILocation(line: 473, column: 10, scope: !431)
!570 = !DILocation(line: 473, column: 2, scope: !431)
!571 = !DILocation(line: 473, column: 8, scope: !431)
!572 = !DILocation(line: 474, column: 20, scope: !431)
!573 = !DILocation(line: 474, column: 25, scope: !431)
!574 = !DILocation(line: 474, column: 34, scope: !431)
!575 = !DILocation(line: 474, column: 10, scope: !431)
!576 = !DILocation(line: 474, column: 2, scope: !431)
!577 = !DILocation(line: 474, column: 8, scope: !431)
!578 = !DILocation(line: 475, column: 6, scope: !579)
!579 = distinct !DILexicalBlock(scope: !431, file: !3, line: 475, column: 6)
!580 = !DILocation(line: 475, column: 20, scope: !579)
!581 = !DILocation(line: 475, column: 6, scope: !431)
!582 = !DILocation(line: 476, column: 11, scope: !583)
!583 = distinct !DILexicalBlock(scope: !579, file: !3, line: 475, column: 25)
!584 = !DILocation(line: 476, column: 18, scope: !583)
!585 = !DILocation(line: 476, column: 23, scope: !583)
!586 = !DILocation(line: 476, column: 3, scope: !583)
!587 = !DILocation(line: 476, column: 34, scope: !583)
!588 = !DILocation(line: 477, column: 2, scope: !583)
!589 = !DILocation(line: 479, column: 9, scope: !431)
!590 = !DILocation(line: 479, column: 20, scope: !431)
!591 = !DILocation(line: 479, column: 24, scope: !431)
!592 = !DILocation(line: 479, column: 40, scope: !431)
!593 = !DILocation(line: 479, column: 30, scope: !431)
!594 = !DILocation(line: 479, column: 28, scope: !431)
!595 = !DILocation(line: 479, column: 59, scope: !431)
!596 = !DILocation(line: 479, column: 49, scope: !431)
!597 = !DILocation(line: 479, column: 47, scope: !431)
!598 = !DILocation(line: 479, column: 2, scope: !431)
!599 = distinct !DISubprogram(name: "main", scope: !93, file: !93, line: 61, type: !600, scopeLine: 61, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !92, retainedNodes: !270)
!600 = !DISubroutineType(types: !601)
!601 = !{!279}
!602 = !DILocalVariable(name: "sock", scope: !599, file: !93, line: 63, type: !279)
!603 = !DILocation(line: 63, column: 9, scope: !599)
!604 = !DILocation(line: 63, column: 16, scope: !599)
!605 = !DILocation(line: 64, column: 9, scope: !606)
!606 = distinct !DILexicalBlock(scope: !599, file: !93, line: 64, column: 9)
!607 = !DILocation(line: 64, column: 14, scope: !606)
!608 = !DILocation(line: 64, column: 9, scope: !599)
!609 = !DILocation(line: 65, column: 9, scope: !610)
!610 = distinct !DILexicalBlock(scope: !606, file: !93, line: 64, column: 19)
!611 = !DILocation(line: 66, column: 9, scope: !610)
!612 = !DILocalVariable(name: "flags", scope: !599, file: !93, line: 70, type: !279)
!613 = !DILocation(line: 70, column: 9, scope: !599)
!614 = !DILocation(line: 70, column: 23, scope: !599)
!615 = !DILocation(line: 70, column: 17, scope: !599)
!616 = !DILocation(line: 71, column: 11, scope: !599)
!617 = !DILocation(line: 71, column: 26, scope: !599)
!618 = !DILocation(line: 71, column: 32, scope: !599)
!619 = !DILocation(line: 71, column: 5, scope: !599)
!620 = !DILocalVariable(name: "dest_addr", scope: !599, file: !93, line: 74, type: !281)
!621 = !DILocation(line: 74, column: 24, scope: !599)
!622 = !DILocation(line: 75, column: 5, scope: !599)
!623 = !DILocation(line: 76, column: 15, scope: !599)
!624 = !DILocation(line: 76, column: 26, scope: !599)
!625 = !DILocation(line: 77, column: 26, scope: !599)
!626 = !DILocation(line: 77, column: 15, scope: !599)
!627 = !DILocation(line: 77, column: 24, scope: !599)
!628 = !DILocation(line: 78, column: 46, scope: !599)
!629 = !DILocation(line: 78, column: 35, scope: !599)
!630 = !DILocation(line: 78, column: 5, scope: !599)
!631 = !DILocation(line: 81, column: 5, scope: !599)
!632 = !DILocation(line: 82, column: 24, scope: !633)
!633 = distinct !DILexicalBlock(scope: !599, file: !93, line: 81, column: 15)
!634 = !DILocation(line: 82, column: 9, scope: !633)
!635 = !DILocation(line: 83, column: 9, scope: !633)
!636 = distinct !{!636, !631, !637}
!637 = !DILocation(line: 84, column: 5, scope: !599)
!638 = distinct !DISubprogram(name: "mavlink_finalize_message", scope: !3, file: !3, line: 296, type: !639, scopeLine: 298, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!639 = !DISubroutineType(types: !640)
!640 = !{!44, !376, !12, !12, !12, !12, !12}
!641 = !DILocalVariable(name: "msg", arg: 1, scope: !638, file: !3, line: 296, type: !376)
!642 = !DILocation(line: 296, column: 69, scope: !638)
!643 = !DILocalVariable(name: "system_id", arg: 2, scope: !638, file: !3, line: 296, type: !12)
!644 = !DILocation(line: 296, column: 82, scope: !638)
!645 = !DILocalVariable(name: "component_id", arg: 3, scope: !638, file: !3, line: 296, type: !12)
!646 = !DILocation(line: 296, column: 101, scope: !638)
!647 = !DILocalVariable(name: "min_length", arg: 4, scope: !638, file: !3, line: 297, type: !12)
!648 = !DILocation(line: 297, column: 16, scope: !638)
!649 = !DILocalVariable(name: "length", arg: 5, scope: !638, file: !3, line: 297, type: !12)
!650 = !DILocation(line: 297, column: 36, scope: !638)
!651 = !DILocalVariable(name: "crc_extra", arg: 6, scope: !638, file: !3, line: 297, type: !12)
!652 = !DILocation(line: 297, column: 52, scope: !638)
!653 = !DILocation(line: 299, column: 42, scope: !638)
!654 = !DILocation(line: 299, column: 47, scope: !638)
!655 = !DILocation(line: 299, column: 58, scope: !638)
!656 = !DILocation(line: 299, column: 88, scope: !638)
!657 = !DILocation(line: 299, column: 100, scope: !638)
!658 = !DILocation(line: 299, column: 108, scope: !638)
!659 = !DILocation(line: 299, column: 12, scope: !638)
!660 = !DILocation(line: 299, column: 5, scope: !638)
!661 = distinct !DISubprogram(name: "mavlink_finalize_message_chan", scope: !3, file: !3, line: 286, type: !662, scopeLine: 288, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!662 = !DISubroutineType(types: !663)
!663 = !{!44, !376, !12, !12, !12, !12, !12, !12}
!664 = !DILocalVariable(name: "msg", arg: 1, scope: !661, file: !3, line: 286, type: !376)
!665 = !DILocation(line: 286, column: 74, scope: !661)
!666 = !DILocalVariable(name: "system_id", arg: 2, scope: !661, file: !3, line: 286, type: !12)
!667 = !DILocation(line: 286, column: 87, scope: !661)
!668 = !DILocalVariable(name: "component_id", arg: 3, scope: !661, file: !3, line: 286, type: !12)
!669 = !DILocation(line: 286, column: 106, scope: !661)
!670 = !DILocalVariable(name: "chan", arg: 4, scope: !661, file: !3, line: 287, type: !12)
!671 = !DILocation(line: 287, column: 21, scope: !661)
!672 = !DILocalVariable(name: "min_length", arg: 5, scope: !661, file: !3, line: 287, type: !12)
!673 = !DILocation(line: 287, column: 35, scope: !661)
!674 = !DILocalVariable(name: "length", arg: 6, scope: !661, file: !3, line: 287, type: !12)
!675 = !DILocation(line: 287, column: 55, scope: !661)
!676 = !DILocalVariable(name: "crc_extra", arg: 7, scope: !661, file: !3, line: 287, type: !12)
!677 = !DILocation(line: 287, column: 71, scope: !661)
!678 = !DILocalVariable(name: "status", scope: !661, file: !3, line: 289, type: !6)
!679 = !DILocation(line: 289, column: 20, scope: !661)
!680 = !DILocation(line: 289, column: 56, scope: !661)
!681 = !DILocation(line: 289, column: 29, scope: !661)
!682 = !DILocation(line: 290, column: 41, scope: !661)
!683 = !DILocation(line: 290, column: 46, scope: !661)
!684 = !DILocation(line: 290, column: 57, scope: !661)
!685 = !DILocation(line: 290, column: 71, scope: !661)
!686 = !DILocation(line: 290, column: 79, scope: !661)
!687 = !DILocation(line: 290, column: 91, scope: !661)
!688 = !DILocation(line: 290, column: 99, scope: !661)
!689 = !DILocation(line: 290, column: 9, scope: !661)
!690 = !DILocation(line: 290, column: 2, scope: !661)
!691 = !DILocalVariable(name: "chan", arg: 1, scope: !2, file: !3, line: 23, type: !12)
!692 = !DILocation(line: 23, column: 69, scope: !2)
!693 = !DILocation(line: 31, column: 27, scope: !2)
!694 = !DILocation(line: 31, column: 10, scope: !2)
!695 = !DILocation(line: 31, column: 2, scope: !2)
!696 = distinct !DISubprogram(name: "mavlink_finalize_message_buffer", scope: !3, file: !3, line: 217, type: !697, scopeLine: 219, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!697 = !DISubroutineType(types: !698)
!698 = !{!44, !376, !12, !12, !6, !12, !12, !12}
!699 = !DILocalVariable(name: "msg", arg: 1, scope: !696, file: !3, line: 217, type: !376)
!700 = !DILocation(line: 217, column: 76, scope: !696)
!701 = !DILocalVariable(name: "system_id", arg: 2, scope: !696, file: !3, line: 217, type: !12)
!702 = !DILocation(line: 217, column: 89, scope: !696)
!703 = !DILocalVariable(name: "component_id", arg: 3, scope: !696, file: !3, line: 217, type: !12)
!704 = !DILocation(line: 217, column: 108, scope: !696)
!705 = !DILocalVariable(name: "status", arg: 4, scope: !696, file: !3, line: 218, type: !6)
!706 = !DILocation(line: 218, column: 31, scope: !696)
!707 = !DILocalVariable(name: "min_length", arg: 5, scope: !696, file: !3, line: 218, type: !12)
!708 = !DILocation(line: 218, column: 47, scope: !696)
!709 = !DILocalVariable(name: "length", arg: 6, scope: !696, file: !3, line: 218, type: !12)
!710 = !DILocation(line: 218, column: 67, scope: !696)
!711 = !DILocalVariable(name: "crc_extra", arg: 7, scope: !696, file: !3, line: 218, type: !12)
!712 = !DILocation(line: 218, column: 83, scope: !696)
!713 = !DILocalVariable(name: "mavlink1", scope: !696, file: !3, line: 220, type: !69)
!714 = !DILocation(line: 220, column: 7, scope: !696)
!715 = !DILocation(line: 220, column: 19, scope: !696)
!716 = !DILocation(line: 220, column: 27, scope: !696)
!717 = !DILocation(line: 220, column: 33, scope: !696)
!718 = !DILocation(line: 220, column: 69, scope: !696)
!719 = !DILocalVariable(name: "signing", scope: !696, file: !3, line: 222, type: !69)
!720 = !DILocation(line: 222, column: 7, scope: !696)
!721 = !DILocation(line: 222, column: 20, scope: !696)
!722 = !DILocation(line: 222, column: 30, scope: !696)
!723 = !DILocation(line: 222, column: 33, scope: !696)
!724 = !DILocation(line: 222, column: 41, scope: !696)
!725 = !DILocation(line: 222, column: 49, scope: !696)
!726 = !DILocation(line: 222, column: 53, scope: !696)
!727 = !DILocation(line: 222, column: 61, scope: !696)
!728 = !DILocation(line: 222, column: 70, scope: !696)
!729 = !DILocation(line: 222, column: 76, scope: !696)
!730 = !DILocation(line: 0, scope: !696)
!731 = !DILocalVariable(name: "signature_len", scope: !696, file: !3, line: 226, type: !12)
!732 = !DILocation(line: 226, column: 10, scope: !696)
!733 = !DILocation(line: 226, column: 26, scope: !696)
!734 = !DILocalVariable(name: "header_len", scope: !696, file: !3, line: 227, type: !12)
!735 = !DILocation(line: 227, column: 17, scope: !696)
!736 = !DILocalVariable(name: "buf", scope: !696, file: !3, line: 228, type: !737)
!737 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, size: 80, elements: !738)
!738 = !{!739}
!739 = !DISubrange(count: 10)
!740 = !DILocation(line: 228, column: 10, scope: !696)
!741 = !DILocation(line: 229, column: 6, scope: !742)
!742 = distinct !DILexicalBlock(scope: !696, file: !3, line: 229, column: 6)
!743 = !DILocation(line: 229, column: 6, scope: !696)
!744 = !DILocation(line: 230, column: 3, scope: !745)
!745 = distinct !DILexicalBlock(scope: !742, file: !3, line: 229, column: 16)
!746 = !DILocation(line: 230, column: 8, scope: !745)
!747 = !DILocation(line: 230, column: 14, scope: !745)
!748 = !DILocation(line: 231, column: 14, scope: !745)
!749 = !DILocation(line: 232, column: 2, scope: !745)
!750 = !DILocation(line: 233, column: 3, scope: !751)
!751 = distinct !DILexicalBlock(scope: !742, file: !3, line: 232, column: 9)
!752 = !DILocation(line: 233, column: 8, scope: !751)
!753 = !DILocation(line: 233, column: 14, scope: !751)
!754 = !DILocation(line: 235, column: 13, scope: !696)
!755 = !DILocation(line: 235, column: 22, scope: !696)
!756 = !DILocation(line: 235, column: 51, scope: !696)
!757 = !DILocation(line: 235, column: 70, scope: !696)
!758 = !DILocation(line: 235, column: 33, scope: !696)
!759 = !DILocation(line: 235, column: 2, scope: !696)
!760 = !DILocation(line: 235, column: 7, scope: !696)
!761 = !DILocation(line: 235, column: 11, scope: !696)
!762 = !DILocation(line: 236, column: 15, scope: !696)
!763 = !DILocation(line: 236, column: 2, scope: !696)
!764 = !DILocation(line: 236, column: 7, scope: !696)
!765 = !DILocation(line: 236, column: 13, scope: !696)
!766 = !DILocation(line: 237, column: 16, scope: !696)
!767 = !DILocation(line: 237, column: 2, scope: !696)
!768 = !DILocation(line: 237, column: 7, scope: !696)
!769 = !DILocation(line: 237, column: 14, scope: !696)
!770 = !DILocation(line: 238, column: 2, scope: !696)
!771 = !DILocation(line: 238, column: 7, scope: !696)
!772 = !DILocation(line: 238, column: 22, scope: !696)
!773 = !DILocation(line: 239, column: 6, scope: !774)
!774 = distinct !DILexicalBlock(scope: !696, file: !3, line: 239, column: 6)
!775 = !DILocation(line: 239, column: 6, scope: !696)
!776 = !DILocation(line: 240, column: 3, scope: !777)
!777 = distinct !DILexicalBlock(scope: !774, file: !3, line: 239, column: 15)
!778 = !DILocation(line: 240, column: 8, scope: !777)
!779 = !DILocation(line: 240, column: 23, scope: !777)
!780 = !DILocation(line: 241, column: 2, scope: !777)
!781 = !DILocation(line: 242, column: 2, scope: !696)
!782 = !DILocation(line: 242, column: 7, scope: !696)
!783 = !DILocation(line: 242, column: 20, scope: !696)
!784 = !DILocation(line: 243, column: 13, scope: !696)
!785 = !DILocation(line: 243, column: 21, scope: !696)
!786 = !DILocation(line: 243, column: 2, scope: !696)
!787 = !DILocation(line: 243, column: 7, scope: !696)
!788 = !DILocation(line: 243, column: 11, scope: !696)
!789 = !DILocation(line: 244, column: 27, scope: !696)
!790 = !DILocation(line: 244, column: 35, scope: !696)
!791 = !DILocation(line: 244, column: 50, scope: !696)
!792 = !DILocation(line: 244, column: 2, scope: !696)
!793 = !DILocation(line: 244, column: 10, scope: !696)
!794 = !DILocation(line: 244, column: 25, scope: !696)
!795 = !DILocation(line: 247, column: 11, scope: !696)
!796 = !DILocation(line: 247, column: 16, scope: !696)
!797 = !DILocation(line: 247, column: 2, scope: !696)
!798 = !DILocation(line: 247, column: 9, scope: !696)
!799 = !DILocation(line: 248, column: 11, scope: !696)
!800 = !DILocation(line: 248, column: 16, scope: !696)
!801 = !DILocation(line: 248, column: 2, scope: !696)
!802 = !DILocation(line: 248, column: 9, scope: !696)
!803 = !DILocation(line: 249, column: 6, scope: !804)
!804 = distinct !DILexicalBlock(scope: !696, file: !3, line: 249, column: 6)
!805 = !DILocation(line: 249, column: 6, scope: !696)
!806 = !DILocation(line: 250, column: 12, scope: !807)
!807 = distinct !DILexicalBlock(scope: !804, file: !3, line: 249, column: 16)
!808 = !DILocation(line: 250, column: 17, scope: !807)
!809 = !DILocation(line: 250, column: 3, scope: !807)
!810 = !DILocation(line: 250, column: 10, scope: !807)
!811 = !DILocation(line: 251, column: 12, scope: !807)
!812 = !DILocation(line: 251, column: 17, scope: !807)
!813 = !DILocation(line: 251, column: 3, scope: !807)
!814 = !DILocation(line: 251, column: 10, scope: !807)
!815 = !DILocation(line: 252, column: 12, scope: !807)
!816 = !DILocation(line: 252, column: 17, scope: !807)
!817 = !DILocation(line: 252, column: 3, scope: !807)
!818 = !DILocation(line: 252, column: 10, scope: !807)
!819 = !DILocation(line: 253, column: 12, scope: !807)
!820 = !DILocation(line: 253, column: 17, scope: !807)
!821 = !DILocation(line: 253, column: 23, scope: !807)
!822 = !DILocation(line: 253, column: 3, scope: !807)
!823 = !DILocation(line: 253, column: 10, scope: !807)
!824 = !DILocation(line: 254, column: 2, scope: !807)
!825 = !DILocation(line: 255, column: 12, scope: !826)
!826 = distinct !DILexicalBlock(scope: !804, file: !3, line: 254, column: 9)
!827 = !DILocation(line: 255, column: 17, scope: !826)
!828 = !DILocation(line: 255, column: 3, scope: !826)
!829 = !DILocation(line: 255, column: 10, scope: !826)
!830 = !DILocation(line: 256, column: 12, scope: !826)
!831 = !DILocation(line: 256, column: 17, scope: !826)
!832 = !DILocation(line: 256, column: 3, scope: !826)
!833 = !DILocation(line: 256, column: 10, scope: !826)
!834 = !DILocation(line: 257, column: 12, scope: !826)
!835 = !DILocation(line: 257, column: 17, scope: !826)
!836 = !DILocation(line: 257, column: 3, scope: !826)
!837 = !DILocation(line: 257, column: 10, scope: !826)
!838 = !DILocation(line: 258, column: 12, scope: !826)
!839 = !DILocation(line: 258, column: 17, scope: !826)
!840 = !DILocation(line: 258, column: 3, scope: !826)
!841 = !DILocation(line: 258, column: 10, scope: !826)
!842 = !DILocation(line: 259, column: 12, scope: !826)
!843 = !DILocation(line: 259, column: 17, scope: !826)
!844 = !DILocation(line: 259, column: 3, scope: !826)
!845 = !DILocation(line: 259, column: 10, scope: !826)
!846 = !DILocation(line: 260, column: 12, scope: !826)
!847 = !DILocation(line: 260, column: 17, scope: !826)
!848 = !DILocation(line: 260, column: 23, scope: !826)
!849 = !DILocation(line: 260, column: 3, scope: !826)
!850 = !DILocation(line: 260, column: 10, scope: !826)
!851 = !DILocation(line: 261, column: 13, scope: !826)
!852 = !DILocation(line: 261, column: 18, scope: !826)
!853 = !DILocation(line: 261, column: 24, scope: !826)
!854 = !DILocation(line: 261, column: 30, scope: !826)
!855 = !DILocation(line: 261, column: 12, scope: !826)
!856 = !DILocation(line: 261, column: 3, scope: !826)
!857 = !DILocation(line: 261, column: 10, scope: !826)
!858 = !DILocation(line: 262, column: 13, scope: !826)
!859 = !DILocation(line: 262, column: 18, scope: !826)
!860 = !DILocation(line: 262, column: 24, scope: !826)
!861 = !DILocation(line: 262, column: 31, scope: !826)
!862 = !DILocation(line: 262, column: 12, scope: !826)
!863 = !DILocation(line: 262, column: 3, scope: !826)
!864 = !DILocation(line: 262, column: 10, scope: !826)
!865 = !DILocalVariable(name: "checksum", scope: !696, file: !3, line: 265, type: !44)
!866 = !DILocation(line: 265, column: 11, scope: !696)
!867 = !DILocation(line: 265, column: 37, scope: !696)
!868 = !DILocation(line: 265, column: 45, scope: !696)
!869 = !DILocation(line: 265, column: 55, scope: !696)
!870 = !DILocation(line: 265, column: 22, scope: !696)
!871 = !DILocation(line: 266, column: 35, scope: !696)
!872 = !DILocation(line: 266, column: 54, scope: !696)
!873 = !DILocation(line: 266, column: 59, scope: !696)
!874 = !DILocation(line: 266, column: 2, scope: !696)
!875 = !DILocation(line: 267, column: 17, scope: !696)
!876 = !DILocation(line: 267, column: 2, scope: !696)
!877 = !DILocation(line: 268, column: 32, scope: !696)
!878 = !DILocation(line: 268, column: 41, scope: !696)
!879 = !DILocation(line: 268, column: 22, scope: !696)
!880 = !DILocation(line: 268, column: 2, scope: !696)
!881 = !DILocation(line: 268, column: 20, scope: !696)
!882 = !DILocation(line: 269, column: 32, scope: !696)
!883 = !DILocation(line: 269, column: 41, scope: !696)
!884 = !DILocation(line: 269, column: 22, scope: !696)
!885 = !DILocation(line: 269, column: 2, scope: !696)
!886 = !DILocation(line: 269, column: 20, scope: !696)
!887 = !DILocation(line: 271, column: 18, scope: !696)
!888 = !DILocation(line: 271, column: 2, scope: !696)
!889 = !DILocation(line: 271, column: 7, scope: !696)
!890 = !DILocation(line: 271, column: 16, scope: !696)
!891 = !DILocation(line: 274, column: 6, scope: !892)
!892 = distinct !DILexicalBlock(scope: !696, file: !3, line: 274, column: 6)
!893 = !DILocation(line: 274, column: 6, scope: !696)
!894 = !DILocation(line: 275, column: 23, scope: !895)
!895 = distinct !DILexicalBlock(scope: !892, file: !3, line: 274, column: 15)
!896 = !DILocation(line: 275, column: 31, scope: !895)
!897 = !DILocation(line: 276, column: 9, scope: !895)
!898 = !DILocation(line: 276, column: 14, scope: !895)
!899 = !DILocation(line: 277, column: 26, scope: !895)
!900 = !DILocation(line: 277, column: 31, scope: !895)
!901 = !DILocation(line: 278, column: 26, scope: !895)
!902 = !DILocation(line: 278, column: 45, scope: !895)
!903 = !DILocation(line: 278, column: 50, scope: !895)
!904 = !DILocation(line: 279, column: 26, scope: !895)
!905 = !DILocation(line: 279, column: 54, scope: !895)
!906 = !DILocation(line: 279, column: 59, scope: !895)
!907 = !DILocation(line: 279, column: 44, scope: !895)
!908 = !DILocation(line: 279, column: 43, scope: !895)
!909 = !DILocation(line: 275, column: 3, scope: !895)
!910 = !DILocation(line: 280, column: 2, scope: !895)
!911 = !DILocation(line: 283, column: 9, scope: !696)
!912 = !DILocation(line: 283, column: 14, scope: !696)
!913 = !DILocation(line: 283, column: 20, scope: !696)
!914 = !DILocation(line: 283, column: 18, scope: !696)
!915 = !DILocation(line: 283, column: 31, scope: !696)
!916 = !DILocation(line: 283, column: 37, scope: !696)
!917 = !DILocation(line: 283, column: 35, scope: !696)
!918 = !DILocation(line: 283, column: 2, scope: !696)
!919 = distinct !DISubprogram(name: "_mav_trim_payload", scope: !3, file: !3, line: 111, type: !920, scopeLine: 112, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!920 = !DISubroutineType(types: !921)
!921 = !{!12, !254, !12}
!922 = !DILocalVariable(name: "payload", arg: 1, scope: !919, file: !3, line: 111, type: !254)
!923 = !DILocation(line: 111, column: 54, scope: !919)
!924 = !DILocalVariable(name: "length", arg: 2, scope: !919, file: !3, line: 111, type: !12)
!925 = !DILocation(line: 111, column: 71, scope: !919)
!926 = !DILocation(line: 113, column: 2, scope: !919)
!927 = !DILocation(line: 113, column: 9, scope: !919)
!928 = !DILocation(line: 113, column: 16, scope: !919)
!929 = !DILocation(line: 113, column: 20, scope: !919)
!930 = !DILocation(line: 113, column: 23, scope: !919)
!931 = !DILocation(line: 113, column: 31, scope: !919)
!932 = !DILocation(line: 113, column: 37, scope: !919)
!933 = !DILocation(line: 113, column: 41, scope: !919)
!934 = !DILocation(line: 0, scope: !919)
!935 = !DILocation(line: 114, column: 9, scope: !936)
!936 = distinct !DILexicalBlock(scope: !919, file: !3, line: 113, column: 47)
!937 = distinct !{!937, !926, !938}
!938 = !DILocation(line: 115, column: 2, scope: !919)
!939 = !DILocation(line: 116, column: 9, scope: !919)
!940 = !DILocation(line: 116, column: 2, scope: !919)
!941 = distinct !DISubprogram(name: "crc_calculate", scope: !942, file: !942, line: 65, type: !943, scopeLine: 66, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!942 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../checksum.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-client/input/source_code/mavlink")
!943 = !DISubroutineType(types: !944)
!944 = !{!44, !257, !44}
!945 = !DILocalVariable(name: "pBuffer", arg: 1, scope: !941, file: !942, line: 65, type: !257)
!946 = !DILocation(line: 65, column: 53, scope: !941)
!947 = !DILocalVariable(name: "length", arg: 2, scope: !941, file: !942, line: 65, type: !44)
!948 = !DILocation(line: 65, column: 71, scope: !941)
!949 = !DILocalVariable(name: "crcTmp", scope: !941, file: !942, line: 67, type: !44)
!950 = !DILocation(line: 67, column: 18, scope: !941)
!951 = !DILocation(line: 68, column: 9, scope: !941)
!952 = !DILocation(line: 69, column: 2, scope: !941)
!953 = !DILocation(line: 69, column: 15, scope: !941)
!954 = !DILocation(line: 70, column: 40, scope: !955)
!955 = distinct !DILexicalBlock(scope: !941, file: !942, line: 69, column: 19)
!956 = !DILocation(line: 70, column: 32, scope: !955)
!957 = !DILocation(line: 70, column: 17, scope: !955)
!958 = distinct !{!958, !952, !959}
!959 = !DILocation(line: 71, column: 9, scope: !941)
!960 = !DILocation(line: 72, column: 16, scope: !941)
!961 = !DILocation(line: 72, column: 9, scope: !941)
!962 = distinct !DISubprogram(name: "crc_accumulate_buffer", scope: !942, file: !942, line: 85, type: !963, scopeLine: 86, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!963 = !DISubroutineType(types: !964)
!964 = !{null, !965, !254, !44}
!965 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !44, size: 64)
!966 = !DILocalVariable(name: "crcAccum", arg: 1, scope: !962, file: !942, line: 85, type: !965)
!967 = !DILocation(line: 85, column: 52, scope: !962)
!968 = !DILocalVariable(name: "pBuffer", arg: 2, scope: !962, file: !942, line: 85, type: !254)
!969 = !DILocation(line: 85, column: 74, scope: !962)
!970 = !DILocalVariable(name: "length", arg: 3, scope: !962, file: !942, line: 85, type: !44)
!971 = !DILocation(line: 85, column: 92, scope: !962)
!972 = !DILocalVariable(name: "p", scope: !962, file: !942, line: 87, type: !257)
!973 = !DILocation(line: 87, column: 17, scope: !962)
!974 = !DILocation(line: 87, column: 38, scope: !962)
!975 = !DILocation(line: 88, column: 2, scope: !962)
!976 = !DILocation(line: 88, column: 15, scope: !962)
!977 = !DILocation(line: 89, column: 34, scope: !978)
!978 = distinct !DILexicalBlock(scope: !962, file: !942, line: 88, column: 19)
!979 = !DILocation(line: 89, column: 32, scope: !978)
!980 = !DILocation(line: 89, column: 38, scope: !978)
!981 = !DILocation(line: 89, column: 17, scope: !978)
!982 = distinct !{!982, !975, !983}
!983 = !DILocation(line: 90, column: 9, scope: !962)
!984 = !DILocation(line: 91, column: 1, scope: !962)
!985 = distinct !DISubprogram(name: "crc_accumulate", scope: !942, file: !942, line: 35, type: !986, scopeLine: 36, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!986 = !DISubroutineType(types: !987)
!987 = !{null, !12, !965}
!988 = !DILocalVariable(name: "data", arg: 1, scope: !985, file: !942, line: 35, type: !12)
!989 = !DILocation(line: 35, column: 43, scope: !985)
!990 = !DILocalVariable(name: "crcAccum", arg: 2, scope: !985, file: !942, line: 35, type: !965)
!991 = !DILocation(line: 35, column: 59, scope: !985)
!992 = !DILocalVariable(name: "tmp", scope: !985, file: !942, line: 38, type: !12)
!993 = !DILocation(line: 38, column: 17, scope: !985)
!994 = !DILocation(line: 40, column: 15, scope: !985)
!995 = !DILocation(line: 40, column: 33, scope: !985)
!996 = !DILocation(line: 40, column: 32, scope: !985)
!997 = !DILocation(line: 40, column: 42, scope: !985)
!998 = !DILocation(line: 40, column: 22, scope: !985)
!999 = !DILocation(line: 40, column: 20, scope: !985)
!1000 = !DILocation(line: 40, column: 13, scope: !985)
!1001 = !DILocation(line: 41, column: 17, scope: !985)
!1002 = !DILocation(line: 41, column: 20, scope: !985)
!1003 = !DILocation(line: 41, column: 13, scope: !985)
!1004 = !DILocation(line: 42, column: 23, scope: !985)
!1005 = !DILocation(line: 42, column: 22, scope: !985)
!1006 = !DILocation(line: 42, column: 31, scope: !985)
!1007 = !DILocation(line: 42, column: 39, scope: !985)
!1008 = !DILocation(line: 42, column: 42, scope: !985)
!1009 = !DILocation(line: 42, column: 36, scope: !985)
!1010 = !DILocation(line: 42, column: 50, scope: !985)
!1011 = !DILocation(line: 42, column: 54, scope: !985)
!1012 = !DILocation(line: 42, column: 47, scope: !985)
!1013 = !DILocation(line: 42, column: 62, scope: !985)
!1014 = !DILocation(line: 42, column: 65, scope: !985)
!1015 = !DILocation(line: 42, column: 59, scope: !985)
!1016 = !DILocation(line: 42, column: 21, scope: !985)
!1017 = !DILocation(line: 42, column: 10, scope: !985)
!1018 = !DILocation(line: 42, column: 19, scope: !985)
!1019 = !DILocation(line: 43, column: 1, scope: !985)
!1020 = distinct !DISubprogram(name: "mavlink_sign_packet", scope: !3, file: !3, line: 73, type: !1021, scopeLine: 78, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!1021 = !DISubroutineType(types: !1022)
!1022 = !{!12, !1023, !256, !257, !12, !257, !12, !257}
!1023 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1024, size: 64)
!1024 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_signing_t", file: !8, line: 254, baseType: !52)
!1025 = !DILocalVariable(name: "signing", arg: 1, scope: !1020, file: !3, line: 73, type: !1023)
!1026 = !DILocation(line: 73, column: 63, scope: !1020)
!1027 = !DILocalVariable(name: "signature", arg: 2, scope: !1020, file: !3, line: 74, type: !256)
!1028 = !DILocation(line: 74, column: 17, scope: !1020)
!1029 = !DILocalVariable(name: "header", arg: 3, scope: !1020, file: !3, line: 75, type: !257)
!1030 = !DILocation(line: 75, column: 24, scope: !1020)
!1031 = !DILocalVariable(name: "header_len", arg: 4, scope: !1020, file: !3, line: 75, type: !12)
!1032 = !DILocation(line: 75, column: 40, scope: !1020)
!1033 = !DILocalVariable(name: "packet", arg: 5, scope: !1020, file: !3, line: 76, type: !257)
!1034 = !DILocation(line: 76, column: 24, scope: !1020)
!1035 = !DILocalVariable(name: "packet_len", arg: 6, scope: !1020, file: !3, line: 76, type: !12)
!1036 = !DILocation(line: 76, column: 40, scope: !1020)
!1037 = !DILocalVariable(name: "crc", arg: 7, scope: !1020, file: !3, line: 77, type: !257)
!1038 = !DILocation(line: 77, column: 23, scope: !1020)
!1039 = !DILocalVariable(name: "ctx", scope: !1020, file: !3, line: 79, type: !1040)
!1040 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_sha256_ctx", file: !265, line: 60, baseType: !1041)
!1041 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !265, line: 53, size: 832, elements: !1042)
!1042 = !{!1043, !1045, !1047}
!1043 = !DIDerivedType(tag: DW_TAG_member, name: "sz", scope: !1041, file: !265, line: 54, baseType: !1044, size: 64)
!1044 = !DICompositeType(tag: DW_TAG_array_type, baseType: !72, size: 64, elements: !318)
!1045 = !DIDerivedType(tag: DW_TAG_member, name: "counter", scope: !1041, file: !265, line: 55, baseType: !1046, size: 256, offset: 64)
!1046 = !DICompositeType(tag: DW_TAG_array_type, baseType: !72, size: 256, elements: !293)
!1047 = !DIDerivedType(tag: DW_TAG_member, name: "u", scope: !1041, file: !265, line: 59, baseType: !1048, size: 512, offset: 320)
!1048 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !1041, file: !265, line: 56, size: 512, elements: !1049)
!1049 = !{!1050, !1052}
!1050 = !DIDerivedType(tag: DW_TAG_member, name: "save_bytes", scope: !1048, file: !265, line: 57, baseType: !1051, size: 512)
!1051 = !DICompositeType(tag: DW_TAG_array_type, baseType: !16, size: 512, elements: !268)
!1052 = !DIDerivedType(tag: DW_TAG_member, name: "save_u32", scope: !1048, file: !265, line: 58, baseType: !1053, size: 512)
!1053 = !DICompositeType(tag: DW_TAG_array_type, baseType: !72, size: 512, elements: !90)
!1054 = !DILocation(line: 79, column: 21, scope: !1020)
!1055 = !DILocalVariable(name: "tstamp", scope: !1020, file: !3, line: 83, type: !1056)
!1056 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !1020, file: !3, line: 80, size: 64, elements: !1057)
!1057 = !{!1058, !1059}
!1058 = !DIDerivedType(tag: DW_TAG_member, name: "t64", scope: !1056, file: !3, line: 81, baseType: !57, size: 64)
!1059 = !DIDerivedType(tag: DW_TAG_member, name: "t8", scope: !1056, file: !3, line: 82, baseType: !1060, size: 64)
!1060 = !DICompositeType(tag: DW_TAG_array_type, baseType: !12, size: 64, elements: !293)
!1061 = !DILocation(line: 83, column: 4, scope: !1020)
!1062 = !DILocation(line: 84, column: 6, scope: !1063)
!1063 = distinct !DILexicalBlock(scope: !1020, file: !3, line: 84, column: 6)
!1064 = !DILocation(line: 84, column: 14, scope: !1063)
!1065 = !DILocation(line: 84, column: 22, scope: !1063)
!1066 = !DILocation(line: 84, column: 27, scope: !1063)
!1067 = !DILocation(line: 84, column: 36, scope: !1063)
!1068 = !DILocation(line: 84, column: 42, scope: !1063)
!1069 = !DILocation(line: 84, column: 6, scope: !1020)
!1070 = !DILocation(line: 85, column: 6, scope: !1071)
!1071 = distinct !DILexicalBlock(scope: !1063, file: !3, line: 84, column: 81)
!1072 = !DILocation(line: 87, column: 17, scope: !1020)
!1073 = !DILocation(line: 87, column: 26, scope: !1020)
!1074 = !DILocation(line: 87, column: 2, scope: !1020)
!1075 = !DILocation(line: 87, column: 15, scope: !1020)
!1076 = !DILocation(line: 88, column: 15, scope: !1020)
!1077 = !DILocation(line: 88, column: 24, scope: !1020)
!1078 = !DILocation(line: 88, column: 9, scope: !1020)
!1079 = !DILocation(line: 88, column: 13, scope: !1020)
!1080 = !DILocation(line: 89, column: 10, scope: !1020)
!1081 = !DILocation(line: 89, column: 31, scope: !1020)
!1082 = !DILocation(line: 89, column: 2, scope: !1020)
!1083 = !DILocation(line: 90, column: 2, scope: !1020)
!1084 = !DILocation(line: 90, column: 11, scope: !1020)
!1085 = !DILocation(line: 90, column: 20, scope: !1020)
!1086 = !DILocation(line: 92, column: 2, scope: !1020)
!1087 = !DILocation(line: 93, column: 30, scope: !1020)
!1088 = !DILocation(line: 93, column: 39, scope: !1020)
!1089 = !DILocation(line: 93, column: 2, scope: !1020)
!1090 = !DILocation(line: 94, column: 30, scope: !1020)
!1091 = !DILocation(line: 94, column: 38, scope: !1020)
!1092 = !DILocation(line: 94, column: 2, scope: !1020)
!1093 = !DILocation(line: 95, column: 30, scope: !1020)
!1094 = !DILocation(line: 95, column: 38, scope: !1020)
!1095 = !DILocation(line: 95, column: 2, scope: !1020)
!1096 = !DILocation(line: 96, column: 30, scope: !1020)
!1097 = !DILocation(line: 96, column: 2, scope: !1020)
!1098 = !DILocation(line: 97, column: 30, scope: !1020)
!1099 = !DILocation(line: 97, column: 2, scope: !1020)
!1100 = !DILocation(line: 98, column: 33, scope: !1020)
!1101 = !DILocation(line: 98, column: 2, scope: !1020)
!1102 = !DILocation(line: 100, column: 2, scope: !1020)
!1103 = !DILocation(line: 101, column: 1, scope: !1020)
!1104 = distinct !DISubprogram(name: "crc_init", scope: !942, file: !942, line: 52, type: !1105, scopeLine: 53, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!1105 = !DISubroutineType(types: !1106)
!1106 = !{null, !965}
!1107 = !DILocalVariable(name: "crcAccum", arg: 1, scope: !1104, file: !942, line: 52, type: !965)
!1108 = !DILocation(line: 52, column: 39, scope: !1104)
!1109 = !DILocation(line: 54, column: 10, scope: !1104)
!1110 = !DILocation(line: 54, column: 19, scope: !1104)
!1111 = !DILocation(line: 55, column: 1, scope: !1104)
!1112 = distinct !DISubprogram(name: "mavlink_sha256_init", scope: !265, file: !265, line: 91, type: !1113, scopeLine: 92, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!1113 = !DISubroutineType(types: !1114)
!1114 = !{null, !1115}
!1115 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1040, size: 64)
!1116 = !DILocalVariable(name: "m", arg: 1, scope: !1112, file: !265, line: 91, type: !1115)
!1117 = !DILocation(line: 91, column: 61, scope: !1112)
!1118 = !DILocation(line: 93, column: 5, scope: !1112)
!1119 = !DILocation(line: 93, column: 8, scope: !1112)
!1120 = !DILocation(line: 93, column: 14, scope: !1112)
!1121 = !DILocation(line: 94, column: 5, scope: !1112)
!1122 = !DILocation(line: 94, column: 8, scope: !1112)
!1123 = !DILocation(line: 94, column: 14, scope: !1112)
!1124 = !DILocation(line: 95, column: 5, scope: !1112)
!1125 = !DILocation(line: 95, column: 8, scope: !1112)
!1126 = !DILocation(line: 95, column: 19, scope: !1112)
!1127 = !DILocation(line: 96, column: 5, scope: !1112)
!1128 = !DILocation(line: 96, column: 8, scope: !1112)
!1129 = !DILocation(line: 96, column: 19, scope: !1112)
!1130 = !DILocation(line: 97, column: 5, scope: !1112)
!1131 = !DILocation(line: 97, column: 8, scope: !1112)
!1132 = !DILocation(line: 97, column: 19, scope: !1112)
!1133 = !DILocation(line: 98, column: 5, scope: !1112)
!1134 = !DILocation(line: 98, column: 8, scope: !1112)
!1135 = !DILocation(line: 98, column: 19, scope: !1112)
!1136 = !DILocation(line: 99, column: 5, scope: !1112)
!1137 = !DILocation(line: 99, column: 8, scope: !1112)
!1138 = !DILocation(line: 99, column: 19, scope: !1112)
!1139 = !DILocation(line: 100, column: 5, scope: !1112)
!1140 = !DILocation(line: 100, column: 8, scope: !1112)
!1141 = !DILocation(line: 100, column: 19, scope: !1112)
!1142 = !DILocation(line: 101, column: 5, scope: !1112)
!1143 = !DILocation(line: 101, column: 8, scope: !1112)
!1144 = !DILocation(line: 101, column: 19, scope: !1112)
!1145 = !DILocation(line: 102, column: 5, scope: !1112)
!1146 = !DILocation(line: 102, column: 8, scope: !1112)
!1147 = !DILocation(line: 102, column: 19, scope: !1112)
!1148 = !DILocation(line: 103, column: 1, scope: !1112)
!1149 = distinct !DISubprogram(name: "mavlink_sha256_update", scope: !265, file: !265, line: 152, type: !1150, scopeLine: 153, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!1150 = !DISubroutineType(types: !1151)
!1151 = !{null, !1115, !1152, !72}
!1152 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1153, size: 64)
!1153 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!1154 = !DILocalVariable(name: "m", arg: 1, scope: !1149, file: !265, line: 152, type: !1115)
!1155 = !DILocation(line: 152, column: 63, scope: !1149)
!1156 = !DILocalVariable(name: "v", arg: 2, scope: !1149, file: !265, line: 152, type: !1152)
!1157 = !DILocation(line: 152, column: 78, scope: !1149)
!1158 = !DILocalVariable(name: "len", arg: 3, scope: !1149, file: !265, line: 152, type: !72)
!1159 = !DILocation(line: 152, column: 90, scope: !1149)
!1160 = !DILocalVariable(name: "p", scope: !1149, file: !265, line: 154, type: !260)
!1161 = !DILocation(line: 154, column: 26, scope: !1149)
!1162 = !DILocation(line: 154, column: 53, scope: !1149)
!1163 = !DILocalVariable(name: "old_sz", scope: !1149, file: !265, line: 155, type: !72)
!1164 = !DILocation(line: 155, column: 14, scope: !1149)
!1165 = !DILocation(line: 155, column: 23, scope: !1149)
!1166 = !DILocation(line: 155, column: 26, scope: !1149)
!1167 = !DILocalVariable(name: "offset", scope: !1149, file: !265, line: 156, type: !72)
!1168 = !DILocation(line: 156, column: 14, scope: !1149)
!1169 = !DILocation(line: 158, column: 17, scope: !1149)
!1170 = !DILocation(line: 158, column: 21, scope: !1149)
!1171 = !DILocation(line: 158, column: 5, scope: !1149)
!1172 = !DILocation(line: 158, column: 8, scope: !1149)
!1173 = !DILocation(line: 158, column: 14, scope: !1149)
!1174 = !DILocation(line: 159, column: 9, scope: !1175)
!1175 = distinct !DILexicalBlock(scope: !1149, file: !265, line: 159, column: 9)
!1176 = !DILocation(line: 159, column: 12, scope: !1175)
!1177 = !DILocation(line: 159, column: 20, scope: !1175)
!1178 = !DILocation(line: 159, column: 18, scope: !1175)
!1179 = !DILocation(line: 159, column: 9, scope: !1149)
!1180 = !DILocation(line: 160, column: 4, scope: !1175)
!1181 = !DILocation(line: 160, column: 7, scope: !1175)
!1182 = !DILocation(line: 160, column: 2, scope: !1175)
!1183 = !DILocation(line: 161, column: 15, scope: !1149)
!1184 = !DILocation(line: 161, column: 22, scope: !1149)
!1185 = !DILocation(line: 161, column: 27, scope: !1149)
!1186 = !DILocation(line: 161, column: 12, scope: !1149)
!1187 = !DILocation(line: 162, column: 5, scope: !1149)
!1188 = !DILocation(line: 162, column: 11, scope: !1149)
!1189 = !DILocation(line: 162, column: 15, scope: !1149)
!1190 = !DILocalVariable(name: "l", scope: !1191, file: !265, line: 163, type: !72)
!1191 = distinct !DILexicalBlock(scope: !1149, file: !265, line: 162, column: 19)
!1192 = !DILocation(line: 163, column: 11, scope: !1191)
!1193 = !DILocation(line: 163, column: 20, scope: !1191)
!1194 = !DILocation(line: 163, column: 18, scope: !1191)
!1195 = !DILocation(line: 164, column: 13, scope: !1196)
!1196 = distinct !DILexicalBlock(scope: !1191, file: !265, line: 164, column: 13)
!1197 = !DILocation(line: 164, column: 19, scope: !1196)
!1198 = !DILocation(line: 164, column: 17, scope: !1196)
!1199 = !DILocation(line: 164, column: 13, scope: !1191)
!1200 = !DILocation(line: 165, column: 17, scope: !1201)
!1201 = distinct !DILexicalBlock(scope: !1196, file: !265, line: 164, column: 22)
!1202 = !DILocation(line: 165, column: 15, scope: !1201)
!1203 = !DILocation(line: 166, column: 9, scope: !1201)
!1204 = !DILocation(line: 167, column: 9, scope: !1191)
!1205 = !DILocation(line: 167, column: 12, scope: !1191)
!1206 = !DILocation(line: 167, column: 14, scope: !1191)
!1207 = !DILocation(line: 167, column: 27, scope: !1191)
!1208 = !DILocation(line: 167, column: 25, scope: !1191)
!1209 = !DILocation(line: 167, column: 35, scope: !1191)
!1210 = !DILocation(line: 167, column: 38, scope: !1191)
!1211 = !DILocation(line: 167, column: 2, scope: !1191)
!1212 = !DILocation(line: 168, column: 12, scope: !1191)
!1213 = !DILocation(line: 168, column: 9, scope: !1191)
!1214 = !DILocation(line: 169, column: 7, scope: !1191)
!1215 = !DILocation(line: 169, column: 4, scope: !1191)
!1216 = !DILocation(line: 170, column: 9, scope: !1191)
!1217 = !DILocation(line: 170, column: 6, scope: !1191)
!1218 = !DILocation(line: 171, column: 5, scope: !1219)
!1219 = distinct !DILexicalBlock(scope: !1191, file: !265, line: 171, column: 5)
!1220 = !DILocation(line: 171, column: 12, scope: !1219)
!1221 = !DILocation(line: 171, column: 5, scope: !1191)
!1222 = !DILocalVariable(name: "i", scope: !1223, file: !265, line: 172, type: !279)
!1223 = distinct !DILexicalBlock(scope: !1219, file: !265, line: 171, column: 18)
!1224 = !DILocation(line: 172, column: 10, scope: !1223)
!1225 = !DILocalVariable(name: "current", scope: !1223, file: !265, line: 173, type: !1053)
!1226 = !DILocation(line: 173, column: 15, scope: !1223)
!1227 = !DILocalVariable(name: "u", scope: !1223, file: !265, line: 174, type: !1228)
!1228 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !267, size: 64)
!1229 = !DILocation(line: 174, column: 22, scope: !1223)
!1230 = !DILocation(line: 174, column: 26, scope: !1223)
!1231 = !DILocation(line: 174, column: 29, scope: !1223)
!1232 = !DILocation(line: 174, column: 31, scope: !1223)
!1233 = !DILocation(line: 175, column: 13, scope: !1234)
!1234 = distinct !DILexicalBlock(scope: !1223, file: !265, line: 175, column: 6)
!1235 = !DILocation(line: 175, column: 11, scope: !1234)
!1236 = !DILocation(line: 175, column: 18, scope: !1237)
!1237 = distinct !DILexicalBlock(scope: !1234, file: !265, line: 175, column: 6)
!1238 = !DILocation(line: 175, column: 20, scope: !1237)
!1239 = !DILocation(line: 175, column: 6, scope: !1234)
!1240 = !DILocalVariable(name: "p1", scope: !1241, file: !265, line: 176, type: !257)
!1241 = distinct !DILexicalBlock(scope: !1237, file: !265, line: 175, column: 30)
!1242 = !DILocation(line: 176, column: 32, scope: !1241)
!1243 = !DILocation(line: 176, column: 55, scope: !1241)
!1244 = !DILocation(line: 176, column: 57, scope: !1241)
!1245 = !DILocation(line: 176, column: 37, scope: !1241)
!1246 = !DILocalVariable(name: "p2", scope: !1241, file: !265, line: 177, type: !256)
!1247 = !DILocation(line: 177, column: 26, scope: !1241)
!1248 = !DILocation(line: 177, column: 51, scope: !1241)
!1249 = !DILocation(line: 177, column: 43, scope: !1241)
!1250 = !DILocation(line: 177, column: 31, scope: !1241)
!1251 = !DILocation(line: 178, column: 25, scope: !1241)
!1252 = !DILocation(line: 178, column: 17, scope: !1241)
!1253 = !DILocation(line: 178, column: 23, scope: !1241)
!1254 = !DILocation(line: 179, column: 25, scope: !1241)
!1255 = !DILocation(line: 179, column: 17, scope: !1241)
!1256 = !DILocation(line: 179, column: 23, scope: !1241)
!1257 = !DILocation(line: 180, column: 25, scope: !1241)
!1258 = !DILocation(line: 180, column: 17, scope: !1241)
!1259 = !DILocation(line: 180, column: 23, scope: !1241)
!1260 = !DILocation(line: 181, column: 25, scope: !1241)
!1261 = !DILocation(line: 181, column: 17, scope: !1241)
!1262 = !DILocation(line: 181, column: 23, scope: !1241)
!1263 = !DILocation(line: 182, column: 6, scope: !1241)
!1264 = !DILocation(line: 175, column: 27, scope: !1237)
!1265 = !DILocation(line: 175, column: 6, scope: !1237)
!1266 = distinct !{!1266, !1239, !1267}
!1267 = !DILocation(line: 182, column: 6, scope: !1234)
!1268 = !DILocation(line: 183, column: 26, scope: !1223)
!1269 = !DILocation(line: 183, column: 29, scope: !1223)
!1270 = !DILocation(line: 183, column: 6, scope: !1223)
!1271 = !DILocation(line: 184, column: 13, scope: !1223)
!1272 = !DILocation(line: 185, column: 2, scope: !1223)
!1273 = distinct !{!1273, !1187, !1274}
!1274 = !DILocation(line: 186, column: 5, scope: !1149)
!1275 = !DILocation(line: 187, column: 1, scope: !1149)
!1276 = distinct !DISubprogram(name: "mavlink_sha256_final_48", scope: !265, file: !265, line: 192, type: !1277, scopeLine: 193, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!1277 = !DISubroutineType(types: !1278)
!1278 = !{null, !1115, !256}
!1279 = !DILocalVariable(name: "m", arg: 1, scope: !1276, file: !265, line: 192, type: !1115)
!1280 = !DILocation(line: 192, column: 65, scope: !1276)
!1281 = !DILocalVariable(name: "result", arg: 2, scope: !1276, file: !265, line: 192, type: !256)
!1282 = !DILocation(line: 192, column: 76, scope: !1276)
!1283 = !DILocalVariable(name: "zeros", scope: !1276, file: !265, line: 194, type: !1284)
!1284 = !DICompositeType(tag: DW_TAG_array_type, baseType: !16, size: 576, elements: !1285)
!1285 = !{!1286}
!1286 = !DISubrange(count: 72)
!1287 = !DILocation(line: 194, column: 19, scope: !1276)
!1288 = !DILocalVariable(name: "offset", scope: !1276, file: !265, line: 195, type: !22)
!1289 = !DILocation(line: 195, column: 14, scope: !1276)
!1290 = !DILocation(line: 195, column: 24, scope: !1276)
!1291 = !DILocation(line: 195, column: 27, scope: !1276)
!1292 = !DILocation(line: 195, column: 33, scope: !1276)
!1293 = !DILocation(line: 195, column: 38, scope: !1276)
!1294 = !DILocalVariable(name: "dstart", scope: !1276, file: !265, line: 196, type: !22)
!1295 = !DILocation(line: 196, column: 18, scope: !1276)
!1296 = !DILocation(line: 196, column: 34, scope: !1276)
!1297 = !DILocation(line: 196, column: 32, scope: !1276)
!1298 = !DILocation(line: 196, column: 41, scope: !1276)
!1299 = !DILocation(line: 196, column: 46, scope: !1276)
!1300 = !DILocation(line: 196, column: 51, scope: !1276)
!1301 = !DILocalVariable(name: "p", scope: !1276, file: !265, line: 197, type: !256)
!1302 = !DILocation(line: 197, column: 14, scope: !1276)
!1303 = !DILocation(line: 197, column: 30, scope: !1276)
!1304 = !DILocation(line: 197, column: 33, scope: !1276)
!1305 = !DILocation(line: 197, column: 18, scope: !1276)
!1306 = !DILocation(line: 199, column: 5, scope: !1276)
!1307 = !DILocation(line: 199, column: 12, scope: !1276)
!1308 = !DILocation(line: 200, column: 13, scope: !1276)
!1309 = !DILocation(line: 200, column: 19, scope: !1276)
!1310 = !DILocation(line: 200, column: 5, scope: !1276)
!1311 = !DILocation(line: 201, column: 24, scope: !1276)
!1312 = !DILocation(line: 201, column: 27, scope: !1276)
!1313 = !DILocation(line: 201, column: 33, scope: !1276)
!1314 = !DILocation(line: 201, column: 39, scope: !1276)
!1315 = !DILocation(line: 201, column: 23, scope: !1276)
!1316 = !DILocation(line: 201, column: 11, scope: !1276)
!1317 = !DILocation(line: 201, column: 17, scope: !1276)
!1318 = !DILocation(line: 201, column: 5, scope: !1276)
!1319 = !DILocation(line: 201, column: 21, scope: !1276)
!1320 = !DILocation(line: 202, column: 24, scope: !1276)
!1321 = !DILocation(line: 202, column: 27, scope: !1276)
!1322 = !DILocation(line: 202, column: 33, scope: !1276)
!1323 = !DILocation(line: 202, column: 39, scope: !1276)
!1324 = !DILocation(line: 202, column: 23, scope: !1276)
!1325 = !DILocation(line: 202, column: 11, scope: !1276)
!1326 = !DILocation(line: 202, column: 17, scope: !1276)
!1327 = !DILocation(line: 202, column: 5, scope: !1276)
!1328 = !DILocation(line: 202, column: 21, scope: !1276)
!1329 = !DILocation(line: 203, column: 24, scope: !1276)
!1330 = !DILocation(line: 203, column: 27, scope: !1276)
!1331 = !DILocation(line: 203, column: 33, scope: !1276)
!1332 = !DILocation(line: 203, column: 40, scope: !1276)
!1333 = !DILocation(line: 203, column: 23, scope: !1276)
!1334 = !DILocation(line: 203, column: 11, scope: !1276)
!1335 = !DILocation(line: 203, column: 17, scope: !1276)
!1336 = !DILocation(line: 203, column: 5, scope: !1276)
!1337 = !DILocation(line: 203, column: 21, scope: !1276)
!1338 = !DILocation(line: 204, column: 24, scope: !1276)
!1339 = !DILocation(line: 204, column: 27, scope: !1276)
!1340 = !DILocation(line: 204, column: 33, scope: !1276)
!1341 = !DILocation(line: 204, column: 40, scope: !1276)
!1342 = !DILocation(line: 204, column: 23, scope: !1276)
!1343 = !DILocation(line: 204, column: 11, scope: !1276)
!1344 = !DILocation(line: 204, column: 17, scope: !1276)
!1345 = !DILocation(line: 204, column: 5, scope: !1276)
!1346 = !DILocation(line: 204, column: 21, scope: !1276)
!1347 = !DILocation(line: 205, column: 24, scope: !1276)
!1348 = !DILocation(line: 205, column: 27, scope: !1276)
!1349 = !DILocation(line: 205, column: 33, scope: !1276)
!1350 = !DILocation(line: 205, column: 39, scope: !1276)
!1351 = !DILocation(line: 205, column: 23, scope: !1276)
!1352 = !DILocation(line: 205, column: 11, scope: !1276)
!1353 = !DILocation(line: 205, column: 17, scope: !1276)
!1354 = !DILocation(line: 205, column: 5, scope: !1276)
!1355 = !DILocation(line: 205, column: 21, scope: !1276)
!1356 = !DILocation(line: 206, column: 24, scope: !1276)
!1357 = !DILocation(line: 206, column: 27, scope: !1276)
!1358 = !DILocation(line: 206, column: 33, scope: !1276)
!1359 = !DILocation(line: 206, column: 39, scope: !1276)
!1360 = !DILocation(line: 206, column: 23, scope: !1276)
!1361 = !DILocation(line: 206, column: 11, scope: !1276)
!1362 = !DILocation(line: 206, column: 17, scope: !1276)
!1363 = !DILocation(line: 206, column: 5, scope: !1276)
!1364 = !DILocation(line: 206, column: 21, scope: !1276)
!1365 = !DILocation(line: 207, column: 24, scope: !1276)
!1366 = !DILocation(line: 207, column: 27, scope: !1276)
!1367 = !DILocation(line: 207, column: 33, scope: !1276)
!1368 = !DILocation(line: 207, column: 40, scope: !1276)
!1369 = !DILocation(line: 207, column: 23, scope: !1276)
!1370 = !DILocation(line: 207, column: 11, scope: !1276)
!1371 = !DILocation(line: 207, column: 17, scope: !1276)
!1372 = !DILocation(line: 207, column: 5, scope: !1276)
!1373 = !DILocation(line: 207, column: 21, scope: !1276)
!1374 = !DILocation(line: 208, column: 24, scope: !1276)
!1375 = !DILocation(line: 208, column: 27, scope: !1276)
!1376 = !DILocation(line: 208, column: 33, scope: !1276)
!1377 = !DILocation(line: 208, column: 40, scope: !1276)
!1378 = !DILocation(line: 208, column: 23, scope: !1276)
!1379 = !DILocation(line: 208, column: 11, scope: !1276)
!1380 = !DILocation(line: 208, column: 17, scope: !1276)
!1381 = !DILocation(line: 208, column: 5, scope: !1276)
!1382 = !DILocation(line: 208, column: 21, scope: !1276)
!1383 = !DILocation(line: 210, column: 27, scope: !1276)
!1384 = !DILocation(line: 210, column: 30, scope: !1276)
!1385 = !DILocation(line: 210, column: 37, scope: !1276)
!1386 = !DILocation(line: 210, column: 44, scope: !1276)
!1387 = !DILocation(line: 210, column: 5, scope: !1276)
!1388 = !DILocation(line: 215, column: 17, scope: !1276)
!1389 = !DILocation(line: 215, column: 5, scope: !1276)
!1390 = !DILocation(line: 215, column: 15, scope: !1276)
!1391 = !DILocation(line: 216, column: 17, scope: !1276)
!1392 = !DILocation(line: 216, column: 5, scope: !1276)
!1393 = !DILocation(line: 216, column: 15, scope: !1276)
!1394 = !DILocation(line: 217, column: 17, scope: !1276)
!1395 = !DILocation(line: 217, column: 5, scope: !1276)
!1396 = !DILocation(line: 217, column: 15, scope: !1276)
!1397 = !DILocation(line: 218, column: 17, scope: !1276)
!1398 = !DILocation(line: 218, column: 5, scope: !1276)
!1399 = !DILocation(line: 218, column: 15, scope: !1276)
!1400 = !DILocation(line: 219, column: 17, scope: !1276)
!1401 = !DILocation(line: 219, column: 5, scope: !1276)
!1402 = !DILocation(line: 219, column: 15, scope: !1276)
!1403 = !DILocation(line: 220, column: 17, scope: !1276)
!1404 = !DILocation(line: 220, column: 5, scope: !1276)
!1405 = !DILocation(line: 220, column: 15, scope: !1276)
!1406 = !DILocation(line: 221, column: 1, scope: !1276)
!1407 = distinct !DISubprogram(name: "mavlink_sha256_calc", scope: !265, file: !265, line: 105, type: !1408, scopeLine: 106, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !92, retainedNodes: !270)
!1408 = !DISubroutineType(types: !1409)
!1409 = !{null, !1115, !1410}
!1410 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !72, size: 64)
!1411 = !DILocalVariable(name: "m", arg: 1, scope: !1407, file: !265, line: 105, type: !1115)
!1412 = !DILocation(line: 105, column: 60, scope: !1407)
!1413 = !DILocalVariable(name: "in", arg: 2, scope: !1407, file: !265, line: 105, type: !1410)
!1414 = !DILocation(line: 105, column: 73, scope: !1407)
!1415 = !DILocalVariable(name: "AA", scope: !1407, file: !265, line: 107, type: !72)
!1416 = !DILocation(line: 107, column: 14, scope: !1407)
!1417 = !DILocalVariable(name: "BB", scope: !1407, file: !265, line: 107, type: !72)
!1418 = !DILocation(line: 107, column: 18, scope: !1407)
!1419 = !DILocalVariable(name: "CC", scope: !1407, file: !265, line: 107, type: !72)
!1420 = !DILocation(line: 107, column: 22, scope: !1407)
!1421 = !DILocalVariable(name: "DD", scope: !1407, file: !265, line: 107, type: !72)
!1422 = !DILocation(line: 107, column: 26, scope: !1407)
!1423 = !DILocalVariable(name: "EE", scope: !1407, file: !265, line: 107, type: !72)
!1424 = !DILocation(line: 107, column: 30, scope: !1407)
!1425 = !DILocalVariable(name: "FF", scope: !1407, file: !265, line: 107, type: !72)
!1426 = !DILocation(line: 107, column: 34, scope: !1407)
!1427 = !DILocalVariable(name: "GG", scope: !1407, file: !265, line: 107, type: !72)
!1428 = !DILocation(line: 107, column: 38, scope: !1407)
!1429 = !DILocalVariable(name: "HH", scope: !1407, file: !265, line: 107, type: !72)
!1430 = !DILocation(line: 107, column: 42, scope: !1407)
!1431 = !DILocalVariable(name: "data", scope: !1407, file: !265, line: 108, type: !1432)
!1432 = !DICompositeType(tag: DW_TAG_array_type, baseType: !72, size: 2048, elements: !268)
!1433 = !DILocation(line: 108, column: 14, scope: !1407)
!1434 = !DILocalVariable(name: "i", scope: !1407, file: !265, line: 109, type: !279)
!1435 = !DILocation(line: 109, column: 9, scope: !1407)
!1436 = !DILocation(line: 111, column: 10, scope: !1407)
!1437 = !DILocation(line: 111, column: 13, scope: !1407)
!1438 = !DILocation(line: 111, column: 8, scope: !1407)
!1439 = !DILocation(line: 112, column: 10, scope: !1407)
!1440 = !DILocation(line: 112, column: 13, scope: !1407)
!1441 = !DILocation(line: 112, column: 8, scope: !1407)
!1442 = !DILocation(line: 113, column: 10, scope: !1407)
!1443 = !DILocation(line: 113, column: 13, scope: !1407)
!1444 = !DILocation(line: 113, column: 8, scope: !1407)
!1445 = !DILocation(line: 114, column: 10, scope: !1407)
!1446 = !DILocation(line: 114, column: 13, scope: !1407)
!1447 = !DILocation(line: 114, column: 8, scope: !1407)
!1448 = !DILocation(line: 115, column: 10, scope: !1407)
!1449 = !DILocation(line: 115, column: 13, scope: !1407)
!1450 = !DILocation(line: 115, column: 8, scope: !1407)
!1451 = !DILocation(line: 116, column: 10, scope: !1407)
!1452 = !DILocation(line: 116, column: 13, scope: !1407)
!1453 = !DILocation(line: 116, column: 8, scope: !1407)
!1454 = !DILocation(line: 117, column: 10, scope: !1407)
!1455 = !DILocation(line: 117, column: 13, scope: !1407)
!1456 = !DILocation(line: 117, column: 8, scope: !1407)
!1457 = !DILocation(line: 118, column: 10, scope: !1407)
!1458 = !DILocation(line: 118, column: 13, scope: !1407)
!1459 = !DILocation(line: 118, column: 8, scope: !1407)
!1460 = !DILocation(line: 120, column: 12, scope: !1461)
!1461 = distinct !DILexicalBlock(scope: !1407, file: !265, line: 120, column: 5)
!1462 = !DILocation(line: 120, column: 10, scope: !1461)
!1463 = !DILocation(line: 120, column: 17, scope: !1464)
!1464 = distinct !DILexicalBlock(scope: !1461, file: !265, line: 120, column: 5)
!1465 = !DILocation(line: 120, column: 19, scope: !1464)
!1466 = !DILocation(line: 120, column: 5, scope: !1461)
!1467 = !DILocation(line: 121, column: 12, scope: !1464)
!1468 = !DILocation(line: 121, column: 15, scope: !1464)
!1469 = !DILocation(line: 121, column: 7, scope: !1464)
!1470 = !DILocation(line: 121, column: 2, scope: !1464)
!1471 = !DILocation(line: 121, column: 10, scope: !1464)
!1472 = !DILocation(line: 120, column: 25, scope: !1464)
!1473 = !DILocation(line: 120, column: 5, scope: !1464)
!1474 = distinct !{!1474, !1466, !1475}
!1475 = !DILocation(line: 121, column: 16, scope: !1461)
!1476 = !DILocation(line: 122, column: 12, scope: !1477)
!1477 = distinct !DILexicalBlock(scope: !1407, file: !265, line: 122, column: 5)
!1478 = !DILocation(line: 122, column: 10, scope: !1477)
!1479 = !DILocation(line: 122, column: 18, scope: !1480)
!1480 = distinct !DILexicalBlock(scope: !1477, file: !265, line: 122, column: 5)
!1481 = !DILocation(line: 122, column: 20, scope: !1480)
!1482 = !DILocation(line: 122, column: 5, scope: !1477)
!1483 = !DILocation(line: 123, column: 12, scope: !1480)
!1484 = !DILocation(line: 123, column: 37, scope: !1480)
!1485 = !DILocation(line: 123, column: 38, scope: !1480)
!1486 = !DILocation(line: 123, column: 32, scope: !1480)
!1487 = !DILocation(line: 123, column: 30, scope: !1480)
!1488 = !DILocation(line: 124, column: 6, scope: !1480)
!1489 = !DILocation(line: 123, column: 42, scope: !1480)
!1490 = !DILocation(line: 124, column: 32, scope: !1480)
!1491 = !DILocation(line: 124, column: 34, scope: !1480)
!1492 = !DILocation(line: 124, column: 27, scope: !1480)
!1493 = !DILocation(line: 124, column: 25, scope: !1480)
!1494 = !DILocation(line: 123, column: 7, scope: !1480)
!1495 = !DILocation(line: 123, column: 2, scope: !1480)
!1496 = !DILocation(line: 123, column: 10, scope: !1480)
!1497 = !DILocation(line: 122, column: 26, scope: !1480)
!1498 = !DILocation(line: 122, column: 5, scope: !1480)
!1499 = distinct !{!1499, !1482, !1500}
!1500 = !DILocation(line: 124, column: 38, scope: !1477)
!1501 = !DILocation(line: 126, column: 12, scope: !1502)
!1502 = distinct !DILexicalBlock(scope: !1407, file: !265, line: 126, column: 5)
!1503 = !DILocation(line: 126, column: 10, scope: !1502)
!1504 = !DILocation(line: 126, column: 17, scope: !1505)
!1505 = distinct !DILexicalBlock(scope: !1502, file: !265, line: 126, column: 5)
!1506 = !DILocation(line: 126, column: 19, scope: !1505)
!1507 = !DILocation(line: 126, column: 5, scope: !1502)
!1508 = !DILocalVariable(name: "T1", scope: !1509, file: !265, line: 127, type: !72)
!1509 = distinct !DILexicalBlock(scope: !1505, file: !265, line: 126, column: 30)
!1510 = !DILocation(line: 127, column: 11, scope: !1509)
!1511 = !DILocalVariable(name: "T2", scope: !1509, file: !265, line: 127, type: !72)
!1512 = !DILocation(line: 127, column: 15, scope: !1509)
!1513 = !DILocation(line: 129, column: 7, scope: !1509)
!1514 = !DILocation(line: 129, column: 12, scope: !1509)
!1515 = !DILocation(line: 129, column: 10, scope: !1509)
!1516 = !DILocation(line: 129, column: 25, scope: !1509)
!1517 = !DILocation(line: 129, column: 23, scope: !1509)
!1518 = !DILocation(line: 129, column: 70, scope: !1509)
!1519 = !DILocation(line: 129, column: 42, scope: !1509)
!1520 = !DILocation(line: 129, column: 40, scope: !1509)
!1521 = !DILocation(line: 129, column: 80, scope: !1509)
!1522 = !DILocation(line: 129, column: 75, scope: !1509)
!1523 = !DILocation(line: 129, column: 73, scope: !1509)
!1524 = !DILocation(line: 129, column: 5, scope: !1509)
!1525 = !DILocation(line: 130, column: 7, scope: !1509)
!1526 = !DILocation(line: 130, column: 20, scope: !1509)
!1527 = !DILocation(line: 130, column: 18, scope: !1509)
!1528 = !DILocation(line: 130, column: 5, scope: !1509)
!1529 = !DILocation(line: 132, column: 7, scope: !1509)
!1530 = !DILocation(line: 132, column: 5, scope: !1509)
!1531 = !DILocation(line: 133, column: 7, scope: !1509)
!1532 = !DILocation(line: 133, column: 5, scope: !1509)
!1533 = !DILocation(line: 134, column: 7, scope: !1509)
!1534 = !DILocation(line: 134, column: 5, scope: !1509)
!1535 = !DILocation(line: 135, column: 7, scope: !1509)
!1536 = !DILocation(line: 135, column: 12, scope: !1509)
!1537 = !DILocation(line: 135, column: 10, scope: !1509)
!1538 = !DILocation(line: 135, column: 5, scope: !1509)
!1539 = !DILocation(line: 136, column: 7, scope: !1509)
!1540 = !DILocation(line: 136, column: 5, scope: !1509)
!1541 = !DILocation(line: 137, column: 7, scope: !1509)
!1542 = !DILocation(line: 137, column: 5, scope: !1509)
!1543 = !DILocation(line: 138, column: 7, scope: !1509)
!1544 = !DILocation(line: 138, column: 5, scope: !1509)
!1545 = !DILocation(line: 139, column: 7, scope: !1509)
!1546 = !DILocation(line: 139, column: 12, scope: !1509)
!1547 = !DILocation(line: 139, column: 10, scope: !1509)
!1548 = !DILocation(line: 139, column: 5, scope: !1509)
!1549 = !DILocation(line: 140, column: 5, scope: !1509)
!1550 = !DILocation(line: 126, column: 26, scope: !1505)
!1551 = !DILocation(line: 126, column: 5, scope: !1505)
!1552 = distinct !{!1552, !1507, !1553}
!1553 = !DILocation(line: 140, column: 5, scope: !1502)
!1554 = !DILocation(line: 142, column: 22, scope: !1407)
!1555 = !DILocation(line: 142, column: 5, scope: !1407)
!1556 = !DILocation(line: 142, column: 8, scope: !1407)
!1557 = !DILocation(line: 142, column: 19, scope: !1407)
!1558 = !DILocation(line: 143, column: 22, scope: !1407)
!1559 = !DILocation(line: 143, column: 5, scope: !1407)
!1560 = !DILocation(line: 143, column: 8, scope: !1407)
!1561 = !DILocation(line: 143, column: 19, scope: !1407)
!1562 = !DILocation(line: 144, column: 22, scope: !1407)
!1563 = !DILocation(line: 144, column: 5, scope: !1407)
!1564 = !DILocation(line: 144, column: 8, scope: !1407)
!1565 = !DILocation(line: 144, column: 19, scope: !1407)
!1566 = !DILocation(line: 145, column: 22, scope: !1407)
!1567 = !DILocation(line: 145, column: 5, scope: !1407)
!1568 = !DILocation(line: 145, column: 8, scope: !1407)
!1569 = !DILocation(line: 145, column: 19, scope: !1407)
!1570 = !DILocation(line: 146, column: 22, scope: !1407)
!1571 = !DILocation(line: 146, column: 5, scope: !1407)
!1572 = !DILocation(line: 146, column: 8, scope: !1407)
!1573 = !DILocation(line: 146, column: 19, scope: !1407)
!1574 = !DILocation(line: 147, column: 22, scope: !1407)
!1575 = !DILocation(line: 147, column: 5, scope: !1407)
!1576 = !DILocation(line: 147, column: 8, scope: !1407)
!1577 = !DILocation(line: 147, column: 19, scope: !1407)
!1578 = !DILocation(line: 148, column: 22, scope: !1407)
!1579 = !DILocation(line: 148, column: 5, scope: !1407)
!1580 = !DILocation(line: 148, column: 8, scope: !1407)
!1581 = !DILocation(line: 148, column: 19, scope: !1407)
!1582 = !DILocation(line: 149, column: 22, scope: !1407)
!1583 = !DILocation(line: 149, column: 5, scope: !1407)
!1584 = !DILocation(line: 149, column: 8, scope: !1407)
!1585 = !DILocation(line: 149, column: 19, scope: !1407)
!1586 = !DILocation(line: 150, column: 1, scope: !1407)
