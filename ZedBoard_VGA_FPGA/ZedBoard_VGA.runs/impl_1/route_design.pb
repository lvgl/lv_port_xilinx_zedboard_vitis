
q
Command: %s
53*	vivadotcl2@
,route_design -directive Explore -tns_cleanup2default:defaultZ4-113h px? 
?
@Attempting to get a license for feature '%s' and/or device '%s'
308*common2"
Implementation2default:default2
xc7z0202default:defaultZ17-347h px? 
?
0Got license for feature '%s' and/or device '%s'
310*common2"
Implementation2default:default2
xc7z0202default:defaultZ17-349h px? 
p
,Running DRC as a precondition to command %s
22*	vivadotcl2 
route_design2default:defaultZ4-22h px? 
P
Running DRC with %s threads
24*drc2
22default:defaultZ23-27h px? 
V
DRC finished with %s
79*	vivadotcl2
0 Errors2default:defaultZ4-198h px? 
e
BPlease refer to the DRC report (report_drc) for more information.
80*	vivadotclZ4-199h px? 
V

Starting %s Task
103*constraints2
Routing2default:defaultZ18-103h px? 
^
Using Router directive '%s'.
20*	routeflow2
Explore2default:defaultZ35-270h px? 
}
BMultithreading enabled for route_design using a maximum of %s CPUs17*	routeflow2
22default:defaultZ35-254h px? 
p

Phase %s%s
101*constraints2
1 2default:default2#
Build RT Design2default:defaultZ18-101h px? 
C
.Phase 1 Build RT Design | Checksum: 18968973d
*commonh px? 
?

%s
*constraints2p
\Time (s): cpu = 00:00:34 ; elapsed = 00:00:30 . Memory (MB): peak = 2017.871 ; gain = 96.8242default:defaulth px? 
v

Phase %s%s
101*constraints2
2 2default:default2)
Router Initialization2default:defaultZ18-101h px? 
{

Phase %s%s
101*constraints2
2.1 2default:default2,
Fix Topology Constraints2default:defaultZ18-101h px? 
N
9Phase 2.1 Fix Topology Constraints | Checksum: 18968973d
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:00:34 ; elapsed = 00:00:30 . Memory (MB): peak = 2024.531 ; gain = 103.4842default:defaulth px? 
t

Phase %s%s
101*constraints2
2.2 2default:default2%
Pre Route Cleanup2default:defaultZ18-101h px? 
G
2Phase 2.2 Pre Route Cleanup | Checksum: 18968973d
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:00:34 ; elapsed = 00:00:30 . Memory (MB): peak = 2024.531 ; gain = 103.4842default:defaulth px? 
p

Phase %s%s
101*constraints2
2.3 2default:default2!
Update Timing2default:defaultZ18-101h px? 
C
.Phase 2.3 Update Timing | Checksum: 1779b9db6
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:00:37 ; elapsed = 00:00:32 . Memory (MB): peak = 2053.719 ; gain = 132.6722default:defaulth px? 
?
Intermediate Timing Summary %s164*route2J
6| WNS=0.236  | TNS=0.000  | WHS=-0.190 | THS=-60.822|
2default:defaultZ35-416h px? 
}

Phase %s%s
101*constraints2
2.4 2default:default2.
Update Timing for Bus Skew2default:defaultZ18-101h px? 
r

Phase %s%s
101*constraints2
2.4.1 2default:default2!
Update Timing2default:defaultZ18-101h px? 
E
0Phase 2.4.1 Update Timing | Checksum: 1f53dda40
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:00:39 ; elapsed = 00:00:34 . Memory (MB): peak = 2053.719 ; gain = 132.6722default:defaulth px? 
?
Intermediate Timing Summary %s164*route2J
6| WNS=0.236  | TNS=0.000  | WHS=N/A    | THS=N/A    |
2default:defaultZ35-416h px? 
P
;Phase 2.4 Update Timing for Bus Skew | Checksum: 12c36e54d
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:00:39 ; elapsed = 00:00:34 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
I
4Phase 2 Router Initialization | Checksum: 189019d70
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:00:39 ; elapsed = 00:00:34 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
p

Phase %s%s
101*constraints2
3 2default:default2#
Initial Routing2default:defaultZ18-101h px? 
q

