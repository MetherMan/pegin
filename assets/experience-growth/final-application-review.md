# 최종 적용 전 레벨업 누적 수정 독립 점검

2026-09-23. 서버 시작·종료, 설치, 소스, 사용자 DB·계정 변경 없이 읽기와 오프라인 에뮬레이션만 수행했다.

## 결과

총레벨 576 솔로 처치 경험치 정지 수정은 현재 카드 수정 누적 빌드에도 남아 있다. 가족 로컬 `server-local/LAQIA_GameServer/LAQIA_GameServer` 파일을 직접 읽어 실제 x86 `CheckDeadMob` 경로의 기존 51개 회귀 검사를 재실행했고 모두 통과했다.

- 시간 단계 7개 × 계열 4개: 30배 경험치 429,840 정상 지급. 수정 전에는 시간 단계 1~6에서 0을 재현했다.
- 총레벨 경계 15개: 기존 0~400 표 동작을 보존하고 401 이상은 표 밖 값을 읽지 않는다.
- 네 계열 레벨업과 초과 경험치 123 이월, 네 계열 200 상한 유지.
- 현재 소스의 네 지급 분기 모두 실제 경험치 수령자 `pExpPlayer`의 레벨로 401바이트 표 범위를 검사한다. 빌드 시 이를 덮어쓰는 UTF-8 fight 사본은 없다.
- DataServer 디렉터리의 과거 fight.cpp는 해당 Makefile의 빌드 대상이 아니므로 현재 게임 경험치 실행 경로와 구분했다.

## 현재 바이너리

아래 세 파일 모두 1,812,236바이트이며 SHA-256은 `ed4a6efdc13b897a33cc8312d1e2c3e52b0ac227a709eebec9e3a5f9bd2471b1`로 동일하다.

- server_bin: `C:\Users\dayba\Downloads\라버 2\LAQIA-THE-LEGEND\server-bin\LAQIA_GameServer`
- workspace_build: `C:\Users\dayba\Downloads\라버 2\LAQIA-THE-LEGEND\runtime\build\LAQIA_GameServer`
- family_server_local: `C:\Users\dayba\Downloads\라버 2\https-laqia-co-kr-https-laqia\work\laqia-runtime\server-local\LAQIA_GameServer\LAQIA_GameServer`

기존 경험치 전용 수정본 `5280d2…`에서 카드 수정이 추가되며 현재 `ed4a6e…`로 바뀐 것이다. 해시 차이는 경험치 수정이 빠졌다는 뜻이 아니며, 현재 파일을 실제 실행한 51개 검사가 이를 확인한다.

## VM 확인 범위

현재 22222·44444·8030·2560 포트는 모두 닫혀 있었다. VM을 시작하지 않았으므로 이번 점검에서 VM 내부 `/opt/laqia/server/LAQIA_GameServer/LAQIA_GameServer`의 최신 바이트나 실행 중 서비스 상태를 직접 확인하지는 못했다. 이전 `assets/card-drops/local-installation-validation.json`에는 같은 `ed4a6e…` 파일을 VM에 적용한 뒤 active 상태와 저장 테이블·배수 보존을 확인한 기록이 있다.

검사는 UI·네트워크·드롭·저장 알림을 대체하는 오프라인 실행이다. 실게임 창에서 직접 사냥하거나 아버지의 별도 PC에 접속한 검사는 아니다.

[51개 실제 실행 결과](final-application-regression.json) · [소스·바이너리·VM 상태 증거](final-application-review.json)
