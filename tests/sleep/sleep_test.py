import subprocess
import time
import re
import os

def test_sleep_valid():
    start = time.time()
    result = subprocess.run(
        [r'.\sleep.exe', '1000'],
        capture_output=True,
        text=True,
        shell=False
    )
    end = time.time()
    
    elapsed = end - start
    assert result.returncode == 0, f"Expected return code 0, got {result.returncode}. Stderr: {result.stderr}"
    assert 0.95 <= elapsed <= 1.5, f"Expected ~1 second delay, got {elapsed:.2f}s"

def test_sleep_invalid():
    result = subprocess.run(
        [r'.\sleep.exe'],
        capture_output=True,
        text=True,
        shell=False
    )

    assert result.returncode == 1, f"Expected return code 1, got {result.returncode}. Stderr: {result.stderr}"
    assert "Usage:" in result.stderr, f"Expected usage message, got: {result.stderr}"
    assert re.search(r"Usage: .* <milliseconds>", result.stderr), "Regex match failed for usage message"

if __name__ == "__main__":
    assert os.path.exists(r'.\sleep.exe'), "sleep.exe not found"
    test_sleep_valid()
    test_sleep_invalid()
    print("[✓] sleep.exe test passed")
