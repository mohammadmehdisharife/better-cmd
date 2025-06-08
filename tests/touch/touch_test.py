import subprocess
import sys
import os

def test_touch():
    test_file = "test_touch_file.txt"
    if os.path.exists(test_file):
        os.remove(test_file)

    try:
        result = subprocess.run(
            [r'.\touch.exe', test_file],
            capture_output=True,
            text=True,
            shell=False,
            encoding='utf-8',        # Force utf-8 decoding
            errors='replace'         # Replace invalid chars if any
        )
        assert result.returncode == 0

        assert os.path.exists(test_file), "File was not created by touch.exe"

        print("[✓] touch.exe test passed")

    except AssertionError as e:
        print(f"[✗] Test failed: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"[!] Unexpected error: {e}")
        sys.exit(2)
    finally:
        if os.path.exists(test_file):
            os.remove(test_file)

if __name__ == "__main__":
    test_touch()