Phase %s%s
101*constraints2
3.1 2default:default2"
Global Routing2default:defaultZ18-101h px? 
D
/Phase 3.1 Global Routing | Checksum: 189019d70
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:00:39 ; elapsed = 00:00:34 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
C
.Phase 3 Initial Routing | Checksum: 1686936fa
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:00:41 ; elapsed = 00:00:35 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
s

Phase %s%s
101*constraints2
4 2default:default2&
Rip-up And Reroute2default:defaultZ18-101h px? 
u

Phase %s%s
101*constraints2
4.1 2default:default2&
Global Iteration 02default:defaultZ18-101h px? 
?
Intermediate Timing Summary %s164*route2J
6| WNS=-0.170 | TNS=-4.027 | WHS=N/A    | THS=N/A    |
2default:defaultZ35-416h px? 
H
3Phase 4.1 Global Iteration 0 | Checksum: 1b22fc63e
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:11 ; elapsed = 00:00:58 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
u

Phase %s%s
101*constraints2
4.2 2default:default2&
Global Iteration 12default:defaultZ18-101h px? 
?
Intermediate Timing Summary %s164*route2J
6| WNS=-0.170 | TNS=-3.379 | WHS=N/A    | THS=N/A    |
2default:defaultZ35-416h px? 
H
3Phase 4.2 Global Iteration 1 | Checksum: 165fbc979
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:20 ; elapsed = 00:01:06 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
F
1Phase 4 Rip-up And Reroute | Checksum: 165fbc979
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:20 ; elapsed = 00:01:06 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
|

Phase %s%s
101*constraints2
5 2default:default2/
Delay and Skew Optimization2default:defaultZ18-101h px? 
n

Phase %s%s
101*constraints2
5.1 2default:default2
TNS Cleanup2default:defaultZ18-101h px? 
r

Phase %s%s
101*constraints2
5.1.1 2default:default2!
Delay CleanUp2default:defaultZ18-101h px? 
t

Phase %s%s
101*constraints2
5.1.1.1 2default:default2!
Update Timing2default:defaultZ18-101h px? 
G
2Phase 5.1.1.1 Update Timing | Checksum: 184c286fb
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:21 ; elapsed = 00:01:06 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
?
Intermediate Timing Summary %s164*route2J
6| WNS=-0.047 | TNS=-0.197 | WHS=N/A    | THS=N/A    |
2default:defaultZ35-416h px? 
t

Phase %s%s
101*constraints2
5.1.1.2 2default:default2!
Update Timing2default:defaultZ18-101h px? 
G
2Phase 5.1.1.2 Update Timing | Checksum: 17229e934
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:21 ; elapsed = 00:01:06 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
?
Intermediate Timing Summary %s164*route2J
6| WNS=0.009  | TNS=0.000  | WHS=N/A    | THS=N/A    |
2default:defaultZ35-416h px? 
E
0Phase 5.1.1 Delay CleanUp | Checksum: 1b9279639
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:21 ; elapsed = 00:01:06 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
A
,Phase 5.1 TNS Cleanup | Checksum: 1b9279639
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:21 ; elapsed = 00:01:06 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
z

Phase %s%s
101*constraints2
5.2 2default:default2+
Clock Skew Optimization2default:defaultZ18-101h px? 
M
8Phase 5.2 Clock Skew Optimization | Checksum: 1b9279639
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:21 ; elapsed = 00:01:06 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
O
:Phase 5 Delay and Skew Optimization | Checksum: 1b9279639
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:21 ; elapsed = 00:01:06 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
n

Phase %s%s
101*constraints2
6 2default:default2!
Post Hold Fix2default:defaultZ18-101h px? 
p

Phase %s%s
101*constraints2
6.1 2default:default2!
Hold Fix Iter2default:defaultZ18-101h px? 
r

Phase %s%s
101*constraints2
6.1.1 2default:default2!
Update Timing2default:defaultZ18-101h px? 
E
0Phase 6.1.1 Update Timing | Checksum: 17737156a
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:22 ; elapsed = 00:01:07 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
?
Intermediate Timing Summary %s164*route2J
6| WNS=0.009  | TNS=0.000  | WHS=0.028  | THS=0.000  |
2default:defaultZ35-416h px? 
B
-Phase 6.1 Hold Fix Iter | Checksum: ff25f30a
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:22 ; elapsed = 00:01:07 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
@
+Phase 6 Post Hold Fix | Checksum: ff25f30a
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:22 ; elapsed = 00:01:07 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
o

