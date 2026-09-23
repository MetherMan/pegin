# 월식 갈고리 지팡이

사용자가 선택한 [P3 원화](../../visual-refresh-20260923/moon-pairs-10/pair-03.png)의
오른쪽 지팡이를 제작했다. 원본은 그대로 보존하고, 지팡이 영역만 정상 추출해
앞뒤 투영 UV와 측면 재질로 사용했다. 새 이미지 생성·채색·리사이즈는 하지 않았다.
큰 푸른 달은 볼륨을 가진 렌즈형 입체이며 긴 갈고리·작은 갈고리·장식·빈 공간을
원화에 맞췄다. 자루는 둥근 단면이다.

## 제작과 검증

```powershell
.\runtime\python\python.exe -B -X utf8 tools/build_eclipse_hook_staff.py
.\runtime\python\python.exe -B -X utf8 assets/twilight-set/verify_assets.py
```

지팡이 전용 빌더와 `geometry.json`을 사용한다. `active-design.json`의 staff
override에 선택을 기록했으며 검·활 빌더를 호출하지 않는다. native 미리보기는
`preview_native.py --kind staff`로 해당 무기만 다시 읽는다.

- 5,553정점·10,996삼각형, 단일 애니메이션 메시, 기존 `ww_0015_1.mod` 축·헤더와 세 LOD 유지.
- 기존 지팡이 길이 대비 99.89%, 손 원점을 가로지르는 둥근 자루와 기존 장착 본 유지.
- 원화 298×1008 영역의 300,384픽셀을 WTM 아틀라스에서 1:1 대조. 원본 SHA 유지.
- 열린 경계·비정상 연결·뒤집힌 면 모두 0. 앞뒤와 측면이 닫힌 6개 구성체.
- 검·활·아이템 표·원화 등 제작 보호 14개 동일, v10 기준 설치 보호 59개와 검 overlay 4개도 동일.
- 42개 아이템의 서버/클라이언트/툴팁 수치, LOD·WTM·UV 연결 검사 통과.

기록은 [build-validation.json](build-validation.json), [item-validation.json](item-validation.json),
[scope-validation.json](scope-validation.json), [browser-validation.json](browser-validation.json)에 있다.
이 빌더는 실제 설치본을 직접 쓰지 않는다. 최종 화면 검수 후 별도 설치 절차로
2026-09-23 15:48:37에 아버지 설치본과 작업공간 runtime에 지팡이 4개 파일씩,
총 8개를 백업·교체했다. 보호 파일 59개·기존 실행 경로·검·활·설치된 스킬은
유지했다. [설치 결과와 백업 경로](installation.json)를 참조한다.

## 교체할 게임 파일

| client-overlay 기준 경로 | 바이트 | SHA-256 |
| --- | ---: | --- |
| `Equip/mt_staff_1.mod` | 858,508 | `0076e75e089a2aa6dd0764f367a1ac611efc75299a0e57c61f774d498acc79c2` |
| `Equip/mt_staff_2.mod` | 858,508 | 위와 동일 |
| `Equip/mt_staff_3.mod` | 858,508 | 위와 동일 |
| `Texture/Equip/mt_staff_atlas.wtm` | 473,189 | `0e81ca9ca31d9fc781c529dbcf65daca5a41fa9f50c4aa6df4ebbc87dbf48ae1` |

## 화면 검수

[모델 뷰어](../index.html?weapon=staff)는 실제 MOD/WTM을 재해석한다.
`native-front.png`, `native-quarter.png`, `native-edge.png`, `native-back.png`에서
원화 실루엣·열린 공간·달 볼륨·측면을 확인했다. `actor-140px-front.png`와
`actor-240px-front.png`는 실제 캐릭터 본과 같은 스케일로 장착한 화면이다.
작은 화면에서도 푸른 달과 갈고리가 읽히며 수집된 콘솔 오류는 없다.
실제 게임 Direct3D 전투·성능 검수는 수행하지 않았다.
