import subprocess
import sys
import os

def test_stat():
    test_file = "stat_test_file.txt"
    with open(test_file, "w", encoding="utf-8") as f:
        f.write("test content for stat\n")

    try:
        result = subprocess.run(
            [r'.\stat.exe', test_file],
            capture_output=True,
            text=True,
            shell=False,
            encoding='utf-8',
            errors='replace'
        )
        assert result.returncode == 0
        assert "Name      :" in result.stdout
        assert test_file in result.stdout
        assert "Size      :" in result.stdout
        assert "Attr      :" in result.stdout
        assert "Created   :" in result.stdout
        assert "Modified  :" in result.stdout
        assert "Accessed  :" in result.stdout
        assert "Path      :" in result.stdout

    except AssertionError:
        print("[✗] stat.exe test failed")
        sys.exit(1)
    except Exception:
        print("[!] Unexpected error in stat.exe test")
        sys.exit(2)
    else:
        print("[✓] stat.exe test passed")
    finally:
        if os.path.exists(test_file):
            os.remove(test_file)

if __name__ == "__main__":
    test_stat()