Phase %s%s
101*constraints2
7 2default:default2"
Route finalize2default:defaultZ18-101h px? 
B
-Phase 7 Route finalize | Checksum: 1ba0502cb
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:22 ; elapsed = 00:01:07 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
v

Phase %s%s
101*constraints2
8 2default:default2)
Verifying routed nets2default:defaultZ18-101h px? 
I
4Phase 8 Verifying routed nets | Checksum: 1ba0502cb
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:22 ; elapsed = 00:01:07 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
r

Phase %s%s
101*constraints2
9 2default:default2%
Depositing Routes2default:defaultZ18-101h px? 
E
0Phase 9 Depositing Routes | Checksum: 15574eb05
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:23 ; elapsed = 00:01:08 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
t

Phase %s%s
101*constraints2
10 2default:default2&
Post Router Timing2default:defaultZ18-101h px? 
?
Post Routing Timing Summary %s
20*route2J
6| WNS=0.010  | TNS=0.000  | WHS=0.028  | THS=0.000  |
2default:defaultZ35-20h px? 
G
2Phase 10 Post Router Timing | Checksum: 1918d4fae
*commonh px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:25 ; elapsed = 00:01:09 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
F
'The design met the timing requirement.
61*routeZ35-61h px? 
@
Router Completed Successfully
2*	routeflowZ35-16h px? 
?

%s
*constraints2q
]Time (s): cpu = 00:01:25 ; elapsed = 00:01:09 . Memory (MB): peak = 2059.215 ; gain = 138.1682default:defaulth px? 
Z
Releasing license: %s
83*common2"
Implementation2default:defaultZ17-83h px? 
?
G%s Infos, %s Warnings, %s Critical Warnings and %s Errors encountered.
28*	vivadotcl2
1042default:default2
42default:default2
02default:default2
02default:defaultZ4-41h px? 
^
%s completed successfully
29*	vivadotcl2 
route_design2default:defaultZ4-42h px? 
?
I%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s
268*common2"
route_design: 2default:default2
00:01:292default:default2
00:01:122default:default2
2059.2152default:default2
138.3752default:defaultZ17-268h px? 
H
&Writing timing data to binary archive.266*timingZ38-480h px? 
=
Writing XDEF routing.
211*designutilsZ20-211h px? 
J
#Writing XDEF routing logical nets.
209*designutilsZ20-209h px? 
J
#Writing XDEF routing special nets.
210*designutilsZ20-210h px? 
?
I%sTime (s): cpu = %s ; elapsed = %s . Memory (MB): peak = %s ; gain = %s
268*common2)
Write XDEF Complete: 2default:default2
00:00:022default:default2 
00:00:00.6412default:default2
2059.2152default:default2
0.0002default:defaultZ17-268h px? 
?
 The %s '%s' has been generated.
621*common2

checkpoint2default:default2u
aE:/ZeboardTemplate/Zed_Git/ZedBoard_VGA_FPGA/ZedBoard_VGA.runs/impl_1/design_1_wrapper_routed.dcp2default:defaultZ17-1381h px? 
?
%s4*runtcl2?
?Executing : report_drc -file design_1_wrapper_drc_routed.rpt -pb design_1_wrapper_drc_routed.pb -rpx design_1_wrapper_drc_routed.rpx
2default:defaulth px? 
?
Command: %s
53*	vivadotcl2?
xreport_drc -file design_1_wrapper_drc_routed.rpt -pb design_1_wrapper_drc_routed.pb -rpx design_1_wrapper_drc_routed.rpx2default:defaultZ4-113h px? 
>
IP Catalog is up to date.1232*coregenZ19-1839h px? 
P
Running DRC with %s threads
24*drc2
22default:defaultZ23-27h px? 
?
#The results of DRC are in file %s.
586*	vivadotcl2?
eE:/ZeboardTemplate/Zed_Git/ZedBoard_VGA_FPGA/ZedBoard_VGA.runs/impl_1/design_1_wrapper_drc_routed.rpteE:/ZeboardTemplate/Zed_Git/ZedBoard_VGA_FPGA/ZedBoard_VGA.runs/impl_1/design_1_wrapper_drc_routed.rpt2default:default8Z2-168h px? 
\
%s completed successfully
29*	vivadotcl2

