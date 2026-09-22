# 지옥마 모델 시안

현재 원본 갈색말(적갈색 외형)인 `Vehicle/horse_1.mod`와 `Texture/Vehicle/horse21.wtm`을 바탕으로 만든 별도 게임용 모델·텍스처다. 기존 32개 본, 171프레임 애니메이션, 393정점/718삼각형과 UV를 유지했다. 네 발굽의 48정점만 폭 1.20배·앞뒤 길이 1.13배로 넓혀 화산암 느낌의 각진 실루엣을 보강했다.

- 몸체: 진한 적색 코트와 원본의 근육 명암.
- 발: 검은 화산암 표면과 노랑·주황 마그마 균열.
- 갈기·꼬리: 밝은 노란색.
- 눈: 날카로운 단색 노랑. 흰자·동공 구분과 발광 재질 없음.
- 안장·고삐: 원본 배치와 구조 유지.

`index.html`의 원본/지옥마 비교, 대기/달리기, 회전·확대, 눈/발 확대를 브라우저에서 검수했다. JavaScript 오류 0개. 네이티브 모델을 다시 파싱해 본 인덱스·UV·삼각형 구조 보존과 WTM 픽셀 왕복 일치를 확인했다. 브라우저는 원본 골격 행렬과 애니메이션을 재생한다.

2026-09-22: 소유증서 **19130**, 탑승 종류 **4**, 이동속도 **95**로 기존 outputs 설치본에 적용했다. `/재뽕 말 지옥마` 또는 `/재뽕 말 4`로 지급할 수 있고 기존 재뽕 창고의 말 분류에도 등록되어 있다. 실제 아이템 사용 패킷으로 탑승·하차, 재접속, 포탈 이동 후 탑승 유지와 속도 95를 검증했다. 기존 세 말의 모델과 속도는 유지한다. 클라이언트의 새 모델 선택, 범위 검사, 말/기수 모션 재사용은 `client-hooks.json`의 에뮬레이터 검사 결과를 참고한다.

파일:

- `payload/Vehicle/mt_hellhorse.mod`: 새 이름의 Wind3D 모델.
- `payload/Texture/Vehicle/mt_hellhorse.wtm`: 1024×1024 네이티브 압축 BMP 텍스처.
- `hellhorse-atlas-imagegen.png`: 내장 ImageGen으로 원본 UV 아틀라스를 편집한 원본 출력.
- `imagegen-prompt.txt`: 정확한 생성 프롬프트.
- `validation.json`: 구조·변환 검증 결과.

재생성: `runtime/python/python.exe -B -X utf8 tools/build_hell_horse.py`.
미리보기: 저장소에서 로컬 HTTP 서버를 실행한 뒤 `/assets/hell-horse/`.

## 현재 말의 이동속도

실제 실행 서버의 `GetRideSpeed` 기계어와 소스 값이 일치한다. 클라이언트 `Creature.h::ChangeSpeed`는 이 값을 10으로 나눠 사용한다. 버프 없는 기본값 비교다.

| 종류 | 서버 속도값 | 클라이언트 이동값 | 맨몸 대비 |
|---|---:|---:|---:|
| 맨몸 | 40 | 4.0 | 1.00배 |
| 갈색말 (적갈색 외형) | 70 | 7.0 | 1.75배 |
| 흑마 | 75 | 7.5 | 1.875배 |
| 백마 | 80 | 8.0 | 2.00배 |
| 지옥마 | 95 | 9.5 | 2.375배 |

소유증서 아이콘은 원본 갈색마 소유증서 하나만 편집 대상으로 내장 ImageGen에서 색상을 변경했다. 아래로 숙인 말머리 방향·갈기 배치·기존 픽셀 그림체를 유지하며 붉은 몸·노란 갈기·노란 눈으로 바꿨다. 다른 구도로 생성됐던 첫 시안은 사용하지 않는다. `certificate-comparison.png`에서 원본(왼쪽)과 수정본(오른쪽)을 같은 배율로 비교할 수 있다. 원본을 최근접 보간으로 20배 확대해 편집한 뒤 같은 방식으로 28×28에 맞춰 픽셀 윤곽을 유지한다. `certificate-imagegen.png`가 생성 원본, `certificate-icon.png`가 실제 28×28 게임용 이미지다. `details-imagegen-prompts.json`에 정확한 아이콘·타로스 로딩/지역명 프롬프트를 보관한다. 원래 갈색말 아이콘은 교체하지 않고 `mt_hellhorse_icon.wtm`을 별도로 등록했다. 일반/압축 클라이언트 카탈로그와 서버 아이템 정의의 양쪽 아이콘 참조를 19130에만 적용했다. 이전 압축 UI 카탈로그에 없던 19130 항목도 추가했다.

## 말 소리 연결 수정

원본 `Vehicle/vehicle.wad`에는 남녀 모두 활을 든 말의 달리기·정지 소리와 모든 걷기 소리가 비어 있었다. 기존 비활 달리기의 `HorseGallop.wav`와 감속의 `HorseBlow.wav` 연결을 활 모션에도 적용하고, 걷기는 기존 `HorseWalk_A01.wav`를 연결했다. 걷기 모션에서도 주기에 맞춰 소리가 반복되도록 네이티브 실행 경로를 수정했다. 애니메이션·탑승 속도·모델은 유지했다.

지옥마 소유증서 19130이 GUI의 말 아이템 소리 분기에서 빠져 있어 사용 시 울음소리도 재생되지 않았다. 기존 말과 동일한 `HorseWhinney.wav` 경로에 추가했다. 네 가지 WAV 원본을 배포에 포함했다. 기존 게임의 효과음 켜기/음량 설정은 그대로 따른다.

리소스 재생성: `runtime/python/python.exe -B -X utf8 tools/build_hell_horse_taros_details.py`.
클라이언트 패치: `runtime/python/python.exe -B -X utf8 tools/patch_mount_taros_client.py`. 현재 파일에서 바이트를 검사해 기존 패치 위에 추가하며, 최초 지옥마 등록용 빌더로 기존 바이너리를 되돌리지 않는다.

검증: `runtime/python/python.exe -B -X utf8 tests/mount_taros_details_test.py`. 남녀/활·비활의 실제 기계어 Update에서 사운드 파일명과 Play3D 호출·주기 전후 동작 16건, 보행 반복 조건 150건, GUI 아이템 소리 분기와 DLL 재배치 16건 통과. 원본 WAV는 첫 0.5초에도 유효한 PCM 소리가 있음을 확인했다. 세부 기록은 `details-validation.json`, `details-native-validation.json`, `sound-weather-exe-hooks.json`, `sound-ui-hooks.json`에 있다. 실제 outputs 게임에서의 청음과 탑승 화면 검수는 아직 하지 않았다.
