# 2026-09-23 로컬 최종 적용

사용자의 최종 적용 요청에 따라 기존 가족용 `outputs` 실행 경로와 workspace 실행본에 현재 client payload를 적용했다. 이전 v13의 스킬 미설치 기록은 이 적용으로 대체된다.

- S4 검, H03 활, 월식 갈고리 지팡이 및 dark-antler-v3 바포메트.
- 네 스킬의 현재 조절값 64개, 색상 3개와 얼음 불투명도를 지원하는 Render.dll.
- UI에서 선택한 얼음 몸체색 `#527DFF`, 불투명도 90%, 진하기 2.5, 밝기 1.6 보존.
- 두 클라이언트의 1,428개 파일 해시 일치. 1,190개 차이를 적용했고 기존 파일은 모두 먼저 백업했다.
- 실행 CMD·개인 접속 설정·서버 데이터 등 보호 대상 303개 해시 보존. 서버·DB·업데이터를 실행하거나 덮어쓰지 않았다.

백업 경로:

- 가족용: `../https-laqia-co-kr-https-laqia/work/laqia-runtime/backups/final-visuals-20260923-180002-538672` (저장소 형제 폴더).
- workspace: `.cache/final-visuals-20260923-180002-538672`.

`tools/apply_final_visuals.py`는 client 항목만 준비된 해시 계획으로 적용한다. 서버·런처·개인 설정은 대상이 아니다. 실패 시 기존 파일 복원을 모두 시도하고, 새 파일만 제거하며 임시 파일도 정리한다.

카드 드롭 62,400회 및 카드 병합/강화 분기 14건, 경험치·레벨업 51건을 기존 네이티브 코드 검사로 통과했다. 가족 `server-local`, 저장소 `server-bin`, workspace build는 카드·성장 수정을 모두 포함하는 동일한 `ed4a6efdc13b897a33cc8312d1e2c3e52b0ac227a709eebec9e3a5f9bd2471b1`이다. 이전 `5280…` 기록은 경험치 단독 수정 시점이다.

VM이 꺼져 있어 이번에는 서버 내부 파일이나 게임 전투 화면을 직접 검증하지 않았다. 원격 아버지 PC 적용 또는 원격 저장소 공개는 수행하지 않았다. 기존 배포 manifest 버전은 유지했고 753개 source 해시를 확인했다.

검증: `final-application-validation.json`, `final-local-installation.json`, `../card-drops/current-card-drop-validation.json`, `../card-drops/current-card-drag-validation.json`, `../experience-growth/final-application-review.json`.
