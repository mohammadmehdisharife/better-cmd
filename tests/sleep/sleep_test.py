import subprocess
import time
import re

def test_sleep_valid():
    start = time.time()
    result = subprocess.run(
        [r'.\sleep.exe', '1000'],
        capture_output=True,
        text=True,
        shell=False
    )
    end = time.time()
    
    # بررسی زمان اجرا
    elapsed = end - start
    assert result.returncode == 0
    assert elapsed >= 1.0, f"Expected at least 1 second delay, got {elapsed:.2f}s"

    print("[✓] test_sleep_valid passed")

def test_sleep_invalid():
    result = subprocess.run(
        [r'.\sleep.exe'],
        capture_output=True,
        text=True,
        shell=False
    )

    assert result.returncode == 1
    assert "Usage:" in result.stderr
    assert re.search(r"Usage: .* <milliseconds>", result.stderr)

    print("[✓] test_sleep_invalid passed")

if __name__ == "__main__":
    test_sleep_valid()
    test_sleep_invalid()
