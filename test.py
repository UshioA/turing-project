#! /usr/bin/env python3
import subprocess
import os
from itertools import zip_longest
import sys

TESTCASE_FOLDER = "testcases"
SAMPLE_OUT_FOLDER = "outputs"
PROGRAM_SUFFIX = ".tm"
OUTPUT_SUFFIX = ".out"
TESTCASES = [
  ["4statebeaver", [""]],
  ["bin2dec", ["1", "101", "1110010"]],
  ["binaryadd", ["0+0", "101+1", "1110010+1000000010"]],
  ["binarymult", ["0x0", "101x1", "1110010x1000000010"]],
  ["concatenate", ["1100111"]],
  ["gcd", ["11101111111", "1111111110111111111111", "1111111111111111111111110111111111"]],
  ["palindrome_detector_2tapes", ["", "1001001", "111000100000111"]],
  ["parentheses", ["()()", "(()(()))()", "()((((()))())()"]],
  ["primetest", ["11", "1001", "10111"]],
  ["reversepolishboolean", ["0~", "01~1|&", "01~1|&11&|~"]],
]

COLORS = {"default": "\033[0m", "red": "\033[31m", "green": "\033[32m"}

def color(name, text):
  if os.isatty(1):
    return COLORS[name] + text + COLORS["default"]
  return text

def run(turing, file, input):
  return subprocess.run([turing, '-v', file, input], 
                        stdout=subprocess.PIPE, 
                        stderr=subprocess.PIPE, text=True, encoding="utf-8")

def test(turing, testcase, index, input):
  print("TEST", (testcase + str(index)).ljust(30), end=': ')
  tm = run(turing, os.path.join(TESTCASE_FOLDER, testcase + PROGRAM_SUFFIX), input)
  if tm.returncode != 0:
    print(color("red", "RE"), "stderr output:", tm.stderr.strip())
  else:
    step = 0
    with open(os.path.join(SAMPLE_OUT_FOLDER, testcase+str(index)+OUTPUT_SUFFIX), "r") as f:
      for i, j in zip_longest(tm.stdout.splitlines(), f.readlines()):
        if j is not None and j.startswith("Step"):
          step += 1
        if i is None or j is None or i.strip() != j.strip():
          print(color("red", "WA"), "first diff at step", step)
          return
    print(color("green", "AC"))

def main():
  if len(sys.argv) != 2:
    print("usage: python3 test.py <turing>")
    exit(1)
  turing = os.path.join('.', sys.argv[1])
  if not os.path.exists(turing):
    print(turing, ': no such file')
    exit(2)
  for testcase, inputs in TESTCASES:
    for i, input in enumerate(inputs):
      test(turing, testcase, i, input)

if __name__ == "__main__":
  main()

