# LAQIA THE LEGEND 작업 저장소

## 실제 플레이와 기존 계정

이 저장소의 `Play.cmd`는 **개발용 `runtime` 서버와 DB**를 실행합니다. 기존 배포본의 계정·캐릭터와 자동으로 공유되지 않습니다. 기존 계정으로 플레이할 사용자에게 이 실행 파일을 안내하지 마세요.

회사 PC에서 확인한 실행 경로는 상위 `라버 2/게임 시작.cmd` → 기존 설치본 `https-laqia-co-kr-https-laqia/outputs/Start-Local-Game.cmd`입니다. 2026-09-22 회사 PC에서는 최신 무기 파일과 서버를 기존 설치본에 적용하고 원래 계정의 로그인과 캐릭터 선택을 검증했습니다. 이 경로가 다른 PC에도 있다고 가정하지 마세요. 집 PC에서는 실제 outputs 설치본을 확인하지 못했으며, 사용자가 아버지 PC의 실행본을 쓰는 것으로 정정했습니다. 확인되지 않은 경로로 새 게임 실행기를 만들지 않습니다.

## 아버지의 기존 설치본 업데이트

아버지는 기존 설치 폴더에서 `자동업데이트.cmd`로 업데이트한 다음, 원래 `outputs`의 게임 시작 버튼을 계속 사용합니다. 저장소 전체를 새로 내려받아 다른 `Play.cmd`를 실행하거나 개발용 VM·DB를 복사하는 방식이 아닙니다.

`distribution/father_update.py`는 공개 GitHub의 manifest에 있는 게임 파일과 정적 서버 데이터만 교체합니다. 계정, 비밀번호, 캐릭터 DB, VM 디스크, 개인 설정은 교체하지 않습니다. 커밋·푸시하지 않은 로컬 변경은 아버지 PC로 배포되지 않습니다.

업데이트는 새 게임서버가 VM 안에서 실제로 2560 포트를 열 때까지 확인하고, 실패하면 이전 서버 파일로 되돌립니다. 내려받은 임시 사본은 매번 지우고 백업은 최근 3개만 남깁니다. 적용한 목록은 `work/laqia-runtime/installed-manifest.json`에 남으며, 게임을 시작할 때 빠졌거나 크기가 달라진 게임 파일(예: 백신이 격리한 DLL)을 같은 버전에서 해시 검증 후 다시 받습니다.

기존 설치본의 업데이트 프로그램이 새 manifest를 먼저 검사하므로 manifest의 파일 종류나 `runtime_files` 이름을 늘리면 안 됩니다. `tests/update_maintenance_test.py`가 이전 두 버전으로 이를 확인합니다.

## VM 시작 오류 복구

`WHPX: No accelerator found` / `failed to initialize whpx` 뒤에 `VM did not become ready`가 나타났다면, [복구 업데이트 ZIP](distribution/father-updater-repair.zip)을 받아 압축을 푼다. 안의 `라키아` 폴더에서 `자동업데이트.cmd`와 `update` 폴더를 기존 설치본의 `outputs`·`work`가 보이는 위치에 한 번 덮어쓰고 실행한다. 기존 업데이트는 VM이 켜진 뒤에야 자기 코드를 교체하므로, 이미 이 오류가 난 PC는 단순 재실행만으로 복구 코드를 받을 수 없다.

