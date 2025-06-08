import subprocess
import tempfile
import os
import sys
import re

def strip_ansi(text):
    return re.sub(r'\x1b\[[0-9;]*m', '', text)

def run_cat_and_check(file_path, expected_lines, should_fail=False):
    result = subprocess.run([r'.\cat.exe', file_path], capture_output=True, text=True, shell=False)
    output = strip_ansi(result.stdout)
    error = strip_ansi(result.stderr)
    if should_fail:
        assert result.returncode != 0
        assert expected_lines[0] in error
    else:
        assert result.returncode == 0
        for line in expected_lines:
            assert line in output

def test_cat_windows_no_utf8():
    passed = True
    tmp_files = []
    try:
        with tempfile.NamedTemporaryFile('w+', delete=False, suffix='.txt') as tmp1:
            tmp1.write("Hello, test!\r\nLine 2\r\n")
            tmp_files.append(tmp1.name)

        run_cat_and_check(tmp_files[-1], ["Hello, test!", "Line 2"])

        with tempfile.NamedTemporaryFile('w+', delete=False, suffix='.txt') as tmp2:
            tmp2.write("")
            tmp_files.append(tmp2.name)

        run_cat_and_check(tmp_files[-1], [""])

        # For some special chars, write with default encoding (ANSI)
        with tempfile.NamedTemporaryFile('w+', delete=False, suffix='.txt') as tmp3:
            # Writing characters that are typically encodable in ANSI CP1252 on Windows
            tmp3.write("Special chars: ü, ñ, ß\r\n")
            tmp_files.append(tmp3.name)

        run_cat_and_check(tmp_files[-1], ["Special chars: ü, ñ, ß"])

        run_cat_and_check("non_existing_file.txt", ["cat: cannot open 'non_existing_file.txt'"], should_fail=True)

    except AssertionError as e:
        passed = False
        print(f"[✗] cat.exe test failed: {e}")
    except Exception as e:
        passed = False
        print(f"[!] Unexpected error during cat.exe test: {e}")
    finally:
        for f in tmp_files:
            if os.path.exists(f):
                os.remove(f)
        if passed:
            print("[✓] cat.exe passed all test cases successfully.")
        else:
            sys.exit(1)

if __name__ == "__main__":
    test_cat_windows_no_utf8()
