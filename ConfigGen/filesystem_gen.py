import os
import sys
import json

def main():
    f = open('ConfigGen/SampleFilesystem.json')
    base_json = json.load(f)
    f.close()
    data_filesys = {}
    data_filesys['partition'] = 'DATA'
    data_filesys['directories'] = ['/Applications']
    data_filesys['files'] = [
        {
        'source': 'config/HAL9000.ini',
        'destination': '/HAL9000.ini'
        }
    ]
    app_dir = 'artifacts/apps'
    apps = []
    for dirname in os.listdir(app_dir):
        curr_appdir = os.path.join(app_dir, dirname)
        if os.path.isdir(curr_appdir):
            apps.append(dirname)
    apps.sort() 
    for app in apps:
        curr_appdir = os.path.join(app_dir, app)
        curr_appexe = f'{app}.exe'
        curr_appexe_path = os.path.join(curr_appdir, curr_appexe)
        data_filesys['files'].append({
            'source': curr_appexe_path,
            'destination': f'/Applications/{curr_appexe}'
        })
    base_json['filesystems'].append(data_filesys)
    f = open('config/HAL9000.json', 'w')
    f.truncate()
    f.write(json.dumps(base_json, indent=4))
    f.close()

if __name__ == '__main__':
    main()
