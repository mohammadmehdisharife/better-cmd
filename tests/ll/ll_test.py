import subprocess
import sys
import re

def test_ll():
    try:
        result = subprocess.run(
            [r'.\ll.exe'],
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
        ansi_pattern = re.compile(r'\x1b\[\d;?(\d+)?m')

        symbol_found = any(sym in output for sym in symbols)
        assert symbol_found
        
        assert ansi_pattern.search(output) is not None
        
        lines = output.splitlines()
        for line in lines:

            assert line.startswith('\x1b[')
            
            found_symbol = False
            for sym in symbols:
                if line.find(sym + '|') != -1:
                    found_symbol = True
                    break
            assert found_symbol
            
            line_clean = re.sub(r'\x1b\[[0-9;]*m', '', line)
            parts = line_clean.split('|')
            assert len(parts) == 2
            name_and_rest = parts[1]
            
            filename_part = name_and_rest[:20]
            assert len(filename_part) >= 20
            
            rest = name_and_rest[20:].strip()
            size_date_match = re.match(r'^([-\dMK]+) (\d{4}-\d{2}-\d{2} \d{2}:\d{2})$', rest)
            assert size_date_match is not None

    except AssertionError:
        print("[✗] ll.exe test failed")
        sys.exit(1)
    except Exception:
        print("[!] Unexpected error in ll.exe test")
        sys.exit(2)
    else:
        print("[✓] ll.exe test passed")

if __name__ == "__main__":
    test_ll()
