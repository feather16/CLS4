# リリースを自動的に作成する

import subprocess
import platform
import os
import datetime
import pytz
import shutil

def get_time_format() -> str:
    now = datetime.datetime.now(pytz.timezone('Asia/Tokyo'))
    return now.strftime('%Y%m%d%H%M')

def get_git_commit_id() -> str:
    command = f"cd \"{os.path.dirname(__file__)}\" && git log -n 1 --pretty=format:\"%H\""
    commit_id = subprocess.run(
        command, 
        capture_output=True,
        shell=True,
    ).stdout.decode('utf-8')
    return commit_id

# 定数
OS_NAME = platform.system()
BASE_DIR_PATH = f'{os.path.dirname(__file__)}'
INTERPRETER_NAME = 'cls4' + ('.exe' if OS_NAME == 'Windows' else '')

# パラメータ
APP_NAME = 'CLS4'
VERSION = f'v{get_time_format()}-alpha'
APP_TITLE = f'/{APP_NAME}-{VERSION}-{OS_NAME}'
ZIP_DIR_PATH = f'{BASE_DIR_PATH}/{APP_TITLE}'
ZIP_FILE_PATH = f'{BASE_DIR_PATH}/{APP_TITLE}.zip'

BUILD = False

if BUILD:
    build_success = os.system('make clean && make -j') == 0
    if not build_success: exit(0)

github_url = f"https://github.com/feather16/CLS4/tree/{get_git_commit_id()}"

# Releaseディレクトリの作成
if not os.path.exists(ZIP_DIR_PATH):
    os.mkdir(ZIP_DIR_PATH)

# Releaseディレクトリへのファイルコピー
def copy_to_zip_dir(filename: str) -> None:
    src = f'{BASE_DIR_PATH}/{filename}'
    dst = f'{ZIP_DIR_PATH}/{filename}'
    if os.path.isdir(src):
        shutil.copytree(src, dst)
    else:
        shutil.copy(src, dst)
copy_to_zip_dir(INTERPRETER_NAME)
copy_to_zip_dir('unittest.py')
os.mkdir(f'{ZIP_DIR_PATH}/unittest')
copy_to_zip_dir('unittest/ans')
copy_to_zip_dir('unittest/src')
copy_to_zip_dir('unittest/stdin')
os.mkdir(f'{ZIP_DIR_PATH}/unittest/out')
os.mkdir(f'{ZIP_DIR_PATH}/unittest/tree')
copy_to_zip_dir('unittest/README.md')
copy_to_zip_dir('unittest/test_priority.txt')

# Releaseディレクトリ内にREADME.mdを作成
with open(f'{ZIP_DIR_PATH}/README.md', 'w', encoding='utf-8') as f:
    f.write(f'''\
GitHub:
<a>{github_url}</a>
    ''')

# zip圧縮
shutil.make_archive(os.path.splitext(ZIP_FILE_PATH)[0], 'zip', ZIP_DIR_PATH)
print(f'Release file \'{ZIP_FILE_PATH}\' created.')