import subprocess
import sys
import tempfile
import os

def test_head():
    content = "\n".join([f"Line {i}" for i in range(1, 16)]) + "\n"
    expected_default = "\n".join([f"Line {i}" for i in range(1, 11)]) + "\n"
    expected_n_2 = "Line 1\nLine 2\n"

    with tempfile.NamedTemporaryFile('w+', delete=False, suffix='.txt') as tmp:
        tmp.write(content)
        tmp.flush()
        tmp_name = tmp.name

    try:
        result_default = subprocess.run([r'.\head.exe', tmp_name], capture_output=True, text=True, shell=False)
        assert result_default.returncode == 0
        assert result_default.stdout == expected_default, f"Default output not as expected:\n{result_default.stdout}"

        result_n_2 = subprocess.run([r'.\head.exe', '-n', '2', tmp_name], capture_output=True, text=True, shell=False)
        assert result_n_2.returncode == 0
        assert result_n_2.stdout == expected_n_2, f"Output with -n 2 not as expected:\n{result_n_2.stdout}"

        print("[✓] head.exe tests passed")

    except AssertionError as e:
        print(f"[✗] Test failed: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"[!] Unexpected error: {e}")
        sys.exit(2)
    finally:
        if os.path.exists(tmp_name):
            os.remove(tmp_name)

if __name__ == "__main__":
    test_head()
