# 금색 기사 · 천상의 날개 무기

## 게임 등록 (Rank 10)

| 무기 | 번호 (+0~+20) | +0 공격 (트와일라잇) | +20 공격 (트와일라잇) | 모델 |
|---|---|---|---|---|
| 천상의 날개 양손검 | 19260~19280 | 293~327 (224~259) | 3895~4150 (3015~3305) | `Equip/cw_sword_{1,2,3}.mod` |
| 천상의 날개 롱보우 | 19290~19310 | 264~285 (197~218) | 3531~3651 (2693~2839) | `Equip/cw_longbow_{1,2,3}.mod` |
| 천상의 날개 스태프 | 19320~19340 | 공격 87~94 · 마법 181~189 (67~74 · 140~148) | 공격 1629~1658 · 마법 2617~2632 (1283~1320 · 2074~2100) | `Equip/cw_staff_{1,2,3}.mod` |

정확한 단계별 수치는 `item_definitions.json`.

- 능력치: 단계마다 같은 종류의 트와일라잇 무기(Rank 9) 행을 복사하고, 숫자 열마다 한 랭크만큼 더 올렸다. 한 랭크의 비율은 같은 종류의 블랙나이트 / 레드나이트(Rank 6/5) 비율을 21단계에 직선으로 맞춘 값이다(푸른달 갑옷과 같은 방식, `tools/register_bluemoon_armor.step_line`). 공격·마법·가격·클라이언트 보너스 열이 대상이고, 착용 제한·무게는 트와일라잇과 같다(없음). 강화 확률은 코드 수정 없이 기존 무기 표를 그대로 쓴다.
- 이름 색: 금색 `#FFD966`. `Engine.dll`의 트와일라잇 색 검사 세 곳 앞에 `.cgold` 훅으로 19260~19340을 먼저 검사한다. 트와일라잇·푸른달 갑옷의 하늘색과 나머지 기본 색은 그대로다(`tools/patch_celestial_colors.py`, 두 적재 주소 112경우). 가격은 19000번 이후라 `.shild`로 실드 표시.
- 재뽕 창고: `세트` 탭 다섯 번째 묶음 `천상의 날개`(양손검 자리 2, 롱보우 7, 스태프 9).
- 금색 검선: `celestial_gold`(트레일 `Effect/trace.tdf`, 텍스처 `Texture/Effect/cw_sword_trail.wtm`). 성기사의 양손검 검선(검흔C, 붉은 swd08)의 무늬·프레임·속도를 그대로 두고 밝기를 금색 단계(검정→호박→금→연한 금)로 다시 칠했다. `Equip/Trace.txt`의 `cw_sword_{1,2,3}.mod`가 이 검선을 쓰고 칼날 뿌리 0.609에서 끝 1.657까지 그린다. 접속 직후에는 게임이 검선을 흰 기본값으로 만들기 때문에(트와일라잇 때 확인한 원래 동작) DeicideOnline `.cwfx` 훅이 장착 무기의 검선 이름이 `celestial_gold`이면 그대로 쓴다(기존 `twilight_blue` 처리는 그대로).
- 금색 화살: 기본 화살(`Magic/arw_d1.ms` 단궁기본)을 복제한 `cw_arrow`(투사체 `cw_arrow.wed`, 타격 `cw_arrow_hit.wed`, 잔상 `cw_arrow_trace`). 효과 색(흰색)은 금색(1.0, 0.80, 0.32)으로 곱하고 잔상 텍스처는 금색 단계로 다시 칠했다. 모양·움직임·소리·타이밍은 원본과 같다. DeicideOnline `.cwfx` 훅이 활 19290~19310에 `cw_arrow`를 고르고, 트와일라잇 롱보우의 푸른 화살(19060~19080)과 다른 활(단궁기본)은 그대로다(`tools/patch_celestial_effects_client.py`, 검선 8경우·화살 31경우를 실제 함수로 에뮬레이션). 식스폴드 샷의 파란 연출은 트와일라잇 활 전용이라 이 활은 기본 연출이다. 실제 게임 화면 검수는 하지 않았다.
- 아이콘(`icons/`): 무기 그림을 대각선으로 눕혀 대표 부분(검 날개·코어, 지팡이 머리)을 크게 잡고 트와일라잇·블랙나이트와 같은 검은 바탕 + 1px 금테에 넣었다.

## 3D 모델

`models.html`(원본 3D / 게임 파일 전환). 컨셉 3번 "천상의 날개"를 Codex 이미지 생성으로 무기별 단독 그림(`concepts/celestial-*.png`, 순검정 배경, 바깥 빛 번짐 없음)으로 다시 뽑고, 트와일라잇 무기와 같은 방식으로 3D로 만들었다(`tools/build_golden_knight_models.py`, 사양 `models/geometry.json`): 실루엣 다각형마다 두께와 둥근 모서리를 가진 닫힌 몸체를 만들고 앞뒤에 그림을 그대로 투영, 손잡이·지팡이 자루는 둥근 원통, 활시위는 얇은 원통. 길이는 트와일라잇 양손검 2.09 / 롱보우 2.00 / 스태프 1.81과 같다. 게임 파일(`tools/register_celestial_weapons.py`)은 같은 종류의 원본 무기(블랙나이트 양손검 `ws_0081`, 화이트헌터 롱보우 `wb_0040`, 달의정령 스태프 `ww_0015`)를 기준으로 방향·감김을 맞춰 하나의 메시와 원본 해상도 2048 텍스처로 내보낸다. 세 모델 모두 닫힌 면 검사 통과(양손검 17,972 · 롱보우 11,176 · 스태프 12,388 삼각형).

## 스킬 제안 미리보기

- `skills/sevenfold.html` 칠익난무(근거리 140 제안): 실제 양손검 스킬 모션 위에 금빛 초승달 7연참과 X섬광·깃털 폭발 마무리. 게임에는 아직 없음. 데이터 `tools/build_sevenfold_preview.py`.

## 컨셉과 이전 시안

- `concepts.html`: Codex 이미지 생성 컨셉 보드 3장(태양 성기사 / 왕관 사자 / 천상의 날개). 프롬프트는 `concepts/prompts.json`. 사용자가 3번을 골랐다.
- `index.html`: 처음 시안. 게임의 상위 무기 원본 모델에 금색 / 백금 색만 입힌 비교(`tools/build_golden_knight_weapons.py`).

재생성: `runtime/python/python.exe -B -X utf8 tools/build_golden_knight_models.py`(미리보기), `tools/build_celestial_effects.py`·`tools/patch_celestial_effects_client.py`(금색 검선·화살), `tools/register_celestial_weapons.py`(게임 파일·아이템), `tools/patch_celestial_colors.py`(이름 색), `tools/readback_celestial_weapons.py`(게임 파일 미리보기). 빌드·실서버 검사: `tools/build_and_verify_celestial_weapons.py`.
