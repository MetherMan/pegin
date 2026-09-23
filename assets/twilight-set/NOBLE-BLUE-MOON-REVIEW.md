# 푸른달의 트와일라잇 · 고결한 푸르름

검과 활을 각각 새 ImageGen 원화에서 다시 모델링했습니다. 기존 철제형 모델을
재도색하거나 별도 달 문양을 덧붙이지 않았습니다. 원화의 큰 초승달이 무기
몸체와 실제 열린 실루엣을 구성합니다. 스태프는 이전 파일을 그대로 유지합니다.

## 원화와 모델

- 검 원화: `../visual-refresh-20260923/noble-blue-moon-sword-imagegen.png`
- 활 원화: `../visual-refresh-20260923/noble-blue-moon-bow-imagegen.png`
- 빌더: `../../tools/build_noble_moon.py`
- 기하 설정: `noble-moon-geometry.json`
- 실제 모델 재독출: `sword-native.glb`, `longbow-native.glb`

원화 알파 175 이상인 실체를 닫힌 입체 모델로 만들었습니다. 주변 오라는
메시로 만들지 않았으며 활의 내부 빈 공간 네 곳을 보존했습니다. 원본 PNG는
변경하지 않았고, UV 배치와 원화 영역 자르기만 수행했습니다. 측면 또한 새
원화의 청색 재질을 사용합니다. 이전 회색 금속 텍스처는 사용하지 않습니다.

최종 아틀라스는 2048² 크기를 유지하면서 검 387×1522, 활 434×1507 원화 영역을
축소 없이 복사합니다. 공통 정사각 타일의 세로 1008픽셀 제한을 없앴습니다.
실제 WTM을 해제해 1,243,052개 원화 픽셀이 정확히 일치하는지 확인했습니다.
아틀라스 변경 전후 3D 위치와 법선 바이트의 SHA-256은 동일합니다.

검의 칼날은 중심에서 절삭부까지 긴 경사를 가지며 첨단 쪽으로 두께가 줄어듭니다.
실제 MOD 단면: 중앙 전체 두께 0.03741188, 첨단 0.00505389 모델 단위.
426개 칼날 정점이 반두께 0.00027 미만의 얇은 날끝을 구성합니다.

## 검증과 범위

- 검 9,688삼각형, 활 10,328삼각형. 기존 한 개 애니메이션 메시와 3개 LOD 유지.
- 닫힌 면, 법선, UV, 재질, 42개 아이템 연결 검사 통과.
- 밝은 표면을 면적으로 가중한 청색 판정: 검 95.3%, 활 92.9%.
- 스태프 MOD 3개와 WTM 1개 SHA-256이 빌드 전후 동일.
- `active-design.json`과 기존 빌드 진입점은 `noble-blue-moon`을 선택합니다.
- 빌더는 `client-overlay`와 제작용 출력만 기록하며 설치본이나 runtime을 수정하지 않습니다.

자세한 수치와 해시는 `noble-moon-validation.json`, `noble-moon-native-validation.json`,
`asset-validation.json`에 기록했습니다.

## 육안 검수

아래 이미지는 실제 MOD/WTM을 다시 읽은 브라우저 렌더입니다. 원화의 방향과
실루엣을 대조했고 검의 정면·사선·날끝 옆면을 확인했습니다. 실제 Direct3D
게임 캡처는 아니며 장착 손 위치는 캐릭터 크기 미리보기에서 별도로 확인합니다.

- `../visual-refresh-20260923/noble-bow-native-front.png`
- `../visual-refresh-20260923/noble-bow-native-quarter.png`
- `../visual-refresh-20260923/noble-sword-native-front.png`
- `../visual-refresh-20260923/noble-sword-native-quarter.png`
- `../visual-refresh-20260923/noble-sword-native-edge.png`

```powershell
.\runtime\python\python.exe -B -X utf8 tools/build_noble_moon.py
.\runtime\python\python.exe -B -X utf8 assets/twilight-set/preview_native.py
.\runtime\python\python.exe -B -X utf8 tools/verify_noble_moon.py
```
