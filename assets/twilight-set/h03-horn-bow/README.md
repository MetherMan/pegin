# H03 월뿔 활 · 승인 원화 기반 게임 모델

사용자가 선택한 [H03 원화](../../visual-refresh-20260923/bow-horns-10/bow-03.png)를 기존 롱보우 ID에 맞춰 제작했다. 원화의 비대칭 휘어진 뿔, 작은 초승달, 별 장식, 손잡이, 시위를 보존했다. 원본 PNG는 수정하지 않았다.

[원화 / 실제 MOD·WTM 비교 뷰어](index.html) · [네이티브 GLB](h03-horn-bow-native.glb) · [작성용 GLB](h03-horn-bow.glb) · [수치 검증](build-validation.json) · [육안 검수](visual-validation.json)

정면 텍스처는 원화의 505×1442 영역을 그대로 추출해 2048 아틀라스에 축소 없이 배치했다. 네이티브 WTM을 다시 풀어 원화 728,210픽셀과 옆면·시위 재질이 정확히 같은지 확인했다. 뿔은 폭에 따라 둥글게 두께가 변하는 앞뒤 면과 닫힌 옆면이며, 손잡이와 시위에는 닫힌 원통을 사용했다.

- 단일 네이티브 메시, 13,381개 정점, 26,580개 삼각형. 기존 활은 10,328개 삼각형이었다.
- 열린 모서리 0, 비정상 공유 모서리 0, 뒤집힌 연결부 0. 닫힌 구성요소 5개 모두 양의 체적.
- 기존 전장 대비 100.13%. 원화의 그립 중심을 기존 장착 원점으로 맞췄다.
- 기존 `wb_0040_1.mod` 헤더와 장착 좌표축, `mt_longbow` 텍스처·장비 이름, 3개 변형을 유지했다. 세 MOD의 바이트는 동일하다.
- 검·지팡이 8개 리소스, 두 무기의 네이티브 GLB, 아이템 표·장착 연결·H03 원화 등 14개 보호 파일의 해시가 동일하다.

정면, 사선 60°, 옆면, 뒷면, 조명 없는 원화색 화면을 직접 확인했다. 실제 게임 D3D 렌더링은 이 검수에 포함하지 않았다. 설치본 반영은 통합 작업에서 별도로 수행한다.

| 리소스 | 바이트 | SHA-256 |
|---|---:|---|
| Equip/mt_longbow_1.mod | 2074492 | 632443413eeb81254cafeea185347b72d659b119d08c6654ca94208b5142fd7c |
| Equip/mt_longbow_2.mod | 2074492 | 632443413eeb81254cafeea185347b72d659b119d08c6654ca94208b5142fd7c |
| Equip/mt_longbow_3.mod | 2074492 | 632443413eeb81254cafeea185347b72d659b119d08c6654ca94208b5142fd7c |
| Texture/Equip/mt_longbow_atlas.wtm | 1079751 | 56d3f3077ca72da189ec94cd80067b8ed3c9fd96729ed158ffacb89d4f8d2f72 |

재생성 명령: 저장소 루트에서 `runtime\python\python.exe -B -X utf8 tools\build_h03_horn_bow.py`. 설정은 [geometry.json](geometry.json)이며 `build_bow(spec=None)`가 모델 결과를 반환한다. 독립 빌더는 H03 폴더, overlay 활 4개 리소스와 `longbow-native.glb`만 기록한다. 실제 설치본·스킬·공용 뷰어·배포 manifest는 기록하지 않는다.
