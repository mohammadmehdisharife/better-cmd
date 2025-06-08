import subprocess
import sys
import re

def strip_ansi_codes(text):
    ansi_escape = re.compile(r'\x1B[@-_][0-?]*[ -/]*[@-~]')
    return ansi_escape.sub('', text)

def test_pwd():
    try:
        result = subprocess.run([r'.\pwd.exe'], capture_output=True, text=True, shell=False)
        assert result.returncode == 0

        output_stripped = strip_ansi_codes(result.stdout).strip()
        import os
        expected_path = os.getcwd()

        assert output_stripped == expected_path, f"Expected path:\n{expected_path}\nGot:\n{output_stripped}"

        print("[✓] pwd.exe test passed")

    except AssertionError as e:
        print(f"[✗] Test failed: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"[!] Unexpected error: {e}")
        sys.exit(2)

if __name__ == "__main__":
    test_pwd()
