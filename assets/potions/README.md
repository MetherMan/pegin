# 초대형 물약 (19132)

사용자가 고른 ImageGen 그림(`hp-extra-large-v2-green.png`, 원본 `ChatGPT 이미지 2026년 10월 1일 오전 10_43_21.png`와 같은 파일)을 게임 아이콘 크기 28×28로 줄여 `hppotion_xl` 아이콘을 만들었다(`hp-extra-large-icon-28.png`). 1254px 그림을 4배 크기까지 평균으로 줄인 뒤 28px로 줄이고, 원래 물약 아이콘처럼 외곽선이 보이게 약하게 선명화했다.

| 항목 | HP 회복 포션(대) 10097 | 초대형 물약 19132 |
|---|---:|---:|
| 회복량 (ITEM_DATA 7열, `UseHPPotion`이 사용) | 120 | 600 |
| 서버 구입가 (실제 차감) | 600 | 720 |
| 상점 창 표시가 (`Item/ITEM.dat`) | 250 | 300 |
| 판매가 / 수리가 (서버) | 300 / 35 | 360 / 42 |

가격은 파일마다 각자의 대형 포션 값에 20%를 더했다. 서버가 실제로 받는 돈은 ITEM_DATA, 상점 창에 보이는 값은 클라이언트 `Item/ITEM.dat`이며 원래부터 서로 달랐다(대형 600 / 250). 그래서 두 곳 모두 대형보다 20% 비싸게 맞췄다.

## 기존 물약과 같은 동작

- 종류 44(포션): 한 칸 최대 9999개(`stack_limits.h`), 같은 물약을 다시 사면 기존 칸에 합쳐진다. 상점에서 개수를 입력해 여러 개 산다. 값은 가격 × 개수다(이 코드의 `MIN(cnt,1)`은 "최소 1"이라는 뜻).
- 판매·거래·창고·드롭은 모두 종류 44 기준이라 수정이 필요 없었다.
- 마시기와 소리만 아이템 번호로 나뉘어 있어 `dHP_POSION_XL`을 추가했다: `message.cpp`의 두 사용 분기(인벤토리 더블클릭 `PACKET_ItemDBClick`, 단축키 `PACKET_UseQuickItem`), `UseHPPotion`의 소리(대형 포션 소리). 서버 빌드는 `message.cpp` 전체가 아니라 보관된 옛 `message.o`에 일부 함수만 소스로 바꿔 끼우므로, 이 두 함수를 `tools/build-server.sh`의 교체 목록에 추가했다(처음 빌드에서는 목록에 없어 마셔도 반응이 없었다).
- 클라이언트 `UInterface.dll`은 여섯 물약 번호만 상점 개수 입력창(`EDT_POSIONBUY_NUM`)을 열고, 네 곳에서 개수 0을 1로 바꾼다. 다섯 곳 모두 19132를 받도록 `.xlpot` 섹션 훅을 추가했다(`ui-hooks.json`, 기존 `.stk16`·`.imeidle`·`.bmsnd` 위에 덧붙임). 두 적재 주소에서 338개 경로를 에뮬레이션으로 확인했다.
- 판매 상점: 물약 상인 9곳(상점 9, 12, 15, 18, 21, 24, 30, 34, 36)에서 대형 포션 바로 뒤에 판다. 재뽕 창고 `기타`에도 있다.

재생성: `runtime/python/python.exe -X utf8 tools/register_extra_large_potion.py`, `runtime/python/python.exe -X utf8 tools/patch_extra_large_potion_client.py`. 실서버 검사는 `tools/verify_bluemoon_armor_potion_live.py`(푸른달 갑옷과 함께).

실서버 검사(2026-10-01, `assets/blue-moon-armor/live-validation.json`): 상점 9 판매, 10개 7,200(대형 10개 6,000), 두 번째 구입이 같은 칸에 합쳐짐(15), 더블클릭·단축키 모두 600 회복(대형 120), 재접속 후 수량 유지.