수정본은 해시 검증된 업데이트 프로그램과 시작 도구 3개를 VM 시작 전에 적용한다. WHPX 실패 기록이 있으면 TCG로 시작하며, 기본 실행도 QEMU 자체의 WHPX→TCG 전환을 사용한다([공식 QEMU 문서](https://www.qemu.org/docs/master/system/invocation.html)). 이미 부팅 중인 VM은 중복 실행하지 않고 최대 6분 기다린다. Windows 기능·VM 디스크·키·계정 DB는 바꾸지 않는다. 계속 실패하면 `work/laqia-runtime/vm-start-diagnostic.txt`에 마지막 연결 오류와 부팅 로그가 남는다. TCG는 하드웨어 가속보다 느릴 수 있다.

## 개발 변경을 실제 플레이 설치본에 적용

게임 종료 후 manifest를 재생성하고, 저장소 루트에서 다음 명령을 실행합니다. `<기존 설치 폴더>`는 `outputs`와 `work`가 함께 있는 폴더입니다.

```powershell
runtime/python/python.exe -X utf8 tools/package_father_update.py --manifest-only --version <배포버전>
runtime/python/python.exe -X utf8 distribution/father_update.py --root '<기존 설치 폴더>' --source .
```

계정 DB를 가져오거나 새 계정을 만드는 대신 기존 설치본을 업데이트합니다. 같은 PC의 두 설치본을 동시에 실행하면 로컬 서버 포트가 충돌하므로 기존 게임을 먼저 종료합니다.

모델·데미지·스킬 조사 기록은 `assets/twilight-set/README.md`, 작업과 검증 내역은 `WORKLOG.md`를 참고하세요.

## 140 스킬과 색상 설정

카드 드래그 병합, 트와일라잇 아이콘 공통 금색 테두리, 밝은 청색 화살과 신규 원거리·흑마법 140 스킬 4종은 `assets/skills140/README.md`에 정리했습니다. 회사 PC에서 연결한 상위 `스킬 색상 설정.cmd`는 메테오·프로스트 색상을 실제 outputs 설치본과 저장소 배포 파일에 함께 저장합니다. 다른 PC에서는 연결 경로를 먼저 확인해야 합니다.

색상 창은 게임의 원본 모델·텍스처·애니메이션을 3D로 재생합니다. 드래그 회전, 휠 확대, 일시정지, 재생 위치·속도 조절, 시전 준비/공격 전환으로 확인한 뒤 적용할 수 있습니다.

아래쪽 아처 미리보기에서 헤븐 페너트레이션 3연사와 식스폴드 샷 6연사를 확인할 수 있습니다. 식스폴드는 기존 활의 흰색 효과와 트와일라잇 활의 파란 효과를 선택해 비교합니다.

최상위 스킬 습득 요구치는 근거리 90 / 원거리 140 / 흑마법 140 / 백마법 115입니다. 스킬 자체의 강화 한도나 캐릭터 총레벨 제한을 뜻하지 않습니다.

## 이동·회전 렉 수정

`client-overlay/Render.dll`은 엔진이 원래 설계한 MIXED 정점 처리로 되돌렸습니다. 뼈대 애니메이션과 지형 블록은 지금처럼 CPU, 나머지는 GPU가 처리합니다. 정점 셰이더 1.0 미만 GPU나 생성 실패 시 기존 방식으로 자동 전환합니다. `client-overlay/UInterface.dll`은 채팅 중이 아닐 때 한글 조합 메시지를 Windows 기본 처리로 넘기지 않습니다. 원본 해시를 확인하는 `tools/patch_render_mixed_vp.py`와 `tools/patch_uinterface_ime_idle.py`로 재생성하고, `tools/verify_render_mixed_vp.py`로 실제 게임 모델의 원본/패치 화면과 프레임 시간을 비교합니다. 근거는 `assets/performance/`와 `WORKLOG.md`에 있습니다.

## 태초의 바포메트 뿔 질감

`runtime/python/python.exe -B -X utf8 tools/horn_preview_server.py` 실행 후 `http://127.0.0.1:8879/assets/primordial-baphomet/`를 엽니다. 뿔 질감 강도 0~8배는 실시간 표시되며 **이 강도로 저장·게임 적용**으로 확정합니다. 현재 모델은 원본 1.7배, 가운데 외눈, 어두운 비대칭 뿔과 아래로 향한 칼날을 사용합니다.

저장하면 배포 텍스처·설정·manifest 해시와 기존 두 로컬 실행본이 함께 갱신됩니다. 커밋·푸시 후 아버지는 기존 `자동업데이트.cmd`로 받고, 평소 outputs의 **혼자 게임 시작**을 사용합니다. 이전 BaphometColors 피부색 편집기는 이전 조형용 도구이며, 현재 텍스처를 덮어쓰지 않도록 저장을 거부합니다.

현재 선택값과 경험치 비교, 재생성 및 검증 기록은 `assets/primordial-baphomet/README.md`를 참고하세요.
