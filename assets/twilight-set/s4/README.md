# S4 달빛 물결 검

사용자가 선택한 `assets/visual-refresh-20260923/noble-moon-15/sword-04.png`를
검 전체의 새 원화로 사용했습니다. 원본 PNG는 변경하지 않았습니다.
긴 S곡선 칼날, 왼쪽의 열린 초승달, 오른쪽으로 흐르는 가드, 작은 장식 체인을
입체 실루엣으로 보존했습니다. 초승달 문양을 별도 부품으로 덧붙이지 않았습니다.

## 검 전용 제작

```powershell
.\runtime\python\python.exe -B -X utf8 tools/build_noble_s4_sword.py
.\runtime\python\python.exe -B -X utf8 tools/verify_noble_moon.py
.\runtime\python\python.exe -B -X utf8 assets/twilight-set/verify_assets.py
```

설정은 `../noble-moon-s4-sword-geometry.json`입니다.
`active-design.json`에는 검만 S4로 지정한 override와
`default_build_kinds: ["sword"]`가 있습니다. 기본 `build_noble_moon.py`도
검만 제작하며, 검 전용 빌더는 활·스태프·아이템 파일 13개의 해시를 검사합니다.
`preview_native.py --kind sword`로 검 GLB만 갱신합니다.

## 이전 검 대비

| 항목 | 이전 푸른달 검 | S4 |
| --- | --- | --- |
| 삼각형 | 9,688 | 11,332 |
| 손 원점에서 첨단까지 | 1.61030 | 1.61291 |
| 전체 길이 | 2.00228 | 1.95013 |
| 최대 가로 폭 | 0.49660 | 0.38689 |
| 칼날 중앙 전체 두께 | 0.03741 | 0.03751 |
| 칼날 첨단 전체 두께 | 0.00505 | 0.00409 |

수치는 모델 단위입니다. 기존 손 기준 원점, 축 방향, `ws_0081_1.mod` 참조,
단일 애니메이션 메시, 손잡이 반지름 0.021, 아이템 ID와 3개 LOD를 유지했습니다.
끝으로 갈수록 단면이 얇아지며 500개 칼날 정점이 반두께 0.00027 미만입니다.

원화의 312×1496 영역, 총 466,752픽셀을 축소 없이 2048² 아틀라스에 넣었습니다.
실제 WTM을 풀어 원화와 픽셀 단위 일치를 확인했습니다. 정면·사선·옆면·뒷면
렌더에서도 원화의 물결 윤곽과 열린 초승달 공간을 확인했습니다. 열린 면,
비정상 연결, 뒤집힌 면은 모두 0입니다.

## 교체할 파일

`client-overlay` 기준으로 아래 네 파일만 설치 대상입니다.

- `Equip/mt_twilight_1.mod`
- `Equip/mt_twilight_2.mod`
- `Equip/mt_twilight_3.mod`
- `Texture/Equip/mt_twilight_atlas.wtm`

세 MOD는 동일하며 각각 884,332바이트입니다.
SHA-256: `5e5267cd8bedf5ee39060dc19e8f4d68d073199ca61ca2cc2cff8be8911adc3d`

WTM은 753,583바이트입니다.
SHA-256: `4cc1289c6876b1d4d462869f31151c2b75ad22da346ef419496290b78c5ffaaf`

제작 검증과 교체 파일 해시는 `build-validation.json`, 실제 설치 결과와 백업
경로는 `installation.json`에 있습니다. 2026-09-23 14:58:21에 아버지 설치본과
작업공간 runtime의 검 파일을 각각 4개씩, 총 8개 백업 후 교체했습니다.
보호 파일 67개의 해시는 모두 유지되었으며, 기존 실행 경로·활·스태프와 스킬
설치 파일도 그대로입니다. 이전 `noble-moon-installation` 기록은 보존했습니다.
실제 게임 전투 화면 검수는 아직 수행하지 않았습니다.

## 검수 화면

- `native-front.png`, `native-quarter.png`, `native-edge.png`, `native-back.png`
- `actor-140px-turned.png`, `actor-240px-turned.png`
- `prior-sword-native.glb`: 이전 검 비교용 보존본

무기 뷰어 `index.html?weapon=sword`는 S4를 바로 열고 시안 링크도 S4를 가리킵니다.
캐릭터 미리보기 데이터를 갱신해 손 위치와 140px·240px 크기에서 확인했습니다.
면이 보이는 각도에서는 푸른 물결과 초승달이 읽히며, 완전히 옆을 보는 각도에서는
얇은 칼날로 보입니다. 이는 native 파일의 브라우저 재현 검수이며 실제 게임
Direct3D 전투 캡처는 아닙니다.

2026-09-23 후속 비율 조정: 손잡이·가드·끝장식은 그대로 두고 칼날만 길이 12%, 정면 폭 10%, 옆 두께 15% 늘렸다. 칼날 뿌리부터 100px 구간은 변화량이 부드럽게 이어진다. 보호 영역 2,277정점과 관련 코너는 기존 바이트 그대로이며, 전체 11,332삼각형과 UV·텍스처도 유지한다. `blade-refinement-validation.json`에 원본/수정본 해시와 보호 영역 검증을 기록했다. `Equip/Trace.txt`의 이 칼에 해당하는 세 끝점만 늘어난 칼끝에 맞춰 연장했다.

최초 수정은 `tools/refine_s4_blade.py`로 기존 네이티브 모델의 칼날 정점과 법선만 변형했다. 원본 해시 검사는 중복 적용을 막는다. 앞으로의 원화 재빌드에는 같은 변형 함수가 적용되며, 보호 영역·UV 검증을 통과하기 전에는 overlay 모델을 교체하지 않는다.
