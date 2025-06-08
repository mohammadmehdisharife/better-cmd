import subprocess
import sys
import tempfile
import os

def test_tail():
    content = "\n".join([f"Line {i}" for i in range(1, 21)]) + "\n"
    expected_default = "\n".join([f"Line {i}" for i in range(11, 21)]) + "\n"
    expected_n_5 = "\n".join([f"Line {i}" for i in range(16, 21)]) + "\n"

    with tempfile.NamedTemporaryFile('w+', delete=False, suffix='.txt') as tmp:
        tmp.write(content)
        tmp.flush()
        tmp_name = tmp.name

    try:
        result_default = subprocess.run([r'.\tail.exe', tmp_name], capture_output=True, text=True, shell=False)
        assert result_default.returncode == 0
        assert result_default.stdout == expected_default, f"Default output not as expected:\n{result_default.stdout}"

        result_n_5 = subprocess.run([r'.\tail.exe', '-n', '5', tmp_name], capture_output=True, text=True, shell=False)
        assert result_n_5.returncode == 0
        assert result_n_5.stdout == expected_n_5, f"Output with -n 5 not as expected:\n{result_n_5.stdout}"

        print("[✓] tail.exe tests passed")

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
    test_tail()
