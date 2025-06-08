import subprocess
import re
import sys

def strip_ansi(text):
    return re.sub(r'\x1b\[[0-9;]*m', '', text)

def test_dirname():
    try:
        input_path = r"C:\Users\Example\file.txt"
        expected_output = r"C:\Users\Example"

        result = subprocess.run([r'.\dirname.exe', input_path], capture_output=True, text=True, shell=False)

        clean_stdout = strip_ansi(result.stdout).strip()

        assert result.returncode == 0, "dirname.exe should return 0"
        assert clean_stdout == expected_output, f"Expected '{expected_output}', got '{clean_stdout}'"

        print("[✓] dirname.exe test passed")

    except AssertionError as e:
        print(f"[✗] Test failed: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"[!] Unexpected error: {e}")
        sys.exit(2)

if __name__ == "__main__":
    test_dirname()
