import subprocess
import sys
import re

def test_ls():
    try:
        result = subprocess.run(
            [r'.\ls.exe'],
            capture_output=True,
            text=True,
            shell=False,
            encoding='utf-8',
            errors='replace'
        )
        assert result.returncode == 0
        
        output = result.stdout.strip()
        assert len(output) > 0
        
        symbols = ["📁", "📦", "🧰", "🔗", "📷", "🎧", "🎬", "📜", "📄"]
        symbol_found = any(sym in output for sym in symbols)
        assert symbol_found
        
        ansi_pattern = re.compile(r'\x1b\[\d;?(\d+)?m')
        assert ansi_pattern.search(output) is not None
        
        lines = output.splitlines()
        for line in lines:
            assert any(line.startswith(sym + "|") for sym in symbols)

    except AssertionError:
        print("[✗] ls.exe test failed")
        sys.exit(1)
    except Exception:
        print("[!] Unexpected error in ls.exe test")
        sys.exit(2)
    else:
        print("[✓] ls.exe test passed")

if __name__ == "__main__":
    test_ls()
