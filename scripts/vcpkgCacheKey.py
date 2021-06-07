from vcpkg.depList import getCommonList
import hashlib
import sys

if __name__ == "__main__":
    print(hashlib.md5("".join(getCommonList(sys.argv[1:])).encode()).hexdigest())