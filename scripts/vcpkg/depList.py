from itertools import chain

#will be called from server/client script
def getCommonList(args):
    base = ["boost-test", "boost-log"]
    add = {"qt6.4.0": ["openssl"]}
    return chain(base, *[add[arg] for arg in filter(lambda x: x in add, args)])
