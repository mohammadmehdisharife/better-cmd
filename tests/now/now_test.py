import subprocess
import sys
import re

def test_now():
    try:
        result = subprocess.run(
            [r'.\now.exe'],
            capture_output=True,
            text=True,
            shell=False,
            encoding='utf-8',
            errors='replace'
        )
        assert result.returncode == 0
        assert result.stdout.startswith("🕰️ ")
        
        # Check date-time format YYYY-MM-DD HH:MM:SS after emoji and color code removed
        # Remove ANSI escape sequences
        import re
        ansi_escape = re.compile(r'\x1b\[[0-9;]*m')
        clean_output = ansi_escape.sub('', result.stdout).strip()

        # After emoji and a space, the rest should match datetime format
        datetime_str = clean_output[2:].strip()
        datetime_match = re.match(r'\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}', datetime_str)
        assert datetime_match is not None

    except AssertionError:
        print("[✗] now.exe test failed")
        sys.exit(1)
    except Exception:
        print("[!] Unexpected error in now.exe test")
        sys.exit(2)
    else:
        print("[✓] now.exe test passed")

if __name__ == "__main__":
    test_now()
