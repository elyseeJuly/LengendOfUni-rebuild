import os

replacements = {
    "TT_ASTROSOCIOLOGY": "TT_PHYSICS",
    "TT_NUCLEAR": "TT_AEROSPACE",
    "TT_SPACEFIGHT": "TT_MILITARY",
    "TT_PROTON": "TT_INFORMATION",
    "TT_ASTROPHYSICS": "TT_INTERSTELLAR",
    "TT_CULTURETEC": "TT_INTERSTELLAR",
    "TT_ECONOMYTEC": "TT_INTERSTELLAR",
}

def process_file(filepath):
    try:
        with open(filepath, 'r', encoding='gbk') as f:
            content = f.read()
    except UnicodeDecodeError:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
            
    modified = False
    for old, new in replacements.items():
        if old in content:
            content = content.replace(old, new)
            modified = True
            
    if modified:
        print(f"Modified: {filepath}")
        try:
            with open(filepath, 'w', encoding='gbk') as f:
                f.write(content)
        except UnicodeEncodeError:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)

for root, dirs, files in os.walk('.'):
    for file in files:
        if file.endswith('.cpp') or file.endswith('.h'):
            # skip define.h since we already changed the enum definition
            if file == 'define.h':
                continue
            # skip Department.cpp since we already changed the logic manually and don't want to revert
            if file == 'Department.cpp':
                continue
            # skip TecTreeManager.cpp since it's already updated
            if file == 'TecTreeManager.cpp':
                continue
            
            process_file(os.path.join(root, file))

print("Done.")
