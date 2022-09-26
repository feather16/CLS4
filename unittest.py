# ユニットテストを行う

import os
from re import sub, template
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

# テストの優先順位
with open(f'{CURRENT_DIR}/unittest/test_priority.txt', encoding='utf-8') as f:
    test_priority = [line.strip() for line in f.readlines() \
        if len(line) > 0 and not line.startswith('#')]

# テストケース名(優先順位でソート)
test_cases = [os.path.splitext(file)[0] for file in os.listdir(SRC_DIR)]
test_cases.sort(
    key=lambda s: (
        [not s.startswith(text) for text in test_priority] + [s]
    )
)

test_case_name_max_length = max(len(test_case) for test_case in test_cases)
extension_set = {os.path.splitext(file)[1] for file in os.listdir(SRC_DIR)}
assert len(extension_set) == 1, f'Error: multiple extensions {extension_set}'
extension = extension_set.pop()

diffs: list[tuple[str, str]] = []
descriptions: list[str] = []

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

    # ansファイルが存在しない場合に作成
    if not os.path.exists(ans_file_path):
        with open(ans_file_path, 'w') as f:
            pass
    
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
    descriptions.append(description)

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

# ソースコードの説明一覧を自動でファイルに出力
DESCRIPTION_HEADER = '''\
<!-- このファイルはユニットテストを実行する度に自動生成されます -->
# テストプログラムの説明
|ファイル名|内容|
|-|-|
'''
with open(f'{CURRENT_DIR}/unittest/README.md', 'w', encoding='utf-8') as f:
    f.write(DESCRIPTION_HEADER)
    for test_case, description in zip(test_cases, descriptions):
        file_name = f'{test_case}{extension}'
        f.write(f'|[{file_name}](src/{file_name})|{description}|\n')