# LAQIA THE LEGEND

Windows 개발용 작업 저장소입니다. 현재 서버 패치, 클라이언트 적용 파일, 블러드나이트 모델·텍스처와 재생성 도구를 포함합니다.

## 다른 PC에서 시작

Windows 10/11 64비트, Git for Windows, 이 Private 저장소에 접근 가능한 GitHub 로그인이 필요합니다. 여유 공간은 12GB 이상을 권장합니다.

```powershell
git clone https://github.com/MetherMan/LAQIA-THE-LEGEND.git
cd LAQIA-THE-LEGEND
.\Initialize.cmd
```

처음 한 번 `Initialize.cmd`를 실행합니다. GitHub Releases에서 기본 게임 리소스·Python·QEMU·Node를 내려받아 SHA-256을 검증합니다. 새 서버 VM을 만들고 Linux 개발 의존성을 설치하므로 인터넷 연결이 필요하며 수 분 이상 걸릴 수 있습니다. 서버 VM과 실제 플레이 데이터는 `runtime/`에만 저장됩니다.

설정 중 **새 로컬 게임 비밀번호**를 입력합니다. 계정 ID는 `developer`, 테스트 캐릭터는 `Developer`입니다. 이 계정에서 `/재뽕` 명령을 사용할 수 있습니다. 기존 PC의 아버지/사용자 계정이나 캐릭터를 복사하지 않습니다.

설정이 끝나면 **`Play.cmd`**로 게임을 실행합니다. 다른 게임 폴더의 서버가 켜져 있다면 먼저 해당 게임을 정상 종료해야 합니다.

## 다른 PC에서 변경을 받기

게임을 종료한 뒤 **`Update-Workspace.cmd`**를 실행합니다. `git pull --ff-only` 후 클라이언트 적용 파일과 서버 프로그램·아이템 데이터를 갱신합니다. 캐릭터 DB와 VM 디스크는 교체하지 않습니다. Git 충돌이나 작업 폴더의 리소스 수정이 발견되면 덮어쓰지 않고 중단합니다.

`git pull`만 하면 소스가 갱신되고, 게임용 복사본에는 다음 `Play.cmd` 또는 `Update-Workspace.cmd` 실행 때 반영됩니다. 변경한 파일을 다른 PC에도 보내려면 해당 변경을 commit/push해야 합니다.

## 작업 위치

| 경로 | 용도 |
|---|---|
| `src/server/utf8/` | 수정한 서버 코드의 UTF-8 편집본. 빌드 시 CP949로 변환 |
| `src/server/LAQIA_GameServer/` | 원본·현재 서버 C++ 코드와 헤더 |
| `src/client/` | 보관된 구형 클라이언트 C++ 소스 |
| `game-data/DATA/` | 서버 아이템·스킬·몬스터 등 정적 데이터 |
| `client-overlay/` | Git으로 관리하는 클라이언트 실행 파일·UI·변경 리소스 |
| `assets/blood-knight/` | 원본 모델, 게임용 모델, 텍스처, 생성 스크립트 |
| `server-bin/` | 검증된 서버 실행 파일 |
| `runtime/` | 다운로드된 실행 환경·캐릭터 DB·접속 키. Git 제외 |

기본 클라이언트의 나머지 모델·텍스처도 초기 설정 후 `runtime/client/GameClient/`에서 열어볼 수 있습니다. 기존 리소스를 변경하여 Git으로 관리하려면 같은 상대 경로로 `client-overlay/`에 복사해서 수정합니다.

**`Build-Model.cmd`**: 블러드나이트 모델을 재생성하고 `client-overlay/`에 넣습니다. Node·NumPy·Pillow는 초기 설정에 포함됩니다. ImageGen 재생성에는 별도의 이미지 생성 도구가 필요하며, API 키는 저장소에 포함하지 않습니다.

**`Build-Server.cmd`**: VM 안에서 현재 서버 수정 모듈을 빌드하고 로컬 개발 서버에 적용합니다. 빌드 결과는 `runtime/build/LAQIA_GameServer`입니다. 배포할 변경을 검증한 뒤 `server-bin/LAQIA_GameServer`로 복사하고 commit합니다. 기존 컴파일 객체를 사용하는 레거시 빌드이며, 모든 서버 원본을 처음부터 다시 빌드하는 도구는 아닙니다.

클라이언트 C++ 소스는 참고·수정용으로 보존했습니다. 현재 DLL 전체를 다시 빌드하려면 구형 Visual C++/DirectX 환경을 별도로 맞춰야 합니다. 이 저장소의 초기 설정은 해당 클라이언트 컴파일 환경까지 설치하지 않습니다.

## 블러드나이트스피어

- `/재뽕 테스트 1`: 0강 한 개 지급
- 양손검 분류 / Rank 15 / 착용 제한 없음 / 무게 0
- 0강 `1840~2110`, 20강 `9160~11220`
- 장착 후 `/재뽕 무기 20` 또는 `/재뽕 무기 0`
- 짙은 핏빛·밝은 칼날 색상 수정 및 금색 테두리 인벤토리 아이콘 포함

## 데이터와 업데이트 범위

개인 캐릭터 DB, 기존 서버 디스크, SSH 개인 키, 실제 계정 설정, 크래시 덤프와 백업은 Git과 런타임 다운로드 파일에 포함하지 않습니다. 새 PC는 빈 DB에서 시작합니다. 기존 캐릭터를 옮기려면 기존 게임의 캐릭터 추출 기능으로 별도 백업·이전해야 합니다.

아버지용 ZIP 설치본을 자동 업데이트하는 별도 배포 기능은 이번 개발용 저장소 구성과 구분됩니다. 현재 제공되는 버튼은 Git 작업 폴더를 사용하는 개발자를 위한 것입니다.
