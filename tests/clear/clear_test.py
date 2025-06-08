import subprocess
import tempfile
import os
import sys
import re

def strip_ansi(text):
    return re.sub(r'\x1b\[[0-9;]*m', '', text)

def test_clear_windows():
    passed = True
    try:
        result = subprocess.run([r'.\clear.exe'], capture_output=True, text=True, shell=False)

        output = strip_ansi(result.stdout)
        error = strip_ansi(result.stderr)

        assert result.returncode == 0, "clear.exe should return 0"
        assert output == "", "clear.exe should produce no stdout output"
        assert error == "", "clear.exe should produce no stderr output"

        print("[✓] clear.exe test passed successfully.")

    except AssertionError as e:
        passed = False
        print(f"[✗] clear.exe test failed: {e}")
    except Exception as e:
        passed = False
        print(f"[!] Unexpected error during clear.exe test: {e}")
    finally:
        if not passed:
            sys.exit(1)

if __name__ == "__main__":
    test_clear_windows()
