#!/usr/bin/env python3

import sys
import subprocess


def main():
    with subprocess.Popen(["grep", "a", "/etc/passwd"], stdout=subprocess.PIPE) as proc:
        lines = proc.stdout.readlines()
        num_lines = len(lines)
        print(f"{num_lines} with string a in /etc/passwd")
        sys.stdout.write("Press any key to continue ... ")
        sys.stdout.flush()
        _ = sys.stdin.readline()


if __name__ == "__main__":
    sys.exit(main())
