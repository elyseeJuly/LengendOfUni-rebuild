import os

person_ini_path = 'LengendOfUni/person.ini'
alien_ini_path = 'LengendOfUni/alien.ini'

original_names = ["丁仪", "罗辑", "林云", "叶文洁", "大史", "伊依", "沈渊",
                  "霍金", "常伟思", "庄颜", "章北海", "水娃", "华华", "希恩斯", "雷迪亚兹",
                  "泰勒", "东方延绪", "雷志成", "杨卫宁", "严井", "白冰", "苗福全",
                  "滑膛", "朱汉扬", "刘慈欣"]

# Added characters with specific stats (Name: [Army, Economy, Leadership, Art, Science, Treachery, Social])
new_chars = {
    "程心": [15, 50, 40, 60, 80, 5, 85],
    "维德": [90, 60, 95, 10, 70, 80, 30],
    "云天明": [20, 30, 30, 95, 75, 10, 40],
    "艾AA": [30, 80, 35, 70, 65, 15, 90],
    "智子": [85, 40, 70, 50, 99, 90, 60],
    "汪淼": [10, 30, 25, 40, 90, 5, 50],
    "杨冬": [5, 10, 10, 30, 95, 0, 20],
    "伊文斯": [40, 95, 80, 70, 50, 95, 65],
    "关一帆": [30, 30, 40, 40, 85, 5, 55],
    "山杉惠子": [25, 40, 50, 45, 70, 70, 60]
}

all_chars = []
for name in original_names:
    all_chars.append({
        "Name": name,
        "Army": 50, "Economy": 50, "Leadership": 50, "Art": 50, "Science": 50, "Treachery": 10, "Social": 50
    })

for name, stats in new_chars.items():
    all_chars.append({
        "Name": name,
        "Army": stats[0], "Economy": stats[1], "Leadership": stats[2], "Art": stats[3],
        "Science": stats[4], "Treachery": stats[5], "Social": stats[6]
    })

with open(person_ini_path, 'w', encoding='gbk') as f:
    f.write(f"[count]\ncount={len(all_chars)}\n\n")
    for i, c in enumerate(all_chars):
        f.write(f"[{i}]\n")
        for k, v in c.items():
            f.write(f"{k}={v}\n")
        f.write("\n")


# Alien civilizations (Name, Pic, StartStars, AI_Personality)
# Personality: 0-Hunter, 1-Cleaner, 2-Defensive, 3-Expansionist, 4-Opportunist
aliens = [
    {"Name": "三体", "IsPlanet": 1, "Res": 1000, "PopLimit": 200, "StarSys": 1, "Personality": 0},
    {"Name": "歌者", "IsPlanet": 1, "Res": 2000, "PopLimit": 500, "StarSys": 3, "Personality": 1},
    {"Name": "边缘世界", "IsPlanet": 1, "Res": 1500, "PopLimit": 300, "StarSys": 2, "Personality": 3},
    {"Name": "魔戒", "IsPlanet": 1, "Res": 1200, "PopLimit": 100, "StarSys": 3, "Personality": 2},
    {"Name": "归零者", "IsPlanet": 1, "Res": 3000, "PopLimit": 1000, "StarSys": 3, "Personality": 2}
]

with open(alien_ini_path, 'w', encoding='gbk') as f:
    f.write(f"[count]\ncount={len(aliens)}\n\n")
    for i, a in enumerate(aliens):
        f.write(f"[{i}]\n")
        for k, v in a.items():
            f.write(f"{k}={v}\n")
        f.write("\n")

print("Generated person.ini and alien.ini in GBK encoding.")
