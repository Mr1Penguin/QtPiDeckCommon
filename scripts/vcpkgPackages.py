from vcpkg.depList import getCommonList
import sys

if __name__ == "__main__":
    res = getCommonList(sys.argv[1:])
    print(" ".join(res) if res else "nil") # there is no package with such name, right?