#!/usr/bin/env python3

import sys
import subprocess


def main():
    with subprocess.Popen(["grep", "a"], stdin=subprocess.PIPE, stdout=subprocess.PIPE) as proc:
        proc.stdin.write(b"gimme banners\n")
        proc.stdin.write(b"leave now\n")
        proc.stdin.write(b"never return\n")
        sys.stdout.write("Press any key to continue ... ")
        sys.stdout.flush()
        _ = sys.stdin.readline()


if __name__ == "__main__":
    sys.exit(main())
