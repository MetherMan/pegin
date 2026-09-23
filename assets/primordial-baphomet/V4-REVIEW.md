# 바포메트 질감·1.7배·칼날·경험치

피부를 무광의 거친 회흑색 질감으로 보정하고 뿔에 건조한 각질 결을 추가했다. 밝은 반사광은 게임 재질의 specular 효과가 아니라 이전 diffuse 그림에 포함돼 있던 표현이다. 외눈·감은 양쪽 눈과 기존 피부 UV 배치는 유지했다. 뿔이 재질 하단의 좁은 부분만 쓰던 V 범위도 넓혔다.

내장 ImageGen 결과 `body-matte-v4-imagegen.png`, `horn-material-matte-v4-imagegen.png`를 사용한다. 프롬프트 원문과 입력·출력은 `imagegen-v4-prompts.json`에 있다. native WTM과 미리보기 PNG 및 원본 이미지의 아틀라스 영역 픽셀 일치를 검증했다.

전체 크기는 일반 원본의 1.7배다. 고정된 1.4배 조형 기준에서 메시를 1.7/1.4로 한 번만 확대하며, 8개 ANI는 원본에서 직접 1.7배로 생성한다. 본 29개와 회전 값은 유지한다. 동일 빌드를 반복해도 크기가 누적되지 않는다.

검 끝은 기존에도 아래·앞을 향하고 있었으므로 길이 방향을 뒤집지 않았다. 실제 그립 중심을 지나는 장축으로 180도 돌려 칼날(edge)이 등(spine)보다 아래로 오게 했다. 대기·걷기·달리기에서 하향, 공격에서는 원래 손목 회전을 따르며 그립은 고정된다. 네 동작 223프레임에서 손잡이 궤적·검의 강체 변환을 검사했다. 새 대검은 열린 경계·비다양체 경계가 모두 0이다.

경험치는 서버 일반 바포메트 ID44의 14,328을 기준으로 28,656이다. ID80/280/380/480/580/680의 EXP 필드만 변경했다. 다른 스탯·일반 바포메트·드롭·카드·경험치 배수는 유지한다. 클라이언트 표의 일반 바포메트 값은 오래된 6,312여서 산정 기준으로 쓰지 않는다.

검증 기록: `v4-geometry-validation.json`, `v4-browser-validation.json`, `experience-validation.json`. 실제 적용 및 서버 확인은 `experience-installation.json`, `v4-final-installation.json`을 참고한다. 게임 Direct3D 화면에서 직접 사냥한 검증은 아니다.

재생성: `runtime/python/python.exe -B -X utf8 tools/build_primordial_baphomet.py --geometry-only`. 경험치 필드 전용 도구는 `tools/primordial_experience.py`이며, 이 요청의 6행 이외를 변경하지 않는다.
