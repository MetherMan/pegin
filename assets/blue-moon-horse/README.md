# 푸른달 말

원본 갈색말 `Vehicle/horse_1.mod` + `Texture/Vehicle/horse21.wtm`을 바탕으로 한 미리보기다. 원본 32개 본·171프레임 애니메이션·정점/UV를 그대로 두고, 텍스처 색만 바꾸고 유니콘 뿔을 추가했다. 지옥마는 사용하지 않았다.

- 몸 색 3종: 심야 청마 / 달빛 은청 / 황혼 청보라. 원본 명암을 푸른 팔레트로 옮겼다.
- 갈기·앞머리·꼬리: 하늘색. 처음 풍성하게 만든 입체 털 뭉치 방식을 유지하고 다듬었다(갈기판 방식 시안은 폐기). 털 뭉치 95개: 목 바깥 갈기 40, 목에 붙은 어두운 아랫겹 20(틈 메움), 목 윗선에서 뒤로 눕는 갈기 12, 앞머리 9, 꼬리 14. 더 가늘고 납작한 다발로 바꾸고 끝을 바늘처럼 뾰족하지 않게 부드럽게 좁혔으며, 서 있던 윗선 뭉치는 뒤로 눕혔다. 털결 텍스처 대비를 높였다. 갈기 끝은 기갑에서 낮고 짧아져 안장 앞머리 밑으로 들어가고, 고삐보다 위에 머문다. 목·머리·꼬리 본에 연결, 목·얼굴·뿔은 실제 삼각형 기준으로 밀어냈다.
- 뿔: 2줄 나선 홈의 진주빛 원뿔. 뿌리는 어두운 은청색, 끝으로 갈수록 밝아져 끝은 흰색·하늘빛. 257정점/496삼각형, 머리 본(`Bip02 Head`)에 연결해 대기·달리기에서 머리를 따라간다. 원본 청크 0 뒤에 붙였고 원본과 같은 법선/UV/감김 방향 형식을 쓴다.
- 안장: 원본 안장 / 푸른달 안장(남색 천·은청 줄무늬·남색 좌석) 선택. 고삐·끈은 원본.
- 눈: 푸른 안광. 흰색 중심에서 파란색으로 번지는 눈과 주변의 푸른 빛 번짐을 텍스처에 그렸다. 발광 재질이 아니므로 게임에서 블룸처럼 빛나는 효과는 없다.

추가 형상 합계: 정점 4165 / 삼각형 7864 (원본 393 / 718).

미리보기: 저장소 루트에서 HTTP 서버 실행 후 `/assets/blue-moon-horse/`.
재생성: `runtime/python/python.exe -B -X utf8 tools/build_blue_moon_horse.py`.

아이템·탑승 종류·속도 등록, 게임 설치, 자동업데이트 manifest 반영은 하지 않았다. 색·안장 선택 후 지옥마와 같은 방식(별도 MOD/WTM, 소유증서, 탑승 종류)으로 등록한다. `payload/Vehicle/mt_bluemoonhorse.mod`는 뿔 포함 네이티브 형식 재파싱 검증용이다.

## 게임 등록 (2026-09-30)

사용자 요청: 소유증서 형태, 이동속도 120, 재뽕 창고 등록. 외형은 미리보기 기본값인 심야 청마 + 원본 안장이다(`tools/register_blue_moon_horse.py`의 `COAT`, `SADDLE`).

| 항목 | 값 |
|---|---|
| 아이템 | 19131 `푸른달의 말 소유증서` (지옥마 19130 행 복제: 종류 41, 가격·바닥 모델 동일) |
| 탑승 종류 | 5 |
| 이동속도 | 120 (클라이언트 12.0, 맨몸 대비 3배) |
| 지급 | `/재뽕 말 푸른달`, `/재뽕 말 5`, 재뽕 창고 말 분류 |

