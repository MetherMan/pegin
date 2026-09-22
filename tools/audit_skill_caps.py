"""Report highest learn requirements from the shipped skill table, not XP caps."""
from pathlib import Path
import json
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'assets/twilight-set'
def rows(p):return [r for l in p.read_text(encoding='cp949').splitlines() if (r:=l.split('\t'))[0].isdigit()]
skills=rows(ROOT/'game-data/DATA/skill_data.txt');classes=rows(ROOT/'game-data/DATA/spe_skill_class.txt')
byitem={int(r[3]):r for r in skills};branches=[]
for index,name in enumerate(['근거리','원거리','흑마법','백마법']):
    eligible=[r for r in skills if len(r)>=30 and int(r[9+index])>0]
    maximum=max(int(r[9+index]) for r in eligible)
    top=[dict(id=int(r[0]),item=int(r[3]),name=r[1],requirements=list(map(int,r[9:13])),class_id=int(r[13]),previous_item=int(r[28])) for r in eligible if int(r[9+index])==maximum]
    branches.append(dict(branch=name,max_learn_level=maximum,skills=top))
special=[]
for c in classes:
    r=byitem.get(int(c[0]));special.append(dict(class_id=int(c[6]),class_name=c[9],skill=c[1],skill_item=int(c[0]),requirements=list(map(int,c[2:6])),skill_table_requirements=list(map(int,r[9:13])) if r else None))
result=dict(source='game-data/DATA/skill_data.txt',interpretation='Highest learnable skill requirement per proficiency, not character XP cap or an individual skill upgrade cap',branches=branches,specializations=special,skill_count=len(skills))
(OUT/'skill-caps.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
lines=['# 클래스별 최상위 스킬 요구 레벨','', '서버가 실제 읽는 `game-data/DATA/skill_data.txt` 기준. 이 게임은 근거리·원거리·흑마법·백마법 숙련도를 각각 확인해 스킬을 배웁니다. 아래는 최고 습득 요구 레벨이며, 캐릭터 최대 레벨이나 스킬 자체의 강화 횟수가 아닙니다.','', '| 계열 | 최고 요구 레벨 | 해당 스킬 |','|---|---:|---|']
for b in branches:lines.append(f"| {b['branch']} | {b['max_learn_level']} | {', '.join(s['name'] for s in b['skills'])} |")
lines+=['','## 직업별 특화 스킬','', '직업을 결정하는 특화 스킬은 별도 목록입니다. 숫자는 근거리 / 원거리 / 흑마법 / 백마법 순서입니다.','', '| 직업 | 특화 스킬 | 요구 숙련도 |','|---|---|---|']
for c in special:lines.append(f"| {c['class_name']} | {c['skill']} | {' / '.join(map(str,c['requirements']))} |")
lines+=['','기준 코드: `src/server/LAQIA_GameServer/skill.cpp`의 `AddPlayerSkill`은 네 요구 숙련도와 선행 스킬을 확인합니다. 특화 스킬을 습득하면 `spe_skill_class.txt`에 지정된 직업으로 설정합니다.']
(OUT/'skill-caps.md').write_text('\n'.join(lines)+'\n',encoding='utf-8');print(json.dumps(branches,ensure_ascii=False))
