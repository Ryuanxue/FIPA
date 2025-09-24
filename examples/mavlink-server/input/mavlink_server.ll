; ModuleID = 'mavlink_server.c'
source_filename = "mavlink_server.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.__mavlink_msg_entry = type { i32, i8, i8, i8, i8, i8, i8 }
%struct.__mavlink_message = type <{ i16, i8, i8, i8, i8, i8, i8, i8, [3 x i8], [33 x i64], [2 x i8], [13 x i8] }>
%struct.__mavlink_status = type { i8, i8, i8, i32, i8, i8, i8, i16, i16, i8, i8, %struct.__mavlink_signing*, %struct.__mavlink_signing_streams* }
%struct.__mavlink_signing = type { i8, i8, i64, [32 x i8], i1 (%struct.__mavlink_status*, i32)* }
%struct.__mavlink_signing_streams = type { i16, [16 x %struct.__mavlink_signing_stream] }
%struct.__mavlink_signing_stream = type { i8, i8, i8, [6 x i8] }
%struct.sockaddr_in = type { i16, i16, %struct.in_addr, [8 x i8] }
%struct.in_addr = type { i32 }
%struct.__mavlink_heartbeat_t = type { i32, i8, i8, i8, i8, i8 }
%struct.sockaddr = type { i16, [14 x i8] }
%struct.mavlink_sha256_ctx = type { [2 x i32], [8 x i32], %union.anon }
%union.anon = type { [16 x i32] }
%union.tstamp = type { i64 }

@.str = private unnamed_addr constant [83 x i8] c"Received heartbeat: sysid=%d, compid=%d, type=%d, autopilot=%d, mode=%d, state=%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [21 x i8] c"Error receiving data\00", align 1
@.str.2 = private unnamed_addr constant [22 x i8] c"Error creating socket\00", align 1
@.str.3 = private unnamed_addr constant [21 x i8] c"Error binding socket\00", align 1
@.str.4 = private unnamed_addr constant [32 x i8] c"Server listening on port %d...\0A\00", align 1
@mavlink_get_msg_entry.mavlink_message_crcs = internal constant [196 x %struct.__mavlink_msg_entry] [%struct.__mavlink_msg_entry { i32 0, i8 50, i8 9, i8 9, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 1, i8 124, i8 31, i8 31, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 2, i8 -119, i8 12, i8 12, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 4, i8 -19, i8 14, i8 14, i8 3, i8 12, i8 13 }, %struct.__mavlink_msg_entry { i32 5, i8 -39, i8 28, i8 28, i8 1, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 6, i8 104, i8 3, i8 3, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 7, i8 119, i8 32, i8 32, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 11, i8 89, i8 6, i8 6, i8 1, i8 4, i8 0 }, %struct.__mavlink_msg_entry { i32 20, i8 -42, i8 20, i8 20, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 21, i8 -97, i8 2, i8 2, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 22, i8 -36, i8 25, i8 25, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 23, i8 -88, i8 23, i8 23, i8 3, i8 4, i8 5 }, %struct.__mavlink_msg_entry { i32 24, i8 24, i8 30, i8 52, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 25, i8 23, i8 101, i8 101, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 26, i8 -86, i8 22, i8 24, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 27, i8 -112, i8 26, i8 29, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 28, i8 67, i8 16, i8 16, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 29, i8 115, i8 14, i8 16, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 30, i8 39, i8 28, i8 28, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 31, i8 -10, i8 32, i8 48, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 32, i8 -71, i8 28, i8 28, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 33, i8 104, i8 28, i8 28, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 34, i8 -19, i8 22, i8 22, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 35, i8 -12, i8 22, i8 22, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 36, i8 -34, i8 21, i8 37, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 37, i8 -44, i8 6, i8 7, i8 3, i8 4, i8 5 }, %struct.__mavlink_msg_entry { i32 38, i8 9, i8 6, i8 7, i8 3, i8 4, i8 5 }, %struct.__mavlink_msg_entry { i32 39, i8 -2, i8 37, i8 38, i8 3, i8 32, i8 33 }, %struct.__mavlink_msg_entry { i32 40, i8 -26, i8 4, i8 5, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 41, i8 28, i8 4, i8 4, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 42, i8 28, i8 2, i8 2, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 43, i8 -124, i8 2, i8 3, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 44, i8 -35, i8 4, i8 5, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 45, i8 -24, i8 2, i8 3, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 46, i8 11, i8 2, i8 2, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 47, i8 -103, i8 3, i8 4, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 48, i8 41, i8 13, i8 21, i8 1, i8 12, i8 0 }, %struct.__mavlink_msg_entry { i32 49, i8 39, i8 12, i8 20, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 50, i8 78, i8 37, i8 37, i8 3, i8 18, i8 19 }, %struct.__mavlink_msg_entry { i32 51, i8 -60, i8 4, i8 5, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 54, i8 15, i8 27, i8 27, i8 3, i8 24, i8 25 }, %struct.__mavlink_msg_entry { i32 55, i8 3, i8 25, i8 25, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 61, i8 -89, i8 72, i8 72, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 62, i8 -73, i8 26, i8 26, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 63, i8 119, i8 -75, i8 -75, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 64, i8 -65, i8 -31, i8 -31, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 65, i8 118, i8 42, i8 42, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 66, i8 -108, i8 6, i8 6, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 67, i8 21, i8 4, i8 4, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 69, i8 -13, i8 11, i8 11, i8 1, i8 10, i8 0 }, %struct.__mavlink_msg_entry { i32 70, i8 124, i8 18, i8 38, i8 3, i8 16, i8 17 }, %struct.__mavlink_msg_entry { i32 73, i8 38, i8 37, i8 38, i8 3, i8 32, i8 33 }, %struct.__mavlink_msg_entry { i32 74, i8 20, i8 20, i8 20, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 75, i8 -98, i8 35, i8 35, i8 3, i8 30, i8 31 }, %struct.__mavlink_msg_entry { i32 76, i8 -104, i8 33, i8 33, i8 3, i8 30, i8 31 }, %struct.__mavlink_msg_entry { i32 77, i8 -113, i8 3, i8 10, i8 3, i8 8, i8 9 }, %struct.__mavlink_msg_entry { i32 81, i8 106, i8 22, i8 22, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 82, i8 49, i8 39, i8 39, i8 3, i8 36, i8 37 }, %struct.__mavlink_msg_entry { i32 83, i8 22, i8 37, i8 37, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 84, i8 -113, i8 53, i8 53, i8 3, i8 50, i8 51 }, %struct.__mavlink_msg_entry { i32 85, i8 -116, i8 51, i8 51, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 86, i8 5, i8 53, i8 53, i8 3, i8 50, i8 51 }, %struct.__mavlink_msg_entry { i32 87, i8 -106, i8 51, i8 51, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 89, i8 -25, i8 28, i8 28, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 90, i8 -73, i8 56, i8 56, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 91, i8 63, i8 42, i8 42, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 92, i8 54, i8 33, i8 33, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 93, i8 47, i8 81, i8 81, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 100, i8 -81, i8 26, i8 34, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 101, i8 102, i8 32, i8 117, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 102, i8 -98, i8 32, i8 117, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 103, i8 -48, i8 20, i8 57, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 104, i8 56, i8 32, i8 116, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 105, i8 93, i8 62, i8 63, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 106, i8 -118, i8 44, i8 44, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 107, i8 108, i8 64, i8 65, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 108, i8 32, i8 84, i8 84, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 109, i8 -71, i8 9, i8 9, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 110, i8 84, i8 -2, i8 -2, i8 3, i8 1, i8 2 }, %struct.__mavlink_msg_entry { i32 111, i8 34, i8 16, i8 16, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 112, i8 -82, i8 12, i8 12, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 113, i8 124, i8 36, i8 39, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 114, i8 -19, i8 44, i8 44, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 115, i8 4, i8 64, i8 64, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 116, i8 76, i8 22, i8 24, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 117, i8 -128, i8 6, i8 6, i8 3, i8 4, i8 5 }, %struct.__mavlink_msg_entry { i32 118, i8 56, i8 14, i8 14, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 119, i8 116, i8 12, i8 12, i8 3, i8 10, i8 11 }, %struct.__mavlink_msg_entry { i32 120, i8 -122, i8 97, i8 97, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 121, i8 -19, i8 2, i8 2, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 122, i8 -53, i8 2, i8 2, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 123, i8 -6, i8 113, i8 113, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 124, i8 87, i8 35, i8 57, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 125, i8 -53, i8 6, i8 6, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 126, i8 -36, i8 79, i8 79, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 127, i8 25, i8 35, i8 35, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 128, i8 -30, i8 35, i8 35, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 129, i8 46, i8 22, i8 24, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 130, i8 29, i8 13, i8 13, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 131, i8 -33, i8 -1, i8 -1, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 132, i8 85, i8 14, i8 39, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 133, i8 6, i8 18, i8 18, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 134, i8 -27, i8 43, i8 43, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 135, i8 -53, i8 8, i8 8, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 136, i8 1, i8 22, i8 22, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 137, i8 -61, i8 14, i8 16, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 138, i8 109, i8 36, i8 120, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 139, i8 -88, i8 43, i8 43, i8 3, i8 41, i8 42 }, %struct.__mavlink_msg_entry { i32 140, i8 -75, i8 41, i8 41, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 141, i8 47, i8 32, i8 32, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 142, i8 72, i8 -13, i8 -13, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 143, i8 -125, i8 14, i8 16, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 144, i8 127, i8 93, i8 93, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 146, i8 103, i8 100, i8 100, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 147, i8 -102, i8 36, i8 54, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 148, i8 -78, i8 60, i8 78, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 149, i8 -56, i8 30, i8 60, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 162, i8 -67, i8 8, i8 9, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 192, i8 36, i8 44, i8 54, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 225, i8 -48, i8 65, i8 73, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 230, i8 -93, i8 42, i8 42, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 231, i8 105, i8 40, i8 40, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 232, i8 -105, i8 63, i8 65, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 233, i8 35, i8 -74, i8 -74, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 234, i8 -106, i8 40, i8 40, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 235, i8 -77, i8 42, i8 42, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 241, i8 90, i8 32, i8 32, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 242, i8 104, i8 52, i8 60, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 243, i8 85, i8 53, i8 61, i8 1, i8 52, i8 0 }, %struct.__mavlink_msg_entry { i32 244, i8 95, i8 6, i8 6, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 245, i8 -126, i8 2, i8 2, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 246, i8 -72, i8 38, i8 38, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 247, i8 81, i8 19, i8 19, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 248, i8 8, i8 -2, i8 -2, i8 3, i8 3, i8 4 }, %struct.__mavlink_msg_entry { i32 249, i8 -52, i8 36, i8 36, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 250, i8 49, i8 30, i8 30, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 251, i8 -86, i8 18, i8 18, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 252, i8 44, i8 18, i8 18, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 253, i8 83, i8 51, i8 54, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 254, i8 46, i8 9, i8 9, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 256, i8 71, i8 42, i8 42, i8 3, i8 8, i8 9 }, %struct.__mavlink_msg_entry { i32 257, i8 -125, i8 9, i8 9, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 258, i8 -69, i8 32, i8 -24, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 259, i8 92, i8 -21, i8 -21, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 260, i8 -110, i8 5, i8 13, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 261, i8 -77, i8 27, i8 60, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 262, i8 12, i8 18, i8 22, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 263, i8 -123, i8 -1, i8 -1, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 264, i8 49, i8 28, i8 28, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 265, i8 26, i8 16, i8 20, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 266, i8 -63, i8 -1, i8 -1, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 267, i8 35, i8 -1, i8 -1, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 268, i8 14, i8 4, i8 4, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 269, i8 109, i8 -43, i8 -43, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 270, i8 59, i8 19, i8 19, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 271, i8 22, i8 52, i8 52, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 275, i8 126, i8 31, i8 31, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 276, i8 18, i8 49, i8 49, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 283, i8 74, i8 -112, i8 -112, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 284, i8 99, i8 32, i8 32, i8 3, i8 30, i8 31 }, %struct.__mavlink_msg_entry { i32 285, i8 -119, i8 40, i8 40, i8 3, i8 38, i8 39 }, %struct.__mavlink_msg_entry { i32 286, i8 -46, i8 53, i8 53, i8 3, i8 50, i8 51 }, %struct.__mavlink_msg_entry { i32 299, i8 19, i8 96, i8 96, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 301, i8 -13, i8 58, i8 58, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 310, i8 28, i8 17, i8 17, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 311, i8 95, i8 116, i8 116, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 320, i8 -13, i8 20, i8 20, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 321, i8 88, i8 2, i8 2, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 322, i8 -13, i8 -107, i8 -107, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 323, i8 78, i8 -109, i8 -109, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 324, i8 -124, i8 -110, i8 -110, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 330, i8 23, i8 -98, i8 -89, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 331, i8 91, i8 -26, i8 -24, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 335, i8 -31, i8 24, i8 24, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 339, i8 -57, i8 5, i8 5, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 340, i8 99, i8 70, i8 70, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 350, i8 -24, i8 20, i8 -4, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 370, i8 75, i8 87, i8 109, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 373, i8 117, i8 42, i8 42, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 375, i8 -5, i8 -116, i8 -116, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 385, i8 -109, i8 -123, i8 -123, i8 3, i8 2, i8 3 }, %struct.__mavlink_msg_entry { i32 386, i8 -124, i8 16, i8 16, i8 3, i8 4, i8 5 }, %struct.__mavlink_msg_entry { i32 387, i8 4, i8 72, i8 72, i8 3, i8 4, i8 5 }, %struct.__mavlink_msg_entry { i32 388, i8 8, i8 37, i8 37, i8 3, i8 32, i8 33 }, %struct.__mavlink_msg_entry { i32 9000, i8 113, i8 -119, i8 -119, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 9005, i8 117, i8 34, i8 34, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 12900, i8 114, i8 44, i8 44, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 12901, i8 -2, i8 59, i8 59, i8 3, i8 30, i8 31 }, %struct.__mavlink_msg_entry { i32 12902, i8 -116, i8 53, i8 53, i8 3, i8 4, i8 5 }, %struct.__mavlink_msg_entry { i32 12903, i8 -7, i8 46, i8 46, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 12904, i8 77, i8 54, i8 54, i8 3, i8 28, i8 29 }, %struct.__mavlink_msg_entry { i32 12905, i8 49, i8 43, i8 43, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 12915, i8 94, i8 -7, i8 -7, i8 3, i8 0, i8 1 }, %struct.__mavlink_msg_entry { i32 12918, i8 -117, i8 51, i8 51, i8 0, i8 0, i8 0 }, %struct.__mavlink_msg_entry { i32 12919, i8 7, i8 18, i8 18, i8 3, i8 16, i8 17 }, %struct.__mavlink_msg_entry { i32 12920, i8 20, i8 5, i8 5, i8 0, i8 0, i8 0 }], align 16, !dbg !0
@mavlink_sha256_constant_256 = internal constant [64 x i32] [i32 1116352408, i32 1899447441, i32 -1245643825, i32 -373957723, i32 961987163, i32 1508970993, i32 -1841331548, i32 -1424204075, i32 -670586216, i32 310598401, i32 607225278, i32 1426881987, i32 1925078388, i32 -2132889090, i32 -1680079193, i32 -1046744716, i32 -459576895, i32 -272742522, i32 264347078, i32 604807628, i32 770255983, i32 1249150122, i32 1555081692, i32 1996064986, i32 -1740746414, i32 -1473132947, i32 -1341970488, i32 -1084653625, i32 -958395405, i32 -710438585, i32 113926993, i32 338241895, i32 666307205, i32 773529912, i32 1294757372, i32 1396182291, i32 1695183700, i32 1986661051, i32 -2117940946, i32 -1838011259, i32 -1564481375, i32 -1474664885, i32 -1035236496, i32 -949202525, i32 -778901479, i32 -694614492, i32 -200395387, i32 275423344, i32 430227734, i32 506948616, i32 659060556, i32 883997877, i32 958139571, i32 1322822218, i32 1537002063, i32 1747873779, i32 1955562222, i32 2024104815, i32 -2067236844, i32 -1933114872, i32 -1866530822, i32 -1538233109, i32 -1090935817, i32 -965641998], align 16, !dbg !126
@mavlink_get_channel_buffer.m_mavlink_buffer = internal global [16 x %struct.__mavlink_message] zeroinitializer, align 16, !dbg !133
@mavlink_get_channel_status.m_mavlink_status = internal global [16 x %struct.__mavlink_status] zeroinitializer, align 16, !dbg !172

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @receive_heartbeat(i32 %sock) #0 !dbg !236 {
entry:
  %sock.addr = alloca i32, align 4
  %buf = alloca [2041 x i8], align 16
  %src_addr = alloca %struct.sockaddr_in, align 4
  %addrlen = alloca i32, align 4
  %recsize = alloca i64, align 8
  %len = alloca i32, align 4
  %msg = alloca %struct.__mavlink_message, align 1
  %status = alloca %struct.__mavlink_status, align 8
  %i = alloca i32, align 4
  %heartbeat = alloca %struct.__mavlink_heartbeat_t, align 4
  store i32 %sock, i32* %sock.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %sock.addr, metadata !240, metadata !DIExpression()), !dbg !241
  call void @llvm.dbg.declare(metadata [2041 x i8]* %buf, metadata !242, metadata !DIExpression()), !dbg !246
  call void @llvm.dbg.declare(metadata %struct.sockaddr_in* %src_addr, metadata !247, metadata !DIExpression()), !dbg !261
  call void @llvm.dbg.declare(metadata i32* %addrlen, metadata !262, metadata !DIExpression()), !dbg !266
  store i32 16, i32* %addrlen, align 4, !dbg !266
  call void @llvm.dbg.declare(metadata i64* %recsize, metadata !267, metadata !DIExpression()), !dbg !272
  %0 = load i32, i32* %sock.addr, align 4, !dbg !273
  %arraydecay = getelementptr inbounds [2041 x i8], [2041 x i8]* %buf, i64 0, i64 0, !dbg !274
  %1 = bitcast %struct.sockaddr_in* %src_addr to %struct.sockaddr*, !dbg !275
  %call = call i64 @recvfrom(i32 %0, i8* %arraydecay, i64 2041, i32 0, %struct.sockaddr* %1, i32* %addrlen), !dbg !276
  store i64 %call, i64* %recsize, align 8, !dbg !277
  call void @llvm.dbg.declare(metadata i32* %len, metadata !278, metadata !DIExpression()), !dbg !279
  %2 = load i64, i64* %recsize, align 8, !dbg !280
  %div = udiv i64 %2, 1, !dbg !281
  %conv = trunc i64 %div to i32, !dbg !280
  store i32 %conv, i32* %len, align 4, !dbg !279
  %3 = load i64, i64* %recsize, align 8, !dbg !282
  %cmp = icmp sgt i64 %3, 0, !dbg !284
  br i1 %cmp, label %if.then, label %if.else, !dbg !285

if.then:                                          ; preds = %entry
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message* %msg, metadata !286, metadata !DIExpression()), !dbg !288
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status* %status, metadata !289, metadata !DIExpression()), !dbg !290
  call void @llvm.dbg.declare(metadata i32* %i, metadata !291, metadata !DIExpression()), !dbg !293
  store i32 0, i32* %i, align 4, !dbg !293
  br label %for.cond, !dbg !294

for.cond:                                         ; preds = %for.inc, %if.then
  %4 = load i32, i32* %i, align 4, !dbg !295
  %conv2 = sext i32 %4 to i64, !dbg !295
  %5 = load i64, i64* %recsize, align 8, !dbg !297
  %cmp3 = icmp slt i64 %conv2, %5, !dbg !298
  br i1 %cmp3, label %for.body, label %for.end, !dbg !299

for.body:                                         ; preds = %for.cond
  %6 = load i32, i32* %i, align 4, !dbg !300
  %idxprom = sext i32 %6 to i64, !dbg !303
  %arrayidx = getelementptr inbounds [2041 x i8], [2041 x i8]* %buf, i64 0, i64 %idxprom, !dbg !303
  %7 = load i8, i8* %arrayidx, align 1, !dbg !303
  %call5 = call zeroext i8 @mavlink_parse_char(i8 zeroext 0, i8 zeroext %7, %struct.__mavlink_message* %msg, %struct.__mavlink_status* %status), !dbg !304
  %tobool = icmp ne i8 %call5, 0, !dbg !304
  br i1 %tobool, label %if.then6, label %if.end17, !dbg !305

if.then6:                                         ; preds = %for.body
  %msgid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %msg, i32 0, i32 8, !dbg !306
  %8 = bitcast [3 x i8]* %msgid to i24*, !dbg !306
  %bf.load = load i24, i24* %8, align 1, !dbg !306
  %bf.cast = zext i24 %bf.load to i32, !dbg !306
  %cmp7 = icmp eq i32 %bf.cast, 0, !dbg !309
  br i1 %cmp7, label %if.then9, label %if.end, !dbg !310

if.then9:                                         ; preds = %if.then6
  call void @llvm.dbg.declare(metadata %struct.__mavlink_heartbeat_t* %heartbeat, metadata !311, metadata !DIExpression()), !dbg !323
  call void @mavlink_msg_heartbeat_decode(%struct.__mavlink_message* %msg, %struct.__mavlink_heartbeat_t* %heartbeat), !dbg !324
  %sysid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %msg, i32 0, i32 6, !dbg !325
  %9 = load i8, i8* %sysid, align 1, !dbg !325
  %conv10 = zext i8 %9 to i32, !dbg !326
  %compid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %msg, i32 0, i32 7, !dbg !327
  %10 = load i8, i8* %compid, align 1, !dbg !327
  %conv11 = zext i8 %10 to i32, !dbg !328
  %type = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %heartbeat, i32 0, i32 1, !dbg !329
  %11 = load i8, i8* %type, align 4, !dbg !329
  %conv12 = zext i8 %11 to i32, !dbg !330
  %autopilot = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %heartbeat, i32 0, i32 2, !dbg !331
  %12 = load i8, i8* %autopilot, align 1, !dbg !331
  %conv13 = zext i8 %12 to i32, !dbg !332
  %base_mode = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %heartbeat, i32 0, i32 3, !dbg !333
  %13 = load i8, i8* %base_mode, align 2, !dbg !333
  %conv14 = zext i8 %13 to i32, !dbg !334
  %system_status = getelementptr inbounds %struct.__mavlink_heartbeat_t, %struct.__mavlink_heartbeat_t* %heartbeat, i32 0, i32 4, !dbg !335
  %14 = load i8, i8* %system_status, align 1, !dbg !335
  %conv15 = zext i8 %14 to i32, !dbg !336
  %call16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([83 x i8], [83 x i8]* @.str, i64 0, i64 0), i32 %conv10, i32 %conv11, i32 %conv12, i32 %conv13, i32 %conv14, i32 %conv15), !dbg !337
  br label %if.end, !dbg !338

if.end:                                           ; preds = %if.then9, %if.then6
  br label %if.end17, !dbg !339

if.end17:                                         ; preds = %if.end, %for.body
  br label %for.inc, !dbg !340

for.inc:                                          ; preds = %if.end17
  %15 = load i32, i32* %i, align 4, !dbg !341
  %inc = add nsw i32 %15, 1, !dbg !341
  store i32 %inc, i32* %i, align 4, !dbg !341
  br label %for.cond, !dbg !342, !llvm.loop !343

for.end:                                          ; preds = %for.cond
  br label %if.end29, !dbg !345

if.else:                                          ; preds = %entry
  %16 = load i64, i64* %recsize, align 8, !dbg !346
  %cmp18 = icmp slt i64 %16, 0, !dbg !348
  br i1 %cmp18, label %land.lhs.true, label %if.end28, !dbg !349

land.lhs.true:                                    ; preds = %if.else
  %call20 = call i32* @__errno_location() #7, !dbg !350
  %17 = load i32, i32* %call20, align 4, !dbg !350
  %cmp21 = icmp ne i32 %17, 11, !dbg !351
  br i1 %cmp21, label %land.lhs.true23, label %if.end28, !dbg !352

land.lhs.true23:                                  ; preds = %land.lhs.true
  %call24 = call i32* @__errno_location() #7, !dbg !353
  %18 = load i32, i32* %call24, align 4, !dbg !353
  %cmp25 = icmp ne i32 %18, 11, !dbg !354
  br i1 %cmp25, label %if.then27, label %if.end28, !dbg !355

if.then27:                                        ; preds = %land.lhs.true23
  call void @perror(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i64 0, i64 0)), !dbg !356
  br label %if.end28, !dbg !358

if.end28:                                         ; preds = %if.then27, %land.lhs.true23, %land.lhs.true, %if.else
  br label %if.end29

if.end29:                                         ; preds = %if.end28, %for.end
  ret void, !dbg !359
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i64 @recvfrom(i32, i8*, i64, i32, %struct.sockaddr*, i32*) #2

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i8 @mavlink_parse_char(i8 zeroext %chan, i8 zeroext %c, %struct.__mavlink_message* %r_message, %struct.__mavlink_status* %r_mavlink_status) #0 !dbg !360 {
entry:
  %retval = alloca i8, align 1
  %chan.addr = alloca i8, align 1
  %c.addr = alloca i8, align 1
  %r_message.addr = alloca %struct.__mavlink_message*, align 8
  %r_mavlink_status.addr = alloca %struct.__mavlink_status*, align 8
  %msg_received = alloca i8, align 1
  %rxmsg = alloca %struct.__mavlink_message*, align 8
  %status = alloca %struct.__mavlink_status*, align 8
  store i8 %chan, i8* %chan.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %chan.addr, metadata !363, metadata !DIExpression()), !dbg !364
  store i8 %c, i8* %c.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %c.addr, metadata !365, metadata !DIExpression()), !dbg !366
  store %struct.__mavlink_message* %r_message, %struct.__mavlink_message** %r_message.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %r_message.addr, metadata !367, metadata !DIExpression()), !dbg !368
  store %struct.__mavlink_status* %r_mavlink_status, %struct.__mavlink_status** %r_mavlink_status.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status** %r_mavlink_status.addr, metadata !369, metadata !DIExpression()), !dbg !370
  call void @llvm.dbg.declare(metadata i8* %msg_received, metadata !371, metadata !DIExpression()), !dbg !372
  %0 = load i8, i8* %chan.addr, align 1, !dbg !373
  %1 = load i8, i8* %c.addr, align 1, !dbg !374
  %2 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !375
  %3 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !376
  %call = call zeroext i8 @mavlink_frame_char(i8 zeroext %0, i8 zeroext %1, %struct.__mavlink_message* %2, %struct.__mavlink_status* %3), !dbg !377
  store i8 %call, i8* %msg_received, align 1, !dbg !372
  %4 = load i8, i8* %msg_received, align 1, !dbg !378
  %conv = zext i8 %4 to i32, !dbg !378
  %cmp = icmp eq i32 %conv, 2, !dbg !380
  br i1 %cmp, label %if.then, label %lor.lhs.false, !dbg !381

lor.lhs.false:                                    ; preds = %entry
  %5 = load i8, i8* %msg_received, align 1, !dbg !382
  %conv2 = zext i8 %5 to i32, !dbg !382
  %cmp3 = icmp eq i32 %conv2, 3, !dbg !383
  br i1 %cmp3, label %if.then, label %if.end13, !dbg !384

if.then:                                          ; preds = %lor.lhs.false, %entry
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %rxmsg, metadata !385, metadata !DIExpression()), !dbg !387
  %6 = load i8, i8* %chan.addr, align 1, !dbg !388
  %call5 = call %struct.__mavlink_message* @mavlink_get_channel_buffer(i8 zeroext %6), !dbg !389
  store %struct.__mavlink_message* %call5, %struct.__mavlink_message** %rxmsg, align 8, !dbg !387
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status** %status, metadata !390, metadata !DIExpression()), !dbg !391
  %7 = load i8, i8* %chan.addr, align 1, !dbg !392
  %call6 = call %struct.__mavlink_status* @mavlink_get_channel_status(i8 zeroext %7), !dbg !393
  store %struct.__mavlink_status* %call6, %struct.__mavlink_status** %status, align 8, !dbg !391
  %8 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status, align 8, !dbg !394
  call void @_mav_parse_error(%struct.__mavlink_status* %8), !dbg !395
  %9 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status, align 8, !dbg !396
  %msg_received7 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %9, i32 0, i32 0, !dbg !397
  store i8 0, i8* %msg_received7, align 8, !dbg !398
  %10 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status, align 8, !dbg !399
  %parse_state = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %10, i32 0, i32 3, !dbg !400
  store i32 1, i32* %parse_state, align 4, !dbg !401
  %11 = load i8, i8* %c.addr, align 1, !dbg !402
  %conv8 = zext i8 %11 to i32, !dbg !402
  %cmp9 = icmp eq i32 %conv8, 253, !dbg !404
  br i1 %cmp9, label %if.then11, label %if.end, !dbg !405

if.then11:                                        ; preds = %if.then
  %12 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status, align 8, !dbg !406
  %parse_state12 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %12, i32 0, i32 3, !dbg !408
  store i32 2, i32* %parse_state12, align 4, !dbg !409
  %13 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg, align 8, !dbg !410
  %len = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %13, i32 0, i32 2, !dbg !411
  store i8 0, i8* %len, align 1, !dbg !412
  %14 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg, align 8, !dbg !413
  call void @mavlink_start_checksum(%struct.__mavlink_message* %14), !dbg !414
  br label %if.end, !dbg !415

if.end:                                           ; preds = %if.then11, %if.then
  store i8 0, i8* %retval, align 1, !dbg !416
  br label %return, !dbg !416

if.end13:                                         ; preds = %lor.lhs.false
  %15 = load i8, i8* %msg_received, align 1, !dbg !417
  store i8 %15, i8* %retval, align 1, !dbg !418
  br label %return, !dbg !418

return:                                           ; preds = %if.end13, %if.end
  %16 = load i8, i8* %retval, align 1, !dbg !419
  ret i8 %16, !dbg !419
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_msg_heartbeat_decode(%struct.__mavlink_message* %msg, %struct.__mavlink_heartbeat_t* %heartbeat) #0 !dbg !420 {
entry:
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %heartbeat.addr = alloca %struct.__mavlink_heartbeat_t*, align 8
  %len = alloca i8, align 1
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !426, metadata !DIExpression()), !dbg !427
  store %struct.__mavlink_heartbeat_t* %heartbeat, %struct.__mavlink_heartbeat_t** %heartbeat.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_heartbeat_t** %heartbeat.addr, metadata !428, metadata !DIExpression()), !dbg !429
  call void @llvm.dbg.declare(metadata i8* %len, metadata !430, metadata !DIExpression()), !dbg !431
  %0 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !432
  %len1 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %0, i32 0, i32 2, !dbg !433
  %1 = load i8, i8* %len1, align 1, !dbg !433
  %conv = zext i8 %1 to i32, !dbg !432
  %cmp = icmp slt i32 %conv, 9, !dbg !434
  br i1 %cmp, label %cond.true, label %cond.false, !dbg !432

cond.true:                                        ; preds = %entry
  %2 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !435
  %len3 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %2, i32 0, i32 2, !dbg !436
  %3 = load i8, i8* %len3, align 1, !dbg !436
  %conv4 = zext i8 %3 to i32, !dbg !435
  br label %cond.end, !dbg !432

cond.false:                                       ; preds = %entry
  br label %cond.end, !dbg !432

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i32 [ %conv4, %cond.true ], [ 9, %cond.false ], !dbg !432
  %conv5 = trunc i32 %cond to i8, !dbg !432
  store i8 %conv5, i8* %len, align 1, !dbg !431
  %4 = load %struct.__mavlink_heartbeat_t*, %struct.__mavlink_heartbeat_t** %heartbeat.addr, align 8, !dbg !437
  %5 = bitcast %struct.__mavlink_heartbeat_t* %4 to i8*, !dbg !438
  call void @llvm.memset.p0i8.i64(i8* align 4 %5, i8 0, i64 9, i1 false), !dbg !438
  %6 = load %struct.__mavlink_heartbeat_t*, %struct.__mavlink_heartbeat_t** %heartbeat.addr, align 8, !dbg !439
  %7 = bitcast %struct.__mavlink_heartbeat_t* %6 to i8*, !dbg !440
  %8 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !441
  %payload64 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %8, i32 0, i32 9, !dbg !441
  %arrayidx = getelementptr inbounds [33 x i64], [33 x i64]* %payload64, i64 0, i64 0, !dbg !441
  %9 = bitcast i64* %arrayidx to i8*, !dbg !440
  %10 = load i8, i8* %len, align 1, !dbg !442
  %conv6 = zext i8 %10 to i64, !dbg !442
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 4 %7, i8* align 1 %9, i64 %conv6, i1 false), !dbg !440
  ret void, !dbg !443
}

declare dso_local i32 @printf(i8*, ...) #2

; Function Attrs: nounwind readnone
declare dso_local i32* @__errno_location() #3

declare dso_local void @perror(i8*) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !444 {
entry:
  %retval = alloca i32, align 4
  %sock = alloca i32, align 4
  %flags = alloca i32, align 4
  %local_addr = alloca %struct.sockaddr_in, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %sock, metadata !447, metadata !DIExpression()), !dbg !448
  %call = call i32 @socket(i32 2, i32 2, i32 17) #8, !dbg !449
  store i32 %call, i32* %sock, align 4, !dbg !448
  %0 = load i32, i32* %sock, align 4, !dbg !450
  %cmp = icmp slt i32 %0, 0, !dbg !452
  br i1 %cmp, label %if.then, label %if.end, !dbg !453

if.then:                                          ; preds = %entry
  call void @perror(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i64 0, i64 0)), !dbg !454
  store i32 1, i32* %retval, align 4, !dbg !456
  br label %return, !dbg !456

if.end:                                           ; preds = %entry
  call void @llvm.dbg.declare(metadata i32* %flags, metadata !457, metadata !DIExpression()), !dbg !458
  %1 = load i32, i32* %sock, align 4, !dbg !459
  %call1 = call i32 (i32, i32, ...) @fcntl(i32 %1, i32 3, i32 0), !dbg !460
  store i32 %call1, i32* %flags, align 4, !dbg !458
  %2 = load i32, i32* %sock, align 4, !dbg !461
  %3 = load i32, i32* %flags, align 4, !dbg !462
  %or = or i32 %3, 2048, !dbg !463
  %call2 = call i32 (i32, i32, ...) @fcntl(i32 %2, i32 4, i32 %or), !dbg !464
  call void @llvm.dbg.declare(metadata %struct.sockaddr_in* %local_addr, metadata !465, metadata !DIExpression()), !dbg !466
  %4 = bitcast %struct.sockaddr_in* %local_addr to i8*, !dbg !467
  call void @llvm.memset.p0i8.i64(i8* align 4 %4, i8 0, i64 16, i1 false), !dbg !467
  %sin_family = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %local_addr, i32 0, i32 0, !dbg !468
  store i16 2, i16* %sin_family, align 4, !dbg !469
  %sin_addr = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %local_addr, i32 0, i32 2, !dbg !470
  %s_addr = getelementptr inbounds %struct.in_addr, %struct.in_addr* %sin_addr, i32 0, i32 0, !dbg !471
  store i32 0, i32* %s_addr, align 4, !dbg !472
  %call3 = call zeroext i16 @htons(i16 zeroext 14550) #7, !dbg !473
  %sin_port = getelementptr inbounds %struct.sockaddr_in, %struct.sockaddr_in* %local_addr, i32 0, i32 1, !dbg !474
  store i16 %call3, i16* %sin_port, align 2, !dbg !475
  %5 = load i32, i32* %sock, align 4, !dbg !476
  %6 = bitcast %struct.sockaddr_in* %local_addr to %struct.sockaddr*, !dbg !478
  %call4 = call i32 @bind(i32 %5, %struct.sockaddr* %6, i32 16) #8, !dbg !479
  %cmp5 = icmp slt i32 %call4, 0, !dbg !480
  br i1 %cmp5, label %if.then6, label %if.end8, !dbg !481

if.then6:                                         ; preds = %if.end
  call void @perror(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i64 0, i64 0)), !dbg !482
  %7 = load i32, i32* %sock, align 4, !dbg !484
  %call7 = call i32 @close(i32 %7), !dbg !485
  store i32 1, i32* %retval, align 4, !dbg !486
  br label %return, !dbg !486

if.end8:                                          ; preds = %if.end
  %call9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.4, i64 0, i64 0), i32 14550), !dbg !487
  br label %while.body, !dbg !488

while.body:                                       ; preds = %if.end8, %while.body
  %8 = load i32, i32* %sock, align 4, !dbg !489
  call void @receive_heartbeat(i32 %8), !dbg !491
  %call10 = call i32 @usleep(i32 100000), !dbg !492
  br label %while.body, !dbg !488, !llvm.loop !493

return:                                           ; preds = %if.then6, %if.then
  %9 = load i32, i32* %retval, align 4, !dbg !495
  ret i32 %9, !dbg !495
}

; Function Attrs: nounwind
declare dso_local i32 @socket(i32, i32, i32) #4

declare dso_local i32 @fcntl(i32, i32, ...) #2

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #5

; Function Attrs: nounwind readnone
declare dso_local zeroext i16 @htons(i16 zeroext) #3

; Function Attrs: nounwind
declare dso_local i32 @bind(i32, %struct.sockaddr*, i32) #4

declare dso_local i32 @close(i32) #2

declare dso_local i32 @usleep(i32) #2

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i8 @mavlink_frame_char(i8 zeroext %chan, i8 zeroext %c, %struct.__mavlink_message* %r_message, %struct.__mavlink_status* %r_mavlink_status) #0 !dbg !496 {
entry:
  %chan.addr = alloca i8, align 1
  %c.addr = alloca i8, align 1
  %r_message.addr = alloca %struct.__mavlink_message*, align 8
  %r_mavlink_status.addr = alloca %struct.__mavlink_status*, align 8
  store i8 %chan, i8* %chan.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %chan.addr, metadata !497, metadata !DIExpression()), !dbg !498
  store i8 %c, i8* %c.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %c.addr, metadata !499, metadata !DIExpression()), !dbg !500
  store %struct.__mavlink_message* %r_message, %struct.__mavlink_message** %r_message.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %r_message.addr, metadata !501, metadata !DIExpression()), !dbg !502
  store %struct.__mavlink_status* %r_mavlink_status, %struct.__mavlink_status** %r_mavlink_status.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status** %r_mavlink_status.addr, metadata !503, metadata !DIExpression()), !dbg !504
  %0 = load i8, i8* %chan.addr, align 1, !dbg !505
  %call = call %struct.__mavlink_message* @mavlink_get_channel_buffer(i8 zeroext %0), !dbg !506
  %1 = load i8, i8* %chan.addr, align 1, !dbg !507
  %call1 = call %struct.__mavlink_status* @mavlink_get_channel_status(i8 zeroext %1), !dbg !508
  %2 = load i8, i8* %c.addr, align 1, !dbg !509
  %3 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !510
  %4 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !511
  %call2 = call zeroext i8 @mavlink_frame_char_buffer(%struct.__mavlink_message* %call, %struct.__mavlink_status* %call1, i8 zeroext %2, %struct.__mavlink_message* %3, %struct.__mavlink_status* %4), !dbg !512
  ret i8 %call2, !dbg !513
}

; Function Attrs: noinline nounwind optnone uwtable
define internal %struct.__mavlink_message* @mavlink_get_channel_buffer(i8 zeroext %chan) #0 !dbg !135 {
entry:
  %chan.addr = alloca i8, align 1
  store i8 %chan, i8* %chan.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %chan.addr, metadata !514, metadata !DIExpression()), !dbg !515
  %0 = load i8, i8* %chan.addr, align 1, !dbg !516
  %idxprom = zext i8 %0 to i64, !dbg !517
  %arrayidx = getelementptr inbounds [16 x %struct.__mavlink_message], [16 x %struct.__mavlink_message]* @mavlink_get_channel_buffer.m_mavlink_buffer, i64 0, i64 %idxprom, !dbg !517
  ret %struct.__mavlink_message* %arrayidx, !dbg !518
}

; Function Attrs: noinline nounwind optnone uwtable
define internal %struct.__mavlink_status* @mavlink_get_channel_status(i8 zeroext %chan) #0 !dbg !174 {
entry:
  %chan.addr = alloca i8, align 1
  store i8 %chan, i8* %chan.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %chan.addr, metadata !519, metadata !DIExpression()), !dbg !520
  %0 = load i8, i8* %chan.addr, align 1, !dbg !521
  %idxprom = zext i8 %0 to i64, !dbg !522
  %arrayidx = getelementptr inbounds [16 x %struct.__mavlink_status], [16 x %struct.__mavlink_status]* @mavlink_get_channel_status.m_mavlink_status, i64 0, i64 %idxprom, !dbg !522
  ret %struct.__mavlink_status* %arrayidx, !dbg !523
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_mav_parse_error(%struct.__mavlink_status* %status) #0 !dbg !524 {
entry:
  %status.addr = alloca %struct.__mavlink_status*, align 8
  store %struct.__mavlink_status* %status, %struct.__mavlink_status** %status.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status** %status.addr, metadata !527, metadata !DIExpression()), !dbg !528
  %0 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !529
  %parse_error = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %0, i32 0, i32 2, !dbg !530
  %1 = load i8, i8* %parse_error, align 2, !dbg !531
  %inc = add i8 %1, 1, !dbg !531
  store i8 %inc, i8* %parse_error, align 2, !dbg !531
  ret void, !dbg !532
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_start_checksum(%struct.__mavlink_message* %msg) #0 !dbg !533 {
entry:
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %crcTmp = alloca i16, align 2
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !536, metadata !DIExpression()), !dbg !537
  call void @llvm.dbg.declare(metadata i16* %crcTmp, metadata !538, metadata !DIExpression()), !dbg !539
  store i16 0, i16* %crcTmp, align 2, !dbg !539
  call void @crc_init(i16* %crcTmp), !dbg !540
  %0 = load i16, i16* %crcTmp, align 2, !dbg !541
  %1 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !542
  %checksum = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %1, i32 0, i32 0, !dbg !543
  store i16 %0, i16* %checksum, align 1, !dbg !544
  ret void, !dbg !545
}

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i8 @mavlink_frame_char_buffer(%struct.__mavlink_message* %rxmsg, %struct.__mavlink_status* %status, i8 zeroext %c, %struct.__mavlink_message* %r_message, %struct.__mavlink_status* %r_mavlink_status) #0 !dbg !546 {
entry:
  %rxmsg.addr = alloca %struct.__mavlink_message*, align 8
  %status.addr = alloca %struct.__mavlink_status*, align 8
  %c.addr = alloca i8, align 1
  %r_message.addr = alloca %struct.__mavlink_message*, align 8
  %r_mavlink_status.addr = alloca %struct.__mavlink_status*, align 8
  %bufferIndex = alloca i32, align 4
  %e = alloca %struct.__mavlink_msg_entry*, align 8
  %crc_extra = alloca i8, align 1
  %sig_ok = alloca i8, align 1
  store %struct.__mavlink_message* %rxmsg, %struct.__mavlink_message** %rxmsg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %rxmsg.addr, metadata !549, metadata !DIExpression()), !dbg !550
  store %struct.__mavlink_status* %status, %struct.__mavlink_status** %status.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status** %status.addr, metadata !551, metadata !DIExpression()), !dbg !552
  store i8 %c, i8* %c.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %c.addr, metadata !553, metadata !DIExpression()), !dbg !554
  store %struct.__mavlink_message* %r_message, %struct.__mavlink_message** %r_message.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %r_message.addr, metadata !555, metadata !DIExpression()), !dbg !556
  store %struct.__mavlink_status* %r_mavlink_status, %struct.__mavlink_status** %r_mavlink_status.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_status** %r_mavlink_status.addr, metadata !557, metadata !DIExpression()), !dbg !558
  call void @llvm.dbg.declare(metadata i32* %bufferIndex, metadata !559, metadata !DIExpression()), !dbg !560
  store i32 0, i32* %bufferIndex, align 4, !dbg !560
  %0 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !561
  %msg_received = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %0, i32 0, i32 0, !dbg !562
  store i8 0, i8* %msg_received, align 8, !dbg !563
  %1 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !564
  %parse_state = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %1, i32 0, i32 3, !dbg !565
  %2 = load i32, i32* %parse_state, align 4, !dbg !565
  switch i32 %2, label %sw.epilog [
    i32 0, label %sw.bb
    i32 1, label %sw.bb
    i32 2, label %sw.bb16
    i32 3, label %sw.bb33
    i32 4, label %sw.bb45
    i32 5, label %sw.bb48
    i32 6, label %sw.bb50
    i32 7, label %sw.bb52
    i32 8, label %sw.bb54
    i32 9, label %sw.bb73
    i32 10, label %sw.bb79
    i32 11, label %sw.bb96
    i32 12, label %sw.bb109
    i32 13, label %sw.bb146
    i32 14, label %sw.bb146
    i32 15, label %sw.bb203
  ], !dbg !566

sw.bb:                                            ; preds = %entry, %entry
  %3 = load i8, i8* %c.addr, align 1, !dbg !567
  %conv = zext i8 %3 to i32, !dbg !567
  %cmp = icmp eq i32 %conv, 253, !dbg !570
  br i1 %cmp, label %if.then, label %if.else, !dbg !571

if.then:                                          ; preds = %sw.bb
  %4 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !572
  %parse_state2 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %4, i32 0, i32 3, !dbg !574
  store i32 2, i32* %parse_state2, align 4, !dbg !575
  %5 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !576
  %len = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %5, i32 0, i32 2, !dbg !577
  store i8 0, i8* %len, align 1, !dbg !578
  %6 = load i8, i8* %c.addr, align 1, !dbg !579
  %7 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !580
  %magic = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %7, i32 0, i32 1, !dbg !581
  store i8 %6, i8* %magic, align 1, !dbg !582
  %8 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !583
  %flags = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %8, i32 0, i32 9, !dbg !584
  %9 = load i8, i8* %flags, align 8, !dbg !585
  %conv3 = zext i8 %9 to i32, !dbg !585
  %and = and i32 %conv3, -2, !dbg !585
  %conv4 = trunc i32 %and to i8, !dbg !585
  store i8 %conv4, i8* %flags, align 8, !dbg !585
  %10 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !586
  call void @mavlink_start_checksum(%struct.__mavlink_message* %10), !dbg !587
  br label %if.end15, !dbg !588

if.else:                                          ; preds = %sw.bb
  %11 = load i8, i8* %c.addr, align 1, !dbg !589
  %conv5 = zext i8 %11 to i32, !dbg !589
  %cmp6 = icmp eq i32 %conv5, 254, !dbg !591
  br i1 %cmp6, label %if.then8, label %if.end, !dbg !592

if.then8:                                         ; preds = %if.else
  %12 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !593
  %parse_state9 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %12, i32 0, i32 3, !dbg !595
  store i32 2, i32* %parse_state9, align 4, !dbg !596
  %13 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !597
  %len10 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %13, i32 0, i32 2, !dbg !598
  store i8 0, i8* %len10, align 1, !dbg !599
  %14 = load i8, i8* %c.addr, align 1, !dbg !600
  %15 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !601
  %magic11 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %15, i32 0, i32 1, !dbg !602
  store i8 %14, i8* %magic11, align 1, !dbg !603
  %16 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !604
  %flags12 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %16, i32 0, i32 9, !dbg !605
  %17 = load i8, i8* %flags12, align 8, !dbg !606
  %conv13 = zext i8 %17 to i32, !dbg !606
  %or = or i32 %conv13, 1, !dbg !606
  %conv14 = trunc i32 %or to i8, !dbg !606
  store i8 %conv14, i8* %flags12, align 8, !dbg !606
  %18 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !607
  call void @mavlink_start_checksum(%struct.__mavlink_message* %18), !dbg !608
  br label %if.end, !dbg !609

if.end:                                           ; preds = %if.then8, %if.else
  br label %if.end15

if.end15:                                         ; preds = %if.end, %if.then
  br label %sw.epilog, !dbg !610

sw.bb16:                                          ; preds = %entry
  %19 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !611
  %msg_received17 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %19, i32 0, i32 0, !dbg !613
  %20 = load i8, i8* %msg_received17, align 8, !dbg !613
  %tobool = icmp ne i8 %20, 0, !dbg !611
  br i1 %tobool, label %if.then18, label %if.else21, !dbg !614

if.then18:                                        ; preds = %sw.bb16
  %21 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !615
  %buffer_overrun = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %21, i32 0, i32 1, !dbg !617
  %22 = load i8, i8* %buffer_overrun, align 1, !dbg !618
  %inc = add i8 %22, 1, !dbg !618
  store i8 %inc, i8* %buffer_overrun, align 1, !dbg !618
  %23 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !619
  call void @_mav_parse_error(%struct.__mavlink_status* %23), !dbg !620
  %24 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !621
  %msg_received19 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %24, i32 0, i32 0, !dbg !622
  store i8 0, i8* %msg_received19, align 8, !dbg !623
  %25 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !624
  %parse_state20 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %25, i32 0, i32 3, !dbg !625
  store i32 1, i32* %parse_state20, align 4, !dbg !626
  br label %if.end32, !dbg !627

if.else21:                                        ; preds = %sw.bb16
  %26 = load i8, i8* %c.addr, align 1, !dbg !628
  %27 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !630
  %len22 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %27, i32 0, i32 2, !dbg !631
  store i8 %26, i8* %len22, align 1, !dbg !632
  %28 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !633
  %packet_idx = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %28, i32 0, i32 4, !dbg !634
  store i8 0, i8* %packet_idx, align 8, !dbg !635
  %29 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !636
  %30 = load i8, i8* %c.addr, align 1, !dbg !637
  call void @mavlink_update_checksum(%struct.__mavlink_message* %29, i8 zeroext %30), !dbg !638
  %31 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !639
  %flags23 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %31, i32 0, i32 9, !dbg !641
  %32 = load i8, i8* %flags23, align 8, !dbg !641
  %conv24 = zext i8 %32 to i32, !dbg !639
  %and25 = and i32 %conv24, 1, !dbg !642
  %tobool26 = icmp ne i32 %and25, 0, !dbg !642
  br i1 %tobool26, label %if.then27, label %if.else29, !dbg !643

if.then27:                                        ; preds = %if.else21
  %33 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !644
  %incompat_flags = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %33, i32 0, i32 3, !dbg !646
  store i8 0, i8* %incompat_flags, align 1, !dbg !647
  %34 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !648
  %compat_flags = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %34, i32 0, i32 4, !dbg !649
  store i8 0, i8* %compat_flags, align 1, !dbg !650
  %35 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !651
  %parse_state28 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %35, i32 0, i32 3, !dbg !652
  store i32 5, i32* %parse_state28, align 4, !dbg !653
  br label %if.end31, !dbg !654

if.else29:                                        ; preds = %if.else21
  %36 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !655
  %parse_state30 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %36, i32 0, i32 3, !dbg !657
  store i32 3, i32* %parse_state30, align 4, !dbg !658
  br label %if.end31

if.end31:                                         ; preds = %if.else29, %if.then27
  br label %if.end32

if.end32:                                         ; preds = %if.end31, %if.then18
  br label %sw.epilog, !dbg !659

sw.bb33:                                          ; preds = %entry
  %37 = load i8, i8* %c.addr, align 1, !dbg !660
  %38 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !661
  %incompat_flags34 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %38, i32 0, i32 3, !dbg !662
  store i8 %37, i8* %incompat_flags34, align 1, !dbg !663
  %39 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !664
  %incompat_flags35 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %39, i32 0, i32 3, !dbg !666
  %40 = load i8, i8* %incompat_flags35, align 1, !dbg !666
  %conv36 = zext i8 %40 to i32, !dbg !664
  %and37 = and i32 %conv36, -2, !dbg !667
  %cmp38 = icmp ne i32 %and37, 0, !dbg !668
  br i1 %cmp38, label %if.then40, label %if.end43, !dbg !669

if.then40:                                        ; preds = %sw.bb33
  %41 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !670
  call void @_mav_parse_error(%struct.__mavlink_status* %41), !dbg !672
  %42 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !673
  %msg_received41 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %42, i32 0, i32 0, !dbg !674
  store i8 0, i8* %msg_received41, align 8, !dbg !675
  %43 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !676
  %parse_state42 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %43, i32 0, i32 3, !dbg !677
  store i32 1, i32* %parse_state42, align 4, !dbg !678
  br label %sw.epilog, !dbg !679

if.end43:                                         ; preds = %sw.bb33
  %44 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !680
  %45 = load i8, i8* %c.addr, align 1, !dbg !681
  call void @mavlink_update_checksum(%struct.__mavlink_message* %44, i8 zeroext %45), !dbg !682
  %46 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !683
  %parse_state44 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %46, i32 0, i32 3, !dbg !684
  store i32 4, i32* %parse_state44, align 4, !dbg !685
  br label %sw.epilog, !dbg !686

sw.bb45:                                          ; preds = %entry
  %47 = load i8, i8* %c.addr, align 1, !dbg !687
  %48 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !688
  %compat_flags46 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %48, i32 0, i32 4, !dbg !689
  store i8 %47, i8* %compat_flags46, align 1, !dbg !690
  %49 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !691
  %50 = load i8, i8* %c.addr, align 1, !dbg !692
  call void @mavlink_update_checksum(%struct.__mavlink_message* %49, i8 zeroext %50), !dbg !693
  %51 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !694
  %parse_state47 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %51, i32 0, i32 3, !dbg !695
  store i32 5, i32* %parse_state47, align 4, !dbg !696
  br label %sw.epilog, !dbg !697

sw.bb48:                                          ; preds = %entry
  %52 = load i8, i8* %c.addr, align 1, !dbg !698
  %53 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !699
  %seq = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %53, i32 0, i32 5, !dbg !700
  store i8 %52, i8* %seq, align 1, !dbg !701
  %54 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !702
  %55 = load i8, i8* %c.addr, align 1, !dbg !703
  call void @mavlink_update_checksum(%struct.__mavlink_message* %54, i8 zeroext %55), !dbg !704
  %56 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !705
  %parse_state49 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %56, i32 0, i32 3, !dbg !706
  store i32 6, i32* %parse_state49, align 4, !dbg !707
  br label %sw.epilog, !dbg !708

sw.bb50:                                          ; preds = %entry
  %57 = load i8, i8* %c.addr, align 1, !dbg !709
  %58 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !710
  %sysid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %58, i32 0, i32 6, !dbg !711
  store i8 %57, i8* %sysid, align 1, !dbg !712
  %59 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !713
  %60 = load i8, i8* %c.addr, align 1, !dbg !714
  call void @mavlink_update_checksum(%struct.__mavlink_message* %59, i8 zeroext %60), !dbg !715
  %61 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !716
  %parse_state51 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %61, i32 0, i32 3, !dbg !717
  store i32 7, i32* %parse_state51, align 4, !dbg !718
  br label %sw.epilog, !dbg !719

sw.bb52:                                          ; preds = %entry
  %62 = load i8, i8* %c.addr, align 1, !dbg !720
  %63 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !721
  %compid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %63, i32 0, i32 7, !dbg !722
  store i8 %62, i8* %compid, align 1, !dbg !723
  %64 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !724
  %65 = load i8, i8* %c.addr, align 1, !dbg !725
  call void @mavlink_update_checksum(%struct.__mavlink_message* %64, i8 zeroext %65), !dbg !726
  %66 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !727
  %parse_state53 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %66, i32 0, i32 3, !dbg !728
  store i32 8, i32* %parse_state53, align 4, !dbg !729
  br label %sw.epilog, !dbg !730

sw.bb54:                                          ; preds = %entry
  %67 = load i8, i8* %c.addr, align 1, !dbg !731
  %conv55 = zext i8 %67 to i32, !dbg !731
  %68 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !732
  %msgid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %68, i32 0, i32 8, !dbg !733
  %69 = bitcast [3 x i8]* %msgid to i24*, !dbg !733
  %70 = trunc i32 %conv55 to i24, !dbg !734
  store i24 %70, i24* %69, align 1, !dbg !734
  %bf.result.cast = zext i24 %70 to i32, !dbg !734
  %71 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !735
  %72 = load i8, i8* %c.addr, align 1, !dbg !736
  call void @mavlink_update_checksum(%struct.__mavlink_message* %71, i8 zeroext %72), !dbg !737
  %73 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !738
  %flags56 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %73, i32 0, i32 9, !dbg !740
  %74 = load i8, i8* %flags56, align 8, !dbg !740
  %conv57 = zext i8 %74 to i32, !dbg !738
  %and58 = and i32 %conv57, 1, !dbg !741
  %tobool59 = icmp ne i32 %and58, 0, !dbg !741
  br i1 %tobool59, label %if.then60, label %if.else70, !dbg !742

if.then60:                                        ; preds = %sw.bb54
  %75 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !743
  %len61 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %75, i32 0, i32 2, !dbg !746
  %76 = load i8, i8* %len61, align 1, !dbg !746
  %conv62 = zext i8 %76 to i32, !dbg !743
  %cmp63 = icmp sgt i32 %conv62, 0, !dbg !747
  br i1 %cmp63, label %if.then65, label %if.else67, !dbg !748

if.then65:                                        ; preds = %if.then60
  %77 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !749
  %parse_state66 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %77, i32 0, i32 3, !dbg !751
  store i32 11, i32* %parse_state66, align 4, !dbg !752
  br label %if.end69, !dbg !753

if.else67:                                        ; preds = %if.then60
  %78 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !754
  %parse_state68 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %78, i32 0, i32 3, !dbg !756
  store i32 12, i32* %parse_state68, align 4, !dbg !757
  br label %if.end69

if.end69:                                         ; preds = %if.else67, %if.then65
  br label %if.end72, !dbg !758

if.else70:                                        ; preds = %sw.bb54
  %79 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !759
  %parse_state71 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %79, i32 0, i32 3, !dbg !761
  store i32 9, i32* %parse_state71, align 4, !dbg !762
  br label %if.end72

if.end72:                                         ; preds = %if.else70, %if.end69
  br label %sw.epilog, !dbg !763

sw.bb73:                                          ; preds = %entry
  %80 = load i8, i8* %c.addr, align 1, !dbg !764
  %conv74 = zext i8 %80 to i32, !dbg !764
  %shl = shl i32 %conv74, 8, !dbg !765
  %81 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !766
  %msgid75 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %81, i32 0, i32 8, !dbg !767
  %82 = bitcast [3 x i8]* %msgid75 to i24*, !dbg !767
  %bf.load = load i24, i24* %82, align 1, !dbg !768
  %bf.cast = zext i24 %bf.load to i32, !dbg !768
  %or76 = or i32 %bf.cast, %shl, !dbg !768
  %83 = trunc i32 %or76 to i24, !dbg !768
  store i24 %83, i24* %82, align 1, !dbg !768
  %bf.result.cast77 = zext i24 %83 to i32, !dbg !768
  %84 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !769
  %85 = load i8, i8* %c.addr, align 1, !dbg !770
  call void @mavlink_update_checksum(%struct.__mavlink_message* %84, i8 zeroext %85), !dbg !771
  %86 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !772
  %parse_state78 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %86, i32 0, i32 3, !dbg !773
  store i32 10, i32* %parse_state78, align 4, !dbg !774
  br label %sw.epilog, !dbg !775

sw.bb79:                                          ; preds = %entry
  %87 = load i8, i8* %c.addr, align 1, !dbg !776
  %conv80 = zext i8 %87 to i32, !dbg !777
  %shl81 = shl i32 %conv80, 16, !dbg !778
  %88 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !779
  %msgid82 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %88, i32 0, i32 8, !dbg !780
  %89 = bitcast [3 x i8]* %msgid82 to i24*, !dbg !780
  %bf.load83 = load i24, i24* %89, align 1, !dbg !781
  %bf.cast84 = zext i24 %bf.load83 to i32, !dbg !781
  %or85 = or i32 %bf.cast84, %shl81, !dbg !781
  %90 = trunc i32 %or85 to i24, !dbg !781
  store i24 %90, i24* %89, align 1, !dbg !781
  %bf.result.cast86 = zext i24 %90 to i32, !dbg !781
  %91 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !782
  %92 = load i8, i8* %c.addr, align 1, !dbg !783
  call void @mavlink_update_checksum(%struct.__mavlink_message* %91, i8 zeroext %92), !dbg !784
  %93 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !785
  %len87 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %93, i32 0, i32 2, !dbg !787
  %94 = load i8, i8* %len87, align 1, !dbg !787
  %conv88 = zext i8 %94 to i32, !dbg !785
  %cmp89 = icmp sgt i32 %conv88, 0, !dbg !788
  br i1 %cmp89, label %if.then91, label %if.else93, !dbg !789

if.then91:                                        ; preds = %sw.bb79
  %95 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !790
  %parse_state92 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %95, i32 0, i32 3, !dbg !792
  store i32 11, i32* %parse_state92, align 4, !dbg !793
  br label %if.end95, !dbg !794

if.else93:                                        ; preds = %sw.bb79
  %96 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !795
  %parse_state94 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %96, i32 0, i32 3, !dbg !797
  store i32 12, i32* %parse_state94, align 4, !dbg !798
  br label %if.end95

if.end95:                                         ; preds = %if.else93, %if.then91
  br label %sw.epilog, !dbg !799

sw.bb96:                                          ; preds = %entry
  %97 = load i8, i8* %c.addr, align 1, !dbg !800
  %98 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !801
  %payload64 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %98, i32 0, i32 9, !dbg !801
  %arrayidx = getelementptr inbounds [33 x i64], [33 x i64]* %payload64, i64 0, i64 0, !dbg !801
  %99 = bitcast i64* %arrayidx to i8*, !dbg !801
  %100 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !802
  %packet_idx97 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %100, i32 0, i32 4, !dbg !803
  %101 = load i8, i8* %packet_idx97, align 8, !dbg !804
  %inc98 = add i8 %101, 1, !dbg !804
  store i8 %inc98, i8* %packet_idx97, align 8, !dbg !804
  %idxprom = zext i8 %101 to i64, !dbg !801
  %arrayidx99 = getelementptr inbounds i8, i8* %99, i64 %idxprom, !dbg !801
  store i8 %97, i8* %arrayidx99, align 1, !dbg !805
  %102 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !806
  %103 = load i8, i8* %c.addr, align 1, !dbg !807
  call void @mavlink_update_checksum(%struct.__mavlink_message* %102, i8 zeroext %103), !dbg !808
  %104 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !809
  %packet_idx100 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %104, i32 0, i32 4, !dbg !811
  %105 = load i8, i8* %packet_idx100, align 8, !dbg !811
  %conv101 = zext i8 %105 to i32, !dbg !809
  %106 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !812
  %len102 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %106, i32 0, i32 2, !dbg !813
  %107 = load i8, i8* %len102, align 1, !dbg !813
  %conv103 = zext i8 %107 to i32, !dbg !812
  %cmp104 = icmp eq i32 %conv101, %conv103, !dbg !814
  br i1 %cmp104, label %if.then106, label %if.end108, !dbg !815

if.then106:                                       ; preds = %sw.bb96
  %108 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !816
  %parse_state107 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %108, i32 0, i32 3, !dbg !818
  store i32 12, i32* %parse_state107, align 4, !dbg !819
  br label %if.end108, !dbg !820

if.end108:                                        ; preds = %if.then106, %sw.bb96
  br label %sw.epilog, !dbg !821

sw.bb109:                                         ; preds = %entry
  call void @llvm.dbg.declare(metadata %struct.__mavlink_msg_entry** %e, metadata !822, metadata !DIExpression()), !dbg !824
  %109 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !825
  %msgid110 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %109, i32 0, i32 8, !dbg !826
  %110 = bitcast [3 x i8]* %msgid110 to i24*, !dbg !826
  %bf.load111 = load i24, i24* %110, align 1, !dbg !826
  %bf.cast112 = zext i24 %bf.load111 to i32, !dbg !826
  %call = call %struct.__mavlink_msg_entry* @mavlink_get_msg_entry(i32 %bf.cast112), !dbg !827
  store %struct.__mavlink_msg_entry* %call, %struct.__mavlink_msg_entry** %e, align 8, !dbg !824
  call void @llvm.dbg.declare(metadata i8* %crc_extra, metadata !828, metadata !DIExpression()), !dbg !829
  %111 = load %struct.__mavlink_msg_entry*, %struct.__mavlink_msg_entry** %e, align 8, !dbg !830
  %tobool113 = icmp ne %struct.__mavlink_msg_entry* %111, null, !dbg !830
  br i1 %tobool113, label %cond.true, label %cond.false, !dbg !830

cond.true:                                        ; preds = %sw.bb109
  %112 = load %struct.__mavlink_msg_entry*, %struct.__mavlink_msg_entry** %e, align 8, !dbg !831
  %crc_extra114 = getelementptr inbounds %struct.__mavlink_msg_entry, %struct.__mavlink_msg_entry* %112, i32 0, i32 1, !dbg !832
  %113 = load i8, i8* %crc_extra114, align 4, !dbg !832
  %conv115 = zext i8 %113 to i32, !dbg !831
  br label %cond.end, !dbg !830

cond.false:                                       ; preds = %sw.bb109
  br label %cond.end, !dbg !830

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i32 [ %conv115, %cond.true ], [ 0, %cond.false ], !dbg !830
  %conv116 = trunc i32 %cond to i8, !dbg !830
  store i8 %conv116, i8* %crc_extra, align 1, !dbg !829
  %114 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !833
  %115 = load i8, i8* %crc_extra, align 1, !dbg !834
  call void @mavlink_update_checksum(%struct.__mavlink_message* %114, i8 zeroext %115), !dbg !835
  %116 = load i8, i8* %c.addr, align 1, !dbg !836
  %conv117 = zext i8 %116 to i32, !dbg !836
  %117 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !838
  %checksum = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %117, i32 0, i32 0, !dbg !839
  %118 = load i16, i16* %checksum, align 1, !dbg !839
  %conv118 = zext i16 %118 to i32, !dbg !838
  %and119 = and i32 %conv118, 255, !dbg !840
  %cmp120 = icmp ne i32 %conv117, %and119, !dbg !841
  br i1 %cmp120, label %if.then122, label %if.else124, !dbg !842

if.then122:                                       ; preds = %cond.end
  %119 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !843
  %parse_state123 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %119, i32 0, i32 3, !dbg !845
  store i32 14, i32* %parse_state123, align 4, !dbg !846
  br label %if.end126, !dbg !847

if.else124:                                       ; preds = %cond.end
  %120 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !848
  %parse_state125 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %120, i32 0, i32 3, !dbg !850
  store i32 13, i32* %parse_state125, align 4, !dbg !851
  br label %if.end126

if.end126:                                        ; preds = %if.else124, %if.then122
  %121 = load i8, i8* %c.addr, align 1, !dbg !852
  %122 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !853
  %ck = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %122, i32 0, i32 10, !dbg !854
  %arrayidx127 = getelementptr inbounds [2 x i8], [2 x i8]* %ck, i64 0, i64 0, !dbg !853
  store i8 %121, i8* %arrayidx127, align 1, !dbg !855
  %123 = load %struct.__mavlink_msg_entry*, %struct.__mavlink_msg_entry** %e, align 8, !dbg !856
  %tobool128 = icmp ne %struct.__mavlink_msg_entry* %123, null, !dbg !856
  br i1 %tobool128, label %land.lhs.true, label %if.end145, !dbg !858

land.lhs.true:                                    ; preds = %if.end126
  %124 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !859
  %packet_idx129 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %124, i32 0, i32 4, !dbg !860
  %125 = load i8, i8* %packet_idx129, align 8, !dbg !860
  %conv130 = zext i8 %125 to i32, !dbg !859
  %126 = load %struct.__mavlink_msg_entry*, %struct.__mavlink_msg_entry** %e, align 8, !dbg !861
  %max_msg_len = getelementptr inbounds %struct.__mavlink_msg_entry, %struct.__mavlink_msg_entry* %126, i32 0, i32 3, !dbg !862
  %127 = load i8, i8* %max_msg_len, align 2, !dbg !862
  %conv131 = zext i8 %127 to i32, !dbg !861
  %cmp132 = icmp slt i32 %conv130, %conv131, !dbg !863
  br i1 %cmp132, label %if.then134, label %if.end145, !dbg !864

if.then134:                                       ; preds = %land.lhs.true
  %128 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !865
  %payload64135 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %128, i32 0, i32 9, !dbg !865
  %arrayidx136 = getelementptr inbounds [33 x i64], [33 x i64]* %payload64135, i64 0, i64 0, !dbg !865
  %129 = bitcast i64* %arrayidx136 to i8*, !dbg !865
  %130 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !867
  %packet_idx137 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %130, i32 0, i32 4, !dbg !868
  %131 = load i8, i8* %packet_idx137, align 8, !dbg !868
  %idxprom138 = zext i8 %131 to i64, !dbg !865
  %arrayidx139 = getelementptr inbounds i8, i8* %129, i64 %idxprom138, !dbg !865
  %132 = load %struct.__mavlink_msg_entry*, %struct.__mavlink_msg_entry** %e, align 8, !dbg !869
  %max_msg_len140 = getelementptr inbounds %struct.__mavlink_msg_entry, %struct.__mavlink_msg_entry* %132, i32 0, i32 3, !dbg !870
  %133 = load i8, i8* %max_msg_len140, align 2, !dbg !870
  %conv141 = zext i8 %133 to i32, !dbg !869
  %134 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !871
  %packet_idx142 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %134, i32 0, i32 4, !dbg !872
  %135 = load i8, i8* %packet_idx142, align 8, !dbg !872
  %conv143 = zext i8 %135 to i32, !dbg !871
  %sub = sub nsw i32 %conv141, %conv143, !dbg !873
  %conv144 = sext i32 %sub to i64, !dbg !869
  call void @llvm.memset.p0i8.i64(i8* align 1 %arrayidx139, i8 0, i64 %conv144, i1 false), !dbg !874
  br label %if.end145, !dbg !875

if.end145:                                        ; preds = %if.then134, %land.lhs.true, %if.end126
  br label %sw.epilog, !dbg !876

sw.bb146:                                         ; preds = %entry, %entry
  %136 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !877
  %parse_state147 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %136, i32 0, i32 3, !dbg !879
  %137 = load i32, i32* %parse_state147, align 4, !dbg !879
  %cmp148 = icmp eq i32 %137, 14, !dbg !880
  br i1 %cmp148, label %if.then155, label %lor.lhs.false, !dbg !881

lor.lhs.false:                                    ; preds = %sw.bb146
  %138 = load i8, i8* %c.addr, align 1, !dbg !882
  %conv150 = zext i8 %138 to i32, !dbg !882
  %139 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !883
  %checksum151 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %139, i32 0, i32 0, !dbg !884
  %140 = load i16, i16* %checksum151, align 1, !dbg !884
  %conv152 = zext i16 %140 to i32, !dbg !883
  %shr = ashr i32 %conv152, 8, !dbg !885
  %cmp153 = icmp ne i32 %conv150, %shr, !dbg !886
  br i1 %cmp153, label %if.then155, label %if.else157, !dbg !887

if.then155:                                       ; preds = %lor.lhs.false, %sw.bb146
  %141 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !888
  %msg_received156 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %141, i32 0, i32 0, !dbg !890
  store i8 2, i8* %msg_received156, align 8, !dbg !891
  br label %if.end159, !dbg !892

if.else157:                                       ; preds = %lor.lhs.false
  %142 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !893
  %msg_received158 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %142, i32 0, i32 0, !dbg !895
  store i8 1, i8* %msg_received158, align 8, !dbg !896
  br label %if.end159

if.end159:                                        ; preds = %if.else157, %if.then155
  %143 = load i8, i8* %c.addr, align 1, !dbg !897
  %144 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !898
  %ck160 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %144, i32 0, i32 10, !dbg !899
  %arrayidx161 = getelementptr inbounds [2 x i8], [2 x i8]* %ck160, i64 0, i64 1, !dbg !898
  store i8 %143, i8* %arrayidx161, align 1, !dbg !900
  %145 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !901
  %incompat_flags162 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %145, i32 0, i32 3, !dbg !903
  %146 = load i8, i8* %incompat_flags162, align 1, !dbg !903
  %conv163 = zext i8 %146 to i32, !dbg !901
  %and164 = and i32 %conv163, 1, !dbg !904
  %tobool165 = icmp ne i32 %and164, 0, !dbg !904
  br i1 %tobool165, label %if.then166, label %if.else175, !dbg !905

if.then166:                                       ; preds = %if.end159
  %147 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !906
  %parse_state167 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %147, i32 0, i32 3, !dbg !908
  store i32 15, i32* %parse_state167, align 4, !dbg !909
  %148 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !910
  %signature_wait = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %148, i32 0, i32 10, !dbg !911
  store i8 13, i8* %signature_wait, align 1, !dbg !912
  %149 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !913
  %msg_received168 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %149, i32 0, i32 0, !dbg !915
  %150 = load i8, i8* %msg_received168, align 8, !dbg !915
  %conv169 = zext i8 %150 to i32, !dbg !913
  %cmp170 = icmp ne i32 %conv169, 2, !dbg !916
  br i1 %cmp170, label %if.then172, label %if.end174, !dbg !917

if.then172:                                       ; preds = %if.then166
  %151 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !918
  %msg_received173 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %151, i32 0, i32 0, !dbg !920
  store i8 0, i8* %msg_received173, align 8, !dbg !921
  br label %if.end174, !dbg !922

if.end174:                                        ; preds = %if.then172, %if.then166
  br label %if.end202, !dbg !923

if.else175:                                       ; preds = %if.end159
  %152 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !924
  %signing = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %152, i32 0, i32 11, !dbg !927
  %153 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing, align 8, !dbg !927
  %tobool176 = icmp ne %struct.__mavlink_signing* %153, null, !dbg !924
  br i1 %tobool176, label %land.lhs.true177, label %if.end196, !dbg !928

land.lhs.true177:                                 ; preds = %if.else175
  %154 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !929
  %signing178 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %154, i32 0, i32 11, !dbg !930
  %155 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing178, align 8, !dbg !930
  %accept_unsigned_callback = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %155, i32 0, i32 4, !dbg !931
  %156 = load i1 (%struct.__mavlink_status*, i32)*, i1 (%struct.__mavlink_status*, i32)** %accept_unsigned_callback, align 8, !dbg !931
  %cmp179 = icmp eq i1 (%struct.__mavlink_status*, i32)* %156, null, !dbg !932
  br i1 %cmp179, label %if.then188, label %lor.lhs.false181, !dbg !933

lor.lhs.false181:                                 ; preds = %land.lhs.true177
  %157 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !934
  %signing182 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %157, i32 0, i32 11, !dbg !935
  %158 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing182, align 8, !dbg !935
  %accept_unsigned_callback183 = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %158, i32 0, i32 4, !dbg !936
  %159 = load i1 (%struct.__mavlink_status*, i32)*, i1 (%struct.__mavlink_status*, i32)** %accept_unsigned_callback183, align 8, !dbg !936
  %160 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !937
  %161 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !938
  %msgid184 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %161, i32 0, i32 8, !dbg !939
  %162 = bitcast [3 x i8]* %msgid184 to i24*, !dbg !939
  %bf.load185 = load i24, i24* %162, align 1, !dbg !939
  %bf.cast186 = zext i24 %bf.load185 to i32, !dbg !939
  %call187 = call zeroext i1 %159(%struct.__mavlink_status* %160, i32 %bf.cast186), !dbg !934
  br i1 %call187, label %if.end196, label %if.then188, !dbg !940

if.then188:                                       ; preds = %lor.lhs.false181, %land.lhs.true177
  %163 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !941
  %msg_received189 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %163, i32 0, i32 0, !dbg !944
  %164 = load i8, i8* %msg_received189, align 8, !dbg !944
  %conv190 = zext i8 %164 to i32, !dbg !941
  %cmp191 = icmp ne i32 %conv190, 2, !dbg !945
  br i1 %cmp191, label %if.then193, label %if.end195, !dbg !946

if.then193:                                       ; preds = %if.then188
  %165 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !947
  %msg_received194 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %165, i32 0, i32 0, !dbg !949
  store i8 3, i8* %msg_received194, align 8, !dbg !950
  br label %if.end195, !dbg !951

if.end195:                                        ; preds = %if.then193, %if.then188
  br label %if.end196, !dbg !952

if.end196:                                        ; preds = %if.end195, %lor.lhs.false181, %if.else175
  %166 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !953
  %parse_state197 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %166, i32 0, i32 3, !dbg !954
  store i32 1, i32* %parse_state197, align 4, !dbg !955
  %167 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !956
  %cmp198 = icmp ne %struct.__mavlink_message* %167, null, !dbg !958
  br i1 %cmp198, label %if.then200, label %if.end201, !dbg !959

if.then200:                                       ; preds = %if.end196
  %168 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !960
  %169 = bitcast %struct.__mavlink_message* %168 to i8*, !dbg !962
  %170 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !963
  %171 = bitcast %struct.__mavlink_message* %170 to i8*, !dbg !962
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %169, i8* align 1 %171, i64 291, i1 false), !dbg !962
  br label %if.end201, !dbg !964

if.end201:                                        ; preds = %if.then200, %if.end196
  br label %if.end202

if.end202:                                        ; preds = %if.end201, %if.end174
  br label %sw.epilog, !dbg !965

sw.bb203:                                         ; preds = %entry
  %172 = load i8, i8* %c.addr, align 1, !dbg !966
  %173 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !967
  %signature = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %173, i32 0, i32 11, !dbg !968
  %174 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !969
  %signature_wait204 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %174, i32 0, i32 10, !dbg !970
  %175 = load i8, i8* %signature_wait204, align 1, !dbg !970
  %conv205 = zext i8 %175 to i32, !dbg !969
  %sub206 = sub nsw i32 13, %conv205, !dbg !971
  %idxprom207 = sext i32 %sub206 to i64, !dbg !967
  %arrayidx208 = getelementptr inbounds [13 x i8], [13 x i8]* %signature, i64 0, i64 %idxprom207, !dbg !967
  store i8 %172, i8* %arrayidx208, align 1, !dbg !972
  %176 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !973
  %signature_wait209 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %176, i32 0, i32 10, !dbg !974
  %177 = load i8, i8* %signature_wait209, align 1, !dbg !975
  %dec = add i8 %177, -1, !dbg !975
  store i8 %dec, i8* %signature_wait209, align 1, !dbg !975
  %178 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !976
  %signature_wait210 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %178, i32 0, i32 10, !dbg !978
  %179 = load i8, i8* %signature_wait210, align 1, !dbg !978
  %conv211 = zext i8 %179 to i32, !dbg !976
  %cmp212 = icmp eq i32 %conv211, 0, !dbg !979
  br i1 %cmp212, label %if.then214, label %if.end243, !dbg !980

if.then214:                                       ; preds = %sw.bb203
  call void @llvm.dbg.declare(metadata i8* %sig_ok, metadata !981, metadata !DIExpression()), !dbg !983
  %180 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !984
  %signing215 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %180, i32 0, i32 11, !dbg !985
  %181 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing215, align 8, !dbg !985
  %182 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !986
  %signing_streams = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %182, i32 0, i32 12, !dbg !987
  %183 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams, align 8, !dbg !987
  %184 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !988
  %call216 = call zeroext i1 @mavlink_signature_check(%struct.__mavlink_signing* %181, %struct.__mavlink_signing_streams* %183, %struct.__mavlink_message* %184), !dbg !989
  %frombool = zext i1 %call216 to i8, !dbg !983
  store i8 %frombool, i8* %sig_ok, align 1, !dbg !983
  %185 = load i8, i8* %sig_ok, align 1, !dbg !990
  %tobool217 = trunc i8 %185 to i1, !dbg !990
  br i1 %tobool217, label %if.end231, label %land.lhs.true218, !dbg !992

land.lhs.true218:                                 ; preds = %if.then214
  %186 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !993
  %signing219 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %186, i32 0, i32 11, !dbg !994
  %187 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing219, align 8, !dbg !994
  %accept_unsigned_callback220 = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %187, i32 0, i32 4, !dbg !995
  %188 = load i1 (%struct.__mavlink_status*, i32)*, i1 (%struct.__mavlink_status*, i32)** %accept_unsigned_callback220, align 8, !dbg !995
  %tobool221 = icmp ne i1 (%struct.__mavlink_status*, i32)* %188, null, !dbg !993
  br i1 %tobool221, label %land.lhs.true222, label %if.end231, !dbg !996

land.lhs.true222:                                 ; preds = %land.lhs.true218
  %189 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !997
  %signing223 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %189, i32 0, i32 11, !dbg !998
  %190 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing223, align 8, !dbg !998
  %accept_unsigned_callback224 = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %190, i32 0, i32 4, !dbg !999
  %191 = load i1 (%struct.__mavlink_status*, i32)*, i1 (%struct.__mavlink_status*, i32)** %accept_unsigned_callback224, align 8, !dbg !999
  %192 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1000
  %193 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !1001
  %msgid225 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %193, i32 0, i32 8, !dbg !1002
  %194 = bitcast [3 x i8]* %msgid225 to i24*, !dbg !1002
  %bf.load226 = load i24, i24* %194, align 1, !dbg !1002
  %bf.cast227 = zext i24 %bf.load226 to i32, !dbg !1002
  %call228 = call zeroext i1 %191(%struct.__mavlink_status* %192, i32 %bf.cast227), !dbg !997
  br i1 %call228, label %if.then230, label %if.end231, !dbg !1003

if.then230:                                       ; preds = %land.lhs.true222
  store i8 1, i8* %sig_ok, align 1, !dbg !1004
  br label %if.end231, !dbg !1006

if.end231:                                        ; preds = %if.then230, %land.lhs.true222, %land.lhs.true218, %if.then214
  %195 = load i8, i8* %sig_ok, align 1, !dbg !1007
  %tobool232 = trunc i8 %195 to i1, !dbg !1007
  br i1 %tobool232, label %if.then233, label %if.else235, !dbg !1009

if.then233:                                       ; preds = %if.end231
  %196 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1010
  %msg_received234 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %196, i32 0, i32 0, !dbg !1012
  store i8 1, i8* %msg_received234, align 8, !dbg !1013
  br label %if.end237, !dbg !1014

if.else235:                                       ; preds = %if.end231
  %197 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1015
  %msg_received236 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %197, i32 0, i32 0, !dbg !1017
  store i8 3, i8* %msg_received236, align 8, !dbg !1018
  br label %if.end237

if.end237:                                        ; preds = %if.else235, %if.then233
  %198 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1019
  %parse_state238 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %198, i32 0, i32 3, !dbg !1020
  store i32 1, i32* %parse_state238, align 4, !dbg !1021
  %199 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !1022
  %cmp239 = icmp ne %struct.__mavlink_message* %199, null, !dbg !1024
  br i1 %cmp239, label %if.then241, label %if.end242, !dbg !1025

if.then241:                                       ; preds = %if.end237
  %200 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !1026
  %201 = bitcast %struct.__mavlink_message* %200 to i8*, !dbg !1028
  %202 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !1029
  %203 = bitcast %struct.__mavlink_message* %202 to i8*, !dbg !1028
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %201, i8* align 1 %203, i64 291, i1 false), !dbg !1028
  br label %if.end242, !dbg !1030

if.end242:                                        ; preds = %if.then241, %if.end237
  br label %if.end243, !dbg !1031

if.end243:                                        ; preds = %if.end242, %sw.bb203
  br label %sw.epilog, !dbg !1032

sw.epilog:                                        ; preds = %entry, %if.end243, %if.end202, %if.end145, %if.end108, %if.end95, %sw.bb73, %if.end72, %sw.bb52, %sw.bb50, %sw.bb48, %sw.bb45, %if.end43, %if.then40, %if.end32, %if.end15
  %204 = load i32, i32* %bufferIndex, align 4, !dbg !1033
  %inc244 = add nsw i32 %204, 1, !dbg !1033
  store i32 %inc244, i32* %bufferIndex, align 4, !dbg !1033
  %205 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1034
  %msg_received245 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %205, i32 0, i32 0, !dbg !1036
  %206 = load i8, i8* %msg_received245, align 8, !dbg !1036
  %conv246 = zext i8 %206 to i32, !dbg !1034
  %cmp247 = icmp eq i32 %conv246, 1, !dbg !1037
  br i1 %cmp247, label %if.then249, label %if.end258, !dbg !1038

if.then249:                                       ; preds = %sw.epilog
  %207 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !1039
  %seq250 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %207, i32 0, i32 5, !dbg !1041
  %208 = load i8, i8* %seq250, align 1, !dbg !1041
  %209 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1042
  %current_rx_seq = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %209, i32 0, i32 5, !dbg !1043
  store i8 %208, i8* %current_rx_seq, align 1, !dbg !1044
  %210 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1045
  %packet_rx_success_count = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %210, i32 0, i32 7, !dbg !1047
  %211 = load i16, i16* %packet_rx_success_count, align 4, !dbg !1047
  %conv251 = zext i16 %211 to i32, !dbg !1045
  %cmp252 = icmp eq i32 %conv251, 0, !dbg !1048
  br i1 %cmp252, label %if.then254, label %if.end255, !dbg !1049

if.then254:                                       ; preds = %if.then249
  %212 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1050
  %packet_rx_drop_count = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %212, i32 0, i32 8, !dbg !1051
  store i16 0, i16* %packet_rx_drop_count, align 2, !dbg !1052
  br label %if.end255, !dbg !1050

if.end255:                                        ; preds = %if.then254, %if.then249
  %213 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1053
  %packet_rx_success_count256 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %213, i32 0, i32 7, !dbg !1054
  %214 = load i16, i16* %packet_rx_success_count256, align 4, !dbg !1055
  %inc257 = add i16 %214, 1, !dbg !1055
  store i16 %inc257, i16* %packet_rx_success_count256, align 4, !dbg !1055
  br label %if.end258, !dbg !1056

if.end258:                                        ; preds = %if.end255, %sw.epilog
  %215 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !1057
  %cmp259 = icmp ne %struct.__mavlink_message* %215, null, !dbg !1059
  br i1 %cmp259, label %if.then261, label %if.end264, !dbg !1060

if.then261:                                       ; preds = %if.end258
  %216 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !1061
  %len262 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %216, i32 0, i32 2, !dbg !1063
  %217 = load i8, i8* %len262, align 1, !dbg !1063
  %218 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !1064
  %len263 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %218, i32 0, i32 2, !dbg !1065
  store i8 %217, i8* %len263, align 1, !dbg !1066
  br label %if.end264, !dbg !1067

if.end264:                                        ; preds = %if.then261, %if.end258
  %219 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !1068
  %cmp265 = icmp ne %struct.__mavlink_status* %219, null, !dbg !1070
  br i1 %cmp265, label %if.then267, label %if.end282, !dbg !1071

if.then267:                                       ; preds = %if.end264
  %220 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1072
  %parse_state268 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %220, i32 0, i32 3, !dbg !1074
  %221 = load i32, i32* %parse_state268, align 4, !dbg !1074
  %222 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !1075
  %parse_state269 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %222, i32 0, i32 3, !dbg !1076
  store i32 %221, i32* %parse_state269, align 4, !dbg !1077
  %223 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1078
  %packet_idx270 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %223, i32 0, i32 4, !dbg !1079
  %224 = load i8, i8* %packet_idx270, align 8, !dbg !1079
  %225 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !1080
  %packet_idx271 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %225, i32 0, i32 4, !dbg !1081
  store i8 %224, i8* %packet_idx271, align 8, !dbg !1082
  %226 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1083
  %current_rx_seq272 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %226, i32 0, i32 5, !dbg !1084
  %227 = load i8, i8* %current_rx_seq272, align 1, !dbg !1084
  %conv273 = zext i8 %227 to i32, !dbg !1083
  %add = add nsw i32 %conv273, 1, !dbg !1085
  %conv274 = trunc i32 %add to i8, !dbg !1083
  %228 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !1086
  %current_rx_seq275 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %228, i32 0, i32 5, !dbg !1087
  store i8 %conv274, i8* %current_rx_seq275, align 1, !dbg !1088
  %229 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1089
  %packet_rx_success_count276 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %229, i32 0, i32 7, !dbg !1090
  %230 = load i16, i16* %packet_rx_success_count276, align 4, !dbg !1090
  %231 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !1091
  %packet_rx_success_count277 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %231, i32 0, i32 7, !dbg !1092
  store i16 %230, i16* %packet_rx_success_count277, align 4, !dbg !1093
  %232 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1094
  %parse_error = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %232, i32 0, i32 2, !dbg !1095
  %233 = load i8, i8* %parse_error, align 2, !dbg !1095
  %conv278 = zext i8 %233 to i16, !dbg !1094
  %234 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !1096
  %packet_rx_drop_count279 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %234, i32 0, i32 8, !dbg !1097
  store i16 %conv278, i16* %packet_rx_drop_count279, align 2, !dbg !1098
  %235 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1099
  %flags280 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %235, i32 0, i32 9, !dbg !1100
  %236 = load i8, i8* %flags280, align 8, !dbg !1100
  %237 = load %struct.__mavlink_status*, %struct.__mavlink_status** %r_mavlink_status.addr, align 8, !dbg !1101
  %flags281 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %237, i32 0, i32 9, !dbg !1102
  store i8 %236, i8* %flags281, align 8, !dbg !1103
  br label %if.end282, !dbg !1104

if.end282:                                        ; preds = %if.then267, %if.end264
  %238 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1105
  %parse_error283 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %238, i32 0, i32 2, !dbg !1106
  store i8 0, i8* %parse_error283, align 2, !dbg !1107
  %239 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1108
  %msg_received284 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %239, i32 0, i32 0, !dbg !1110
  %240 = load i8, i8* %msg_received284, align 8, !dbg !1110
  %conv285 = zext i8 %240 to i32, !dbg !1108
  %cmp286 = icmp eq i32 %conv285, 2, !dbg !1111
  br i1 %cmp286, label %if.then288, label %if.end303, !dbg !1112

if.then288:                                       ; preds = %if.end282
  %241 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !1113
  %cmp289 = icmp ne %struct.__mavlink_message* %241, null, !dbg !1116
  br i1 %cmp289, label %if.then291, label %if.end302, !dbg !1117

if.then291:                                       ; preds = %if.then288
  %242 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !1118
  %ck292 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %242, i32 0, i32 10, !dbg !1120
  %arrayidx293 = getelementptr inbounds [2 x i8], [2 x i8]* %ck292, i64 0, i64 0, !dbg !1118
  %243 = load i8, i8* %arrayidx293, align 1, !dbg !1118
  %conv294 = zext i8 %243 to i32, !dbg !1118
  %244 = load %struct.__mavlink_message*, %struct.__mavlink_message** %rxmsg.addr, align 8, !dbg !1121
  %ck295 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %244, i32 0, i32 10, !dbg !1122
  %arrayidx296 = getelementptr inbounds [2 x i8], [2 x i8]* %ck295, i64 0, i64 1, !dbg !1121
  %245 = load i8, i8* %arrayidx296, align 1, !dbg !1121
  %conv297 = zext i8 %245 to i32, !dbg !1121
  %shl298 = shl i32 %conv297, 8, !dbg !1123
  %or299 = or i32 %conv294, %shl298, !dbg !1124
  %conv300 = trunc i32 %or299 to i16, !dbg !1118
  %246 = load %struct.__mavlink_message*, %struct.__mavlink_message** %r_message.addr, align 8, !dbg !1125
  %checksum301 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %246, i32 0, i32 0, !dbg !1126
  store i16 %conv300, i16* %checksum301, align 1, !dbg !1127
  br label %if.end302, !dbg !1128

if.end302:                                        ; preds = %if.then291, %if.then288
  br label %if.end303, !dbg !1129

if.end303:                                        ; preds = %if.end302, %if.end282
  %247 = load %struct.__mavlink_status*, %struct.__mavlink_status** %status.addr, align 8, !dbg !1130
  %msg_received304 = getelementptr inbounds %struct.__mavlink_status, %struct.__mavlink_status* %247, i32 0, i32 0, !dbg !1131
  %248 = load i8, i8* %msg_received304, align 8, !dbg !1131
  ret i8 %248, !dbg !1132
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_update_checksum(%struct.__mavlink_message* %msg, i8 zeroext %c) #0 !dbg !1133 {
entry:
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %c.addr = alloca i8, align 1
  %checksum = alloca i16, align 2
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !1136, metadata !DIExpression()), !dbg !1137
  store i8 %c, i8* %c.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %c.addr, metadata !1138, metadata !DIExpression()), !dbg !1139
  call void @llvm.dbg.declare(metadata i16* %checksum, metadata !1140, metadata !DIExpression()), !dbg !1141
  %0 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1142
  %checksum1 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %0, i32 0, i32 0, !dbg !1143
  %1 = load i16, i16* %checksum1, align 1, !dbg !1143
  store i16 %1, i16* %checksum, align 2, !dbg !1141
  %2 = load i8, i8* %c.addr, align 1, !dbg !1144
  call void @crc_accumulate(i8 zeroext %2, i16* %checksum), !dbg !1145
  %3 = load i16, i16* %checksum, align 2, !dbg !1146
  %4 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1147
  %checksum2 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %4, i32 0, i32 0, !dbg !1148
  store i16 %3, i16* %checksum2, align 1, !dbg !1149
  ret void, !dbg !1150
}

; Function Attrs: noinline nounwind optnone uwtable
define internal %struct.__mavlink_msg_entry* @mavlink_get_msg_entry(i32 %msgid) #0 !dbg !2 {
entry:
  %retval = alloca %struct.__mavlink_msg_entry*, align 8
  %msgid.addr = alloca i32, align 4
  %low = alloca i32, align 4
  %high = alloca i32, align 4
  %mid = alloca i32, align 4
  store i32 %msgid, i32* %msgid.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %msgid.addr, metadata !1151, metadata !DIExpression()), !dbg !1152
  call void @llvm.dbg.declare(metadata i32* %low, metadata !1153, metadata !DIExpression()), !dbg !1154
  store i32 0, i32* %low, align 4, !dbg !1154
  call void @llvm.dbg.declare(metadata i32* %high, metadata !1155, metadata !DIExpression()), !dbg !1156
  store i32 195, i32* %high, align 4, !dbg !1156
  br label %while.cond, !dbg !1157

while.cond:                                       ; preds = %if.then8, %if.then, %entry
  %0 = load i32, i32* %low, align 4, !dbg !1158
  %1 = load i32, i32* %high, align 4, !dbg !1159
  %cmp = icmp ult i32 %0, %1, !dbg !1160
  br i1 %cmp, label %while.body, label %while.end, !dbg !1157

while.body:                                       ; preds = %while.cond
  call void @llvm.dbg.declare(metadata i32* %mid, metadata !1161, metadata !DIExpression()), !dbg !1163
  %2 = load i32, i32* %low, align 4, !dbg !1164
  %add = add i32 %2, 1, !dbg !1165
  %3 = load i32, i32* %high, align 4, !dbg !1166
  %add1 = add i32 %add, %3, !dbg !1167
  %div = udiv i32 %add1, 2, !dbg !1168
  store i32 %div, i32* %mid, align 4, !dbg !1163
  %4 = load i32, i32* %msgid.addr, align 4, !dbg !1169
  %5 = load i32, i32* %mid, align 4, !dbg !1171
  %idxprom = zext i32 %5 to i64, !dbg !1172
  %arrayidx = getelementptr inbounds [196 x %struct.__mavlink_msg_entry], [196 x %struct.__mavlink_msg_entry]* @mavlink_get_msg_entry.mavlink_message_crcs, i64 0, i64 %idxprom, !dbg !1172
  %msgid2 = getelementptr inbounds %struct.__mavlink_msg_entry, %struct.__mavlink_msg_entry* %arrayidx, i32 0, i32 0, !dbg !1173
  %6 = load i32, i32* %msgid2, align 4, !dbg !1173
  %cmp3 = icmp ult i32 %4, %6, !dbg !1174
  br i1 %cmp3, label %if.then, label %if.end, !dbg !1175

if.then:                                          ; preds = %while.body
  %7 = load i32, i32* %mid, align 4, !dbg !1176
  %sub = sub i32 %7, 1, !dbg !1178
  store i32 %sub, i32* %high, align 4, !dbg !1179
  br label %while.cond, !dbg !1180, !llvm.loop !1181

if.end:                                           ; preds = %while.body
  %8 = load i32, i32* %msgid.addr, align 4, !dbg !1183
  %9 = load i32, i32* %mid, align 4, !dbg !1185
  %idxprom4 = zext i32 %9 to i64, !dbg !1186
  %arrayidx5 = getelementptr inbounds [196 x %struct.__mavlink_msg_entry], [196 x %struct.__mavlink_msg_entry]* @mavlink_get_msg_entry.mavlink_message_crcs, i64 0, i64 %idxprom4, !dbg !1186
  %msgid6 = getelementptr inbounds %struct.__mavlink_msg_entry, %struct.__mavlink_msg_entry* %arrayidx5, i32 0, i32 0, !dbg !1187
  %10 = load i32, i32* %msgid6, align 4, !dbg !1187
  %cmp7 = icmp ugt i32 %8, %10, !dbg !1188
  br i1 %cmp7, label %if.then8, label %if.end9, !dbg !1189

if.then8:                                         ; preds = %if.end
  %11 = load i32, i32* %mid, align 4, !dbg !1190
  store i32 %11, i32* %low, align 4, !dbg !1192
  br label %while.cond, !dbg !1193, !llvm.loop !1181

if.end9:                                          ; preds = %if.end
  %12 = load i32, i32* %mid, align 4, !dbg !1194
  store i32 %12, i32* %low, align 4, !dbg !1195
  br label %while.end, !dbg !1196

while.end:                                        ; preds = %if.end9, %while.cond
  %13 = load i32, i32* %low, align 4, !dbg !1197
  %idxprom10 = zext i32 %13 to i64, !dbg !1199
  %arrayidx11 = getelementptr inbounds [196 x %struct.__mavlink_msg_entry], [196 x %struct.__mavlink_msg_entry]* @mavlink_get_msg_entry.mavlink_message_crcs, i64 0, i64 %idxprom10, !dbg !1199
  %msgid12 = getelementptr inbounds %struct.__mavlink_msg_entry, %struct.__mavlink_msg_entry* %arrayidx11, i32 0, i32 0, !dbg !1200
  %14 = load i32, i32* %msgid12, align 4, !dbg !1200
  %15 = load i32, i32* %msgid.addr, align 4, !dbg !1201
  %cmp13 = icmp ne i32 %14, %15, !dbg !1202
  br i1 %cmp13, label %if.then14, label %if.end15, !dbg !1203

if.then14:                                        ; preds = %while.end
  store %struct.__mavlink_msg_entry* null, %struct.__mavlink_msg_entry** %retval, align 8, !dbg !1204
  br label %return, !dbg !1204

if.end15:                                         ; preds = %while.end
  %16 = load i32, i32* %low, align 4, !dbg !1206
  %idxprom16 = zext i32 %16 to i64, !dbg !1207
  %arrayidx17 = getelementptr inbounds [196 x %struct.__mavlink_msg_entry], [196 x %struct.__mavlink_msg_entry]* @mavlink_get_msg_entry.mavlink_message_crcs, i64 0, i64 %idxprom16, !dbg !1207
  store %struct.__mavlink_msg_entry* %arrayidx17, %struct.__mavlink_msg_entry** %retval, align 8, !dbg !1208
  br label %return, !dbg !1208

return:                                           ; preds = %if.end15, %if.then14
  %17 = load %struct.__mavlink_msg_entry*, %struct.__mavlink_msg_entry** %retval, align 8, !dbg !1209
  ret %struct.__mavlink_msg_entry* %17, !dbg !1209
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #5

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i1 @mavlink_signature_check(%struct.__mavlink_signing* %signing, %struct.__mavlink_signing_streams* %signing_streams, %struct.__mavlink_message* %msg) #0 !dbg !1210 {
entry:
  %retval = alloca i1, align 1
  %signing.addr = alloca %struct.__mavlink_signing*, align 8
  %signing_streams.addr = alloca %struct.__mavlink_signing_streams*, align 8
  %msg.addr = alloca %struct.__mavlink_message*, align 8
  %p = alloca i8*, align 8
  %psig = alloca i8*, align 8
  %incoming_signature = alloca i8*, align 8
  %ctx = alloca %struct.mavlink_sha256_ctx, align 4
  %signature1 = alloca [6 x i8], align 1
  %i = alloca i16, align 2
  %tstamp = alloca %union.tstamp, align 8
  %link_id = alloca i8, align 1
  %last_tstamp = alloca %union.tstamp, align 8
  store %struct.__mavlink_signing* %signing, %struct.__mavlink_signing** %signing.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_signing** %signing.addr, metadata !1217, metadata !DIExpression()), !dbg !1218
  store %struct.__mavlink_signing_streams* %signing_streams, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_signing_streams** %signing_streams.addr, metadata !1219, metadata !DIExpression()), !dbg !1220
  store %struct.__mavlink_message* %msg, %struct.__mavlink_message** %msg.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.__mavlink_message** %msg.addr, metadata !1221, metadata !DIExpression()), !dbg !1222
  %0 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1223
  %cmp = icmp eq %struct.__mavlink_signing* %0, null, !dbg !1225
  br i1 %cmp, label %if.then, label %if.end, !dbg !1226

if.then:                                          ; preds = %entry
  store i1 true, i1* %retval, align 1, !dbg !1227
  br label %return, !dbg !1227

if.end:                                           ; preds = %entry
  call void @llvm.dbg.declare(metadata i8** %p, metadata !1229, metadata !DIExpression()), !dbg !1230
  %1 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1231
  %magic = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %1, i32 0, i32 1, !dbg !1232
  store i8* %magic, i8** %p, align 8, !dbg !1230
  call void @llvm.dbg.declare(metadata i8** %psig, metadata !1233, metadata !DIExpression()), !dbg !1234
  %2 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1235
  %signature = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %2, i32 0, i32 11, !dbg !1236
  %arraydecay = getelementptr inbounds [13 x i8], [13 x i8]* %signature, i64 0, i64 0, !dbg !1235
  store i8* %arraydecay, i8** %psig, align 8, !dbg !1234
  call void @llvm.dbg.declare(metadata i8** %incoming_signature, metadata !1237, metadata !DIExpression()), !dbg !1238
  %3 = load i8*, i8** %psig, align 8, !dbg !1239
  %add.ptr = getelementptr inbounds i8, i8* %3, i64 7, !dbg !1240
  store i8* %add.ptr, i8** %incoming_signature, align 8, !dbg !1238
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx* %ctx, metadata !1241, metadata !DIExpression()), !dbg !1256
  call void @llvm.dbg.declare(metadata [6 x i8]* %signature1, metadata !1257, metadata !DIExpression()), !dbg !1258
  call void @llvm.dbg.declare(metadata i16* %i, metadata !1259, metadata !DIExpression()), !dbg !1260
  call void @mavlink_sha256_init(%struct.mavlink_sha256_ctx* %ctx), !dbg !1261
  %4 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1262
  %secret_key = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %4, i32 0, i32 3, !dbg !1263
  %arraydecay2 = getelementptr inbounds [32 x i8], [32 x i8]* %secret_key, i64 0, i64 0, !dbg !1262
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %arraydecay2, i32 32), !dbg !1264
  %5 = load i8*, i8** %p, align 8, !dbg !1265
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %5, i32 10), !dbg !1266
  %6 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1267
  %payload64 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %6, i32 0, i32 9, !dbg !1267
  %arrayidx = getelementptr inbounds [33 x i64], [33 x i64]* %payload64, i64 0, i64 0, !dbg !1267
  %7 = bitcast i64* %arrayidx to i8*, !dbg !1267
  %8 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1268
  %len = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %8, i32 0, i32 2, !dbg !1269
  %9 = load i8, i8* %len, align 1, !dbg !1269
  %conv = zext i8 %9 to i32, !dbg !1268
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %7, i32 %conv), !dbg !1270
  %10 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1271
  %ck = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %10, i32 0, i32 10, !dbg !1272
  %arraydecay3 = getelementptr inbounds [2 x i8], [2 x i8]* %ck, i64 0, i64 0, !dbg !1271
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %arraydecay3, i32 2), !dbg !1273
  %11 = load i8*, i8** %psig, align 8, !dbg !1274
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %ctx, i8* %11, i32 7), !dbg !1275
  %arraydecay4 = getelementptr inbounds [6 x i8], [6 x i8]* %signature1, i64 0, i64 0, !dbg !1276
  call void @mavlink_sha256_final_48(%struct.mavlink_sha256_ctx* %ctx, i8* %arraydecay4), !dbg !1277
  %arraydecay5 = getelementptr inbounds [6 x i8], [6 x i8]* %signature1, i64 0, i64 0, !dbg !1278
  %12 = load i8*, i8** %incoming_signature, align 8, !dbg !1280
  %call = call i32 @memcmp(i8* %arraydecay5, i8* %12, i64 6) #9, !dbg !1281
  %cmp6 = icmp ne i32 %call, 0, !dbg !1282
  br i1 %cmp6, label %if.then8, label %if.end9, !dbg !1283

if.then8:                                         ; preds = %if.end
  store i1 false, i1* %retval, align 1, !dbg !1284
  br label %return, !dbg !1284

if.end9:                                          ; preds = %if.end
  call void @llvm.dbg.declare(metadata %union.tstamp* %tstamp, metadata !1286, metadata !DIExpression()), !dbg !1292
  call void @llvm.dbg.declare(metadata i8* %link_id, metadata !1293, metadata !DIExpression()), !dbg !1294
  %13 = load i8*, i8** %psig, align 8, !dbg !1295
  %arrayidx10 = getelementptr inbounds i8, i8* %13, i64 0, !dbg !1295
  %14 = load i8, i8* %arrayidx10, align 1, !dbg !1295
  store i8 %14, i8* %link_id, align 1, !dbg !1294
  %t64 = bitcast %union.tstamp* %tstamp to i64*, !dbg !1296
  store i64 0, i64* %t64, align 8, !dbg !1297
  %t8 = bitcast %union.tstamp* %tstamp to [8 x i8]*, !dbg !1298
  %arraydecay11 = getelementptr inbounds [8 x i8], [8 x i8]* %t8, i64 0, i64 0, !dbg !1299
  %15 = load i8*, i8** %psig, align 8, !dbg !1300
  %add.ptr12 = getelementptr inbounds i8, i8* %15, i64 1, !dbg !1301
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %arraydecay11, i8* align 1 %add.ptr12, i64 6, i1 false), !dbg !1299
  %16 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1302
  %cmp13 = icmp eq %struct.__mavlink_signing_streams* %16, null, !dbg !1304
  br i1 %cmp13, label %if.then15, label %if.end16, !dbg !1305

if.then15:                                        ; preds = %if.end9
  store i1 false, i1* %retval, align 1, !dbg !1306
  br label %return, !dbg !1306

if.end16:                                         ; preds = %if.end9
  store i16 0, i16* %i, align 2, !dbg !1308
  br label %for.cond, !dbg !1310

for.cond:                                         ; preds = %for.inc, %if.end16
  %17 = load i16, i16* %i, align 2, !dbg !1311
  %conv17 = zext i16 %17 to i32, !dbg !1311
  %18 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1313
  %num_signing_streams = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %18, i32 0, i32 0, !dbg !1314
  %19 = load i16, i16* %num_signing_streams, align 2, !dbg !1314
  %conv18 = zext i16 %19 to i32, !dbg !1313
  %cmp19 = icmp slt i32 %conv17, %conv18, !dbg !1315
  br i1 %cmp19, label %for.body, label %for.end, !dbg !1316

for.body:                                         ; preds = %for.cond
  %20 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1317
  %sysid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %20, i32 0, i32 6, !dbg !1320
  %21 = load i8, i8* %sysid, align 1, !dbg !1320
  %conv21 = zext i8 %21 to i32, !dbg !1317
  %22 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1321
  %stream = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %22, i32 0, i32 1, !dbg !1322
  %23 = load i16, i16* %i, align 2, !dbg !1323
  %idxprom = zext i16 %23 to i64, !dbg !1321
  %arrayidx22 = getelementptr inbounds [16 x %struct.__mavlink_signing_stream], [16 x %struct.__mavlink_signing_stream]* %stream, i64 0, i64 %idxprom, !dbg !1321
  %sysid23 = getelementptr inbounds %struct.__mavlink_signing_stream, %struct.__mavlink_signing_stream* %arrayidx22, i32 0, i32 1, !dbg !1324
  %24 = load i8, i8* %sysid23, align 1, !dbg !1324
  %conv24 = zext i8 %24 to i32, !dbg !1321
  %cmp25 = icmp eq i32 %conv21, %conv24, !dbg !1325
  br i1 %cmp25, label %land.lhs.true, label %if.end45, !dbg !1326

land.lhs.true:                                    ; preds = %for.body
  %25 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1327
  %compid = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %25, i32 0, i32 7, !dbg !1328
  %26 = load i8, i8* %compid, align 1, !dbg !1328
  %conv27 = zext i8 %26 to i32, !dbg !1327
  %27 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1329
  %stream28 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %27, i32 0, i32 1, !dbg !1330
  %28 = load i16, i16* %i, align 2, !dbg !1331
  %idxprom29 = zext i16 %28 to i64, !dbg !1329
  %arrayidx30 = getelementptr inbounds [16 x %struct.__mavlink_signing_stream], [16 x %struct.__mavlink_signing_stream]* %stream28, i64 0, i64 %idxprom29, !dbg !1329
  %compid31 = getelementptr inbounds %struct.__mavlink_signing_stream, %struct.__mavlink_signing_stream* %arrayidx30, i32 0, i32 2, !dbg !1332
  %29 = load i8, i8* %compid31, align 1, !dbg !1332
  %conv32 = zext i8 %29 to i32, !dbg !1329
  %cmp33 = icmp eq i32 %conv27, %conv32, !dbg !1333
  br i1 %cmp33, label %land.lhs.true35, label %if.end45, !dbg !1334

land.lhs.true35:                                  ; preds = %land.lhs.true
  %30 = load i8, i8* %link_id, align 1, !dbg !1335
  %conv36 = zext i8 %30 to i32, !dbg !1335
  %31 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1336
  %stream37 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %31, i32 0, i32 1, !dbg !1337
  %32 = load i16, i16* %i, align 2, !dbg !1338
  %idxprom38 = zext i16 %32 to i64, !dbg !1336
  %arrayidx39 = getelementptr inbounds [16 x %struct.__mavlink_signing_stream], [16 x %struct.__mavlink_signing_stream]* %stream37, i64 0, i64 %idxprom38, !dbg !1336
  %link_id40 = getelementptr inbounds %struct.__mavlink_signing_stream, %struct.__mavlink_signing_stream* %arrayidx39, i32 0, i32 0, !dbg !1339
  %33 = load i8, i8* %link_id40, align 1, !dbg !1339
  %conv41 = zext i8 %33 to i32, !dbg !1336
  %cmp42 = icmp eq i32 %conv36, %conv41, !dbg !1340
  br i1 %cmp42, label %if.then44, label %if.end45, !dbg !1341

if.then44:                                        ; preds = %land.lhs.true35
  br label %for.end, !dbg !1342

if.end45:                                         ; preds = %land.lhs.true35, %land.lhs.true, %for.body
  br label %for.inc, !dbg !1344

for.inc:                                          ; preds = %if.end45
  %34 = load i16, i16* %i, align 2, !dbg !1345
  %inc = add i16 %34, 1, !dbg !1345
  store i16 %inc, i16* %i, align 2, !dbg !1345
  br label %for.cond, !dbg !1346, !llvm.loop !1347

for.end:                                          ; preds = %if.then44, %for.cond
  %35 = load i16, i16* %i, align 2, !dbg !1349
  %conv46 = zext i16 %35 to i32, !dbg !1349
  %36 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1351
  %num_signing_streams47 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %36, i32 0, i32 0, !dbg !1352
  %37 = load i16, i16* %num_signing_streams47, align 2, !dbg !1352
  %conv48 = zext i16 %37 to i32, !dbg !1351
  %cmp49 = icmp eq i32 %conv46, %conv48, !dbg !1353
  br i1 %cmp49, label %if.then51, label %if.else, !dbg !1354

if.then51:                                        ; preds = %for.end
  %38 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1355
  %num_signing_streams52 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %38, i32 0, i32 0, !dbg !1358
  %39 = load i16, i16* %num_signing_streams52, align 2, !dbg !1358
  %conv53 = zext i16 %39 to i32, !dbg !1355
  %cmp54 = icmp sge i32 %conv53, 16, !dbg !1359
  br i1 %cmp54, label %if.then56, label %if.end57, !dbg !1360

if.then56:                                        ; preds = %if.then51
  store i1 false, i1* %retval, align 1, !dbg !1361
  br label %return, !dbg !1361

if.end57:                                         ; preds = %if.then51
  %t6458 = bitcast %union.tstamp* %tstamp to i64*, !dbg !1363
  %40 = load i64, i64* %t6458, align 8, !dbg !1363
  %add = add i64 %40, 6000000, !dbg !1365
  %41 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1366
  %timestamp = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %41, i32 0, i32 2, !dbg !1367
  %42 = load i64, i64* %timestamp, align 8, !dbg !1367
  %cmp59 = icmp ult i64 %add, %42, !dbg !1368
  br i1 %cmp59, label %if.then61, label %if.end62, !dbg !1369

if.then61:                                        ; preds = %if.end57
  store i1 false, i1* %retval, align 1, !dbg !1370
  br label %return, !dbg !1370

if.end62:                                         ; preds = %if.end57
  %43 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1372
  %sysid63 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %43, i32 0, i32 6, !dbg !1373
  %44 = load i8, i8* %sysid63, align 1, !dbg !1373
  %45 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1374
  %stream64 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %45, i32 0, i32 1, !dbg !1375
  %46 = load i16, i16* %i, align 2, !dbg !1376
  %idxprom65 = zext i16 %46 to i64, !dbg !1374
  %arrayidx66 = getelementptr inbounds [16 x %struct.__mavlink_signing_stream], [16 x %struct.__mavlink_signing_stream]* %stream64, i64 0, i64 %idxprom65, !dbg !1374
  %sysid67 = getelementptr inbounds %struct.__mavlink_signing_stream, %struct.__mavlink_signing_stream* %arrayidx66, i32 0, i32 1, !dbg !1377
  store i8 %44, i8* %sysid67, align 1, !dbg !1378
  %47 = load %struct.__mavlink_message*, %struct.__mavlink_message** %msg.addr, align 8, !dbg !1379
  %compid68 = getelementptr inbounds %struct.__mavlink_message, %struct.__mavlink_message* %47, i32 0, i32 7, !dbg !1380
  %48 = load i8, i8* %compid68, align 1, !dbg !1380
  %49 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1381
  %stream69 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %49, i32 0, i32 1, !dbg !1382
  %50 = load i16, i16* %i, align 2, !dbg !1383
  %idxprom70 = zext i16 %50 to i64, !dbg !1381
  %arrayidx71 = getelementptr inbounds [16 x %struct.__mavlink_signing_stream], [16 x %struct.__mavlink_signing_stream]* %stream69, i64 0, i64 %idxprom70, !dbg !1381
  %compid72 = getelementptr inbounds %struct.__mavlink_signing_stream, %struct.__mavlink_signing_stream* %arrayidx71, i32 0, i32 2, !dbg !1384
  store i8 %48, i8* %compid72, align 1, !dbg !1385
  %51 = load i8, i8* %link_id, align 1, !dbg !1386
  %52 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1387
  %stream73 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %52, i32 0, i32 1, !dbg !1388
  %53 = load i16, i16* %i, align 2, !dbg !1389
  %idxprom74 = zext i16 %53 to i64, !dbg !1387
  %arrayidx75 = getelementptr inbounds [16 x %struct.__mavlink_signing_stream], [16 x %struct.__mavlink_signing_stream]* %stream73, i64 0, i64 %idxprom74, !dbg !1387
  %link_id76 = getelementptr inbounds %struct.__mavlink_signing_stream, %struct.__mavlink_signing_stream* %arrayidx75, i32 0, i32 0, !dbg !1390
  store i8 %51, i8* %link_id76, align 1, !dbg !1391
  %54 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1392
  %num_signing_streams77 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %54, i32 0, i32 0, !dbg !1393
  %55 = load i16, i16* %num_signing_streams77, align 2, !dbg !1394
  %inc78 = add i16 %55, 1, !dbg !1394
  store i16 %inc78, i16* %num_signing_streams77, align 2, !dbg !1394
  br label %if.end92, !dbg !1395

if.else:                                          ; preds = %for.end
  call void @llvm.dbg.declare(metadata %union.tstamp* %last_tstamp, metadata !1396, metadata !DIExpression()), !dbg !1398
  %t6479 = bitcast %union.tstamp* %last_tstamp to i64*, !dbg !1399
  store i64 0, i64* %t6479, align 8, !dbg !1400
  %t880 = bitcast %union.tstamp* %last_tstamp to [8 x i8]*, !dbg !1401
  %arraydecay81 = getelementptr inbounds [8 x i8], [8 x i8]* %t880, i64 0, i64 0, !dbg !1402
  %56 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1403
  %stream82 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %56, i32 0, i32 1, !dbg !1404
  %57 = load i16, i16* %i, align 2, !dbg !1405
  %idxprom83 = zext i16 %57 to i64, !dbg !1403
  %arrayidx84 = getelementptr inbounds [16 x %struct.__mavlink_signing_stream], [16 x %struct.__mavlink_signing_stream]* %stream82, i64 0, i64 %idxprom83, !dbg !1403
  %timestamp_bytes = getelementptr inbounds %struct.__mavlink_signing_stream, %struct.__mavlink_signing_stream* %arrayidx84, i32 0, i32 3, !dbg !1406
  %arraydecay85 = getelementptr inbounds [6 x i8], [6 x i8]* %timestamp_bytes, i64 0, i64 0, !dbg !1402
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %arraydecay81, i8* align 1 %arraydecay85, i64 6, i1 false), !dbg !1402
  %t6486 = bitcast %union.tstamp* %tstamp to i64*, !dbg !1407
  %58 = load i64, i64* %t6486, align 8, !dbg !1407
  %t6487 = bitcast %union.tstamp* %last_tstamp to i64*, !dbg !1409
  %59 = load i64, i64* %t6487, align 8, !dbg !1409
  %cmp88 = icmp ule i64 %58, %59, !dbg !1410
  br i1 %cmp88, label %if.then90, label %if.end91, !dbg !1411

if.then90:                                        ; preds = %if.else
  store i1 false, i1* %retval, align 1, !dbg !1412
  br label %return, !dbg !1412

if.end91:                                         ; preds = %if.else
  br label %if.end92

if.end92:                                         ; preds = %if.end91, %if.end62
  %60 = load %struct.__mavlink_signing_streams*, %struct.__mavlink_signing_streams** %signing_streams.addr, align 8, !dbg !1414
  %stream93 = getelementptr inbounds %struct.__mavlink_signing_streams, %struct.__mavlink_signing_streams* %60, i32 0, i32 1, !dbg !1415
  %61 = load i16, i16* %i, align 2, !dbg !1416
  %idxprom94 = zext i16 %61 to i64, !dbg !1414
  %arrayidx95 = getelementptr inbounds [16 x %struct.__mavlink_signing_stream], [16 x %struct.__mavlink_signing_stream]* %stream93, i64 0, i64 %idxprom94, !dbg !1414
  %timestamp_bytes96 = getelementptr inbounds %struct.__mavlink_signing_stream, %struct.__mavlink_signing_stream* %arrayidx95, i32 0, i32 3, !dbg !1417
  %arraydecay97 = getelementptr inbounds [6 x i8], [6 x i8]* %timestamp_bytes96, i64 0, i64 0, !dbg !1418
  %62 = load i8*, i8** %psig, align 8, !dbg !1419
  %add.ptr98 = getelementptr inbounds i8, i8* %62, i64 1, !dbg !1420
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %arraydecay97, i8* align 1 %add.ptr98, i64 6, i1 false), !dbg !1418
  %t6499 = bitcast %union.tstamp* %tstamp to i64*, !dbg !1421
  %63 = load i64, i64* %t6499, align 8, !dbg !1421
  %64 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1423
  %timestamp100 = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %64, i32 0, i32 2, !dbg !1424
  %65 = load i64, i64* %timestamp100, align 8, !dbg !1424
  %cmp101 = icmp ugt i64 %63, %65, !dbg !1425
  br i1 %cmp101, label %if.then103, label %if.end106, !dbg !1426

if.then103:                                       ; preds = %if.end92
  %t64104 = bitcast %union.tstamp* %tstamp to i64*, !dbg !1427
  %66 = load i64, i64* %t64104, align 8, !dbg !1427
  %67 = load %struct.__mavlink_signing*, %struct.__mavlink_signing** %signing.addr, align 8, !dbg !1429
  %timestamp105 = getelementptr inbounds %struct.__mavlink_signing, %struct.__mavlink_signing* %67, i32 0, i32 2, !dbg !1430
  store i64 %66, i64* %timestamp105, align 8, !dbg !1431
  br label %if.end106, !dbg !1432

if.end106:                                        ; preds = %if.then103, %if.end92
  store i1 true, i1* %retval, align 1, !dbg !1433
  br label %return, !dbg !1433

return:                                           ; preds = %if.end106, %if.then90, %if.then61, %if.then56, %if.then15, %if.then8, %if.then
  %68 = load i1, i1* %retval, align 1, !dbg !1434
  ret i1 %68, !dbg !1434
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @crc_accumulate(i8 zeroext %data, i16* %crcAccum) #0 !dbg !1435 {
entry:
  %data.addr = alloca i8, align 1
  %crcAccum.addr = alloca i16*, align 8
  %tmp = alloca i8, align 1
  store i8 %data, i8* %data.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %data.addr, metadata !1440, metadata !DIExpression()), !dbg !1441
  store i16* %crcAccum, i16** %crcAccum.addr, align 8
  call void @llvm.dbg.declare(metadata i16** %crcAccum.addr, metadata !1442, metadata !DIExpression()), !dbg !1443
  call void @llvm.dbg.declare(metadata i8* %tmp, metadata !1444, metadata !DIExpression()), !dbg !1445
  %0 = load i8, i8* %data.addr, align 1, !dbg !1446
  %conv = zext i8 %0 to i32, !dbg !1446
  %1 = load i16*, i16** %crcAccum.addr, align 8, !dbg !1447
  %2 = load i16, i16* %1, align 2, !dbg !1448
  %conv1 = zext i16 %2 to i32, !dbg !1448
  %and = and i32 %conv1, 255, !dbg !1449
  %conv2 = trunc i32 %and to i8, !dbg !1450
  %conv3 = zext i8 %conv2 to i32, !dbg !1450
  %xor = xor i32 %conv, %conv3, !dbg !1451
  %conv4 = trunc i32 %xor to i8, !dbg !1446
  store i8 %conv4, i8* %tmp, align 1, !dbg !1452
  %3 = load i8, i8* %tmp, align 1, !dbg !1453
  %conv5 = zext i8 %3 to i32, !dbg !1453
  %shl = shl i32 %conv5, 4, !dbg !1454
  %4 = load i8, i8* %tmp, align 1, !dbg !1455
  %conv6 = zext i8 %4 to i32, !dbg !1455
  %xor7 = xor i32 %conv6, %shl, !dbg !1455
  %conv8 = trunc i32 %xor7 to i8, !dbg !1455
  store i8 %conv8, i8* %tmp, align 1, !dbg !1455
  %5 = load i16*, i16** %crcAccum.addr, align 8, !dbg !1456
  %6 = load i16, i16* %5, align 2, !dbg !1457
  %conv9 = zext i16 %6 to i32, !dbg !1457
  %shr = ashr i32 %conv9, 8, !dbg !1458
  %7 = load i8, i8* %tmp, align 1, !dbg !1459
  %conv10 = zext i8 %7 to i32, !dbg !1459
  %shl11 = shl i32 %conv10, 8, !dbg !1460
  %xor12 = xor i32 %shr, %shl11, !dbg !1461
  %8 = load i8, i8* %tmp, align 1, !dbg !1462
  %conv13 = zext i8 %8 to i32, !dbg !1462
  %shl14 = shl i32 %conv13, 3, !dbg !1463
  %xor15 = xor i32 %xor12, %shl14, !dbg !1464
  %9 = load i8, i8* %tmp, align 1, !dbg !1465
  %conv16 = zext i8 %9 to i32, !dbg !1465
  %shr17 = ashr i32 %conv16, 4, !dbg !1466
  %xor18 = xor i32 %xor15, %shr17, !dbg !1467
  %conv19 = trunc i32 %xor18 to i16, !dbg !1468
  %10 = load i16*, i16** %crcAccum.addr, align 8, !dbg !1469
  store i16 %conv19, i16* %10, align 2, !dbg !1470
  ret void, !dbg !1471
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_sha256_init(%struct.mavlink_sha256_ctx* %m) #0 !dbg !1472 {
entry:
  %m.addr = alloca %struct.mavlink_sha256_ctx*, align 8
  store %struct.mavlink_sha256_ctx* %m, %struct.mavlink_sha256_ctx** %m.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx** %m.addr, metadata !1476, metadata !DIExpression()), !dbg !1477
  %0 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1478
  %sz = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %0, i32 0, i32 0, !dbg !1479
  %arrayidx = getelementptr inbounds [2 x i32], [2 x i32]* %sz, i64 0, i64 0, !dbg !1478
  store i32 0, i32* %arrayidx, align 4, !dbg !1480
  %1 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1481
  %sz1 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %1, i32 0, i32 0, !dbg !1482
  %arrayidx2 = getelementptr inbounds [2 x i32], [2 x i32]* %sz1, i64 0, i64 1, !dbg !1481
  store i32 0, i32* %arrayidx2, align 4, !dbg !1483
  %2 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1484
  %counter = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %2, i32 0, i32 1, !dbg !1485
  %arrayidx3 = getelementptr inbounds [8 x i32], [8 x i32]* %counter, i64 0, i64 0, !dbg !1484
  store i32 1779033703, i32* %arrayidx3, align 4, !dbg !1486
  %3 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1487
  %counter4 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %3, i32 0, i32 1, !dbg !1488
  %arrayidx5 = getelementptr inbounds [8 x i32], [8 x i32]* %counter4, i64 0, i64 1, !dbg !1487
  store i32 -1150833019, i32* %arrayidx5, align 4, !dbg !1489
  %4 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1490
  %counter6 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %4, i32 0, i32 1, !dbg !1491
  %arrayidx7 = getelementptr inbounds [8 x i32], [8 x i32]* %counter6, i64 0, i64 2, !dbg !1490
  store i32 1013904242, i32* %arrayidx7, align 4, !dbg !1492
  %5 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1493
  %counter8 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %5, i32 0, i32 1, !dbg !1494
  %arrayidx9 = getelementptr inbounds [8 x i32], [8 x i32]* %counter8, i64 0, i64 3, !dbg !1493
  store i32 -1521486534, i32* %arrayidx9, align 4, !dbg !1495
  %6 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1496
  %counter10 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %6, i32 0, i32 1, !dbg !1497
  %arrayidx11 = getelementptr inbounds [8 x i32], [8 x i32]* %counter10, i64 0, i64 4, !dbg !1496
  store i32 1359893119, i32* %arrayidx11, align 4, !dbg !1498
  %7 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1499
  %counter12 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %7, i32 0, i32 1, !dbg !1500
  %arrayidx13 = getelementptr inbounds [8 x i32], [8 x i32]* %counter12, i64 0, i64 5, !dbg !1499
  store i32 -1694144372, i32* %arrayidx13, align 4, !dbg !1501
  %8 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1502
  %counter14 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %8, i32 0, i32 1, !dbg !1503
  %arrayidx15 = getelementptr inbounds [8 x i32], [8 x i32]* %counter14, i64 0, i64 6, !dbg !1502
  store i32 528734635, i32* %arrayidx15, align 4, !dbg !1504
  %9 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1505
  %counter16 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %9, i32 0, i32 1, !dbg !1506
  %arrayidx17 = getelementptr inbounds [8 x i32], [8 x i32]* %counter16, i64 0, i64 7, !dbg !1505
  store i32 1541459225, i32* %arrayidx17, align 4, !dbg !1507
  ret void, !dbg !1508
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %m, i8* %v, i32 %len) #0 !dbg !1509 {
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
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx** %m.addr, metadata !1514, metadata !DIExpression()), !dbg !1515
  store i8* %v, i8** %v.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %v.addr, metadata !1516, metadata !DIExpression()), !dbg !1517
  store i32 %len, i32* %len.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %len.addr, metadata !1518, metadata !DIExpression()), !dbg !1519
  call void @llvm.dbg.declare(metadata i8** %p, metadata !1520, metadata !DIExpression()), !dbg !1521
  %0 = load i8*, i8** %v.addr, align 8, !dbg !1522
  store i8* %0, i8** %p, align 8, !dbg !1521
  call void @llvm.dbg.declare(metadata i32* %old_sz, metadata !1523, metadata !DIExpression()), !dbg !1524
  %1 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1525
  %sz = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %1, i32 0, i32 0, !dbg !1526
  %arrayidx = getelementptr inbounds [2 x i32], [2 x i32]* %sz, i64 0, i64 0, !dbg !1525
  %2 = load i32, i32* %arrayidx, align 4, !dbg !1525
  store i32 %2, i32* %old_sz, align 4, !dbg !1524
  call void @llvm.dbg.declare(metadata i32* %offset, metadata !1527, metadata !DIExpression()), !dbg !1528
  %3 = load i32, i32* %len.addr, align 4, !dbg !1529
  %mul = mul i32 %3, 8, !dbg !1530
  %4 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1531
  %sz1 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %4, i32 0, i32 0, !dbg !1532
  %arrayidx2 = getelementptr inbounds [2 x i32], [2 x i32]* %sz1, i64 0, i64 0, !dbg !1531
  %5 = load i32, i32* %arrayidx2, align 4, !dbg !1533
  %add = add i32 %5, %mul, !dbg !1533
  store i32 %add, i32* %arrayidx2, align 4, !dbg !1533
  %6 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1534
  %sz3 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %6, i32 0, i32 0, !dbg !1536
  %arrayidx4 = getelementptr inbounds [2 x i32], [2 x i32]* %sz3, i64 0, i64 0, !dbg !1534
  %7 = load i32, i32* %arrayidx4, align 4, !dbg !1534
  %8 = load i32, i32* %old_sz, align 4, !dbg !1537
  %cmp = icmp ult i32 %7, %8, !dbg !1538
  br i1 %cmp, label %if.then, label %if.end, !dbg !1539

if.then:                                          ; preds = %entry
  %9 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1540
  %sz5 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %9, i32 0, i32 0, !dbg !1541
  %arrayidx6 = getelementptr inbounds [2 x i32], [2 x i32]* %sz5, i64 0, i64 1, !dbg !1540
  %10 = load i32, i32* %arrayidx6, align 4, !dbg !1542
  %inc = add i32 %10, 1, !dbg !1542
  store i32 %inc, i32* %arrayidx6, align 4, !dbg !1542
  br label %if.end, !dbg !1542

if.end:                                           ; preds = %if.then, %entry
  %11 = load i32, i32* %old_sz, align 4, !dbg !1543
  %div = udiv i32 %11, 8, !dbg !1544
  %rem = urem i32 %div, 64, !dbg !1545
  store i32 %rem, i32* %offset, align 4, !dbg !1546
  br label %while.cond, !dbg !1547

while.cond:                                       ; preds = %if.end36, %if.end
  %12 = load i32, i32* %len.addr, align 4, !dbg !1548
  %cmp7 = icmp ugt i32 %12, 0, !dbg !1549
  br i1 %cmp7, label %while.body, label %while.end, !dbg !1547

while.body:                                       ; preds = %while.cond
  call void @llvm.dbg.declare(metadata i32* %l, metadata !1550, metadata !DIExpression()), !dbg !1552
  %13 = load i32, i32* %offset, align 4, !dbg !1553
  %sub = sub i32 64, %13, !dbg !1554
  store i32 %sub, i32* %l, align 4, !dbg !1552
  %14 = load i32, i32* %len.addr, align 4, !dbg !1555
  %15 = load i32, i32* %l, align 4, !dbg !1557
  %cmp8 = icmp ult i32 %14, %15, !dbg !1558
  br i1 %cmp8, label %if.then9, label %if.end10, !dbg !1559

if.then9:                                         ; preds = %while.body
  %16 = load i32, i32* %len.addr, align 4, !dbg !1560
  store i32 %16, i32* %l, align 4, !dbg !1562
  br label %if.end10, !dbg !1563

if.end10:                                         ; preds = %if.then9, %while.body
  %17 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1564
  %u = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %17, i32 0, i32 2, !dbg !1565
  %save_bytes = bitcast %union.anon* %u to [64 x i8]*, !dbg !1566
  %arraydecay = getelementptr inbounds [64 x i8], [64 x i8]* %save_bytes, i64 0, i64 0, !dbg !1564
  %18 = load i32, i32* %offset, align 4, !dbg !1567
  %idx.ext = zext i32 %18 to i64, !dbg !1568
  %add.ptr = getelementptr inbounds i8, i8* %arraydecay, i64 %idx.ext, !dbg !1568
  %19 = load i8*, i8** %p, align 8, !dbg !1569
  %20 = load i32, i32* %l, align 4, !dbg !1570
  %conv = zext i32 %20 to i64, !dbg !1570
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %add.ptr, i8* align 1 %19, i64 %conv, i1 false), !dbg !1571
  %21 = load i32, i32* %l, align 4, !dbg !1572
  %22 = load i32, i32* %offset, align 4, !dbg !1573
  %add11 = add i32 %22, %21, !dbg !1573
  store i32 %add11, i32* %offset, align 4, !dbg !1573
  %23 = load i32, i32* %l, align 4, !dbg !1574
  %24 = load i8*, i8** %p, align 8, !dbg !1575
  %idx.ext12 = zext i32 %23 to i64, !dbg !1575
  %add.ptr13 = getelementptr inbounds i8, i8* %24, i64 %idx.ext12, !dbg !1575
  store i8* %add.ptr13, i8** %p, align 8, !dbg !1575
  %25 = load i32, i32* %l, align 4, !dbg !1576
  %26 = load i32, i32* %len.addr, align 4, !dbg !1577
  %sub14 = sub i32 %26, %25, !dbg !1577
  store i32 %sub14, i32* %len.addr, align 4, !dbg !1577
  %27 = load i32, i32* %offset, align 4, !dbg !1578
  %cmp15 = icmp eq i32 %27, 64, !dbg !1580
  br i1 %cmp15, label %if.then17, label %if.end36, !dbg !1581

if.then17:                                        ; preds = %if.end10
  call void @llvm.dbg.declare(metadata i32* %i, metadata !1582, metadata !DIExpression()), !dbg !1584
  call void @llvm.dbg.declare(metadata [16 x i32]* %current, metadata !1585, metadata !DIExpression()), !dbg !1586
  call void @llvm.dbg.declare(metadata i32** %u18, metadata !1587, metadata !DIExpression()), !dbg !1589
  %28 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1590
  %u19 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %28, i32 0, i32 2, !dbg !1591
  %save_u32 = bitcast %union.anon* %u19 to [16 x i32]*, !dbg !1592
  %arraydecay20 = getelementptr inbounds [16 x i32], [16 x i32]* %save_u32, i64 0, i64 0, !dbg !1590
  store i32* %arraydecay20, i32** %u18, align 8, !dbg !1589
  store i32 0, i32* %i, align 4, !dbg !1593
  br label %for.cond, !dbg !1595

for.cond:                                         ; preds = %for.inc, %if.then17
  %29 = load i32, i32* %i, align 4, !dbg !1596
  %cmp21 = icmp slt i32 %29, 16, !dbg !1598
  br i1 %cmp21, label %for.body, label %for.end, !dbg !1599

for.body:                                         ; preds = %for.cond
  call void @llvm.dbg.declare(metadata i8** %p1, metadata !1600, metadata !DIExpression()), !dbg !1602
  %30 = load i32*, i32** %u18, align 8, !dbg !1603
  %31 = load i32, i32* %i, align 4, !dbg !1604
  %idxprom = sext i32 %31 to i64, !dbg !1603
  %arrayidx23 = getelementptr inbounds i32, i32* %30, i64 %idxprom, !dbg !1603
  %32 = bitcast i32* %arrayidx23 to i8*, !dbg !1605
  store i8* %32, i8** %p1, align 8, !dbg !1602
  call void @llvm.dbg.declare(metadata i8** %p2, metadata !1606, metadata !DIExpression()), !dbg !1607
  %33 = load i32, i32* %i, align 4, !dbg !1608
  %idxprom24 = sext i32 %33 to i64, !dbg !1609
  %arrayidx25 = getelementptr inbounds [16 x i32], [16 x i32]* %current, i64 0, i64 %idxprom24, !dbg !1609
  %34 = bitcast i32* %arrayidx25 to i8*, !dbg !1610
  store i8* %34, i8** %p2, align 8, !dbg !1607
  %35 = load i8*, i8** %p1, align 8, !dbg !1611
  %arrayidx26 = getelementptr inbounds i8, i8* %35, i64 3, !dbg !1611
  %36 = load i8, i8* %arrayidx26, align 1, !dbg !1611
  %37 = load i8*, i8** %p2, align 8, !dbg !1612
  %arrayidx27 = getelementptr inbounds i8, i8* %37, i64 0, !dbg !1612
  store i8 %36, i8* %arrayidx27, align 1, !dbg !1613
  %38 = load i8*, i8** %p1, align 8, !dbg !1614
  %arrayidx28 = getelementptr inbounds i8, i8* %38, i64 2, !dbg !1614
  %39 = load i8, i8* %arrayidx28, align 1, !dbg !1614
  %40 = load i8*, i8** %p2, align 8, !dbg !1615
  %arrayidx29 = getelementptr inbounds i8, i8* %40, i64 1, !dbg !1615
  store i8 %39, i8* %arrayidx29, align 1, !dbg !1616
  %41 = load i8*, i8** %p1, align 8, !dbg !1617
  %arrayidx30 = getelementptr inbounds i8, i8* %41, i64 1, !dbg !1617
  %42 = load i8, i8* %arrayidx30, align 1, !dbg !1617
  %43 = load i8*, i8** %p2, align 8, !dbg !1618
  %arrayidx31 = getelementptr inbounds i8, i8* %43, i64 2, !dbg !1618
  store i8 %42, i8* %arrayidx31, align 1, !dbg !1619
  %44 = load i8*, i8** %p1, align 8, !dbg !1620
  %arrayidx32 = getelementptr inbounds i8, i8* %44, i64 0, !dbg !1620
  %45 = load i8, i8* %arrayidx32, align 1, !dbg !1620
  %46 = load i8*, i8** %p2, align 8, !dbg !1621
  %arrayidx33 = getelementptr inbounds i8, i8* %46, i64 3, !dbg !1621
  store i8 %45, i8* %arrayidx33, align 1, !dbg !1622
  br label %for.inc, !dbg !1623

for.inc:                                          ; preds = %for.body
  %47 = load i32, i32* %i, align 4, !dbg !1624
  %inc34 = add nsw i32 %47, 1, !dbg !1624
  store i32 %inc34, i32* %i, align 4, !dbg !1624
  br label %for.cond, !dbg !1625, !llvm.loop !1626

for.end:                                          ; preds = %for.cond
  %48 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1628
  %arraydecay35 = getelementptr inbounds [16 x i32], [16 x i32]* %current, i64 0, i64 0, !dbg !1629
  call void @mavlink_sha256_calc(%struct.mavlink_sha256_ctx* %48, i32* %arraydecay35), !dbg !1630
  store i32 0, i32* %offset, align 4, !dbg !1631
  br label %if.end36, !dbg !1632

if.end36:                                         ; preds = %for.end, %if.end10
  br label %while.cond, !dbg !1547, !llvm.loop !1633

while.end:                                        ; preds = %while.cond
  ret void, !dbg !1635
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_sha256_final_48(%struct.mavlink_sha256_ctx* %m, i8* %result) #0 !dbg !1636 {
entry:
  %m.addr = alloca %struct.mavlink_sha256_ctx*, align 8
  %result.addr = alloca i8*, align 8
  %zeros = alloca [72 x i8], align 16
  %offset = alloca i32, align 4
  %dstart = alloca i32, align 4
  %p = alloca i8*, align 8
  store %struct.mavlink_sha256_ctx* %m, %struct.mavlink_sha256_ctx** %m.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx** %m.addr, metadata !1639, metadata !DIExpression()), !dbg !1640
  store i8* %result, i8** %result.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %result.addr, metadata !1641, metadata !DIExpression()), !dbg !1642
  call void @llvm.dbg.declare(metadata [72 x i8]* %zeros, metadata !1643, metadata !DIExpression()), !dbg !1647
  call void @llvm.dbg.declare(metadata i32* %offset, metadata !1648, metadata !DIExpression()), !dbg !1649
  %0 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1650
  %sz = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %0, i32 0, i32 0, !dbg !1651
  %arrayidx = getelementptr inbounds [2 x i32], [2 x i32]* %sz, i64 0, i64 0, !dbg !1650
  %1 = load i32, i32* %arrayidx, align 4, !dbg !1650
  %div = udiv i32 %1, 8, !dbg !1652
  %rem = urem i32 %div, 64, !dbg !1653
  store i32 %rem, i32* %offset, align 4, !dbg !1649
  call void @llvm.dbg.declare(metadata i32* %dstart, metadata !1654, metadata !DIExpression()), !dbg !1655
  %2 = load i32, i32* %offset, align 4, !dbg !1656
  %sub = sub i32 120, %2, !dbg !1657
  %sub1 = sub i32 %sub, 1, !dbg !1658
  %rem2 = urem i32 %sub1, 64, !dbg !1659
  %add = add i32 %rem2, 1, !dbg !1660
  store i32 %add, i32* %dstart, align 4, !dbg !1655
  call void @llvm.dbg.declare(metadata i8** %p, metadata !1661, metadata !DIExpression()), !dbg !1662
  %3 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1663
  %counter = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %3, i32 0, i32 1, !dbg !1664
  %arrayidx3 = getelementptr inbounds [8 x i32], [8 x i32]* %counter, i64 0, i64 0, !dbg !1663
  %4 = bitcast i32* %arrayidx3 to i8*, !dbg !1665
  store i8* %4, i8** %p, align 8, !dbg !1662
  %arraydecay = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 0, !dbg !1666
  store i8 -128, i8* %arraydecay, align 16, !dbg !1667
  %arraydecay4 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 0, !dbg !1668
  %add.ptr = getelementptr inbounds i8, i8* %arraydecay4, i64 1, !dbg !1669
  call void @llvm.memset.p0i8.i64(i8* align 1 %add.ptr, i8 0, i64 71, i1 false), !dbg !1670
  %5 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1671
  %sz5 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %5, i32 0, i32 0, !dbg !1672
  %arrayidx6 = getelementptr inbounds [2 x i32], [2 x i32]* %sz5, i64 0, i64 0, !dbg !1671
  %6 = load i32, i32* %arrayidx6, align 4, !dbg !1671
  %shr = lshr i32 %6, 0, !dbg !1673
  %and = and i32 %shr, 255, !dbg !1674
  %conv = trunc i32 %and to i8, !dbg !1675
  %7 = load i32, i32* %dstart, align 4, !dbg !1676
  %add7 = add i32 %7, 7, !dbg !1677
  %idxprom = zext i32 %add7 to i64, !dbg !1678
  %arrayidx8 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom, !dbg !1678
  store i8 %conv, i8* %arrayidx8, align 1, !dbg !1679
  %8 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1680
  %sz9 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %8, i32 0, i32 0, !dbg !1681
  %arrayidx10 = getelementptr inbounds [2 x i32], [2 x i32]* %sz9, i64 0, i64 0, !dbg !1680
  %9 = load i32, i32* %arrayidx10, align 4, !dbg !1680
  %shr11 = lshr i32 %9, 8, !dbg !1682
  %and12 = and i32 %shr11, 255, !dbg !1683
  %conv13 = trunc i32 %and12 to i8, !dbg !1684
  %10 = load i32, i32* %dstart, align 4, !dbg !1685
  %add14 = add i32 %10, 6, !dbg !1686
  %idxprom15 = zext i32 %add14 to i64, !dbg !1687
  %arrayidx16 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom15, !dbg !1687
  store i8 %conv13, i8* %arrayidx16, align 1, !dbg !1688
  %11 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1689
  %sz17 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %11, i32 0, i32 0, !dbg !1690
  %arrayidx18 = getelementptr inbounds [2 x i32], [2 x i32]* %sz17, i64 0, i64 0, !dbg !1689
  %12 = load i32, i32* %arrayidx18, align 4, !dbg !1689
  %shr19 = lshr i32 %12, 16, !dbg !1691
  %and20 = and i32 %shr19, 255, !dbg !1692
  %conv21 = trunc i32 %and20 to i8, !dbg !1693
  %13 = load i32, i32* %dstart, align 4, !dbg !1694
  %add22 = add i32 %13, 5, !dbg !1695
  %idxprom23 = zext i32 %add22 to i64, !dbg !1696
  %arrayidx24 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom23, !dbg !1696
  store i8 %conv21, i8* %arrayidx24, align 1, !dbg !1697
  %14 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1698
  %sz25 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %14, i32 0, i32 0, !dbg !1699
  %arrayidx26 = getelementptr inbounds [2 x i32], [2 x i32]* %sz25, i64 0, i64 0, !dbg !1698
  %15 = load i32, i32* %arrayidx26, align 4, !dbg !1698
  %shr27 = lshr i32 %15, 24, !dbg !1700
  %and28 = and i32 %shr27, 255, !dbg !1701
  %conv29 = trunc i32 %and28 to i8, !dbg !1702
  %16 = load i32, i32* %dstart, align 4, !dbg !1703
  %add30 = add i32 %16, 4, !dbg !1704
  %idxprom31 = zext i32 %add30 to i64, !dbg !1705
  %arrayidx32 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom31, !dbg !1705
  store i8 %conv29, i8* %arrayidx32, align 1, !dbg !1706
  %17 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1707
  %sz33 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %17, i32 0, i32 0, !dbg !1708
  %arrayidx34 = getelementptr inbounds [2 x i32], [2 x i32]* %sz33, i64 0, i64 1, !dbg !1707
  %18 = load i32, i32* %arrayidx34, align 4, !dbg !1707
  %shr35 = lshr i32 %18, 0, !dbg !1709
  %and36 = and i32 %shr35, 255, !dbg !1710
  %conv37 = trunc i32 %and36 to i8, !dbg !1711
  %19 = load i32, i32* %dstart, align 4, !dbg !1712
  %add38 = add i32 %19, 3, !dbg !1713
  %idxprom39 = zext i32 %add38 to i64, !dbg !1714
  %arrayidx40 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom39, !dbg !1714
  store i8 %conv37, i8* %arrayidx40, align 1, !dbg !1715
  %20 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1716
  %sz41 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %20, i32 0, i32 0, !dbg !1717
  %arrayidx42 = getelementptr inbounds [2 x i32], [2 x i32]* %sz41, i64 0, i64 1, !dbg !1716
  %21 = load i32, i32* %arrayidx42, align 4, !dbg !1716
  %shr43 = lshr i32 %21, 8, !dbg !1718
  %and44 = and i32 %shr43, 255, !dbg !1719
  %conv45 = trunc i32 %and44 to i8, !dbg !1720
  %22 = load i32, i32* %dstart, align 4, !dbg !1721
  %add46 = add i32 %22, 2, !dbg !1722
  %idxprom47 = zext i32 %add46 to i64, !dbg !1723
  %arrayidx48 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom47, !dbg !1723
  store i8 %conv45, i8* %arrayidx48, align 1, !dbg !1724
  %23 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1725
  %sz49 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %23, i32 0, i32 0, !dbg !1726
  %arrayidx50 = getelementptr inbounds [2 x i32], [2 x i32]* %sz49, i64 0, i64 1, !dbg !1725
  %24 = load i32, i32* %arrayidx50, align 4, !dbg !1725
  %shr51 = lshr i32 %24, 16, !dbg !1727
  %and52 = and i32 %shr51, 255, !dbg !1728
  %conv53 = trunc i32 %and52 to i8, !dbg !1729
  %25 = load i32, i32* %dstart, align 4, !dbg !1730
  %add54 = add i32 %25, 1, !dbg !1731
  %idxprom55 = zext i32 %add54 to i64, !dbg !1732
  %arrayidx56 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom55, !dbg !1732
  store i8 %conv53, i8* %arrayidx56, align 1, !dbg !1733
  %26 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1734
  %sz57 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %26, i32 0, i32 0, !dbg !1735
  %arrayidx58 = getelementptr inbounds [2 x i32], [2 x i32]* %sz57, i64 0, i64 1, !dbg !1734
  %27 = load i32, i32* %arrayidx58, align 4, !dbg !1734
  %shr59 = lshr i32 %27, 24, !dbg !1736
  %and60 = and i32 %shr59, 255, !dbg !1737
  %conv61 = trunc i32 %and60 to i8, !dbg !1738
  %28 = load i32, i32* %dstart, align 4, !dbg !1739
  %add62 = add i32 %28, 0, !dbg !1740
  %idxprom63 = zext i32 %add62 to i64, !dbg !1741
  %arrayidx64 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 %idxprom63, !dbg !1741
  store i8 %conv61, i8* %arrayidx64, align 1, !dbg !1742
  %29 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1743
  %arraydecay65 = getelementptr inbounds [72 x i8], [72 x i8]* %zeros, i64 0, i64 0, !dbg !1744
  %30 = load i32, i32* %dstart, align 4, !dbg !1745
  %add66 = add i32 %30, 8, !dbg !1746
  call void @mavlink_sha256_update(%struct.mavlink_sha256_ctx* %29, i8* %arraydecay65, i32 %add66), !dbg !1747
  %31 = load i8*, i8** %p, align 8, !dbg !1748
  %arrayidx67 = getelementptr inbounds i8, i8* %31, i64 3, !dbg !1748
  %32 = load i8, i8* %arrayidx67, align 1, !dbg !1748
  %33 = load i8*, i8** %result.addr, align 8, !dbg !1749
  %arrayidx68 = getelementptr inbounds i8, i8* %33, i64 0, !dbg !1749
  store i8 %32, i8* %arrayidx68, align 1, !dbg !1750
  %34 = load i8*, i8** %p, align 8, !dbg !1751
  %arrayidx69 = getelementptr inbounds i8, i8* %34, i64 2, !dbg !1751
  %35 = load i8, i8* %arrayidx69, align 1, !dbg !1751
  %36 = load i8*, i8** %result.addr, align 8, !dbg !1752
  %arrayidx70 = getelementptr inbounds i8, i8* %36, i64 1, !dbg !1752
  store i8 %35, i8* %arrayidx70, align 1, !dbg !1753
  %37 = load i8*, i8** %p, align 8, !dbg !1754
  %arrayidx71 = getelementptr inbounds i8, i8* %37, i64 1, !dbg !1754
  %38 = load i8, i8* %arrayidx71, align 1, !dbg !1754
  %39 = load i8*, i8** %result.addr, align 8, !dbg !1755
  %arrayidx72 = getelementptr inbounds i8, i8* %39, i64 2, !dbg !1755
  store i8 %38, i8* %arrayidx72, align 1, !dbg !1756
  %40 = load i8*, i8** %p, align 8, !dbg !1757
  %arrayidx73 = getelementptr inbounds i8, i8* %40, i64 0, !dbg !1757
  %41 = load i8, i8* %arrayidx73, align 1, !dbg !1757
  %42 = load i8*, i8** %result.addr, align 8, !dbg !1758
  %arrayidx74 = getelementptr inbounds i8, i8* %42, i64 3, !dbg !1758
  store i8 %41, i8* %arrayidx74, align 1, !dbg !1759
  %43 = load i8*, i8** %p, align 8, !dbg !1760
  %arrayidx75 = getelementptr inbounds i8, i8* %43, i64 7, !dbg !1760
  %44 = load i8, i8* %arrayidx75, align 1, !dbg !1760
  %45 = load i8*, i8** %result.addr, align 8, !dbg !1761
  %arrayidx76 = getelementptr inbounds i8, i8* %45, i64 4, !dbg !1761
  store i8 %44, i8* %arrayidx76, align 1, !dbg !1762
  %46 = load i8*, i8** %p, align 8, !dbg !1763
  %arrayidx77 = getelementptr inbounds i8, i8* %46, i64 6, !dbg !1763
  %47 = load i8, i8* %arrayidx77, align 1, !dbg !1763
  %48 = load i8*, i8** %result.addr, align 8, !dbg !1764
  %arrayidx78 = getelementptr inbounds i8, i8* %48, i64 5, !dbg !1764
  store i8 %47, i8* %arrayidx78, align 1, !dbg !1765
  ret void, !dbg !1766
}

; Function Attrs: nounwind readonly
declare dso_local i32 @memcmp(i8*, i8*, i64) #6

; Function Attrs: noinline nounwind optnone uwtable
define internal void @mavlink_sha256_calc(%struct.mavlink_sha256_ctx* %m, i32* %in) #0 !dbg !1767 {
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
  call void @llvm.dbg.declare(metadata %struct.mavlink_sha256_ctx** %m.addr, metadata !1771, metadata !DIExpression()), !dbg !1772
  store i32* %in, i32** %in.addr, align 8
  call void @llvm.dbg.declare(metadata i32** %in.addr, metadata !1773, metadata !DIExpression()), !dbg !1774
  call void @llvm.dbg.declare(metadata i32* %AA, metadata !1775, metadata !DIExpression()), !dbg !1776
  call void @llvm.dbg.declare(metadata i32* %BB, metadata !1777, metadata !DIExpression()), !dbg !1778
  call void @llvm.dbg.declare(metadata i32* %CC, metadata !1779, metadata !DIExpression()), !dbg !1780
  call void @llvm.dbg.declare(metadata i32* %DD, metadata !1781, metadata !DIExpression()), !dbg !1782
  call void @llvm.dbg.declare(metadata i32* %EE, metadata !1783, metadata !DIExpression()), !dbg !1784
  call void @llvm.dbg.declare(metadata i32* %FF, metadata !1785, metadata !DIExpression()), !dbg !1786
  call void @llvm.dbg.declare(metadata i32* %GG, metadata !1787, metadata !DIExpression()), !dbg !1788
  call void @llvm.dbg.declare(metadata i32* %HH, metadata !1789, metadata !DIExpression()), !dbg !1790
  call void @llvm.dbg.declare(metadata [64 x i32]* %data, metadata !1791, metadata !DIExpression()), !dbg !1793
  call void @llvm.dbg.declare(metadata i32* %i, metadata !1794, metadata !DIExpression()), !dbg !1795
  %0 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1796
  %counter = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %0, i32 0, i32 1, !dbg !1797
  %arrayidx = getelementptr inbounds [8 x i32], [8 x i32]* %counter, i64 0, i64 0, !dbg !1796
  %1 = load i32, i32* %arrayidx, align 4, !dbg !1796
  store i32 %1, i32* %AA, align 4, !dbg !1798
  %2 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1799
  %counter1 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %2, i32 0, i32 1, !dbg !1800
  %arrayidx2 = getelementptr inbounds [8 x i32], [8 x i32]* %counter1, i64 0, i64 1, !dbg !1799
  %3 = load i32, i32* %arrayidx2, align 4, !dbg !1799
  store i32 %3, i32* %BB, align 4, !dbg !1801
  %4 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1802
  %counter3 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %4, i32 0, i32 1, !dbg !1803
  %arrayidx4 = getelementptr inbounds [8 x i32], [8 x i32]* %counter3, i64 0, i64 2, !dbg !1802
  %5 = load i32, i32* %arrayidx4, align 4, !dbg !1802
  store i32 %5, i32* %CC, align 4, !dbg !1804
  %6 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1805
  %counter5 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %6, i32 0, i32 1, !dbg !1806
  %arrayidx6 = getelementptr inbounds [8 x i32], [8 x i32]* %counter5, i64 0, i64 3, !dbg !1805
  %7 = load i32, i32* %arrayidx6, align 4, !dbg !1805
  store i32 %7, i32* %DD, align 4, !dbg !1807
  %8 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1808
  %counter7 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %8, i32 0, i32 1, !dbg !1809
  %arrayidx8 = getelementptr inbounds [8 x i32], [8 x i32]* %counter7, i64 0, i64 4, !dbg !1808
  %9 = load i32, i32* %arrayidx8, align 4, !dbg !1808
  store i32 %9, i32* %EE, align 4, !dbg !1810
  %10 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1811
  %counter9 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %10, i32 0, i32 1, !dbg !1812
  %arrayidx10 = getelementptr inbounds [8 x i32], [8 x i32]* %counter9, i64 0, i64 5, !dbg !1811
  %11 = load i32, i32* %arrayidx10, align 4, !dbg !1811
  store i32 %11, i32* %FF, align 4, !dbg !1813
  %12 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1814
  %counter11 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %12, i32 0, i32 1, !dbg !1815
  %arrayidx12 = getelementptr inbounds [8 x i32], [8 x i32]* %counter11, i64 0, i64 6, !dbg !1814
  %13 = load i32, i32* %arrayidx12, align 4, !dbg !1814
  store i32 %13, i32* %GG, align 4, !dbg !1816
  %14 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1817
  %counter13 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %14, i32 0, i32 1, !dbg !1818
  %arrayidx14 = getelementptr inbounds [8 x i32], [8 x i32]* %counter13, i64 0, i64 7, !dbg !1817
  %15 = load i32, i32* %arrayidx14, align 4, !dbg !1817
  store i32 %15, i32* %HH, align 4, !dbg !1819
  store i32 0, i32* %i, align 4, !dbg !1820
  br label %for.cond, !dbg !1822

for.cond:                                         ; preds = %for.inc, %entry
  %16 = load i32, i32* %i, align 4, !dbg !1823
  %cmp = icmp slt i32 %16, 16, !dbg !1825
  br i1 %cmp, label %for.body, label %for.end, !dbg !1826

for.body:                                         ; preds = %for.cond
  %17 = load i32*, i32** %in.addr, align 8, !dbg !1827
  %18 = load i32, i32* %i, align 4, !dbg !1828
  %idxprom = sext i32 %18 to i64, !dbg !1827
  %arrayidx15 = getelementptr inbounds i32, i32* %17, i64 %idxprom, !dbg !1827
  %19 = load i32, i32* %arrayidx15, align 4, !dbg !1827
  %20 = load i32, i32* %i, align 4, !dbg !1829
  %idxprom16 = sext i32 %20 to i64, !dbg !1830
  %arrayidx17 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom16, !dbg !1830
  store i32 %19, i32* %arrayidx17, align 4, !dbg !1831
  br label %for.inc, !dbg !1830

for.inc:                                          ; preds = %for.body
  %21 = load i32, i32* %i, align 4, !dbg !1832
  %inc = add nsw i32 %21, 1, !dbg !1832
  store i32 %inc, i32* %i, align 4, !dbg !1832
  br label %for.cond, !dbg !1833, !llvm.loop !1834

for.end:                                          ; preds = %for.cond
  store i32 16, i32* %i, align 4, !dbg !1836
  br label %for.cond18, !dbg !1838

for.cond18:                                       ; preds = %for.inc74, %for.end
  %22 = load i32, i32* %i, align 4, !dbg !1839
  %cmp19 = icmp slt i32 %22, 64, !dbg !1841
  br i1 %cmp19, label %for.body20, label %for.end76, !dbg !1842

for.body20:                                       ; preds = %for.cond18
  %23 = load i32, i32* %i, align 4, !dbg !1843
  %sub = sub nsw i32 %23, 2, !dbg !1843
  %idxprom21 = sext i32 %sub to i64, !dbg !1843
  %arrayidx22 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom21, !dbg !1843
  %24 = load i32, i32* %arrayidx22, align 4, !dbg !1843
  %shr = lshr i32 %24, 17, !dbg !1843
  %25 = load i32, i32* %i, align 4, !dbg !1843
  %sub23 = sub nsw i32 %25, 2, !dbg !1843
  %idxprom24 = sext i32 %sub23 to i64, !dbg !1843
  %arrayidx25 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom24, !dbg !1843
  %26 = load i32, i32* %arrayidx25, align 4, !dbg !1843
  %shl = shl i32 %26, 15, !dbg !1843
  %or = or i32 %shr, %shl, !dbg !1843
  %27 = load i32, i32* %i, align 4, !dbg !1843
  %sub26 = sub nsw i32 %27, 2, !dbg !1843
  %idxprom27 = sext i32 %sub26 to i64, !dbg !1843
  %arrayidx28 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom27, !dbg !1843
  %28 = load i32, i32* %arrayidx28, align 4, !dbg !1843
  %shr29 = lshr i32 %28, 19, !dbg !1843
  %29 = load i32, i32* %i, align 4, !dbg !1843
  %sub30 = sub nsw i32 %29, 2, !dbg !1843
  %idxprom31 = sext i32 %sub30 to i64, !dbg !1843
  %arrayidx32 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom31, !dbg !1843
  %30 = load i32, i32* %arrayidx32, align 4, !dbg !1843
  %shl33 = shl i32 %30, 13, !dbg !1843
  %or34 = or i32 %shr29, %shl33, !dbg !1843
  %xor = xor i32 %or, %or34, !dbg !1843
  %31 = load i32, i32* %i, align 4, !dbg !1843
  %sub35 = sub nsw i32 %31, 2, !dbg !1843
  %idxprom36 = sext i32 %sub35 to i64, !dbg !1843
  %arrayidx37 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom36, !dbg !1843
  %32 = load i32, i32* %arrayidx37, align 4, !dbg !1843
  %shr38 = lshr i32 %32, 10, !dbg !1843
  %xor39 = xor i32 %xor, %shr38, !dbg !1843
  %33 = load i32, i32* %i, align 4, !dbg !1844
  %sub40 = sub nsw i32 %33, 7, !dbg !1845
  %idxprom41 = sext i32 %sub40 to i64, !dbg !1846
  %arrayidx42 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom41, !dbg !1846
  %34 = load i32, i32* %arrayidx42, align 4, !dbg !1846
  %add = add i32 %xor39, %34, !dbg !1847
  %35 = load i32, i32* %i, align 4, !dbg !1848
  %sub43 = sub nsw i32 %35, 15, !dbg !1848
  %idxprom44 = sext i32 %sub43 to i64, !dbg !1848
  %arrayidx45 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom44, !dbg !1848
  %36 = load i32, i32* %arrayidx45, align 4, !dbg !1848
  %shr46 = lshr i32 %36, 7, !dbg !1848
  %37 = load i32, i32* %i, align 4, !dbg !1848
  %sub47 = sub nsw i32 %37, 15, !dbg !1848
  %idxprom48 = sext i32 %sub47 to i64, !dbg !1848
  %arrayidx49 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom48, !dbg !1848
  %38 = load i32, i32* %arrayidx49, align 4, !dbg !1848
  %shl50 = shl i32 %38, 25, !dbg !1848
  %or51 = or i32 %shr46, %shl50, !dbg !1848
  %39 = load i32, i32* %i, align 4, !dbg !1848
  %sub52 = sub nsw i32 %39, 15, !dbg !1848
  %idxprom53 = sext i32 %sub52 to i64, !dbg !1848
  %arrayidx54 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom53, !dbg !1848
  %40 = load i32, i32* %arrayidx54, align 4, !dbg !1848
  %shr55 = lshr i32 %40, 18, !dbg !1848
  %41 = load i32, i32* %i, align 4, !dbg !1848
  %sub56 = sub nsw i32 %41, 15, !dbg !1848
  %idxprom57 = sext i32 %sub56 to i64, !dbg !1848
  %arrayidx58 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom57, !dbg !1848
  %42 = load i32, i32* %arrayidx58, align 4, !dbg !1848
  %shl59 = shl i32 %42, 14, !dbg !1848
  %or60 = or i32 %shr55, %shl59, !dbg !1848
  %xor61 = xor i32 %or51, %or60, !dbg !1848
  %43 = load i32, i32* %i, align 4, !dbg !1848
  %sub62 = sub nsw i32 %43, 15, !dbg !1848
  %idxprom63 = sext i32 %sub62 to i64, !dbg !1848
  %arrayidx64 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom63, !dbg !1848
  %44 = load i32, i32* %arrayidx64, align 4, !dbg !1848
  %shr65 = lshr i32 %44, 3, !dbg !1848
  %xor66 = xor i32 %xor61, %shr65, !dbg !1848
  %add67 = add i32 %add, %xor66, !dbg !1849
  %45 = load i32, i32* %i, align 4, !dbg !1850
  %sub68 = sub nsw i32 %45, 16, !dbg !1851
  %idxprom69 = sext i32 %sub68 to i64, !dbg !1852
  %arrayidx70 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom69, !dbg !1852
  %46 = load i32, i32* %arrayidx70, align 4, !dbg !1852
  %add71 = add i32 %add67, %46, !dbg !1853
  %47 = load i32, i32* %i, align 4, !dbg !1854
  %idxprom72 = sext i32 %47 to i64, !dbg !1855
  %arrayidx73 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom72, !dbg !1855
  store i32 %add71, i32* %arrayidx73, align 4, !dbg !1856
  br label %for.inc74, !dbg !1855

for.inc74:                                        ; preds = %for.body20
  %48 = load i32, i32* %i, align 4, !dbg !1857
  %inc75 = add nsw i32 %48, 1, !dbg !1857
  store i32 %inc75, i32* %i, align 4, !dbg !1857
  br label %for.cond18, !dbg !1858, !llvm.loop !1859

for.end76:                                        ; preds = %for.cond18
  store i32 0, i32* %i, align 4, !dbg !1861
  br label %for.cond77, !dbg !1863

for.cond77:                                       ; preds = %for.inc120, %for.end76
  %49 = load i32, i32* %i, align 4, !dbg !1864
  %cmp78 = icmp slt i32 %49, 64, !dbg !1866
  br i1 %cmp78, label %for.body79, label %for.end122, !dbg !1867

for.body79:                                       ; preds = %for.cond77
  call void @llvm.dbg.declare(metadata i32* %T1, metadata !1868, metadata !DIExpression()), !dbg !1870
  call void @llvm.dbg.declare(metadata i32* %T2, metadata !1871, metadata !DIExpression()), !dbg !1872
  %50 = load i32, i32* %HH, align 4, !dbg !1873
  %51 = load i32, i32* %EE, align 4, !dbg !1874
  %shr80 = lshr i32 %51, 6, !dbg !1874
  %52 = load i32, i32* %EE, align 4, !dbg !1874
  %shl81 = shl i32 %52, 26, !dbg !1874
  %or82 = or i32 %shr80, %shl81, !dbg !1874
  %53 = load i32, i32* %EE, align 4, !dbg !1874
  %shr83 = lshr i32 %53, 11, !dbg !1874
  %54 = load i32, i32* %EE, align 4, !dbg !1874
  %shl84 = shl i32 %54, 21, !dbg !1874
  %or85 = or i32 %shr83, %shl84, !dbg !1874
  %xor86 = xor i32 %or82, %or85, !dbg !1874
  %55 = load i32, i32* %EE, align 4, !dbg !1874
  %shr87 = lshr i32 %55, 25, !dbg !1874
  %56 = load i32, i32* %EE, align 4, !dbg !1874
  %shl88 = shl i32 %56, 7, !dbg !1874
  %or89 = or i32 %shr87, %shl88, !dbg !1874
  %xor90 = xor i32 %xor86, %or89, !dbg !1874
  %add91 = add i32 %50, %xor90, !dbg !1875
  %57 = load i32, i32* %EE, align 4, !dbg !1876
  %58 = load i32, i32* %FF, align 4, !dbg !1876
  %and = and i32 %57, %58, !dbg !1876
  %59 = load i32, i32* %EE, align 4, !dbg !1876
  %neg = xor i32 %59, -1, !dbg !1876
  %60 = load i32, i32* %GG, align 4, !dbg !1876
  %and92 = and i32 %neg, %60, !dbg !1876
  %xor93 = xor i32 %and, %and92, !dbg !1876
  %add94 = add i32 %add91, %xor93, !dbg !1877
  %61 = load i32, i32* %i, align 4, !dbg !1878
  %idxprom95 = sext i32 %61 to i64, !dbg !1879
  %arrayidx96 = getelementptr inbounds [64 x i32], [64 x i32]* @mavlink_sha256_constant_256, i64 0, i64 %idxprom95, !dbg !1879
  %62 = load i32, i32* %arrayidx96, align 4, !dbg !1879
  %add97 = add i32 %add94, %62, !dbg !1880
  %63 = load i32, i32* %i, align 4, !dbg !1881
  %idxprom98 = sext i32 %63 to i64, !dbg !1882
  %arrayidx99 = getelementptr inbounds [64 x i32], [64 x i32]* %data, i64 0, i64 %idxprom98, !dbg !1882
  %64 = load i32, i32* %arrayidx99, align 4, !dbg !1882
  %add100 = add i32 %add97, %64, !dbg !1883
  store i32 %add100, i32* %T1, align 4, !dbg !1884
  %65 = load i32, i32* %AA, align 4, !dbg !1885
  %shr101 = lshr i32 %65, 2, !dbg !1885
  %66 = load i32, i32* %AA, align 4, !dbg !1885
  %shl102 = shl i32 %66, 30, !dbg !1885
  %or103 = or i32 %shr101, %shl102, !dbg !1885
  %67 = load i32, i32* %AA, align 4, !dbg !1885
  %shr104 = lshr i32 %67, 13, !dbg !1885
  %68 = load i32, i32* %AA, align 4, !dbg !1885
  %shl105 = shl i32 %68, 19, !dbg !1885
  %or106 = or i32 %shr104, %shl105, !dbg !1885
  %xor107 = xor i32 %or103, %or106, !dbg !1885
  %69 = load i32, i32* %AA, align 4, !dbg !1885
  %shr108 = lshr i32 %69, 22, !dbg !1885
  %70 = load i32, i32* %AA, align 4, !dbg !1885
  %shl109 = shl i32 %70, 10, !dbg !1885
  %or110 = or i32 %shr108, %shl109, !dbg !1885
  %xor111 = xor i32 %xor107, %or110, !dbg !1885
  %71 = load i32, i32* %AA, align 4, !dbg !1886
  %72 = load i32, i32* %BB, align 4, !dbg !1886
  %and112 = and i32 %71, %72, !dbg !1886
  %73 = load i32, i32* %AA, align 4, !dbg !1886
  %74 = load i32, i32* %CC, align 4, !dbg !1886
  %and113 = and i32 %73, %74, !dbg !1886
  %xor114 = xor i32 %and112, %and113, !dbg !1886
  %75 = load i32, i32* %BB, align 4, !dbg !1886
  %76 = load i32, i32* %CC, align 4, !dbg !1886
  %and115 = and i32 %75, %76, !dbg !1886
  %xor116 = xor i32 %xor114, %and115, !dbg !1886
  %add117 = add i32 %xor111, %xor116, !dbg !1887
  store i32 %add117, i32* %T2, align 4, !dbg !1888
  %77 = load i32, i32* %GG, align 4, !dbg !1889
  store i32 %77, i32* %HH, align 4, !dbg !1890
  %78 = load i32, i32* %FF, align 4, !dbg !1891
  store i32 %78, i32* %GG, align 4, !dbg !1892
  %79 = load i32, i32* %EE, align 4, !dbg !1893
  store i32 %79, i32* %FF, align 4, !dbg !1894
  %80 = load i32, i32* %DD, align 4, !dbg !1895
  %81 = load i32, i32* %T1, align 4, !dbg !1896
  %add118 = add i32 %80, %81, !dbg !1897
  store i32 %add118, i32* %EE, align 4, !dbg !1898
  %82 = load i32, i32* %CC, align 4, !dbg !1899
  store i32 %82, i32* %DD, align 4, !dbg !1900
  %83 = load i32, i32* %BB, align 4, !dbg !1901
  store i32 %83, i32* %CC, align 4, !dbg !1902
  %84 = load i32, i32* %AA, align 4, !dbg !1903
  store i32 %84, i32* %BB, align 4, !dbg !1904
  %85 = load i32, i32* %T1, align 4, !dbg !1905
  %86 = load i32, i32* %T2, align 4, !dbg !1906
  %add119 = add i32 %85, %86, !dbg !1907
  store i32 %add119, i32* %AA, align 4, !dbg !1908
  br label %for.inc120, !dbg !1909

for.inc120:                                       ; preds = %for.body79
  %87 = load i32, i32* %i, align 4, !dbg !1910
  %inc121 = add nsw i32 %87, 1, !dbg !1910
  store i32 %inc121, i32* %i, align 4, !dbg !1910
  br label %for.cond77, !dbg !1911, !llvm.loop !1912

for.end122:                                       ; preds = %for.cond77
  %88 = load i32, i32* %AA, align 4, !dbg !1914
  %89 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1915
  %counter123 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %89, i32 0, i32 1, !dbg !1916
  %arrayidx124 = getelementptr inbounds [8 x i32], [8 x i32]* %counter123, i64 0, i64 0, !dbg !1915
  %90 = load i32, i32* %arrayidx124, align 4, !dbg !1917
  %add125 = add i32 %90, %88, !dbg !1917
  store i32 %add125, i32* %arrayidx124, align 4, !dbg !1917
  %91 = load i32, i32* %BB, align 4, !dbg !1918
  %92 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1919
  %counter126 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %92, i32 0, i32 1, !dbg !1920
  %arrayidx127 = getelementptr inbounds [8 x i32], [8 x i32]* %counter126, i64 0, i64 1, !dbg !1919
  %93 = load i32, i32* %arrayidx127, align 4, !dbg !1921
  %add128 = add i32 %93, %91, !dbg !1921
  store i32 %add128, i32* %arrayidx127, align 4, !dbg !1921
  %94 = load i32, i32* %CC, align 4, !dbg !1922
  %95 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1923
  %counter129 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %95, i32 0, i32 1, !dbg !1924
  %arrayidx130 = getelementptr inbounds [8 x i32], [8 x i32]* %counter129, i64 0, i64 2, !dbg !1923
  %96 = load i32, i32* %arrayidx130, align 4, !dbg !1925
  %add131 = add i32 %96, %94, !dbg !1925
  store i32 %add131, i32* %arrayidx130, align 4, !dbg !1925
  %97 = load i32, i32* %DD, align 4, !dbg !1926
  %98 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1927
  %counter132 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %98, i32 0, i32 1, !dbg !1928
  %arrayidx133 = getelementptr inbounds [8 x i32], [8 x i32]* %counter132, i64 0, i64 3, !dbg !1927
  %99 = load i32, i32* %arrayidx133, align 4, !dbg !1929
  %add134 = add i32 %99, %97, !dbg !1929
  store i32 %add134, i32* %arrayidx133, align 4, !dbg !1929
  %100 = load i32, i32* %EE, align 4, !dbg !1930
  %101 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1931
  %counter135 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %101, i32 0, i32 1, !dbg !1932
  %arrayidx136 = getelementptr inbounds [8 x i32], [8 x i32]* %counter135, i64 0, i64 4, !dbg !1931
  %102 = load i32, i32* %arrayidx136, align 4, !dbg !1933
  %add137 = add i32 %102, %100, !dbg !1933
  store i32 %add137, i32* %arrayidx136, align 4, !dbg !1933
  %103 = load i32, i32* %FF, align 4, !dbg !1934
  %104 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1935
  %counter138 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %104, i32 0, i32 1, !dbg !1936
  %arrayidx139 = getelementptr inbounds [8 x i32], [8 x i32]* %counter138, i64 0, i64 5, !dbg !1935
  %105 = load i32, i32* %arrayidx139, align 4, !dbg !1937
  %add140 = add i32 %105, %103, !dbg !1937
  store i32 %add140, i32* %arrayidx139, align 4, !dbg !1937
  %106 = load i32, i32* %GG, align 4, !dbg !1938
  %107 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1939
  %counter141 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %107, i32 0, i32 1, !dbg !1940
  %arrayidx142 = getelementptr inbounds [8 x i32], [8 x i32]* %counter141, i64 0, i64 6, !dbg !1939
  %108 = load i32, i32* %arrayidx142, align 4, !dbg !1941
  %add143 = add i32 %108, %106, !dbg !1941
  store i32 %add143, i32* %arrayidx142, align 4, !dbg !1941
  %109 = load i32, i32* %HH, align 4, !dbg !1942
  %110 = load %struct.mavlink_sha256_ctx*, %struct.mavlink_sha256_ctx** %m.addr, align 8, !dbg !1943
  %counter144 = getelementptr inbounds %struct.mavlink_sha256_ctx, %struct.mavlink_sha256_ctx* %110, i32 0, i32 1, !dbg !1944
  %arrayidx145 = getelementptr inbounds [8 x i32], [8 x i32]* %counter144, i64 0, i64 7, !dbg !1943
  %111 = load i32, i32* %arrayidx145, align 4, !dbg !1945
  %add146 = add i32 %111, %109, !dbg !1945
  store i32 %add146, i32* %arrayidx145, align 4, !dbg !1945
  ret void, !dbg !1946
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @crc_init(i16* %crcAccum) #0 !dbg !1947 {
entry:
  %crcAccum.addr = alloca i16*, align 8
  store i16* %crcAccum, i16** %crcAccum.addr, align 8
  call void @llvm.dbg.declare(metadata i16** %crcAccum.addr, metadata !1950, metadata !DIExpression()), !dbg !1951
  %0 = load i16*, i16** %crcAccum.addr, align 8, !dbg !1952
  store i16 -1, i16* %0, align 2, !dbg !1953
  ret void, !dbg !1954
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { argmemonly nounwind willreturn }
attributes #6 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { nounwind readnone }
attributes #8 = { nounwind }
attributes #9 = { nounwind readonly }

!llvm.dbg.cu = !{!27}
!llvm.module.flags = !{!232, !233, !234}
!llvm.ident = !{!235}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "mavlink_message_crcs", scope: !2, file: !3, line: 512, type: !229, isLocal: true, isDefinition: true)
!2 = distinct !DISubprogram(name: "mavlink_get_msg_entry", scope: !3, file: !3, line: 510, type: !4, scopeLine: 511, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!3 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../mavlink_helpers.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-server/input/source_code/mavlink")
!4 = !DISubroutineType(types: !5)
!5 = !{!6, !13}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64)
!7 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !8)
!8 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_msg_entry_t", file: !9, line: 291, baseType: !10)
!9 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../mavlink_types.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-server/input/source_code/mavlink")
!10 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_msg_entry", file: !9, line: 283, size: 96, elements: !11)
!11 = !{!12, !18, !22, !23, !24, !25, !26}
!12 = !DIDerivedType(tag: DW_TAG_member, name: "msgid", scope: !10, file: !9, line: 284, baseType: !13, size: 32)
!13 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint32_t", file: !14, line: 26, baseType: !15)
!14 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/stdint-uintn.h", directory: "")
!15 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint32_t", file: !16, line: 42, baseType: !17)
!16 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types.h", directory: "")
!17 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!18 = !DIDerivedType(tag: DW_TAG_member, name: "crc_extra", scope: !10, file: !9, line: 285, baseType: !19, size: 8, offset: 32)
!19 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint8_t", file: !14, line: 24, baseType: !20)
!20 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint8_t", file: !16, line: 38, baseType: !21)
!21 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!22 = !DIDerivedType(tag: DW_TAG_member, name: "min_msg_len", scope: !10, file: !9, line: 286, baseType: !19, size: 8, offset: 40)
!23 = !DIDerivedType(tag: DW_TAG_member, name: "max_msg_len", scope: !10, file: !9, line: 287, baseType: !19, size: 8, offset: 48)
!24 = !DIDerivedType(tag: DW_TAG_member, name: "flags", scope: !10, file: !9, line: 288, baseType: !19, size: 8, offset: 56)
!25 = !DIDerivedType(tag: DW_TAG_member, name: "target_system_ofs", scope: !10, file: !9, line: 289, baseType: !19, size: 8, offset: 64)
!26 = !DIDerivedType(tag: DW_TAG_member, name: "target_component_ofs", scope: !10, file: !9, line: 290, baseType: !19, size: 8, offset: 72)
!27 = distinct !DICompileUnit(language: DW_LANG_C99, file: !28, producer: "clang version 10.0.0-4ubuntu1 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !29, retainedTypes: !101, globals: !125, splitDebugInlining: false, nameTableKind: None)
!28 = !DIFile(filename: "mavlink_server.c", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-server/input/source_code/mavlink")
!29 = !{!30, !48, !54, !66, !95}
!30 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !9, line: 186, baseType: !17, size: 32, elements: !31)
!31 = !{!32, !33, !34, !35, !36, !37, !38, !39, !40, !41, !42, !43, !44, !45, !46, !47}
!32 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_UNINIT", value: 0, isUnsigned: true)
!33 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_IDLE", value: 1, isUnsigned: true)
!34 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_STX", value: 2, isUnsigned: true)
!35 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_LENGTH", value: 3, isUnsigned: true)
!36 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_INCOMPAT_FLAGS", value: 4, isUnsigned: true)
!37 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_COMPAT_FLAGS", value: 5, isUnsigned: true)
!38 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_SEQ", value: 6, isUnsigned: true)
!39 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_SYSID", value: 7, isUnsigned: true)
!40 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_COMPID", value: 8, isUnsigned: true)
!41 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_MSGID1", value: 9, isUnsigned: true)
!42 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_MSGID2", value: 10, isUnsigned: true)
!43 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_MSGID3", value: 11, isUnsigned: true)
!44 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_PAYLOAD", value: 12, isUnsigned: true)
!45 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_CRC1", value: 13, isUnsigned: true)
!46 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_GOT_BAD_CRC1", value: 14, isUnsigned: true)
!47 = !DIEnumerator(name: "MAVLINK_PARSE_STATE_SIGNATURE_WAIT", value: 15, isUnsigned: true)
!48 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !9, line: 165, baseType: !17, size: 32, elements: !49)
!49 = !{!50, !51, !52, !53}
!50 = !DIEnumerator(name: "MAVLINK_COMM_0", value: 0, isUnsigned: true)
!51 = !DIEnumerator(name: "MAVLINK_COMM_1", value: 1, isUnsigned: true)
!52 = !DIEnumerator(name: "MAVLINK_COMM_2", value: 2, isUnsigned: true)
!53 = !DIEnumerator(name: "MAVLINK_COMM_3", value: 3, isUnsigned: true)
!54 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "__socket_type", file: !55, line: 24, baseType: !17, size: 32, elements: !56)
!55 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/socket_type.h", directory: "")
!56 = !{!57, !58, !59, !60, !61, !62, !63, !64, !65}
!57 = !DIEnumerator(name: "SOCK_STREAM", value: 1, isUnsigned: true)
!58 = !DIEnumerator(name: "SOCK_DGRAM", value: 2, isUnsigned: true)
!59 = !DIEnumerator(name: "SOCK_RAW", value: 3, isUnsigned: true)
!60 = !DIEnumerator(name: "SOCK_RDM", value: 4, isUnsigned: true)
!61 = !DIEnumerator(name: "SOCK_SEQPACKET", value: 5, isUnsigned: true)
!62 = !DIEnumerator(name: "SOCK_DCCP", value: 6, isUnsigned: true)
!63 = !DIEnumerator(name: "SOCK_PACKET", value: 10, isUnsigned: true)
!64 = !DIEnumerator(name: "SOCK_CLOEXEC", value: 524288, isUnsigned: true)
!65 = !DIEnumerator(name: "SOCK_NONBLOCK", value: 2048, isUnsigned: true)
!66 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !67, line: 40, baseType: !17, size: 32, elements: !68)
!67 = !DIFile(filename: "/usr/include/netinet/in.h", directory: "")
!68 = !{!69, !70, !71, !72, !73, !74, !75, !76, !77, !78, !79, !80, !81, !82, !83, !84, !85, !86, !87, !88, !89, !90, !91, !92, !93, !94}
!69 = !DIEnumerator(name: "IPPROTO_IP", value: 0, isUnsigned: true)
!70 = !DIEnumerator(name: "IPPROTO_ICMP", value: 1, isUnsigned: true)
!71 = !DIEnumerator(name: "IPPROTO_IGMP", value: 2, isUnsigned: true)
!72 = !DIEnumerator(name: "IPPROTO_IPIP", value: 4, isUnsigned: true)
!73 = !DIEnumerator(name: "IPPROTO_TCP", value: 6, isUnsigned: true)
!74 = !DIEnumerator(name: "IPPROTO_EGP", value: 8, isUnsigned: true)
!75 = !DIEnumerator(name: "IPPROTO_PUP", value: 12, isUnsigned: true)
!76 = !DIEnumerator(name: "IPPROTO_UDP", value: 17, isUnsigned: true)
!77 = !DIEnumerator(name: "IPPROTO_IDP", value: 22, isUnsigned: true)
!78 = !DIEnumerator(name: "IPPROTO_TP", value: 29, isUnsigned: true)
!79 = !DIEnumerator(name: "IPPROTO_DCCP", value: 33, isUnsigned: true)
!80 = !DIEnumerator(name: "IPPROTO_IPV6", value: 41, isUnsigned: true)
!81 = !DIEnumerator(name: "IPPROTO_RSVP", value: 46, isUnsigned: true)
!82 = !DIEnumerator(name: "IPPROTO_GRE", value: 47, isUnsigned: true)
!83 = !DIEnumerator(name: "IPPROTO_ESP", value: 50, isUnsigned: true)
!84 = !DIEnumerator(name: "IPPROTO_AH", value: 51, isUnsigned: true)
!85 = !DIEnumerator(name: "IPPROTO_MTP", value: 92, isUnsigned: true)
!86 = !DIEnumerator(name: "IPPROTO_BEETPH", value: 94, isUnsigned: true)
!87 = !DIEnumerator(name: "IPPROTO_ENCAP", value: 98, isUnsigned: true)
!88 = !DIEnumerator(name: "IPPROTO_PIM", value: 103, isUnsigned: true)
!89 = !DIEnumerator(name: "IPPROTO_COMP", value: 108, isUnsigned: true)
!90 = !DIEnumerator(name: "IPPROTO_SCTP", value: 132, isUnsigned: true)
!91 = !DIEnumerator(name: "IPPROTO_UDPLITE", value: 136, isUnsigned: true)
!92 = !DIEnumerator(name: "IPPROTO_MPLS", value: 137, isUnsigned: true)
!93 = !DIEnumerator(name: "IPPROTO_RAW", value: 255, isUnsigned: true)
!94 = !DIEnumerator(name: "IPPROTO_MAX", value: 256, isUnsigned: true)
!95 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !9, line: 205, baseType: !17, size: 32, elements: !96)
!96 = !{!97, !98, !99, !100}
!97 = !DIEnumerator(name: "MAVLINK_FRAMING_INCOMPLETE", value: 0, isUnsigned: true)
!98 = !DIEnumerator(name: "MAVLINK_FRAMING_OK", value: 1, isUnsigned: true)
!99 = !DIEnumerator(name: "MAVLINK_FRAMING_BAD_CRC", value: 2, isUnsigned: true)
!100 = !DIEnumerator(name: "MAVLINK_FRAMING_BAD_SIGNATURE", value: 3, isUnsigned: true)
!101 = !{!102, !115, !13, !112, !116, !117, !19, !118, !120, !122, !124}
!102 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !103, size: 64)
!103 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sockaddr", file: !104, line: 178, size: 128, elements: !105)
!104 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/socket.h", directory: "")
!105 = !{!106, !110}
!106 = !DIDerivedType(tag: DW_TAG_member, name: "sa_family", scope: !103, file: !104, line: 180, baseType: !107, size: 16)
!107 = !DIDerivedType(tag: DW_TAG_typedef, name: "sa_family_t", file: !108, line: 28, baseType: !109)
!108 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/sockaddr.h", directory: "")
!109 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!110 = !DIDerivedType(tag: DW_TAG_member, name: "sa_data", scope: !103, file: !104, line: 181, baseType: !111, size: 112, offset: 16)
!111 = !DICompositeType(tag: DW_TAG_array_type, baseType: !112, size: 112, elements: !113)
!112 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!113 = !{!114}
!114 = !DISubrange(count: 14)
!115 = !DIDerivedType(tag: DW_TAG_typedef, name: "in_addr_t", file: !67, line: 30, baseType: !13)
!116 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !112, size: 64)
!117 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!118 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !119, size: 64)
!119 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !19)
!120 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !121, size: 64)
!121 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !112)
!122 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !123, size: 64)
!123 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !21)
!124 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 64)
!125 = !{!0, !126, !133, !172}
!126 = !DIGlobalVariableExpression(var: !127, expr: !DIExpression())
!127 = distinct !DIGlobalVariable(name: "mavlink_sha256_constant_256", scope: !27, file: !128, line: 72, type: !129, isLocal: true, isDefinition: true)
!128 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../mavlink_sha256.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-server/input/source_code/mavlink")
!129 = !DICompositeType(tag: DW_TAG_array_type, baseType: !130, size: 2048, elements: !131)
!130 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !13)
!131 = !{!132}
!132 = !DISubrange(count: 64)
!133 = !DIGlobalVariableExpression(var: !134, expr: !DIExpression())
!134 = distinct !DIGlobalVariable(name: "m_mavlink_buffer", scope: !135, file: !3, line: 46, type: !169, isLocal: true, isDefinition: true)
!135 = distinct !DISubprogram(name: "mavlink_get_channel_buffer", scope: !3, file: !3, line: 39, type: !136, scopeLine: 40, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!136 = !DISubroutineType(types: !137)
!137 = !{!138, !19}
!138 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !139, size: 64)
!139 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_message_t", file: !9, line: 121, baseType: !140)
!140 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_message", file: !9, line: 107, size: 2328, elements: !141)
!141 = !{!142, !145, !146, !147, !148, !149, !150, !151, !152, !153, !160, !164}
!142 = !DIDerivedType(tag: DW_TAG_member, name: "checksum", scope: !140, file: !9, line: 107, baseType: !143, size: 16)
!143 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint16_t", file: !14, line: 25, baseType: !144)
!144 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint16_t", file: !16, line: 40, baseType: !109)
!145 = !DIDerivedType(tag: DW_TAG_member, name: "magic", scope: !140, file: !9, line: 107, baseType: !19, size: 8, offset: 16)
!146 = !DIDerivedType(tag: DW_TAG_member, name: "len", scope: !140, file: !9, line: 107, baseType: !19, size: 8, offset: 24)
!147 = !DIDerivedType(tag: DW_TAG_member, name: "incompat_flags", scope: !140, file: !9, line: 107, baseType: !19, size: 8, offset: 32)
!148 = !DIDerivedType(tag: DW_TAG_member, name: "compat_flags", scope: !140, file: !9, line: 107, baseType: !19, size: 8, offset: 40)
!149 = !DIDerivedType(tag: DW_TAG_member, name: "seq", scope: !140, file: !9, line: 107, baseType: !19, size: 8, offset: 48)
!150 = !DIDerivedType(tag: DW_TAG_member, name: "sysid", scope: !140, file: !9, line: 107, baseType: !19, size: 8, offset: 56)
!151 = !DIDerivedType(tag: DW_TAG_member, name: "compid", scope: !140, file: !9, line: 107, baseType: !19, size: 8, offset: 64)
!152 = !DIDerivedType(tag: DW_TAG_member, name: "msgid", scope: !140, file: !9, line: 107, baseType: !13, size: 24, offset: 72, flags: DIFlagBitField, extraData: i64 72)
!153 = !DIDerivedType(tag: DW_TAG_member, name: "payload64", scope: !140, file: !9, line: 107, baseType: !154, size: 2112, offset: 96)
!154 = !DICompositeType(tag: DW_TAG_array_type, baseType: !155, size: 2112, elements: !158)
!155 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint64_t", file: !14, line: 27, baseType: !156)
!156 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint64_t", file: !16, line: 45, baseType: !157)
!157 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!158 = !{!159}
!159 = !DISubrange(count: 33)
!160 = !DIDerivedType(tag: DW_TAG_member, name: "ck", scope: !140, file: !9, line: 107, baseType: !161, size: 16, offset: 2208)
!161 = !DICompositeType(tag: DW_TAG_array_type, baseType: !19, size: 16, elements: !162)
!162 = !{!163}
!163 = !DISubrange(count: 2)
!164 = !DIDerivedType(tag: DW_TAG_member, name: "signature", scope: !140, file: !9, line: 107, baseType: !165, size: 104, offset: 2224)
!165 = !DICompositeType(tag: DW_TAG_array_type, baseType: !19, size: 104, elements: !166)
!166 = !{!167}
!167 = !DISubrange(count: 13)
!168 = !{}
!169 = !DICompositeType(tag: DW_TAG_array_type, baseType: !139, size: 37248, elements: !170)
!170 = !{!171}
!171 = !DISubrange(count: 16)
!172 = !DIGlobalVariableExpression(var: !173, expr: !DIExpression())
!173 = distinct !DIGlobalVariable(name: "m_mavlink_status", scope: !174, file: !3, line: 29, type: !228, isLocal: true, isDefinition: true)
!174 = distinct !DISubprogram(name: "mavlink_get_channel_status", scope: !3, file: !3, line: 23, type: !175, scopeLine: 24, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!175 = !DISubroutineType(types: !176)
!176 = !{!177, !19}
!177 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !178, size: 64)
!178 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_status_t", file: !9, line: 233, baseType: !179)
!179 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_status", file: !9, line: 219, size: 320, elements: !180)
!180 = !{!181, !182, !183, !184, !186, !187, !188, !189, !190, !191, !192, !193, !212}
!181 = !DIDerivedType(tag: DW_TAG_member, name: "msg_received", scope: !179, file: !9, line: 220, baseType: !19, size: 8)
!182 = !DIDerivedType(tag: DW_TAG_member, name: "buffer_overrun", scope: !179, file: !9, line: 221, baseType: !19, size: 8, offset: 8)
!183 = !DIDerivedType(tag: DW_TAG_member, name: "parse_error", scope: !179, file: !9, line: 222, baseType: !19, size: 8, offset: 16)
!184 = !DIDerivedType(tag: DW_TAG_member, name: "parse_state", scope: !179, file: !9, line: 223, baseType: !185, size: 32, offset: 32)
!185 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_parse_state_t", file: !9, line: 203, baseType: !30)
!186 = !DIDerivedType(tag: DW_TAG_member, name: "packet_idx", scope: !179, file: !9, line: 224, baseType: !19, size: 8, offset: 64)
!187 = !DIDerivedType(tag: DW_TAG_member, name: "current_rx_seq", scope: !179, file: !9, line: 225, baseType: !19, size: 8, offset: 72)
!188 = !DIDerivedType(tag: DW_TAG_member, name: "current_tx_seq", scope: !179, file: !9, line: 226, baseType: !19, size: 8, offset: 80)
!189 = !DIDerivedType(tag: DW_TAG_member, name: "packet_rx_success_count", scope: !179, file: !9, line: 227, baseType: !143, size: 16, offset: 96)
!190 = !DIDerivedType(tag: DW_TAG_member, name: "packet_rx_drop_count", scope: !179, file: !9, line: 228, baseType: !143, size: 16, offset: 112)
!191 = !DIDerivedType(tag: DW_TAG_member, name: "flags", scope: !179, file: !9, line: 229, baseType: !19, size: 8, offset: 128)
!192 = !DIDerivedType(tag: DW_TAG_member, name: "signature_wait", scope: !179, file: !9, line: 230, baseType: !19, size: 8, offset: 136)
!193 = !DIDerivedType(tag: DW_TAG_member, name: "signing", scope: !179, file: !9, line: 231, baseType: !194, size: 64, offset: 192)
!194 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !195, size: 64)
!195 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_signing", file: !9, line: 248, size: 448, elements: !196)
!196 = !{!197, !198, !199, !200, !204}
!197 = !DIDerivedType(tag: DW_TAG_member, name: "flags", scope: !195, file: !9, line: 249, baseType: !19, size: 8)
!198 = !DIDerivedType(tag: DW_TAG_member, name: "link_id", scope: !195, file: !9, line: 250, baseType: !19, size: 8, offset: 8)
!199 = !DIDerivedType(tag: DW_TAG_member, name: "timestamp", scope: !195, file: !9, line: 251, baseType: !155, size: 64, offset: 64)
!200 = !DIDerivedType(tag: DW_TAG_member, name: "secret_key", scope: !195, file: !9, line: 252, baseType: !201, size: 256, offset: 128)
!201 = !DICompositeType(tag: DW_TAG_array_type, baseType: !19, size: 256, elements: !202)
!202 = !{!203}
!203 = !DISubrange(count: 32)
!204 = !DIDerivedType(tag: DW_TAG_member, name: "accept_unsigned_callback", scope: !195, file: !9, line: 253, baseType: !205, size: 64, offset: 384)
!205 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_accept_unsigned_t", file: !9, line: 238, baseType: !206)
!206 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !207, size: 64)
!207 = !DISubroutineType(types: !208)
!208 = !{!209, !210, !13}
!209 = !DIBasicType(name: "_Bool", size: 8, encoding: DW_ATE_boolean)
!210 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !211, size: 64)
!211 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !178)
!212 = !DIDerivedType(tag: DW_TAG_member, name: "signing_streams", scope: !179, file: !9, line: 232, baseType: !213, size: 64, offset: 256)
!213 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !214, size: 64)
!214 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_signing_streams", file: !9, line: 263, size: 1168, elements: !215)
!215 = !{!216, !217}
!216 = !DIDerivedType(tag: DW_TAG_member, name: "num_signing_streams", scope: !214, file: !9, line: 264, baseType: !143, size: 16)
!217 = !DIDerivedType(tag: DW_TAG_member, name: "stream", scope: !214, file: !9, line: 270, baseType: !218, size: 1152, offset: 16)
!218 = !DICompositeType(tag: DW_TAG_array_type, baseType: !219, size: 1152, elements: !170)
!219 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_signing_stream", file: !9, line: 265, size: 72, elements: !220)
!220 = !{!221, !222, !223, !224}
!221 = !DIDerivedType(tag: DW_TAG_member, name: "link_id", scope: !219, file: !9, line: 266, baseType: !19, size: 8)
!222 = !DIDerivedType(tag: DW_TAG_member, name: "sysid", scope: !219, file: !9, line: 267, baseType: !19, size: 8, offset: 8)
!223 = !DIDerivedType(tag: DW_TAG_member, name: "compid", scope: !219, file: !9, line: 268, baseType: !19, size: 8, offset: 16)
!224 = !DIDerivedType(tag: DW_TAG_member, name: "timestamp_bytes", scope: !219, file: !9, line: 269, baseType: !225, size: 48, offset: 24)
!225 = !DICompositeType(tag: DW_TAG_array_type, baseType: !19, size: 48, elements: !226)
!226 = !{!227}
!227 = !DISubrange(count: 6)
!228 = !DICompositeType(tag: DW_TAG_array_type, baseType: !178, size: 5120, elements: !170)
!229 = !DICompositeType(tag: DW_TAG_array_type, baseType: !7, size: 18816, elements: !230)
!230 = !{!231}
!231 = !DISubrange(count: 196)
!232 = !{i32 7, !"Dwarf Version", i32 4}
!233 = !{i32 2, !"Debug Info Version", i32 3}
!234 = !{i32 1, !"wchar_size", i32 4}
!235 = !{!"clang version 10.0.0-4ubuntu1 "}
!236 = distinct !DISubprogram(name: "receive_heartbeat", scope: !28, file: !28, line: 18, type: !237, scopeLine: 18, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !27, retainedNodes: !168)
!237 = !DISubroutineType(types: !238)
!238 = !{null, !239}
!239 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!240 = !DILocalVariable(name: "sock", arg: 1, scope: !236, file: !28, line: 18, type: !239)
!241 = !DILocation(line: 18, column: 28, scope: !236)
!242 = !DILocalVariable(name: "buf", scope: !236, file: !28, line: 19, type: !243)
!243 = !DICompositeType(tag: DW_TAG_array_type, baseType: !19, size: 16328, elements: !244)
!244 = !{!245}
!245 = !DISubrange(count: 2041)
!246 = !DILocation(line: 19, column: 13, scope: !236)
!247 = !DILocalVariable(name: "src_addr", scope: !236, file: !28, line: 20, type: !248)
!248 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sockaddr_in", file: !67, line: 238, size: 128, elements: !249)
!249 = !{!250, !251, !253, !257}
!250 = !DIDerivedType(tag: DW_TAG_member, name: "sin_family", scope: !248, file: !67, line: 240, baseType: !107, size: 16)
!251 = !DIDerivedType(tag: DW_TAG_member, name: "sin_port", scope: !248, file: !67, line: 241, baseType: !252, size: 16, offset: 16)
!252 = !DIDerivedType(tag: DW_TAG_typedef, name: "in_port_t", file: !67, line: 119, baseType: !143)
!253 = !DIDerivedType(tag: DW_TAG_member, name: "sin_addr", scope: !248, file: !67, line: 242, baseType: !254, size: 32, offset: 32)
!254 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "in_addr", file: !67, line: 31, size: 32, elements: !255)
!255 = !{!256}
!256 = !DIDerivedType(tag: DW_TAG_member, name: "s_addr", scope: !254, file: !67, line: 33, baseType: !115, size: 32)
!257 = !DIDerivedType(tag: DW_TAG_member, name: "sin_zero", scope: !248, file: !67, line: 245, baseType: !258, size: 64, offset: 64)
!258 = !DICompositeType(tag: DW_TAG_array_type, baseType: !21, size: 64, elements: !259)
!259 = !{!260}
!260 = !DISubrange(count: 8)
!261 = !DILocation(line: 20, column: 24, scope: !236)
!262 = !DILocalVariable(name: "addrlen", scope: !236, file: !28, line: 21, type: !263)
!263 = !DIDerivedType(tag: DW_TAG_typedef, name: "socklen_t", file: !264, line: 274, baseType: !265)
!264 = !DIFile(filename: "/usr/include/unistd.h", directory: "")
!265 = !DIDerivedType(tag: DW_TAG_typedef, name: "__socklen_t", file: !16, line: 209, baseType: !17)
!266 = !DILocation(line: 21, column: 15, scope: !236)
!267 = !DILocalVariable(name: "recsize", scope: !236, file: !28, line: 22, type: !268)
!268 = !DIDerivedType(tag: DW_TAG_typedef, name: "ssize_t", file: !269, line: 77, baseType: !270)
!269 = !DIFile(filename: "/usr/include/stdio.h", directory: "")
!270 = !DIDerivedType(tag: DW_TAG_typedef, name: "__ssize_t", file: !16, line: 193, baseType: !271)
!271 = !DIBasicType(name: "long int", size: 64, encoding: DW_ATE_signed)
!272 = !DILocation(line: 22, column: 13, scope: !236)
!273 = !DILocation(line: 24, column: 24, scope: !236)
!274 = !DILocation(line: 24, column: 30, scope: !236)
!275 = !DILocation(line: 24, column: 53, scope: !236)
!276 = !DILocation(line: 24, column: 15, scope: !236)
!277 = !DILocation(line: 24, column: 13, scope: !236)
!278 = !DILocalVariable(name: "len", scope: !236, file: !28, line: 25, type: !239)
!279 = !DILocation(line: 25, column: 9, scope: !236)
!280 = !DILocation(line: 25, column: 14, scope: !236)
!281 = !DILocation(line: 25, column: 21, scope: !236)
!282 = !DILocation(line: 27, column: 9, scope: !283)
!283 = distinct !DILexicalBlock(scope: !236, file: !28, line: 27, column: 9)
!284 = !DILocation(line: 27, column: 17, scope: !283)
!285 = !DILocation(line: 27, column: 9, scope: !236)
!286 = !DILocalVariable(name: "msg", scope: !287, file: !28, line: 28, type: !139)
!287 = distinct !DILexicalBlock(scope: !283, file: !28, line: 27, column: 22)
!288 = !DILocation(line: 28, column: 27, scope: !287)
!289 = !DILocalVariable(name: "status", scope: !287, file: !28, line: 29, type: !178)
!290 = !DILocation(line: 29, column: 26, scope: !287)
!291 = !DILocalVariable(name: "i", scope: !292, file: !28, line: 32, type: !239)
!292 = distinct !DILexicalBlock(scope: !287, file: !28, line: 32, column: 9)
!293 = !DILocation(line: 32, column: 18, scope: !292)
!294 = !DILocation(line: 32, column: 14, scope: !292)
!295 = !DILocation(line: 32, column: 25, scope: !296)
!296 = distinct !DILexicalBlock(scope: !292, file: !28, line: 32, column: 9)
!297 = !DILocation(line: 32, column: 29, scope: !296)
!298 = !DILocation(line: 32, column: 27, scope: !296)
!299 = !DILocation(line: 32, column: 9, scope: !292)
!300 = !DILocation(line: 33, column: 56, scope: !301)
!301 = distinct !DILexicalBlock(scope: !302, file: !28, line: 33, column: 17)
!302 = distinct !DILexicalBlock(scope: !296, file: !28, line: 32, column: 43)
!303 = !DILocation(line: 33, column: 52, scope: !301)
!304 = !DILocation(line: 33, column: 17, scope: !301)
!305 = !DILocation(line: 33, column: 17, scope: !302)
!306 = !DILocation(line: 35, column: 25, scope: !307)
!307 = distinct !DILexicalBlock(scope: !308, file: !28, line: 35, column: 21)
!308 = distinct !DILexicalBlock(scope: !301, file: !28, line: 33, column: 76)
!309 = !DILocation(line: 35, column: 31, scope: !307)
!310 = !DILocation(line: 35, column: 21, scope: !308)
!311 = !DILocalVariable(name: "heartbeat", scope: !312, file: !28, line: 36, type: !313)
!312 = distinct !DILexicalBlock(scope: !307, file: !28, line: 35, column: 60)
!313 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_heartbeat_t", file: !314, line: 16, baseType: !315)
!314 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../minimal/./mavlink_msg_heartbeat.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-server/input/source_code/mavlink")
!315 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__mavlink_heartbeat_t", file: !314, line: 9, size: 96, elements: !316)
!316 = !{!317, !318, !319, !320, !321, !322}
!317 = !DIDerivedType(tag: DW_TAG_member, name: "custom_mode", scope: !315, file: !314, line: 10, baseType: !13, size: 32)
!318 = !DIDerivedType(tag: DW_TAG_member, name: "type", scope: !315, file: !314, line: 11, baseType: !19, size: 8, offset: 32)
!319 = !DIDerivedType(tag: DW_TAG_member, name: "autopilot", scope: !315, file: !314, line: 12, baseType: !19, size: 8, offset: 40)
!320 = !DIDerivedType(tag: DW_TAG_member, name: "base_mode", scope: !315, file: !314, line: 13, baseType: !19, size: 8, offset: 48)
!321 = !DIDerivedType(tag: DW_TAG_member, name: "system_status", scope: !315, file: !314, line: 14, baseType: !19, size: 8, offset: 56)
!322 = !DIDerivedType(tag: DW_TAG_member, name: "mavlink_version", scope: !315, file: !314, line: 15, baseType: !19, size: 8, offset: 64)
!323 = !DILocation(line: 36, column: 41, scope: !312)
!324 = !DILocation(line: 37, column: 21, scope: !312)
!325 = !DILocation(line: 39, column: 32, scope: !312)
!326 = !DILocation(line: 39, column: 28, scope: !312)
!327 = !DILocation(line: 39, column: 43, scope: !312)
!328 = !DILocation(line: 39, column: 39, scope: !312)
!329 = !DILocation(line: 39, column: 61, scope: !312)
!330 = !DILocation(line: 39, column: 51, scope: !312)
!331 = !DILocation(line: 39, column: 77, scope: !312)
!332 = !DILocation(line: 39, column: 67, scope: !312)
!333 = !DILocation(line: 39, column: 98, scope: !312)
!334 = !DILocation(line: 39, column: 88, scope: !312)
!335 = !DILocation(line: 39, column: 119, scope: !312)
!336 = !DILocation(line: 39, column: 109, scope: !312)
!337 = !DILocation(line: 38, column: 21, scope: !312)
!338 = !DILocation(line: 40, column: 17, scope: !312)
!339 = !DILocation(line: 41, column: 13, scope: !308)
!340 = !DILocation(line: 42, column: 9, scope: !302)
!341 = !DILocation(line: 32, column: 39, scope: !296)
!342 = !DILocation(line: 32, column: 9, scope: !296)
!343 = distinct !{!343, !299, !344}
!344 = !DILocation(line: 42, column: 9, scope: !292)
!345 = !DILocation(line: 43, column: 5, scope: !287)
!346 = !DILocation(line: 43, column: 16, scope: !347)
!347 = distinct !DILexicalBlock(scope: !283, file: !28, line: 43, column: 16)
!348 = !DILocation(line: 43, column: 24, scope: !347)
!349 = !DILocation(line: 43, column: 28, scope: !347)
!350 = !DILocation(line: 43, column: 31, scope: !347)
!351 = !DILocation(line: 43, column: 37, scope: !347)
!352 = !DILocation(line: 43, column: 47, scope: !347)
!353 = !DILocation(line: 43, column: 50, scope: !347)
!354 = !DILocation(line: 43, column: 56, scope: !347)
!355 = !DILocation(line: 43, column: 16, scope: !283)
!356 = !DILocation(line: 44, column: 9, scope: !357)
!357 = distinct !DILexicalBlock(scope: !347, file: !28, line: 43, column: 72)
!358 = !DILocation(line: 45, column: 5, scope: !357)
!359 = !DILocation(line: 46, column: 1, scope: !236)
!360 = distinct !DISubprogram(name: "mavlink_parse_char", scope: !3, file: !3, line: 984, type: !361, scopeLine: 985, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!361 = !DISubroutineType(types: !362)
!362 = !{!19, !19, !19, !138, !177}
!363 = !DILocalVariable(name: "chan", arg: 1, scope: !360, file: !3, line: 984, type: !19)
!364 = !DILocation(line: 984, column: 51, scope: !360)
!365 = !DILocalVariable(name: "c", arg: 2, scope: !360, file: !3, line: 984, type: !19)
!366 = !DILocation(line: 984, column: 65, scope: !360)
!367 = !DILocalVariable(name: "r_message", arg: 3, scope: !360, file: !3, line: 984, type: !138)
!368 = !DILocation(line: 984, column: 87, scope: !360)
!369 = !DILocalVariable(name: "r_mavlink_status", arg: 4, scope: !360, file: !3, line: 984, type: !177)
!370 = !DILocation(line: 984, column: 116, scope: !360)
!371 = !DILocalVariable(name: "msg_received", scope: !360, file: !3, line: 986, type: !19)
!372 = !DILocation(line: 986, column: 13, scope: !360)
!373 = !DILocation(line: 986, column: 47, scope: !360)
!374 = !DILocation(line: 986, column: 53, scope: !360)
!375 = !DILocation(line: 986, column: 56, scope: !360)
!376 = !DILocation(line: 986, column: 67, scope: !360)
!377 = !DILocation(line: 986, column: 28, scope: !360)
!378 = !DILocation(line: 987, column: 9, scope: !379)
!379 = distinct !DILexicalBlock(scope: !360, file: !3, line: 987, column: 9)
!380 = !DILocation(line: 987, column: 22, scope: !379)
!381 = !DILocation(line: 987, column: 49, scope: !379)
!382 = !DILocation(line: 988, column: 2, scope: !379)
!383 = !DILocation(line: 988, column: 15, scope: !379)
!384 = !DILocation(line: 987, column: 9, scope: !360)
!385 = !DILocalVariable(name: "rxmsg", scope: !386, file: !3, line: 990, type: !138)
!386 = distinct !DILexicalBlock(scope: !379, file: !3, line: 988, column: 49)
!387 = !DILocation(line: 990, column: 25, scope: !386)
!388 = !DILocation(line: 990, column: 60, scope: !386)
!389 = !DILocation(line: 990, column: 33, scope: !386)
!390 = !DILocalVariable(name: "status", scope: !386, file: !3, line: 991, type: !177)
!391 = !DILocation(line: 991, column: 24, scope: !386)
!392 = !DILocation(line: 991, column: 60, scope: !386)
!393 = !DILocation(line: 991, column: 33, scope: !386)
!394 = !DILocation(line: 992, column: 23, scope: !386)
!395 = !DILocation(line: 992, column: 6, scope: !386)
!396 = !DILocation(line: 993, column: 6, scope: !386)
!397 = !DILocation(line: 993, column: 14, scope: !386)
!398 = !DILocation(line: 993, column: 27, scope: !386)
!399 = !DILocation(line: 994, column: 6, scope: !386)
!400 = !DILocation(line: 994, column: 14, scope: !386)
!401 = !DILocation(line: 994, column: 26, scope: !386)
!402 = !DILocation(line: 995, column: 10, scope: !403)
!403 = distinct !DILexicalBlock(scope: !386, file: !3, line: 995, column: 10)
!404 = !DILocation(line: 995, column: 12, scope: !403)
!405 = !DILocation(line: 995, column: 10, scope: !386)
!406 = !DILocation(line: 997, column: 7, scope: !407)
!407 = distinct !DILexicalBlock(scope: !403, file: !3, line: 996, column: 6)
!408 = !DILocation(line: 997, column: 15, scope: !407)
!409 = !DILocation(line: 997, column: 27, scope: !407)
!410 = !DILocation(line: 998, column: 7, scope: !407)
!411 = !DILocation(line: 998, column: 14, scope: !407)
!412 = !DILocation(line: 998, column: 18, scope: !407)
!413 = !DILocation(line: 999, column: 30, scope: !407)
!414 = !DILocation(line: 999, column: 7, scope: !407)
!415 = !DILocation(line: 1000, column: 6, scope: !407)
!416 = !DILocation(line: 1001, column: 6, scope: !386)
!417 = !DILocation(line: 1003, column: 12, scope: !360)
!418 = !DILocation(line: 1003, column: 5, scope: !360)
!419 = !DILocation(line: 1004, column: 1, scope: !360)
!420 = distinct !DISubprogram(name: "mavlink_msg_heartbeat_decode", scope: !314, file: !314, line: 332, type: !421, scopeLine: 333, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!421 = !DISubroutineType(types: !422)
!422 = !{null, !423, !425}
!423 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !424, size: 64)
!424 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !139)
!425 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !313, size: 64)
!426 = !DILocalVariable(name: "msg", arg: 1, scope: !420, file: !314, line: 332, type: !423)
!427 = !DILocation(line: 332, column: 74, scope: !420)
!428 = !DILocalVariable(name: "heartbeat", arg: 2, scope: !420, file: !314, line: 332, type: !425)
!429 = !DILocation(line: 332, column: 100, scope: !420)
!430 = !DILocalVariable(name: "len", scope: !420, file: !314, line: 342, type: !19)
!431 = !DILocation(line: 342, column: 17, scope: !420)
!432 = !DILocation(line: 342, column: 23, scope: !420)
!433 = !DILocation(line: 342, column: 28, scope: !420)
!434 = !DILocation(line: 342, column: 32, scope: !420)
!435 = !DILocation(line: 342, column: 64, scope: !420)
!436 = !DILocation(line: 342, column: 69, scope: !420)
!437 = !DILocation(line: 343, column: 16, scope: !420)
!438 = !DILocation(line: 343, column: 9, scope: !420)
!439 = !DILocation(line: 344, column: 12, scope: !420)
!440 = !DILocation(line: 344, column: 5, scope: !420)
!441 = !DILocation(line: 344, column: 23, scope: !420)
!442 = !DILocation(line: 344, column: 42, scope: !420)
!443 = !DILocation(line: 346, column: 1, scope: !420)
!444 = distinct !DISubprogram(name: "main", scope: !28, file: !28, line: 48, type: !445, scopeLine: 48, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !27, retainedNodes: !168)
!445 = !DISubroutineType(types: !446)
!446 = !{!239}
!447 = !DILocalVariable(name: "sock", scope: !444, file: !28, line: 50, type: !239)
!448 = !DILocation(line: 50, column: 9, scope: !444)
!449 = !DILocation(line: 50, column: 16, scope: !444)
!450 = !DILocation(line: 51, column: 9, scope: !451)
!451 = distinct !DILexicalBlock(scope: !444, file: !28, line: 51, column: 9)
!452 = !DILocation(line: 51, column: 14, scope: !451)
!453 = !DILocation(line: 51, column: 9, scope: !444)
!454 = !DILocation(line: 52, column: 9, scope: !455)
!455 = distinct !DILexicalBlock(scope: !451, file: !28, line: 51, column: 19)
!456 = !DILocation(line: 53, column: 9, scope: !455)
!457 = !DILocalVariable(name: "flags", scope: !444, file: !28, line: 57, type: !239)
!458 = !DILocation(line: 57, column: 9, scope: !444)
!459 = !DILocation(line: 57, column: 23, scope: !444)
!460 = !DILocation(line: 57, column: 17, scope: !444)
!461 = !DILocation(line: 58, column: 11, scope: !444)
!462 = !DILocation(line: 58, column: 26, scope: !444)
!463 = !DILocation(line: 58, column: 32, scope: !444)
!464 = !DILocation(line: 58, column: 5, scope: !444)
!465 = !DILocalVariable(name: "local_addr", scope: !444, file: !28, line: 61, type: !248)
!466 = !DILocation(line: 61, column: 24, scope: !444)
!467 = !DILocation(line: 62, column: 5, scope: !444)
!468 = !DILocation(line: 63, column: 16, scope: !444)
!469 = !DILocation(line: 63, column: 27, scope: !444)
!470 = !DILocation(line: 64, column: 16, scope: !444)
!471 = !DILocation(line: 64, column: 25, scope: !444)
!472 = !DILocation(line: 64, column: 32, scope: !444)
!473 = !DILocation(line: 65, column: 27, scope: !444)
!474 = !DILocation(line: 65, column: 16, scope: !444)
!475 = !DILocation(line: 65, column: 25, scope: !444)
!476 = !DILocation(line: 67, column: 14, scope: !477)
!477 = distinct !DILexicalBlock(scope: !444, file: !28, line: 67, column: 9)
!478 = !DILocation(line: 67, column: 20, scope: !477)
!479 = !DILocation(line: 67, column: 9, scope: !477)
!480 = !DILocation(line: 67, column: 72, scope: !477)
!481 = !DILocation(line: 67, column: 9, scope: !444)
!482 = !DILocation(line: 68, column: 9, scope: !483)
!483 = distinct !DILexicalBlock(scope: !477, file: !28, line: 67, column: 77)
!484 = !DILocation(line: 69, column: 15, scope: !483)
!485 = !DILocation(line: 69, column: 9, scope: !483)
!486 = !DILocation(line: 70, column: 9, scope: !483)
!487 = !DILocation(line: 73, column: 5, scope: !444)
!488 = !DILocation(line: 76, column: 5, scope: !444)
!489 = !DILocation(line: 77, column: 27, scope: !490)
!490 = distinct !DILexicalBlock(scope: !444, file: !28, line: 76, column: 15)
!491 = !DILocation(line: 77, column: 9, scope: !490)
!492 = !DILocation(line: 78, column: 9, scope: !490)
!493 = distinct !{!493, !488, !494}
!494 = !DILocation(line: 79, column: 5, scope: !444)
!495 = !DILocation(line: 83, column: 1, scope: !444)
!496 = distinct !DISubprogram(name: "mavlink_frame_char", scope: !3, file: !3, line: 906, type: !361, scopeLine: 907, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!497 = !DILocalVariable(name: "chan", arg: 1, scope: !496, file: !3, line: 906, type: !19)
!498 = !DILocation(line: 906, column: 51, scope: !496)
!499 = !DILocalVariable(name: "c", arg: 2, scope: !496, file: !3, line: 906, type: !19)
!500 = !DILocation(line: 906, column: 65, scope: !496)
!501 = !DILocalVariable(name: "r_message", arg: 3, scope: !496, file: !3, line: 906, type: !138)
!502 = !DILocation(line: 906, column: 87, scope: !496)
!503 = !DILocalVariable(name: "r_mavlink_status", arg: 4, scope: !496, file: !3, line: 906, type: !177)
!504 = !DILocation(line: 906, column: 116, scope: !496)
!505 = !DILocation(line: 908, column: 62, scope: !496)
!506 = !DILocation(line: 908, column: 35, scope: !496)
!507 = !DILocation(line: 909, column: 34, scope: !496)
!508 = !DILocation(line: 909, column: 7, scope: !496)
!509 = !DILocation(line: 910, column: 7, scope: !496)
!510 = !DILocation(line: 911, column: 7, scope: !496)
!511 = !DILocation(line: 912, column: 7, scope: !496)
!512 = !DILocation(line: 908, column: 9, scope: !496)
!513 = !DILocation(line: 908, column: 2, scope: !496)
!514 = !DILocalVariable(name: "chan", arg: 1, scope: !135, file: !3, line: 39, type: !19)
!515 = !DILocation(line: 39, column: 70, scope: !135)
!516 = !DILocation(line: 48, column: 27, scope: !135)
!517 = !DILocation(line: 48, column: 10, scope: !135)
!518 = !DILocation(line: 48, column: 2, scope: !135)
!519 = !DILocalVariable(name: "chan", arg: 1, scope: !174, file: !3, line: 23, type: !19)
!520 = !DILocation(line: 23, column: 69, scope: !174)
!521 = !DILocation(line: 31, column: 27, scope: !174)
!522 = !DILocation(line: 31, column: 10, scope: !174)
!523 = !DILocation(line: 31, column: 2, scope: !174)
!524 = distinct !DISubprogram(name: "_mav_parse_error", scope: !3, file: !3, line: 302, type: !525, scopeLine: 303, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!525 = !DISubroutineType(types: !526)
!526 = !{null, !177}
!527 = !DILocalVariable(name: "status", arg: 1, scope: !524, file: !3, line: 302, type: !177)
!528 = !DILocation(line: 302, column: 55, scope: !524)
!529 = !DILocation(line: 304, column: 5, scope: !524)
!530 = !DILocation(line: 304, column: 13, scope: !524)
!531 = !DILocation(line: 304, column: 24, scope: !524)
!532 = !DILocation(line: 305, column: 1, scope: !524)
!533 = distinct !DISubprogram(name: "mavlink_start_checksum", scope: !3, file: !3, line: 492, type: !534, scopeLine: 493, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!534 = !DISubroutineType(types: !535)
!535 = !{null, !138}
!536 = !DILocalVariable(name: "msg", arg: 1, scope: !533, file: !3, line: 492, type: !138)
!537 = !DILocation(line: 492, column: 63, scope: !533)
!538 = !DILocalVariable(name: "crcTmp", scope: !533, file: !3, line: 494, type: !143)
!539 = !DILocation(line: 494, column: 11, scope: !533)
!540 = !DILocation(line: 495, column: 2, scope: !533)
!541 = !DILocation(line: 496, column: 18, scope: !533)
!542 = !DILocation(line: 496, column: 2, scope: !533)
!543 = !DILocation(line: 496, column: 7, scope: !533)
!544 = !DILocation(line: 496, column: 16, scope: !533)
!545 = !DILocation(line: 497, column: 1, scope: !533)
!546 = distinct !DISubprogram(name: "mavlink_frame_char_buffer", scope: !3, file: !3, line: 582, type: !547, scopeLine: 587, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!547 = !DISubroutineType(types: !548)
!548 = !{!19, !138, !177, !19, !138, !177}
!549 = !DILocalVariable(name: "rxmsg", arg: 1, scope: !546, file: !3, line: 582, type: !138)
!550 = !DILocation(line: 582, column: 69, scope: !546)
!551 = !DILocalVariable(name: "status", arg: 2, scope: !546, file: !3, line: 583, type: !177)
!552 = !DILocation(line: 583, column: 68, scope: !546)
!553 = !DILocalVariable(name: "c", arg: 3, scope: !546, file: !3, line: 584, type: !19)
!554 = !DILocation(line: 584, column: 58, scope: !546)
!555 = !DILocalVariable(name: "r_message", arg: 4, scope: !546, file: !3, line: 585, type: !138)
!556 = !DILocation(line: 585, column: 69, scope: !546)
!557 = !DILocalVariable(name: "r_mavlink_status", arg: 5, scope: !546, file: !3, line: 586, type: !177)
!558 = !DILocation(line: 586, column: 68, scope: !546)
!559 = !DILocalVariable(name: "bufferIndex", scope: !546, file: !3, line: 588, type: !239)
!560 = !DILocation(line: 588, column: 6, scope: !546)
!561 = !DILocation(line: 590, column: 2, scope: !546)
!562 = !DILocation(line: 590, column: 10, scope: !546)
!563 = !DILocation(line: 590, column: 23, scope: !546)
!564 = !DILocation(line: 592, column: 10, scope: !546)
!565 = !DILocation(line: 592, column: 18, scope: !546)
!566 = !DILocation(line: 592, column: 2, scope: !546)
!567 = !DILocation(line: 596, column: 7, scope: !568)
!568 = distinct !DILexicalBlock(scope: !569, file: !3, line: 596, column: 7)
!569 = distinct !DILexicalBlock(scope: !546, file: !3, line: 593, column: 2)
!570 = !DILocation(line: 596, column: 9, scope: !568)
!571 = !DILocation(line: 596, column: 7, scope: !569)
!572 = !DILocation(line: 598, column: 4, scope: !573)
!573 = distinct !DILexicalBlock(scope: !568, file: !3, line: 597, column: 3)
!574 = !DILocation(line: 598, column: 12, scope: !573)
!575 = !DILocation(line: 598, column: 24, scope: !573)
!576 = !DILocation(line: 599, column: 4, scope: !573)
!577 = !DILocation(line: 599, column: 11, scope: !573)
!578 = !DILocation(line: 599, column: 15, scope: !573)
!579 = !DILocation(line: 600, column: 19, scope: !573)
!580 = !DILocation(line: 600, column: 4, scope: !573)
!581 = !DILocation(line: 600, column: 11, scope: !573)
!582 = !DILocation(line: 600, column: 17, scope: !573)
!583 = !DILocation(line: 601, column: 25, scope: !573)
!584 = !DILocation(line: 601, column: 33, scope: !573)
!585 = !DILocation(line: 601, column: 39, scope: !573)
!586 = !DILocation(line: 602, column: 27, scope: !573)
!587 = !DILocation(line: 602, column: 4, scope: !573)
!588 = !DILocation(line: 603, column: 3, scope: !573)
!589 = !DILocation(line: 603, column: 14, scope: !590)
!590 = distinct !DILexicalBlock(scope: !568, file: !3, line: 603, column: 14)
!591 = !DILocation(line: 603, column: 16, scope: !590)
!592 = !DILocation(line: 603, column: 14, scope: !568)
!593 = !DILocation(line: 605, column: 4, scope: !594)
!594 = distinct !DILexicalBlock(scope: !590, file: !3, line: 604, column: 3)
!595 = !DILocation(line: 605, column: 12, scope: !594)
!596 = !DILocation(line: 605, column: 24, scope: !594)
!597 = !DILocation(line: 606, column: 4, scope: !594)
!598 = !DILocation(line: 606, column: 11, scope: !594)
!599 = !DILocation(line: 606, column: 15, scope: !594)
!600 = !DILocation(line: 607, column: 19, scope: !594)
!601 = !DILocation(line: 607, column: 4, scope: !594)
!602 = !DILocation(line: 607, column: 11, scope: !594)
!603 = !DILocation(line: 607, column: 17, scope: !594)
!604 = !DILocation(line: 608, column: 25, scope: !594)
!605 = !DILocation(line: 608, column: 33, scope: !594)
!606 = !DILocation(line: 608, column: 39, scope: !594)
!607 = !DILocation(line: 609, column: 27, scope: !594)
!608 = !DILocation(line: 609, column: 4, scope: !594)
!609 = !DILocation(line: 610, column: 3, scope: !594)
!610 = !DILocation(line: 611, column: 3, scope: !569)
!611 = !DILocation(line: 614, column: 8, scope: !612)
!612 = distinct !DILexicalBlock(scope: !569, file: !3, line: 614, column: 8)
!613 = !DILocation(line: 614, column: 16, scope: !612)
!614 = !DILocation(line: 614, column: 8, scope: !569)
!615 = !DILocation(line: 622, column: 4, scope: !616)
!616 = distinct !DILexicalBlock(scope: !612, file: !3, line: 621, column: 3)
!617 = !DILocation(line: 622, column: 12, scope: !616)
!618 = !DILocation(line: 622, column: 26, scope: !616)
!619 = !DILocation(line: 623, column: 21, scope: !616)
!620 = !DILocation(line: 623, column: 4, scope: !616)
!621 = !DILocation(line: 624, column: 4, scope: !616)
!622 = !DILocation(line: 624, column: 12, scope: !616)
!623 = !DILocation(line: 624, column: 25, scope: !616)
!624 = !DILocation(line: 625, column: 4, scope: !616)
!625 = !DILocation(line: 625, column: 12, scope: !616)
!626 = !DILocation(line: 625, column: 24, scope: !616)
!627 = !DILocation(line: 626, column: 3, scope: !616)
!628 = !DILocation(line: 630, column: 17, scope: !629)
!629 = distinct !DILexicalBlock(scope: !612, file: !3, line: 628, column: 3)
!630 = !DILocation(line: 630, column: 4, scope: !629)
!631 = !DILocation(line: 630, column: 11, scope: !629)
!632 = !DILocation(line: 630, column: 15, scope: !629)
!633 = !DILocation(line: 631, column: 4, scope: !629)
!634 = !DILocation(line: 631, column: 12, scope: !629)
!635 = !DILocation(line: 631, column: 23, scope: !629)
!636 = !DILocation(line: 632, column: 28, scope: !629)
!637 = !DILocation(line: 632, column: 35, scope: !629)
!638 = !DILocation(line: 632, column: 4, scope: !629)
!639 = !DILocation(line: 633, column: 29, scope: !640)
!640 = distinct !DILexicalBlock(scope: !629, file: !3, line: 633, column: 29)
!641 = !DILocation(line: 633, column: 37, scope: !640)
!642 = !DILocation(line: 633, column: 43, scope: !640)
!643 = !DILocation(line: 633, column: 29, scope: !629)
!644 = !DILocation(line: 634, column: 29, scope: !645)
!645 = distinct !DILexicalBlock(scope: !640, file: !3, line: 633, column: 78)
!646 = !DILocation(line: 634, column: 36, scope: !645)
!647 = !DILocation(line: 634, column: 51, scope: !645)
!648 = !DILocation(line: 635, column: 29, scope: !645)
!649 = !DILocation(line: 635, column: 36, scope: !645)
!650 = !DILocation(line: 635, column: 49, scope: !645)
!651 = !DILocation(line: 636, column: 29, scope: !645)
!652 = !DILocation(line: 636, column: 37, scope: !645)
!653 = !DILocation(line: 636, column: 49, scope: !645)
!654 = !DILocation(line: 637, column: 25, scope: !645)
!655 = !DILocation(line: 638, column: 29, scope: !656)
!656 = distinct !DILexicalBlock(scope: !640, file: !3, line: 637, column: 32)
!657 = !DILocation(line: 638, column: 37, scope: !656)
!658 = !DILocation(line: 638, column: 49, scope: !656)
!659 = !DILocation(line: 641, column: 3, scope: !569)
!660 = !DILocation(line: 644, column: 27, scope: !569)
!661 = !DILocation(line: 644, column: 3, scope: !569)
!662 = !DILocation(line: 644, column: 10, scope: !569)
!663 = !DILocation(line: 644, column: 25, scope: !569)
!664 = !DILocation(line: 645, column: 8, scope: !665)
!665 = distinct !DILexicalBlock(scope: !569, file: !3, line: 645, column: 7)
!666 = !DILocation(line: 645, column: 15, scope: !665)
!667 = !DILocation(line: 645, column: 30, scope: !665)
!668 = !DILocation(line: 645, column: 53, scope: !665)
!669 = !DILocation(line: 645, column: 7, scope: !569)
!670 = !DILocation(line: 647, column: 21, scope: !671)
!671 = distinct !DILexicalBlock(scope: !665, file: !3, line: 645, column: 59)
!672 = !DILocation(line: 647, column: 4, scope: !671)
!673 = !DILocation(line: 648, column: 4, scope: !671)
!674 = !DILocation(line: 648, column: 12, scope: !671)
!675 = !DILocation(line: 648, column: 25, scope: !671)
!676 = !DILocation(line: 649, column: 4, scope: !671)
!677 = !DILocation(line: 649, column: 12, scope: !671)
!678 = !DILocation(line: 649, column: 24, scope: !671)
!679 = !DILocation(line: 650, column: 4, scope: !671)
!680 = !DILocation(line: 652, column: 27, scope: !569)
!681 = !DILocation(line: 652, column: 34, scope: !569)
!682 = !DILocation(line: 652, column: 3, scope: !569)
!683 = !DILocation(line: 653, column: 3, scope: !569)
!684 = !DILocation(line: 653, column: 11, scope: !569)
!685 = !DILocation(line: 653, column: 23, scope: !569)
!686 = !DILocation(line: 654, column: 3, scope: !569)
!687 = !DILocation(line: 657, column: 25, scope: !569)
!688 = !DILocation(line: 657, column: 3, scope: !569)
!689 = !DILocation(line: 657, column: 10, scope: !569)
!690 = !DILocation(line: 657, column: 23, scope: !569)
!691 = !DILocation(line: 658, column: 27, scope: !569)
!692 = !DILocation(line: 658, column: 34, scope: !569)
!693 = !DILocation(line: 658, column: 3, scope: !569)
!694 = !DILocation(line: 659, column: 3, scope: !569)
!695 = !DILocation(line: 659, column: 11, scope: !569)
!696 = !DILocation(line: 659, column: 23, scope: !569)
!697 = !DILocation(line: 660, column: 3, scope: !569)
!698 = !DILocation(line: 663, column: 16, scope: !569)
!699 = !DILocation(line: 663, column: 3, scope: !569)
!700 = !DILocation(line: 663, column: 10, scope: !569)
!701 = !DILocation(line: 663, column: 14, scope: !569)
!702 = !DILocation(line: 664, column: 27, scope: !569)
!703 = !DILocation(line: 664, column: 34, scope: !569)
!704 = !DILocation(line: 664, column: 3, scope: !569)
!705 = !DILocation(line: 665, column: 3, scope: !569)
!706 = !DILocation(line: 665, column: 11, scope: !569)
!707 = !DILocation(line: 665, column: 23, scope: !569)
!708 = !DILocation(line: 666, column: 3, scope: !569)
!709 = !DILocation(line: 669, column: 18, scope: !569)
!710 = !DILocation(line: 669, column: 3, scope: !569)
!711 = !DILocation(line: 669, column: 10, scope: !569)
!712 = !DILocation(line: 669, column: 16, scope: !569)
!713 = !DILocation(line: 670, column: 27, scope: !569)
!714 = !DILocation(line: 670, column: 34, scope: !569)
!715 = !DILocation(line: 670, column: 3, scope: !569)
!716 = !DILocation(line: 671, column: 3, scope: !569)
!717 = !DILocation(line: 671, column: 11, scope: !569)
!718 = !DILocation(line: 671, column: 23, scope: !569)
!719 = !DILocation(line: 672, column: 3, scope: !569)
!720 = !DILocation(line: 675, column: 19, scope: !569)
!721 = !DILocation(line: 675, column: 3, scope: !569)
!722 = !DILocation(line: 675, column: 10, scope: !569)
!723 = !DILocation(line: 675, column: 17, scope: !569)
!724 = !DILocation(line: 676, column: 27, scope: !569)
!725 = !DILocation(line: 676, column: 34, scope: !569)
!726 = !DILocation(line: 676, column: 3, scope: !569)
!727 = !DILocation(line: 677, column: 17, scope: !569)
!728 = !DILocation(line: 677, column: 25, scope: !569)
!729 = !DILocation(line: 677, column: 37, scope: !569)
!730 = !DILocation(line: 678, column: 3, scope: !569)
!731 = !DILocation(line: 681, column: 18, scope: !569)
!732 = !DILocation(line: 681, column: 3, scope: !569)
!733 = !DILocation(line: 681, column: 10, scope: !569)
!734 = !DILocation(line: 681, column: 16, scope: !569)
!735 = !DILocation(line: 682, column: 27, scope: !569)
!736 = !DILocation(line: 682, column: 34, scope: !569)
!737 = !DILocation(line: 682, column: 3, scope: !569)
!738 = !DILocation(line: 683, column: 7, scope: !739)
!739 = distinct !DILexicalBlock(scope: !569, file: !3, line: 683, column: 7)
!740 = !DILocation(line: 683, column: 15, scope: !739)
!741 = !DILocation(line: 683, column: 21, scope: !739)
!742 = !DILocation(line: 683, column: 7, scope: !569)
!743 = !DILocation(line: 684, column: 7, scope: !744)
!744 = distinct !DILexicalBlock(scope: !745, file: !3, line: 684, column: 7)
!745 = distinct !DILexicalBlock(scope: !739, file: !3, line: 683, column: 56)
!746 = !DILocation(line: 684, column: 14, scope: !744)
!747 = !DILocation(line: 684, column: 18, scope: !744)
!748 = !DILocation(line: 684, column: 7, scope: !745)
!749 = !DILocation(line: 685, column: 5, scope: !750)
!750 = distinct !DILexicalBlock(scope: !744, file: !3, line: 684, column: 23)
!751 = !DILocation(line: 685, column: 13, scope: !750)
!752 = !DILocation(line: 685, column: 25, scope: !750)
!753 = !DILocation(line: 686, column: 4, scope: !750)
!754 = !DILocation(line: 687, column: 5, scope: !755)
!755 = distinct !DILexicalBlock(scope: !744, file: !3, line: 686, column: 11)
!756 = !DILocation(line: 687, column: 13, scope: !755)
!757 = !DILocation(line: 687, column: 25, scope: !755)
!758 = !DILocation(line: 697, column: 3, scope: !745)
!759 = !DILocation(line: 698, column: 4, scope: !760)
!760 = distinct !DILexicalBlock(scope: !739, file: !3, line: 697, column: 10)
!761 = !DILocation(line: 698, column: 12, scope: !760)
!762 = !DILocation(line: 698, column: 24, scope: !760)
!763 = !DILocation(line: 700, column: 3, scope: !569)
!764 = !DILocation(line: 703, column: 19, scope: !569)
!765 = !DILocation(line: 703, column: 20, scope: !569)
!766 = !DILocation(line: 703, column: 3, scope: !569)
!767 = !DILocation(line: 703, column: 10, scope: !569)
!768 = !DILocation(line: 703, column: 16, scope: !569)
!769 = !DILocation(line: 704, column: 27, scope: !569)
!770 = !DILocation(line: 704, column: 34, scope: !569)
!771 = !DILocation(line: 704, column: 3, scope: !569)
!772 = !DILocation(line: 705, column: 3, scope: !569)
!773 = !DILocation(line: 705, column: 11, scope: !569)
!774 = !DILocation(line: 705, column: 23, scope: !569)
!775 = !DILocation(line: 706, column: 3, scope: !569)
!776 = !DILocation(line: 709, column: 30, scope: !569)
!777 = !DILocation(line: 709, column: 20, scope: !569)
!778 = !DILocation(line: 709, column: 32, scope: !569)
!779 = !DILocation(line: 709, column: 3, scope: !569)
!780 = !DILocation(line: 709, column: 10, scope: !569)
!781 = !DILocation(line: 709, column: 16, scope: !569)
!782 = !DILocation(line: 710, column: 27, scope: !569)
!783 = !DILocation(line: 710, column: 34, scope: !569)
!784 = !DILocation(line: 710, column: 3, scope: !569)
!785 = !DILocation(line: 711, column: 6, scope: !786)
!786 = distinct !DILexicalBlock(scope: !569, file: !3, line: 711, column: 6)
!787 = !DILocation(line: 711, column: 13, scope: !786)
!788 = !DILocation(line: 711, column: 17, scope: !786)
!789 = !DILocation(line: 711, column: 6, scope: !569)
!790 = !DILocation(line: 712, column: 4, scope: !791)
!791 = distinct !DILexicalBlock(scope: !786, file: !3, line: 711, column: 21)
!792 = !DILocation(line: 712, column: 12, scope: !791)
!793 = !DILocation(line: 712, column: 24, scope: !791)
!794 = !DILocation(line: 713, column: 3, scope: !791)
!795 = !DILocation(line: 714, column: 4, scope: !796)
!796 = distinct !DILexicalBlock(scope: !786, file: !3, line: 713, column: 10)
!797 = !DILocation(line: 714, column: 12, scope: !796)
!798 = !DILocation(line: 714, column: 24, scope: !796)
!799 = !DILocation(line: 725, column: 3, scope: !569)
!800 = !DILocation(line: 728, column: 63, scope: !569)
!801 = !DILocation(line: 728, column: 3, scope: !569)
!802 = !DILocation(line: 728, column: 33, scope: !569)
!803 = !DILocation(line: 728, column: 41, scope: !569)
!804 = !DILocation(line: 728, column: 51, scope: !569)
!805 = !DILocation(line: 728, column: 55, scope: !569)
!806 = !DILocation(line: 729, column: 27, scope: !569)
!807 = !DILocation(line: 729, column: 34, scope: !569)
!808 = !DILocation(line: 729, column: 3, scope: !569)
!809 = !DILocation(line: 730, column: 7, scope: !810)
!810 = distinct !DILexicalBlock(scope: !569, file: !3, line: 730, column: 7)
!811 = !DILocation(line: 730, column: 15, scope: !810)
!812 = !DILocation(line: 730, column: 29, scope: !810)
!813 = !DILocation(line: 730, column: 36, scope: !810)
!814 = !DILocation(line: 730, column: 26, scope: !810)
!815 = !DILocation(line: 730, column: 7, scope: !569)
!816 = !DILocation(line: 732, column: 4, scope: !817)
!817 = distinct !DILexicalBlock(scope: !810, file: !3, line: 731, column: 3)
!818 = !DILocation(line: 732, column: 12, scope: !817)
!819 = !DILocation(line: 732, column: 24, scope: !817)
!820 = !DILocation(line: 733, column: 3, scope: !817)
!821 = !DILocation(line: 734, column: 3, scope: !569)
!822 = !DILocalVariable(name: "e", scope: !823, file: !3, line: 737, type: !6)
!823 = distinct !DILexicalBlock(scope: !569, file: !3, line: 736, column: 40)
!824 = !DILocation(line: 737, column: 30, scope: !823)
!825 = !DILocation(line: 737, column: 56, scope: !823)
!826 = !DILocation(line: 737, column: 63, scope: !823)
!827 = !DILocation(line: 737, column: 34, scope: !823)
!828 = !DILocalVariable(name: "crc_extra", scope: !823, file: !3, line: 738, type: !19)
!829 = !DILocation(line: 738, column: 11, scope: !823)
!830 = !DILocation(line: 738, column: 23, scope: !823)
!831 = !DILocation(line: 738, column: 25, scope: !823)
!832 = !DILocation(line: 738, column: 28, scope: !823)
!833 = !DILocation(line: 739, column: 27, scope: !823)
!834 = !DILocation(line: 739, column: 34, scope: !823)
!835 = !DILocation(line: 739, column: 3, scope: !823)
!836 = !DILocation(line: 740, column: 7, scope: !837)
!837 = distinct !DILexicalBlock(scope: !823, file: !3, line: 740, column: 7)
!838 = !DILocation(line: 740, column: 13, scope: !837)
!839 = !DILocation(line: 740, column: 20, scope: !837)
!840 = !DILocation(line: 740, column: 29, scope: !837)
!841 = !DILocation(line: 740, column: 9, scope: !837)
!842 = !DILocation(line: 740, column: 7, scope: !823)
!843 = !DILocation(line: 741, column: 4, scope: !844)
!844 = distinct !DILexicalBlock(scope: !837, file: !3, line: 740, column: 38)
!845 = !DILocation(line: 741, column: 12, scope: !844)
!846 = !DILocation(line: 741, column: 24, scope: !844)
!847 = !DILocation(line: 742, column: 3, scope: !844)
!848 = !DILocation(line: 743, column: 4, scope: !849)
!849 = distinct !DILexicalBlock(scope: !837, file: !3, line: 742, column: 10)
!850 = !DILocation(line: 743, column: 12, scope: !849)
!851 = !DILocation(line: 743, column: 24, scope: !849)
!852 = !DILocation(line: 745, column: 32, scope: !823)
!853 = !DILocation(line: 745, column: 17, scope: !823)
!854 = !DILocation(line: 745, column: 24, scope: !823)
!855 = !DILocation(line: 745, column: 30, scope: !823)
!856 = !DILocation(line: 748, column: 21, scope: !857)
!857 = distinct !DILexicalBlock(scope: !823, file: !3, line: 748, column: 21)
!858 = !DILocation(line: 748, column: 23, scope: !857)
!859 = !DILocation(line: 748, column: 26, scope: !857)
!860 = !DILocation(line: 748, column: 34, scope: !857)
!861 = !DILocation(line: 748, column: 47, scope: !857)
!862 = !DILocation(line: 748, column: 50, scope: !857)
!863 = !DILocation(line: 748, column: 45, scope: !857)
!864 = !DILocation(line: 748, column: 21, scope: !823)
!865 = !DILocation(line: 749, column: 33, scope: !866)
!866 = distinct !DILexicalBlock(scope: !857, file: !3, line: 748, column: 63)
!867 = !DILocation(line: 749, column: 63, scope: !866)
!868 = !DILocation(line: 749, column: 71, scope: !866)
!869 = !DILocation(line: 749, column: 87, scope: !866)
!870 = !DILocation(line: 749, column: 90, scope: !866)
!871 = !DILocation(line: 749, column: 104, scope: !866)
!872 = !DILocation(line: 749, column: 112, scope: !866)
!873 = !DILocation(line: 749, column: 102, scope: !866)
!874 = !DILocation(line: 749, column: 25, scope: !866)
!875 = !DILocation(line: 750, column: 3, scope: !866)
!876 = !DILocation(line: 751, column: 3, scope: !823)
!877 = !DILocation(line: 756, column: 7, scope: !878)
!878 = distinct !DILexicalBlock(scope: !569, file: !3, line: 756, column: 7)
!879 = !DILocation(line: 756, column: 15, scope: !878)
!880 = !DILocation(line: 756, column: 27, scope: !878)
!881 = !DILocation(line: 756, column: 63, scope: !878)
!882 = !DILocation(line: 756, column: 66, scope: !878)
!883 = !DILocation(line: 756, column: 72, scope: !878)
!884 = !DILocation(line: 756, column: 79, scope: !878)
!885 = !DILocation(line: 756, column: 88, scope: !878)
!886 = !DILocation(line: 756, column: 68, scope: !878)
!887 = !DILocation(line: 756, column: 7, scope: !569)
!888 = !DILocation(line: 758, column: 4, scope: !889)
!889 = distinct !DILexicalBlock(scope: !878, file: !3, line: 756, column: 95)
!890 = !DILocation(line: 758, column: 12, scope: !889)
!891 = !DILocation(line: 758, column: 25, scope: !889)
!892 = !DILocation(line: 759, column: 3, scope: !889)
!893 = !DILocation(line: 761, column: 4, scope: !894)
!894 = distinct !DILexicalBlock(scope: !878, file: !3, line: 759, column: 10)
!895 = !DILocation(line: 761, column: 12, scope: !894)
!896 = !DILocation(line: 761, column: 25, scope: !894)
!897 = !DILocation(line: 763, column: 18, scope: !569)
!898 = !DILocation(line: 763, column: 3, scope: !569)
!899 = !DILocation(line: 763, column: 10, scope: !569)
!900 = !DILocation(line: 763, column: 16, scope: !569)
!901 = !DILocation(line: 765, column: 7, scope: !902)
!902 = distinct !DILexicalBlock(scope: !569, file: !3, line: 765, column: 7)
!903 = !DILocation(line: 765, column: 14, scope: !902)
!904 = !DILocation(line: 765, column: 29, scope: !902)
!905 = !DILocation(line: 765, column: 7, scope: !569)
!906 = !DILocation(line: 766, column: 4, scope: !907)
!907 = distinct !DILexicalBlock(scope: !902, file: !3, line: 765, column: 53)
!908 = !DILocation(line: 766, column: 12, scope: !907)
!909 = !DILocation(line: 766, column: 24, scope: !907)
!910 = !DILocation(line: 767, column: 4, scope: !907)
!911 = !DILocation(line: 767, column: 12, scope: !907)
!912 = !DILocation(line: 767, column: 27, scope: !907)
!913 = !DILocation(line: 771, column: 8, scope: !914)
!914 = distinct !DILexicalBlock(scope: !907, file: !3, line: 771, column: 8)
!915 = !DILocation(line: 771, column: 16, scope: !914)
!916 = !DILocation(line: 771, column: 29, scope: !914)
!917 = !DILocation(line: 771, column: 8, scope: !907)
!918 = !DILocation(line: 772, column: 5, scope: !919)
!919 = distinct !DILexicalBlock(scope: !914, file: !3, line: 771, column: 57)
!920 = !DILocation(line: 772, column: 13, scope: !919)
!921 = !DILocation(line: 772, column: 26, scope: !919)
!922 = !DILocation(line: 773, column: 4, scope: !919)
!923 = !DILocation(line: 774, column: 3, scope: !907)
!924 = !DILocation(line: 775, column: 8, scope: !925)
!925 = distinct !DILexicalBlock(scope: !926, file: !3, line: 775, column: 8)
!926 = distinct !DILexicalBlock(scope: !902, file: !3, line: 774, column: 10)
!927 = !DILocation(line: 775, column: 16, scope: !925)
!928 = !DILocation(line: 775, column: 24, scope: !925)
!929 = !DILocation(line: 776, column: 9, scope: !925)
!930 = !DILocation(line: 776, column: 17, scope: !925)
!931 = !DILocation(line: 776, column: 26, scope: !925)
!932 = !DILocation(line: 776, column: 51, scope: !925)
!933 = !DILocation(line: 776, column: 59, scope: !925)
!934 = !DILocation(line: 777, column: 10, scope: !925)
!935 = !DILocation(line: 777, column: 18, scope: !925)
!936 = !DILocation(line: 777, column: 27, scope: !925)
!937 = !DILocation(line: 777, column: 52, scope: !925)
!938 = !DILocation(line: 777, column: 60, scope: !925)
!939 = !DILocation(line: 777, column: 67, scope: !925)
!940 = !DILocation(line: 775, column: 8, scope: !926)
!941 = !DILocation(line: 780, column: 9, scope: !942)
!942 = distinct !DILexicalBlock(scope: !943, file: !3, line: 780, column: 9)
!943 = distinct !DILexicalBlock(scope: !925, file: !3, line: 777, column: 76)
!944 = !DILocation(line: 780, column: 17, scope: !942)
!945 = !DILocation(line: 780, column: 30, scope: !942)
!946 = !DILocation(line: 780, column: 9, scope: !943)
!947 = !DILocation(line: 781, column: 6, scope: !948)
!948 = distinct !DILexicalBlock(scope: !942, file: !3, line: 780, column: 58)
!949 = !DILocation(line: 781, column: 14, scope: !948)
!950 = !DILocation(line: 781, column: 27, scope: !948)
!951 = !DILocation(line: 782, column: 5, scope: !948)
!952 = !DILocation(line: 783, column: 4, scope: !943)
!953 = !DILocation(line: 784, column: 4, scope: !926)
!954 = !DILocation(line: 784, column: 12, scope: !926)
!955 = !DILocation(line: 784, column: 24, scope: !926)
!956 = !DILocation(line: 785, column: 8, scope: !957)
!957 = distinct !DILexicalBlock(scope: !926, file: !3, line: 785, column: 8)
!958 = !DILocation(line: 785, column: 18, scope: !957)
!959 = !DILocation(line: 785, column: 8, scope: !926)
!960 = !DILocation(line: 786, column: 12, scope: !961)
!961 = distinct !DILexicalBlock(scope: !957, file: !3, line: 785, column: 27)
!962 = !DILocation(line: 786, column: 5, scope: !961)
!963 = !DILocation(line: 786, column: 23, scope: !961)
!964 = !DILocation(line: 787, column: 4, scope: !961)
!965 = !DILocation(line: 789, column: 3, scope: !569)
!966 = !DILocation(line: 791, column: 74, scope: !569)
!967 = !DILocation(line: 791, column: 3, scope: !569)
!968 = !DILocation(line: 791, column: 10, scope: !569)
!969 = !DILocation(line: 791, column: 48, scope: !569)
!970 = !DILocation(line: 791, column: 56, scope: !569)
!971 = !DILocation(line: 791, column: 47, scope: !569)
!972 = !DILocation(line: 791, column: 72, scope: !569)
!973 = !DILocation(line: 792, column: 3, scope: !569)
!974 = !DILocation(line: 792, column: 11, scope: !569)
!975 = !DILocation(line: 792, column: 25, scope: !569)
!976 = !DILocation(line: 793, column: 7, scope: !977)
!977 = distinct !DILexicalBlock(scope: !569, file: !3, line: 793, column: 7)
!978 = !DILocation(line: 793, column: 15, scope: !977)
!979 = !DILocation(line: 793, column: 30, scope: !977)
!980 = !DILocation(line: 793, column: 7, scope: !569)
!981 = !DILocalVariable(name: "sig_ok", scope: !982, file: !3, line: 796, type: !209)
!982 = distinct !DILexicalBlock(scope: !977, file: !3, line: 793, column: 36)
!983 = !DILocation(line: 796, column: 9, scope: !982)
!984 = !DILocation(line: 796, column: 42, scope: !982)
!985 = !DILocation(line: 796, column: 50, scope: !982)
!986 = !DILocation(line: 796, column: 59, scope: !982)
!987 = !DILocation(line: 796, column: 67, scope: !982)
!988 = !DILocation(line: 796, column: 84, scope: !982)
!989 = !DILocation(line: 796, column: 18, scope: !982)
!990 = !DILocation(line: 800, column: 9, scope: !991)
!991 = distinct !DILexicalBlock(scope: !982, file: !3, line: 800, column: 8)
!992 = !DILocation(line: 800, column: 16, scope: !991)
!993 = !DILocation(line: 801, column: 9, scope: !991)
!994 = !DILocation(line: 801, column: 17, scope: !991)
!995 = !DILocation(line: 801, column: 26, scope: !991)
!996 = !DILocation(line: 801, column: 51, scope: !991)
!997 = !DILocation(line: 802, column: 9, scope: !991)
!998 = !DILocation(line: 802, column: 17, scope: !991)
!999 = !DILocation(line: 802, column: 26, scope: !991)
!1000 = !DILocation(line: 802, column: 51, scope: !991)
!1001 = !DILocation(line: 802, column: 59, scope: !991)
!1002 = !DILocation(line: 802, column: 66, scope: !991)
!1003 = !DILocation(line: 800, column: 8, scope: !982)
!1004 = !DILocation(line: 804, column: 12, scope: !1005)
!1005 = distinct !DILexicalBlock(scope: !991, file: !3, line: 802, column: 75)
!1006 = !DILocation(line: 805, column: 4, scope: !1005)
!1007 = !DILocation(line: 806, column: 8, scope: !1008)
!1008 = distinct !DILexicalBlock(scope: !982, file: !3, line: 806, column: 8)
!1009 = !DILocation(line: 806, column: 8, scope: !982)
!1010 = !DILocation(line: 807, column: 5, scope: !1011)
!1011 = distinct !DILexicalBlock(scope: !1008, file: !3, line: 806, column: 16)
!1012 = !DILocation(line: 807, column: 13, scope: !1011)
!1013 = !DILocation(line: 807, column: 26, scope: !1011)
!1014 = !DILocation(line: 808, column: 4, scope: !1011)
!1015 = !DILocation(line: 809, column: 5, scope: !1016)
!1016 = distinct !DILexicalBlock(scope: !1008, file: !3, line: 808, column: 11)
!1017 = !DILocation(line: 809, column: 13, scope: !1016)
!1018 = !DILocation(line: 809, column: 26, scope: !1016)
!1019 = !DILocation(line: 811, column: 4, scope: !982)
!1020 = !DILocation(line: 811, column: 12, scope: !982)
!1021 = !DILocation(line: 811, column: 24, scope: !982)
!1022 = !DILocation(line: 812, column: 8, scope: !1023)
!1023 = distinct !DILexicalBlock(scope: !982, file: !3, line: 812, column: 8)
!1024 = !DILocation(line: 812, column: 18, scope: !1023)
!1025 = !DILocation(line: 812, column: 8, scope: !982)
!1026 = !DILocation(line: 813, column: 12, scope: !1027)
!1027 = distinct !DILexicalBlock(scope: !1023, file: !3, line: 812, column: 26)
!1028 = !DILocation(line: 813, column: 5, scope: !1027)
!1029 = !DILocation(line: 813, column: 23, scope: !1027)
!1030 = !DILocation(line: 814, column: 4, scope: !1027)
!1031 = !DILocation(line: 815, column: 3, scope: !982)
!1032 = !DILocation(line: 816, column: 3, scope: !569)
!1033 = !DILocation(line: 819, column: 13, scope: !546)
!1034 = !DILocation(line: 821, column: 6, scope: !1035)
!1035 = distinct !DILexicalBlock(scope: !546, file: !3, line: 821, column: 6)
!1036 = !DILocation(line: 821, column: 14, scope: !1035)
!1037 = !DILocation(line: 821, column: 27, scope: !1035)
!1038 = !DILocation(line: 821, column: 6, scope: !546)
!1039 = !DILocation(line: 828, column: 28, scope: !1040)
!1040 = distinct !DILexicalBlock(scope: !1035, file: !3, line: 822, column: 2)
!1041 = !DILocation(line: 828, column: 35, scope: !1040)
!1042 = !DILocation(line: 828, column: 3, scope: !1040)
!1043 = !DILocation(line: 828, column: 11, scope: !1040)
!1044 = !DILocation(line: 828, column: 26, scope: !1040)
!1045 = !DILocation(line: 830, column: 7, scope: !1046)
!1046 = distinct !DILexicalBlock(scope: !1040, file: !3, line: 830, column: 7)
!1047 = !DILocation(line: 830, column: 15, scope: !1046)
!1048 = !DILocation(line: 830, column: 39, scope: !1046)
!1049 = !DILocation(line: 830, column: 7, scope: !1040)
!1050 = !DILocation(line: 830, column: 45, scope: !1046)
!1051 = !DILocation(line: 830, column: 53, scope: !1046)
!1052 = !DILocation(line: 830, column: 74, scope: !1046)
!1053 = !DILocation(line: 832, column: 3, scope: !1040)
!1054 = !DILocation(line: 832, column: 11, scope: !1040)
!1055 = !DILocation(line: 832, column: 34, scope: !1040)
!1056 = !DILocation(line: 833, column: 2, scope: !1040)
!1057 = !DILocation(line: 835, column: 12, scope: !1058)
!1058 = distinct !DILexicalBlock(scope: !546, file: !3, line: 835, column: 12)
!1059 = !DILocation(line: 835, column: 22, scope: !1058)
!1060 = !DILocation(line: 835, column: 12, scope: !546)
!1061 = !DILocation(line: 836, column: 29, scope: !1062)
!1062 = distinct !DILexicalBlock(scope: !1058, file: !3, line: 835, column: 31)
!1063 = !DILocation(line: 836, column: 36, scope: !1062)
!1064 = !DILocation(line: 836, column: 12, scope: !1062)
!1065 = !DILocation(line: 836, column: 23, scope: !1062)
!1066 = !DILocation(line: 836, column: 27, scope: !1062)
!1067 = !DILocation(line: 837, column: 8, scope: !1062)
!1068 = !DILocation(line: 838, column: 12, scope: !1069)
!1069 = distinct !DILexicalBlock(scope: !546, file: !3, line: 838, column: 12)
!1070 = !DILocation(line: 838, column: 29, scope: !1069)
!1071 = !DILocation(line: 838, column: 12, scope: !546)
!1072 = !DILocation(line: 839, column: 44, scope: !1073)
!1073 = distinct !DILexicalBlock(scope: !1069, file: !3, line: 838, column: 38)
!1074 = !DILocation(line: 839, column: 52, scope: !1073)
!1075 = !DILocation(line: 839, column: 12, scope: !1073)
!1076 = !DILocation(line: 839, column: 30, scope: !1073)
!1077 = !DILocation(line: 839, column: 42, scope: !1073)
!1078 = !DILocation(line: 840, column: 43, scope: !1073)
!1079 = !DILocation(line: 840, column: 51, scope: !1073)
!1080 = !DILocation(line: 840, column: 12, scope: !1073)
!1081 = !DILocation(line: 840, column: 30, scope: !1073)
!1082 = !DILocation(line: 840, column: 41, scope: !1073)
!1083 = !DILocation(line: 841, column: 47, scope: !1073)
!1084 = !DILocation(line: 841, column: 55, scope: !1073)
!1085 = !DILocation(line: 841, column: 69, scope: !1073)
!1086 = !DILocation(line: 841, column: 12, scope: !1073)
!1087 = !DILocation(line: 841, column: 30, scope: !1073)
!1088 = !DILocation(line: 841, column: 45, scope: !1073)
!1089 = !DILocation(line: 842, column: 56, scope: !1073)
!1090 = !DILocation(line: 842, column: 64, scope: !1073)
!1091 = !DILocation(line: 842, column: 12, scope: !1073)
!1092 = !DILocation(line: 842, column: 30, scope: !1073)
!1093 = !DILocation(line: 842, column: 54, scope: !1073)
!1094 = !DILocation(line: 843, column: 53, scope: !1073)
!1095 = !DILocation(line: 843, column: 61, scope: !1073)
!1096 = !DILocation(line: 843, column: 12, scope: !1073)
!1097 = !DILocation(line: 843, column: 30, scope: !1073)
!1098 = !DILocation(line: 843, column: 51, scope: !1073)
!1099 = !DILocation(line: 844, column: 38, scope: !1073)
!1100 = !DILocation(line: 844, column: 46, scope: !1073)
!1101 = !DILocation(line: 844, column: 12, scope: !1073)
!1102 = !DILocation(line: 844, column: 30, scope: !1073)
!1103 = !DILocation(line: 844, column: 36, scope: !1073)
!1104 = !DILocation(line: 845, column: 8, scope: !1073)
!1105 = !DILocation(line: 846, column: 8, scope: !546)
!1106 = !DILocation(line: 846, column: 16, scope: !546)
!1107 = !DILocation(line: 846, column: 28, scope: !546)
!1108 = !DILocation(line: 848, column: 6, scope: !1109)
!1109 = distinct !DILexicalBlock(scope: !546, file: !3, line: 848, column: 6)
!1110 = !DILocation(line: 848, column: 14, scope: !1109)
!1111 = !DILocation(line: 848, column: 27, scope: !1109)
!1112 = !DILocation(line: 848, column: 6, scope: !546)
!1113 = !DILocation(line: 856, column: 17, scope: !1114)
!1114 = distinct !DILexicalBlock(scope: !1115, file: !3, line: 856, column: 17)
!1115 = distinct !DILexicalBlock(scope: !1109, file: !3, line: 848, column: 55)
!1116 = !DILocation(line: 856, column: 27, scope: !1114)
!1117 = !DILocation(line: 856, column: 17, scope: !1115)
!1118 = !DILocation(line: 857, column: 39, scope: !1119)
!1119 = distinct !DILexicalBlock(scope: !1114, file: !3, line: 856, column: 36)
!1120 = !DILocation(line: 857, column: 46, scope: !1119)
!1121 = !DILocation(line: 857, column: 55, scope: !1119)
!1122 = !DILocation(line: 857, column: 62, scope: !1119)
!1123 = !DILocation(line: 857, column: 67, scope: !1119)
!1124 = !DILocation(line: 857, column: 52, scope: !1119)
!1125 = !DILocation(line: 857, column: 17, scope: !1119)
!1126 = !DILocation(line: 857, column: 28, scope: !1119)
!1127 = !DILocation(line: 857, column: 37, scope: !1119)
!1128 = !DILocation(line: 858, column: 13, scope: !1119)
!1129 = !DILocation(line: 859, column: 2, scope: !1115)
!1130 = !DILocation(line: 861, column: 9, scope: !546)
!1131 = !DILocation(line: 861, column: 17, scope: !546)
!1132 = !DILocation(line: 861, column: 2, scope: !546)
!1133 = distinct !DISubprogram(name: "mavlink_update_checksum", scope: !3, file: !3, line: 499, type: !1134, scopeLine: 500, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!1134 = !DISubroutineType(types: !1135)
!1135 = !{null, !138, !19}
!1136 = !DILocalVariable(name: "msg", arg: 1, scope: !1133, file: !3, line: 499, type: !138)
!1137 = !DILocation(line: 499, column: 64, scope: !1133)
!1138 = !DILocalVariable(name: "c", arg: 2, scope: !1133, file: !3, line: 499, type: !19)
!1139 = !DILocation(line: 499, column: 77, scope: !1133)
!1140 = !DILocalVariable(name: "checksum", scope: !1133, file: !3, line: 501, type: !143)
!1141 = !DILocation(line: 501, column: 11, scope: !1133)
!1142 = !DILocation(line: 501, column: 22, scope: !1133)
!1143 = !DILocation(line: 501, column: 27, scope: !1133)
!1144 = !DILocation(line: 502, column: 17, scope: !1133)
!1145 = !DILocation(line: 502, column: 2, scope: !1133)
!1146 = !DILocation(line: 503, column: 18, scope: !1133)
!1147 = !DILocation(line: 503, column: 2, scope: !1133)
!1148 = !DILocation(line: 503, column: 7, scope: !1133)
!1149 = !DILocation(line: 503, column: 16, scope: !1133)
!1150 = !DILocation(line: 504, column: 1, scope: !1133)
!1151 = !DILocalVariable(name: "msgid", arg: 1, scope: !2, file: !3, line: 510, type: !13)
!1152 = !DILocation(line: 510, column: 74, scope: !2)
!1153 = !DILocalVariable(name: "low", scope: !2, file: !3, line: 517, type: !13)
!1154 = !DILocation(line: 517, column: 18, scope: !2)
!1155 = !DILocalVariable(name: "high", scope: !2, file: !3, line: 517, type: !13)
!1156 = !DILocation(line: 517, column: 25, scope: !2)
!1157 = !DILocation(line: 518, column: 9, scope: !2)
!1158 = !DILocation(line: 518, column: 16, scope: !2)
!1159 = !DILocation(line: 518, column: 22, scope: !2)
!1160 = !DILocation(line: 518, column: 20, scope: !2)
!1161 = !DILocalVariable(name: "mid", scope: !1162, file: !3, line: 519, type: !13)
!1162 = distinct !DILexicalBlock(scope: !2, file: !3, line: 518, column: 28)
!1163 = !DILocation(line: 519, column: 22, scope: !1162)
!1164 = !DILocation(line: 519, column: 29, scope: !1162)
!1165 = !DILocation(line: 519, column: 32, scope: !1162)
!1166 = !DILocation(line: 519, column: 35, scope: !1162)
!1167 = !DILocation(line: 519, column: 34, scope: !1162)
!1168 = !DILocation(line: 519, column: 40, scope: !1162)
!1169 = !DILocation(line: 520, column: 17, scope: !1170)
!1170 = distinct !DILexicalBlock(scope: !1162, file: !3, line: 520, column: 17)
!1171 = !DILocation(line: 520, column: 46, scope: !1170)
!1172 = !DILocation(line: 520, column: 25, scope: !1170)
!1173 = !DILocation(line: 520, column: 51, scope: !1170)
!1174 = !DILocation(line: 520, column: 23, scope: !1170)
!1175 = !DILocation(line: 520, column: 17, scope: !1162)
!1176 = !DILocation(line: 521, column: 24, scope: !1177)
!1177 = distinct !DILexicalBlock(scope: !1170, file: !3, line: 520, column: 58)
!1178 = !DILocation(line: 521, column: 27, scope: !1177)
!1179 = !DILocation(line: 521, column: 22, scope: !1177)
!1180 = !DILocation(line: 522, column: 17, scope: !1177)
!1181 = distinct !{!1181, !1157, !1182}
!1182 = !DILocation(line: 530, column: 9, scope: !2)
!1183 = !DILocation(line: 524, column: 17, scope: !1184)
!1184 = distinct !DILexicalBlock(scope: !1162, file: !3, line: 524, column: 17)
!1185 = !DILocation(line: 524, column: 46, scope: !1184)
!1186 = !DILocation(line: 524, column: 25, scope: !1184)
!1187 = !DILocation(line: 524, column: 51, scope: !1184)
!1188 = !DILocation(line: 524, column: 23, scope: !1184)
!1189 = !DILocation(line: 524, column: 17, scope: !1162)
!1190 = !DILocation(line: 525, column: 23, scope: !1191)
!1191 = distinct !DILexicalBlock(scope: !1184, file: !3, line: 524, column: 58)
!1192 = !DILocation(line: 525, column: 21, scope: !1191)
!1193 = !DILocation(line: 526, column: 17, scope: !1191)
!1194 = !DILocation(line: 528, column: 19, scope: !1162)
!1195 = !DILocation(line: 528, column: 17, scope: !1162)
!1196 = !DILocation(line: 529, column: 13, scope: !1162)
!1197 = !DILocation(line: 531, column: 34, scope: !1198)
!1198 = distinct !DILexicalBlock(scope: !2, file: !3, line: 531, column: 13)
!1199 = !DILocation(line: 531, column: 13, scope: !1198)
!1200 = !DILocation(line: 531, column: 39, scope: !1198)
!1201 = !DILocation(line: 531, column: 48, scope: !1198)
!1202 = !DILocation(line: 531, column: 45, scope: !1198)
!1203 = !DILocation(line: 531, column: 13, scope: !2)
!1204 = !DILocation(line: 533, column: 13, scope: !1205)
!1205 = distinct !DILexicalBlock(scope: !1198, file: !3, line: 531, column: 55)
!1206 = !DILocation(line: 535, column: 38, scope: !2)
!1207 = !DILocation(line: 535, column: 17, scope: !2)
!1208 = !DILocation(line: 535, column: 9, scope: !2)
!1209 = !DILocation(line: 536, column: 1, scope: !2)
!1210 = distinct !DISubprogram(name: "mavlink_signature_check", scope: !3, file: !3, line: 123, type: !1211, scopeLine: 126, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!1211 = !DISubroutineType(types: !1212)
!1212 = !{!209, !1213, !1215, !423}
!1213 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1214, size: 64)
!1214 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_signing_t", file: !9, line: 254, baseType: !195)
!1215 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1216, size: 64)
!1216 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_signing_streams_t", file: !9, line: 271, baseType: !214)
!1217 = !DILocalVariable(name: "signing", arg: 1, scope: !1210, file: !3, line: 123, type: !1213)
!1218 = !DILocation(line: 123, column: 64, scope: !1210)
!1219 = !DILocalVariable(name: "signing_streams", arg: 2, scope: !1210, file: !3, line: 124, type: !1215)
!1220 = !DILocation(line: 124, column: 37, scope: !1210)
!1221 = !DILocalVariable(name: "msg", arg: 3, scope: !1210, file: !3, line: 125, type: !423)
!1222 = !DILocation(line: 125, column: 35, scope: !1210)
!1223 = !DILocation(line: 127, column: 6, scope: !1224)
!1224 = distinct !DILexicalBlock(scope: !1210, file: !3, line: 127, column: 6)
!1225 = !DILocation(line: 127, column: 14, scope: !1224)
!1226 = !DILocation(line: 127, column: 6, scope: !1210)
!1227 = !DILocation(line: 128, column: 3, scope: !1228)
!1228 = distinct !DILexicalBlock(scope: !1224, file: !3, line: 127, column: 23)
!1229 = !DILocalVariable(name: "p", scope: !1210, file: !3, line: 130, type: !118)
!1230 = !DILocation(line: 130, column: 24, scope: !1210)
!1231 = !DILocation(line: 130, column: 46, scope: !1210)
!1232 = !DILocation(line: 130, column: 51, scope: !1210)
!1233 = !DILocalVariable(name: "psig", scope: !1210, file: !3, line: 131, type: !118)
!1234 = !DILocation(line: 131, column: 17, scope: !1210)
!1235 = !DILocation(line: 131, column: 24, scope: !1210)
!1236 = !DILocation(line: 131, column: 29, scope: !1210)
!1237 = !DILocalVariable(name: "incoming_signature", scope: !1210, file: !3, line: 132, type: !118)
!1238 = !DILocation(line: 132, column: 24, scope: !1210)
!1239 = !DILocation(line: 132, column: 45, scope: !1210)
!1240 = !DILocation(line: 132, column: 49, scope: !1210)
!1241 = !DILocalVariable(name: "ctx", scope: !1210, file: !3, line: 133, type: !1242)
!1242 = !DIDerivedType(tag: DW_TAG_typedef, name: "mavlink_sha256_ctx", file: !128, line: 60, baseType: !1243)
!1243 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !128, line: 53, size: 832, elements: !1244)
!1244 = !{!1245, !1247, !1249}
!1245 = !DIDerivedType(tag: DW_TAG_member, name: "sz", scope: !1243, file: !128, line: 54, baseType: !1246, size: 64)
!1246 = !DICompositeType(tag: DW_TAG_array_type, baseType: !13, size: 64, elements: !162)
!1247 = !DIDerivedType(tag: DW_TAG_member, name: "counter", scope: !1243, file: !128, line: 55, baseType: !1248, size: 256, offset: 64)
!1248 = !DICompositeType(tag: DW_TAG_array_type, baseType: !13, size: 256, elements: !259)
!1249 = !DIDerivedType(tag: DW_TAG_member, name: "u", scope: !1243, file: !128, line: 59, baseType: !1250, size: 512, offset: 320)
!1250 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !1243, file: !128, line: 56, size: 512, elements: !1251)
!1251 = !{!1252, !1254}
!1252 = !DIDerivedType(tag: DW_TAG_member, name: "save_bytes", scope: !1250, file: !128, line: 57, baseType: !1253, size: 512)
!1253 = !DICompositeType(tag: DW_TAG_array_type, baseType: !21, size: 512, elements: !131)
!1254 = !DIDerivedType(tag: DW_TAG_member, name: "save_u32", scope: !1250, file: !128, line: 58, baseType: !1255, size: 512)
!1255 = !DICompositeType(tag: DW_TAG_array_type, baseType: !13, size: 512, elements: !170)
!1256 = !DILocation(line: 133, column: 21, scope: !1210)
!1257 = !DILocalVariable(name: "signature", scope: !1210, file: !3, line: 134, type: !225)
!1258 = !DILocation(line: 134, column: 10, scope: !1210)
!1259 = !DILocalVariable(name: "i", scope: !1210, file: !3, line: 135, type: !143)
!1260 = !DILocation(line: 135, column: 11, scope: !1210)
!1261 = !DILocation(line: 137, column: 2, scope: !1210)
!1262 = !DILocation(line: 138, column: 30, scope: !1210)
!1263 = !DILocation(line: 138, column: 39, scope: !1210)
!1264 = !DILocation(line: 138, column: 2, scope: !1210)
!1265 = !DILocation(line: 139, column: 30, scope: !1210)
!1266 = !DILocation(line: 139, column: 2, scope: !1210)
!1267 = !DILocation(line: 140, column: 30, scope: !1210)
!1268 = !DILocation(line: 140, column: 49, scope: !1210)
!1269 = !DILocation(line: 140, column: 54, scope: !1210)
!1270 = !DILocation(line: 140, column: 2, scope: !1210)
!1271 = !DILocation(line: 141, column: 30, scope: !1210)
!1272 = !DILocation(line: 141, column: 35, scope: !1210)
!1273 = !DILocation(line: 141, column: 2, scope: !1210)
!1274 = !DILocation(line: 142, column: 30, scope: !1210)
!1275 = !DILocation(line: 142, column: 2, scope: !1210)
!1276 = !DILocation(line: 143, column: 32, scope: !1210)
!1277 = !DILocation(line: 143, column: 2, scope: !1210)
!1278 = !DILocation(line: 144, column: 13, scope: !1279)
!1279 = distinct !DILexicalBlock(scope: !1210, file: !3, line: 144, column: 6)
!1280 = !DILocation(line: 144, column: 24, scope: !1279)
!1281 = !DILocation(line: 144, column: 6, scope: !1279)
!1282 = !DILocation(line: 144, column: 47, scope: !1279)
!1283 = !DILocation(line: 144, column: 6, scope: !1210)
!1284 = !DILocation(line: 145, column: 3, scope: !1285)
!1285 = distinct !DILexicalBlock(scope: !1279, file: !3, line: 144, column: 53)
!1286 = !DILocalVariable(name: "tstamp", scope: !1210, file: !3, line: 152, type: !1287)
!1287 = distinct !DICompositeType(tag: DW_TAG_union_type, name: "tstamp", scope: !1210, file: !3, line: 149, size: 64, elements: !1288)
!1288 = !{!1289, !1290}
!1289 = !DIDerivedType(tag: DW_TAG_member, name: "t64", scope: !1287, file: !3, line: 150, baseType: !155, size: 64)
!1290 = !DIDerivedType(tag: DW_TAG_member, name: "t8", scope: !1287, file: !3, line: 151, baseType: !1291, size: 64)
!1291 = !DICompositeType(tag: DW_TAG_array_type, baseType: !19, size: 64, elements: !259)
!1292 = !DILocation(line: 152, column: 4, scope: !1210)
!1293 = !DILocalVariable(name: "link_id", scope: !1210, file: !3, line: 153, type: !19)
!1294 = !DILocation(line: 153, column: 10, scope: !1210)
!1295 = !DILocation(line: 153, column: 20, scope: !1210)
!1296 = !DILocation(line: 154, column: 9, scope: !1210)
!1297 = !DILocation(line: 154, column: 13, scope: !1210)
!1298 = !DILocation(line: 155, column: 16, scope: !1210)
!1299 = !DILocation(line: 155, column: 2, scope: !1210)
!1300 = !DILocation(line: 155, column: 20, scope: !1210)
!1301 = !DILocation(line: 155, column: 24, scope: !1210)
!1302 = !DILocation(line: 157, column: 6, scope: !1303)
!1303 = distinct !DILexicalBlock(scope: !1210, file: !3, line: 157, column: 6)
!1304 = !DILocation(line: 157, column: 22, scope: !1303)
!1305 = !DILocation(line: 157, column: 6, scope: !1210)
!1306 = !DILocation(line: 158, column: 3, scope: !1307)
!1307 = distinct !DILexicalBlock(scope: !1303, file: !3, line: 157, column: 31)
!1308 = !DILocation(line: 162, column: 8, scope: !1309)
!1309 = distinct !DILexicalBlock(scope: !1210, file: !3, line: 162, column: 2)
!1310 = !DILocation(line: 162, column: 7, scope: !1309)
!1311 = !DILocation(line: 162, column: 12, scope: !1312)
!1312 = distinct !DILexicalBlock(scope: !1309, file: !3, line: 162, column: 2)
!1313 = !DILocation(line: 162, column: 14, scope: !1312)
!1314 = !DILocation(line: 162, column: 31, scope: !1312)
!1315 = !DILocation(line: 162, column: 13, scope: !1312)
!1316 = !DILocation(line: 162, column: 2, scope: !1309)
!1317 = !DILocation(line: 163, column: 7, scope: !1318)
!1318 = distinct !DILexicalBlock(scope: !1319, file: !3, line: 163, column: 7)
!1319 = distinct !DILexicalBlock(scope: !1312, file: !3, line: 162, column: 57)
!1320 = !DILocation(line: 163, column: 12, scope: !1318)
!1321 = !DILocation(line: 163, column: 21, scope: !1318)
!1322 = !DILocation(line: 163, column: 38, scope: !1318)
!1323 = !DILocation(line: 163, column: 45, scope: !1318)
!1324 = !DILocation(line: 163, column: 48, scope: !1318)
!1325 = !DILocation(line: 163, column: 18, scope: !1318)
!1326 = !DILocation(line: 163, column: 54, scope: !1318)
!1327 = !DILocation(line: 164, column: 7, scope: !1318)
!1328 = !DILocation(line: 164, column: 12, scope: !1318)
!1329 = !DILocation(line: 164, column: 22, scope: !1318)
!1330 = !DILocation(line: 164, column: 39, scope: !1318)
!1331 = !DILocation(line: 164, column: 46, scope: !1318)
!1332 = !DILocation(line: 164, column: 49, scope: !1318)
!1333 = !DILocation(line: 164, column: 19, scope: !1318)
!1334 = !DILocation(line: 164, column: 56, scope: !1318)
!1335 = !DILocation(line: 165, column: 7, scope: !1318)
!1336 = !DILocation(line: 165, column: 18, scope: !1318)
!1337 = !DILocation(line: 165, column: 35, scope: !1318)
!1338 = !DILocation(line: 165, column: 42, scope: !1318)
!1339 = !DILocation(line: 165, column: 45, scope: !1318)
!1340 = !DILocation(line: 165, column: 15, scope: !1318)
!1341 = !DILocation(line: 163, column: 7, scope: !1319)
!1342 = !DILocation(line: 166, column: 4, scope: !1343)
!1343 = distinct !DILexicalBlock(scope: !1318, file: !3, line: 165, column: 54)
!1344 = !DILocation(line: 168, column: 2, scope: !1319)
!1345 = !DILocation(line: 162, column: 53, scope: !1312)
!1346 = !DILocation(line: 162, column: 2, scope: !1312)
!1347 = distinct !{!1347, !1316, !1348}
!1348 = !DILocation(line: 168, column: 2, scope: !1309)
!1349 = !DILocation(line: 169, column: 6, scope: !1350)
!1350 = distinct !DILexicalBlock(scope: !1210, file: !3, line: 169, column: 6)
!1351 = !DILocation(line: 169, column: 11, scope: !1350)
!1352 = !DILocation(line: 169, column: 28, scope: !1350)
!1353 = !DILocation(line: 169, column: 8, scope: !1350)
!1354 = !DILocation(line: 169, column: 6, scope: !1210)
!1355 = !DILocation(line: 170, column: 7, scope: !1356)
!1356 = distinct !DILexicalBlock(scope: !1357, file: !3, line: 170, column: 7)
!1357 = distinct !DILexicalBlock(scope: !1350, file: !3, line: 169, column: 49)
!1358 = !DILocation(line: 170, column: 24, scope: !1356)
!1359 = !DILocation(line: 170, column: 44, scope: !1356)
!1360 = !DILocation(line: 170, column: 7, scope: !1357)
!1361 = !DILocation(line: 172, column: 4, scope: !1362)
!1362 = distinct !DILexicalBlock(scope: !1356, file: !3, line: 170, column: 76)
!1363 = !DILocation(line: 175, column: 14, scope: !1364)
!1364 = distinct !DILexicalBlock(scope: !1357, file: !3, line: 175, column: 7)
!1365 = !DILocation(line: 175, column: 18, scope: !1364)
!1366 = !DILocation(line: 175, column: 34, scope: !1364)
!1367 = !DILocation(line: 175, column: 43, scope: !1364)
!1368 = !DILocation(line: 175, column: 32, scope: !1364)
!1369 = !DILocation(line: 175, column: 7, scope: !1357)
!1370 = !DILocation(line: 176, column: 4, scope: !1371)
!1371 = distinct !DILexicalBlock(scope: !1364, file: !3, line: 175, column: 54)
!1372 = !DILocation(line: 179, column: 38, scope: !1357)
!1373 = !DILocation(line: 179, column: 43, scope: !1357)
!1374 = !DILocation(line: 179, column: 3, scope: !1357)
!1375 = !DILocation(line: 179, column: 20, scope: !1357)
!1376 = !DILocation(line: 179, column: 27, scope: !1357)
!1377 = !DILocation(line: 179, column: 30, scope: !1357)
!1378 = !DILocation(line: 179, column: 36, scope: !1357)
!1379 = !DILocation(line: 180, column: 39, scope: !1357)
!1380 = !DILocation(line: 180, column: 44, scope: !1357)
!1381 = !DILocation(line: 180, column: 3, scope: !1357)
!1382 = !DILocation(line: 180, column: 20, scope: !1357)
!1383 = !DILocation(line: 180, column: 27, scope: !1357)
!1384 = !DILocation(line: 180, column: 30, scope: !1357)
!1385 = !DILocation(line: 180, column: 37, scope: !1357)
!1386 = !DILocation(line: 181, column: 40, scope: !1357)
!1387 = !DILocation(line: 181, column: 3, scope: !1357)
!1388 = !DILocation(line: 181, column: 20, scope: !1357)
!1389 = !DILocation(line: 181, column: 27, scope: !1357)
!1390 = !DILocation(line: 181, column: 30, scope: !1357)
!1391 = !DILocation(line: 181, column: 38, scope: !1357)
!1392 = !DILocation(line: 182, column: 3, scope: !1357)
!1393 = !DILocation(line: 182, column: 20, scope: !1357)
!1394 = !DILocation(line: 182, column: 39, scope: !1357)
!1395 = !DILocation(line: 183, column: 2, scope: !1357)
!1396 = !DILocalVariable(name: "last_tstamp", scope: !1397, file: !3, line: 184, type: !1287)
!1397 = distinct !DILexicalBlock(scope: !1350, file: !3, line: 183, column: 9)
!1398 = !DILocation(line: 184, column: 16, scope: !1397)
!1399 = !DILocation(line: 185, column: 15, scope: !1397)
!1400 = !DILocation(line: 185, column: 19, scope: !1397)
!1401 = !DILocation(line: 186, column: 22, scope: !1397)
!1402 = !DILocation(line: 186, column: 3, scope: !1397)
!1403 = !DILocation(line: 186, column: 26, scope: !1397)
!1404 = !DILocation(line: 186, column: 43, scope: !1397)
!1405 = !DILocation(line: 186, column: 50, scope: !1397)
!1406 = !DILocation(line: 186, column: 53, scope: !1397)
!1407 = !DILocation(line: 187, column: 14, scope: !1408)
!1408 = distinct !DILexicalBlock(scope: !1397, file: !3, line: 187, column: 7)
!1409 = !DILocation(line: 187, column: 33, scope: !1408)
!1410 = !DILocation(line: 187, column: 18, scope: !1408)
!1411 = !DILocation(line: 187, column: 7, scope: !1397)
!1412 = !DILocation(line: 189, column: 4, scope: !1413)
!1413 = distinct !DILexicalBlock(scope: !1408, file: !3, line: 187, column: 38)
!1414 = !DILocation(line: 194, column: 9, scope: !1210)
!1415 = !DILocation(line: 194, column: 26, scope: !1210)
!1416 = !DILocation(line: 194, column: 33, scope: !1210)
!1417 = !DILocation(line: 194, column: 36, scope: !1210)
!1418 = !DILocation(line: 194, column: 2, scope: !1210)
!1419 = !DILocation(line: 194, column: 53, scope: !1210)
!1420 = !DILocation(line: 194, column: 57, scope: !1210)
!1421 = !DILocation(line: 197, column: 13, scope: !1422)
!1422 = distinct !DILexicalBlock(scope: !1210, file: !3, line: 197, column: 6)
!1423 = !DILocation(line: 197, column: 19, scope: !1422)
!1424 = !DILocation(line: 197, column: 28, scope: !1422)
!1425 = !DILocation(line: 197, column: 17, scope: !1422)
!1426 = !DILocation(line: 197, column: 6, scope: !1210)
!1427 = !DILocation(line: 198, column: 31, scope: !1428)
!1428 = distinct !DILexicalBlock(scope: !1422, file: !3, line: 197, column: 39)
!1429 = !DILocation(line: 198, column: 3, scope: !1428)
!1430 = !DILocation(line: 198, column: 12, scope: !1428)
!1431 = !DILocation(line: 198, column: 22, scope: !1428)
!1432 = !DILocation(line: 199, column: 2, scope: !1428)
!1433 = !DILocation(line: 200, column: 2, scope: !1210)
!1434 = !DILocation(line: 201, column: 1, scope: !1210)
!1435 = distinct !DISubprogram(name: "crc_accumulate", scope: !1436, file: !1436, line: 35, type: !1437, scopeLine: 36, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!1436 = !DIFile(filename: "generated/include/mavlink/v2.0/common/../checksum.h", directory: "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/mavlink-server/input/source_code/mavlink")
!1437 = !DISubroutineType(types: !1438)
!1438 = !{null, !19, !1439}
!1439 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !143, size: 64)
!1440 = !DILocalVariable(name: "data", arg: 1, scope: !1435, file: !1436, line: 35, type: !19)
!1441 = !DILocation(line: 35, column: 43, scope: !1435)
!1442 = !DILocalVariable(name: "crcAccum", arg: 2, scope: !1435, file: !1436, line: 35, type: !1439)
!1443 = !DILocation(line: 35, column: 59, scope: !1435)
!1444 = !DILocalVariable(name: "tmp", scope: !1435, file: !1436, line: 38, type: !19)
!1445 = !DILocation(line: 38, column: 17, scope: !1435)
!1446 = !DILocation(line: 40, column: 15, scope: !1435)
!1447 = !DILocation(line: 40, column: 33, scope: !1435)
!1448 = !DILocation(line: 40, column: 32, scope: !1435)
!1449 = !DILocation(line: 40, column: 42, scope: !1435)
!1450 = !DILocation(line: 40, column: 22, scope: !1435)
!1451 = !DILocation(line: 40, column: 20, scope: !1435)
!1452 = !DILocation(line: 40, column: 13, scope: !1435)
!1453 = !DILocation(line: 41, column: 17, scope: !1435)
!1454 = !DILocation(line: 41, column: 20, scope: !1435)
!1455 = !DILocation(line: 41, column: 13, scope: !1435)
!1456 = !DILocation(line: 42, column: 23, scope: !1435)
!1457 = !DILocation(line: 42, column: 22, scope: !1435)
!1458 = !DILocation(line: 42, column: 31, scope: !1435)
!1459 = !DILocation(line: 42, column: 39, scope: !1435)
!1460 = !DILocation(line: 42, column: 42, scope: !1435)
!1461 = !DILocation(line: 42, column: 36, scope: !1435)
!1462 = !DILocation(line: 42, column: 50, scope: !1435)
!1463 = !DILocation(line: 42, column: 54, scope: !1435)
!1464 = !DILocation(line: 42, column: 47, scope: !1435)
!1465 = !DILocation(line: 42, column: 62, scope: !1435)
!1466 = !DILocation(line: 42, column: 65, scope: !1435)
!1467 = !DILocation(line: 42, column: 59, scope: !1435)
!1468 = !DILocation(line: 42, column: 21, scope: !1435)
!1469 = !DILocation(line: 42, column: 10, scope: !1435)
!1470 = !DILocation(line: 42, column: 19, scope: !1435)
!1471 = !DILocation(line: 43, column: 1, scope: !1435)
!1472 = distinct !DISubprogram(name: "mavlink_sha256_init", scope: !128, file: !128, line: 91, type: !1473, scopeLine: 92, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!1473 = !DISubroutineType(types: !1474)
!1474 = !{null, !1475}
!1475 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1242, size: 64)
!1476 = !DILocalVariable(name: "m", arg: 1, scope: !1472, file: !128, line: 91, type: !1475)
!1477 = !DILocation(line: 91, column: 61, scope: !1472)
!1478 = !DILocation(line: 93, column: 5, scope: !1472)
!1479 = !DILocation(line: 93, column: 8, scope: !1472)
!1480 = !DILocation(line: 93, column: 14, scope: !1472)
!1481 = !DILocation(line: 94, column: 5, scope: !1472)
!1482 = !DILocation(line: 94, column: 8, scope: !1472)
!1483 = !DILocation(line: 94, column: 14, scope: !1472)
!1484 = !DILocation(line: 95, column: 5, scope: !1472)
!1485 = !DILocation(line: 95, column: 8, scope: !1472)
!1486 = !DILocation(line: 95, column: 19, scope: !1472)
!1487 = !DILocation(line: 96, column: 5, scope: !1472)
!1488 = !DILocation(line: 96, column: 8, scope: !1472)
!1489 = !DILocation(line: 96, column: 19, scope: !1472)
!1490 = !DILocation(line: 97, column: 5, scope: !1472)
!1491 = !DILocation(line: 97, column: 8, scope: !1472)
!1492 = !DILocation(line: 97, column: 19, scope: !1472)
!1493 = !DILocation(line: 98, column: 5, scope: !1472)
!1494 = !DILocation(line: 98, column: 8, scope: !1472)
!1495 = !DILocation(line: 98, column: 19, scope: !1472)
!1496 = !DILocation(line: 99, column: 5, scope: !1472)
!1497 = !DILocation(line: 99, column: 8, scope: !1472)
!1498 = !DILocation(line: 99, column: 19, scope: !1472)
!1499 = !DILocation(line: 100, column: 5, scope: !1472)
!1500 = !DILocation(line: 100, column: 8, scope: !1472)
!1501 = !DILocation(line: 100, column: 19, scope: !1472)
!1502 = !DILocation(line: 101, column: 5, scope: !1472)
!1503 = !DILocation(line: 101, column: 8, scope: !1472)
!1504 = !DILocation(line: 101, column: 19, scope: !1472)
!1505 = !DILocation(line: 102, column: 5, scope: !1472)
!1506 = !DILocation(line: 102, column: 8, scope: !1472)
!1507 = !DILocation(line: 102, column: 19, scope: !1472)
!1508 = !DILocation(line: 103, column: 1, scope: !1472)
!1509 = distinct !DISubprogram(name: "mavlink_sha256_update", scope: !128, file: !128, line: 152, type: !1510, scopeLine: 153, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!1510 = !DISubroutineType(types: !1511)
!1511 = !{null, !1475, !1512, !13}
!1512 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1513, size: 64)
!1513 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!1514 = !DILocalVariable(name: "m", arg: 1, scope: !1509, file: !128, line: 152, type: !1475)
!1515 = !DILocation(line: 152, column: 63, scope: !1509)
!1516 = !DILocalVariable(name: "v", arg: 2, scope: !1509, file: !128, line: 152, type: !1512)
!1517 = !DILocation(line: 152, column: 78, scope: !1509)
!1518 = !DILocalVariable(name: "len", arg: 3, scope: !1509, file: !128, line: 152, type: !13)
!1519 = !DILocation(line: 152, column: 90, scope: !1509)
!1520 = !DILocalVariable(name: "p", scope: !1509, file: !128, line: 154, type: !122)
!1521 = !DILocation(line: 154, column: 26, scope: !1509)
!1522 = !DILocation(line: 154, column: 53, scope: !1509)
!1523 = !DILocalVariable(name: "old_sz", scope: !1509, file: !128, line: 155, type: !13)
!1524 = !DILocation(line: 155, column: 14, scope: !1509)
!1525 = !DILocation(line: 155, column: 23, scope: !1509)
!1526 = !DILocation(line: 155, column: 26, scope: !1509)
!1527 = !DILocalVariable(name: "offset", scope: !1509, file: !128, line: 156, type: !13)
!1528 = !DILocation(line: 156, column: 14, scope: !1509)
!1529 = !DILocation(line: 158, column: 17, scope: !1509)
!1530 = !DILocation(line: 158, column: 21, scope: !1509)
!1531 = !DILocation(line: 158, column: 5, scope: !1509)
!1532 = !DILocation(line: 158, column: 8, scope: !1509)
!1533 = !DILocation(line: 158, column: 14, scope: !1509)
!1534 = !DILocation(line: 159, column: 9, scope: !1535)
!1535 = distinct !DILexicalBlock(scope: !1509, file: !128, line: 159, column: 9)
!1536 = !DILocation(line: 159, column: 12, scope: !1535)
!1537 = !DILocation(line: 159, column: 20, scope: !1535)
!1538 = !DILocation(line: 159, column: 18, scope: !1535)
!1539 = !DILocation(line: 159, column: 9, scope: !1509)
!1540 = !DILocation(line: 160, column: 4, scope: !1535)
!1541 = !DILocation(line: 160, column: 7, scope: !1535)
!1542 = !DILocation(line: 160, column: 2, scope: !1535)
!1543 = !DILocation(line: 161, column: 15, scope: !1509)
!1544 = !DILocation(line: 161, column: 22, scope: !1509)
!1545 = !DILocation(line: 161, column: 27, scope: !1509)
!1546 = !DILocation(line: 161, column: 12, scope: !1509)
!1547 = !DILocation(line: 162, column: 5, scope: !1509)
!1548 = !DILocation(line: 162, column: 11, scope: !1509)
!1549 = !DILocation(line: 162, column: 15, scope: !1509)
!1550 = !DILocalVariable(name: "l", scope: !1551, file: !128, line: 163, type: !13)
!1551 = distinct !DILexicalBlock(scope: !1509, file: !128, line: 162, column: 19)
!1552 = !DILocation(line: 163, column: 11, scope: !1551)
!1553 = !DILocation(line: 163, column: 20, scope: !1551)
!1554 = !DILocation(line: 163, column: 18, scope: !1551)
!1555 = !DILocation(line: 164, column: 13, scope: !1556)
!1556 = distinct !DILexicalBlock(scope: !1551, file: !128, line: 164, column: 13)
!1557 = !DILocation(line: 164, column: 19, scope: !1556)
!1558 = !DILocation(line: 164, column: 17, scope: !1556)
!1559 = !DILocation(line: 164, column: 13, scope: !1551)
!1560 = !DILocation(line: 165, column: 17, scope: !1561)
!1561 = distinct !DILexicalBlock(scope: !1556, file: !128, line: 164, column: 22)
!1562 = !DILocation(line: 165, column: 15, scope: !1561)
!1563 = !DILocation(line: 166, column: 9, scope: !1561)
!1564 = !DILocation(line: 167, column: 9, scope: !1551)
!1565 = !DILocation(line: 167, column: 12, scope: !1551)
!1566 = !DILocation(line: 167, column: 14, scope: !1551)
!1567 = !DILocation(line: 167, column: 27, scope: !1551)
!1568 = !DILocation(line: 167, column: 25, scope: !1551)
!1569 = !DILocation(line: 167, column: 35, scope: !1551)
!1570 = !DILocation(line: 167, column: 38, scope: !1551)
!1571 = !DILocation(line: 167, column: 2, scope: !1551)
!1572 = !DILocation(line: 168, column: 12, scope: !1551)
!1573 = !DILocation(line: 168, column: 9, scope: !1551)
!1574 = !DILocation(line: 169, column: 7, scope: !1551)
!1575 = !DILocation(line: 169, column: 4, scope: !1551)
!1576 = !DILocation(line: 170, column: 9, scope: !1551)
!1577 = !DILocation(line: 170, column: 6, scope: !1551)
!1578 = !DILocation(line: 171, column: 5, scope: !1579)
!1579 = distinct !DILexicalBlock(scope: !1551, file: !128, line: 171, column: 5)
!1580 = !DILocation(line: 171, column: 12, scope: !1579)
!1581 = !DILocation(line: 171, column: 5, scope: !1551)
!1582 = !DILocalVariable(name: "i", scope: !1583, file: !128, line: 172, type: !239)
!1583 = distinct !DILexicalBlock(scope: !1579, file: !128, line: 171, column: 18)
!1584 = !DILocation(line: 172, column: 10, scope: !1583)
!1585 = !DILocalVariable(name: "current", scope: !1583, file: !128, line: 173, type: !1255)
!1586 = !DILocation(line: 173, column: 15, scope: !1583)
!1587 = !DILocalVariable(name: "u", scope: !1583, file: !128, line: 174, type: !1588)
!1588 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !130, size: 64)
!1589 = !DILocation(line: 174, column: 22, scope: !1583)
!1590 = !DILocation(line: 174, column: 26, scope: !1583)
!1591 = !DILocation(line: 174, column: 29, scope: !1583)
!1592 = !DILocation(line: 174, column: 31, scope: !1583)
!1593 = !DILocation(line: 175, column: 13, scope: !1594)
!1594 = distinct !DILexicalBlock(scope: !1583, file: !128, line: 175, column: 6)
!1595 = !DILocation(line: 175, column: 11, scope: !1594)
!1596 = !DILocation(line: 175, column: 18, scope: !1597)
!1597 = distinct !DILexicalBlock(scope: !1594, file: !128, line: 175, column: 6)
!1598 = !DILocation(line: 175, column: 20, scope: !1597)
!1599 = !DILocation(line: 175, column: 6, scope: !1594)
!1600 = !DILocalVariable(name: "p1", scope: !1601, file: !128, line: 176, type: !118)
!1601 = distinct !DILexicalBlock(scope: !1597, file: !128, line: 175, column: 30)
!1602 = !DILocation(line: 176, column: 32, scope: !1601)
!1603 = !DILocation(line: 176, column: 55, scope: !1601)
!1604 = !DILocation(line: 176, column: 57, scope: !1601)
!1605 = !DILocation(line: 176, column: 37, scope: !1601)
!1606 = !DILocalVariable(name: "p2", scope: !1601, file: !128, line: 177, type: !124)
!1607 = !DILocation(line: 177, column: 26, scope: !1601)
!1608 = !DILocation(line: 177, column: 51, scope: !1601)
!1609 = !DILocation(line: 177, column: 43, scope: !1601)
!1610 = !DILocation(line: 177, column: 31, scope: !1601)
!1611 = !DILocation(line: 178, column: 25, scope: !1601)
!1612 = !DILocation(line: 178, column: 17, scope: !1601)
!1613 = !DILocation(line: 178, column: 23, scope: !1601)
!1614 = !DILocation(line: 179, column: 25, scope: !1601)
!1615 = !DILocation(line: 179, column: 17, scope: !1601)
!1616 = !DILocation(line: 179, column: 23, scope: !1601)
!1617 = !DILocation(line: 180, column: 25, scope: !1601)
!1618 = !DILocation(line: 180, column: 17, scope: !1601)
!1619 = !DILocation(line: 180, column: 23, scope: !1601)
!1620 = !DILocation(line: 181, column: 25, scope: !1601)
!1621 = !DILocation(line: 181, column: 17, scope: !1601)
!1622 = !DILocation(line: 181, column: 23, scope: !1601)
!1623 = !DILocation(line: 182, column: 6, scope: !1601)
!1624 = !DILocation(line: 175, column: 27, scope: !1597)
!1625 = !DILocation(line: 175, column: 6, scope: !1597)
!1626 = distinct !{!1626, !1599, !1627}
!1627 = !DILocation(line: 182, column: 6, scope: !1594)
!1628 = !DILocation(line: 183, column: 26, scope: !1583)
!1629 = !DILocation(line: 183, column: 29, scope: !1583)
!1630 = !DILocation(line: 183, column: 6, scope: !1583)
!1631 = !DILocation(line: 184, column: 13, scope: !1583)
!1632 = !DILocation(line: 185, column: 2, scope: !1583)
!1633 = distinct !{!1633, !1547, !1634}
!1634 = !DILocation(line: 186, column: 5, scope: !1509)
!1635 = !DILocation(line: 187, column: 1, scope: !1509)
!1636 = distinct !DISubprogram(name: "mavlink_sha256_final_48", scope: !128, file: !128, line: 192, type: !1637, scopeLine: 193, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!1637 = !DISubroutineType(types: !1638)
!1638 = !{null, !1475, !124}
!1639 = !DILocalVariable(name: "m", arg: 1, scope: !1636, file: !128, line: 192, type: !1475)
!1640 = !DILocation(line: 192, column: 65, scope: !1636)
!1641 = !DILocalVariable(name: "result", arg: 2, scope: !1636, file: !128, line: 192, type: !124)
!1642 = !DILocation(line: 192, column: 76, scope: !1636)
!1643 = !DILocalVariable(name: "zeros", scope: !1636, file: !128, line: 194, type: !1644)
!1644 = !DICompositeType(tag: DW_TAG_array_type, baseType: !21, size: 576, elements: !1645)
!1645 = !{!1646}
!1646 = !DISubrange(count: 72)
!1647 = !DILocation(line: 194, column: 19, scope: !1636)
!1648 = !DILocalVariable(name: "offset", scope: !1636, file: !128, line: 195, type: !17)
!1649 = !DILocation(line: 195, column: 14, scope: !1636)
!1650 = !DILocation(line: 195, column: 24, scope: !1636)
!1651 = !DILocation(line: 195, column: 27, scope: !1636)
!1652 = !DILocation(line: 195, column: 33, scope: !1636)
!1653 = !DILocation(line: 195, column: 38, scope: !1636)
!1654 = !DILocalVariable(name: "dstart", scope: !1636, file: !128, line: 196, type: !17)
!1655 = !DILocation(line: 196, column: 18, scope: !1636)
!1656 = !DILocation(line: 196, column: 34, scope: !1636)
!1657 = !DILocation(line: 196, column: 32, scope: !1636)
!1658 = !DILocation(line: 196, column: 41, scope: !1636)
!1659 = !DILocation(line: 196, column: 46, scope: !1636)
!1660 = !DILocation(line: 196, column: 51, scope: !1636)
!1661 = !DILocalVariable(name: "p", scope: !1636, file: !128, line: 197, type: !124)
!1662 = !DILocation(line: 197, column: 14, scope: !1636)
!1663 = !DILocation(line: 197, column: 30, scope: !1636)
!1664 = !DILocation(line: 197, column: 33, scope: !1636)
!1665 = !DILocation(line: 197, column: 18, scope: !1636)
!1666 = !DILocation(line: 199, column: 5, scope: !1636)
!1667 = !DILocation(line: 199, column: 12, scope: !1636)
!1668 = !DILocation(line: 200, column: 13, scope: !1636)
!1669 = !DILocation(line: 200, column: 19, scope: !1636)
!1670 = !DILocation(line: 200, column: 5, scope: !1636)
!1671 = !DILocation(line: 201, column: 24, scope: !1636)
!1672 = !DILocation(line: 201, column: 27, scope: !1636)
!1673 = !DILocation(line: 201, column: 33, scope: !1636)
!1674 = !DILocation(line: 201, column: 39, scope: !1636)
!1675 = !DILocation(line: 201, column: 23, scope: !1636)
!1676 = !DILocation(line: 201, column: 11, scope: !1636)
!1677 = !DILocation(line: 201, column: 17, scope: !1636)
!1678 = !DILocation(line: 201, column: 5, scope: !1636)
!1679 = !DILocation(line: 201, column: 21, scope: !1636)
!1680 = !DILocation(line: 202, column: 24, scope: !1636)
!1681 = !DILocation(line: 202, column: 27, scope: !1636)
!1682 = !DILocation(line: 202, column: 33, scope: !1636)
!1683 = !DILocation(line: 202, column: 39, scope: !1636)
!1684 = !DILocation(line: 202, column: 23, scope: !1636)
!1685 = !DILocation(line: 202, column: 11, scope: !1636)
!1686 = !DILocation(line: 202, column: 17, scope: !1636)
!1687 = !DILocation(line: 202, column: 5, scope: !1636)
!1688 = !DILocation(line: 202, column: 21, scope: !1636)
!1689 = !DILocation(line: 203, column: 24, scope: !1636)
!1690 = !DILocation(line: 203, column: 27, scope: !1636)
!1691 = !DILocation(line: 203, column: 33, scope: !1636)
!1692 = !DILocation(line: 203, column: 40, scope: !1636)
!1693 = !DILocation(line: 203, column: 23, scope: !1636)
!1694 = !DILocation(line: 203, column: 11, scope: !1636)
!1695 = !DILocation(line: 203, column: 17, scope: !1636)
!1696 = !DILocation(line: 203, column: 5, scope: !1636)
!1697 = !DILocation(line: 203, column: 21, scope: !1636)
!1698 = !DILocation(line: 204, column: 24, scope: !1636)
!1699 = !DILocation(line: 204, column: 27, scope: !1636)
!1700 = !DILocation(line: 204, column: 33, scope: !1636)
!1701 = !DILocation(line: 204, column: 40, scope: !1636)
!1702 = !DILocation(line: 204, column: 23, scope: !1636)
!1703 = !DILocation(line: 204, column: 11, scope: !1636)
!1704 = !DILocation(line: 204, column: 17, scope: !1636)
!1705 = !DILocation(line: 204, column: 5, scope: !1636)
!1706 = !DILocation(line: 204, column: 21, scope: !1636)
!1707 = !DILocation(line: 205, column: 24, scope: !1636)
!1708 = !DILocation(line: 205, column: 27, scope: !1636)
!1709 = !DILocation(line: 205, column: 33, scope: !1636)
!1710 = !DILocation(line: 205, column: 39, scope: !1636)
!1711 = !DILocation(line: 205, column: 23, scope: !1636)
!1712 = !DILocation(line: 205, column: 11, scope: !1636)
!1713 = !DILocation(line: 205, column: 17, scope: !1636)
!1714 = !DILocation(line: 205, column: 5, scope: !1636)
!1715 = !DILocation(line: 205, column: 21, scope: !1636)
!1716 = !DILocation(line: 206, column: 24, scope: !1636)
!1717 = !DILocation(line: 206, column: 27, scope: !1636)
!1718 = !DILocation(line: 206, column: 33, scope: !1636)
!1719 = !DILocation(line: 206, column: 39, scope: !1636)
!1720 = !DILocation(line: 206, column: 23, scope: !1636)
!1721 = !DILocation(line: 206, column: 11, scope: !1636)
!1722 = !DILocation(line: 206, column: 17, scope: !1636)
!1723 = !DILocation(line: 206, column: 5, scope: !1636)
!1724 = !DILocation(line: 206, column: 21, scope: !1636)
!1725 = !DILocation(line: 207, column: 24, scope: !1636)
!1726 = !DILocation(line: 207, column: 27, scope: !1636)
!1727 = !DILocation(line: 207, column: 33, scope: !1636)
!1728 = !DILocation(line: 207, column: 40, scope: !1636)
!1729 = !DILocation(line: 207, column: 23, scope: !1636)
!1730 = !DILocation(line: 207, column: 11, scope: !1636)
!1731 = !DILocation(line: 207, column: 17, scope: !1636)
!1732 = !DILocation(line: 207, column: 5, scope: !1636)
!1733 = !DILocation(line: 207, column: 21, scope: !1636)
!1734 = !DILocation(line: 208, column: 24, scope: !1636)
!1735 = !DILocation(line: 208, column: 27, scope: !1636)
!1736 = !DILocation(line: 208, column: 33, scope: !1636)
!1737 = !DILocation(line: 208, column: 40, scope: !1636)
!1738 = !DILocation(line: 208, column: 23, scope: !1636)
!1739 = !DILocation(line: 208, column: 11, scope: !1636)
!1740 = !DILocation(line: 208, column: 17, scope: !1636)
!1741 = !DILocation(line: 208, column: 5, scope: !1636)
!1742 = !DILocation(line: 208, column: 21, scope: !1636)
!1743 = !DILocation(line: 210, column: 27, scope: !1636)
!1744 = !DILocation(line: 210, column: 30, scope: !1636)
!1745 = !DILocation(line: 210, column: 37, scope: !1636)
!1746 = !DILocation(line: 210, column: 44, scope: !1636)
!1747 = !DILocation(line: 210, column: 5, scope: !1636)
!1748 = !DILocation(line: 215, column: 17, scope: !1636)
!1749 = !DILocation(line: 215, column: 5, scope: !1636)
!1750 = !DILocation(line: 215, column: 15, scope: !1636)
!1751 = !DILocation(line: 216, column: 17, scope: !1636)
!1752 = !DILocation(line: 216, column: 5, scope: !1636)
!1753 = !DILocation(line: 216, column: 15, scope: !1636)
!1754 = !DILocation(line: 217, column: 17, scope: !1636)
!1755 = !DILocation(line: 217, column: 5, scope: !1636)
!1756 = !DILocation(line: 217, column: 15, scope: !1636)
!1757 = !DILocation(line: 218, column: 17, scope: !1636)
!1758 = !DILocation(line: 218, column: 5, scope: !1636)
!1759 = !DILocation(line: 218, column: 15, scope: !1636)
!1760 = !DILocation(line: 219, column: 17, scope: !1636)
!1761 = !DILocation(line: 219, column: 5, scope: !1636)
!1762 = !DILocation(line: 219, column: 15, scope: !1636)
!1763 = !DILocation(line: 220, column: 17, scope: !1636)
!1764 = !DILocation(line: 220, column: 5, scope: !1636)
!1765 = !DILocation(line: 220, column: 15, scope: !1636)
!1766 = !DILocation(line: 221, column: 1, scope: !1636)
!1767 = distinct !DISubprogram(name: "mavlink_sha256_calc", scope: !128, file: !128, line: 105, type: !1768, scopeLine: 106, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!1768 = !DISubroutineType(types: !1769)
!1769 = !{null, !1475, !1770}
!1770 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!1771 = !DILocalVariable(name: "m", arg: 1, scope: !1767, file: !128, line: 105, type: !1475)
!1772 = !DILocation(line: 105, column: 60, scope: !1767)
!1773 = !DILocalVariable(name: "in", arg: 2, scope: !1767, file: !128, line: 105, type: !1770)
!1774 = !DILocation(line: 105, column: 73, scope: !1767)
!1775 = !DILocalVariable(name: "AA", scope: !1767, file: !128, line: 107, type: !13)
!1776 = !DILocation(line: 107, column: 14, scope: !1767)
!1777 = !DILocalVariable(name: "BB", scope: !1767, file: !128, line: 107, type: !13)
!1778 = !DILocation(line: 107, column: 18, scope: !1767)
!1779 = !DILocalVariable(name: "CC", scope: !1767, file: !128, line: 107, type: !13)
!1780 = !DILocation(line: 107, column: 22, scope: !1767)
!1781 = !DILocalVariable(name: "DD", scope: !1767, file: !128, line: 107, type: !13)
!1782 = !DILocation(line: 107, column: 26, scope: !1767)
!1783 = !DILocalVariable(name: "EE", scope: !1767, file: !128, line: 107, type: !13)
!1784 = !DILocation(line: 107, column: 30, scope: !1767)
!1785 = !DILocalVariable(name: "FF", scope: !1767, file: !128, line: 107, type: !13)
!1786 = !DILocation(line: 107, column: 34, scope: !1767)
!1787 = !DILocalVariable(name: "GG", scope: !1767, file: !128, line: 107, type: !13)
!1788 = !DILocation(line: 107, column: 38, scope: !1767)
!1789 = !DILocalVariable(name: "HH", scope: !1767, file: !128, line: 107, type: !13)
!1790 = !DILocation(line: 107, column: 42, scope: !1767)
!1791 = !DILocalVariable(name: "data", scope: !1767, file: !128, line: 108, type: !1792)
!1792 = !DICompositeType(tag: DW_TAG_array_type, baseType: !13, size: 2048, elements: !131)
!1793 = !DILocation(line: 108, column: 14, scope: !1767)
!1794 = !DILocalVariable(name: "i", scope: !1767, file: !128, line: 109, type: !239)
!1795 = !DILocation(line: 109, column: 9, scope: !1767)
!1796 = !DILocation(line: 111, column: 10, scope: !1767)
!1797 = !DILocation(line: 111, column: 13, scope: !1767)
!1798 = !DILocation(line: 111, column: 8, scope: !1767)
!1799 = !DILocation(line: 112, column: 10, scope: !1767)
!1800 = !DILocation(line: 112, column: 13, scope: !1767)
!1801 = !DILocation(line: 112, column: 8, scope: !1767)
!1802 = !DILocation(line: 113, column: 10, scope: !1767)
!1803 = !DILocation(line: 113, column: 13, scope: !1767)
!1804 = !DILocation(line: 113, column: 8, scope: !1767)
!1805 = !DILocation(line: 114, column: 10, scope: !1767)
!1806 = !DILocation(line: 114, column: 13, scope: !1767)
!1807 = !DILocation(line: 114, column: 8, scope: !1767)
!1808 = !DILocation(line: 115, column: 10, scope: !1767)
!1809 = !DILocation(line: 115, column: 13, scope: !1767)
!1810 = !DILocation(line: 115, column: 8, scope: !1767)
!1811 = !DILocation(line: 116, column: 10, scope: !1767)
!1812 = !DILocation(line: 116, column: 13, scope: !1767)
!1813 = !DILocation(line: 116, column: 8, scope: !1767)
!1814 = !DILocation(line: 117, column: 10, scope: !1767)
!1815 = !DILocation(line: 117, column: 13, scope: !1767)
!1816 = !DILocation(line: 117, column: 8, scope: !1767)
!1817 = !DILocation(line: 118, column: 10, scope: !1767)
!1818 = !DILocation(line: 118, column: 13, scope: !1767)
!1819 = !DILocation(line: 118, column: 8, scope: !1767)
!1820 = !DILocation(line: 120, column: 12, scope: !1821)
!1821 = distinct !DILexicalBlock(scope: !1767, file: !128, line: 120, column: 5)
!1822 = !DILocation(line: 120, column: 10, scope: !1821)
!1823 = !DILocation(line: 120, column: 17, scope: !1824)
!1824 = distinct !DILexicalBlock(scope: !1821, file: !128, line: 120, column: 5)
!1825 = !DILocation(line: 120, column: 19, scope: !1824)
!1826 = !DILocation(line: 120, column: 5, scope: !1821)
!1827 = !DILocation(line: 121, column: 12, scope: !1824)
!1828 = !DILocation(line: 121, column: 15, scope: !1824)
!1829 = !DILocation(line: 121, column: 7, scope: !1824)
!1830 = !DILocation(line: 121, column: 2, scope: !1824)
!1831 = !DILocation(line: 121, column: 10, scope: !1824)
!1832 = !DILocation(line: 120, column: 25, scope: !1824)
!1833 = !DILocation(line: 120, column: 5, scope: !1824)
!1834 = distinct !{!1834, !1826, !1835}
!1835 = !DILocation(line: 121, column: 16, scope: !1821)
!1836 = !DILocation(line: 122, column: 12, scope: !1837)
!1837 = distinct !DILexicalBlock(scope: !1767, file: !128, line: 122, column: 5)
!1838 = !DILocation(line: 122, column: 10, scope: !1837)
!1839 = !DILocation(line: 122, column: 18, scope: !1840)
!1840 = distinct !DILexicalBlock(scope: !1837, file: !128, line: 122, column: 5)
!1841 = !DILocation(line: 122, column: 20, scope: !1840)
!1842 = !DILocation(line: 122, column: 5, scope: !1837)
!1843 = !DILocation(line: 123, column: 12, scope: !1840)
!1844 = !DILocation(line: 123, column: 37, scope: !1840)
!1845 = !DILocation(line: 123, column: 38, scope: !1840)
!1846 = !DILocation(line: 123, column: 32, scope: !1840)
!1847 = !DILocation(line: 123, column: 30, scope: !1840)
!1848 = !DILocation(line: 124, column: 6, scope: !1840)
!1849 = !DILocation(line: 123, column: 42, scope: !1840)
!1850 = !DILocation(line: 124, column: 32, scope: !1840)
!1851 = !DILocation(line: 124, column: 34, scope: !1840)
!1852 = !DILocation(line: 124, column: 27, scope: !1840)
!1853 = !DILocation(line: 124, column: 25, scope: !1840)
!1854 = !DILocation(line: 123, column: 7, scope: !1840)
!1855 = !DILocation(line: 123, column: 2, scope: !1840)
!1856 = !DILocation(line: 123, column: 10, scope: !1840)
!1857 = !DILocation(line: 122, column: 26, scope: !1840)
!1858 = !DILocation(line: 122, column: 5, scope: !1840)
!1859 = distinct !{!1859, !1842, !1860}
!1860 = !DILocation(line: 124, column: 38, scope: !1837)
!1861 = !DILocation(line: 126, column: 12, scope: !1862)
!1862 = distinct !DILexicalBlock(scope: !1767, file: !128, line: 126, column: 5)
!1863 = !DILocation(line: 126, column: 10, scope: !1862)
!1864 = !DILocation(line: 126, column: 17, scope: !1865)
!1865 = distinct !DILexicalBlock(scope: !1862, file: !128, line: 126, column: 5)
!1866 = !DILocation(line: 126, column: 19, scope: !1865)
!1867 = !DILocation(line: 126, column: 5, scope: !1862)
!1868 = !DILocalVariable(name: "T1", scope: !1869, file: !128, line: 127, type: !13)
!1869 = distinct !DILexicalBlock(scope: !1865, file: !128, line: 126, column: 30)
!1870 = !DILocation(line: 127, column: 11, scope: !1869)
!1871 = !DILocalVariable(name: "T2", scope: !1869, file: !128, line: 127, type: !13)
!1872 = !DILocation(line: 127, column: 15, scope: !1869)
!1873 = !DILocation(line: 129, column: 7, scope: !1869)
!1874 = !DILocation(line: 129, column: 12, scope: !1869)
!1875 = !DILocation(line: 129, column: 10, scope: !1869)
!1876 = !DILocation(line: 129, column: 25, scope: !1869)
!1877 = !DILocation(line: 129, column: 23, scope: !1869)
!1878 = !DILocation(line: 129, column: 70, scope: !1869)
!1879 = !DILocation(line: 129, column: 42, scope: !1869)
!1880 = !DILocation(line: 129, column: 40, scope: !1869)
!1881 = !DILocation(line: 129, column: 80, scope: !1869)
!1882 = !DILocation(line: 129, column: 75, scope: !1869)
!1883 = !DILocation(line: 129, column: 73, scope: !1869)
!1884 = !DILocation(line: 129, column: 5, scope: !1869)
!1885 = !DILocation(line: 130, column: 7, scope: !1869)
!1886 = !DILocation(line: 130, column: 20, scope: !1869)
!1887 = !DILocation(line: 130, column: 18, scope: !1869)
!1888 = !DILocation(line: 130, column: 5, scope: !1869)
!1889 = !DILocation(line: 132, column: 7, scope: !1869)
!1890 = !DILocation(line: 132, column: 5, scope: !1869)
!1891 = !DILocation(line: 133, column: 7, scope: !1869)
!1892 = !DILocation(line: 133, column: 5, scope: !1869)
!1893 = !DILocation(line: 134, column: 7, scope: !1869)
!1894 = !DILocation(line: 134, column: 5, scope: !1869)
!1895 = !DILocation(line: 135, column: 7, scope: !1869)
!1896 = !DILocation(line: 135, column: 12, scope: !1869)
!1897 = !DILocation(line: 135, column: 10, scope: !1869)
!1898 = !DILocation(line: 135, column: 5, scope: !1869)
!1899 = !DILocation(line: 136, column: 7, scope: !1869)
!1900 = !DILocation(line: 136, column: 5, scope: !1869)
!1901 = !DILocation(line: 137, column: 7, scope: !1869)
!1902 = !DILocation(line: 137, column: 5, scope: !1869)
!1903 = !DILocation(line: 138, column: 7, scope: !1869)
!1904 = !DILocation(line: 138, column: 5, scope: !1869)
!1905 = !DILocation(line: 139, column: 7, scope: !1869)
!1906 = !DILocation(line: 139, column: 12, scope: !1869)
!1907 = !DILocation(line: 139, column: 10, scope: !1869)
!1908 = !DILocation(line: 139, column: 5, scope: !1869)
!1909 = !DILocation(line: 140, column: 5, scope: !1869)
!1910 = !DILocation(line: 126, column: 26, scope: !1865)
!1911 = !DILocation(line: 126, column: 5, scope: !1865)
!1912 = distinct !{!1912, !1867, !1913}
!1913 = !DILocation(line: 140, column: 5, scope: !1862)
!1914 = !DILocation(line: 142, column: 22, scope: !1767)
!1915 = !DILocation(line: 142, column: 5, scope: !1767)
!1916 = !DILocation(line: 142, column: 8, scope: !1767)
!1917 = !DILocation(line: 142, column: 19, scope: !1767)
!1918 = !DILocation(line: 143, column: 22, scope: !1767)
!1919 = !DILocation(line: 143, column: 5, scope: !1767)
!1920 = !DILocation(line: 143, column: 8, scope: !1767)
!1921 = !DILocation(line: 143, column: 19, scope: !1767)
!1922 = !DILocation(line: 144, column: 22, scope: !1767)
!1923 = !DILocation(line: 144, column: 5, scope: !1767)
!1924 = !DILocation(line: 144, column: 8, scope: !1767)
!1925 = !DILocation(line: 144, column: 19, scope: !1767)
!1926 = !DILocation(line: 145, column: 22, scope: !1767)
!1927 = !DILocation(line: 145, column: 5, scope: !1767)
!1928 = !DILocation(line: 145, column: 8, scope: !1767)
!1929 = !DILocation(line: 145, column: 19, scope: !1767)
!1930 = !DILocation(line: 146, column: 22, scope: !1767)
!1931 = !DILocation(line: 146, column: 5, scope: !1767)
!1932 = !DILocation(line: 146, column: 8, scope: !1767)
!1933 = !DILocation(line: 146, column: 19, scope: !1767)
!1934 = !DILocation(line: 147, column: 22, scope: !1767)
!1935 = !DILocation(line: 147, column: 5, scope: !1767)
!1936 = !DILocation(line: 147, column: 8, scope: !1767)
!1937 = !DILocation(line: 147, column: 19, scope: !1767)
!1938 = !DILocation(line: 148, column: 22, scope: !1767)
!1939 = !DILocation(line: 148, column: 5, scope: !1767)
!1940 = !DILocation(line: 148, column: 8, scope: !1767)
!1941 = !DILocation(line: 148, column: 19, scope: !1767)
!1942 = !DILocation(line: 149, column: 22, scope: !1767)
!1943 = !DILocation(line: 149, column: 5, scope: !1767)
!1944 = !DILocation(line: 149, column: 8, scope: !1767)
!1945 = !DILocation(line: 149, column: 19, scope: !1767)
!1946 = !DILocation(line: 150, column: 1, scope: !1767)
!1947 = distinct !DISubprogram(name: "crc_init", scope: !1436, file: !1436, line: 52, type: !1948, scopeLine: 53, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !27, retainedNodes: !168)
!1948 = !DISubroutineType(types: !1949)
!1949 = !{null, !1439}
!1950 = !DILocalVariable(name: "crcAccum", arg: 1, scope: !1947, file: !1436, line: 52, type: !1439)
!1951 = !DILocation(line: 52, column: 39, scope: !1947)
!1952 = !DILocation(line: 54, column: 10, scope: !1947)
!1953 = !DILocation(line: 54, column: 19, scope: !1947)
!1954 = !DILocation(line: 55, column: 1, scope: !1947)
