# 로컬 클라이언트 종료 진단

수헤르 보스전 중 발생했던 종료는 아직 재현되지 않았고, 당시 오류 로그/덤프도 없습니다. 이 기능은 원인을 확정하기 위한 기록 장치이며 당시 문제의 수정 완료를 뜻하지 않습니다.

`LaqiaCrash.dll`은 게임의 CRT 초기화 후 WinMain 직전에 초기화됩니다. 게임 폴더의 `CrashDumps`에 오류 코드, 모듈/RVA, 레지스터, 메모리 접근 유형을 기록한 TXT와 미니덤프를 만듭니다. 외부 전송은 없습니다. 정상 종료나 프로그램에서 처리한 예외는 기록하지 않습니다. OS 강제 종료, 전원 종료, 처리된 오류 뒤의 정상 종료는 포착하지 못합니다.

미니덤프 작성은 별도 대기 스레드가 담당하며 예외 스레드는 최대 8초 기다립니다. 시스템 DbgHelp를 미리 로드하고 전체 메모리 덤프 옵션은 사용하지 않습니다. 스택에는 실행 중인 데이터 일부가 포함될 수 있으므로 덤프는 공개 저장소에 넣지 않습니다. 기본 경로는 로컬 설치 `runtime/client/GameClient/CrashDumps`, 아버지 설치 `work/laqia-runtime/client/GameClient/CrashDumps`입니다.

구현 기준: [MiniDumpWriteDump](https://learn.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump), [SetThreadStackGuarantee](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadstackguarantee).

빌드: `tools/build_crash_reporter.cmd` (Visual Studio 2022 Community x86 C++ 도구). 출력은 `.cache/crash-reporter`이며 자동 설치하지 않습니다. `tools/verify_crash_reporter.py`는 독립된 네이티브 x86 프로그램에서 정상 종료, 처리된 예외, 메인/별도 스레드의 실제 접근 오류, 실제 스택 초과를 검증하고 덤프의 예외·스레드·모듈 스트림을 읽습니다. 게임 자체에 고의 오류를 발생시키지 않습니다. PDB는 로컬 빌드 폴더에 유지합니다.

실행 파일 초기화 패치는 `tools/patch_stack_client.py`에 포함됩니다. 0x44fd74의 WinMain 호출을 감싸되 모든 레지스터/플래그와 원래 호출 인자를 보존합니다. 게임에 포함된 VC6 CRT의 기본 예외 경로(0x454287)는 UnhandledExceptionFilter를 호출하므로 등록한 필터로 이어집니다. 원인 조사 시 TXT의 모듈/RVA와 해당 배포 해시, DMP의 예외 스레드를 함께 확인해야 합니다.

조사 결과: 전투 패킷의 캐릭터/몬스터 조회는 null 검사를 하고, 몬스터 해제 시 공격자/대상 연결 정리가 있습니다. 이것만으로 생명주기 오류가 없다고 결론 내리지 않았습니다. 상점의 기존 오류 로그에 `%d`/`%s` 인자가 뒤바뀐 문제는 별도로 수정했지만 보스전 종료와의 연관 증거는 없습니다. Windows 오류 이벤트, 클라이언트 errlog, 서버 journal에서 보고된 사고를 특정할 기록은 확보하지 못했습니다.

추가 조사: Windows 이벤트 조회 기간을 30일로 넓혀 2026-09-14 21:49:54와 22:02:02(KST)의 게임 접근 오류 두 건을 찾았습니다. 모듈은 UInterface.dll, RVA는 각각 0x24b9d/0x24ba2입니다. 현재 DLL에서는 상점 목록 삽입 함수 `InsertItemInShop` 범위이며 두 주소 모두 명령어 중간에 위치합니다. 기존 GM 창고 패치가 적용된 현재 DLL로 당시 실행 상태를 재현할 수 없고, PE 타임스탬프만으로 수정된 DLL 버전을 구별할 수도 없습니다. 따라서 이를 수헤르 전투 종료 원인으로 단정하지 않습니다. 해당 WER 보고서 폴더 두 개는 Windows 접근 권한 때문에 읽지 못했으며 권한 설정을 변경하지 않았습니다. 원본 이벤트에서 필요한 오류 필드만 `tests/results/historical-crashes.json`에 기록했습니다.
