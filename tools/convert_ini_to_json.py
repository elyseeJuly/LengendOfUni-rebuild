import os
import configparser
import json
import codecs

INI_DIR = 'windows_mfc/LengendOfUni'
JSON_DIR = 'legend-of-uni-web/src/data'

def convert_ini_to_json(ini_filename, json_filename):
    ini_path = os.path.join(INI_DIR, ini_filename)
    json_path = os.path.join(JSON_DIR, json_filename)
    
    if not os.path.exists(ini_path):
        print(f"Skipping {ini_filename}, file not found.")
        return
    
    # Read with GBK encoding
    config = configparser.ConfigParser()
    try:
        with codecs.open(ini_path, 'r', encoding='gbk') as f:
            config.read_file(f)
    except Exception as e:
        print(f"Error reading {ini_filename}: {e}")
        return

    data = []
    
    if 'count' in config and 'count' in config['count']:
        count = int(config['count']['count'])
        for i in range(count):
            section = str(i)
            if section in config:
                item = dict(config[section])
                # Try to convert numeric values to int
                for k, v in item.items():
                    if v.isdigit():
                        item[k] = int(v)
                    else:
                        try:
                            # Try to float if possible, but actually we mostly have ints
                            pass
                        except ValueError:
                            pass
                data.append(item)
    
    with codecs.open(json_path, 'w', encoding='utf-8') as f:
        json.dump(data, f, ensure_ascii=False, indent=2)
    print(f"Converted {ini_filename} -> {json_filename}")

if __name__ == '__main__':
    convert_ini_to_json('person.ini', 'persons.json')
    convert_ini_to_json('alien.ini', 'aliens.json')
    convert_ini_to_json('star.ini', 'stars.json')
    convert_ini_to_json('weapon.ini', 'weapons.json')
    convert_ini_to_json('gameevent.ini', 'events.json')
    convert_ini_to_json('randomevent.ini', 'randomevents.json')
