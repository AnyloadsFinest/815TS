[1mdiff --git a/GettingStarted/ExampleInfo.ENU.xml b/Getting Started/ExampleInfo.ENU.xml[m
[1msimilarity index 100%[m
[1mrename from GettingStarted/ExampleInfo.ENU.xml[m
[1mrename to Getting Started/ExampleInfo.ENU.xml[m
[1mdiff --git a/Getting Started/Flash Debug/Exe/GettingStarted.sim b/Getting Started/Flash Debug/Exe/GettingStarted.sim[m
[1mnew file mode 100644[m
[1mindex 0000000..452f4c1[m
Binary files /dev/null and b/Getting Started/Flash Debug/Exe/GettingStarted.sim differ
[1mdiff --git a/Getting Started/Flash Debug/List/GettingStarted.map b/Getting Started/Flash Debug/List/GettingStarted.map[m
[1mnew file mode 100644[m
[1mindex 0000000..4cb9a96[m
[1m--- /dev/null[m
[1m+++ b/Getting Started/Flash Debug/List/GettingStarted.map[m	
[36m@@ -0,0 +1,1199 @@[m
[32m+[m[32m###############################################################################[m
[32m+[m[32m#[m
[32m+[m[32m# IAR ELF Linker V8.22.2.15995/W32 for ARM                28/Nov/2024  09:43:46[m
[32m+[m[32m# Copyright 2007-2018 IAR Systems AB.[m
[32m+[m[32m#[m
[32m+[m[32m#    Output file  =[m[41m  [m
[32m+[m[32m#        D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Exe\GettingStarted.out.tmp[m
[32m+[m[32m#    Map file     =[m[41m  [m
[32m+[m[32m#        D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\List\GettingStarted.map[m
[32m+[m[32m#    Command line =[m[41m  [m
[32m+[m[32m#        -f C:\Users\Hamid\AppData\Local\Temp\EWC1BC.tmp ("D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\ads1232.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\audit.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\calcmd.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\cstartup_M.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\display.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\filter.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\flash.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\globles.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\gpio.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\irq.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\keypad.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\keyscan.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\main.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\motion.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\peakhold.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\rtc.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\spi.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\strings.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\sys.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\system_init.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\tare.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\target.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\timedate.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\timer.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\uart.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\units.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\version[m
[32m+[m[32m#        control.o" "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\wdt.o" "D:\01 Hamid[m
[32m+[m[32m#        Projects\2021 Anyload Indicator 815TS\Software Development\124 Any 815[m
[32m+[m[32m#        factory\124 Any 815 factory\GettingStarted\Flash Debug\Obj\weight.o"[m
[32m+[m[32m#        "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\Flash Debug\Obj\zero.o" --redirect[m
[32m+[m[32m#        _Printf=_PrintfFullNoMb --redirect _Scanf=_ScanfFullNoMb -o "D:\01[m
[32m+[m[32m#        Hamid Projects\2021 Anyload Indicator 815TS\Software Development\124[m
[32m+[m[32m#        Any 815 factory\124 Any 815 factory\GettingStarted\Flash[m
[32m+[m[32m#        Debug\Exe\GettingStarted.out.tmp" --map "D:\01 Hamid Projects\2021[m
[32m+[m[32m#        Anyload Indicator 815TS\Software Development\124 Any 815 factory\124[m
[32m+[m[32m#        Any 815 factory\GettingStarted\Flash Debug\List\GettingStarted.map"[m
[32m+[m[32m#        --config "D:\01 Hamid Projects\2021 Anyload Indicator 815TS\Software[m
[32m+[m[32m#        Development\124 Any 815 factory\124 Any 815[m
[32m+[m[32m#        factory\GettingStarted\config\LPC1768_Flash.icf" --semihosting --entry[m
[32m+[m[32m#        __iar_program_start --redirect __iar_sh_stdout=__iar_sh_stdout_swo[m
[32m+[m[32m#        --vfe --text_out locale)[m
[32m+[m[32m#[m
[32m+[m[32m###############################################################################[m
[32m+[m
[32m+[m[32m*******************************************************************************[m
[32m+[m[32m*** MESSAGES[m
[32m+[m[32m***[m
[32m+[m
[32m+[m[32mError[Li005]: no definition for "format_string" [referenced from D:\01 Hamid[m
[32m+[m[32m          Projects\2021 Anyload Indicator 815TS\Software Development\124 Any[m
[32m+[m[32m          815 factory\124 Any 815 factory\GettingStarted\Flash[m
[32m+[m[32m          Debug\Obj\strings.o][m
[32m+[m
[32m+[m
[32m+[m[32m*******************************************************************************[m
[32m+[m[32m*** RUNTIME MODEL ATTRIBUTES[m
[32m+[m[32m***[m
[32m+[m
[32m+[m[32mCppFlavor       = *[m
[32m+[m[32m__SystemLibrary = DLib[m
[32m+[m[32m__dlib_version  = 6[m
[32m+[m
[32m+[m
[32m+[m[32m*******************************************************************************[m
[32m+[m[32m*** HEAP SELECTION[m
[32m+[m[32m***[m
[32m+[m
[32m+[m[32mThe basic heap was selected because no calls to memory allocation[m
[32m+[m[32mfunctions were found in the application outside of system library[m
[32m+[m[32mfunctions, and there are calls to deallocation functions in the[m
[32m+[m[32mapplication.[m
[32m+[m
[32m+[m
[32m+[m[32m*******************************************************************************[m
[32m+[m[32m*** PLACEMENT SUMMARY[m
[32m+[m[32m***[m
[32m+[m
[32m+[m[32m"A0":  place at 0x00000000 { ro section .intvec };[m
[32m+[m[32m"P1":  place in [from 0x00000000 to 0x000002fb] |[m
[32m+[m[32m                [from 0x00000300 to 0x0007ffff] { ro };[m
[32m+[m[32mdefine block CSTACK with size = 2K, alignment = 8 { };[m
[32m+[m[32mdefine block HEAP with size = 1K, alignment = 8 { };[m
[32m+[m[32m"P2":  place in [from 0x10000000 to 0x10007fdf] {[m
[32m+[m[32m          rw, block CSTACK, block HEAP };[m
[32m+[m[32m"P4":  place in [from 0x000002fc to 0x000002ff] { section .crp };[m
[32m+[m[32minitialize by copy { rw };[m
[32m+[m
[32m+[m[32mNo sections matched the following patterns:[m
[32m+[m
[32m+[m[32m  rw data section AHB_RAM_MEMORY  in "P3"[m
[32m+[m[32m  section EMAC_DMA_RAM            in "P3"[m
[32m+[m[32m  section USB_DMA_RAM             in "P3"[m
[32m+[m
[32m+[m
[32m+[m[32m  Section            Kind        Address    Size  Object[m
[32m+[m[32m  -------            ----        -------    ----  ------[m
[32m+[m[32m"A0":                                       0xcc[m
[32m+[m[32m  .intvec            ro code  0x00000000    0xcc  cstartup_M.o [1][m
[32m+[m[32m                            - 0x000000cc    0xcc[m
[32m+[m
[32m+[m[32m"P1", part 1 of 2:                          0x78[m
[32m+[m[32m  .rodata            const    0x000000cc     0x0  zero_init3.o [5][m
[32m+[m[32m  .rodata            const    0x000000cc     0x0  copy_init3.o [5][m
[32m+[m[32m  Initializer bytes  const    0x000000cc    0x78  <for P2 s0> (used: 0x0)[m
[32m+[m[32m                            - 0x00000144    0x78[m
[32m+[m
[32m+[m[32m"P4":                                        0x4[m
[32m+[m[32m  .crp               ro code  0x000002fc     0x4  cstartup_M.o [1][m
[32m+[m[32m                            - 0x00000300     0x4[m
[32m+[m
[32m+[m[32m"P1", part 2 of 2:                        0x9598[m
[32m+[m[32m  .text              ro code  0x00000300  0x20d0  calcmd.o [1][m
[32m+[m[32m  .text              ro code  0x000023d0   0xa5c  sys.o [1][m
[32m+[m[32m  .text              ro code  0x00002e2c   0x2e0  zero.o [1][m
[32m+[m[32m  .text              ro code  0x0000310c   0x390  weight.o [1][m
[32m+[m[32m  .text              ro code  0x0000349c    0xfc  FltDiv.o [4][m
[32m+[m[32m  .text              ro code  0x00003598   0x4e4  system_init.o [1][m
[32m+[m[32m  .text              ro code  0x00003a7c   0x710  uart.o [1][m
[32m+[m[32m  .text              ro code  0x0000418c   0x1d8  gpio.o [1][m
[32m+[m[32m  .text              ro code  0x00004364   0x194  version control.o [1][m
[32m+[m[32m  .text              ro code  0x000044f8   0x31c  timer.o [1][m
[32m+[m[32m  .text              ro code  0x00004814   0x260  keypad.o [1][m
[32m+[m[32m  .text              ro code  0x00004a74    0x40  sprintf.o [3][m
[32m+[m[32m  .text              ro code  0x00004ab4   0x6e8  display.o [1][m
[32m+[m[32m  .text              ro code  0x0000519c   0x2c0  filter.o [1][m
[32m+[m[32m  .text              ro code  0x0000545c    0x1c  U32ToFlt.o [4][m
[32m+[m[32m  .text              ro code  0x00005478    0xd8  FltMul.o [4][m
[32m+[m[32m  .text              ro code  0x00005550    0x24  S32ToFlt.o [4][m
[32m+[m[32m  .text              ro code  0x00005574   0x1fc  flash.o [1][m
[32m+[m[32m  .text              ro code  0x00005770    0x1c  FltCmpEq.o [4][m
[32m+[m[32m  .text              ro code  0x0000578c    0x22  S32ToDbl.o [4][m
[32m+[m[32m  .text              ro code  0x000057ae     0xa  xsprout.o [3][m
[32m+[m[32m  .text              ro code  0x000057b8    0x20  DblCmpEq.o [4][m
[32m+[m[32m  .text              ro code  0x000057d8   0x246  DblDiv.o [4][m
[32m+[m[32m  .text              ro code  0x00005a1e     0x2  cstartup_M.o [1][m
[32m+[m[32m  .text              ro code  0x00005a20    0xb8  modf.o [4][m
[32m+[m[32m  .text              ro code  0x00005ad8   0x1a2  DblMul.o [4][m
[32m+[m[32m  .text              ro code  0x00005c7c    0x2a  FltToS32.o [4][m
[32m+[m[32m  .text              ro code  0x00005ca8   0x10c  audit.o [1][m
[32m+[m[32m  .text              ro code  0x00005db4    0x22  FltToU32.o [4][m
[32m+[m[32m  .text              ro code  0x00005dd8    0x24  FltCmpGe.o [4][m
[32m+[m[32m  .text              ro code  0x00005dfc    0x80  FltSub.o [4][m
[32m+[m[32m  .text              ro code  0x00005e7c   0x108  tare.o [1][m
[32m+[m[32m  .text              ro code  0x00005f84    0x3a  DblToS32.o [4][m
[32m+[m[32m  .text              ro code  0x00005fc0    0x48  FltToDbl.o [4][m
[32m+[m[32m  .text              ro code  0x00006008    0x68  DblToFlt.o [4][m
[32m+[m[32m  .text              ro code  0x00006070    0x24  FltCmpLe.o [4][m
[32m+[m[32m  .text              ro code  0x00006094   0x150  peakhold.o [1][m
[32m+[m[32m  .text              ro code  0x000061e4   0xb08  units.o [1][m
[32m+[m[32m  .text              ro code  0x00006cec    0x84  irq.o [1][m
[32m+[m[32m  .text              ro code  0x00006d70     0x8  labs.o [3][m
[32m+[m[32m  .text              ro code  0x00006d78    0x60  keyscan.o [1][m
[32m+[m[32m  .text              ro code  0x00006dd8   0xe22  xprintffull_nomb.o [3][m
[32m+[m[32m  .text              ro code  0x00007bfc    0xa6  ABImemcpy.o [5][m
[32m+[m[32m  .text              ro code  0x00007ca4    0x6c  spi.o [1][m
[32m+[m[32m  .text             