report_drc2default:defaultZ4-42h px? 
?
%s4*runtcl2?
?Executing : report_methodology -file design_1_wrapper_methodology_drc_routed.rpt -pb design_1_wrapper_methodology_drc_routed.pb -rpx design_1_wrapper_methodology_drc_routed.rpx
2default:defaulth px? 
?
Command: %s
53*	vivadotcl2?
?report_methodology -file design_1_wrapper_methodology_drc_routed.rpt -pb design_1_wrapper_methodology_drc_routed.pb -rpx design_1_wrapper_methodology_drc_routed.rpx2default:defaultZ4-113h px? 
E
%Done setting XDC timing constraints.
35*timingZ38-35h px? 
Y
$Running Methodology with %s threads
74*drc2
22default:defaultZ23-133h px? 
?
2The results of Report Methodology are in file %s.
609*	vivadotcl2?
qE:/ZeboardTemplate/Zed_Git/ZedBoard_VGA_FPGA/ZedBoard_VGA.runs/impl_1/design_1_wrapper_methodology_drc_routed.rptqE:/ZeboardTemplate/Zed_Git/ZedBoard_VGA_FPGA/ZedBoard_VGA.runs/impl_1/design_1_wrapper_methodology_drc_routed.rpt2default:default8Z2-1520h px? 
d
%s completed successfully
29*	vivadotcl2&
report_methodology2default:defaultZ4-42h px? 
?
%s4*runtcl2?
?Executing : report_power -file design_1_wrapper_power_routed.rpt -pb design_1_wrapper_power_summary_routed.pb -rpx design_1_wrapper_power_routed.rpx
2default:defaulth px? 
?
Command: %s
53*	vivadotcl2?
?report_power -file design_1_wrapper_power_routed.rpt -pb design_1_wrapper_power_summary_routed.pb -rpx design_1_wrapper_power_routed.rpx2default:defaultZ4-113h px? 
E
%Done setting XDC timing constraints.
35*timingZ38-35h px? 
K
,Running Vector-less Activity Propagation...
51*powerZ33-51h px? 
P
3
Finished Running Vector-less Activity Propagation
1*powerZ33-1h px? 
?
G%s Infos, %s Warnings, %s Critical Warnings and %s Errors encountered.
28*	vivadotcl2
1162default:default2
42default:default2
02default:default2
02default:defaultZ4-41h px? 
^
%s completed successfully
29*	vivadotcl2 
report_power2default:defaultZ4-42h px? 
?
%s4*runtcl2?
mExecuting : report_route_status -file design_1_wrapper_route_status.rpt -pb design_1_wrapper_route_status.pb
2default:defaulth px? 
?
%s4*runtcl2?
?Executing : report_timing_summary -max_paths 10 -report_unconstrained -file design_1_wrapper_timing_summary_routed.rpt -pb design_1_wrapper_timing_summary_routed.pb -rpx design_1_wrapper_timing_summary_routed.rpx
2default:defaulth px? 
r
UpdateTimingParams:%s.
91*timing29
% Speed grade: -1, Delay Type: min_max2default:defaultZ38-91h px? 
|
CMultithreading enabled for timing update using a maximum of %s CPUs155*timing2
22default:defaultZ38-191h px? 
?
}There are set_bus_skew constraint(s) in this design. Please run report_bus_skew to ensure that bus skew requirements are met.223*timingZ38-436h px? 
?
%s4*runtcl2m
YExecuting : report_incremental_reuse -file design_1_wrapper_incremental_reuse_routed.rpt
2default:defaulth px? 
g
BIncremental flow is disabled. No incremental reuse Info to report.423*	vivadotclZ4-1062h px? 
?
%s4*runtcl2m
YExecuting : report_clock_utilization -file design_1_wrapper_clock_utilization_routed.rpt
2default:defaulth px? 
?
%s4*runtcl2?
?Executing : report_bus_skew -warn_on_violation -file design_1_wrapper_bus_skew_routed.rpt -pb design_1_wrapper_bus_skew_routed.pb -rpx design_1_wrapper_bus_skew_routed.rpx
2default:defaulth px? 
r
UpdateTimingParams:%s.
91*timing29
% Speed grade: -1, Delay Type: min_max2default:defaultZ38-91h px? 
|
CMultithreading enabled for timing update using a maximum of %s CPUs155*timing2
22default:defaultZ38-191h px? 


End Record