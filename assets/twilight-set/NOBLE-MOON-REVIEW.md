# 푸른달의 트와일라잇 · 최종 모델 기록

검과 활은 새 ImageGen 원화 전체를 기준으로 다시 모델링했습니다. 별도 달
장식을 추가하지 않았으며 초승달 자체가 몸체와 열린 실루엣을 이룹니다.
기존 회색 금속 텍스처는 사용하지 않습니다. 스태프 4개 파일은 동일합니다.

현재 `active-design.json`은 `noble-blue-moon`을 선택합니다.
`tools/build_twilight.py`와 `tools/build_twilight_set.py`도 새 빌더로 연결되어
이전 디자인으로 되돌아가지 않습니다.

## 최종 파일과 결과

| 항목 | 검 | 활 |
| --- | --- | --- |
| ID 및 파일 접두사 | `mt_twilight` 유지 | `mt_longbow` 유지 |
| 삼각형 | 9,688 | 10,328 |
| 애니메이션 메시 | 1 | 1 |
| LOD | 동일 형상의 1·2·3 | 동일 형상의 1·2·3 |
| 원화 면 해상도 | 387×1522 | 434×1507 |
| 아틀라스 | 2048² | 2048² |
| 밝은 표면의 청색 면적 비율 | 95.3% | 92.9% |
| 최종 MOD SHA-256 | `5778ca345c00514b1070beab36814011c99c28cbf61f79f545541e0da5ea8ddd` | `aa8798210a1e56bdd475c360f3d0c01f519218660fb62dfddf134b33cc838427` |

정사각 1008픽셀 타일로 줄이던 과정을 제거해 원화 영역을 1:1로 보존했습니다.
실제 WTM의 1,243,052픽셀을 원화와 대조했고 완전히 일치합니다. 패킹 변경
전후 모델 위치·법선 바이트도 동일하므로 형상 변화는 없습니다.

검은 중앙 전체 두께 0.03741188에서 첨단 0.00505389 모델 단위로 가늘어집니다.
날끝 426정점은 반두께 0.00027 미만입니다. 손잡이는 둥근 입체이며 손 기준점과
기존 네이티브 축·애니메이션 연결을 유지했습니다. 활의 내부 빈 공간 네 곳을
보존하고 시위는 끊기지 않는 별도 입체로 만들었습니다.

검증 보고서:

- `noble-moon-validation.json`: 빌드 설정, 원화 해시, 아틀라스, 스태프 보존.
- `noble-moon-native-validation.json`: WTM 원화 픽셀 일치, 청색 표면, 검 단면, 닫힌 면.
- `closed-surface-validation.json`: 열린 면·비정상 연결·뒤집힌 면 모두 0.
- `asset-validation.json`: UV·재질·LOD와 42개 아이템 연결 통과.

## 재생성

```powershell
.\runtime\python\python.exe -B -X utf8 tools/build_noble_moon.py
.\runtime\python\python.exe -B -X utf8 assets/twilight-set/preview_native.py
.\runtime\python\python.exe -B -X utf8 tools/verify_noble_moon.py
.\runtime\python\python.exe -B -X utf8 assets/twilight-set/verify_closed_surfaces.py
.\runtime\python\python.exe -B -X utf8 assets/twilight-set/verify_assets.py
```

원화·기하 설정: `noble-moon-geometry.json`.
검·활 빌더: `tools/build_noble_moon.py`.
픽셀 보존 패커: `tools/noble_moon_atlas.py`.

최종 모델 파일은 고정했습니다. 정면·사선·검 날끝 옆면을 native MOD/WTM
재독출 화면으로 확인했습니다. 실제 게임 Direct3D 전투 캡처는 아닙니다.
캐릭터 140/240px 장착과 최종 픽셀 패킹 이후 정면 화면도 확인했습니다.
이 빌더는 제작용 출력과 `client-overlay`만 기록하며 설치본·runtime·계정·실행기를
수정하지 않습니다. 설치 반영 기록은 부모 작업의 보호 파일 해시 검증 결과를 따릅니다.

최종 검·활 8파일은 기존 두 로컬 클라이언트에 각각 반영했습니다. 총 16파일을
백업·교체했고 실행기·설정·표·지팡이·설치된 스킬 59파일은 동일합니다.
`noble-moon-installation.json`에 최종 해시와 `noble-blue-moon-20260923-141001`
백업 경로를 기록했습니다. 새 스킬은 미리보기용이며 DB·VM·계정은 수정하지 않았습니다.
