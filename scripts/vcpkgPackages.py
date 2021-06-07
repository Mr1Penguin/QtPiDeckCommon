import sys

#will be called from server/client script
def getCommonList(args):
    base = ["boost-test"]
    add = {"qt6.1.0": "openssl"}
    return base + [add[arg] for arg in filter(lambda x: x in add, args)]

if __name__ == "__main__":
    res = getCommonList(sys.argv[1:])
    print(" ".join(res) if res else "nil") # there is no package with such name, right?