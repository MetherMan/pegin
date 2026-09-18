# 푸른달의 트와일라잇 양손검

Rank 9 / 양손검 / 무게 0 / 착용 능력치 제한 없음. 이름 #72CFFF, Rank #348BFF.
`/재뽕 테스트 2`로 0강 한 개 지급. 0강 224~259, 20강 3015~3305. 이 검에만 푸른 검선이 적용됩니다.

## 현재 모델

흩날리는 붕대·실·천 장식을 제거하고 손잡이를 짙은 가죽으로 변경했습니다. 주 칼날의 넓은 절삭부를 손잡이 방향으로 연장했으며, 달 문양과 돌출 초승달의 정렬을 유지합니다. 날끝·손잡이 위치와 전체 길이는 그대로입니다. 총 12,990 삼각형입니다.

게임 좌표상 전체 길이 2.0973, 비교 대상 블랙나이트 스피어는 2.2121입니다. 손에서 날끝까지는 둘 다 1.6058입니다. `size_spec.json`의 72.9143% 배율을 유지합니다.

## 미리보기와 색 조절

`모델_돌려보기.html`에서 회전·확대·옆면·뒷면과 **블랙나이트 크기 비교**를 사용할 수 있습니다. 오른쪽에서 푸른 문양의 색·채도·밝기, 은빛 금속의 색·거칠기를 조절합니다. 기존 브라우저 저장값은 유지합니다. 삭제한 붕대 조절은 숨겼습니다.

**설정 파일 저장**으로 `twilight-colors.json`을 내보내면 다음 게임 텍스처 반영에 사용할 수 있습니다. 미리보기의 색 조절은 게임 파일이나 아이콘에 자동 적용되지 않습니다.

## 소스와 검증

Node로 `build_model.mjs`, NumPy/Pillow Python으로 `convert_native.py`를 실행합니다. `prepare_preview.py --viewer`로 미리보기를 생성합니다. 과거 초안용 `prepare_native.py`는 사용하지 않습니다.
칼날은 `concept_texture.png`, 옆면은 `moonsteel_side_albedo.png`를 사용합니다. 인벤토리 아이콘은 칼날 상단을 담은 `inventory_icon_imagegen.png`입니다.
`blade_revision_validation.json`, `native_model_validation.json`, `asset_validation.json`에 현재 검증 결과를 보관합니다. `live_validation.json`은 등록 시 지급·강화·재접속 확인 기록입니다.

게임 모델과 로컬 Git 작업 폴더에 반영했습니다. GitHub commit/push는 하지 않았습니다. 실제 게임 전투 화면은 별도 확인이 필요합니다.

## 사용자 확정 색상

`game_appearance.json`의 #2962DB / 채도 69% / 밝기 43%, 금속 #AAABC0 / 거칠기 100%를 게임 텍스처에 구웠습니다. 원본 GLB와 원본 그림은 색 재조절을 위해 유지합니다. `convert_native.py`는 이후 재생성에도 이 설정을 적용합니다. 구형 게임의 금속 반사는 끈 상태입니다. 인벤토리 아이콘은 내장 ImageGen으로 같은 색감·검은 배경·금색 테두리에 맞춰 새로 제작했습니다. 프롬프트는 `icon_user_colors_prompt.txt`, 검증은 `appearance_validation.json`에 있습니다.
