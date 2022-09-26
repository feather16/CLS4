# ユニットテストを行う

import os
from re import sub
import subprocess

def file_equal(path1: str, path2: str) -> bool:
    '''
    ファイルの内容が同じか(末尾の改行は無視)
    '''
    with open(path1) as file1:
        content1 = file1.read().rstrip('\n')
    with open(path2) as file2:
        content2 = file2.read().rstrip('\n')
    return content1 == content2

CURRENT_DIR = os.path.dirname(__file__)
SRC_DIR = f'{CURRENT_DIR}/unittest/src'
STDIN_DIR = f'{CURRENT_DIR}/unittest/stdin'
OUT_DIR = f'{CURRENT_DIR}/unittest/out'
ANS_DIR = f'{CURRENT_DIR}/unittest/ans'
TREE_DIR = f'{CURRENT_DIR}/unittest/tree'

INTERPRETER = 'cls4'

test_cases = [os.path.splitext(file)[0] for file in os.listdir(SRC_DIR)]
test_cases.sort(
    key=lambda s: (not s.startswith('Normal'), not s.startswith('Test'), s)
)
test_case_name_max_length = max(len(test_case) for test_case in test_cases)
extension_set = {os.path.splitext(file)[1] for file in os.listdir(SRC_DIR)}
assert len(extension_set) == 1, f'Error: multiple extensions {extension_set}'
extension = extension_set.pop()

diffs: list[tuple[str, str]] = []

for test_case in test_cases:
    src_file_path = f'{SRC_DIR}/{test_case}{extension}'
    out_file_path = f'{OUT_DIR}/{test_case}.txt'
    stdin_file_path = f'{STDIN_DIR}/{test_case}.txt'
    ans_file_path = f'{ANS_DIR}/{test_case}.txt'
    tree_file_path = f'{TREE_DIR}/{test_case}.txt'

    # 実行
    if os.path.exists(stdin_file_path): # 標準入力あり
        command = f'{INTERPRETER} {src_file_path} {tree_file_path} < {stdin_file_path} > {out_file_path} 2>&1'
    else: # 標準入力なし
        command = f'{INTERPRETER} {src_file_path} {tree_file_path} > {out_file_path} 2>&1'
    os.system(command)
    
    # 比較
    print(f'{test_case: <{test_case_name_max_length}} ', end='')
    if file_equal(out_file_path, ans_file_path):
        print('\033[32mo\033[0m', end='') # 緑
    else:
        print('\033[31mx\033[0m', end='') # 赤
        diff = subprocess.run(f'diff {out_file_path} {ans_file_path}', capture_output=True).stdout.decode('utf-8')
        diffs.append((test_case, diff))
    description = ''
    try:
        description = open(src_file_path, encoding='utf-8').readlines()[0].split('//')[1].strip()
    except:
        pass
    print(f' : {description}')

# 結果表示
print('')
if len(test_cases) - len(diffs) > 0:
    case = 'cases' if len(test_cases) - len(diffs) >= 2 else 'case'
    print(f'\033[32mPassed {len(test_cases) - len(diffs)} {case}\033[0m') # 緑
if len(diffs) > 0:
    case = 'cases' if len(diffs) >= 2 else 'case'
    print(f'\033[31mFailed {len(diffs)} {case}\033[0m') # 赤
print('')

# 差分表示
for test_case, diff in diffs:
    print(f'{test_case}:')
    print(diff)