- 아이콘: 원본 갈색마 소유증서 28×28 픽셀 그림을 그대로 두고 몸 → 짙은 파랑, 흰 갈기 → 하늘색, 눈 → 푸른 빛, 귀 사이에 진주색 뿔 픽셀. `certificate-comparison.png`, `tools/build_blue_moon_certificate.py`.
- 클라이언트 자원: `Vehicle/mt_bluemoonhorse.mod`·`.act`, `Texture/Vehicle/mt_bluemoonhorse.wtm`, `Item`·`Texture/Body/mt_bluemoonhorse_icon.wtm`, `Item/ITEM.dat`·`Interface/item.dat` 행 추가. 서버 `game-data/DATA/ITEM_DATA.txt` 행 추가. 세 카탈로그 모두 19131 한 줄만 늘었다.
- `DeicideOnline.exe` `.bmoon`: 종류 5 → `mt_bluemoonhorse` 모델, 기수 모드 7 → 원본 말 자세. 다른 종류는 기존 `.hell` 코드로 그대로 넘어간다. 말 애니메이션 모드(0x408780)와 일반 발굽 먼지는 이미 종류 5를 처리하므로 바꾸지 않았다.
- `UInterface.dll` `.bmsnd`: 19131을 기존 말 울음·아이템 처리 분기에 추가.
- 검증: 모델 선택 8종류, 말 애니메이션 모드 7종류, 기수 모드 9개, 울음 분기 18건(DLL 두 로딩 주소) 기계어 실행 통과(`client-hooks.json`, `sound-ui-hooks.json`). 재실행 시 동일 바이트. 기존 `tests/mount_taros_details_test.py` 통과.
- 서버 소스: `item.h`(dRIDE_ITEM5 19131 / dRIDE_TYPE5 5), `item.cpp`(아이템 사용·접속 시 탑승 복원), `player.cpp` GetRideSpeed 120, `gm_parser`·`gm_commands`(`푸른달`/`5` 지급), `gm_catalog_data`(창고). UTF-8 빌드 입력과 CP949 원본을 같이 수정했다(`tools/add_blue_moon_horse_server.py`).
- 수정 전 소스를 개발 VM에서 빌드한 결과가 현재 `server-bin/LAQIA_GameServer`와 빌드 ID 20바이트 외 동일함을 확인했다(소스가 기준).

서버: 수정된 소스를 개발 VM에서 빌드해 개발 서버와 `server-bin`에 설치했다(sha256 089c576d…). 실제 GetRideSpeed 기계어를 36가지 종류·탑승·도보 보너스 조건으로 실행해 종류 5 = 120, 기존 70/75/80/100과 도보 40(+15) 불변을 확인했다(`speed120-validation.json`). `tools/verify_blue_moon_horse_live.py` 실서버 검사 통과: 임시 캐릭터로 다섯 소유증서 실제 사용 패킷 탑승·하차 70/75/80/100/120, 재접속 후 푸른달 말 120 유지, `/재뽕 말 푸른달`·`/재뽕 말 5` 지급과 `/재뽕 말 지옥마` 유지, 재뽕 창고 목록 표시, 기존 저장 테이블 5개 불변(`live-validation.json`). 재생성·검사: `tools/build_and_verify_blue_moon_horse.py`.

배포 버전 `2026-09-30-blue-moon-horse-speed120`: manifest에 새 말 파일 5개가 추가되고 클라이언트 실행 파일·UInterface·아이템 카탈로그 2종·서버 ITEM_DATA·GameServer 6개가 바뀐다. 자동업데이트 테스트 13개 통과. 아버지는 기존 `자동업데이트.cmd` 실행 후 outputs의 혼자 게임 시작을 사용한다. 실제 게임 화면에서 탑승한 모습과 아버지 PC 적용은 직접 검수하지 않았다.

기존 말과 같은 한계: 탑승 중 소유증서 버리기·판매·휴지통 금지 검사(`message.cpp` 구형 처리)는 원래 갈색·흑·백마 세 종류만 확인하며 지옥마와 푸른달 말은 포함되지 않는다.
