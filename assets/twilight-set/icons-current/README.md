# 푸른달 트와일라잇 — 적용된 인벤토리 아이콘

2026-09-24 사용자가 승인한 ImageGen 그림을 적용했다.

| 무기 | 승인 원본 | 구도 |
| --- | --- | --- |
| 검 | `../icons-gold-20260923/sword-v4.png` | 완만한 칼날과 열린 초승달 확대 |
| 활 | `../icons-gold-20260923/longbow-v4.png` | 중앙 손잡이와 위아래 활대, 끝부분만 잘림 |
| 지팡이 | `../icons-gold-20260923/staff-v2.png` | 달·갈고리 머리와 연결부 확대 |

생성 프롬프트는 원본 폴더의 `prompt-sword-v4.json`,
`prompt-longbow-v4.json`, `prompts-v2.json`에 보관한다.
이전 시안과 readable 원본은 기록용이며 현재 빌드에 사용하지 않는다.

`manifest.json`의 `art_box`는 생성 그림의 외곽 금테두리만 제외한다.
`tools/build_current_weapon_icons.py`는 내부 그림을 24×24로 축소한 뒤,
블랙나이트 `ws_0081.wtm`에서 가져온 2픽셀 금테두리를 붙인다.
최종 28×28 RGB24 BMP를 TEAMMAY WTM으로 저장한다.
무기 그림을 코드로 다시 그리거나 색을 바꾸지 않는다.

실행: `runtime/python/python.exe -B -X utf8 tools/build_current_weapon_icons.py`

세 무기의 기존 빌드 경로도 이 manifest를 사용한다.
`client-overlay/Item`과 `client-overlay/Texture/Body` 양쪽의
`mt_twilight_icon.wtm`, `mt_longbow_icon.wtm`, `mt_staff_icon.wtm`이 대상이다.
아이템 ID 19030–19050, 19060–19080, 19090–19110의 인벤토리·착용 슬롯에서
기존 참조를 그대로 사용한다.

이 PC에서 확인한 `runtime/client/GameClient`에 같은 6파일을 백업 후
설치했고, 배포 manifest의 해당 6항목 해시와 버전을 갱신했다.
모델·텍스처·아이템 정의 등 보호 파일 68개의 해시는 유지됐다.
설치·백업 경로와 검증은 `installation-20260924.json`,
변환 결과는 `validation.json`을 참고한다.
원격 PC 배포와 게임 화면 직접 확인은 하지 않았다.

`index.html`은 원본 게임 아이콘과 실제 크기·4배 확대 비교,
`approved-preview.png`는 적용된 세 아이콘의 실제 크기·8배 확대이다.
