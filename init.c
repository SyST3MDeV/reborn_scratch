
/* WARNING: Could not reconcile some variable overlaps */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_140dd0c00(void)

{
  bool bVar1;
  bool bVar2;
  short *_Src;
  char cVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  char cVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  ulong uVar11;
  DWORD DVar12;
  rsize_t *prVar13;
  longlong lVar14;
  rsize_t *prVar15;
  undefined8 *puVar16;
  int *piVar17;
  HANDLE pvVar18;
  undefined2 *puVar19;
  wchar_t *pwVar20;
  int iVar21;
  rsize_t _SizeInWords;
  rsize_t *prVar22;
  undefined8 uVar23;
  longlong lVar24;
  undefined2 *puVar25;
  int iVar26;
  wchar_t *_Src_00;
  undefined2 *puVar27;
  rsize_t *prVar28;
  undefined4 extraout_XMM0_Da;
  undefined4 uVar29;
  ulong uVar30;
  ulong uVar31;
  undefined local_res10 [8];
  undefined local_res18 [8];
  undefined local_res20 [8];
  ulong uVar32;
  undefined8 uStack_2b0;
  undefined8 *local_2a8 [2];
  undefined8 *local_298;
  ulonglong uStack_290;
  int local_288 [2];
  wchar_t *local_280;
  short *local_278;
  wchar_t local_270 [15];
  undefined2 local_252;
  wchar_t *local_250;
  wchar_t *local_248;
  undefined8 local_240;
  longlong local_238;
  wchar_t *local_230;
  wchar_t *local_228;
  int local_220 [2];
  undefined2 *local_218;
  rsize_t *local_210;
  int local_208 [2];
  wchar_t *local_200;
  undefined local_1f8 [8];
  int local_1f0 [2];
  longlong local_1e8;
  undefined4 local_1e0;
  undefined4 local_1dc;
  int local_1d8 [2];
  undefined2 *local_1d0;
  undefined local_1c8 [8];
  longlong local_1c0;
  ulong local_1b8;
  ulong uStack_1b4;
  ulong uStack_1b0;
  ulong uStack_1ac;
  undefined local_1a8 [8];
  longlong local_1a0;
  wchar_t *local_198 [2];
  undefined local_188 [16];
  undefined8 local_178;
  _MEMORYSTATUSEX local_168;
  _MEMORYSTATUSEX local_128;
  undefined local_e8 [16];
  rsize_t *local_d8;
  undefined local_98 [16];
  rsize_t *local_88;
  
  local_178 = 0xfffffffffffffffe;
  prVar28 = (rsize_t *)0x0;
  DAT_143500358 = _set_purecall_handler(FUN_140dd0510);
  _DAT_1434a0b84 = GetCurrentThreadId();
  FUN_14051eb10(0);
  DAT_1434a0b7c = 1;
  prVar13 = (rsize_t *)
            FUN_140d2e160(0xfffffffe,0,0x58,8,0,0,0x31c0006,&DAT_1426b62fa,0,&DAT_1426b62fa);
  DAT_143495af0 = prVar28;
  if (prVar13 != (rsize_t *)0x0) {
    *prVar13 = (rsize_t)&PTR_FUN_142a17730;
    prVar13[1] = 0;
    prVar13[2] = 0;
    prVar13[5] = 0;
    prVar13[6] = 0;
    prVar13[7] = 0;
    prVar13[9] = 0;
    *(undefined4 *)(prVar13 + 10) = 0;
    DAT_143495af0 = prVar13;
  }
  FUN_140dd0940();
  PTR_FUN_143243510 = &DAT_1402e9ca0;
  if (DAT_1434a0ba0 == '\0') {
    cVar3 = FUN_140055e40(&DAT_143387550,L"SEEKFREEPACKAGEMAP");
    DAT_1434a0ba0 = '\0';
    if (cVar3 != '\0') goto LAB_140dd0d13;
  }
  else {
LAB_140dd0d13:
    DAT_1434a0ba0 = '\x01';
  }
  DAT_1434a0ba3 = FUN_140055e40(&DAT_143387550,L"COOKFORDEMO");
  lVar24 = -1;
  lVar14 = -1;
  do {
    lVar14 = lVar14 + 1;
  } while ((&DAT_143387550)[lVar14] != 0);
  _SizeInWords = (rsize_t)((int)lVar14 + 1);
  prVar15 = (rsize_t *)
            FUN_140d2e160(0xfffffffe,0,_SizeInWords * 2 + 8,2,8,0,0x3cf0007,&DAT_1426b62fa,0,
                          &DAT_1426b62fa);
  prVar13 = prVar28;
  if (prVar15 != (rsize_t *)0x0) {
    *prVar15 = _SizeInWords;
    prVar13 = prVar15 + 1;
  }
  wcscpy_s((wchar_t *)prVar13,_SizeInWords,&DAT_143387550);
  local_210 = prVar13;
  FUN_140056ba0(local_288,&local_210,0);
  FUN_140038590(local_288);
  pwVar20 = L"";
  if (local_288[0] != 0) {
    pwVar20 = local_280;
  }
  iVar8 = _wcsicmp(pwVar20,L"EDITOR");
  iVar26 = 1;
  if (iVar8 == 0) {
    FUN_140134470(local_res10,0,L"Editor is not supported in this build configuration.");
    iVar21 = 1;
    do {
      if (DAT_1434ddf68 == 0) break;
      Sleep(100);
      iVar21 = iVar21 + 1;
    } while (iVar21 < 0x32);
    PostQuitMessage(0);
    DAT_1434a4616 = '\x01';
  }
  DAT_1434a0b7f = 1;
  cVar3 = FUN_140055e40(&DAT_143387550,L"NOSEEKFREELOADING");
  iVar21 = 0;
  if (cVar3 != '\0') {
    DAT_1434a0b7f = 0;
  }
  cVar3 = FUN_140055e40(&DAT_143387550,L"SEEKFREELOADINGSERVER");
  DAT_143495afa = cVar3 != '\0';
  cVar3 = FUN_140055e40(&DAT_143387550,L"SEEKFREELOADINGAPOLLOWORKER");
  DAT_1434a0b7e = cVar3 != '\0';
  DAT_1434a4617 = 1;
  DAT_1434a0b7f = DAT_1434a0b7f | DAT_1434a0b7e | DAT_143495afa | 1;
  pwVar20 = L"";
  if (local_288[0] != 0) {
    pwVar20 = local_280;
  }
  iVar9 = _wcsicmp(pwVar20,L"MAKE");
  if (iVar9 == 0) {
LAB_140dd0ee2:
    FUN_140134470(local_res18,0,L"Make commandlet is not supported in this build configuration.");
    iVar9 = 1;
    do {
      if (DAT_1434ddf68 == 0) break;
      Sleep(100);
      iVar9 = iVar9 + 1;
    } while (iVar9 < 0x32);
    PostQuitMessage(0);
    DAT_1434a4616 = '\x01';
    DAT_14349e329 = '\x01';
    DAT_143495afc = DAT_143495afc | 2;
  }
  else {
    pwVar20 = L"";
    if (local_288[0] != 0) {
      pwVar20 = local_280;
    }
    iVar9 = _wcsicmp(pwVar20,L"MAKECOMMANDLET");
    if (iVar9 == 0) goto LAB_140dd0ee2;
  }
  DAT_1434a43eb = FUN_140055e40(&DAT_143387550,L"FORCELOGFLUSH");
  DAT_1434a0ba6 = FUN_140055e40(&DAT_143387550,L"FORCESOUNDRECOOK");
  DAT_1434a4623 = FUN_140055e40(&DAT_143387550,L"BIASCOMPRESSIONFORSIZE");
  lVar14 = FUN_140053100(&DAT_143387550,L"SETVERIFYGC=");
  if (lVar14 != 0) {
    if (*(wchar_t *)(lVar14 + 0x18) == L'\"') {
      wcsncpy_s(local_270,0x10,(wchar_t *)(lVar14 + 0x1a),0xf);
      local_252 = 0;
      pwVar20 = L"\"";
    }
    else {
      wcsncpy_s(local_270,0x10,(wchar_t *)(lVar14 + 0x18),0xf);
      local_252 = 0;
      pwVar20 = wcsstr(local_270,L" ");
      if (pwVar20 != (wchar_t *)0x0) {
        *pwVar20 = L'\0';
      }
      pwVar20 = wcsstr(local_270,L"\r");
      if (pwVar20 != (wchar_t *)0x0) {
        *pwVar20 = L'\0';
      }
      pwVar20 = wcsstr(local_270,L"\n");
      if (pwVar20 != (wchar_t *)0x0) {
        *pwVar20 = L'\0';
      }
      pwVar20 = wcsstr(local_270,L"\t");
      if (pwVar20 != (wchar_t *)0x0) {
        *pwVar20 = L'\0';
      }
      pwVar20 = &DAT_1426b5864;
    }
    pwVar20 = wcsstr(local_270,pwVar20);
    if (pwVar20 != (wchar_t *)0x0) {
      *pwVar20 = L'\0';
    }
    iVar9 = _wcsicmp(local_270,L"On");
    if (((iVar9 != 0) && (iVar9 = _wcsicmp(local_270,L"True"), iVar9 != 0)) &&
       (iVar9 = _wcsicmp(local_270,u_True_1432433d0), iVar9 != 0)) {
      iVar9 = _wcsicmp(local_270,L"1");
      DAT_14340f99d = 0;
      if (iVar9 != 0) goto LAB_140dd10af;
    }
    DAT_14340f99d = 1;
  }
LAB_140dd10af:
  DAT_1434e1555 = FUN_140055e40(&DAT_143387550,L"DEBUGFACEFX");
  DAT_1434e058b = FUN_140055e40(&DAT_143387550,L"TRACEANIMUSAGE");
  cVar3 = FUN_140055e40(&DAT_143387550,L"ForceMinimalShaderCompilation");
  if (cVar3 != '\0') {
    DAT_1434deb77 = 1;
  }
  if (DAT_1432c7838 == -1) {
    FUN_1400937c0();
  }
  FUN_140dccf80();
  puVar16 = (undefined8 *)
            FUN_140d2e160(0xfffffffe,0,8,8,0,0,0x3cf0008,&DAT_1426b62fa,0,&DAT_1426b62fa);
  if (puVar16 != (undefined8 *)0x0) {
    *puVar16 = &PTR_FUN_142a17700;
  }
  local_2a8[0] = (undefined8 *)
                 FUN_140d2e160(0xfffffffe,0,0x2468,8,0,0,0x3cf0008,&DAT_1426b62fa,0,&DAT_1426b62fa);
  if (local_2a8[0] != (undefined8 *)0x0) {
    *local_2a8[0] = &PTR_FUN_142a17480;
    `eh_vector_constructor_iterator'
              (local_2a8[0] + 1,0x60,0x61,(_func_void_void_ptr *)&LAB_140dd4ad0,FUN_140dcb910);
  }
  FUN_14005d2c0();
  if (iVar8 == 0) {
    local_278 = &DAT_143387550;
    FUN_140056ba0(local_1f0,&local_278,1);
    _Src = local_278;
    if ((local_1f0[0] != 0) && (0 < local_1f0[0] + -1)) {
      iVar8 = iVar21;
      if ((local_278 != (short *)0x0) && (*local_278 != 0)) {
        lVar14 = -1;
        do {
          lVar14 = lVar14 + 1;
        } while (local_278[lVar14] != 0);
        iVar8 = (int)lVar14 + 1;
      }
      FUN_14003ce30(local_208,iVar8);
      pwVar20 = local_200;
      if (local_208[0] == 0) {
        _Src_00 = L"";
      }
      else {
        memcpy(local_200,_Src,(longlong)local_208[0] * 2);
        _Src_00 = pwVar20;
      }
      wcscpy_s(&DAT_143387550,0x4000,_Src_00);
      local_278 = (short *)0x0;
      if (pwVar20 != (wchar_t *)0x0) {
        FUN_140d2e720(2,pwVar20);
        local_200 = (wchar_t *)0x0;
      }
    }
    if (local_1e8 != 0) {
      FUN_140d2e720(2);
      local_1e8 = 0;
    }
  }
  cVar3 = FUN_140055e40(&DAT_143387550,L"firstinstall");
  if (cVar3 != '\0') {
    FUN_1400acd50(DAT_14349e308,0,0);
    do {
      if (DAT_1434ddf68 == 0) break;
      Sleep(100);
      iVar26 = iVar26 + 1;
    } while (iVar26 < 0x32);
    DAT_1434a0bc9 = 1;
    _set_abort_behavior(0,3);
                    /* WARNING: Subroutine does not return */
    abort();
  }
  if (DAT_14324350c == '\0') {
    piVar17 = (int *)FUN_1400573f0(local_1c8,L"Error_GameAlreadyRunning",L"Launch");
    if (*piVar17 == 0) {
      puVar27 = &DAT_1426a9d58;
    }
    else {
      puVar27 = *(undefined2 **)(piVar17 + 2);
    }
    FUN_140134470(local_res20,0,puVar27);
    if (local_1c0 != 0) {
      FUN_140d2e720();
      local_1c0 = 0;
    }
    iVar8 = 1;
    do {
      if (DAT_1434ddf68 == 0) break;
      Sleep(100);
      iVar8 = iVar8 + 1;
    } while (iVar8 < 0x32);
    PostQuitMessage(0);
    DAT_1434a4616 = '\x01';
  }
  local_2a8[0] = (undefined8 *)
                 FUN_140d2e160(0xfffffffe,0,0x90,8,0,0,0x6190005,&DAT_1426b62fa,0,&DAT_1426b62fa);
  prVar15 = prVar28;
  if (local_2a8[0] != (undefined8 *)0x0) {
    prVar15 = (rsize_t *)FUN_1400a3c90(local_2a8[0]);
  }
  iVar8 = iVar21;
  if (DAT_143387550 != 0) {
    lVar14 = -1;
    do {
      lVar14 = lVar14 + 1;
    } while ((&DAT_143387550)[lVar14] != 0);
    iVar8 = (int)lVar14 + 1;
  }
  DAT_1434a0b40 = prVar15;
  FUN_14003ce30(local_1d8,iVar8);
  puVar27 = local_1d0;
  if (local_1d8[0] != 0) {
    memcpy(local_1d0,&DAT_143387550,(longlong)local_1d8[0] * 2);
  }
  bVar2 = false;
  puVar19 = &DAT_1426a9d58;
  if (local_1d8[0] != 0) {
    puVar19 = puVar27;
  }
  lVar14 = FUN_1400531f0(puVar19,L"DoingASentinelRun=1");
  if (lVar14 == 0) {
LAB_140dd14b4:
    iVar8 = iVar21;
    if (DAT_143387550 != 0) {
      do {
        lVar24 = lVar24 + 1;
      } while ((&DAT_143387550)[lVar24] != 0);
      iVar8 = (int)lVar24 + 1;
    }
    FUN_14003ce30(local_220,iVar8);
    puVar25 = local_218;
    if (local_220[0] != 0) {
      memcpy(local_218,&DAT_143387550,(longlong)local_220[0] * 2);
    }
    bVar2 = true;
    puVar19 = &DAT_1426a9d58;
    if (local_220[0] != 0) {
      puVar19 = puVar25;
    }
    lVar14 = FUN_1400531f0(puVar19,L"gDASR=1");
    if (lVar14 != 0) {
      puVar19 = &DAT_1426a9d58;
      if (local_220[0] != 0) {
        puVar19 = puVar25;
      }
      if ((int)(lVar14 - (longlong)puVar19 >> 1) != -1) goto LAB_140dd1543;
    }
    bVar1 = false;
  }
  else {
    puVar19 = &DAT_1426a9d58;
    if (local_1d8[0] != 0) {
      puVar19 = puVar27;
    }
    puVar25 = local_218;
    if ((int)(lVar14 - (longlong)puVar19 >> 1) == -1) goto LAB_140dd14b4;
LAB_140dd1543:
    bVar1 = true;
  }
  if ((bVar2) && (puVar25 != (undefined2 *)0x0)) {
    FUN_140d2e720(2,puVar25);
    local_218 = (undefined2 *)0x0;
  }
  if (puVar27 != (undefined2 *)0x0) {
    FUN_140d2e720(2,puVar27);
    local_1d0 = (undefined2 *)0x0;
  }
  prVar15 = DAT_1434a0b38;
  if ((bVar1) &&
     (local_2a8[0] = (undefined8 *)
                     FUN_140d2e160(0xfffffffe,0,0x68,8,0,0,0x6190005,&DAT_1426b62fa,0,&DAT_1426b62fa
                                  ), prVar15 = prVar28, local_298 = local_2a8[0],
     local_2a8[0] != (undefined8 *)0x0)) {
    prVar15 = (rsize_t *)FUN_1400a4530();
  }
  DAT_1434a0b38 = prVar15;
  local_1e0 = 0x280;
  local_1dc = 0x1e0;
  iVar8 = GetSystemMetrics(0);
  if ((iVar8 < 0x280) || (iVar8 = GetSystemMetrics(1), iVar8 < 0x1e0)) {
    piVar17 = (int *)FUN_1400573f0(local_1a8,L"Error_ResolutionTooLow",L"Launch");
    if (*piVar17 == 0) {
      puVar27 = &DAT_1426a9d58;
    }
    else {
      puVar27 = *(undefined2 **)(piVar17 + 2);
    }
    FUN_140134470(local_1f8,0,puVar27);
    if (local_1a0 != 0) {
      FUN_140d2e720();
      local_1a0 = 0;
    }
    iVar8 = 1;
    do {
      if (DAT_1434ddf68 == 0) break;
      Sleep(100);
      iVar8 = iVar8 + 1;
    } while (iVar8 < 0x32);
    PostQuitMessage(0);
    DAT_1434a4616 = '\x01';
  }
  cVar3 = FUN_140137400();
  if (cVar3 == '\0') {
    iVar8 = 1;
    do {
      if (DAT_1434ddf68 == 0) break;
      Sleep(100);
      iVar8 = iVar8 + 1;
    } while (iVar8 < 0x32);
    PostQuitMessage(0);
    DAT_1434a4616 = '\x01';
  }
  FUN_1402dff80();
  local_2a8[0] = (undefined8 *)&DAT_1426a9d58;
  cVar3 = FUN_1400adf60(DAT_14349e308,L"Engine.Engine",L"bUseTextureStreaming",local_2a8,
                        &DAT_1434a0330);
  puVar16 = local_2a8[0];
  if (cVar3 != '\0') {
    iVar8 = _wcsicmp((wchar_t *)local_2a8[0],L"On");
    if ((((iVar8 != 0) && (iVar8 = _wcsicmp((wchar_t *)puVar16,L"True"), iVar8 != 0)) &&
        (iVar8 = _wcsicmp((wchar_t *)puVar16,L"Yes"), iVar8 != 0)) &&
       ((iVar8 = _wcsicmp((wchar_t *)puVar16,(wchar_t *)&DAT_143243600), iVar8 != 0 &&
        (iVar8 = _wcsicmp((wchar_t *)puVar16,u_True_1432433d0), iVar8 != 0)))) {
      iVar8 = _wcsicmp((wchar_t *)puVar16,L"1");
      DAT_1432766b6 = 0;
      if (iVar8 != 0) goto LAB_140dd17aa;
    }
    DAT_1432766b6 = 1;
  }
LAB_140dd17aa:
  iVar8 = GetSystemMetrics(0x1000);
  if (((DAT_14348fad4 == '\0') && (iVar8 == 0)) &&
     ((DAT_14349e329 == '\0' &&
      (lVar14 = FUN_1400531f0(&DAT_143387550,L"CookPackages"), lVar14 == 0)))) {
    FUN_1400a08b0(&local_1b8);
    local_298 = (undefined8 *)0x0;
    uStack_290 = 0;
    local_250 = L"";
    cVar3 = FUN_1400adf60(DAT_14349e308,L"AppCompat",L"CompatLevelComposite",&local_250,
                          &DAT_1434a0330);
    if (cVar3 != '\0') {
      uVar10 = _wtoi(local_250);
      local_298 = (undefined8 *)((ulonglong)local_298 & 0xffffffff00000000 | (ulonglong)uVar10);
    }
    local_228 = L"";
    cVar4 = FUN_1400adf60(DAT_14349e308,L"AppCompat",L"CompatLevelCPU",&local_228,&DAT_1434a0330);
    if (cVar4 != '\0') {
      uVar10 = _wtoi(local_228);
      local_298 = (undefined8 *)((ulonglong)local_298 & 0xffffffff | (ulonglong)uVar10 << 0x20);
    }
    local_230 = L"";
    cVar5 = FUN_1400adf60(DAT_14349e308,L"AppCompat",L"CompatLevelGPU",&local_230,&DAT_1434a0330);
    if (cVar5 != '\0') {
      uVar10 = _wtoi(local_230);
      uStack_290 = uStack_290 & 0xffffffff00000000 | (ulonglong)uVar10;
    }
    local_248 = L"";
    cVar6 = FUN_1400adf60(DAT_14349e308,L"AppCompat",L"CompatLevelVRAM",&local_248,&DAT_1434a0330);
    if (cVar6 != '\0') {
      uVar10 = _wtoi(local_248);
      uStack_290 = uStack_290 & 0xffffffff | (ulonglong)uVar10 << 0x20;
    }
    cVar7 = FUN_140055e40(&DAT_143387550,L"ReDetectCompat");
    pwVar20 = L"CompatScale=";
    lVar14 = FUN_140053100(&DAT_143387550,L"CompatScale=");
    if (lVar14 == 0) {
      bVar2 = false;
      if (cVar7 == '\0' && (((cVar3 != '\0' && cVar4 != '\0') && cVar5 != '\0') && cVar6 != '\0'))
      goto LAB_140dd1a63;
      uStack_2b0 = CONCAT44(uStack_1ac,uStack_1b0);
      uVar11 = local_1b8;
      uVar30 = uStack_1b0;
      uVar31 = uStack_1ac;
      uVar32 = uStack_1b4;
    }
    else {
      pwVar20 = (wchar_t *)local_198;
      uVar11 = wcstoul((wchar_t *)(lVar14 + 0x18),(wchar_t **)pwVar20,10);
      bVar2 = true;
      uStack_2b0 = CONCAT44(uVar11,uVar11);
      uVar30 = uVar11;
      uVar31 = uVar11;
      uVar32 = uVar11;
      if (0x7fffffff < uVar11) {
        uStack_2b0 = CONCAT44(uStack_1ac,uStack_1b0);
        uVar11 = local_1b8;
        uVar30 = uStack_1b0;
        uVar31 = uStack_1ac;
        uVar32 = uStack_1b4;
      }
    }
    local_188 = CONCAT412(uVar31,CONCAT48(uVar30,CONCAT44(uVar32,uVar11)));
    cVar3 = FUN_1400a0380(local_188,(ulonglong)pwVar20 & 0xffffffffffffff00 | (ulonglong)!bVar2);
    if ((cVar3 != '\0') && (!bVar2)) {
      uVar29 = FUN_1400afc50(extraout_XMM0_Da,L"AppCompat",L"CompatLevelComposite",uVar11,
                             &DAT_1434a0330);
      uVar29 = FUN_1400afc50(uVar29,L"AppCompat",L"CompatLevelCPU",uVar32,&DAT_1434a0330);
      uVar29 = FUN_1400afc50(uVar29,L"AppCompat",L"CompatLevelGPU",uStack_2b0 & 0xffffffff,
                             &DAT_1434a0330);
      FUN_1400afc50(uVar29,L"AppCompat",L"CompatLevelVRAM",uStack_2b0._4_4_,&DAT_1434a0330);
      FUN_1400acd50(DAT_14349e308,0,&DAT_1434a0330);
    }
  }
LAB_140dd1a63:
  cVar3 = FUN_140055e40(&DAT_143387550);
  if (cVar3 != '\0') {
    FUN_14004b8c0();
  }
  FUN_140dd3b10();
  if (DAT_1434ebf10 == 0) {
    DAT_1434ebf10 = FUN_140014580(DAT_143503060);
  }
  lVar14 = FUN_140d2e160(0xfffffffe,0,0x2f8,8,0,0,0x41d0503,&DAT_1426b62fa,0,&DAT_1426b62fa);
  prVar15 = prVar28;
  local_298 = (undefined8 *)lVar14;
  if (lVar14 != 0) {
    prVar15 = (rsize_t *)FUN_14061fca0(lVar14);
  }
  DAT_1434e0550 = prVar15;
  prVar15 = (rsize_t *)
            FUN_140d2e160(0xfffffffe,0,0x50,0x10,0,0,0x301000a,&DAT_1426b62fa,0,&DAT_1426b62fa,
                          lVar14);
  DAT_1434a43d8 = prVar28;
  if (prVar15 != (rsize_t *)0x0) {
    *prVar15 = 0;
    prVar22 = prVar15;
    while (prVar22 = prVar22 + 2, DAT_1434a43d8 = prVar15, prVar22 != prVar15 + 10) {
      *prVar22 = 0;
      prVar22[1] = 0;
    }
  }
  lVar14 = FUN_140d2e160(0xfffffffe,0,0x140,0x10,0,0,0x301000a,&DAT_1426b62fa,0,&DAT_1426b62fa,
                         lVar14);
  prVar15 = prVar28;
  if (lVar14 != 0) {
    prVar15 = (rsize_t *)FUN_14010d970(lVar14);
  }
  DAT_143500c18 = prVar15;
  *(undefined4 *)(prVar15 + 0x25) = 0;
  local_168.dwLength = 0x40;
  GlobalMemoryStatusEx(&local_168);
  DVar12 = GetCurrentProcessId();
  pvVar18 = OpenProcess(0x410,0,DVar12);
  prVar15 = prVar28;
  if (pvVar18 != (HANDLE)0x0) {
    iVar8 = K32GetProcessMemoryInfo(pvVar18,local_e8,0x48);
    if (iVar8 != 0) {
      prVar15 = local_d8;
    }
    CloseHandle(pvVar18);
  }
  DAT_143500be0 = FUN_140133130();
  local_128.dwLength = 0x40;
  GlobalMemoryStatusEx(&local_128);
  DVar12 = GetCurrentProcessId();
  pvVar18 = OpenProcess(0x410,0,DVar12);
  prVar22 = prVar28;
  if (pvVar18 != (HANDLE)0x0) {
    iVar8 = K32GetProcessMemoryInfo(pvVar18,local_98,0x48);
    if (iVar8 != 0) {
      prVar22 = local_88;
    }
    CloseHandle(pvVar18);
  }
  DAT_1434a0b48 = (longlong)prVar22 + (DAT_1434a0b48 - (longlong)prVar15);
  FUN_14003cf60();
  FUN_14003dd10();
  FUN_14003eb20();
  FUN_14003f240();
  FUN_1400401d0();
  FUN_1400420e0();
  FUN_140042540();
  FUN_140044b80();
  FUN_140040780();
  FUN_140041ed0();
  if (DAT_1434a4621 == '\0') {
    FUN_14069bbf0();
  }
  if (((DAT_14349e32a == '\0') && (DAT_1434a4613 == '\0')) &&
     (DAT_1434e0418 =
           (undefined8 *)
           FUN_140d2e160(0xfffffffe,0,8,8,0,0,0x31c000c,&DAT_1426b62fa,0,&DAT_1426b62fa),
     DAT_1434e0418 != (undefined8 *)0x0)) {
    *DAT_1434e0418 = &PTR_FUN_142a178b0;
  }
  else {
    DAT_1434e0418 = (undefined8 *)0x0;
  }
  FUN_140134ab0();
  DAT_143495af8 = 1;
  DAT_14348fad7 = 0;
  DAT_14349e32a = '\0';
  DAT_1434a4615 = 0;
  DAT_14349e32b = 1;
  if (DAT_1434a4616 != '\0') {
    if (DAT_1434e04a0 != (longlong *)0x0) {
      (**(code **)(*DAT_1434e04a0 + 0x248))();
    }
    FUN_14007dd60();
    if (DAT_1434de0f0 != 0) {
      FUN_1401327d0();
    }
    if (DAT_14348face != '\0') {
      (*DAT_1434de138)();
      DAT_14348face = '\0';
    }
    iVar21 = iVar26;
    if (DAT_14338f560 != 0) {
      FUN_14006e420();
      DAT_14338f560 = 0;
    }
    goto LAB_140dd20e9;
  }
  lVar14 = FUN_140053100(&DAT_143387550,L"DUMPMOVIE_TILEDSHOT=");
  if (lVar14 != 0) {
    DAT_143276944 = _wtoi((wchar_t *)(lVar14 + 0x28));
    lVar14 = 1;
  }
  DAT_1434a0ba5 = (char)lVar14;
  local_240 = 0;
  local_238 = 0;
  DAT_1434e1553 = DAT_1434a0ba5;
  if ((DAT_1434a0ba5 == '\0') &&
     ((cVar3 = FUN_140055e40(&DAT_143387550,L"DUMPMOVIE"), cVar3 != '\0' ||
      (cVar3 = FUN_140055f00(&DAT_143387550,L"-MATINEESSCAPTURE=",&local_240,1), cVar3 != '\0')))) {
    DAT_1434a0ba7 = 1;
LAB_140dd1e9c:
    DAT_14324350f = 0;
  }
  else {
    DAT_1434a0ba7 = 0;
    DAT_14324350f = 1;
    if (DAT_1434e1553 != '\0') goto LAB_140dd1e9c;
  }
  cVar3 = FUN_140055e40(&DAT_143387550,L"NOSCREENMESSAGES");
  if (cVar3 != '\0') {
    DAT_14324350f = 0;
  }
  if (DAT_1432786bc == '\0') {
LAB_140dd1ee8:
    DAT_1432786bc = '\0';
  }
  else {
    cVar3 = FUN_140055e40(&DAT_143387550,L"NOFORCEFEEDBACK");
    DAT_1432786bc = '\x01';
    if (cVar3 != '\0') goto LAB_140dd1ee8;
  }
  cVar3 = FUN_140055e40(&DAT_143387550,L"CaptureFPSChartInfo");
  if (cVar3 == '\0') {
    cVar3 = FUN_140055e40(&DAT_143387550,L"gCFPSCI");
    DAT_1434ddf66 = 0;
    if (cVar3 != '\0') goto LAB_140dd1f1c;
  }
  else {
LAB_140dd1f1c:
    DAT_1434ddf66 = 1;
  }
  DAT_1434a0bbf = FUN_140055e40(&DAT_143387550,L"FATALSCRIPTWARNINGS");
  DAT_1434a0bc8 = FUN_140055e40(&DAT_143387550,L"SCRIPTSTACKONWARNINGS");
  cVar3 = FUN_140055e40(&DAT_143387550,L"noThreadedAnims");
  prVar15 = (rsize_t *)(ulonglong)DAT_143276962;
  if (cVar3 != '\0') {
    prVar15 = prVar28;
  }
  DAT_143276962 = (byte)prVar15;
  if (((DAT_1434a0bbd != '\0') ||
      ((DAT_1432c7838 != -1 && (FUN_1400938b0(), DAT_1434a0bbd != '\0')))) ||
     (cVar3 = FUN_140055e40(&DAT_143387550,L"NOINI"), cVar3 != '\0')) {
    lVar14 = FUN_1400ac8d0(DAT_14349e308,&DAT_1434a0330,1);
    if (lVar14 != 0) {
      *(undefined *)(lVar14 + 0x51) = 1;
    }
    lVar14 = FUN_1400ac8d0(DAT_14349e308,&DAT_14349f330,1);
    if (lVar14 != 0) {
      *(undefined *)(lVar14 + 0x51) = 1;
    }
    lVar14 = FUN_1400ac8d0(DAT_14349e308,&DAT_14349eb30,1);
    if (lVar14 != 0) {
      *(undefined *)(lVar14 + 0x51) = 1;
    }
    lVar14 = FUN_1400ac8d0(DAT_14349e308,&DAT_14349fb30,1);
    if (lVar14 != 0) {
      *(undefined *)(lVar14 + 0x51) = 1;
    }
    lVar14 = FUN_1400ac8d0(DAT_14349e308,&DAT_14349e330,1);
    if (lVar14 != 0) {
      *(undefined *)(lVar14 + 0x51) = 1;
    }
  }
  cVar3 = FUN_140055e40(&DAT_143387550,L"noBS");
  if (((cVar3 == '\0') && (cVar3 = FUN_140055e40(&DAT_143387550,L"nosound"), cVar3 == '\0')) &&
     (DAT_1434a0bbd == '\0')) {
    uVar23 = 1;
    if (DAT_1434a4622 == '\0') {
      FUN_140d68a40();
    }
  }
  else {
    uVar23 = 0;
  }
  FUN_140dcd700(uVar23);
  FUN_1408931c0();
  FUN_140135850();
  FUN_140d2afc0(&DAT_1434fe8c0);
  uVar29 = FUN_140d2ea30(DAT_1434fd860,prVar13);
  FUN_140d2b260(&DAT_1434fe8c0);
  FUN_140071cb0(uVar29,prVar13);
  DAT_143500c10 = 0;
  if (local_238 != 0) {
    FUN_140d2e720(2);
    local_238 = 0;
  }
LAB_140dd20e9:
  if (local_280 != (wchar_t *)0x0) {
    FUN_140d2e720(2);
  }
  return iVar21;
}

