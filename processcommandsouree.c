
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Could not reconcile some variable overlaps */

bool EngineProcessCommand(undefined8 *param_1,longlong **param_2,undefined8 param_3)

{
  longlong *plVar1;
  bool bVar2;
  uint uVar3;
  char cVar4;
  byte bVar5;
  undefined uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  undefined4 uVar10;
  int iVar11;
  longlong lVar12;
  longlong **pplVar13;
  undefined8 uVar14;
  longlong *plVar15;
  int *piVar16;
  undefined8 *puVar17;
  undefined8 uVar18;
  undefined **ppuVar19;
  wchar_t *pwVar20;
  undefined4 *puVar21;
  undefined ***pppuVar22;
  undefined *puVar23;
  longlong **pplVar24;
  wchar_t *pwVar25;
  longlong **pplVar26;
  ulonglong uVar27;
  longlong **pplVar28;
  longlong lVar29;
  longlong *plVar30;
  int *piVar31;
  undefined4 *puVar32;
  undefined2 *puVar33;
  undefined8 *puVar34;
  ulonglong uVar35;
  ulonglong uVar36;
  longlong **pplVar37;
  bool bVar38;
  float fVar39;
  undefined4 extraout_XMM0_Da;
  undefined4 extraout_XMM0_Da_00;
  longlong **local_res10;
  undefined8 local_res18;
  undefined8 local_res20;
  undefined8 in_stack_fffffffffffff4a8;
  undefined8 local_b38;
  longlong **local_b30;
  undefined **local_b28;
  undefined8 local_b20;
  uint local_b18;
  ulonglong local_b10;
  undefined8 *local_b08;
  undefined8 *local_b00;
  undefined8 *local_af8;
  undefined **local_af0;
  uint local_ae8;
  undefined4 uStack_ae4;
  undefined **local_ad8;
  undefined8 *local_ad0;
  int local_ac8;
  uint local_ac4;
  uint local_ac0;
  int local_abc;
  int local_ab8;
  int local_ab4;
  int local_ab0;
  int local_aa8 [2];
  undefined8 *local_aa0;
  longlong local_a90;
  undefined4 local_a88;
  undefined8 local_a78;
  longlong local_a70;
  undefined local_a68 [16];
  undefined *local_a58;
  undefined8 local_a50;
  undefined8 local_a48;
  undefined local_a40 [8];
  undefined *local_a38;
  int local_a30;
  undefined8 local_a28;
  longlong local_a20;
  undefined4 local_a18 [4];
  undefined **local_a08;
  undefined8 local_a00;
  undefined local_9f8 [12];
  int iStack_9ec;
  undefined8 local_9e8;
  undefined8 local_9e0;
  undefined8 local_9d8;
  undefined8 local_9d0;
  undefined8 local_9c8;
  undefined **local_9b8;
  wchar_t *local_9b0;
  uint local_9a8;
  undefined8 local_9a4;
  int local_990;
  undefined2 *local_988;
  uint local_968;
  uint local_960;
  undefined8 local_898;
  undefined **local_888;
  undefined8 local_880;
  byte local_878;
  
  uVar10 = (undefined4)((ulonglong)in_stack_fffffffffffff4a8 >> 0x20);
  local_898 = 0xfffffffffffffffe;
  pplVar28 = (longlong **)0x0;
  uVar9 = 0;
  iVar8 = 0;
  local_res20 = (int *)((ulonglong)local_res20 & 0xffffffff00000000);
  local_res10 = param_2;
  local_res18 = param_3;
  local_b38 = param_2;
  cVar4 = FUN_140056770(&local_b38,L"MEMTAG_UPDATE");
  if (cVar4 != '\0') {
    lVar12 = FUN_140053100(&DAT_143387550,L"-MemTaggingNumWarmUpRounds=");
    if ((lVar12 != 0) && (iVar7 = _wtoi((wchar_t *)(lVar12 + 0x36)), iVar7 != 0)) {
      iVar8 = iVar7;
    }
    lVar12 = FUN_140053100(&DAT_143387550,L"-MemTaggingRoundsBetweenTagging=");
    if (lVar12 != 0) {
      _wtoi((wchar_t *)(lVar12 + 0x40));
    }
    DAT_1433115b0 = DAT_1433115b0 + 1;
    if (DAT_1433115b0 < iVar8) {
      return (bool)1;
    }
    lVar12 = FUN_140053100(&DAT_143387550,L"-RoundsUntilQuit=");
    if (lVar12 == 0) {
      return (bool)1;
    }
    iVar8 = _wtoi((wchar_t *)(lVar12 + 0x22));
    if (iVar8 < 1) {
      return (bool)1;
    }
    if (DAT_1433115b0 < iVar8) {
      return (bool)1;
    }
    (**(code **)DAT_1434e04a0[0xb])(DAT_1434e04a0 + 0xb,L"QUIT",PTR_DAT_143243518);
    return (bool)1;
  }
  cVar4 = FUN_140056770(&local_b38,L"REATTACHCOMPONENTS");
  uVar14 = DAT_143387528;
  if (cVar4 != '\0') {
    cVar4 = FUN_140055d20(local_b38,L"CLASS=",&local_888,0x400,1);
    if (cVar4 == '\0') {
      return (bool)1;
    }
    iVar8 = _wcsicmp((wchar_t *)&local_888,L"NONE");
    pplVar13 = pplVar28;
    pplVar24 = pplVar28;
    if ((iVar8 != 0) &&
       (pplVar13 = (longlong **)FUN_14007c690(uVar14,0xffffffffffffffff,&local_888,0),
       pplVar24 = pplVar13, pplVar13 == (longlong **)0x0)) {
      return (bool)1;
    }
    while( true ) {
      if (pplVar13 == (longlong **)0x0) {
        return (bool)1;
      }
      if (pplVar13 == DAT_1434df990) break;
      pplVar13 = (longlong **)pplVar13[0x10];
    }
    uVar27 = 0x200000000;
    if (DAT_1434a43e9 == '\0') {
      uVar27 = 0x40200000000;
    }
    pplVar13 = DAT_1434df990;
    lVar12 = maybeGObjects;
    pplVar26 = DAT_14340f9a0;
    iVar8 = DAT_1435152d0;
    if (0 < DAT_1435152d0) {
      do {
        lVar29 = *(longlong *)(maybeGObjects + (longlong)(int)pplVar28 * 8);
        if ((lVar29 != 0) &&
           ((*(ulonglong *)(lVar29 + 0x10) & uVar27 | (ulonglong)(uVar27 != 0xffffffffffffffff) - 1)
            == 0)) {
          if (pplVar24 == DAT_14340f9a0) break;
          for (pplVar37 = *(longlong ***)(lVar29 + 0x48); pplVar37 != (longlong **)0x0;
              pplVar37 = (longlong **)pplVar37[0x10]) {
            if (pplVar37 == pplVar24) goto LAB_1401fcc70;
          }
        }
        uVar9 = (int)pplVar28 + 1;
        pplVar28 = (longlong **)(ulonglong)uVar9;
      } while ((int)uVar9 < DAT_1435152d0);
    }
LAB_1401fcc70:
    do {
      uVar9 = (uint)pplVar28;
      if ((int)uVar9 < 0) {
        return (bool)1;
      }
      if (iVar8 <= (int)uVar9) {
        return (bool)1;
      }
      lVar29 = *(longlong *)(lVar12 + (longlong)(int)uVar9 * 8);
      if (lVar29 != 0) {
        if (pplVar13 == (longlong **)0x0) {
LAB_1401fccb3:
          FUN_1403aeef0(&local_af0);
          FUN_1403aefa0(&local_af0);
          pplVar26 = DAT_14340f9a0;
          lVar12 = maybeGObjects;
          pplVar13 = DAT_1434df990;
          iVar8 = DAT_1435152d0;
        }
        else {
          for (pplVar28 = *(longlong ***)(lVar29 + 0x48); pplVar28 != (longlong **)0x0;
              pplVar28 = (longlong **)pplVar28[0x10]) {
            if (pplVar28 == pplVar13) goto LAB_1401fccb3;
          }
        }
      }
      while( true ) {
        uVar9 = uVar9 + 1;
        pplVar28 = (longlong **)(ulonglong)uVar9;
        if (iVar8 <= (int)uVar9) break;
        lVar29 = *(longlong *)(lVar12 + (longlong)(int)uVar9 * 8);
        if ((lVar29 != 0) &&
           ((*(ulonglong *)(lVar29 + 0x10) & uVar27 | (ulonglong)(uVar27 != 0xffffffffffffffff) - 1)
            == 0)) {
          if (pplVar24 == pplVar26) break;
          for (pplVar37 = *(longlong ***)(lVar29 + 0x48); pplVar37 != (longlong **)0x0;
              pplVar37 = (longlong **)pplVar37[0x10]) {
            if (pplVar37 == pplVar24) goto LAB_1401fcc70;
          }
        }
      }
    } while( true );
  }
  cVar4 = FUN_140056770(&local_b38,L"MOVIE");
  if (cVar4 != '\0') {
    FUN_140056ba0(&local_b28,&local_b38,0);
    pwVar25 = L"";
    if ((int)local_b28 != 0) {
      pwVar25 = local_b20;
    }
    lVar12 = FUN_1400531f0(pwVar25,L"PLAY");
    if (lVar12 != 0) {
      pwVar25 = L"";
      if ((int)local_b28 != 0) {
        pwVar25 = local_b20;
      }
      if ((int)(lVar12 - (longlong)pwVar25 >> 1) != -1) {
        FUN_140211500(&local_af0);
        while ((-1 < (int)local_ae8 && ((int)local_ae8 < DAT_1435152d0))) {
          plVar30 = *(longlong **)(maybeGObjects + (longlong)(int)local_ae8 * 8);
          (**(code **)(*plVar30 + 0x250))(plVar30,0);
          FUN_14003bf10(&local_af0);
        }
        goto LAB_1401fced5;
      }
    }
    pwVar25 = L"";
    if ((int)local_b28 != 0) {
      pwVar25 = local_b20;
    }
    lVar12 = FUN_1400531f0(pwVar25,L"PAUSE");
    if (lVar12 != 0) {
      pwVar25 = L"";
      if ((int)local_b28 != 0) {
        pwVar25 = local_b20;
      }
      if ((int)(lVar12 - (longlong)pwVar25 >> 1) != -1) {
        FUN_140211500(&local_af0);
        while ((-1 < (int)local_ae8 && ((int)local_ae8 < DAT_1435152d0))) {
          plVar30 = *(longlong **)(maybeGObjects + (longlong)(int)local_ae8 * 8);
          (**(code **)(*plVar30 + 600))(plVar30,0);
          FUN_14003bf10(&local_af0);
        }
        goto LAB_1401fced5;
      }
    }
    pwVar25 = L"";
    if ((int)local_b28 != 0) {
      pwVar25 = local_b20;
    }
    lVar12 = FUN_1400531f0(pwVar25,L"STOP");
    if (lVar12 != 0) {
      pwVar25 = L"";
      if ((int)local_b28 != 0) {
        pwVar25 = local_b20;
      }
      if ((int)(lVar12 - (longlong)pwVar25 >> 1) != -1) {
        FUN_140211500(&local_af0);
        while ((-1 < (int)local_ae8 && ((int)local_ae8 < DAT_1435152d0))) {
          (**(code **)(**(longlong **)(maybeGObjects + (longlong)(int)local_ae8 * 8) + 0x260))();
          FUN_14003bf10(&local_af0);
        }
      }
    }
LAB_1401fced5:
    if (local_b20 != (wchar_t *)0x0) {
      FUN_140d2e720(2,local_b20);
    }
    return true;
  }
  cVar4 = FUN_140056770(&local_b38,L"DEFERRED_STOPMEMTRACKING_AND_DUMP");
  plVar30 = DAT_1434e04a0;
  if (cVar4 != '\0') {
    iVar8 = *(int *)(DAT_1434e04a0 + 0xd5);
    iVar7 = iVar8 + 1;
    *(int *)(DAT_1434e04a0 + 0xd5) = iVar7;
    if (*(int *)((longlong)plVar30 + 0x6ac) < iVar7) {
      uVar10 = FUN_1400a2df0(iVar7,*(int *)((longlong)plVar30 + 0x6ac),0x10);
      *(undefined4 *)((longlong)plVar30 + 0x6ac) = uVar10;
      FUN_14003c090(plVar30 + 0xd6);
      plVar30 = DAT_1434e04a0;
    }
    lVar12 = (longlong)iVar8 * 0x10 + plVar30[0xd6];
    if (lVar12 != 0) {
      FUN_14003cef0(lVar12,L"SNAPSHOTMEMORY");
      plVar30 = DAT_1434e04a0;
    }
    iVar8 = *(int *)(plVar30 + 0xd5);
    iVar7 = iVar8 + 1;
    *(int *)(plVar30 + 0xd5) = iVar7;
    if (*(int *)((longlong)plVar30 + 0x6ac) < iVar7) {
      uVar10 = FUN_1400a2df0(iVar7,*(int *)((longlong)plVar30 + 0x6ac),0x10);
      *(undefined4 *)((longlong)plVar30 + 0x6ac) = uVar10;
      FUN_14003c090(plVar30 + 0xd6);
      plVar30 = DAT_1434e04a0;
    }
    lVar12 = (longlong)iVar8 * 0x10 + plVar30[0xd6];
    if (lVar12 != 0) {
      FUN_14003cef0(lVar12,L"STOPTRACKING");
      plVar30 = DAT_1434e04a0;
    }
    iVar8 = *(int *)(plVar30 + 0xd5);
    iVar7 = iVar8 + 1;
    *(int *)(plVar30 + 0xd5) = iVar7;
    if (*(int *)((longlong)plVar30 + 0x6ac) < iVar7) {
      uVar10 = FUN_1400a2df0(iVar7,*(int *)((longlong)plVar30 + 0x6ac),0x10);
      *(undefined4 *)((longlong)plVar30 + 0x6ac) = uVar10;
      FUN_14003c090(plVar30 + 0xd6);
      plVar30 = DAT_1434e04a0;
    }
    lVar12 = (longlong)iVar8 * 0x10 + plVar30[0xd6];
    if (lVar12 == 0) {
      return (bool)1;
    }
    FUN_14003cef0(lVar12,L"DUMPALLOCSTOFILE");
    return (bool)1;
  }
  cVar4 = FUN_140056770(&local_b38,L"OPEN");
  pplVar13 = local_b38;
  if (cVar4 == '\0') {
    cVar4 = FUN_140056770(&local_b38,L"STREAMMAP");
    if (cVar4 != '\0') {
      FUN_14044ce10(&local_9b8,param_1 + 300,local_b38,1);
      cVar4 = FUN_14044e140(&local_9b8);
      if (cVar4 == '\0') {
        FUN_14004fcd0(param_3,L"ERROR: Can only perform streaming load for local URLs.");
      }
      else {
        local_b00 = (undefined8 *)0x0;
        local_af8 = (undefined8 *)0x0;
        puVar33 = &DAT_1426a9d58;
        if (local_990 != 0) {
          puVar33 = local_988;
        }
        cVar4 = FUN_1400d1930(DAT_1434c46d0,puVar33,0,&local_b00);
        if (cVar4 == '\0') {
          puVar33 = &DAT_1426a9d58;
          if (local_990 != 0) {
            puVar33 = local_988;
          }
          FUN_14004fcd0(param_3,L"ERROR: The map \'%s\' does not exist.",puVar33);
        }
        else {
          local_b38 = (longlong **)0x0;
          local_b30 = (longlong **)0x0;
          puVar33 = &DAT_1426a9d58;
          if (local_990 != 0) {
            puVar33 = local_988;
          }
          FUN_140063eb0(&local_res10,puVar33,0,1,1);
          uVar9 = (uint)local_b38 + 1;
          uVar27 = (ulonglong)local_b38 & 0xffffffff00000000;
          local_b38 = (longlong **)(uVar27 | uVar9);
          local_b38._4_4_ = (int)(uVar27 >> 0x20);
          if (local_b38._4_4_ < (int)uVar9) {
            uVar9 = FUN_1400a2df0(uVar9,local_b38._4_4_,8);
            local_b38 = (longlong **)((ulonglong)local_b38 & 0xffffffff | (ulonglong)uVar9 << 0x20);
            FUN_14003c110(&local_b30);
          }
          if (local_b30 + (int)(uint)local_b38 != (longlong **)0x0) {
            local_b30[(int)(uint)local_b38] = (longlong *)local_res10;
          }
          (**(code **)(param_1[-0xb] + 0x3b0))(param_1 + -0xb,&local_b38);
          *(uint *)(param_1 + 0x129) = *(uint *)(param_1 + 0x129) | 4;
          (**(code **)(param_1[-0xb] + 0x3c8))(param_1 + -0xb);
          if (local_b30 != (longlong **)0x0) {
            FUN_140d2e720(0xfffffffe);
          }
        }
        if (local_af8 != (undefined8 *)0x0) {
          FUN_140d2e720(2);
        }
      }
      FUN_140177980(&local_9b8);
      return (bool)1;
    }
    cVar4 = FUN_140056770(&local_b38,L"START");
    pplVar13 = local_b38;
    if (cVar4 == '\0') {
      cVar4 = FUN_140056770(&local_b38,L"SERVERTRAVEL");
      if ((cVar4 != '\0') && (cVar4 = FUN_14045cde0(DAT_1434dfca0), cVar4 != '\0')) {
        FUN_14003cef0(&local_b28,local_b38);
        plVar30 = **(longlong ***)(*(longlong *)(DAT_1434dfca0 + 0x80) + 0x60);
        (**(code **)(*plVar30 + 0xbe0))(plVar30,&local_b28,0,0);
joined_r0x0001401fdf63:
        if (local_b20 == (wchar_t *)0x0) {
          return (bool)1;
        }
        FUN_140d2e720(2,local_b20);
        return (bool)1;
      }
      if ((DAT_14348fad7 != '\0') &&
         ((DAT_143495af8 == '\0' &&
          (cVar4 = FUN_140056770(&local_b38,&DAT_1426fcb88), cVar4 != '\0')))) {
        FUN_14003cef0(&local_b28,local_b38);
        local_res10 = (longlong **)0x0;
        plVar30 = *(longlong **)
                   (**(longlong **)(*(longlong *)(DAT_1434dfca0 + 0x80) + 0x60) + 0x4f8);
        (**(code **)(*plVar30 + 0xda0))(plVar30,0,&local_b28,0);
        goto joined_r0x0001401fdf63;
      }
      cVar4 = FUN_140056770(&local_b38,L"DISCONNECT");
      pplVar13 = local_res10;
      if (cVar4 != '\0') {
        bVar38 = false;
        plVar30 = param_1 + -0xb;
        iVar8 = -1;
        if (plVar30 != (longlong *)0x0) {
          do {
            iVar8 = iVar8 + 1;
            if ((iVar8 < 0) || (*(int *)(param_1 + 199) <= iVar8)) break;
          } while (*(longlong *)(param_1[200] + (longlong)iVar8 * 8) == 0);
        }
        do {
          if (((plVar30 == (longlong *)0x0) || (iVar8 < 0)) || (*(int *)(param_1 + 199) <= iVar8)) {
            if (bVar38) {
              return (bool)1;
            }
            FUN_14044e400(param_1 + 300,L"Listen");
            FUN_14044e400(param_1 + 300,&DAT_1426fcbf8);
            FUN_14044e400(param_1 + 300,L"steamsockets");
            local_res10 = (longlong **)0x0;
            plVar15 = (longlong *)FUN_1402183d0(DAT_1434dfca0,0);
            if (((plVar15 != (longlong *)0x0) && (plVar15[0x10] != 0)) &&
               (plVar1 = *(longlong **)(plVar15[0x10] + 0xf48), plVar1 != (longlong *)0x0)) {
              (**(code **)(*plVar1 + 0x210))();
              (**(code **)(*(longlong *)plVar15[0x10] + 0x250))((longlong *)plVar15[0x10],1);
            }
            if (((param_1[0x128] != 0) &&
                (lVar12 = *(longlong *)(param_1[0x128] + 0xd8), lVar12 != 0)) &&
               ((lVar12 = *(longlong *)(lVar12 + 0x80), lVar12 != 0 &&
                (plVar1 = *(longlong **)(lVar12 + 0xf48), plVar1 != (longlong *)0x0)))) {
              (**(code **)(*plVar1 + 0x210))();
              plVar1 = *(longlong **)(*(longlong *)(param_1[0x128] + 0xd8) + 0x80);
              (**(code **)(*plVar1 + 0x250))(plVar1,1);
            }
            cVar4 = FUN_140056770(&local_b38,L"LOCAL");
            if (cVar4 != '\0') {
              if (plVar15 != (longlong *)0x0) {
                if (plVar15[0x10] != 0) {
                  FUN_14061a850();
                  (**(code **)(*(longlong *)plVar15[0x10] + 0x250))((longlong *)plVar15[0x10],0);
                }
                (**(code **)(*plVar15 + 0x218))(plVar15);
              }
              *(undefined8 *)(DAT_1434dfca0 + 0x128) = 0;
              return (bool)1;
            }
            (**(code **)(*plVar30 + 0x250))(plVar30,L"?closed",0);
            return (bool)1;
          }
          plVar15 = *(longlong **)(*(longlong *)(param_1[200] + (longlong)iVar8 * 8) + 0x60);
          if (plVar15 != (longlong *)0x0) {
            FUN_14003cef0(&local_b28,pplVar13);
            local_res20 = (int *)CONCAT44(local_res20._4_4_,1);
            cVar4 = (**(code **)(*plVar15 + 0x1d10))(plVar15,&local_b28);
            if ((cVar4 != '\0') || (bVar38)) {
              bVar38 = true;
            }
            if (local_b20 != (wchar_t *)0x0) {
              FUN_140d2e720(2);
              local_b20 = (wchar_t *)0x0;
            }
          }
          do {
            iVar8 = iVar8 + 1;
            if ((iVar8 < 0) || (*(int *)(param_1 + 199) <= iVar8)) break;
          } while (*(longlong *)(param_1[200] + (longlong)iVar8 * 8) == 0);
        } while( true );
      }
      cVar4 = FUN_140056770(&local_b38,L"RECONNECT");
      if (cVar4 != '\0') {
        if (*(int *)(param_1 + 0x143) == 0) {
          return (bool)1;
        }
        puVar33 = &DAT_1426a9d58;
        cVar4 = FUN_140071110(param_1 + 0x13a,&DAT_1426a9d58);
        if (cVar4 == '\0') {
          return (bool)1;
        }
        piVar16 = (int *)FUN_14044ddd0(param_1 + 0x138,&local_b28,0);
        if (*piVar16 != 0) {
          puVar33 = *(undefined2 **)(piVar16 + 2);
        }
        (**(code **)(param_1[-0xb] + 0x250))(param_1 + -0xb,puVar33,0);
        goto joined_r0x0001401fdf63;
      }
      cVar4 = FUN_140056770(&local_res10,L"EXIT");
      if ((cVar4 == '\0') && (cVar4 = FUN_140056770(&local_res10,L"QUIT"), cVar4 == '\0')) {
        cVar4 = FUN_140056770(&local_b38,L"GETMAXTICKRATE");
        if (cVar4 != '\0') {
          fVar39 = (float)(**(code **)(param_1[-0xb] + 600))(param_1 + -0xb,0,0);
          FUN_14004fcd0(param_3,&DAT_1426c23b0,(double)fVar39);
          return (bool)1;
        }
        cVar4 = FUN_140056770(&local_res10,L"CANCEL");
        if (cVar4 != '\0') {
          if (DAT_143572c04 != '\0') {
            return (bool)1;
          }
          DAT_143572c04 = 1;
          if ((longlong *)param_1[0x128] == (longlong *)0x0) {
            FUN_14003cef0(&local_ad8,&DAT_1426a9d58);
            FUN_14003cef0(&local_b28,&DAT_1426a9d58);
            (**(code **)(param_1[-0xb] + 0x260))(param_1 + -0xb,0,&local_b28,&local_ad8);
            local_aa0 = local_ad0;
            if (local_b20 != (wchar_t *)0x0) {
              FUN_140d2e720(2);
              local_b20 = (wchar_t *)0x0;
              local_aa0 = local_ad0;
            }
          }
          else {
            cVar4 = (**(code **)(*(longlong *)param_1[0x128] + 0x218))();
            if (cVar4 != '\0') {
              DAT_143572c04 = '\0';
              return (bool)1;
            }
            puVar33 = &DAT_1426a9d58;
            FUN_14003cef0(local_aa8,&DAT_1426a9d58);
            piVar16 = (int *)FUN_140057430(&local_ad8,L"CancelledConnect",L"Engine");
            if (*piVar16 != 0) {
              puVar33 = *(undefined2 **)(piVar16 + 2);
            }
            FUN_14003cef0(&local_b28,puVar33);
            (**(code **)(param_1[-0xb] + 0x260))(param_1 + -0xb,1,&local_b28,local_aa8);
            if (local_b20 != (wchar_t *)0x0) {
              FUN_140d2e720(2);
              local_b20 = (wchar_t *)0x0;
            }
            if (local_ad0 != (undefined8 *)0x0) {
              FUN_140d2e720(2);
              local_ad0 = (undefined8 *)0x0;
            }
          }
          if (local_aa0 != (undefined8 *)0x0) {
            FUN_140d2e720(2);
          }
          (**(code **)(param_1[-0xb] + 0x398))(param_1 + -0xb);
          DAT_143572c04 = '\0';
          return (bool)1;
        }
        cVar4 = FUN_140056770(&local_res10,L"ListPrecacheMapPackages");
        if (cVar4 != '\0') {
          local_b10 = 0;
          local_b08 = (undefined8 *)0x0;
          FUN_1401112c0(&local_b10);
          puVar17 = local_b08;
          uVar27 = local_b10;
          FUN_140215990(local_b08,local_b10 & 0xffffffff);
          FUN_14004fcd0(param_3,L"Total Number Of Packages In PrecacheMap: %i ",uVar27 & 0xffffffff)
          ;
          if (0 < (int)local_b10) {
            do {
              if (*(int *)puVar17 == 0) {
                puVar33 = &DAT_1426a9d58;
              }
              else {
                puVar33 = (undefined2 *)puVar17[1];
              }
              FUN_14004fcd0(param_3,L"%i %s",pplVar28,puVar33);
              uVar9 = (int)pplVar28 + 1;
              pplVar28 = (longlong **)(ulonglong)uVar9;
              puVar17 = puVar17 + 2;
            } while ((int)uVar9 < (int)local_b10);
          }
          FUN_14004fcd0(param_3,L"Total Number Of Packages In PrecacheMap: %i ",uVar27 & 0xffffffff)
          ;
          FUN_1400397d0(&local_b10);
          return (bool)1;
        }
        cVar4 = FUN_140056770(&local_res10,L"ListLoadedPackages");
        iVar7 = 0;
        if (cVar4 != '\0') {
          local_b38 = (longlong **)0x0;
          local_b30 = (longlong **)0x0;
          FUN_140211590(&local_af0);
          pplVar24 = pplVar28;
          pplVar26 = pplVar28;
          pplVar13 = pplVar28;
          while( true ) {
            iVar8 = (int)pplVar24;
            if (((int)local_ae8 < 0) || (DAT_1435152d0 <= (int)local_ae8)) break;
            lVar12 = *(longlong *)(maybeGObjects + (longlong)(int)local_ae8 * 8);
            if (*(longlong *)(lVar12 + 0x38) == 0) {
              puVar17 = (undefined8 *)FUN_140074b50(lVar12,&local_b28,0);
              local_b38._0_4_ = iVar8 + 1;
              local_b38 = (longlong **)
                          ((ulonglong)local_b38 & 0xffffffff00000000 | (ulonglong)(uint)local_b38);
              if ((int)pplVar13 < (int)(uint)local_b38) {
                uVar9 = FUN_1400a2df0((uint)local_b38,pplVar13,0x10);
                local_b38 = (longlong **)
                            ((ulonglong)local_b38 & 0xffffffff | (ulonglong)uVar9 << 0x20);
                FUN_14003c090(&local_b30);
                pplVar13 = (longlong **)((ulonglong)local_b38 >> 0x20);
                pplVar26 = local_b30;
              }
              pplVar24 = (longlong **)(ulonglong)(uint)local_b38;
              pplVar37 = pplVar26 + (longlong)iVar8 * 2;
              if (pplVar37 != (longlong **)0x0) {
                *(int *)pplVar37 = *(int *)puVar17;
                *(int *)((longlong)pplVar37 + 4) = *(int *)((longlong)puVar17 + 4);
                pplVar37[1] = (longlong *)puVar17[1];
                puVar17[1] = 0;
                *puVar17 = 0;
              }
              if (local_b20 != (wchar_t *)0x0) {
                FUN_140d2e720(2);
                local_b20 = (wchar_t *)0x0;
              }
            }
            FUN_14003bf10(&local_af0);
          }
          FUN_140215990(pplVar26,pplVar24);
          FUN_14004fcd0(param_3,L"Total Number Of Packages Loaded: %i ",pplVar24);
          if (0 < iVar8) {
            do {
              if (*(int *)pplVar26 == 0) {
                plVar30 = (longlong *)&DAT_1426a9d58;
              }
              else {
                plVar30 = pplVar26[1];
              }
              FUN_14004fcd0(param_3,L"%4i %s",pplVar28,plVar30);
              uVar9 = (int)pplVar28 + 1;
              pplVar28 = (longlong **)(ulonglong)uVar9;
              pplVar26 = pplVar26 + 2;
            } while ((int)uVar9 < iVar8);
          }
          FUN_14004fcd0(param_3,L"Total Number Of Packages Loaded: %i ",pplVar24);
          FUN_1400397d0(&local_b38);
          return (bool)1;
        }
        cVar4 = FUN_140056770(&local_res10,L"MemReport");
        if (cVar4 == '\0') {
          cVar4 = FUN_140056770(&local_res10,L"MemLeakCheck");
          if (cVar4 != '\0') {
            cVar4 = FUN_140055e40(local_b38,L"FAST");
            plVar30 = DAT_1434e04a0;
            iVar8 = *(int *)(DAT_1434e04a0 + 0xd5);
            iVar7 = iVar8 + 1;
            *(int *)(DAT_1434e04a0 + 0xd5) = iVar7;
            if (*(int *)((longlong)plVar30 + 0x6ac) < iVar7) {
              uVar10 = FUN_1400a2df0(iVar7,*(int *)((longlong)plVar30 + 0x6ac),0x10);
              *(undefined4 *)((longlong)plVar30 + 0x6ac) = uVar10;
              FUN_14003c090(plVar30 + 0xd6);
              plVar30 = DAT_1434e04a0;
            }
            lVar12 = (longlong)iVar8 * 0x10 + plVar30[0xd6];
            if (lVar12 == 0) {
              return (bool)1;
            }
            pwVar25 = L"MemLeakCheckPostGC";
            if (cVar4 != '\0') {
              pwVar25 = L"MemLeakCheckPostGC -FAST";
            }
            FUN_14003cef0(lVar12,pwVar25);
            return (bool)1;
          }
          cVar4 = FUN_140056770(&local_res10,L"MemLeakCheckPostGC");
          if (cVar4 == '\0') {
            cVar4 = FUN_140056770(&local_res10,L"MEMORYSPLIT");
            if (cVar4 == '\0') {
              cVar4 = FUN_140056770(&local_res10,L"PARTICLEMESHUSAGE");
              if (cVar4 != '\0') {
                local_a78 = 0;
                local_a70 = 0;
                local_a58 = (undefined *)0x0;
                local_a50 = 0;
                local_a48 = 0;
                local_a38 = (undefined *)0x0;
                local_a30 = 0;
                local_b38 = (longlong **)0x0;
                local_b30 = (longlong **)0x0;
                FUN_1402118e0(&local_af0);
                do {
                  pplVar13 = local_b30;
                  if (((int)local_ae8 < 0) || (DAT_1435152d0 <= (int)local_ae8)) {
                    uVar27 = (ulonglong)local_b38 & 0xffffffff;
                    local_res10 = local_b30;
                    if ((local_b30 != (longlong **)0x0) && (0 < (int)(uint)local_b38)) {
                      FUN_1402169c0(local_b30,local_b30 + (int)(uint)local_b38,
                                    (longlong)(local_b30 + (int)(uint)local_b38) -
                                    (longlong)local_b30 >> 3,0);
                    }
                    pplVar24 = pplVar13;
                    uVar36 = uVar27;
                    if (0 < (int)(uint)local_b38) {
                      do {
                        iVar8 = (**(code **)(**pplVar24 + 0x1a8))();
                        uVar9 = (int)pplVar28 + iVar8;
                        pplVar28 = (longlong **)(ulonglong)uVar9;
                        uVar36 = uVar36 - 1;
                        pplVar24 = pplVar24 + 1;
                      } while (uVar36 != 0);
                    }
                    FUN_14004fcd0(param_3,
                                  L"%5i KByte of static meshes referenced by particle systems:",
                                  (int)(uVar9 + ((int)uVar9 >> 0x1f & 0x3ffU)) >> 10);
                    if (0 < (int)(uint)local_b38) {
                      do {
                        plVar30 = *pplVar13;
                        local_b10 = 0;
                        local_b08 = (undefined8 *)0x0;
                        local_res20 = (int *)uVar27;
                        FUN_140215a40(&local_a78,plVar30,&local_b10);
                        lVar12 = FUN_14003c600(&local_888);
                        FUN_140074900(plVar30);
                        uVar14 = *(undefined8 *)(lVar12 + 8);
                        iVar8 = (**(code **)(*plVar30 + 0x1a8))(plVar30);
                        FUN_14004fcd0(param_3,L"%5i KByte  %s",
                                      (int)((iVar8 >> 0x1f & 0x3ffU) + iVar8) >> 10,uVar14);
                        local_888 = &PTR_FUN_1426ce7a0;
                        if ((local_878 & 1) != 0) {
                          FUN_140d2e720(2,local_880);
                        }
                        if (0 < (int)local_b10) {
                          uVar36 = local_b10 & 0xffffffff;
                          puVar17 = local_b08;
                          do {
                            uVar14 = *puVar17;
                            lVar12 = FUN_14003c600(&local_888);
                            FUN_140074900(uVar14);
                            FUN_14004fcd0(param_3,L"             %s",*(undefined8 *)(lVar12 + 8));
                            local_888 = &PTR_FUN_1426ce7a0;
                            if ((local_878 & 1) != 0) {
                              FUN_140d2e720(2,local_880);
                            }
                            puVar17 = puVar17 + 1;
                            uVar36 = uVar36 - 1;
                            uVar27 = (ulonglong)local_res20;
                          } while (uVar36 != 0);
                        }
                        FUN_14003c6d0(&local_b08);
                        pplVar13 = local_res10 + 1;
                        uVar27 = uVar27 - 1;
                        local_res10 = pplVar13;
                        local_res20 = (int *)uVar27;
                      } while (uVar27 != 0);
                    }
                    FUN_14003c6d0(&local_b30);
                    FUN_140200a50(&local_a78);
                    return (bool)1;
                  }
                  lVar12 = *(longlong *)(maybeGObjects + (longlong)(int)local_ae8 * 8);
                  local_res20 = *(int **)(lVar12 + 0x60);
                  if (local_res20 != (int *)0x0) {
                    local_res10 = (longlong **)0x0;
                    lVar12 = *(longlong *)(lVar12 + 0x38);
                    if (lVar12 != 0) {
                      do {
                        if (local_res10 != (longlong **)0x0) goto LAB_1401ff6b2;
                        local_res10 = (longlong **)FUN_140215a00(lVar12);
                        lVar12 = *(longlong *)(lVar12 + 0x38);
                      } while (lVar12 != 0);
                      if (local_res10 != (longlong **)0x0) {
LAB_1401ff6b2:
                        FUN_140211620(&local_a78,&local_res20,&local_res10);
                        FUN_140211830(&local_b38,&local_res20);
                      }
                    }
                  }
                  FUN_14003bf10();
                } while( true );
              }
              cVar4 = FUN_140056770(&local_res10,L"PARTICLEMEMORY");
              if (cVar4 != '\0') {
                local_b38 = (longlong **)0x0;
                local_b30 = (longlong **)0x0;
                FUN_1402119a0(local_aa8);
                pplVar13 = pplVar28;
                pplVar24 = pplVar28;
                pplVar26 = pplVar28;
                while( true ) {
                  iVar8 = (int)pplVar28;
                  iVar11 = (int)pplVar26;
                  iVar7 = (int)pplVar13;
                  if (((int)local_aa0 < 0) || (DAT_1435152d0 <= (int)local_aa0)) break;
                  lVar12 = *(longlong *)(maybeGObjects + (longlong)(int)local_aa0 * 8);
                  pplVar24 = (longlong **)(ulonglong)((int)pplVar24 + 1);
                  FUN_14009f6f0(&local_9b8,lVar12);
                  uVar9 = local_960;
                  pplVar28 = (longlong **)(ulonglong)local_960;
                  if (iVar7 < (int)local_960) {
                    if (lVar12 == 0) {
                      uVar14 = FUN_14003cef0(&local_ad8,L"No Component!");
                      FUN_14003a0f0(&local_b38,uVar14);
                      pppuVar22 = &local_ad8;
                    }
                    else if (*(longlong *)(lVar12 + 0x370) == 0) {
                      uVar14 = FUN_14003cef0(&local_b28,L"No Template!");
                      FUN_14003a0f0(&local_b38,uVar14);
                      pppuVar22 = &local_b28;
                    }
                    else {
                      uVar14 = FUN_140074840(*(longlong *)(lVar12 + 0x370),&local_af0,0);
                      FUN_14003a0f0(&local_b38,uVar14);
                      pppuVar22 = &local_af0;
                    }
                    FUN_14003ceb0(pppuVar22);
                    pplVar13 = pplVar28;
                  }
                  pplVar28 = (longlong **)(ulonglong)(iVar8 + uVar9);
                  pplVar26 = (longlong **)(ulonglong)(iVar11 + local_968);
                  FUN_14003bf10(local_aa8);
                }
                FUN_14004fcd0(param_3,L"PSysComp Count     : %6i",pplVar24);
                FUN_14004fcd0(param_3,L"Particles (Active) : %6i",
                              (int)((iVar11 >> 0x1f & 0x3ffU) + iVar11) >> 10);
                FUN_14004fcd0(param_3,L"Particles (Peak)   : %6i",
                              (int)((iVar8 >> 0x1f & 0x3ffU) + iVar8) >> 10);
                FUN_14004fcd0(param_3,L"Particles (Single) : %6i",
                              (int)(iVar7 + (iVar7 >> 0x1f & 0x3ffU)) >> 10);
                pplVar28 = (longlong **)&DAT_1426a9d58;
                if ((uint)local_b38 != 0) {
                  pplVar28 = local_b30;
                }
                FUN_14004fcd0(param_3,L"    PSys           : %s",pplVar28);
                FUN_14003ceb0(&local_b38);
                return (bool)1;
              }
              cVar4 = FUN_140056770(&local_res10,L"DUMPPARTICLECOUNTS");
              if (cVar4 == '\0') {
                cVar4 = FUN_140056770(&local_res10,L"LISTGFXRESOURCES");
                if (cVar4 != '\0') {
                  FUN_14004fcd0(param_3,L"Listing referenced GFx resources");
                  FUN_140211a30(local_aa8);
                  while( true ) {
                    uVar27 = 0;
                    if ((int)local_aa0 < 0) {
                      return (bool)1;
                    }
                    if (DAT_1435152d0 <= (int)local_aa0) break;
                    local_res10 = (longlong **)((ulonglong)local_res10 & 0xffffffff00000000);
                    lVar12 = *(longlong *)(maybeGObjects + (longlong)(int)local_aa0 * 8);
                    uVar14 = FUN_14003c600(&local_888);
                    local_res20 = (int *)FUN_140038cc0(*(undefined8 *)(lVar12 + 0x48),&local_af0);
                    lVar29 = FUN_14003ae50(lVar12,uVar14);
                    if (*local_res20 == 0) {
                      puVar33 = &DAT_1426a9d58;
                    }
                    else {
                      puVar33 = *(undefined2 **)(local_res20 + 2);
                    }
                    FUN_14004fcd0(param_3,L"%s(%s)",puVar33,*(undefined8 *)(lVar29 + 8));
                    FUN_14003cd90(&local_af0);
                    local_888 = &PTR_FUN_1426ce7a0;
                    if ((local_878 & 1) != 0) {
                      FUN_140d2e720(2,local_880);
                    }
                    FUN_14004fcd0(param_3,L"%d References",*(undefined4 *)(lVar12 + 0x78));
                    FUN_14004fcd0(param_3,L"\tClass\tName\tSize");
                    local_res20 = (int *)((ulonglong)local_res20 & 0xffffffff00000000);
                    uVar36 = uVar27;
                    if (0 < *(int *)(lVar12 + 0x78)) {
                      local_res10 = (longlong **)0x0;
                      do {
                        plVar30 = *(longlong **)(*(longlong *)(lVar12 + 0x80) + uVar27);
                        if (plVar30 != (longlong *)0x0) {
                          uVar9 = (**(code **)(*plVar30 + 0x1a8))(plVar30);
                          local_res10 = (longlong **)
                                        ((ulonglong)local_res10 & 0xffffffff00000000 |
                                        (ulonglong)uVar9);
                          uVar36 = (ulonglong)((int)uVar36 + uVar9);
                          uVar14 = FUN_14003c600(&local_888);
                          local_b38 = (longlong **)FUN_140038cc0(plVar30[9],&local_b28);
                          lVar29 = FUN_14003ae50(plVar30,uVar14);
                          if (*(int *)local_b38 == 0) {
                            plVar30 = (longlong *)&DAT_1426a9d58;
                          }
                          else {
                            plVar30 = local_b38[1];
                          }
                          FUN_14004fcd0(param_3,L"\t%s\t%s\t%0.2f KB",plVar30,
                                        *(undefined8 *)(lVar29 + 8),
                                        (double)((float)((ulonglong)local_res10 & 0xffffffff) *
                                                0.0009765625));
                          FUN_14003cd90(&local_b28);
                          local_888 = &PTR_FUN_1426ce7a0;
                          if ((local_878 & 1) != 0) {
                            FUN_140d2e720(2,local_880);
                          }
                        }
                        local_res20 = (int *)((ulonglong)local_res20 & 0xffffffff00000000 |
                                             (ulonglong)((uint)local_res20 + 1));
                        uVar27 = uVar27 + 8;
                      } while ((int)((uint)local_res20 + 1) < *(int *)(lVar12 + 0x78));
                      local_res10 = (longlong **)
                                    ((ulonglong)local_res10 & 0xffffffff00000000 | uVar36);
                    }
                    uVar35 = 0;
                    FUN_14004fcd0(param_3,L"%d User References",*(undefined4 *)(lVar12 + 0x88));
                    FUN_14004fcd0(param_3,L"\tClass\tName\tSize");
                    uVar27 = uVar35;
                    if (0 < *(int *)(lVar12 + 0x88)) {
                      do {
                        plVar30 = *(longlong **)(*(longlong *)(lVar12 + 0x90) + uVar27);
                        if (plVar30 != (longlong *)0x0) {
                          uVar9 = (**(code **)(*plVar30 + 0x1a8))(plVar30);
                          local_res20 = (int *)((ulonglong)local_res20 & 0xffffffff00000000 |
                                               (ulonglong)uVar9);
                          local_res10 = (longlong **)
                                        ((ulonglong)local_res10 & 0xffffffff00000000 |
                                        (ulonglong)((int)local_res10 + uVar9));
                          uVar14 = FUN_14003c600(&local_888);
                          local_b38 = (longlong **)FUN_140038cc0(plVar30[9],&local_ad8);
                          lVar29 = FUN_14003ae50(plVar30,uVar14);
                          if (*(int *)local_b38 == 0) {
                            plVar30 = (longlong *)&DAT_1426a9d58;
                          }
                          else {
                            plVar30 = local_b38[1];
                          }
                          FUN_14004fcd0(param_3,L"\t%s\t%s\t%0.2f KB",plVar30,
                                        *(undefined8 *)(lVar29 + 8),
                                        (double)((float)((ulonglong)local_res20 & 0xffffffff) *
                                                0.0009765625));
                          FUN_14003cd90(&local_ad8);
                          local_888 = &PTR_FUN_1426ce7a0;
                          if ((local_878 & 1) != 0) {
                            FUN_140d2e720(2,local_880);
                          }
                        }
                        uVar9 = (int)uVar35 + 1;
                        uVar35 = (ulonglong)uVar9;
                        uVar27 = uVar27 + 8;
                      } while ((int)uVar9 < *(int *)(lVar12 + 0x88));
                      uVar36 = (ulonglong)local_res10 & 0xffffffff;
                    }
                    FUN_14004fcd0(param_3,L"Total Resource Size: %0.2f KB",
                                  (double)((float)uVar36 * 0.0009765625));
                    FUN_14003bf10(local_aa8);
                  }
                  return (bool)1;
                }
                cVar4 = FUN_140056770(&local_res10,L"DUMPGFXMEM");
                if (cVar4 != '\0') {
                  FUN_140c8cbb0(param_3);
                  return (bool)1;
                }
                cVar4 = FUN_140056770(&local_res10,L"MeshesWithCollision");
                if (cVar4 != '\0') {
                  FUN_14003bfa0(&local_af0,DAT_14340f9a0,0);
                  iVar8 = DAT_1435152d0;
                  while( true ) {
                    if ((int)local_ae8 < 0) {
                      return (bool)1;
                    }
                    if (iVar8 <= (int)local_ae8) break;
                    FUN_14003bf10(&local_af0);
                  }
                  return (bool)1;
                }
                cVar4 = FUN_140056770(&local_res10,&DAT_1426fd970);
                if (cVar4 != '\0') {
                  plVar30 = (longlong *)FUN_1402f7060();
                  if (plVar30 == (longlong *)0x0) {
                    FUN_14004fcd0(param_3,
                                  L"No Ad Manager has been created. See [Engine.GameEngine] : InGame AdManagerClassName"
                                 );
                    return (bool)1;
                  }
                  pwVar25 = L"Show";
                  cVar4 = FUN_140056770(&local_res10,L"Show");
                  if (cVar4 != '\0') {
                    iVar8 = _wtoi((wchar_t *)local_res10);
                    (**(code **)(*plVar30 + 0x230))
                              (plVar30,(ulonglong)pwVar25 & 0xffffffffffffff00 |
                                       (ulonglong)(iVar8 != 0));
                    return (bool)1;
                  }
                  cVar4 = FUN_140056770(&local_res10,L"Hide");
                  if (cVar4 != '\0') {
                    (**(code **)(*plVar30 + 0x238))(plVar30);
                    return (bool)1;
                  }
                  cVar4 = FUN_140056770(&local_res10,L"Close");
                  if (cVar4 == '\0') {
                    return (bool)1;
                  }
                  (**(code **)(*plVar30 + 0x240))(plVar30);
                  return (bool)1;
                }
                cVar4 = FUN_140056770(&local_res10,&DAT_1426fda84);
                if (cVar4 == '\0') {
                  cVar4 = FUN_140056770(&local_res10,L"FREELISTSTATS");
                  pplVar28 = local_res10;
                  if (cVar4 != '\0') {
                    return (bool)1;
                  }
                  if (DAT_1434dfca0 != 0) {
                    cVar4 = FUN_140457010(extraout_XMM0_Da_00,local_res10,param_3);
                    if (cVar4 != '\0') {
                      return (bool)1;
                    }
                    if ((DAT_1434dfca0 != 0) &&
                       (lVar12 = FUN_14045a9b0(DAT_1434dfca0,0), *(longlong *)(lVar12 + 0x4f8) != 0)
                       ) {
                      lVar12 = FUN_14045a9b0(DAT_1434dfca0,0);
                      cVar4 = FUN_140077510(*(undefined8 *)(lVar12 + 0x4f8),pplVar28,param_3,0);
                      if (cVar4 != '\0') {
                        return (bool)1;
                      }
                    }
                  }
                  if (((param_1[0x128] != 0) &&
                      (lVar12 = *(longlong *)(param_1[0x128] + 0xd8), lVar12 != 0)) &&
                     (puVar17 = (undefined8 *)(lVar12 + 0x58),
                     cVar4 = (**(code **)*puVar17)(puVar17,pplVar28,param_3), cVar4 != '\0')) {
                    return (bool)1;
                  }
                  cVar4 = FUN_140056770(&local_b38,&DAT_1426bdcc8);
                  if ((cVar4 != '\0') ||
                     (cVar4 = FUN_140056770(&local_b38,L"SETNOPEC"), cVar4 != '\0')) {
                    if (param_1[0x128] != 0) {
                      return (bool)1;
                    }
                    lVar12 = FUN_14045a9b0(DAT_1434dfca0,0);
                    if (*(char *)(lVar12 + 0x4c0) != '\0') {
                      return (bool)1;
                    }
                    DAT_1434de4dc = 1;
                  }
                  cVar4 = FUN_1405dbd90(param_1,pplVar28,param_3);
                  return cVar4 != '\0';
                }
                if (DAT_1434e04a0[0x7e] == 0) {
                  FUN_14004fcd0(param_3,L"This platform has no Facebook support");
                  return (bool)1;
                }
                cVar4 = FUN_140056770(&local_res10,L"auth");
                if (cVar4 != '\0') {
                  (**(code **)(*(longlong *)DAT_1434e04a0[0x7e] + 0x238))();
                  return (bool)1;
                }
                cVar4 = FUN_140056770(&local_res10,L"isauthed");
                if (cVar4 != '\0') {
                  uVar6 = (**(code **)(*(longlong *)DAT_1434e04a0[0x7e] + 0x240))();
                  FUN_14004fcd0(param_3,L"Authorized? %d",uVar6);
                  return (bool)1;
                }
                cVar4 = FUN_140056770(&local_res10,L"username");
                if (cVar4 != '\0') {
                  if (*(int *)(DAT_1434e04a0[0x7e] + 0x68) == 0) {
                    puVar33 = &DAT_1426a9d58;
                  }
                  else {
                    puVar33 = *(undefined2 **)(DAT_1434e04a0[0x7e] + 0x70);
                  }
                  FUN_14004fcd0(param_3,L"FB username is %s",puVar33);
                  return (bool)1;
                }
                cVar4 = FUN_140056770(&local_res10,L"disconnect");
                if (cVar4 == '\0') {
                  return (bool)1;
                }
                (**(code **)(*(longlong *)DAT_1434e04a0[0x7e] + 600))();
                return (bool)1;
              }
              local_a78 = 0;
              local_a70 = 0;
              local_a58 = (undefined *)0x0;
              local_a50 = 0;
              local_a48 = 0;
              local_a38 = (undefined *)0x0;
              local_a30 = 0;
              bVar5 = FUN_140056770(&local_res10,L"USAGE");
              local_res20 = (int *)((ulonglong)local_res20 & 0xffffffffffffff00 | (ulonglong)bVar5);
              bVar5 = FUN_140056770(&local_res10);
              local_res10 = (longlong **)
                            ((ulonglong)local_res10 & 0xffffffffffffff00 | (ulonglong)bVar5);
              FUN_14009fa10(&local_af0);
              while ((-1 < (int)local_ae8 && ((int)local_ae8 < DAT_1435152d0))) {
                lVar12 = FUN_1402159c0(*(undefined8 *)(maybeGObjects + (longlong)(int)local_ae8 * 8)
                                      );
                iVar8 = local_a30;
                if (lVar12 != 0) {
                  ppuVar19 = *(undefined ***)(lVar12 + 0x370);
                  pplVar13 = pplVar28;
                  if (ppuVar19 == (undefined **)0x0) {
LAB_1401ffcb3:
                  }
                  else if (((byte)local_res20 != '\0') || ((byte)local_res10 != '\0')) {
                    if (local_a30 != 0) {
                      uVar9 = FUN_14003bc10(ppuVar19);
                      puVar23 = local_a40;
                      if (local_a38 != (undefined *)0x0) {
                        puVar23 = local_a38;
                      }
                      for (iVar8 = *(int *)(puVar23 + (longlong)(int)(iVar8 - 1U & uVar9) * 4);
                          iVar8 != -1; iVar8 = *(int *)((longlong)iVar8 * 0x58 + 0x50 + local_a70))
                      {
                        if (*(undefined ***)((longlong)iVar8 * 0x58 + local_a70) == ppuVar19) {
                          if (((iVar8 != -1) &&
                              (lVar29 = (longlong)iVar8 * 0x58 + local_a70, lVar29 != 0)) &&
                             (pplVar13 = (longlong **)(lVar29 + 8), pplVar13 != (longlong **)0x0)) {
                            *(int *)(lVar29 + 0x10) = *(int *)(lVar29 + 0x10) + 1;
                            goto LAB_1401ffcab;
                          }
                          break;
                        }
                      }
                    }
                    local_a00 = 1;
                    _local_9f8 = ZEXT816(0);
                    local_9e8 = 0;
                    local_9e0 = 0;
                    local_9d8 = 0;
                    local_9d0 = 0;
                    local_9c8 = 0;
                    local_a08 = ppuVar19;
                    local_9b8 = ppuVar19;
                    FUN_140212a50(&local_9b0,&local_a08);
                    FUN_140212720(&local_a78,&local_b38,&local_9b8);
                    FUN_1401fc9a0(&local_9b0);
                    iVar8 = local_a30;
                    pplVar13 = pplVar28;
                    if (local_a30 != 0) {
                      uVar9 = FUN_14003bc10(ppuVar19);
                      puVar23 = local_a40;
                      if (local_a38 != (undefined *)0x0) {
                        puVar23 = local_a38;
                      }
                      for (iVar8 = *(int *)(puVar23 + (longlong)(int)(iVar8 - 1U & uVar9) * 4);
                          iVar8 != -1; iVar8 = *(int *)((longlong)iVar8 * 0x58 + 0x50 + local_a70))
                      {
                        if (*(undefined ***)((longlong)iVar8 * 0x58 + local_a70) == ppuVar19) {
                          if ((iVar8 != -1) &&
                             (lVar29 = (longlong)iVar8 * 0x58 + local_a70, lVar29 != 0)) {
                            pplVar13 = (longlong **)(lVar29 + 8);
                          }
                          break;
                        }
                      }
                    }
                    FUN_1401fc9a0(&local_a08);
LAB_1401ffcab:
                    goto LAB_1401ffcb3;
                  }
                  pplVar24 = pplVar28;
                  pplVar26 = pplVar28;
                  pplVar37 = pplVar28;
                  if (*(int *)(lVar12 + 0x390) < 1) {
                    if ((ppuVar19 != (undefined **)0x0) && (0 < *(int *)(ppuVar19 + 0xe))) {
                      local_a90 = 0;
                      do {
                        lVar12 = *(longlong *)(ppuVar19[0xf] + local_a90);
                        iVar8 = (int)pplVar26;
                        if (lVar12 != 0) {
                          iVar11 = iVar7;
                          if (0 < (int)*(uint *)(lVar12 + 0x70)) {
                            plVar30 = *(longlong **)(lVar12 + 0x78);
                            uVar27 = (ulonglong)*(uint *)(lVar12 + 0x70);
                            pplVar26 = pplVar28;
                            do {
                              if ((*plVar30 != 0) &&
                                 (uVar9 = *(uint *)(*plVar30 + 0xf0), (int)pplVar26 < (int)uVar9)) {
                                pplVar26 = (longlong **)(ulonglong)uVar9;
                              }
                              iVar11 = (int)pplVar26;
                              plVar30 = plVar30 + 1;
                              uVar27 = uVar27 - 1;
                            } while (uVar27 != 0);
                          }
                          if (((byte)local_res20 != '\0') || ((byte)local_res10 != '\0')) {
                            *(int *)((longlong)pplVar13 + 0x14) =
                                 *(int *)((longlong)pplVar13 + 0x14) + iVar11;
                            if (*(int *)(pplVar13 + 7) == iVar8 || *(int *)(pplVar13 + 7) < iVar8) {
                              FUN_14011cb20(pplVar13 + 3,1);
                              FUN_14011cb20(pplVar13 + 5,1);
                              FUN_14011cb20(pplVar13 + 7);
                            }
                            *(int *)((longlong)pplVar13[8] + (longlong)pplVar24) =
                                 *(int *)((longlong)pplVar13[8] + (longlong)pplVar24) + iVar11;
                          }
                        }
                        pplVar26 = (longlong **)(ulonglong)(iVar8 + 1U);
                        local_a90 = local_a90 + 8;
                        pplVar24 = (longlong **)((longlong)pplVar24 + 4);
                      } while ((int)(iVar8 + 1U) < *(int *)(ppuVar19 + 0xe));
                    }
                  }
                  else {
                    do {
                      lVar29 = *(longlong *)((longlong)pplVar26 + *(longlong *)(lVar12 + 0x398));
                      iVar8 = (int)pplVar37;
                      if ((lVar29 != 0) &&
                         (((byte)local_res20 != '\0' || ((byte)local_res10 != '\0')))) {
                        *(int *)((longlong)pplVar13 + 0xc) =
                             *(int *)((longlong)pplVar13 + 0xc) + *(int *)(lVar29 + 0xc0);
                        iVar11 = iVar7;
                        if (*(ulonglong *)(lVar29 + 0x30) != 0) {
                          iVar11 = (int)((ulonglong)(longlong)*(int *)(lVar29 + 0x68) /
                                        *(ulonglong *)(lVar29 + 0x30));
                        }
                        *(int *)(pplVar13 + 2) = *(int *)(pplVar13 + 2) + iVar11;
                        iVar11 = iVar7;
                        if (*(ulonglong *)(lVar29 + 0x30) != 0) {
                          iVar11 = (int)((ulonglong)(longlong)*(int *)(lVar29 + 0x68) /
                                        *(ulonglong *)(lVar29 + 0x30));
                        }
                        *(int *)((longlong)pplVar13 + 0x14) =
                             *(int *)((longlong)pplVar13 + 0x14) + iVar11;
                        pplVar37 = pplVar13 + 3;
                        if (*(int *)pplVar37 == iVar8 || *(int *)pplVar37 < iVar8) {
                          FUN_14011cb20(pplVar37,1);
                          FUN_14011cb20(pplVar13 + 5,1);
                          FUN_14011cb20(pplVar13 + 7);
                        }
                        *(int *)((longlong)pplVar13[4] + (longlong)pplVar24) =
                             *(int *)((longlong)pplVar13[4] + (longlong)pplVar24) +
                             *(int *)(lVar29 + 0xc0);
                        iVar11 = iVar7;
                        if (*(ulonglong *)(lVar29 + 0x30) != 0) {
                          iVar11 = (int)((ulonglong)(longlong)*(int *)(lVar29 + 0x68) /
                                        *(ulonglong *)(lVar29 + 0x30));
                        }
                        *(int *)((longlong)pplVar13[6] + (longlong)pplVar24) =
                             *(int *)((longlong)pplVar13[6] + (longlong)pplVar24) + iVar11;
                        iVar11 = iVar7;
                        if (*(ulonglong *)(lVar29 + 0x30) != 0) {
                          iVar11 = (int)((ulonglong)(longlong)*(int *)(lVar29 + 0x68) /
                                        *(ulonglong *)(lVar29 + 0x30));
                        }
                        *(int *)((longlong)pplVar13[8] + (longlong)pplVar24) =
                             *(int *)((longlong)pplVar13[8] + (longlong)pplVar24) + iVar11;
                      }
                      pplVar24 = (longlong **)((longlong)pplVar24 + 4);
                      pplVar26 = pplVar26 + 1;
                      pplVar37 = (longlong **)(ulonglong)(iVar8 + 1U);
                    } while ((int)(iVar8 + 1U) < *(int *)(lVar12 + 0x390));
                  }
                }
                FUN_14009f9c0(&local_af0);
              }
              if (((byte)local_res20 != '\0') || ((byte)local_res10 != '\0')) {
                pplVar13 = pplVar28;
                if (0 < (int)(uint)local_a78) {
                  puVar23 = local_a68;
                  if (local_a58 != (undefined *)0x0) {
                    pplVar13 = (longlong **)0x0;
                    puVar23 = local_a58;
                  }
                  do {
                    iVar8 = (int)pplVar13;
                    if ((*(uint *)(puVar23 +
                                  (longlong)((int)(iVar8 + (iVar8 >> 0x1f & 0x1fU)) >> 5) * 4) &
                        1 << ((byte)pplVar13 & 0x1f)) != 0) break;
                    pplVar13 = (longlong **)(ulonglong)(iVar8 + 1U);
                  } while ((int)(iVar8 + 1U) < (int)(uint)local_a78);
                }
                uVar9 = (uint)pplVar13;
                while (uVar9 != (uint)local_a78) {
                  iVar8 = *(int *)((longlong)(int)pplVar13 * 0x58 + local_a70 + 0x20);
                  pplVar24 = pplVar28;
                  if (0 < iVar8) {
                    do {
                      uVar9 = (int)pplVar24 + 1;
                      pplVar24 = (longlong **)(ulonglong)uVar9;
                    } while ((int)uVar9 < iVar8);
                  }
                  uVar9 = (int)pplVar13 + 1;
                  pplVar13 = (longlong **)(ulonglong)uVar9;
                  if ((int)uVar9 < (int)(uint)local_a78) {
                    puVar23 = local_a68;
                    if (local_a58 != (undefined *)0x0) {
                      puVar23 = local_a58;
                    }
                    do {
                      iVar8 = (int)pplVar13;
                      if ((*(uint *)(puVar23 +
                                    (longlong)((int)(iVar8 + (iVar8 >> 0x1f & 0x1fU)) >> 5) * 4) &
                          1 << ((byte)pplVar13 & 0x1f)) != 0) break;
                      pplVar13 = (longlong **)(ulonglong)(iVar8 + 1U);
                    } while ((int)(iVar8 + 1U) < (int)(uint)local_a78);
                    uVar9 = (uint)pplVar13;
                  }
                }
              }
              FUN_140200ac0(&local_a78);
              return (bool)1;
            }
            uVar14 = FUN_140056ba0(&local_b28,&local_res10,0);
            local_res10._0_1_ = FUN_140039fd0(uVar14,L"ACTUAL");
            local_res10 = (longlong **)
                          ((ulonglong)local_res10 & 0xffffffffffffff00 |
                          (ulonglong)(byte)local_res10);
            uVar10 = extraout_XMM0_Da;
            if (local_b20 != (wchar_t *)0x0) {
              uVar10 = FUN_140d2e720(2);
              local_b20 = (wchar_t *)0x0;
            }
            local_ab0 = 0;
            local_ac4 = 0;
            local_ac0 = 0;
            local_ab8 = 0;
            local_abc = 0;
            local_ab4 = 0;
            local_ac8 = 0;
            lVar12 = FUN_1400ae560(uVar10,L"MemorySplitClassesToTrack",0,1,&DAT_1434a0330);
            local_res20 = (int *)((ulonglong)local_res20 & 0xffffffffffffff00 |
                                 (ulonglong)DAT_14340fa58);
            DAT_14340fa58 = 1;
            local_a78 = 0;
            local_a70 = 0;
            local_a58 = (undefined *)0x0;
            local_a50 = 0;
            local_a48 = 0;
            local_a38 = (undefined *)0x0;
            local_a30 = 0;
            pplVar13 = pplVar28;
            if (lVar12 != 0) {
              FUN_14006de20(&local_a08,lVar12);
              pplVar13 = (longlong **)0x0;
              if (iStack_9ec < *(int *)((longlong)local_9f8._0_8_ + 0x18)) {
                local_a18[0] = 0;
                local_b18 = local_9f8._8_4_;
                do {
                  if (*(int *)(local_a08[1] + (longlong)iStack_9ec * 0x20 + 8) == 0) {
                    puVar33 = &DAT_1426a9d58;
                  }
                  else {
                    puVar33 = *(undefined2 **)(local_a08[1] + (longlong)iStack_9ec * 0x20 + 0x10);
                  }
                  ppuVar19 = (undefined **)FUN_14007c690(DAT_143387528,0xffffffffffffffff,puVar33,0)
                  ;
                  if (ppuVar19 != (undefined **)0x0) {
                    local_b20 = (wchar_t *)((ulonglong)local_b20 & 0xffffffff00000000);
                    local_b28 = ppuVar19;
                    FUN_140212580(&local_a78,&local_b38,&local_b28);
                  }
                  local_b18 = local_b18 & ~local_a00._4_4_;
                  puVar21 = local_9f8._0_8_;
                  if (*(undefined4 **)(local_9f8._0_8_ + 4) != (undefined4 *)0x0) {
                    puVar21 = *(undefined4 **)(local_9f8._0_8_ + 4);
                  }
                  puVar32 = local_a18;
                  if (puVar21 != (undefined4 *)0x0) {
                    puVar32 = puVar21;
                  }
                  uVar9 = puVar32[(int)local_a00] & local_b18;
                  if (uVar9 == 0) {
                    iStack_9ec = local_9f8._0_8_[6];
                    do {
                      local_a00._0_4_ = (int)local_a00 + 1;
                      local_9e8._0_4_ = (int)local_9e8 + 0x20;
                      if ((int)((iStack_9ec + -1 >> 0x1f & 0x1fU) + iStack_9ec + -1) >> 5 <
                          (int)local_a00) goto LAB_1401fed92;
                      uVar9 = puVar32[(int)local_a00];
                      local_b18 = 0xffffffff;
                    } while (uVar9 == 0);
                  }
                  local_a00._4_4_ = uVar9 - 1 & uVar9 ^ uVar9;
                  if (local_a00._4_4_ == 0) {
                    iVar8 = 0x20;
                  }
                  else {
                    iVar8 = 0x1f;
                    if (local_a00._4_4_ != 0) {
                      for (; local_a00._4_4_ >> iVar8 == 0; iVar8 = iVar8 + -1) {
                      }
                    }
                    iVar8 = 0x1f - iVar8;
                  }
                  iStack_9ec = ((int)local_9e8 - iVar8) + 0x1f;
LAB_1401fed92:
                } while (iStack_9ec < local_9f8._0_8_[6]);
                pplVar13 = (longlong **)(ulonglong)local_ac4;
              }
            }
            FUN_14009fa10(&local_af0,0);
            pplVar24 = pplVar13;
            iVar8 = local_ab0;
            iVar7 = local_ab0;
            while ((uVar14 = local_res18, -1 < (int)local_ae8 && ((int)local_ae8 < DAT_1435152d0)))
            {
              plVar30 = *(longlong **)(maybeGObjects + (longlong)(int)local_ae8 * 8);
              local_b18 = (**(code **)(*plVar30 + 0x1a8))(plVar30);
              if ((local_b18 == 0) &&
                 (FUN_14009f6f0(&local_9b8,plVar30), local_b18 = local_968, (byte)local_res10 != 0))
              {
                local_b18 = local_960;
              }
              iVar11 = local_a30;
              lVar29 = local_a70;
              lVar12 = plVar30[9];
              if (local_a30 != 0) {
                uVar9 = FUN_14003bc10(lVar12);
                puVar23 = local_a40;
                if (local_a38 != (undefined *)0x0) {
                  puVar23 = local_a38;
                }
                for (iVar11 = *(int *)(puVar23 + (longlong)(int)(iVar11 - 1U & uVar9) * 4);
                    iVar11 != -1; iVar11 = *(int *)(lVar29 + 0x10 + (longlong)iVar11 * 0x18)) {
                  if (*(longlong *)(lVar29 + (longlong)iVar11 * 0x18) == lVar12) {
                    if (((iVar11 != -1) && (lVar12 = lVar29 + (longlong)iVar11 * 0x18, lVar12 != 0))
                       && (piVar16 = (int *)(lVar12 + 8), piVar16 != (int *)0x0))
                    goto LAB_1401fef4e;
                    break;
                  }
                }
              }
              pplVar13 = pplVar28;
              if (0 < (int)(uint)local_a78) {
                puVar23 = local_a68;
                if (local_a58 != (undefined *)0x0) {
                  pplVar13 = (longlong **)0x0;
                  puVar23 = local_a58;
                }
                do {
                  iVar11 = (int)pplVar13;
                  if ((*(uint *)(puVar23 +
                                (longlong)((int)(iVar11 + (iVar11 >> 0x1f & 0x1fU)) >> 5) * 4) &
                      1 << ((byte)pplVar13 & 0x1f)) != 0) break;
                  pplVar13 = (longlong **)(ulonglong)(iVar11 + 1U);
                } while ((int)(iVar11 + 1U) < (int)(uint)local_a78);
              }
              uVar9 = (uint)pplVar13;
              while (uVar9 != (uint)local_a78) {
                iVar11 = (int)pplVar13;
                local_b38 = (longlong **)((longlong)iVar11 * 3);
                cVar4 = FUN_14003c640(plVar30,*(undefined8 *)(lVar29 + (longlong)iVar11 * 0x18));
                if (cVar4 != '\0') {
                  piVar16 = (int *)(lVar29 + ((longlong)local_b38 + 1) * 8);
                  if (piVar16 != (int *)0x0) {
LAB_1401fef4e:
                    *piVar16 = *piVar16 + local_b18;
                  }
                  break;
                }
                uVar9 = iVar11 + 1;
                pplVar13 = (longlong **)(ulonglong)uVar9;
                if ((int)uVar9 < (int)(uint)local_a78) {
                  puVar23 = local_a68;
                  if (local_a58 != (undefined *)0x0) {
                    puVar23 = local_a58;
                  }
                  do {
                    iVar11 = (int)pplVar13;
                    if ((*(uint *)(puVar23 +
                                  (longlong)((int)(iVar11 + (iVar11 >> 0x1f & 0x1fU)) >> 5) * 4) &
                        1 << ((byte)pplVar13 & 0x1f)) != 0) break;
                    pplVar13 = (longlong **)(ulonglong)(iVar11 + 1U);
                  } while ((int)(iVar11 + 1U) < (int)(uint)local_a78);
                  uVar9 = (uint)pplVar13;
                }
              }
              uVar9 = local_b18;
              cVar4 = FUN_14003c640(plVar30,DAT_1434dfb30);
              if ((cVar4 != '\0') || (cVar4 = FUN_14003c640(plVar30,DAT_1434e0420), cVar4 != '\0'))
              {
                iVar8 = iVar8 + uVar9;
              }
              cVar4 = FUN_14003c640(plVar30,DAT_1434dfac0);
              if (((cVar4 != '\0') || (cVar4 = FUN_14003c640(plVar30,DAT_1434e0580), cVar4 != '\0'))
                 || (cVar4 = FUN_14003c640(plVar30,DAT_1434dfb38), cVar4 != '\0')) {
                iVar7 = iVar7 + uVar9;
              }
              cVar4 = FUN_14003c640(plVar30,DAT_1434de0c8);
              if (((((((cVar4 != '\0') ||
                      (cVar4 = FUN_14003c640(plVar30,DAT_1434de0d8), cVar4 != '\0')) ||
                     (cVar4 = FUN_14003c640(plVar30,DAT_143387528), cVar4 != '\0')) ||
                    ((cVar4 = FUN_14003c640(plVar30,DAT_1434de100), cVar4 != '\0' ||
                     (cVar4 = FUN_14003c640(plVar30,DAT_1434de130), cVar4 != '\0')))) ||
                   (cVar4 = FUN_14003c640(plVar30,DAT_1433874f0), cVar4 != '\0')) ||
                  (((cVar4 = FUN_14003c640(plVar30,DAT_1434de118), cVar4 != '\0' ||
                    (cVar4 = FUN_14003c640(plVar30,DAT_1433874c0), cVar4 != '\0')) ||
                   ((((cVar4 = FUN_14003c640(plVar30,DAT_143387520), cVar4 != '\0' ||
                      (((cVar4 = FUN_14003c640(plVar30,DAT_1434ddf18), cVar4 != '\0' ||
                        (cVar4 = FUN_14003c640(plVar30,DAT_1433874f8), cVar4 != '\0')) ||
                       (cVar4 = FUN_14003c640(plVar30,DAT_1434ddf20), cVar4 != '\0')))) ||
                     ((cVar4 = FUN_14003c640(plVar30,DAT_1434de120), cVar4 != '\0' ||
                      (cVar4 = FUN_14003c640(plVar30,DAT_1434de108), cVar4 != '\0')))) ||
                    (cVar4 = FUN_14003c640(plVar30,DAT_1434de0c0), cVar4 != '\0')))))) ||
                 (((cVar4 = FUN_14003c640(plVar30,DAT_1434de128), cVar4 != '\0' ||
                   (cVar4 = FUN_14003c640(plVar30,DAT_143387510), cVar4 != '\0')) ||
                  ((cVar4 = FUN_14003c640(plVar30,DAT_1434de110), cVar4 != '\0' ||
                   (cVar4 = FUN_14003c640(plVar30,DAT_1434de0b8), cVar4 != '\0')))))) {
                local_ab8 = local_ab8 + uVar9;
              }
              cVar4 = FUN_14003c640(plVar30,DAT_1434dfaa8);
              if ((((cVar4 != '\0') || (cVar4 = FUN_14003c640(plVar30,DAT_1434dfce0), cVar4 != '\0')
                   ) || (cVar4 = FUN_14003c640(plVar30,DAT_1434dfcd8), cVar4 != '\0')) ||
                 ((cVar4 = FUN_14003c640(plVar30,DAT_1434dfb68), cVar4 != '\0' ||
                  (cVar4 = FUN_14003c640(plVar30,DAT_1434e1660), cVar4 != '\0')))) {
                local_abc = local_abc + uVar9;
              }
              cVar4 = FUN_14003c640(plVar30,DAT_1434e0e48);
              if (((cVar4 != '\0') || (cVar4 = FUN_14003c640(plVar30,DAT_1434e0e68), cVar4 != '\0'))
                 || ((cVar4 = FUN_14003c640(plVar30,DAT_1434e0e60), cVar4 != '\0' ||
                     ((cVar4 = FUN_14003c640(plVar30,DAT_1434e0e70), cVar4 != '\0' ||
                      (cVar4 = FUN_14003c640(plVar30,DAT_1434df2c0), cVar4 != '\0')))))) {
                local_ab4 = local_ab4 + uVar9;
              }
              cVar4 = FUN_14003c640(plVar30,DAT_1434e0458);
              if ((cVar4 != '\0') || (cVar4 = FUN_14003c640(plVar30,DAT_1434dfc90), cVar4 != '\0'))
              {
                local_ac8 = local_ac8 + uVar9;
              }
              cVar4 = FUN_14003c640(plVar30,DAT_1434dfdf0);
              uVar9 = local_ac0;
              if (cVar4 != '\0') {
                FUN_14009f6f0(&local_9b8,plVar30);
                uVar9 = local_960;
                uVar3 = local_968;
                plVar30 = (longlong *)FUN_1402159c0(plVar30);
                iVar11 = (**(code **)(*plVar30 + 0x1a8))(plVar30);
                uVar9 = local_ac0 + iVar11 + uVar9;
                local_ac0 = uVar9;
                iVar11 = (**(code **)(*plVar30 + 0x1a8))(plVar30);
                local_ac4 = local_ac4 + iVar11 + uVar3;
              }
              pplVar24 = (longlong **)(ulonglong)uVar9;
              pplVar13 = (longlong **)(ulonglong)local_ac4;
              FUN_14009f9c0(&local_af0);
            }
            DAT_14340fa58 = (byte)local_res20;
            local_ab0 = iVar8;
            FUN_14004fcd0(local_res18,L"Memory split (in KByte)");
            FUN_14004fcd0(uVar14,L"  %32s: %6i",&DAT_1426fd4a0,0);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",&DAT_1426fd4c0,
                          (int)((local_ab0 >> 0x1f & 0x3ffU) + local_ab0) >> 10);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",L"Level",
                          (int)((iVar7 >> 0x1f & 0x3ffU) + iVar7) >> 10);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",L"Particles (Active)",
                          (int)(((int)pplVar13 >> 0x1f & 0x3ffU) + (int)pplVar13) >> 10);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",L"Particles (Peak)",
                          (int)(((int)pplVar24 >> 0x1f & 0x3ffU) + (int)pplVar24) >> 10);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",L"Pathing",0);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",L"Script",
                          (int)((local_ab8 >> 0x1f & 0x3ffU) + local_ab8) >> 10);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",L"SkeletalMesh",
                          (int)((local_abc >> 0x1f & 0x3ffU) + local_abc) >> 10);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",L"StaticMesh",
                          (int)((local_ab4 >> 0x1f & 0x3ffU) + local_ab4) >> 10);
            FUN_14004fcd0(uVar14,L"  %32s: %6i",&DAT_1426fb010,
                          (int)((local_ac8 >> 0x1f & 0x3ffU) + local_ac8) >> 10);
            FUN_14004fcd0(uVar14,&DAT_1426fd554);
            iVar8 = (uint)local_a78;
            if (0 < (int)(uint)local_a78) {
              puVar23 = local_a68;
              if (local_a58 != (undefined *)0x0) {
                puVar23 = local_a58;
              }
              do {
                iVar7 = (int)pplVar28;
                if ((*(uint *)(puVar23 + (longlong)((int)(iVar7 + (iVar7 >> 0x1f & 0x1fU)) >> 5) * 4
                              ) & 1 << ((byte)pplVar28 & 0x1f)) != 0) break;
                pplVar28 = (longlong **)(ulonglong)(iVar7 + 1U);
              } while ((int)(iVar7 + 1U) < (int)(uint)local_a78);
            }
            iVar7 = (int)pplVar28;
            lVar12 = local_a70;
            do {
              local_a70 = lVar12;
              if (iVar7 == iVar8) {
                local_b20 = (wchar_t *)CONCAT44((int)((ulonglong)local_b20 >> 0x20),0x10001);
                local_b28 = &PTR_FUN_1426b7590;
                FUN_140323600();
                FUN_14004fcd0(uVar14,L"  %32s: %6i",L"RenderTargets",0);
                uVar27 = FUN_1400b0a30();
                FUN_14004fcd0(uVar14,L"  %32s: %6i",L"FConfigCache",uVar27 >> 10);
                FUN_14004fcd0(uVar14,L"  %32s: %6i",L"TexturePool",0);
                FUN_1402009e0(&local_a78);
                return (bool)1;
              }
              iVar7 = (int)pplVar28;
              piVar16 = (int *)FUN_140038cc0(*(undefined8 *)(lVar12 + (longlong)iVar7 * 0x18),
                                             &local_af0);
              if (*piVar16 == 0) {
                puVar33 = &DAT_1426a9d58;
              }
              else {
                puVar33 = *(undefined2 **)(piVar16 + 2);
              }
              iVar11 = *(int *)(lVar12 + 8 + (longlong)iVar7 * 0x18);
              FUN_14004fcd0(uVar14,L"  %32s: %6i",puVar33,
                            (int)((iVar11 >> 0x1f & 0x3ffU) + iVar11) >> 10);
              FUN_14003ceb0(&local_af0);
              pplVar28 = (longlong **)(ulonglong)(iVar7 + 1U);
              if ((int)(iVar7 + 1U) < (int)(uint)local_a78) {
                puVar23 = local_a68;
                if (local_a58 != (undefined *)0x0) {
                  puVar23 = local_a58;
                }
                do {
                  iVar7 = (int)pplVar28;
                  if ((*(uint *)(puVar23 +
                                (longlong)((int)(iVar7 + (iVar7 >> 0x1f & 0x1fU)) >> 5) * 4) &
                      1 << ((byte)pplVar28 & 0x1f)) != 0) break;
                  pplVar28 = (longlong **)(ulonglong)(iVar7 + 1U);
                } while ((int)(iVar7 + 1U) < (int)(uint)local_a78);
              }
              iVar7 = (int)pplVar28;
              lVar12 = local_a70;
            } while( true );
          }
          cVar4 = FUN_140055e40(local_b38,L"FAST");
          bVar38 = cVar4 == '\0';
          local_af0 = (undefined **)&DAT_14348facf;
          local_res10 = (longlong **)
                        ((ulonglong)local_res10 & 0xffffffffffffff00 | (ulonglong)DAT_14348facf);
          local_ae8 = local_ae8 & 0xffffff00 | (uint)DAT_14348facf;
          DAT_14348facf = 0;
          local_res20._0_4_ = 0;
          local_res20._4_4_ = 0;
          if (DAT_143515408 != 0) {
            FUN_14000c030(0,0,0);
          }
          uVar14 = 0x400000000000000;
          if (DAT_14349e32a != '\0') {
            uVar14 = 0x408000000000000;
          }
          FUN_140090400(uVar14,1);
          FUN_140323600();
          uVar14 = FUN_1400589d0(&local_b00);
          uVar18 = FUN_14003a200(&local_a28,L"MemLeaks\\");
          piVar16 = (int *)FUN_140039670(uVar14,local_aa8,uVar18);
          if (*piVar16 == 0) {
            puVar33 = &DAT_1426a9d58;
          }
          else {
            puVar33 = *(undefined2 **)(piVar16 + 2);
          }
          FUN_14003cef0(&local_b28,puVar33);
          if (local_aa0 != (undefined8 *)0x0) {
            FUN_140d2e720(2);
            local_aa0 = (undefined8 *)0x0;
          }
          if (local_af8 != (undefined8 *)0x0) {
            FUN_140d2e720(2);
            local_af8 = (undefined8 *)0x0;
          }
          pwVar25 = L"";
          if ((int)local_b28 != 0) {
            pwVar25 = local_b20;
          }
          FUN_1400b4fa0(DAT_1434a0b30,pwVar25,0);
          FUN_14003cef0(local_aa8,L".memlk");
          FUN_1400d3950(&local_ad8,local_aa8,1);
          if (local_aa0 != (undefined8 *)0x0) {
            FUN_140d2e720(2);
          }
          if ((int)local_ad8 != 0) {
            iVar8 = (int)local_ad8 + -1;
          }
          local_b00 = local_ad0;
          local_af8 = (undefined8 *)((longlong)local_ad0 + (longlong)iVar8 * 2);
          FUN_140039670(&local_b28,local_aa8,&local_b00);
          puVar17 = (undefined8 *)&DAT_1426a9d58;
          if (local_aa8[0] != 0) {
            puVar17 = local_aa0;
          }
          FUN_1400926d0(&local_888,puVar17);
          local_9b0 = (wchar_t *)0x0;
          local_9a4 = 0;
          local_9b8 = &PTR_FUN_1426b5c30;
          local_9a8 = 0;
          FUN_14003b570(&local_9b8);
          lVar12 = FUN_14013cf00(&local_9b8,L"CommandLine Options: %s",&DAT_143387550);
          FUN_14004fcd0(&local_888,*(undefined8 *)(lVar12 + 8));
          local_9b8 = &PTR_FUN_1426ce7a0;
          if ((local_9a8 & 1) != 0) {
            FUN_140d2e720(2,local_9b0);
          }
          FUN_14004fcd0(&local_888,&DAT_1426aec34);
          iVar8 = -1;
          if (param_1 == (undefined8 *)&DAT_00000058) {
LAB_1401fe3ff:
            if (bVar38) {
              (**(code **)*param_1)(param_1,L"MEMORYSPLIT ACTUAL",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
            }
            uVar10 = FUN_14014e8d0(&local_9b8);
            piVar16 = (int *)FUN_140154a30(uVar10,&local_b00);
            if (*piVar16 == 0) {
              puVar33 = &DAT_1426a9d58;
            }
            else {
              puVar33 = *(undefined2 **)(piVar16 + 2);
            }
            FUN_14004fcd0(&local_888,puVar33);
            if (local_af8 != (undefined8 *)0x0) {
              FUN_140d2e720(2);
            }
            piVar16 = (int *)FUN_140154a70(&local_9b8,&local_b00);
            if (*piVar16 == 0) {
              puVar33 = &DAT_1426a9d58;
            }
            else {
              puVar33 = *(undefined2 **)(piVar16 + 2);
            }
            FUN_14004fcd0(&local_888,puVar33);
            if (local_af8 != (undefined8 *)0x0) {
              FUN_140d2e720(2);
            }
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"OBJ LIST -ALPHASORT",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            if (bVar38) {
              (**(code **)*param_1)
                        (param_1,L"OBJ LIST class=SkeletalMesh -ALPHASORT -DetailedInfo",&local_888)
              ;
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
              (**(code **)*param_1)(param_1,L"LISTANIMSETS -ALPHASORT",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
              (**(code **)*param_1)(param_1,L"LISTMATINEEANIMSETS -ALPHASORT",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
              (**(code **)*param_1)(param_1,L"LISTANIMTREES -ALPHASORT",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
              (**(code **)*param_1)(param_1,L"LISTFACEFX -ALPHASORT",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
            }
            (**(code **)*param_1)(param_1,L"MEM DETAILED",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"GAMESPECIFIC_MEMORY",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"RENDERTARGET_MEM_USAGE",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"GPURESOURCEDUMP",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"DUMPCOMPRESSEDSHADERSTATS",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"DUMPTEXTURESTREAMINGSTATS",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            for (plVar30 = DAT_1434ebc90; plVar30 != (longlong *)0x0;
                plVar30 = (longlong *)plVar30[1]) {
              pplVar28 = (longlong **)
                         (ulonglong)
                         (uint)((int)pplVar28 +
                               (*(int *)(*plVar30 + 0x40) - *(int *)(*plVar30 + 0x74)));
            }
            FUN_14004fcd0(&local_888,L"NumShadersInMemory: %d",pplVar28);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"ANALYZEOCTREE",&local_888);
            uVar10 = FUN_14004fcd0(&local_888,&DAT_1426aec34);
            FUN_1400b0320(uVar10,&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"ListPrecacheMapPackages",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"ListLoadedPackages",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            if (bVar38) {
              (**(code **)*param_1)(param_1,L"LISTTEXTURES -ALPHASORT",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
              (**(code **)*param_1)(param_1,L"LISTSOUNDS -ALPHASORT",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
              (**(code **)*param_1)(param_1,L"LISTAUDIOCOMPONENTS",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
              (**(code **)*param_1)(param_1,L"LISTPARTICLESYSTEMS -ALPHASORT",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
            }
            (**(code **)*param_1)(param_1,L"LISTSPAWNEDACTORS",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            if (DAT_1434e04a0[0xd4] != 0) {
              puVar17 = (undefined8 *)(DAT_1434e04a0[0xd4] + 0x60);
              (**(code **)*puVar17)(puVar17,L"LOGOUTSTATLEVELS",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
            }
            if (bVar38) {
              (**(code **)*param_1)(param_1,L"LISTGFXRESOURCES",&local_888);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
            }
            (**(code **)*param_1)(param_1,L"DUMPPARTICLEMEM",&local_888);
            FUN_14004fcd0(&local_888,&DAT_1426aec34);
            (**(code **)*param_1)(param_1,L"DUMPGFXMEM",&local_888);
            uVar10 = FUN_14004fcd0(&local_888,&DAT_1426aec34);
            lVar12 = FUN_1400ae560(uVar10,L"MemLeakCheckExtraExecsToRun",0,1,&DAT_1434a0330);
            if ((lVar12 != 0) && (bVar38)) {
              FUN_14006de20(&local_a08,lVar12);
              if (iStack_9ec < *(int *)((longlong)local_9f8._0_8_ + 0x18)) {
                local_ac8 = 0;
                local_res20._0_4_ = local_9f8._8_4_;
                do {
                  local_b38 = (longlong **)local_a08[1];
                  lVar12 = (longlong)iStack_9ec;
                  if (*(int *)(local_b38 + lVar12 * 4 + 1) == 0) {
                    plVar30 = (longlong *)&DAT_1426a9d58;
                  }
                  else {
                    plVar30 = local_b38[lVar12 * 4 + 2];
                  }
                  FUN_14004fcd0(&local_888,plVar30);
                  if (*(int *)(local_b38 + lVar12 * 4 + 1) == 0) {
                    plVar30 = (longlong *)&DAT_1426a9d58;
                  }
                  else {
                    plVar30 = local_b38[lVar12 * 4 + 2];
                  }
                  (**(code **)*param_1)(param_1,plVar30,&local_888);
                  FUN_14004fcd0(&local_888,&DAT_1426aec34);
                  local_res20._0_4_ = (uint)local_res20 & ~local_a00._4_4_;
                  piVar16 = local_9f8._0_8_;
                  if (*(int **)(local_9f8._0_8_ + 4) != (int *)0x0) {
                    piVar16 = *(int **)(local_9f8._0_8_ + 4);
                  }
                  piVar31 = &local_ac8;
                  if (piVar16 != (int *)0x0) {
                    piVar31 = piVar16;
                  }
                  uVar9 = piVar31[(int)local_a00] & (uint)local_res20;
                  if (uVar9 == 0) {
                    iStack_9ec = local_9f8._0_8_[6];
                    do {
                      local_a00._0_4_ = (int)local_a00 + 1;
                      local_9e8._0_4_ = (int)local_9e8 + 0x20;
                      if ((int)((iStack_9ec + -1 >> 0x1f & 0x1fU) + iStack_9ec + -1) >> 5 <
                          (int)local_a00) goto LAB_1401feaaa;
                      uVar9 = piVar31[(int)local_a00];
                      local_res20._0_4_ = 0xffffffff;
                    } while (uVar9 == 0);
                  }
                  local_a00._4_4_ = uVar9 - 1 & uVar9 ^ uVar9;
                  if (local_a00._4_4_ == 0) {
                    iVar8 = 0x20;
                  }
                  else {
                    iVar8 = 0x1f;
                    if (local_a00._4_4_ != 0) {
                      for (; local_a00._4_4_ >> iVar8 == 0; iVar8 = iVar8 + -1) {
                      }
                    }
                    iVar8 = 0x1f - iVar8;
                  }
                  iStack_9ec = ((int)local_9e8 - iVar8) + 0x1f;
LAB_1401feaaa:
                } while (iStack_9ec < local_9f8._0_8_[6]);
              }
            }
            FUN_1400927a0(&local_888);
            puVar17 = local_aa0;
            puVar34 = (undefined8 *)&DAT_1426a9d58;
            if (local_aa8[0] != 0) {
              puVar34 = local_aa0;
            }
            FUN_14003cef0(&local_a28,puVar34);
            FUN_14003cef0(&local_b00,L"UE_PROFILER!MEMLEAK:");
            FUN_1400d3810(&local_b00,&local_a28);
            if (local_af8 != (undefined8 *)0x0) {
              FUN_140d2e720(2);
            }
            if (local_a20 != 0) {
              FUN_140d2e720(2);
            }
            local_888 = &PTR_FUN_1426b3980;
            if (puVar17 != (undefined8 *)0x0) {
              FUN_140d2e720(2,puVar17);
            }
            if (local_ad0 != (undefined8 *)0x0) {
              FUN_140d2e720(2);
              local_ad0 = (undefined8 *)0x0;
            }
            if (local_b20 != (wchar_t *)0x0) {
              FUN_140d2e720(2);
            }
            DAT_14348facf = (byte)local_res10;
            return (bool)1;
          }
          do {
            iVar8 = iVar8 + 1;
            if ((iVar8 < 0) || (*(int *)(param_1 + 199) <= iVar8)) break;
          } while (*(longlong *)(param_1[200] + (longlong)iVar8 * 8) == 0);
          do {
            if (((param_1 == (undefined8 *)&DAT_00000058) || (iVar8 < 0)) ||
               (*(int *)(param_1 + 199) <= iVar8)) goto LAB_1401fe3ff;
            local_a90 = 0;
            local_a88 = 0;
            local_b38 = (longlong **)0x0;
            local_b30 = (longlong **)((ulonglong)local_b30 & 0xffffffff00000000);
            plVar30 = *(longlong **)(*(longlong *)(param_1[200] + (longlong)iVar8 * 8) + 0x60);
            if (plVar30 != (longlong *)0x0) {
              (**(code **)(*plVar30 + 0xed0))(plVar30,&local_a90,&local_b38,0);
              local_b10 = 0;
              local_b08 = (undefined8 *)0x0;
              local_b00 = (undefined8 *)0x0;
              local_af8 = (undefined8 *)0x0;
              local_a28 = 0;
              local_a20 = 0;
              (**(code **)(*plVar30 + 0x1f78))
                        (plVar30,1,&local_a90,&local_b38,&local_b10,&local_b00,&local_a28);
              puVar17 = (undefined8 *)&DAT_1426a9d58;
              if ((int)local_b10 != 0) {
                puVar17 = local_b08;
              }
              FUN_14004fcd0(&local_888,puVar17);
              FUN_14004fcd0(&local_888,&DAT_1426aec34);
              if (local_a20 != 0) {
                FUN_140d2e720(2);
                local_a20 = 0;
              }
              if (local_af8 != (undefined8 *)0x0) {
                FUN_140d2e720(2);
                local_af8 = (undefined8 *)0x0;
              }
              if (local_b08 != (undefined8 *)0x0) {
                FUN_140d2e720(2);
                local_b08 = (undefined8 *)0x0;
              }
            }
            do {
              iVar8 = iVar8 + 1;
              if ((iVar8 < 0) || (*(int *)(param_1 + 199) <= iVar8)) break;
            } while (*(longlong *)(param_1[200] + (longlong)iVar8 * 8) == 0);
          } while( true );
        }
        FUN_140056ba0(&local_b28,&local_b38,0);
        puVar33 = &DAT_1426a9d58;
        pwVar25 = L"";
        if ((int)local_b28 != 0) {
          pwVar25 = local_b20;
        }
        iVar8 = _wcsicmp(pwVar25,L"FRAG");
        plVar30 = DAT_1434e04a0;
        local_res10 = (longlong **)
                      ((ulonglong)local_res10 & 0xffffffffffffff00 | (ulonglong)(iVar8 == 0));
        iVar8 = *(int *)(DAT_1434e04a0 + 0xd5);
        iVar7 = iVar8 + 1;
        *(int *)(DAT_1434e04a0 + 0xd5) = iVar7;
        if (*(int *)((longlong)plVar30 + 0x6ac) < iVar7) {
          uVar10 = FUN_1400a2df0(iVar7,*(int *)((longlong)plVar30 + 0x6ac),0x10);
          *(undefined4 *)((longlong)plVar30 + 0x6ac) = uVar10;
          FUN_14003c090(plVar30 + 0xd6);
          plVar30 = DAT_1434e04a0;
        }
        lVar12 = (longlong)iVar8 * 0x10 + plVar30[0xd6];
        if (lVar12 != 0) {
          FUN_14003cef0(lVar12,L"MEM DETAILED");
        }
        (**(code **)*param_1)(param_1,L"MEMORYSPLIT ACTUAL",PTR_DAT_143243518);
        FUN_14014e8d0(&local_9b8);
        piVar16 = (int *)FUN_140154a70(&local_9b8,&local_ad8);
        if (*piVar16 != 0) {
          puVar33 = *(undefined2 **)(piVar16 + 2);
        }
        FUN_14004fcd0(param_3,puVar33);
        if (local_ad0 != (undefined8 *)0x0) {
          FUN_140d2e720(2);
          local_ad0 = (undefined8 *)0x0;
        }
        (**(code **)*param_1)(param_1,L"OBJ LIST -ALPHASORT",PTR_DAT_143243518);
        (**(code **)*param_1)(param_1,L"LISTTEXTURES",PTR_DAT_143243518);
        (**(code **)*param_1)(param_1,L"LISTSOUNDS",PTR_DAT_143243518);
        (**(code **)*param_1)(param_1,L"LISTAUDIOCOMPONENTS",PTR_DAT_143243518);
        (**(code **)*param_1)(param_1,L"ListLoadedPackages",PTR_DAT_143243518);
        uVar10 = (**(code **)*param_1)(param_1,L"ListPrecacheMapPackages",PTR_DAT_143243518);
        FUN_1400b0320(uVar10,param_3);
        if ((byte)local_res10 == '\x01') {
          (**(code **)*param_1)(param_1,L"MemFragCheck",PTR_DAT_143243518);
        }
        goto joined_r0x0001401fdf63;
      }
      FUN_14003cef0(&local_af0,L"Quit");
      if (CONCAT44(uStack_ae4,local_ae8) != 0) {
        FUN_140d2e720(2);
      }
      (**(code **)(param_1[-0xb] + 0x288))(param_1 + -0xb,0);
      (**(code **)(param_1[-0xb] + 0x290))(param_1 + -0xb);
      (**(code **)(param_1[-0xb] + 0x2a0))(param_1 + -0xb,0);
      uVar14 = FUN_14003cef0(&local_af0,&DAT_143387550);
      bVar38 = false;
      local_res20._0_4_ = 2;
      uVar18 = CONCAT44(uVar10,0xffffffff);
      iVar8 = FUN_1400380b0(uVar14,L"DoingASentinelRun=1",0,1,uVar18);
      uVar10 = (undefined4)((ulonglong)uVar18 >> 0x20);
      if (iVar8 == -1) {
        uVar14 = FUN_14003cef0(&local_b28,&DAT_143387550);
        bVar38 = true;
        local_res20._0_4_ = 6;
        iVar8 = FUN_1400380b0(uVar14,L"gDASR=1",0,1,CONCAT44(uVar10,0xffffffff));
        if ((iVar8 == -1) && (DAT_143243520 == -1)) {
          bVar2 = false;
          goto LAB_1402007be;
        }
      }
      bVar2 = true;
LAB_1402007be:
      if ((bVar38) && (local_b20 != (wchar_t *)0x0)) {
        FUN_140d2e720(2);
        local_b20 = (wchar_t *)0x0;
      }
      if (CONCAT44(uStack_ae4,local_ae8) != 0) {
        FUN_140d2e720(2);
      }
      if (bVar2) {
        FUN_140051460(&local_b28,L"EXEC EndRun @RunID=%i, @ResultDescription=\'%s\'",DAT_143243520,
                      PTR_u_Passed_143243240);
        pwVar25 = local_b20;
        pwVar20 = L"";
        if ((int)local_b28 != 0) {
          pwVar20 = local_b20;
        }
        FUN_14003cef0(&local_ad8,pwVar20);
        plVar30 = *(longlong **)(DAT_1434a0b38 + 8);
        if (plVar30 != (longlong *)0x0) {
          puVar17 = (undefined8 *)&DAT_1426a9d58;
          if ((int)local_ad8 != 0) {
            puVar17 = local_ad0;
          }
          (**(code **)(*plVar30 + 0x20))(plVar30,puVar17);
        }
        if (local_ad0 != (undefined8 *)0x0) {
          FUN_140d2e720(2,local_ad0);
        }
        if (pwVar25 != (wchar_t *)0x0) {
          FUN_140d2e720(2,pwVar25);
        }
      }
      lVar12 = **(longlong **)(*(longlong *)(DAT_1434dfca0 + 0x80) + 0x60);
      if ((lVar12 != 0) && (*(longlong *)(lVar12 + 0x4f8) != 0)) {
        (**(code **)(**(longlong **)(lVar12 + 0x4f8) + 0xbf0))();
      }
      local_res10 = (longlong **)0x0;
      lVar12 = FUN_1402183d0(DAT_1434dfca0,0);
      if (((lVar12 != 0) && (*(longlong *)(lVar12 + 0x80) != 0)) &&
         (plVar30 = *(longlong **)(*(longlong *)(lVar12 + 0x80) + 0xf48), plVar30 != (longlong *)0x0
         )) {
        (**(code **)(*plVar30 + 0x210))();
        (**(code **)(**(longlong **)(lVar12 + 0x80) + 0x250))(*(longlong **)(lVar12 + 0x80),0);
      }
      if (((param_1[0x128] != 0) && (lVar12 = *(longlong *)(param_1[0x128] + 0xd8), lVar12 != 0)) &&
         ((lVar12 = *(longlong *)(lVar12 + 0x80), lVar12 != 0 &&
          (plVar30 = *(longlong **)(lVar12 + 0xf48), plVar30 != (longlong *)0x0)))) {
        (**(code **)(*plVar30 + 0x210))();
        plVar30 = *(longlong **)(*(longlong *)(param_1[0x128] + 0xd8) + 0x80);
        (**(code **)(*plVar30 + 0x250))(plVar30,0);
      }
      FUN_14004f9d0(param_3,L"Closing by request");
      FUN_140133fd0(0);
      return (bool)1;
    }
    FUN_14044ce10(&local_9b8,param_1 + 300,local_b38,0);
    cVar4 = FUN_14044e140(&local_9b8);
    if (cVar4 == '\0') {
      pwVar25 = L"";
      if (DAT_1435215c0 != 0) {
        pwVar25 = DAT_1435215c8;
      }
      pwVar20 = L"";
      if ((int)local_9b8 != 0) {
        pwVar20 = local_9b0;
      }
      iVar8 = _wcsicmp(pwVar20,pwVar25);
      if (iVar8 == 0) {
        FUN_14003cef0(&local_ad8,&DAT_1426a9d58);
        uVar14 = FUN_140057430(&local_b28,L"Connecting",L"Engine");
        (**(code **)(*DAT_1434e04a0 + 0x260))(DAT_1434e04a0,1,&local_ad8,uVar14);
        if (local_b20 != (wchar_t *)0x0) {
          FUN_140d2e720(2);
          local_b20 = (wchar_t *)0x0;
        }
        goto LAB_1401fd533;
      }
    }
    else {
      local_b10 = 0;
      local_b08 = (undefined8 *)0x0;
      puVar33 = &DAT_1426a9d58;
      if (local_990 != 0) {
        puVar33 = local_988;
      }
      cVar4 = FUN_1400d1930(DAT_1434c46d0,puVar33,0,&local_b10);
      local_ad0 = local_b08;
      if (cVar4 == '\0') {
        puVar33 = &DAT_1426a9d58;
        if (local_990 != 0) {
          puVar33 = local_988;
        }
        FUN_14004fcd0(param_3,L"ERROR: The map \'%s\' does not exist.",puVar33);
        if (local_b08 != (undefined8 *)0x0) {
          FUN_140d2e720(2);
        }
        goto LAB_1401fd556;
      }
LAB_1401fd533:
      if (local_ad0 != (undefined8 *)0x0) {
        FUN_140d2e720(2);
      }
    }
    (**(code **)(param_1[-0xb] + 0x250))(param_1 + -0xb,pplVar13,0);
LAB_1401fd556:
    FUN_140177980(&local_9b8);
    return (bool)1;
  }
  FUN_14044ce10(&local_9b8,param_1 + 300,local_b38,1);
  pwVar25 = L"";
  if (DAT_1435215c0 != 0) {
    pwVar25 = DAT_1435215c8;
  }
  pwVar20 = L"";
  if ((int)local_9b8 != 0) {
    pwVar20 = local_9b0;
  }
  iVar8 = _wcsicmp(pwVar20,pwVar25);
  if ((iVar8 == 0) && ((local_9a8 == 0 || (local_9a8 == 1)))) {
    local_b10 = 0;
    local_b08 = (undefined8 *)0x0;
    puVar33 = &DAT_1426a9d58;
    if (local_990 != 0) {
      puVar33 = local_988;
    }
    cVar4 = FUN_1400d1930(DAT_1434c46d0,puVar33,0,&local_b10);
    local_af8 = local_b08;
    if (cVar4 == '\0') {
      puVar33 = &DAT_1426a9d58;
      if (local_990 != 0) {
        puVar33 = local_988;
      }
      FUN_14004fcd0(param_3,L"ERROR: The map \'%s\' does not exist.",puVar33);
      if (local_b08 != (undefined8 *)0x0) {
        FUN_140d2e720(2);
      }
      goto LAB_1401fd21b;
    }
LAB_1401fd1f8:
    if (local_af8 != (undefined8 *)0x0) {
      FUN_140d2e720(2);
    }
  }
  else {
    pwVar25 = L"";
    if (DAT_1435215c0 != 0) {
      pwVar25 = DAT_1435215c8;
    }
    pwVar20 = L"";
    if ((int)local_9b8 != 0) {
      pwVar20 = local_9b0;
    }
    iVar8 = _wcsicmp(pwVar20,pwVar25);
    if (iVar8 == 0) {
      FUN_14003ce30(&local_b00,0);
      if ((int)local_b00 != 0) {
        memcpy(local_af8,&DAT_1426a9d58,(longlong)(int)local_b00 * 2);
      }
      uVar14 = FUN_140057430(&local_b28,L"Connecting",L"Engine");
      (**(code **)(*DAT_1434e04a0 + 0x260))(DAT_1434e04a0,1,&local_b00,uVar14);
      if (local_b20 != (wchar_t *)0x0) {
        FUN_140d2e720(2);
        local_b20 = (wchar_t *)0x0;
      }
      goto LAB_1401fd1f8;
    }
  }
  (**(code **)(param_1[-0xb] + 0x250))(param_1 + -0xb,pplVar13,1);
LAB_1401fd21b:
  FUN_140177980(&local_9b8);
  return (bool)1;
}

